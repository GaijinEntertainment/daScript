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
  YYSYMBOL_BEGIN_STRING = 163,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 164,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 165,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 166,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 167,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 168,          /* "}"  */
  YYSYMBOL_END_OF_READ = 169,              /* "end of failed eader macro"  */
  YYSYMBOL_170_begin_of_code_block_ = 170, /* "begin of code block"  */
  YYSYMBOL_171_end_of_code_block_ = 171,   /* "end of code block"  */
  YYSYMBOL_172_end_of_expression_ = 172,   /* "end of expression"  */
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
  YYSYMBOL_204_ = 204,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 205,                 /* $accept  */
  YYSYMBOL_program = 206,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 207,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 208, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 209,              /* module_name  */
  YYSYMBOL_module_declaration = 210,       /* module_declaration  */
  YYSYMBOL_character_sequence = 211,       /* character_sequence  */
  YYSYMBOL_string_constant = 212,          /* string_constant  */
  YYSYMBOL_string_builder_body = 213,      /* string_builder_body  */
  YYSYMBOL_string_builder = 214,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 215, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 216,              /* expr_reader  */
  YYSYMBOL_217_1 = 217,                    /* $@1  */
  YYSYMBOL_options_declaration = 218,      /* options_declaration  */
  YYSYMBOL_require_declaration = 219,      /* require_declaration  */
  YYSYMBOL_require_module_name = 220,      /* require_module_name  */
  YYSYMBOL_require_module = 221,           /* require_module  */
  YYSYMBOL_is_public_module = 222,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 223,       /* expect_declaration  */
  YYSYMBOL_expect_list = 224,              /* expect_list  */
  YYSYMBOL_expect_error = 225,             /* expect_error  */
  YYSYMBOL_expression_label = 226,         /* expression_label  */
  YYSYMBOL_expression_goto = 227,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 228,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 229,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 230,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 231, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 232,  /* expression_if_one_liner  */
  YYSYMBOL_optional_semi_list = 233,       /* optional_semi_list  */
  YYSYMBOL_optional_semis = 234,           /* optional_semis  */
  YYSYMBOL_expression_if_then_else = 235,  /* expression_if_then_else  */
  YYSYMBOL_236_2 = 236,                    /* $@2  */
  YYSYMBOL_expression_if_then_else_oneliner = 237, /* expression_if_then_else_oneliner  */
  YYSYMBOL_expression_for_loop = 238,      /* expression_for_loop  */
  YYSYMBOL_239_3 = 239,                    /* $@3  */
  YYSYMBOL_expression_unsafe = 240,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 241,    /* expression_while_loop  */
  YYSYMBOL_242_4 = 242,                    /* $@4  */
  YYSYMBOL_expression_with = 243,          /* expression_with  */
  YYSYMBOL_244_5 = 244,                    /* $@5  */
  YYSYMBOL_expression_with_alias = 245,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 246, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 247, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 248, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 249,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 250, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 251,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 252, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 253, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 254,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 255,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 256, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 257, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 258,   /* optional_function_type  */
  YYSYMBOL_function_name = 259,            /* function_name  */
  YYSYMBOL_global_function_declaration = 260, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 261, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 262, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 263,     /* function_declaration  */
  YYSYMBOL_264_6 = 264,                    /* $@6  */
  YYSYMBOL_expression_block_finally = 265, /* expression_block_finally  */
  YYSYMBOL_266_7 = 266,                    /* $@7  */
  YYSYMBOL_267_8 = 267,                    /* $@8  */
  YYSYMBOL_expression_block = 268,         /* expression_block  */
  YYSYMBOL_269_9 = 269,                    /* $@9  */
  YYSYMBOL_270_10 = 270,                   /* $@10  */
  YYSYMBOL_expr_call_pipe = 271,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 272,           /* expression_any  */
  YYSYMBOL_expressions = 273,              /* expressions  */
  YYSYMBOL_optional_expr_list = 274,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_map_tuple_list = 275, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 276, /* type_declaration_no_options_list  */
  YYSYMBOL_name_in_namespace = 277,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 278,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 279,     /* new_type_declaration  */
  YYSYMBOL_280_11 = 280,                   /* $@11  */
  YYSYMBOL_281_12 = 281,                   /* $@12  */
  YYSYMBOL_expr_new = 282,                 /* expr_new  */
  YYSYMBOL_expression_break = 283,         /* expression_break  */
  YYSYMBOL_expression_continue = 284,      /* expression_continue  */
  YYSYMBOL_expression_return = 285,        /* expression_return  */
  YYSYMBOL_expression_yield = 286,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 287,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 288,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 289,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 290,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 291,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 292, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 293,           /* expression_let  */
  YYSYMBOL_expr_cast = 294,                /* expr_cast  */
  YYSYMBOL_295_13 = 295,                   /* $@13  */
  YYSYMBOL_296_14 = 296,                   /* $@14  */
  YYSYMBOL_297_15 = 297,                   /* $@15  */
  YYSYMBOL_298_16 = 298,                   /* $@16  */
  YYSYMBOL_299_17 = 299,                   /* $@17  */
  YYSYMBOL_300_18 = 300,                   /* $@18  */
  YYSYMBOL_expr_type_decl = 301,           /* expr_type_decl  */
  YYSYMBOL_302_19 = 302,                   /* $@19  */
  YYSYMBOL_303_20 = 303,                   /* $@20  */
  YYSYMBOL_expr_type_info = 304,           /* expr_type_info  */
  YYSYMBOL_expr_list = 305,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 306,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 307,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 308,            /* capture_entry  */
  YYSYMBOL_capture_list = 309,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 310,    /* optional_capture_list  */
  YYSYMBOL_expr_full_block = 311,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 312, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 313,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 314,              /* expr_assign  */
  YYSYMBOL_expr_named_call = 315,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 316,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 317,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 318,           /* func_addr_expr  */
  YYSYMBOL_319_21 = 319,                   /* $@21  */
  YYSYMBOL_320_22 = 320,                   /* $@22  */
  YYSYMBOL_321_23 = 321,                   /* $@23  */
  YYSYMBOL_322_24 = 322,                   /* $@24  */
  YYSYMBOL_expr_field = 323,               /* expr_field  */
  YYSYMBOL_324_25 = 324,                   /* $@25  */
  YYSYMBOL_325_26 = 325,                   /* $@26  */
  YYSYMBOL_expr_call = 326,                /* expr_call  */
  YYSYMBOL_expr = 327,                     /* expr  */
  YYSYMBOL_328_27 = 328,                   /* $@27  */
  YYSYMBOL_329_28 = 329,                   /* $@28  */
  YYSYMBOL_330_29 = 330,                   /* $@29  */
  YYSYMBOL_331_30 = 331,                   /* $@30  */
  YYSYMBOL_332_31 = 332,                   /* $@31  */
  YYSYMBOL_333_32 = 333,                   /* $@32  */
  YYSYMBOL_expr_generator = 334,           /* expr_generator  */
  YYSYMBOL_expr_mtag = 335,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 336, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 337,        /* optional_override  */
  YYSYMBOL_optional_constant = 338,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 339, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 340, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 341, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 342, /* struct_variable_declaration_list  */
  YYSYMBOL_343_33 = 343,                   /* $@33  */
  YYSYMBOL_344_34 = 344,                   /* $@34  */
  YYSYMBOL_345_35 = 345,                   /* $@35  */
  YYSYMBOL_function_argument_declaration = 346, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 347,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 348,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 349,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 350,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 351,             /* variant_type  */
  YYSYMBOL_variant_type_list = 352,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 353,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 354,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 355,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 356,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 357,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 358, /* let_variable_name_with_pos_list  */
  YYSYMBOL_global_let_variable_name_with_pos_list = 359, /* global_let_variable_name_with_pos_list  */
  YYSYMBOL_variable_declaration_list = 360, /* variable_declaration_list  */
  YYSYMBOL_let_variable_declaration = 361, /* let_variable_declaration  */
  YYSYMBOL_global_let_variable_declaration = 362, /* global_let_variable_declaration  */
  YYSYMBOL_optional_shared = 363,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 364, /* optional_public_or_private_variable  */
  YYSYMBOL_global_variable_declaration_list = 365, /* global_variable_declaration_list  */
  YYSYMBOL_366_36 = 366,                   /* $@36  */
  YYSYMBOL_global_let = 367,               /* global_let  */
  YYSYMBOL_368_37 = 368,                   /* $@37  */
  YYSYMBOL_enum_expression = 369,          /* enum_expression  */
  YYSYMBOL_commas = 370,                   /* commas  */
  YYSYMBOL_enum_list = 371,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 372, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 373,             /* single_alias  */
  YYSYMBOL_374_38 = 374,                   /* $@38  */
  YYSYMBOL_alias_declaration = 375,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 376, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 377,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 378, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_optional_commas = 379,          /* optional_commas  */
  YYSYMBOL_enum_declaration = 380,         /* enum_declaration  */
  YYSYMBOL_381_39 = 381,                   /* $@39  */
  YYSYMBOL_382_40 = 382,                   /* $@40  */
  YYSYMBOL_383_41 = 383,                   /* $@41  */
  YYSYMBOL_optional_structure_parent = 384, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 385,          /* optional_sealed  */
  YYSYMBOL_structure_name = 386,           /* structure_name  */
  YYSYMBOL_class_or_struct = 387,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 388, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 389, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 390,    /* structure_declaration  */
  YYSYMBOL_391_42 = 391,                   /* $@42  */
  YYSYMBOL_392_43 = 392,                   /* $@43  */
  YYSYMBOL_variable_name_with_pos_list = 393, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 394,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 395, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 396, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 397,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 398,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 399,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 400, /* bitfield_type_declaration  */
  YYSYMBOL_401_44 = 401,                   /* $@44  */
  YYSYMBOL_402_45 = 402,                   /* $@45  */
  YYSYMBOL_c_or_s = 403,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 404,          /* table_type_pair  */
  YYSYMBOL_dim_list = 405,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 406, /* type_declaration_no_options  */
  YYSYMBOL_type_declaration_no_options_no_dim = 407, /* type_declaration_no_options_no_dim  */
  YYSYMBOL_408_46 = 408,                   /* $@46  */
  YYSYMBOL_409_47 = 409,                   /* $@47  */
  YYSYMBOL_410_48 = 410,                   /* $@48  */
  YYSYMBOL_411_49 = 411,                   /* $@49  */
  YYSYMBOL_412_50 = 412,                   /* $@50  */
  YYSYMBOL_413_51 = 413,                   /* $@51  */
  YYSYMBOL_414_52 = 414,                   /* $@52  */
  YYSYMBOL_415_53 = 415,                   /* $@53  */
  YYSYMBOL_416_54 = 416,                   /* $@54  */
  YYSYMBOL_417_55 = 417,                   /* $@55  */
  YYSYMBOL_418_56 = 418,                   /* $@56  */
  YYSYMBOL_419_57 = 419,                   /* $@57  */
  YYSYMBOL_420_58 = 420,                   /* $@58  */
  YYSYMBOL_421_59 = 421,                   /* $@59  */
  YYSYMBOL_422_60 = 422,                   /* $@60  */
  YYSYMBOL_423_61 = 423,                   /* $@61  */
  YYSYMBOL_424_62 = 424,                   /* $@62  */
  YYSYMBOL_425_63 = 425,                   /* $@63  */
  YYSYMBOL_426_64 = 426,                   /* $@64  */
  YYSYMBOL_427_65 = 427,                   /* $@65  */
  YYSYMBOL_428_66 = 428,                   /* $@66  */
  YYSYMBOL_429_67 = 429,                   /* $@67  */
  YYSYMBOL_430_68 = 430,                   /* $@68  */
  YYSYMBOL_431_69 = 431,                   /* $@69  */
  YYSYMBOL_432_70 = 432,                   /* $@70  */
  YYSYMBOL_433_71 = 433,                   /* $@71  */
  YYSYMBOL_434_72 = 434,                   /* $@72  */
  YYSYMBOL_type_declaration = 435,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 436,  /* tuple_alias_declaration  */
  YYSYMBOL_437_73 = 437,                   /* $@73  */
  YYSYMBOL_438_74 = 438,                   /* $@74  */
  YYSYMBOL_439_75 = 439,                   /* $@75  */
  YYSYMBOL_variant_alias_declaration = 440, /* variant_alias_declaration  */
  YYSYMBOL_441_76 = 441,                   /* $@76  */
  YYSYMBOL_442_77 = 442,                   /* $@77  */
  YYSYMBOL_443_78 = 443,                   /* $@78  */
  YYSYMBOL_bitfield_alias_declaration = 444, /* bitfield_alias_declaration  */
  YYSYMBOL_445_79 = 445,                   /* $@79  */
  YYSYMBOL_446_80 = 446,                   /* $@80  */
  YYSYMBOL_447_81 = 447,                   /* $@81  */
  YYSYMBOL_make_decl = 448,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 449,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 450,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 451,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim_list = 452,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 453,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 454, /* optional_make_struct_dim_decl  */
  YYSYMBOL_use_initializer = 455,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 456,         /* make_struct_decl  */
  YYSYMBOL_457_82 = 457,                   /* $@82  */
  YYSYMBOL_458_83 = 458,                   /* $@83  */
  YYSYMBOL_459_84 = 459,                   /* $@84  */
  YYSYMBOL_460_85 = 460,                   /* $@85  */
  YYSYMBOL_461_86 = 461,                   /* $@86  */
  YYSYMBOL_462_87 = 462,                   /* $@87  */
  YYSYMBOL_463_88 = 463,                   /* $@88  */
  YYSYMBOL_464_89 = 464,                   /* $@89  */
  YYSYMBOL_make_map_tuple = 465,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 466,          /* make_tuple_call  */
  YYSYMBOL_467_90 = 467,                   /* $@90  */
  YYSYMBOL_468_91 = 468,                   /* $@91  */
  YYSYMBOL_make_dim_decl = 469,            /* make_dim_decl  */
  YYSYMBOL_470_92 = 470,                   /* $@92  */
  YYSYMBOL_471_93 = 471,                   /* $@93  */
  YYSYMBOL_472_94 = 472,                   /* $@94  */
  YYSYMBOL_473_95 = 473,                   /* $@95  */
  YYSYMBOL_474_96 = 474,                   /* $@96  */
  YYSYMBOL_475_97 = 475,                   /* $@97  */
  YYSYMBOL_476_98 = 476,                   /* $@98  */
  YYSYMBOL_477_99 = 477,                   /* $@99  */
  YYSYMBOL_478_100 = 478,                  /* $@100  */
  YYSYMBOL_479_101 = 479,                  /* $@101  */
  YYSYMBOL_expr_map_tuple_list = 480,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 481,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 482, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 483,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 484       /* array_comprehension  */
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
#define YYLAST   10222

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  205
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  280
/* YYNRULES -- Number of rules.  */
#define YYNRULES  813
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1484

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   432


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
       2,     2,     2,   190,     2,   204,   202,   186,   179,     2,
     200,   201,   184,   183,   173,   182,   197,   185,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   176,   172,
     180,   174,   181,   175,   203,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   198,     2,   199,   178,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   170,   177,   171,   189,     2,     2,     2,
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
     165,   166,   167,   168,   169,   187,   188,   191,   192,   193,
     194,   195,   196
};

#if DAS2_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   531,   531,   532,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   553,   559,   560,
     561,   565,   566,   570,   588,   589,   590,   591,   595,   596,
     600,   605,   614,   622,   638,   643,   651,   651,   690,   708,
     712,   715,   719,   725,   734,   737,   743,   744,   748,   752,
     753,   757,   760,   766,   772,   775,   781,   782,   786,   787,
     788,   797,   798,   802,   803,   809,   810,   811,   812,   813,
     817,   818,   822,   823,   827,   827,   839,   846,   846,   855,
     863,   863,   876,   876,   888,   895,   896,   897,   898,   899,
     900,   904,   909,   917,   918,   919,   923,   924,   925,   926,
     927,   928,   929,   930,   936,   939,   945,   948,   954,   955,
     956,   960,   973,   991,   994,  1002,  1013,  1024,  1035,  1038,
    1045,  1049,  1056,  1057,  1061,  1062,  1063,  1067,  1070,  1077,
    1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,
    1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,
    1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,
    1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,  1120,
    1121,  1122,  1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,
    1131,  1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,
    1141,  1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,
    1151,  1152,  1153,  1154,  1155,  1156,  1157,  1158,  1159,  1160,
    1161,  1162,  1163,  1164,  1169,  1187,  1188,  1189,  1193,  1199,
    1199,  1216,  1219,  1222,  1219,  1231,  1234,  1231,  1250,  1263,
    1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,
    1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1285,  1290,
    1296,  1302,  1303,  1307,  1308,  1312,  1316,  1323,  1324,  1335,
    1339,  1342,  1350,  1350,  1350,  1353,  1359,  1362,  1366,  1370,
    1377,  1384,  1390,  1394,  1398,  1401,  1404,  1412,  1415,  1423,
    1429,  1430,  1431,  1435,  1436,  1440,  1441,  1445,  1450,  1458,
    1464,  1476,  1479,  1482,  1488,  1488,  1488,  1491,  1491,  1491,
    1496,  1496,  1496,  1504,  1504,  1504,  1510,  1520,  1531,  1546,
    1549,  1555,  1556,  1563,  1574,  1575,  1576,  1580,  1581,  1582,
    1583,  1584,  1588,  1593,  1601,  1602,  1606,  1613,  1617,  1623,
    1624,  1625,  1626,  1627,  1628,  1629,  1633,  1634,  1635,  1636,
    1637,  1638,  1639,  1640,  1641,  1642,  1643,  1644,  1645,  1646,
    1647,  1648,  1649,  1650,  1651,  1655,  1662,  1674,  1679,  1689,
    1693,  1700,  1703,  1703,  1703,  1708,  1708,  1708,  1721,  1725,
    1729,  1734,  1741,  1746,  1753,  1753,  1753,  1760,  1764,  1774,
    1783,  1792,  1796,  1799,  1805,  1806,  1807,  1808,  1809,  1810,
    1811,  1812,  1813,  1814,  1815,  1816,  1817,  1818,  1819,  1820,
    1821,  1822,  1823,  1824,  1825,  1826,  1827,  1828,  1829,  1830,
    1831,  1832,  1833,  1834,  1835,  1836,  1837,  1838,  1839,  1840,
    1846,  1847,  1848,  1849,  1850,  1863,  1864,  1865,  1866,  1867,
    1868,  1869,  1870,  1871,  1872,  1873,  1874,  1875,  1876,  1879,
    1879,  1879,  1882,  1887,  1891,  1895,  1895,  1895,  1900,  1903,
    1907,  1907,  1907,  1912,  1915,  1916,  1917,  1918,  1919,  1920,
    1921,  1922,  1923,  1925,  1929,  1930,  1938,  1941,  1944,  1953,
    1954,  1955,  1956,  1957,  1958,  1959,  1963,  1967,  1971,  1975,
    1979,  1983,  1987,  1991,  1995,  2002,  2003,  2007,  2008,  2009,
    2013,  2014,  2018,  2019,  2020,  2024,  2025,  2029,  2040,  2043,
    2043,  2062,  2061,  2075,  2074,  2090,  2099,  2109,  2110,  2114,
    2117,  2126,  2127,  2131,  2134,  2137,  2153,  2162,  2163,  2167,
    2170,  2173,  2187,  2188,  2192,  2198,  2204,  2207,  2211,  2220,
    2221,  2222,  2226,  2227,  2231,  2238,  2243,  2252,  2258,  2269,
    2276,  2285,  2288,  2291,  2298,  2301,  2306,  2317,  2320,  2325,
    2337,  2338,  2342,  2343,  2344,  2348,  2351,  2354,  2354,  2374,
    2377,  2377,  2395,  2400,  2408,  2409,  2413,  2416,  2429,  2446,
    2447,  2448,  2453,  2453,  2479,  2483,  2484,  2485,  2489,  2499,
    2502,  2508,  2509,  2513,  2516,  2521,  2513,  2538,  2539,  2543,
    2544,  2548,  2554,  2555,  2559,  2560,  2561,  2565,  2568,  2574,
    2579,  2574,  2593,  2600,  2605,  2614,  2620,  2631,  2632,  2633,
    2634,  2635,  2636,  2637,  2638,  2639,  2640,  2641,  2642,  2643,
    2644,  2645,  2646,  2647,  2648,  2649,  2650,  2651,  2652,  2653,
    2654,  2655,  2656,  2657,  2661,  2662,  2663,  2664,  2665,  2666,
    2667,  2668,  2672,  2683,  2687,  2694,  2706,  2713,  2722,  2727,
    2737,  2750,  2750,  2750,  2763,  2764,  2768,  2772,  2779,  2783,
    2787,  2791,  2798,  2801,  2819,  2820,  2821,  2822,  2823,  2823,
    2823,  2827,  2832,  2839,  2839,  2846,  2850,  2854,  2859,  2864,
    2869,  2874,  2878,  2882,  2887,  2891,  2895,  2900,  2900,  2900,
    2906,  2913,  2913,  2913,  2918,  2918,  2918,  2924,  2924,  2924,
    2929,  2933,  2933,  2933,  2938,  2938,  2938,  2947,  2951,  2951,
    2951,  2956,  2956,  2956,  2965,  2969,  2969,  2969,  2974,  2974,
    2974,  2983,  2983,  2983,  2989,  2989,  2989,  2998,  3001,  3012,
    3028,  3033,  3038,  3028,  3063,  3068,  3074,  3063,  3099,  3104,
    3109,  3099,  3139,  3140,  3141,  3142,  3143,  3147,  3154,  3161,
    3167,  3173,  3180,  3187,  3193,  3202,  3208,  3216,  3221,  3228,
    3233,  3239,  3240,  3244,  3245,  3249,  3249,  3249,  3257,  3257,
    3257,  3264,  3264,  3264,  3271,  3271,  3271,  3282,  3288,  3294,
    3300,  3300,  3300,  3310,  3319,  3319,  3319,  3329,  3329,  3329,
    3339,  3339,  3339,  3349,  3358,  3358,  3358,  3378,  3385,  3385,
    3385,  3395,  3398,  3404,  3412,  3420,  3440,  3465,  3466,  3470,
    3471,  3475,  3478,  3481
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
  "\"double constant\"", "\"name\"", "\"start of the string\"",
  "STRING_CHARACTER", "STRING_CHARACTER_ESC", "\"end of the string\"",
  "\"{\"", "\"}\"", "\"end of failed eader macro\"",
  "\"begin of code block\"", "\"end of code block\"",
  "\"end of expression\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'",
  "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS",
  "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC", "LLPIPE", "POST_INC",
  "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('", "')'", "'$'", "'@'",
  "'#'", "$accept", "program", "top_level_reader_macro",
  "optional_public_or_private_module", "module_name", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "reader_character_sequence", "expr_reader", "$@1",
  "options_declaration", "require_declaration", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "expression_else", "if_or_static_if",
  "expression_else_one_liner", "expression_if_one_liner",
  "optional_semi_list", "optional_semis", "expression_if_then_else", "$@2",
  "expression_if_then_else_oneliner", "expression_for_loop", "$@3",
  "expression_unsafe", "expression_while_loop", "$@4", "expression_with",
  "$@5", "expression_with_alias", "annotation_argument_value",
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
  "optional_enum_basic_type_declaration", "optional_commas",
  "enum_declaration", "$@39", "$@40", "$@41", "optional_structure_parent",
  "optional_sealed", "structure_name", "class_or_struct",
  "optional_public_or_private_structure",
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
  "tuple_alias_declaration", "$@73", "$@74", "$@75",
  "variant_alias_declaration", "$@76", "$@77", "$@78",
  "bitfield_alias_declaration", "$@79", "$@80", "$@81", "make_decl",
  "make_struct_fields", "make_variant_dim", "make_struct_single",
  "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "use_initializer", "make_struct_decl",
  "$@82", "$@83", "$@84", "$@85", "$@86", "$@87", "$@88", "$@89",
  "make_map_tuple", "make_tuple_call", "$@90", "$@91", "make_dim_decl",
  "$@92", "$@93", "$@94", "$@95", "$@96", "$@97", "$@98", "$@99", "$@100",
  "$@101", "expr_map_tuple_list", "make_table_decl",
  "array_comprehension_where", "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1292)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-719)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1292,    30, -1292, -1292,    55,   -98,   -23,   228, -1292,   -91,
     228,   228,   228, -1292,   304,   293, -1292, -1292,   -94,   -42,
     -26, -1292,   431, -1292,    89, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292,    -5, -1292,    91,    27,   128,
   -1292, -1292,   -23,    57, -1292, -1292, -1292,   179,   172, -1292,
   -1292,    89,   200,   243,   264,   320,   277, -1292, -1292, -1292,
     293,   293,   293,   295, -1292,   528,   -29, -1292, -1292, -1292,
   -1292, -1292, -1292,   455, -1292,   488, -1292,   527,    77,    55,
     298,   -98,   271,   346, -1292,   349,   426, -1292, -1292, -1292,
     535, -1292, -1292, -1292, -1292,   429,   439, -1292, -1292,   -43,
      55,   293,   293,   293,   293, -1292, -1292, -1292, -1292, -1292,
     565, -1292, -1292, -1292, -1292, -1292,   438, -1292, -1292, -1292,
   -1292, -1292,   512,   110, -1292, -1292, -1292, -1292,   581, -1292,
   -1292,   478, -1292, -1292, -1292,   495,   509,   519, -1292, -1292,
     529, -1292,  -120, -1292,   311,   553,   528, 10060, -1292, -1292,
     532,   589, -1292,   493, -1292, -1292, -1292,   517, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292,   -21, -1292,  5939, -1292, -1292,
   -1292, -1292, -1292,  8625, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,   662,
     663, -1292,   499, -1292, -1292,   524, -1292,   540, -1292,   204,
      55,   500,   543, -1292, -1292, -1292,   110, -1292,   526,   530,
     533,   508,   534,   537, -1292, -1292, -1292,   518, -1292, -1292,
   -1292, -1292, -1292,   539, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292,   542, -1292, -1292, -1292,   548,
     549, -1292, -1292, -1292, -1292,   550,   552,   520,   304, -1292,
   -1292, -1292, -1292, -1292, -1292,    52,   556,   545, -1292, -1292,
     547,   572, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292,   575,   513, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292,   716, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,   580,
     554, -1292, -1292,   -84,   568, -1292,   576,   442,   584,   579,
     586, -1292, -1292,   493, -1292,    55, -1292,   453, -1292, -1292,
   -1292, -1292, -1292,  5633, -1292, -1292,   595, -1292,   207,   323,
     333, -1292, -1292,  5633,     4, -1292, -1292, -1292,     3, -1292,
   -1292, -1292,    51,  3173, -1292,   560,  1357, -1292,   588,  1539,
     388, -1292, -1292, -1292, -1292,   597,   630, -1292,   563, -1292,
      65, -1292,   -99,  5939, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292,   304, -1292, -1292, -1292,
     104, -1292,   602,  5939, -1292,   164,  5939,  5939,  5939,   585,
     587, -1292, -1292,    38,   304,   590,    19, -1292,   178,   566,
     592,   593,   569,   594,   577,   322,   596, -1292,   344,   598,
     601,  5633,  5633,   582,   583,   603,   607,   608,   609, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,  3364,  5633,
    5633,  5633,  5633,  5633,  2853,  5633, -1292,   591, -1292, -1292,
   -1292,   612, -1292, -1292, -1292, -1292,   570, -1292, -1292, -1292,
   -1292, -1292, -1292,   116,  6027, -1292, -1292,   613, -1292, -1292,
   -1292, -1292, -1292, -1292,  5939,  5939,   614,   622,  5939,   499,
    5939,   499,  5939,   499,  6031,   624,  6126, -1292,  5633, -1292,
   -1292, -1292, -1292,   615, -1292, -1292,  8093,  3524, -1292, -1292,
     625, -1292,    12, -1292, -1292,   448, -1292,   556,   604,   616,
     448, -1292,   618, -1292, -1292,  5633, -1292, -1292,   150,   -68,
   -1292,   556,  1784,   631, -1292,    15,   617,   752,   454, -1292,
   -1292,   253, -1292, -1292, -1292,  5633,    44,   375,   621,   462,
   -1292, -1292, -1292,   619, -1292, -1292,   354, -1292,   636,   638,
     640, -1292,  5633,  5939,  5633,  5633, -1292, -1292,  5633, -1292,
    5633, -1292,  5633, -1292, -1292,  5633, -1292,  5939,   329,   329,
    5633,  5633,  5633,  5633,  5633,  5633,   507,   623,  8656, -1292,
     648,   329,   329,   -70,   329,   329,   626,   814,   655,  9444,
     655,   137,  2533,   825, -1292,   627,   570, -1292,  1246,  9853,
    5633,  5633, -1292, -1292,  5633,  5633,  5633,  5633,   669,  5633,
     393,  5633,  5633,  5633,  5633,  5633,  5633,  5633,  5633,  5633,
    3713,  5633,  5633,  5633,  5633,  5633,  5633,  5633,  5633,  5633,
    5633,  9972,  5633, -1292,  3873,   395,   459, -1292, -1292,   390,
     464,   568,   465,   568,   467,   568, -1292,   386, -1292,   392,
   -1292,  5939,   632,   655, -1292, -1292, -1292,  8178, -1292,   661,
    5939, -1292, -1292,  5939, -1292, -1292,  6157,   635,   793, -1292,
     134, -1292, -1292,  4033, -1292, -1292,  4193,   676, -1292,   668,
     686,  4353,    87,  4513, -1292,   670,   672,   835, -1292,   651,
   -1292, -1292,   833, -1292, -1292,   838, -1292,   806,   677,   680,
   -1292,   681,   682,   684,   687,   688, -1292,   786, -1292,   689,
    8509,   690, -1292,   685, -1292,   854,   493,  5633,   701,   703,
    5939,   164, -1292,  5633,  8741, -1292, -1292, -1292,  5939,  5939,
    5939,  5939,  3013,   704,  5633,  5939, -1292, -1292, -1292,  5939,
     655,   396,   648,  6247,  5939,  5939,  6278,  5939,  6368,  5939,
     655,  5939,  5939,   655,  5939,   691,  6399,  6489,  6520,  6610,
    6641,  6731, -1292,  5633,   523,   150,  5633,  5633,   697,   150,
     675,  5633,   671,   673,   678,   683,   339, -1292, -1292,  -101,
     692,    33,  2693, -1292,   115,   708,   693,   695,   499,  1984,
   -1292,   825,   696,   698, -1292, -1292,   702,   699, -1292, -1292,
     236,   236,  1285,  1285,  1371,  1371,   705,   175,   706, -1292,
    8209,   -74,   -74,   613,   236,   236,  9673,  9559,  9589,  9475,
    9941,  8825,  9703,  9787,  1056,  1285,  1285,   151,   151,   175,
     175,   175,   456,  5633,   707,   709,   457,  5633,   876,   710,
    8294, -1292,   141, -1292, -1292,   722, -1292, -1292,   715, -1292,
     720, -1292,   721, -1292,  6031, -1292,   624,   402,   556, -1292,
   -1292, -1292, -1292,   556,   556, -1292,  5633,   723, -1292,   724,
   -1292,  5939, -1292,  5633,  5633,  9444,   873,  5633,  9444,   714,
   -1292,   727,   735,  9444, -1292,   719, -1292,  5633,  9444, -1292,
   -1292,   711, -1292, -1292,   717,   725,   726,   734, -1292,   860,
   -1292, -1292, -1292, -1292, -1292, -1292,   -71, -1292,  5633,  5633,
    5633,  5633,  5633,  5633,  5633,  5633,  5633,  5633,  5633,  5633,
    5633,  5633,  5633,  5633,  5633,  5633,  5633, -1292,    60, -1292,
     574,   837,   736,  6762, -1292,   871,   258,  5633,  8909, -1292,
   -1292, -1292, -1292,   556,   738,   739,   474,   461,   158,   737,
     740,   434,  6852,   476,  5939,  5939,  5939,   741,   742,   744,
    5939,   745, -1292,   743,   754, -1292,   755, -1292,   756,   746,
     758,   759,   748,   761,   825, -1292, -1292, -1292, -1292, -1292,
     753,  8993,     8,  9444, -1292, -1292,    14,   150,  9444, -1292,
   -1292,  5633,  5633,  5939,   499,   183, -1292,   760,  5633,  5633,
    5633,    72,  5793, -1292,   463, -1292,   -31,   568, -1292,   499,
   -1292,  5633, -1292,  5633,  4673,  5633, -1292,   765,   757, -1292,
   -1292,  5633,   766, -1292,  8325,  5633,  4833,   768, -1292,  8410,
   -1292,  4993, -1292,  5633, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,   791,
    5939,  6883, -1292,   878,    16,  9444,  9444, -1292,  9444,  5633,
   -1292, -1292, -1292, -1292,  9444,  5633,  5633,   150,  5633,  5633,
   -1292, -1292, -1292,   792, -1292, -1292,  9444,  9444,  9444,  9444,
    9444,  9444,  9444,  9444,  9444,  9444,  9444,  9444,  9444,  9444,
    9444,  9444,  9444,  9444,  9444, -1292, -1292,   789, -1292, -1292,
     490, -1292,    -1, -1292, -1292,   832, -1292,  5633,  9024, -1292,
   -1292, -1292, -1292,   794, -1292, -1292, -1292,   796,   836, -1292,
   -1292,   816,   818,   819, -1292, -1292,  5633,   820, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,   198,  5153,
   -1292,  5633,  5633,    18,  6973,  7004,   821,   568, -1292,  7094,
    9444,  9444,   802,  2693,   805,   197,   844,   845,   808,   847,
     848, -1292,   237,   751,   568,  5939,  7125,  5939,  7215, -1292,
     239,  7246, -1292,  5633,   971,  5633, -1292,  7336, -1292,   250,
    5633, -1292, -1292, -1292,   255, -1292, -1292, -1292,  5633,   556,
   -1292,   849,  5633, -1292,  9444,  7367,  7457,    21,  7488,  7578,
     842,   326, -1292,   268, -1292,   980,    -1, -1292, -1292,   574,
   -1292,  9108, -1292,   822,   823, -1292, -1292,  5633,   839, -1292,
   -1292, -1292, -1292,   824,   813,   648,   826,  5633,  5633,  5633,
     827,   946,   828,   829,  5313, -1292, -1292,   288,   289,   291,
    5633, -1292, -1292, -1292,   840,   242, -1292,   122,  5633,  5633,
    5633, -1292, -1292,   856, -1292, -1292,   -31, -1292,   882,   751,
     852, -1292,   477, -1292, -1292, -1292,  5939,  7609,  7699, -1292,
   -1292,  7730, -1292,   834, -1292,  9444,  1013,   751,  5633,   751,
     751, -1292, -1292, -1292, -1292,   868,    50, -1292, -1292,   980,
     150, -1292,   946,   946,  7820,   843,   850,   851,   853,  5633,
   -1292, -1292,  5633,  1285,  1285,  1285,  5633, -1292, -1292,   946,
     461, -1292,  7851, -1292, -1292,   864,   865,   299,   391, -1292,
    5633,  5633,   841,  7941,  9444,  9444,   846, -1292,  5473, -1292,
   -1292, -1292, -1292, -1292,   483, -1292, -1292, -1292, -1292,  2373,
     866, -1292,   302, -1292, -1292,  2184, -1292,  5939,   164, 10060,
   -1292, -1292,   -15,   -15, -1292,  5633,   946,   946,   461,   855,
     857,   655,   -15,   708,   858, -1292,  5633,  5633,   872, -1292,
     391,  9444,  9444, -1292,   303, -1292,  5633,  9475, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292,  9444, -1292,   482,
     751, -1292, -1292,   869,   164,  5633,   877, 10060,   461,   708,
     875, -1292,   859,   861,  7972,   -15,   -15,   862, -1292, -1292,
     863,   867, -1292,   880,  9192,  5633, -1292,  5633,  5633,   971,
   -1292,   751, -1292, -1292,   870, -1292, -1292, -1292,  5633,  9276,
   -1292, -1292,   306,   874, -1292, -1292, -1292, -1292,   879,   881,
   -1292, -1292, -1292,   992,   883,   884,  9192,  9444,  9444, -1292,
    5633, -1292,  9360, -1292, -1292, -1292,   461, -1292, -1292,  5633,
   -1292, -1292,   885, -1292,  8062, -1292,   318,  9444, -1292,   751,
   -1292, -1292,   482, -1292
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   122,     1,   283,     0,     0,     0,   569,   284,     0,
     569,   569,   569,    16,     0,     0,    15,     3,     0,     0,
       0,     8,     0,     7,   550,     6,    11,     5,     4,    13,
      12,    14,    94,    95,    93,   102,   104,    38,    51,    48,
      49,    40,     0,    46,    39,   571,   570,     0,     0,    22,
      21,   550,     0,     0,     0,     0,   257,    36,   109,   110,
       0,     0,     0,   111,   113,   120,     0,   108,    17,    10,
       9,   593,   592,   215,   583,   594,   551,   552,     0,     0,
       0,     0,    41,     0,    47,     0,     0,    44,   572,   574,
      18,   738,   730,   734,   259,     0,     0,   119,   114,     0,
       0,     0,     0,     0,     0,   123,   217,   216,   219,   214,
     575,   596,   595,   599,   554,   553,   560,   100,   101,    98,
      99,    97,     0,     0,    96,   105,    52,    50,    46,    43,
      42,     0,    19,    20,    23,     0,     0,     0,   258,    34,
      37,   118,     0,   115,   116,   117,   121,     0,   577,   576,
       0,   589,   555,   485,    24,    25,    29,     0,    89,    90,
      87,    88,    86,    85,    91,     0,    45,     0,   739,   731,
     735,    35,   112,     0,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,     0,
       0,   129,   124,   225,   578,   579,   590,     0,   600,   557,
       0,   486,     0,    26,    27,    28,     0,   103,     0,     0,
       0,     0,     0,     0,   607,   627,   608,   643,   609,   613,
     614,   615,   616,   633,   620,   621,   622,   623,   624,   625,
     626,   628,   629,   630,   631,   700,   612,   619,   632,   707,
     714,   610,   617,   611,   618,     0,     0,     0,     0,   642,
     664,   667,   665,   666,   727,   662,   573,   648,   513,   519,
     183,   184,   181,   132,   133,   135,   134,   136,   137,   138,
     139,   165,   166,   163,   164,   156,   167,   168,   157,   154,
     155,   182,   176,     0,   180,   169,   170,   171,   172,   143,
     144,   145,   140,   141,   142,   153,     0,   159,   160,   158,
     151,   152,   147,   146,   148,   149,   150,   131,   130,   175,
       0,   161,   162,   485,   127,   220,     0,     0,     0,   587,
     597,   559,   556,   485,   106,     0,   539,   532,   561,    92,
     668,   691,   694,     0,   697,   687,     0,   651,   701,   708,
     715,   721,   724,     0,     0,   677,   682,   676,     0,   690,
     686,   679,     0,     0,   681,   663,     0,   649,   809,   732,
     736,   185,   186,   179,   174,   187,   177,   173,     0,   125,
     282,   507,     0,     0,   218,   248,   634,   637,   640,   641,
     635,   638,   636,   639,   580,   584,     0,   591,   498,   601,
       0,   107,     0,     0,   533,     0,     0,     0,     0,     0,
       0,   390,   391,     0,     0,     0,     0,   384,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   633,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   474,
     329,   331,   330,   332,   333,   334,   335,    30,     0,     0,
       0,     0,     0,     0,     0,     0,   314,   315,   388,   387,
     464,   385,   457,   456,   455,   454,   122,   460,   386,   459,
     458,   431,   392,   432,     0,   436,   393,     0,   389,   742,
     746,   743,   744,   745,     0,     0,     0,     0,     0,   124,
       0,   124,     0,   124,     0,     0,     0,   673,   251,   684,
     685,   678,   680,     0,   683,   659,     0,     0,   729,   728,
     810,   740,   257,   655,   654,     0,   514,   509,     0,     0,
       0,   520,     0,   188,   178,     0,   280,   281,     0,   485,
     126,   128,     0,   566,   588,   499,     0,   534,   532,   558,
     540,     0,   530,   531,   529,     0,     0,     0,     0,   656,
     765,   768,   262,   266,   265,   271,     0,   303,     0,     0,
       0,   794,     0,     0,     0,     0,   294,   297,     0,   300,
       0,   798,     0,   774,   780,     0,   771,     0,   420,   421,
       0,     0,     0,     0,     0,     0,     0,     0,   778,   801,
     809,   397,   396,   433,   395,   394,     0,     0,   809,   309,
     809,   316,     0,   324,   248,   315,   122,   228,     0,     0,
       0,     0,   422,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   374,     0,   671,     0,     0,     0,   644,   646,     0,
       0,   127,     0,   127,     0,   127,   511,     0,   517,     0,
     645,     0,     0,   809,   675,   658,   661,     0,   650,     0,
       0,   515,   733,     0,   521,   737,     0,     0,   602,   505,
     524,   508,   250,   274,   272,   225,     0,     0,   273,     0,
       0,     0,    72,     0,   229,     0,     0,     0,   242,     0,
     243,   237,     0,   234,   233,     0,   235,     0,     0,     0,
     249,     0,    68,    69,    66,    67,   244,   286,   232,     0,
      65,   562,   567,   581,   598,     0,   485,     0,     0,     0,
       0,     0,   547,     0,     0,   669,   692,   695,     0,     0,
       0,     0,   763,     0,     0,     0,   784,   787,   790,     0,
     809,     0,   809,     0,     0,     0,     0,     0,     0,     0,
     809,     0,     0,   809,     0,     0,     0,     0,     0,     0,
       0,     0,    33,     0,    31,     0,     0,   810,     0,     0,
       0,   810,     0,     0,     0,     0,   362,   359,   361,     0,
       0,   257,     0,   377,     0,   756,     0,     0,   124,     0,
     316,   324,     0,     0,   443,   442,     0,     0,   444,   448,
     398,   399,   411,   412,   409,   410,     0,   437,     0,   429,
       0,   461,   462,   463,   400,   401,   416,   417,   418,   419,
       0,     0,   414,   415,   413,   407,   408,   403,   402,   404,
     405,   406,     0,     0,     0,   368,     0,     0,     0,     0,
       0,   382,     0,   698,   688,     0,   652,   702,     0,   709,
       0,   716,     0,   722,     0,   725,     0,     0,   255,   672,
     252,   660,   741,   510,   516,   506,     0,     0,   523,     0,
     522,     0,   525,     0,     0,   275,     0,     0,   260,     0,
     247,     0,     0,    55,    70,    73,   225,     0,   277,   245,
     246,     0,    61,    62,     0,     0,     0,     0,   236,   221,
     231,   238,   239,   240,   241,   285,     0,   230,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   564,   582,   585,
     492,   495,     0,     0,   536,   537,     0,     0,     0,   549,
     670,   693,   696,   657,     0,     0,     0,   764,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,   435,     0,   465,     0,     0,
       0,     0,     0,     0,   324,   469,   470,   471,   472,   473,
       0,     0,     0,   777,   802,   803,     0,     0,   310,   783,
     424,     0,     0,     0,   124,     0,   378,     0,     0,     0,
       0,     0,     0,   381,     0,   379,     0,   127,   328,   124,
     439,     0,   445,     0,     0,     0,   427,     0,     0,   449,
     453,     0,     0,   430,     0,     0,     0,     0,   369,     0,
     375,     0,   425,     0,   383,   699,   689,   647,   653,   703,
     705,   710,   712,   717,   719,   723,   512,   726,   518,     0,
       0,     0,   604,   605,   526,   528,   276,   225,   261,     0,
      53,    54,    71,    79,   278,     0,     0,     0,     0,     0,
     222,   227,   541,     0,   292,   291,   346,   347,   349,   348,
     350,   340,   341,   342,   351,   352,   338,   339,   353,   354,
     343,   344,   345,   337,   563,   565,   568,     0,   493,   494,
     495,   496,   487,   500,   535,     0,   544,     0,     0,   548,
     766,   769,   263,     0,   268,   269,   267,     0,     0,   306,
     304,     0,     0,     0,   795,   793,   253,     0,   804,   295,
     298,   301,   799,   797,   775,   781,   779,   772,    72,     0,
      32,     0,     0,     0,     0,     0,     0,   127,   380,     0,
     748,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   322,     0,    72,   127,     0,     0,     0,     0,   357,
       0,     0,   450,     0,   438,     0,   428,     0,   370,     0,
       0,   426,   376,   372,     0,   706,   713,   720,   251,   256,
     603,     0,     0,   279,    84,     0,     0,     0,     0,     0,
       0,     0,   287,     0,   586,   490,   487,   488,   489,   492,
     538,     0,   546,     0,     0,   264,   270,     0,     0,   305,
     785,   788,   791,     0,     0,   809,     0,     0,     0,     0,
       0,   763,     0,     0,     0,   225,   475,     0,     0,     0,
       0,   360,   484,   363,     0,     0,   355,     0,     0,     0,
       0,   319,   320,     0,   318,   317,     0,   325,   225,    72,
       0,   483,     0,   481,   358,   478,     0,     0,     0,   477,
     371,     0,   373,     0,   606,   527,    63,    72,     0,    72,
      72,   248,   293,   542,   543,     0,   532,   491,   501,   490,
       0,   545,   763,   763,     0,     0,     0,     0,     0,   251,
     805,   254,   253,   296,   299,   302,     0,   764,   776,   763,
       0,   466,     0,   468,   476,     0,     0,     0,     0,   366,
       0,     0,     0,     0,   750,   749,     0,   323,     0,   311,
     326,   225,   440,   446,     0,   482,   480,   479,   674,     0,
       0,   225,     0,   225,   225,     0,   288,     0,     0,     0,
     503,   497,   762,   762,   307,     0,   763,   763,     0,     0,
       0,   809,   762,   755,     0,   467,     0,     0,     0,   364,
       0,   752,   751,   356,     0,   321,     0,   312,   327,   441,
     447,   451,    64,    68,    69,    66,    67,    65,    76,    58,
      72,    81,    83,     0,     0,     0,     0,     0,     0,   759,
     809,   761,     0,     0,     0,   762,   762,     0,   796,   806,
       0,     0,   773,   807,   807,     0,   367,     0,     0,   313,
     452,    72,    56,    57,     0,    75,   225,   224,     0,     0,
     502,   225,     0,   810,   760,   767,   770,   308,     0,     0,
     792,   800,   782,     0,     0,     0,   807,   754,   753,   225,
       0,    78,     0,   290,   504,   757,     0,   786,   789,     0,
     813,   811,     0,    59,     0,   289,     0,   808,   812,    72,
     758,   225,    58,    60
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1292, -1292, -1292, -1292, -1292, -1292,   469,   979, -1292, -1292,
   -1292,  1060, -1292, -1292, -1292,  1023, -1292,   938, -1292, -1292,
     986, -1292, -1292, -1292,  -413, -1292, -1292,  -278, -1292, -1133,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292,   887, -1292, -1292,   -61,   972, -1292, -1292, -1292,   383,
   -1292,  -401,  -472,  -626, -1292, -1292, -1292, -1291, -1292, -1292,
   -1292, -1292, -1292,  -602, -1292, -1292, -1292, -1292,  -601, -1149,
    -239, -1292,   -14, -1292, -1292, -1292, -1292, -1292,  -273,  -270,
    -264,  -258, -1292, -1292,  1096, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
    -361, -1292,   633,  -159, -1292,  -769, -1292, -1292, -1292, -1292,
   -1292, -1292, -1272, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292,   348, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292,  -144,  -108,  -190,  -106,     5, -1292, -1292, -1292, -1292,
   -1292,   599, -1292,  -484, -1292, -1292,  -469, -1292, -1292,  -669,
    -186,  -533,  -536, -1292, -1292, -1292,  -900, -1292,  1066, -1292,
   -1292, -1292, -1292, -1292,   180, -1292, -1292,   674, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,  -744,  -160,
   -1292,   713, -1292, -1292, -1292, -1292, -1292, -1292,  -337, -1292,
   -1292,  -353, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
    -145, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292,   728,  -597,  -249,  -722, -1292, -1292, -1254,
   -1199, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
    -771, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292, -1292,
   -1292, -1292, -1292, -1292, -1292,  -409, -1292, -1204,  -586, -1292
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   134,    51,    17,   157,   163,   586,   458,
     140,   459,    96,    19,    20,    43,    44,    87,    21,    39,
      40,   695,   696,  1434,  1435,   904,  1350,   697,   895,   896,
     698,   699,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   164,   165,    35,    36,    37,   211,    63,    64,    65,
      66,    22,   324,   384,   202,    23,   108,   203,   109,   147,
    1081,  1210,  1403,   325,   326,   709,   460,   710,   532,   662,
    1234,   867,   461,   711,   553,   741,  1225,   462,   712,   713,
     714,   715,   716,   528,   717,   916,  1213,  1084,   718,   463,
     754,  1237,   755,  1238,   757,  1239,   464,   745,  1229,   465,
     598,  1340,   466,  1171,  1172,   798,   467,   607,   468,   719,
     469,   470,   788,   471,  1003,  1328,  1004,  1380,   472,   848,
    1192,   473,   599,  1175,  1389,  1177,  1390,  1276,  1430,   475,
     476,   380,  1219,  1298,  1110,  1112,   942,   535,   726,  1359,
    1407,   381,   382,   515,   657,   369,   520,   659,   370,  1010,
     679,   545,   405,   538,   337,  1211,   539,   338,    77,   116,
     209,   333,    25,   153,   722,   938,   723,    47,    48,   131,
      26,   150,   205,   328,   939,    27,   110,   533,  1107,   397,
     207,   208,    75,   113,   399,    28,   151,   330,   680,   477,
     394,   261,   262,   649,   368,   263,   487,  1048,   516,   548,
     365,   264,   265,   406,   950,   661,   485,  1046,   407,   951,
     408,   952,   484,  1045,   488,  1049,   489,  1195,   490,  1051,
     491,  1196,   492,  1053,   493,  1197,   494,  1055,   495,  1057,
     517,    29,   136,   268,   518,    30,   137,   269,   522,    31,
     135,   267,   669,   478,  1409,  1374,   796,  1410,  1411,  1412,
     960,   479,   739,  1223,   740,  1224,   764,  1243,   761,  1241,
     589,   480,   762,  1242,   481,   964,  1306,   965,  1307,   966,
    1308,   749,  1233,   759,  1240,  1235,   482,  1454,   511,   483
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      57,    67,   731,   799,   778,   795,   994,   260,   733,   212,
     656,   883,   782,   509,   783,  1245,  1085,   651,   125,   653,
     959,   655,   266,  -122,   558,   858,   658,   860,  1151,   862,
       2,   992,  1019,   521,  1152,   996,   499,     3,  1250,   590,
    1268,  1288,  1318,   409,   410,  1217,    67,    67,    67,  1283,
     541,   618,   790,    79,   620,   621,  1379,    38,   620,   621,
       4,  1005,     5,   416,     6,   603,   378,  1007,  1406,   418,
       7,    49,   526,   529,    32,    33,   536,   870,    68,    83,
       8,   172,   378,   886,   501,   355,     9,    67,    67,    67,
      67,   537,   117,   118,   600,  1166,   101,   102,   103,  1082,
    1006,  1167,   530,  1362,  1363,  1218,   425,   426,  1426,  1413,
      10,    50,   356,   357,    84,   527,  1441,   379,  1421,   210,
    1372,   559,   560,   641,   642,   158,   159,   641,   642,  1083,
      69,  1168,    11,    12,   725,   210,  1341,   663,   790,    41,
     428,   429,   878,  1169,   104,   795,    70,  1005,  1170,   334,
      95,    76,   216,   259,  1351,   752,  1353,  1354,   141,   878,
    1369,  1448,  1449,    42,   968,  1009,   971,  1415,  1416,    78,
     105,    95,   608,   609,   979,   358,    55,   982,   671,   359,
     217,   879,   500,   674,   497,  1408,   724,   879,   670,   400,
     880,   879,   795,   366,   879,  1011,   608,   609,   947,   561,
      56,   750,    13,    80,   498,   801,   260,   880,   448,   260,
     751,   760,   738,    15,   763,  1148,    14,    34,   552,   562,
    1455,   366,   721,   260,   765,   735,  1357,   360,    15,   404,
     502,   361,   119,  1105,   362,  1123,   454,   120,   531,   121,
     122,   794,    85,   260,   354,  1014,   260,   260,   260,   503,
     363,   536,  1472,  1153,    86,   504,   364,   608,   609,   894,
     878,   546,   547,   549,    79,   160,   537,  1436,   612,   613,
     161,  1162,   162,   122,   401,    55,   618,   123,   619,   620,
     621,   622,   623,   852,   784,    45,   604,   570,  1012,   785,
     542,    46,   612,   613,  1073,  1014,   543,   677,  1459,    56,
     618,    81,   619,   620,   621,   622,   623,   879,   880,   878,
     881,  1294,   678,   882,  1043,  1009,  1013,   786,   456,   605,
     864,  1332,   866,   878,   260,   260,  1017,    58,   260,  1259,
     260,  1043,   260,  1207,   260,   638,   639,   640,   544,   645,
     646,    88,  1044,   650,    89,   652,  1481,   654,   641,   642,
     608,   609,   259,   612,   613,   259,    59,   880,   563,  1124,
     795,   618,    91,   619,   620,   621,   622,   623,   878,   259,
     894,   880,   641,   642,  1330,   331,   332,   946,   564,   542,
    1056,   958,   534,  -704,   542,   543,   954,   955,  -704,   259,
     543,  1173,   259,   259,   259,  1202,   967,  1058,  1244,   259,
     556,   973,   974,   260,   976,    92,   978,  -704,   980,   981,
    1266,   983,  1043,  1117,   970,    95,   880,   260,   636,   637,
     638,   639,   640,  1043,    60,   732,    93,   544,  1043,   878,
    1116,    55,   544,   641,   642,  1427,    71,    72,  1267,    73,
    1274,  1295,    55,    97,    98,    99,   612,   613,   805,   809,
     101,  1280,   103,   126,   618,    56,  1282,   620,   621,   622,
     623,  1043,  1043,   823,  1043,  1203,    56,    74,    86,  1296,
     259,   259,  1043,   536,   259,  1043,   259,   880,   259,  1014,
     259,   849,    94,    61,   143,   144,   145,   146,   537,  1324,
    1325,  1014,  1326,    62,  1431,   100,  1260,  1292,  1293,  -711,
    1378,   260,   571,  1400,  -711,  1432,  1433,  1465,   128,  -718,
     260,   129,   106,   260,  -718,  -365,   868,   386,   107,  1480,
    -365,   387,   572,  -711,   574,   873,   641,   642,   874,    55,
    1060,  1254,  1157,  -718,   743,   388,   389,  1215,   784,  -365,
     390,   391,   392,   393,   575,   111,   818,  1174,  1269,   259,
     519,   112,   366,    56,   744,   819,   736,  1111,   513,   514,
     513,   514,   855,   259,   513,   514,  1257,   863,   513,   514,
     260,   856,   366,   865,   513,   514,   853,   969,   260,   260,
     260,   260,   941,  1059,   114,   260,  1331,   787,   130,   260,
     115,   138,   132,   953,   260,   260,   956,   260,   133,   260,
     963,   260,   260,   139,   260,  1423,   513,   514,   152,  1032,
    1037,  1131,  1132,  1133,   790,  1127,  1164,  1137,  1033,  1038,
     513,   514,   148,  1005,  1128,  1165,   402,   729,   149,   403,
     730,  1108,   404,   404,   513,   514,   366,  1109,    84,   366,
     854,   366,   366,  1323,   366,   857,   859,   259,   861,  1311,
    1156,   366,   167,   366,   366,  1122,   259,  1130,  1343,   259,
     366,   902,   903,  1180,  1391,   168,  1339,   101,   102,   103,
    1030,   154,   155,   772,   773,  1189,   154,   155,   156,   169,
    1194,   213,   214,   215,    52,    53,    54,   213,   214,   170,
    1355,   474,   101,   171,   204,   206,   210,   321,   322,   323,
     327,   496,   329,   335,   260,   336,   340,   367,   343,   371,
     341,   506,   374,   342,   344,  1428,   259,   345,   346,   347,
     353,   260,   348,  1373,   259,   259,   259,   259,   349,   350,
     351,   259,   352,   366,   372,   259,  1064,   373,   375,  1388,
     259,   259,   376,   259,   383,   259,   385,   259,   259,  1399,
     259,  1401,  1402,   377,   395,   396,   398,   486,   507,   523,
    1358,   510,   524,   525,   540,   550,   565,   551,    15,   568,
     557,  1373,   566,   567,   569,   672,   573,   570,   576,   578,
     579,   577,   580,   581,   648,  1420,   519,   668,  1247,   675,
    1248,  1249,   673,   721,   601,   728,   588,   591,   592,   593,
     594,   595,   737,   582,   260,   260,   260,   583,   584,   585,
     260,  1442,   602,   644,   664,   647,   746,   727,   747,   742,
     748,   777,  1270,   775,  1444,  1405,   779,   780,   781,   797,
     800,   816,   872,   869,  1461,   876,   877,   663,   889,  1464,
     890,   891,   899,   260,   900,   901,   905,   906,   907,   908,
     259,   909,   915,   910,   911,   667,   912,  1473,   937,   913,
     914,   917,   940,   944,   936,   945,   961,   259,   995,  1476,
     999,  1438,   984,   676,  1000,   997,  1020,  1040,  1001,  1482,
     720,  1014,  1022,  1002,  1047,  1062,  1063,  1067,  1069,  1327,
    1071,  1072,  1008,   734,  1015,  1016,  1050,  1080,  1021,  1023,
     260,  1052,  1054,  1070,  1111,  1024,  1025,  1035,  1113,  1036,
    1041,  1075,   588,   753,  1115,  1199,   756,  1076,   758,  1120,
    1121,  1201,  1134,   894,  1139,  1077,  1078,  1352,   766,   767,
     768,   769,   770,   771,  1079,  1140,  1141,  1142,  1125,  1144,
    1145,  1126,  1147,  1135,  1136,  1182,  1138,  1143,   663,  1146,
     259,   259,   259,  1149,  1212,  1371,   259,  1183,   810,   811,
    1214,  1158,   812,   813,   814,   815,  1185,   817,  1190,   820,
     821,   822,   824,   825,   826,   827,   828,   829,   831,   832,
     833,   834,   835,   836,   837,   838,   839,   840,   841,   259,
     850,  1198,   608,   609,  1220,  1226,  1227,  1230,  1228,  1231,
    1232,  1236,  1253,  1256,  1404,  1258,  1261,  1262,  1263,  1264,
    1265,  1284,  1291,  1297,  1310,   260,  1317,   260,  1336,  1338,
    1305,  1329,  1302,  1303,  1309,  1349,  1312,  1316,  1319,  1320,
    1356,   885,  1272,  1342,   888,  1348,  1376,  1377,  1398,   893,
    1437,   898,  1383,  1365,  1425,  1469,   259,  1385,  1443,  1440,
    1366,  1367,  1453,  1368,  1470,   774,  1418,   124,  1419,  1422,
    1445,    18,  1446,  1450,  1451,    82,   166,   127,  1452,  1483,
    1460,  1392,   142,  1370,  1466,   943,  1393,   608,   609,  1394,
    1467,   948,  1468,  1471,  1478,  1395,   610,   611,   612,   613,
     614,  1396,   962,   615,   616,   617,   618,    24,   619,   620,
     621,   622,   623,   339,   624,   625,   606,  1337,  1299,  1360,
     626,   627,   628,  1300,  1361,  1216,   260,    90,  1106,  1417,
       0,   991,     0,     0,   993,   588,   554,     0,   681,   998,
       0,  1344,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,     0,     0,     0,     0,   630,   720,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,   259,     0,   259,     0,     0,     0,     0,   641,   642,
       0,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,  1034,     0,     0,     0,  1039,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1061,     0,     0,     0,     0,     0,
       0,  1065,  1066,     0,     0,  1068,   634,   635,   636,   637,
     638,   639,   640,     0,     0,  1074,     0,     0,     0,     0,
       0,     0,     0,   641,   642,     0,     0,     0,     0,     0,
       0,     0,   259,     0,     0,   802,  1086,  1087,  1088,  1089,
    1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,
    1100,  1101,  1102,  1103,  1104,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1118,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   608,   609,     0,     0,
       0,     0,     0,     0,   787,     0,     0,   224,   225,   226,
       0,   228,   229,   230,   231,   232,   427,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,     0,   246,
     247,   248,     0,   259,   251,   252,   253,   254,     0,  1154,
    1155,     0,     0,     0,     0,     0,  1159,  1160,  1161,     0,
     998,     0,     0,     0,     0,     0,   787,     0,     0,  1176,
       0,  1178,     0,  1181,     0,     0,   218,     0,     0,  1184,
       0,     0,   219,  1187,     0,     0,     0,     0,   220,     0,
       0,   998,   608,   609,     0,     0,     0,     0,   221,   803,
     610,   611,   612,   613,     0,     0,   222,     0,   804,     0,
     618,     0,   619,   620,   621,   622,   623,  1204,   624,   625,
       0,   223,     0,  1205,  1206,     0,  1208,  1209,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,     0,     0,     0,     0,  1221,     0,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   641,   642,   588,     0,   610,   611,   612,   613,
     614,     0,     0,   615,     0,    55,   618,     0,   619,   620,
     621,   622,   623,     0,   624,   625,     0,     0,   257,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1277,     0,  1278,     0,     0,     0,     0,  1281,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1285,   634,   635,   636,   637,   638,   639,   640,   218,   258,
       0,   508,     0,     0,   219,     0,     0,     0,   641,   642,
     220,     0,     0,     0,     0,  1304,     0,     0,     0,     0,
     221,     0,     0,     0,     0,  1313,  1314,  1315,   222,     0,
       0,     0,  1322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   223,     0,     0,  1333,  1334,  1335,     0,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     588,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,  1381,  1382,
       0,     0,     0,     0,     0,     0,  1387,     0,     0,     0,
     257,     0,     0,     0,     0,     0,     0,  1397,     0,     0,
       0,   512,     0,   720,     0,     0,     0,     0,     0,     0,
       0,   513,   514,  1414,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   588,  1424,     0,     0,     0,     0,
       0,     0,     0,     0,  1429,     0,     0,     0,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1456,     0,  1457,  1458,     0,     0,     0,
       0,     0,     0,     0,     0,   682,  1462,     0,     0,   409,
     410,     3,     0,   -80,   -74,   -74,     0,   -77,     0,   411,
     412,   413,   414,   415,     0,     0,     0,     0,  1474,   416,
     683,   417,   684,   685,     0,   418,     0,  1477,     0,     0,
       0,     0,   686,   419,     0,     0,   -82,     0,   687,   420,
       0,     0,   421,     0,     8,   422,   688,     0,   689,   423,
       0,     0,   690,   691,     0,     0,     0,     0,     0,   692,
       0,     0,   425,   426,     0,   224,   225,   226,     0,   228,
     229,   230,   231,   232,   427,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,     0,   246,   247,   248,
       0,     0,   251,   252,   253,   254,   428,   429,   430,   693,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
     433,   434,   435,   436,   437,     0,   438,     0,   439,   440,
     441,   442,   443,   444,   445,   446,    56,   447,     0,     0,
       0,     0,     0,     0,   448,  -226,   694,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   449,   450,   451,     0,
      14,     0,     0,   452,   453,     0,     0,     0,     0,     0,
       0,     0,   454,     0,   455,   682,   456,   457,     0,   409,
     410,     3,     0,   -80,   -74,   -74,     0,   -77,     0,   411,
     412,   413,   414,   415,     0,     0,     0,     0,     0,   416,
     683,   417,   684,   685,     0,   418,     0,     0,     0,     0,
       0,     0,   686,   419,     0,     0,   -82,     0,   687,   420,
       0,     0,   421,     0,     8,   422,   688,     0,   689,   423,
       0,     0,   690,   691,     0,     0,     0,     0,     0,   692,
       0,     0,   425,   426,     0,   224,   225,   226,     0,   228,
     229,   230,   231,   232,   427,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,     0,   246,   247,   248,
       0,     0,   251,   252,   253,   254,   428,   429,   430,   693,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
     433,   434,   435,   436,   437,     0,   438,     0,   439,   440,
     441,   442,   443,   444,   445,   446,    56,   447,     0,     0,
       0,     0,     0,     0,   448,  1018,   694,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   449,   450,   451,     0,
      14,     0,     0,   452,   453,     0,     0,     0,     0,     0,
       0,     0,   454,     0,   455,   682,   456,   457,     0,   409,
     410,     3,     0,   -80,   -74,   -74,     0,   -77,     0,   411,
     412,   413,   414,   415,     0,     0,     0,     0,     0,   416,
     683,   417,   684,   685,     0,   418,     0,     0,     0,     0,
       0,     0,   686,   419,     0,     0,   -82,     0,   687,   420,
       0,     0,   421,     0,     8,   422,   688,     0,   689,   423,
       0,     0,   690,   691,     0,     0,     0,     0,     0,   692,
       0,     0,   425,   426,     0,   224,   225,   226,     0,   228,
     229,   230,   231,   232,   427,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,     0,   246,   247,   248,
       0,     0,   251,   252,   253,   254,   428,   429,   430,   693,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
     433,   434,   435,   436,   437,     0,   438,     0,   439,   440,
     441,   442,   443,   444,   445,   446,    56,   447,     0,     0,
       0,     0,     0,     0,   448,  -223,   694,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   449,   450,   451,     0,
      14,     0,     0,   452,   453,     0,     0,     0,   409,   410,
       0,     0,   454,     0,   455,     0,   456,   457,   411,   412,
     413,   414,   415,     0,     0,     0,     0,     0,   416,   683,
     417,   684,     0,     0,   418,     0,     0,     0,     0,     0,
       0,     0,   419,     0,     0,     0,     0,     0,   420,     0,
       0,   421,     0,     0,   422,   688,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,   693,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,     0,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,   789,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,   790,   439,   440,   441,
     442,   443,   444,   445,   446,   791,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,   596,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   792,   419,   455,   793,   456,   457,     0,   420,     0,
       0,   421,   597,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,   790,   439,   440,   441,
     442,   443,   444,   445,   446,   791,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,   596,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,     0,   456,   457,     0,   420,     0,
       0,   421,   597,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,     0,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,   957,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,   790,   439,   440,   441,
     442,   443,   444,   445,   446,   791,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,     0,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,     0,     0,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   449,   450,   451,     0,    14,
       0,     0,   452,   453,     0,     0,     0,     0,     0,   409,
     410,   454,   505,   455,     0,   456,   457,   587,     0,   411,
     412,   413,   414,   415,     0,     0,     0,     0,     0,   416,
       0,   417,     0,     0,     0,   418,     0,     0,     0,     0,
       0,     0,     0,   419,     0,     0,     0,     0,     0,   420,
       0,     0,   421,     0,     0,   422,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   424,
       0,     0,   425,   426,     0,   224,   225,   226,     0,   228,
     229,   230,   231,   232,   427,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,     0,   246,   247,   248,
       0,     0,   251,   252,   253,   254,   428,   429,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
     433,   434,   435,   436,   437,     0,   438,     0,   439,   440,
     441,   442,   443,   444,   445,   446,    56,   447,     0,   409,
     410,     0,     0,     0,   448,     0,     0,     0,     0,   411,
     412,   413,   414,   415,     0,     0,   449,   450,   451,   416,
      14,   417,     0,   452,   453,   418,     0,     0,     0,     0,
       0,     0,   454,   419,   455,     0,   456,   457,     0,   420,
       0,     0,   421,     0,     0,   422,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   424,
       0,     0,   425,   426,     0,   224,   225,   226,     0,   228,
     229,   230,   231,   232,   427,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,     0,   246,   247,   248,
       0,     0,   251,   252,   253,   254,   428,   429,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
     433,   434,   435,   436,   437,     0,   438,     0,   439,   440,
     441,   442,   443,   444,   445,   446,    56,   447,     0,     0,
       0,     0,     0,     0,   448,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   449,   450,   451,     0,
      14,     0,     0,   452,   453,     0,     0,     0,   409,   410,
       0,     0,   454,   666,   455,     0,   456,   457,   411,   412,
     413,   414,   415,     0,     0,   830,     0,     0,   416,     0,
     417,     0,     0,     0,   418,     0,     0,     0,     0,     0,
       0,     0,   419,     0,     0,     0,     0,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,     0,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,   851,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,   884,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,     0,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,   887,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,     0,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,   892,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,     0,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,   897,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,     0,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,  1179,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,  1188,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,  1193,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,  1246,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,  1321,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,  1386,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,     0,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   409,   410,
       0,     0,     0,   448,     0,     0,     0,     0,   411,   412,
     413,   414,   415,     0,     0,   449,   450,   451,   416,    14,
     417,     0,   452,   453,   418,     0,     0,     0,     0,     0,
       0,   454,   419,   455,     0,   456,   457,     0,   420,     0,
       0,   421,     0,     0,   422,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,   425,   426,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,   428,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   433,
     434,   435,   436,   437,     0,   438,     0,   439,   440,   441,
     442,   443,   444,   445,   446,    56,   447,     0,   218,     0,
       0,     0,     0,   448,   219,     0,     0,     0,     0,     0,
     220,     0,     0,     0,     0,   449,   450,   451,     0,    14,
     221,     0,   452,   453,     0,     0,     0,     0,   222,     0,
       0,  1163,     0,   455,     0,   456,   457,     0,     0,     0,
       0,     0,     0,   223,     0,     0,     0,     0,     0,     0,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,     0,     0,     0,     0,     0,   608,   609,
     218,     0,     0,     0,     0,     0,   219,     0,     0,     0,
       0,     0,   220,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,     0,     0,     0,    55,     0,     0,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     257,     0,     0,     0,     0,   223,     0,     0,     0,     0,
       0,    56,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,     0,     0,     0,     0,     0,
       0,   258,   610,   611,   612,   613,   614,   608,   609,   615,
     616,   617,   618,     0,   619,   620,   621,   622,   623,     0,
     624,   625,     0,     0,     0,     0,   626,   627,   628,    55,
       0,     0,   629,     0,     0,     0,     0,     0,   608,   609,
       0,     0,   257,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   641,   642,     0,     0,   643,     0,
       0,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,   608,   609,
       0,   629,   610,   611,   612,   613,   614,     0,     0,   615,
     616,   617,   618,     0,   619,   620,   621,   622,   623,     0,
     624,   625,     0,     0,     0,     0,   626,   627,   628,   608,
     609,   630,   629,   631,   632,   633,   634,   635,   636,   637,
     638,   639,   640,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   641,   642,     0,     0,   660,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   641,   642,     0,     0,   875,     0,
       0,     0,   610,   611,   612,   613,   614,     0,     0,   615,
     616,   617,   618,     0,   619,   620,   621,   622,   623,     0,
     624,   625,     0,     0,     0,     0,   626,   627,   628,   608,
     609,     0,   629,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
     608,   609,   630,   629,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   641,   642,     0,     0,   972,     0,
       0,     0,     0,   630,     0,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   641,   642,     0,     0,   975,
       0,     0,     0,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
     608,   609,     0,   629,   610,   611,   612,   613,   614,     0,
       0,   615,   616,   617,   618,     0,   619,   620,   621,   622,
     623,     0,   624,   625,     0,     0,     0,     0,   626,   627,
     628,   608,   609,   630,   629,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   641,   642,     0,     0,   977,
       0,     0,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,     0,     0,
     985,     0,     0,     0,   610,   611,   612,   613,   614,     0,
       0,   615,   616,   617,   618,     0,   619,   620,   621,   622,
     623,     0,   624,   625,     0,     0,     0,     0,   626,   627,
     628,   608,   609,     0,   629,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,   608,   609,   630,   629,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,     0,     0,
     986,     0,     0,     0,     0,   630,     0,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   641,   642,     0,
       0,   987,     0,     0,     0,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,   608,   609,     0,   629,   610,   611,   612,   613,
     614,     0,     0,   615,   616,   617,   618,     0,   619,   620,
     621,   622,   623,     0,   624,   625,     0,     0,     0,     0,
     626,   627,   628,   608,   609,   630,   629,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   641,   642,     0,
       0,   988,     0,     0,     0,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   641,   642,
       0,     0,   989,     0,     0,     0,   610,   611,   612,   613,
     614,     0,     0,   615,   616,   617,   618,     0,   619,   620,
     621,   622,   623,     0,   624,   625,     0,     0,     0,     0,
     626,   627,   628,   608,   609,     0,   629,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,   608,   609,   630,   629,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   641,   642,
       0,     0,   990,     0,     0,     0,     0,   630,     0,   631,
     632,   633,   634,   635,   636,   637,   638,   639,   640,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   641,
     642,     0,     0,  1114,     0,     0,     0,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,   608,   609,     0,   629,   610,   611,
     612,   613,   614,     0,     0,   615,   616,   617,   618,     0,
     619,   620,   621,   622,   623,     0,   624,   625,     0,     0,
       0,     0,   626,   627,   628,   608,   609,   630,   629,   631,
     632,   633,   634,   635,   636,   637,   638,   639,   640,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   641,
     642,     0,     0,  1129,     0,     0,     0,     0,   630,     0,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     641,   642,     0,     0,  1200,     0,     0,     0,   610,   611,
     612,   613,   614,     0,     0,   615,   616,   617,   618,     0,
     619,   620,   621,   622,   623,     0,   624,   625,     0,     0,
       0,     0,   626,   627,   628,   608,   609,     0,   629,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
       0,     0,     0,   626,   627,   628,   608,   609,   630,   629,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     641,   642,     0,     0,  1251,     0,     0,     0,     0,   630,
       0,   631,   632,   633,   634,   635,   636,   637,   638,   639,
     640,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   641,   642,     0,     0,  1252,     0,     0,     0,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
       0,     0,     0,   626,   627,   628,   608,   609,     0,   629,
     610,   611,   612,   613,   614,     0,     0,   615,   616,   617,
     618,     0,   619,   620,   621,   622,   623,     0,   624,   625,
       0,     0,     0,     0,   626,   627,   628,   608,   609,   630,
     629,   631,   632,   633,   634,   635,   636,   637,   638,   639,
     640,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   641,   642,     0,     0,  1255,     0,     0,     0,     0,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   641,   642,     0,     0,  1271,     0,     0,     0,
     610,   611,   612,   613,   614,     0,     0,   615,   616,   617,
     618,     0,   619,   620,   621,   622,   623,     0,   624,   625,
       0,     0,     0,     0,   626,   627,   628,   608,   609,     0,
     629,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,   608,   609,
     630,   629,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   641,   642,     0,     0,  1273,     0,     0,     0,
       0,   630,     0,   631,   632,   633,   634,   635,   636,   637,
     638,   639,   640,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   641,   642,     0,     0,  1275,     0,     0,
       0,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,   608,   609,
       0,   629,   610,   611,   612,   613,   614,     0,     0,   615,
     616,   617,   618,     0,   619,   620,   621,   622,   623,     0,
     624,   625,     0,     0,     0,     0,   626,   627,   628,   608,
     609,   630,   629,   631,   632,   633,   634,   635,   636,   637,
     638,   639,   640,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   641,   642,     0,     0,  1279,     0,     0,
       0,     0,   630,     0,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   641,   642,     0,     0,  1286,     0,
       0,     0,   610,   611,   612,   613,   614,     0,     0,   615,
     616,   617,   618,     0,   619,   620,   621,   622,   623,     0,
     624,   625,     0,     0,     0,     0,   626,   627,   628,   608,
     609,     0,   629,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
     608,   609,   630,   629,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   641,   642,     0,     0,  1287,     0,
       0,     0,     0,   630,     0,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   641,   642,     0,     0,  1289,
       0,     0,     0,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
     608,   609,     0,   629,   610,   611,   612,   613,   614,     0,
       0,   615,   616,   617,   618,     0,   619,   620,   621,   622,
     623,     0,   624,   625,     0,     0,     0,     0,   626,   627,
     628,   608,   609,   630,   629,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   641,   642,     0,     0,  1290,
       0,     0,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,     0,     0,
    1345,     0,     0,     0,   610,   611,   612,   613,   614,     0,
       0,   615,   616,   617,   618,     0,   619,   620,   621,   622,
     623,     0,   624,   625,     0,     0,     0,     0,   626,   627,
     628,   608,   609,     0,   629,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,   608,   609,   630,   629,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,     0,     0,
    1346,     0,     0,     0,     0,   630,     0,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   641,   642,     0,
       0,  1347,     0,     0,     0,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,   608,   609,     0,   629,   610,   611,   612,   613,
     614,     0,     0,   615,   616,   617,   618,     0,   619,   620,
     621,   622,   623,     0,   624,   625,     0,     0,     0,     0,
     626,   627,   628,   608,   609,   630,   629,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   641,   642,     0,
       0,  1364,     0,     0,     0,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   641,   642,
       0,     0,  1375,     0,     0,     0,   610,   611,   612,   613,
     614,     0,     0,   615,   616,   617,   618,     0,   619,   620,
     621,   622,   623,     0,   624,   625,     0,     0,     0,     0,
     626,   627,   628,   608,   609,     0,   629,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,   608,   609,   630,   629,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   641,   642,
       0,     0,  1384,     0,     0,     0,     0,   630,     0,   631,
     632,   633,   634,   635,   636,   637,   638,   639,   640,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   641,
     642,     0,     0,  1447,     0,     0,     0,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,   608,
     609,   626,   627,   628,     0,     0,     0,   629,   610,   611,
     612,   613,   614,     0,     0,   615,   616,   617,   618,     0,
     619,   620,   621,   622,   623,     0,   624,   625,     0,     0,
     608,   609,   626,   627,   628,     0,     0,   630,   629,   631,
     632,   633,   634,   635,   636,   637,   638,   639,   640,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   641,
     642,     0,     0,  1479,     0,     0,     0,     0,   630,     0,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     641,   642,   665,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,   608,   609,   626,   627,   628,
       0,     0,     0,   629,   610,   611,   612,   613,   614,     0,
       0,   615,   616,   617,   618,     0,   619,   620,   621,   622,
     623,     0,   624,   625,     0,     0,   608,   609,   626,   627,
     628,     0,     0,   630,   629,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   641,   642,   871,     0,     0,
       0,     0,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,  1026,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
       0,   608,   609,   626,   627,   628,     0,     0,     0,   629,
     610,   611,   612,   613,   614,     0,     0,   615,   616,   617,
     618,     0,   619,   620,   621,   622,   623,     0,   624,   625,
       0,     0,     0,     0,   626,   627,   628,     0,     0,   630,
     629,   631,   632,   633,   634,   635,   636,   637,   638,   639,
     640,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   641,   642,  1042,     0,     0,     0,     0,     0,     0,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   641,   642,  1186,   610,   611,   612,   613,   614,
     608,   609,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,     0,     0,     0,   626,
     627,   628,     0,     0,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   630,     0,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   641,   642,  1191,
       0,     0,     0,     0,     0,     0,   918,   919,   920,   921,
     922,   923,   924,   925,   610,   611,   612,   613,   614,   926,
     927,   615,   616,   617,   618,   928,   619,   620,   621,   622,
     623,   929,   624,   625,   930,   931,   270,   271,   626,   627,
     628,   932,   933,   934,   629,     0,     0,     0,     0,     0,
       0,     0,     0,   272,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   608,   609,     0,
       0,  -336,     0,   935,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
       0,     0,   291,   292,   293,     0,     0,   294,   295,   296,
     297,   298,   608,   609,   299,   300,   301,   302,   303,   304,
     305,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,   776,     0,   626,   627,   628,     0,     0,
     306,   629,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,     0,     0,   317,   318,     0,     0,     0,     0,
       0,     0,   319,   320,     0,     0,     0,     0,     0,     0,
       0,   630,     0,   631,   632,   633,   634,   635,   636,   637,
     638,   639,   640,     0,     0,     0,   608,   609,     0,     0,
       0,     0,     0,   641,   642,     0,   610,   611,   612,   613,
     614,     0,     0,   615,   616,   617,   618,     0,   619,   620,
     621,   622,   623,     0,   624,   625,     0,     0,     0,     0,
     626,   627,   628,     0,     0,     0,   629,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   949,     0,     0,   630,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,     0,     0,
     608,   609,     0,     0,     0,     0,     0,     0,   641,   642,
     610,   611,   612,   613,   614,     0,     0,   615,   616,   617,
     618,     0,   619,   620,   621,   622,   623,     0,   624,   625,
       0,     0,     0,     0,   626,   627,   628,     0,     0,     0,
     629,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     630,  1031,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,   608,   609,     0,     0,     0,     0,
       0,     0,   641,   642,   610,   611,   612,   613,   614,     0,
       0,   615,   616,   617,   618,     0,   619,   620,   621,   622,
     623,     0,   624,   625,     0,   608,   609,     0,   626,   627,
     628,     0,     0,     0,   629,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1119,     0,     0,   630,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,   610,   611,
     612,   613,   614,     0,     0,   615,   616,   617,   618,     0,
     619,   620,   621,   622,   623,     0,   624,   625,     0,   608,
     609,     0,   626,   627,   628,     0,     0,     0,   629,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
       0,  1150,     0,   626,   627,   628,     0,     0,   630,   629,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     641,   642,     0,     0,     0,     0,  1222,     0,     0,   630,
       0,   631,   632,   633,   634,   635,   636,   637,   638,   639,
     640,     0,     0,   608,   609,     0,     0,     0,     0,     0,
       0,   641,   642,   610,   611,   612,   613,   614,     0,     0,
     615,   616,   617,   618,     0,   619,   620,   621,   622,   623,
       0,   624,   625,     0,     0,     0,     0,   626,   627,   628,
       0,     0,     0,   629,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1301,     0,     0,   630,     0,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,     0,     0,   608,   609,     0,
       0,     0,     0,     0,     0,   641,   642,   610,   611,   612,
     613,   614,     0,     0,   615,   616,   617,   618,     0,   619,
     620,   621,   622,   623,     0,   624,   625,     0,     0,     0,
       0,   626,   627,   628,     0,     0,     0,   629,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1453,     0,     0,   630,     0,   631,
     632,   633,   634,   635,   636,   637,   638,   639,   640,     0,
       0,   608,   609,     0,     0,     0,     0,     0,     0,   641,
     642,   610,   611,   612,   613,   614,     0,     0,   615,   616,
     617,   618,     0,   619,   620,   621,   622,   623,     0,   624,
     625,     0,     0,     0,     0,   626,   627,   628,     0,     0,
       0,   629,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1463,     0,
       0,   630,     0,   631,   632,   633,   634,   635,   636,   637,
     638,   639,   640,     0,     0,   608,   609,     0,     0,     0,
       0,     0,     0,   641,   642,   610,   611,   612,   613,   614,
       0,     0,   615,   616,   617,   618,     0,   619,   620,   621,
     622,   623,     0,   624,   625,     0,   608,   609,     0,   626,
     627,   628,     0,     0,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1475,     0,     0,   630,     0,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   641,   642,   610,
     611,   612,   613,   614,     0,     0,   615,   616,   617,   618,
       0,   619,   620,   621,   622,   623,     0,   624,   625,     0,
     608,   609,     0,   626,   627,   628,     0,     0,     0,   629,
     610,   611,   612,   613,   614,     0,     0,   615,   616,   617,
     618,     0,   619,   620,   621,   622,   623,     0,   624,   625,
     608,   609,     0,     0,   626,   627,   628,     0,     0,   630,
    -719,   631,   632,   633,   634,   635,   636,   637,   638,   639,
     640,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   641,   642,     0,     0,     0,     0,     0,     0,     0,
     630,     0,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   641,   642,   610,   611,   612,   613,   614,     0,
       0,   615,   616,   617,   618,     0,   619,   620,   621,   622,
     623,     0,   624,   625,   608,   609,     0,     0,   626,     0,
     628,     0,     0,     0,   610,   611,   612,   613,   614,     0,
       0,   615,   616,   617,   618,     0,   619,   620,   621,   622,
     623,     0,   624,   625,   608,   609,     0,     0,   626,     0,
       0,     0,     0,     0,     0,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,     0,     0,
       0,     0,     0,     0,     0,     0,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   641,   642,   610,   611,
     612,   613,   614,     0,     0,   615,   616,   617,   618,     0,
     619,   620,   621,   622,   623,     0,   624,   625,   608,   609,
       0,     0,     0,     0,     0,     0,     0,     0,   610,   611,
     612,   613,   614,     0,     0,   615,   616,   617,   618,     0,
     619,   620,   621,   622,   623,     0,   624,   625,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     641,   642,   806,     0,     0,     0,     0,     0,     0,     0,
       0,   632,   633,   634,   635,   636,   637,   638,   639,   640,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     641,   642,   610,   611,   612,   613,   614,     0,     0,   615,
     616,   617,   618,     0,   619,   620,   621,   622,   623,     0,
     624,   625,     0,     0,   224,   225,   226,     0,   228,   229,
     230,   231,   232,   427,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   246,   247,   248,     0,
       0,   251,   252,   253,   254,     0,     0,     0,     0,     0,
    1027,     0,     0,     0,     0,     0,   633,   634,   635,   636,
     637,   638,   639,   640,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   641,   642,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   807,     0,     0,     0,
       0,     0,   224,   225,   226,   808,   228,   229,   230,   231,
     232,   427,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,     0,   246,   247,   248,     0,     0,   251,
     252,   253,   254,   224,   225,   226,     0,   228,   229,   230,
     231,   232,   427,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,     0,   246,   247,   248,     0,     0,
     251,   252,   253,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1028,   173,     0,     0,     0,     0,
     842,   843,     0,  1029,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   844,     0,     0,     0,     0,
       0,   174,     0,   175,   845,   176,   177,   178,   179,   180,
       0,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,     0,   192,   193,   194,     0,     0,   195,   196,
     197,   198,     0,     0,     0,     0,     0,     0,     0,   846,
     847,     0,     0,     0,     0,     0,     0,   199,   200,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   201
};

static const yytype_int16 yycheck[] =
{
      14,    15,   538,   604,   590,   602,   777,   167,   541,   153,
     494,   680,   598,   366,   600,  1148,   916,   489,    79,   491,
     742,   493,   167,     8,     5,   651,   495,   653,    20,   655,
       0,   775,   801,   370,    20,   779,    33,     7,    20,   448,
    1173,    20,  1241,     5,     6,    46,    60,    61,    62,  1198,
     403,   125,   153,   173,   128,   129,  1328,   155,   128,   129,
      30,   162,    32,    25,    34,   466,   150,   789,  1359,    31,
      40,   162,     7,   172,    19,    20,   147,   663,   172,    22,
      50,   201,   150,   685,    33,    33,    56,   101,   102,   103,
     104,   162,    15,    16,   455,   126,   139,   140,   141,   170,
     201,   132,   201,  1302,  1303,   106,    68,    69,  1380,  1363,
      80,   202,    60,    61,    57,    50,  1407,   201,  1372,   203,
    1319,   102,   103,   197,   198,    15,    16,   197,   198,   200,
     172,   162,   102,   103,   535,   203,  1269,   498,   153,   162,
     102,   103,   126,   174,   173,   742,   172,   162,   179,   210,
     138,    62,   173,   167,  1287,   564,  1289,  1290,   201,   126,
    1309,  1415,  1416,   186,   750,   132,   752,  1366,  1367,   174,
     199,   138,    21,    22,   760,   123,   138,   763,   515,   127,
     201,   173,   179,   520,   180,   200,   171,   173,   176,   333,
     174,   173,   789,   177,   173,   792,    21,    22,   731,   180,
     162,   562,   172,   176,   200,   606,   366,   174,   170,   369,
     563,   572,   549,   198,   575,   984,   186,   162,   180,   200,
    1424,   177,   162,   383,   577,   181,   176,   175,   198,   179,
     179,   179,   155,   173,   182,   957,   198,   160,   383,   162,
     163,   602,   185,   403,   258,   173,   406,   407,   408,   198,
     198,   147,  1456,   997,   197,   204,   204,    21,    22,   172,
     126,   406,   407,   408,   173,   155,   162,  1400,   117,   118,
     160,   199,   162,   163,   335,   138,   125,   200,   127,   128,
     129,   130,   131,   644,   147,    57,   170,   200,   173,   152,
     126,    63,   117,   118,   896,   173,   132,   147,  1431,   162,
     125,   173,   127,   128,   129,   130,   131,   173,   174,   126,
     176,  1211,   162,   179,   173,   132,   201,   180,   202,   203,
     657,   199,   659,   126,   484,   485,   798,    34,   488,   132,
     490,   173,   492,  1077,   494,   184,   185,   186,   174,   484,
     485,   162,   201,   488,   172,   490,  1479,   492,   197,   198,
      21,    22,   366,   117,   118,   369,    63,   174,   180,   201,
     957,   125,   162,   127,   128,   129,   130,   131,   126,   383,
     172,   174,   197,   198,   132,   171,   172,   730,   200,   126,
     864,   742,   396,   176,   126,   132,   739,   740,   181,   403,
     132,  1017,   406,   407,   408,  1064,   749,   866,   200,   413,
     414,   754,   755,   563,   757,   162,   759,   200,   761,   762,
     173,   764,   173,   946,   751,   138,   174,   577,   182,   183,
     184,   185,   186,   173,   131,   172,   162,   174,   173,   126,
     172,   138,   174,   197,   198,   132,     5,     6,   201,     8,
     201,   173,   138,    60,    61,    62,   117,   118,   608,   609,
     139,   201,   141,   155,   125,   162,   201,   128,   129,   130,
     131,   173,   173,   623,   173,  1067,   162,    36,   197,   201,
     484,   485,   173,   147,   488,   173,   490,   174,   492,   173,
     494,   641,   162,   190,   101,   102,   103,   104,   162,   201,
     201,   173,   201,   200,    12,   200,  1165,   171,   172,   176,
     201,   661,   180,   201,   181,    23,    24,   201,   162,   176,
     670,   162,    57,   673,   181,   176,   661,    75,    63,   201,
     181,    79,   200,   200,   180,   670,   197,   198,   673,   138,
     867,  1157,  1004,   200,   180,    93,    94,    47,   147,   200,
      98,    99,   100,   101,   200,    57,   153,  1019,  1174,   563,
     162,    63,   177,   162,   200,   162,   181,    67,   172,   173,
     172,   173,   172,   577,   172,   173,  1163,   181,   172,   173,
     730,   181,   177,   181,   172,   173,   181,   181,   738,   739,
     740,   741,   726,   181,    57,   745,  1255,   601,   162,   749,
      63,   162,    57,   738,   754,   755,   741,   757,    63,   759,
     745,   761,   762,   164,   764,  1376,   172,   173,   170,   153,
     153,   964,   965,   966,   153,   181,   153,   970,   162,   162,
     172,   173,    57,   162,   961,   162,   173,   173,    63,   176,
     176,    57,   179,   179,   172,   173,   177,    63,    57,   177,
     181,   177,   177,  1245,   177,   181,   181,   661,   181,  1235,
    1003,   177,   174,   177,   177,   181,   670,   181,   181,   673,
     177,    10,    11,  1024,   181,   170,  1268,   139,   140,   141,
     830,   164,   165,   166,   167,  1036,   164,   165,   166,   170,
    1041,   164,   165,   166,    10,    11,    12,   164,   165,   170,
    1291,   343,   139,   164,   162,   106,   203,    35,    35,   200,
     176,   353,   162,   203,   864,   162,   180,   162,   200,   162,
     180,   363,   199,   180,   180,  1384,   730,   180,   200,   180,
     200,   881,   180,  1320,   738,   739,   740,   741,   180,   180,
     180,   745,   180,   177,   162,   749,   881,   162,    22,  1341,
     754,   755,   162,   757,   176,   759,   170,   761,   762,  1351,
     764,  1353,  1354,   199,   170,   176,   170,   162,   198,   162,
    1296,   173,   132,   200,   162,   180,   200,   180,   198,   200,
     180,  1368,   180,   180,   180,   171,   180,   200,   180,   431,
     432,   180,   200,   200,   162,  1371,   162,   162,  1149,   171,
    1151,  1152,   176,   162,   203,    43,   448,   449,   450,   451,
     452,   453,   181,   200,   964,   965,   966,   200,   200,   200,
     970,  1408,   200,   200,   199,   201,   180,   200,   180,   200,
     180,   173,  1175,   200,  1410,  1358,   200,    13,   173,     4,
     203,   162,   171,   201,  1436,   200,    43,  1198,   162,  1441,
     172,   155,   172,  1003,   172,    10,    13,     9,    42,   172,
     864,   171,    66,   172,   172,   507,   172,  1459,   173,   172,
     172,   172,     8,   162,   174,   162,   162,   881,   171,  1466,
     199,  1404,   181,   525,   201,   200,   180,     1,   200,  1481,
     532,   173,   180,   200,   162,   162,   162,    14,   174,  1250,
     155,   172,   200,   545,   201,   200,   181,    37,   200,   200,
    1060,   181,   181,   176,    67,   200,   200,   200,   172,   200,
     200,   200,   564,   565,    43,  1060,   568,   200,   570,   181,
     181,    43,   181,   172,   181,   200,   200,  1288,   580,   581,
     582,   583,   584,   585,   200,   181,   181,   181,   201,   181,
     181,   201,   181,   201,   200,   180,   201,   201,  1309,   201,
     964,   965,   966,   200,   162,  1316,   970,   200,   610,   611,
     171,   201,   614,   615,   616,   617,   200,   619,   200,   621,
     622,   623,   624,   625,   626,   627,   628,   629,   630,   631,
     632,   633,   634,   635,   636,   637,   638,   639,   640,  1003,
     642,   200,    21,    22,   162,   201,   200,   181,   162,   181,
     181,   181,   181,   201,  1357,   200,   162,   162,   200,   162,
     162,   162,   170,    33,   201,  1175,    70,  1177,   162,   137,
     181,   181,   200,   200,   200,    12,   200,   200,   200,   200,
     162,   683,  1177,   181,   686,   201,   172,   172,   172,   691,
     171,   693,   201,   200,   172,    53,  1060,   201,   173,   172,
     200,   200,   172,   200,   171,   586,   201,    78,   201,   201,
     201,     1,   201,   201,   201,    42,   128,    81,   201,  1482,
     200,  1349,   100,  1312,   200,   727,  1349,    21,    22,  1349,
     201,   733,   201,   199,   199,  1349,   115,   116,   117,   118,
     119,  1349,   744,   122,   123,   124,   125,     1,   127,   128,
     129,   130,   131,   216,   133,   134,   473,  1266,  1216,  1299,
     139,   140,   141,  1219,  1300,  1110,  1276,    51,   938,  1368,
      -1,   773,    -1,    -1,   776,   777,   413,    -1,   529,   781,
      -1,  1276,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   413,    -1,    -1,    -1,    -1,   175,   799,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,  1175,    -1,  1177,    -1,    -1,    -1,    -1,   197,   198,
      -1,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,   843,    -1,    -1,    -1,   847,    -1,  1357,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   876,    -1,    -1,    -1,    -1,    -1,
      -1,   883,   884,    -1,    -1,   887,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,   897,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1276,    -1,    -1,    19,   918,   919,   920,   921,
     922,   923,   924,   925,   926,   927,   928,   929,   930,   931,
     932,   933,   934,   935,   936,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   947,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,  1328,    -1,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,  1357,    98,    99,   100,   101,    -1,  1001,
    1002,    -1,    -1,    -1,    -1,    -1,  1008,  1009,  1010,    -1,
    1012,    -1,    -1,    -1,    -1,    -1,  1380,    -1,    -1,  1021,
      -1,  1023,    -1,  1025,    -1,    -1,    19,    -1,    -1,  1031,
      -1,    -1,    25,  1035,    -1,    -1,    -1,    -1,    31,    -1,
      -1,  1043,    21,    22,    -1,    -1,    -1,    -1,    41,   153,
     115,   116,   117,   118,    -1,    -1,    49,    -1,   162,    -1,
     125,    -1,   127,   128,   129,   130,   131,  1069,   133,   134,
      -1,    64,    -1,  1075,  1076,    -1,  1078,  1079,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,  1117,    -1,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,  1136,    -1,   115,   116,   117,   118,
     119,    -1,    -1,   122,    -1,   138,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1183,    -1,  1185,    -1,    -1,    -1,    -1,  1190,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1202,   180,   181,   182,   183,   184,   185,   186,    19,   202,
      -1,   204,    -1,    -1,    25,    -1,    -1,    -1,   197,   198,
      31,    -1,    -1,    -1,    -1,  1227,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,  1237,  1238,  1239,    49,    -1,
      -1,    -1,  1244,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,  1258,  1259,  1260,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1312,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,  1330,  1331,
      -1,    -1,    -1,    -1,    -1,    -1,  1338,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,  1349,    -1,    -1,
      -1,   162,    -1,  1355,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   172,   173,  1365,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1376,  1377,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1386,    -1,    -1,    -1,    -1,    -1,
      -1,   202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1405,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1425,    -1,  1427,  1428,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,  1438,    -1,    -1,     5,
       6,     7,    -1,     9,    10,    11,    -1,    13,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,  1460,    25,
      26,    27,    28,    29,    -1,    31,    -1,  1469,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    42,    -1,    44,    45,
      -1,    -1,    48,    -1,    50,    51,    52,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   170,   171,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    -1,   200,     1,   202,   203,    -1,     5,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   170,   171,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    -1,   200,     1,   202,   203,    -1,     5,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   170,   171,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,     5,     6,
      -1,    -1,   198,    -1,   200,    -1,   202,   203,    15,    16,
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
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,    -1,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    70,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    13,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,   201,   202,   203,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    13,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,    -1,   202,   203,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,    -1,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    70,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,    -1,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,     5,
       6,   198,   199,   200,    -1,   202,   203,    13,    -1,    15,
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
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,     5,
       6,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,
      16,    17,    18,    19,    -1,    -1,   182,   183,   184,    25,
     186,    27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    39,   200,    -1,   202,   203,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,
     186,    -1,    -1,   189,   190,    -1,    -1,    -1,     5,     6,
      -1,    -1,   198,   199,   200,    -1,   202,   203,    15,    16,
      17,    18,    19,    -1,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,    -1,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,   201,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,    -1,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,    -1,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,    -1,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,    -1,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,   201,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,   201,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,   201,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,   201,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,   201,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,    -1,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,     5,     6,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    -1,   182,   183,   184,    25,   186,
      27,    -1,   189,   190,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    39,   200,    -1,   202,   203,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    19,    -1,
      -1,    -1,    -1,   170,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,   182,   183,   184,    -1,   186,
      41,    -1,   189,   190,    -1,    -1,    -1,    -1,    49,    -1,
      -1,   198,    -1,   200,    -1,   202,   203,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    21,    22,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,   162,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   115,   116,   117,   118,   119,    21,    22,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,   138,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,    -1,    -1,   201,    -1,
      -1,    -1,    -1,   202,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    21,    22,
      -1,   145,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    21,
      22,   175,   145,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,    -1,    -1,   201,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,    -1,    -1,   201,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    21,
      22,    -1,   145,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      21,    22,   175,   145,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,    -1,    -1,   201,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,   201,
      -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      21,    22,    -1,   145,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    21,    22,   175,   145,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,   201,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,
     201,    -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    21,    22,    -1,   145,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    21,    22,   175,   145,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,
     201,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,
      -1,   201,    -1,    -1,    -1,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    21,    22,    -1,   145,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    21,    22,   175,   145,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,
      -1,   201,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
      -1,    -1,   201,    -1,    -1,    -1,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    21,    22,    -1,   145,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    21,    22,   175,   145,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
      -1,    -1,   201,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,    -1,    -1,   201,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    21,    22,    -1,   145,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    21,    22,   175,   145,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,    -1,    -1,   201,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,    -1,    -1,   201,    -1,    -1,    -1,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    21,    22,    -1,   145,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    21,    22,   175,   145,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,    -1,    -1,   201,    -1,    -1,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,    -1,    -1,   201,    -1,    -1,    -1,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    21,    22,    -1,   145,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    21,    22,   175,
     145,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,    -1,    -1,   201,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,    -1,    -1,   201,    -1,    -1,    -1,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    21,    22,    -1,
     145,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    21,    22,
     175,   145,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,    -1,    -1,   201,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,    -1,    -1,   201,    -1,    -1,
      -1,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    21,    22,
      -1,   145,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    21,
      22,   175,   145,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,    -1,    -1,   201,    -1,    -1,
      -1,    -1,   175,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,    -1,    -1,   201,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    21,
      22,    -1,   145,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      21,    22,   175,   145,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,    -1,    -1,   201,    -1,
      -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,   201,
      -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      21,    22,    -1,   145,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    21,    22,   175,   145,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,   201,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,
     201,    -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    21,    22,    -1,   145,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    21,    22,   175,   145,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,
     201,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,
      -1,   201,    -1,    -1,    -1,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    21,    22,    -1,   145,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    21,    22,   175,   145,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,
      -1,   201,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
      -1,    -1,   201,    -1,    -1,    -1,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    21,    22,    -1,   145,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    21,    22,   175,   145,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
      -1,    -1,   201,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,    -1,    -1,   201,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    21,
      22,   139,   140,   141,    -1,    -1,    -1,   145,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      21,    22,   139,   140,   141,    -1,    -1,   175,   145,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,    -1,    -1,   201,    -1,    -1,    -1,    -1,   175,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   199,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    21,    22,   139,   140,   141,
      -1,    -1,    -1,   145,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    -1,    21,    22,   139,   140,
     141,    -1,    -1,   175,   145,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,    21,    22,   139,   140,   141,    -1,    -1,    -1,   145,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,   175,
     145,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,   115,   116,   117,   118,   119,
      21,    22,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,    21,    22,   139,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,   172,    -1,   174,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,   136,    21,    22,   139,   140,   141,   142,   143,   144,
     145,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,   137,    -1,   139,   140,   141,    -1,    -1,
     175,   145,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,   197,   198,    -1,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    -1,   175,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    -1,    21,    22,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    -1,   175,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    21,
      22,    -1,   139,   140,   141,    -1,    -1,    -1,   145,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      -1,   168,    -1,   139,   140,   141,    -1,    -1,   175,   145,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,    -1,    -1,    -1,    -1,   172,    -1,    -1,   175,
      -1,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   115,   116,   117,   118,   119,    -1,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,    -1,   175,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,    -1,   175,    -1,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
      -1,   175,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,    -1,   133,   134,    -1,    21,    22,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,    -1,   175,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,    -1,   133,   134,    -1,
      21,    22,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,    -1,   133,   134,
      21,    22,    -1,    -1,   139,   140,   141,    -1,    -1,   175,
     145,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    21,    22,    -1,    -1,   139,    -1,
     141,    -1,    -1,    -1,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,    -1,   133,   134,    21,    22,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,    -1,
     133,   134,    -1,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   179,   180,   181,   182,
     183,   184,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,   162,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    35,    -1,    -1,    -1,    -1,
     128,   129,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,   162,    75,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   162
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   206,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   172,   186,   198,   207,   210,   216,   218,
     219,   223,   256,   260,   289,   367,   375,   380,   390,   436,
     440,   444,    19,    20,   162,   248,   249,   250,   155,   224,
     225,   162,   186,   220,   221,    57,    63,   372,   373,   162,
     202,   209,   372,   372,   372,   138,   162,   277,    34,    63,
     131,   190,   200,   252,   253,   254,   255,   277,   172,   172,
     172,     5,     6,     8,    36,   387,    62,   363,   174,   173,
     176,   173,   220,    22,    57,   185,   197,   222,   162,   172,
     363,   162,   162,   162,   162,   138,   217,   254,   254,   254,
     200,   139,   140,   141,   173,   199,    57,    63,   261,   263,
     381,    57,    63,   388,    57,    63,   364,    15,    16,   155,
     160,   162,   163,   200,   212,   249,   155,   225,   162,   162,
     162,   374,    57,    63,   208,   445,   437,   441,   162,   164,
     215,   201,   250,   254,   254,   254,   254,   264,    57,    63,
     376,   391,   170,   368,   164,   165,   166,   211,    15,    16,
     155,   160,   162,   212,   246,   247,   222,   174,   170,   170,
     170,   164,   201,    35,    71,    73,    75,    76,    77,    78,
      79,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    93,    94,    95,    98,    99,   100,   101,   117,
     118,   162,   259,   262,   162,   377,   106,   385,   386,   365,
     203,   251,   336,   164,   165,   166,   173,   201,    19,    25,
      31,    41,    49,    64,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   151,   202,   277,
     394,   396,   397,   400,   406,   407,   435,   446,   438,   442,
      21,    22,    38,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   127,   128,   129,   132,   133,   134,   135,   136,   139,
     140,   141,   142,   143,   144,   145,   175,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   189,   190,   197,
     198,    35,    35,   200,   257,   268,   269,   176,   378,   162,
     392,   171,   172,   366,   249,   203,   162,   359,   362,   246,
     180,   180,   180,   200,   180,   180,   200,   180,   180,   180,
     180,   180,   180,   200,   277,    33,    60,    61,   123,   127,
     175,   179,   182,   198,   204,   405,   177,   162,   399,   350,
     353,   162,   162,   162,   199,    22,   162,   199,   150,   201,
     336,   346,   347,   176,   258,   170,    75,    79,    93,    94,
      98,    99,   100,   101,   395,   170,   176,   384,   170,   389,
     336,   249,   173,   176,   179,   357,   408,   413,   415,     5,
       6,    15,    16,    17,    18,    19,    25,    27,    31,    39,
      45,    48,    51,    55,    65,    68,    69,    80,   102,   103,
     104,   117,   118,   146,   147,   148,   149,   150,   152,   154,
     155,   156,   157,   158,   159,   160,   161,   163,   170,   182,
     183,   184,   189,   190,   198,   200,   202,   203,   214,   216,
     271,   277,   282,   294,   301,   304,   307,   311,   313,   315,
     316,   318,   323,   326,   327,   334,   335,   394,   448,   456,
     466,   469,   481,   484,   417,   411,   162,   401,   419,   421,
     423,   425,   427,   429,   431,   433,   327,   180,   200,    33,
     179,    33,   179,   198,   204,   199,   327,   198,   204,   406,
     173,   483,   162,   172,   173,   348,   403,   435,   439,   162,
     351,   403,   443,   162,   132,   200,     7,    50,   288,   172,
     201,   435,   273,   382,   277,   342,   147,   162,   358,   361,
     162,   406,   126,   132,   174,   356,   435,   435,   404,   435,
     180,   180,   180,   279,   396,   448,   277,   180,     5,   102,
     103,   180,   200,   180,   200,   200,   180,   180,   200,   180,
     200,   180,   200,   180,   180,   200,   180,   180,   327,   327,
     200,   200,   200,   200,   200,   200,   213,    13,   327,   465,
     480,   327,   327,   327,   327,   327,    13,    49,   305,   327,
     305,   203,   200,   256,   170,   203,   307,   312,    21,    22,
     115,   116,   117,   118,   119,   122,   123,   124,   125,   127,
     128,   129,   130,   131,   133,   134,   139,   140,   141,   145,
     175,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   197,   198,   201,   200,   435,   435,   201,   162,   398,
     435,   257,   435,   257,   435,   257,   348,   349,   351,   352,
     201,   410,   274,   305,   199,   199,   199,   327,   162,   447,
     176,   403,   171,   176,   403,   171,   327,   147,   162,   355,
     393,   346,     1,    26,    28,    29,    38,    44,    52,    54,
      58,    59,    65,   105,   172,   226,   227,   232,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   270,
     272,   278,   283,   284,   285,   286,   287,   289,   293,   314,
     327,   162,   369,   371,   171,   256,   343,   200,    43,   173,
     176,   357,   172,   356,   327,   181,   181,   181,   403,   457,
     459,   280,   200,   180,   200,   302,   180,   180,   180,   476,
     305,   406,   480,   327,   295,   297,   327,   299,   327,   478,
     305,   463,   467,   305,   461,   406,   327,   327,   327,   327,
     327,   327,   166,   167,   211,   200,   137,   173,   483,   200,
      13,   173,   483,   483,   147,   152,   180,   277,   317,    70,
     153,   162,   198,   201,   305,   449,   451,     4,   310,   273,
     203,   256,    19,   153,   162,   394,    19,   153,   162,   394,
     327,   327,   327,   327,   327,   327,   162,   327,   153,   162,
     327,   327,   327,   394,   327,   327,   327,   327,   327,   327,
      22,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   128,   129,   153,   162,   197,   198,   324,   394,
     327,   201,   305,   181,   181,   172,   181,   181,   258,   181,
     258,   181,   258,   181,   403,   181,   403,   276,   435,   201,
     483,   199,   171,   435,   435,   201,   200,    43,   126,   173,
     174,   176,   179,   354,   126,   327,   268,    61,   327,   162,
     172,   155,    58,   327,   172,   233,   234,   126,   327,   172,
     172,    10,    10,    11,   230,    13,     9,    42,   172,   171,
     172,   172,   172,   172,   172,    66,   290,   172,   107,   108,
     109,   110,   111,   112,   113,   114,   120,   121,   126,   132,
     135,   136,   142,   143,   144,   174,   174,   173,   370,   379,
       8,   336,   341,   327,   162,   162,   406,   356,   327,   172,
     409,   414,   416,   435,   406,   406,   435,    70,   305,   451,
     455,   162,   327,   435,   470,   472,   474,   406,   483,   181,
     403,   483,   201,   406,   406,   201,   406,   201,   406,   483,
     406,   406,   483,   406,   181,   201,   201,   201,   201,   201,
     201,   327,   393,   327,   465,   171,   393,   200,   327,   199,
     201,   200,   200,   319,   321,   162,   201,   451,   200,   132,
     354,   449,   173,   201,   173,   201,   200,   257,   171,   310,
     180,   200,   180,   200,   200,   200,   199,    19,   153,   162,
     394,   176,   153,   162,   327,   200,   200,   153,   162,   327,
       1,   200,   199,   173,   201,   418,   412,   162,   402,   420,
     181,   424,   181,   428,   181,   432,   348,   434,   351,   181,
     403,   327,   162,   162,   435,   327,   327,    14,   327,   174,
     176,   155,   172,   268,   327,   200,   200,   200,   200,   200,
      37,   265,   170,   200,   292,   361,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   173,   369,   383,    57,    63,
     339,    67,   340,   172,   201,    43,   172,   356,   327,   172,
     181,   181,   181,   451,   201,   201,   201,   181,   403,   201,
     181,   406,   406,   406,   181,   201,   200,   406,   201,   181,
     181,   181,   181,   201,   181,   181,   201,   181,   310,   200,
     168,    20,    20,   393,   327,   327,   406,   257,   201,   327,
     327,   327,   199,   198,   153,   162,   126,   132,   162,   174,
     179,   308,   309,   258,   257,   328,   327,   330,   327,   201,
     305,   327,   180,   200,   327,   200,   199,   327,   201,   305,
     200,   199,   325,   201,   305,   422,   426,   430,   200,   435,
     201,    43,   354,   268,   327,   327,   327,   393,   327,   327,
     266,   360,   162,   291,   171,    47,   340,    46,   106,   337,
     162,   327,   172,   458,   460,   281,   201,   200,   162,   303,
     181,   181,   181,   477,   275,   480,   181,   296,   298,   300,
     479,   464,   468,   462,   200,   234,   201,   305,   305,   305,
      20,   201,   201,   181,   258,   201,   201,   449,   200,   132,
     354,   162,   162,   200,   162,   162,   173,   201,   234,   258,
     406,   201,   435,   201,   201,   201,   332,   327,   327,   201,
     201,   327,   201,   274,   162,   327,   201,   201,    20,   201,
     201,   170,   171,   172,   361,   173,   201,    33,   338,   337,
     339,   172,   200,   200,   327,   181,   471,   473,   475,   200,
     201,   483,   200,   327,   327,   327,   200,    70,   455,   200,
     200,   201,   327,   268,   201,   201,   201,   305,   320,   181,
     132,   354,   199,   327,   327,   327,   162,   308,   137,   268,
     306,   234,   181,   181,   435,   201,   201,   201,   201,    12,
     231,   234,   305,   234,   234,   273,   162,   176,   357,   344,
     338,   355,   455,   455,   201,   200,   200,   200,   200,   274,
     275,   305,   455,   449,   450,   201,   172,   172,   201,   317,
     322,   327,   327,   201,   201,   201,   126,   327,   268,   329,
     331,   181,   232,   283,   284,   285,   286,   327,   172,   268,
     201,   268,   268,   267,   406,   356,   262,   345,   200,   449,
     452,   453,   454,   454,   327,   455,   455,   450,   201,   201,
     483,   454,   201,   465,   327,   172,   317,   132,   354,   327,
     333,    12,    23,    24,   228,   229,   234,   171,   356,   327,
     172,   262,   449,   173,   483,   201,   201,   201,   454,   454,
     201,   201,   201,   172,   482,   482,   327,   327,   327,   234,
     200,   268,   327,   172,   268,   201,   200,   201,   201,    53,
     171,   199,   482,   268,   327,   172,   449,   327,   199,   201,
     201,   234,   268,   229
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   205,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   207,   208,   208,
     208,   209,   209,   210,   211,   211,   211,   211,   212,   212,
     213,   213,   213,   214,   215,   215,   217,   216,   218,   219,
     220,   220,   220,   220,   221,   221,   222,   222,   223,   224,
     224,   225,   225,   226,   227,   227,   228,   228,   229,   229,
     229,   230,   230,   231,   231,   232,   232,   232,   232,   232,
     233,   233,   234,   234,   236,   235,   237,   239,   238,   240,
     242,   241,   244,   243,   245,   246,   246,   246,   246,   246,
     246,   247,   247,   248,   248,   248,   249,   249,   249,   249,
     249,   249,   249,   249,   250,   250,   251,   251,   252,   252,
     252,   253,   253,   254,   254,   254,   254,   254,   254,   254,
     255,   255,   256,   256,   257,   257,   257,   258,   258,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   260,   261,   261,   261,   262,   264,
     263,   265,   266,   267,   265,   269,   270,   268,   271,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   273,   273,
     273,   274,   274,   275,   275,   276,   276,   277,   277,   277,
     278,   278,   280,   281,   279,   279,   282,   282,   282,   282,
     282,   282,   283,   284,   285,   285,   285,   286,   286,   287,
     288,   288,   288,   289,   289,   290,   290,   291,   291,   292,
     292,   293,   293,   293,   295,   296,   294,   297,   298,   294,
     299,   300,   294,   302,   303,   301,   304,   304,   304,   305,
     305,   306,   306,   306,   307,   307,   307,   308,   308,   308,
     308,   308,   309,   309,   310,   310,   311,   312,   312,   313,
     313,   313,   313,   313,   313,   313,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   315,   315,   316,   316,   317,
     317,   318,   319,   320,   318,   321,   322,   318,   323,   323,
     323,   323,   323,   323,   324,   325,   323,   326,   326,   326,
     326,   326,   326,   326,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   328,
     329,   327,   327,   327,   327,   330,   331,   327,   327,   327,
     332,   333,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   334,   334,   334,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   336,   336,   337,   337,   337,
     338,   338,   339,   339,   339,   340,   340,   341,   342,   343,
     342,   344,   342,   345,   342,   346,   346,   347,   347,   348,
     348,   349,   349,   350,   350,   350,   351,   352,   352,   353,
     353,   353,   354,   354,   355,   355,   355,   355,   355,   356,
     356,   356,   357,   357,   358,   358,   358,   358,   358,   359,
     359,   360,   360,   360,   361,   361,   361,   362,   362,   362,
     363,   363,   364,   364,   364,   365,   365,   366,   365,   367,
     368,   367,   369,   369,   370,   370,   371,   371,   371,   372,
     372,   372,   374,   373,   375,   376,   376,   376,   377,   378,
     378,   379,   379,   381,   382,   383,   380,   384,   384,   385,
     385,   386,   387,   387,   388,   388,   388,   389,   389,   391,
     392,   390,   393,   393,   393,   393,   393,   394,   394,   394,
     394,   394,   394,   394,   394,   394,   394,   394,   394,   394,
     394,   394,   394,   394,   394,   394,   394,   394,   394,   394,
     394,   394,   394,   394,   395,   395,   395,   395,   395,   395,
     395,   395,   396,   397,   397,   397,   398,   398,   399,   399,
     399,   401,   402,   400,   403,   403,   404,   404,   405,   405,
     405,   405,   406,   406,   407,   407,   407,   407,   408,   409,
     407,   407,   407,   410,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   411,   412,   407,
     407,   413,   414,   407,   415,   416,   407,   417,   418,   407,
     407,   419,   420,   407,   421,   422,   407,   407,   423,   424,
     407,   425,   426,   407,   407,   427,   428,   407,   429,   430,
     407,   431,   432,   407,   433,   434,   407,   435,   435,   435,
     437,   438,   439,   436,   441,   442,   443,   440,   445,   446,
     447,   444,   448,   448,   448,   448,   448,   449,   449,   449,
     449,   449,   449,   449,   449,   450,   451,   452,   452,   453,
     453,   454,   454,   455,   455,   457,   458,   456,   459,   460,
     456,   461,   462,   456,   463,   464,   456,   465,   465,   466,
     467,   468,   466,   469,   470,   471,   469,   472,   473,   469,
     474,   475,   469,   469,   476,   477,   469,   469,   478,   479,
     469,   480,   480,   481,   481,   481,   481,   482,   482,   483,
     483,   484,   484,   484
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       1,     1,     1,     4,     1,     1,     2,     2,     3,     2,
       0,     2,     4,     3,     1,     2,     0,     4,     2,     2,
       1,     2,     3,     3,     2,     4,     0,     1,     2,     1,
       3,     1,     3,     3,     3,     2,     1,     1,     0,     3,
       7,     1,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     2,     0,     1,     0,     8,     7,     0,     9,     3,
       0,     7,     0,     7,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     1,     5,     1,     3,     2,     3,     1,     1,
       1,     1,     4,     1,     2,     3,     3,     3,     3,     2,
       1,     3,     0,     3,     0,     2,     3,     0,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     2,     2,     3,     4,     3,
       2,     2,     2,     2,     2,     3,     3,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     1,     1,     3,     0,
       4,     0,     0,     0,     6,     0,     0,     6,     2,     1,
       2,     2,     1,     1,     1,     1,     2,     1,     2,     2,
       2,     2,     1,     1,     1,     2,     2,     2,     0,     2,
       2,     0,     2,     0,     2,     1,     3,     1,     3,     2,
       2,     3,     0,     0,     5,     1,     2,     5,     5,     5,
       6,     2,     1,     1,     1,     2,     3,     2,     3,     4,
       1,     1,     0,     1,     1,     1,     0,     1,     3,     8,
       7,     3,     3,     5,     0,     0,     7,     0,     0,     7,
       0,     0,     7,     0,     0,     6,     5,     8,    10,     1,
       3,     1,     2,     3,     1,     1,     2,     2,     2,     2,
       2,     4,     1,     3,     0,     4,     7,     7,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     6,     8,     5,     6,     1,
       4,     3,     0,     0,     8,     0,     0,     9,     3,     4,
       5,     6,     5,     6,     0,     0,     5,     3,     4,     4,
       5,     4,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     4,     4,     5,     4,     5,     3,
       4,     1,     1,     2,     4,     4,     1,     3,     5,     0,
       0,     8,     3,     3,     3,     0,     0,     8,     3,     4,
       0,     0,     9,     4,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     1,     4,     7,     8,     7,     4,
       4,     4,     4,     4,     1,     6,     7,     6,     6,     7,
       7,     6,     7,     6,     6,     0,     1,     0,     1,     1,
       0,     1,     0,     1,     1,     0,     1,     5,     0,     0,
       4,     0,     9,     0,    10,     3,     4,     1,     3,     1,
       3,     1,     3,     0,     2,     3,     3,     1,     3,     0,
       2,     3,     1,     1,     1,     2,     3,     5,     3,     1,
       1,     1,     0,     1,     1,     4,     3,     3,     5,     1,
       3,     0,     2,     2,     4,     6,     5,     4,     6,     5,
       0,     1,     0,     1,     1,     0,     2,     0,     4,     6,
       0,     6,     1,     3,     1,     2,     0,     1,     3,     0,
       1,     1,     0,     5,     3,     0,     1,     1,     1,     0,
       2,     0,     1,     0,     0,     0,    12,     0,     2,     0,
       1,     3,     1,     1,     0,     1,     1,     0,     3,     0,
       0,     7,     1,     4,     3,     3,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     1,     3,     0,     1,
       3,     0,     0,     6,     1,     1,     1,     3,     3,     2,
       4,     3,     1,     2,     1,     1,     1,     1,     0,     0,
       6,     4,     5,     0,     9,     4,     2,     2,     3,     2,
       3,     2,     2,     3,     3,     3,     2,     0,     0,     6,
       2,     0,     0,     6,     0,     0,     6,     0,     0,     6,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     0,     0,     6,     0,     0,     6,     1,     3,     3,
       0,     0,     0,     9,     0,     0,     0,     9,     0,     0,
       0,    10,     1,     1,     1,     1,     1,     3,     3,     5,
       5,     6,     6,     8,     8,     1,     1,     3,     5,     1,
       2,     1,     0,     0,     1,     0,     0,    10,     0,     0,
      10,     0,     0,     9,     0,     0,     7,     3,     1,     5,
       0,     0,    10,     4,     0,     0,    11,     0,     0,    11,
       0,     0,    10,     5,     0,     0,     9,     5,     0,     0,
      10,     1,     3,     4,     5,     7,     9,     0,     3,     0,
       1,    11,    12,    11
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

  case 17: /* top_level_reader_macro: expr_reader "end of expression"  */
                             {
        delete (yyvsp[-1].pExpression);    // we do nothing, we don't even attemp to 'visit'
    }
    break;

  case 18: /* optional_public_or_private_module: %empty  */
                        { (yyval.b) = yyextra->g_Program->policies.default_module_public; }
    break;

  case 19: /* optional_public_or_private_module: "public"  */
                        { (yyval.b) = true; }
    break;

  case 20: /* optional_public_or_private_module: "private"  */
                        { (yyval.b) = false; }
    break;

  case 21: /* module_name: '$'  */
                    { (yyval.s) = new string("$"); }
    break;

  case 22: /* module_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 23: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module  */
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

  case 24: /* character_sequence: STRING_CHARACTER  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 25: /* character_sequence: STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 26: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 27: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 28: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 29: /* string_constant: "start of the string" "end of the string"  */
                                                           { (yyval.s) = new string(); }
    break;

  case 30: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 31: /* string_builder_body: string_builder_body character_sequence  */
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

  case 32: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                                        {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 33: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 34: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 35: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das2_yyend_reader(scanner);
        }
    }
    break;

  case 36: /* $@1: %empty  */
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

  case 37: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
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

  case 38: /* options_declaration: "options" annotation_argument_list  */
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

  case 40: /* require_module_name: "name"  */
                   {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 41: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 42: /* require_module_name: require_module_name '.' "name"  */
                                                {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 43: /* require_module_name: require_module_name '/' "name"  */
                                                {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 44: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 45: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 46: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 47: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 51: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 52: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 53: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 54: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 55: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 56: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 57: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 58: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 59: /* expression_else: "else" optional_semis expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 60: /* expression_else: elif_or_static_elif '(' expr ')' optional_semis expression_block expression_else  */
                                                                                                                 {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-4].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 61: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 62: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 63: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 64: /* expression_else_one_liner: "else" expression_if_one_liner  */
                                                      {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 65: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 66: /* expression_if_one_liner: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 67: /* expression_if_one_liner: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 69: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 74: /* $@2: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 75: /* expression_if_then_else: $@2 if_or_static_if '(' expr ')' optional_semis expression_block expression_else  */
                                                                                                           {
        yyextra->das_keyword = false;
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-4].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-6].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 76: /* expression_if_then_else_oneliner: expression_if_one_liner "if" '(' expr ')' expression_else_one_liner "end of expression"  */
                                                                                                                {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-6].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 77: /* $@3: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 78: /* expression_for_loop: $@3 "for" '(' variable_name_with_pos_list "in" expr_list ')' optional_semis expression_block  */
                                                                                                                            {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-5].pNameWithPosList),(yyvsp[-3].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 79: /* expression_unsafe: "unsafe" optional_semis expression_block  */
                                                               {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-2])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 80: /* $@4: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 81: /* expression_while_loop: $@4 "while" '(' expr ')' optional_semis expression_block  */
                                                                                    {
        yyextra->das_keyword = false;
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-5])));
        pWhile->cond = ExpressionPtr((yyvsp[-3].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 82: /* $@5: %empty  */
        {
        yyextra->das_keyword = true;
    }
    break;

  case 83: /* expression_with: $@5 "with" '(' expr ')' optional_semis expression_block  */
                                                                              {
        yyextra->das_keyword = false;
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-5])));
        pWith->with = ExpressionPtr((yyvsp[-3].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 84: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-2].s);
    }
    break;

  case 85: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 86: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 87: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 88: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 89: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 90: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 91: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 92: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 93: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 94: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 95: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 96: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 97: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 98: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 99: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 100: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 101: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 102: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 103: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 104: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 105: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 106: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 107: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 108: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 109: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 110: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 111: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 112: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 113: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 114: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 115: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 116: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 117: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 118: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 119: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 120: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 121: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 122: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 123: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 124: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 125: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 126: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 127: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 128: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 129: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 130: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 131: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 132: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 133: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 134: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 135: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 136: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 137: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 138: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 139: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 140: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 141: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 142: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 143: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 144: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 145: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 146: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 147: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 148: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 149: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 150: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 151: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 152: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 153: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 154: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 155: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 156: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 157: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 158: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 159: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 160: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 161: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 162: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 163: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 164: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 165: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 166: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 167: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 168: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 169: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 170: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 171: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 172: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 173: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 174: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 175: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 176: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 177: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 178: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 179: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 180: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 181: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 182: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 183: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 184: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 185: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 186: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 187: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 188: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 189: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 190: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 191: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 192: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 193: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 194: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 195: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 196: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 197: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 198: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 199: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 200: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 201: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 202: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 203: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 204: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 205: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 206: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 207: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 208: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 209: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 210: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 211: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 212: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 213: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 214: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 215: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 216: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 217: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 218: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 219: /* $@6: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 220: /* function_declaration: optional_public_or_private_function $@6 function_declaration_header expression_block  */
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

  case 221: /* expression_block_finally: %empty  */
        {
        (yyval.pExpression) = nullptr;
    }
    break;

  case 222: /* $@7: %empty  */
                  {
        yyextra->das_indent_level++;
        yyextra->push_nesteds();
    }
    break;

  case 223: /* $@8: %empty  */
                             {
        yyextra->das_indent_level--;
        yyextra->pop_nesteds();
    }
    break;

  case 224: /* expression_block_finally: "finally" $@7 "begin of code block" expressions $@8 "end of code block"  */
          {
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 225: /* $@9: %empty  */
        {
        yyextra->das_indent_level++;
        yyextra->push_nesteds();
    }
    break;

  case 226: /* $@10: %empty  */
                                      {
        yyextra->das_indent_level--;
        yyextra->pop_nesteds();
    }
    break;

  case 227: /* expression_block: $@9 "begin of code block" expressions $@10 "end of code block" expression_block_finally  */
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

  case 228: /* expr_call_pipe: expr_call expr_full_block_assumed_piped  */
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

  case 229: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 230: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 231: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 232: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 235: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 236: /* expression_any: expression_with_alias "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 237: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 238: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 239: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 240: /* expression_any: expression_return "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 241: /* expression_any: expression_yield "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 242: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 243: /* expression_any: expression_if_then_else_oneliner  */
                                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 244: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 245: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 246: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 247: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 248: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 249: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 250: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 251: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 252: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 253: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 254: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 255: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 256: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 257: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 258: /* name_in_namespace: "name" "::" "name"  */
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

  case 259: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 260: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 261: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 262: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 263: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 264: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 265: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 266: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 267: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 268: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 269: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 270: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 271: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 272: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 273: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 274: /* expression_return: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 275: /* expression_return: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 276: /* expression_return: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 277: /* expression_yield: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 278: /* expression_yield: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 279: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 280: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 281: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 282: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 283: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 284: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 285: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 286: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 287: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 288: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 289: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 290: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 291: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 292: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 293: /* expression_let: kwd_let optional_in_scope "begin of code block" variable_declaration_list "end of code block"  */
                                                                               {
        (yyval.pExpression) = ast_LetList(scanner,(yyvsp[-4].b),(yyvsp[-3].b),*(yyvsp[-1].pVarDeclList),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 294: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 295: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 296: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 297: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 298: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 299: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 300: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 301: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 302: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 303: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 304: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 305: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 306: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 307: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 308: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" c_or_s "name" '>' '(' expr ')'  */
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

  case 309: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 310: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 311: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 312: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 313: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 314: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 315: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 316: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 317: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 318: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 319: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 320: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 321: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 322: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 323: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 324: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 325: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 326: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_semis block_or_simple_block  */
                                                                                                           {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 327: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type optional_semis expression_block  */
                                                                                                      {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-5].faList),(yyvsp[-4].pCaptList),(yyvsp[-3].pVarDeclList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 328: /* expr_full_block_assumed_piped: "begin of code block" expressions "end of code block"  */
                                   {
        (yyval.pExpression) = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,new TypeDecl(Type::autoinfer),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 329: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 330: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 331: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 332: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 333: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 334: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 335: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 336: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 337: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 344: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 345: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 356: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 357: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 358: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 359: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 360: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 361: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 362: /* $@21: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 363: /* $@22: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 364: /* func_addr_expr: '@' '@' '<' $@21 type_declaration_no_options '>' $@22 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 365: /* $@23: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 366: /* $@24: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 367: /* func_addr_expr: '@' '@' '<' $@23 optional_function_argument_list optional_function_type '>' $@24 func_addr_name  */
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

  case 368: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 369: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 370: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 371: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 372: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 373: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 374: /* $@25: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 375: /* $@26: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 376: /* expr_field: expr '.' $@25 error $@26  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 377: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 378: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 379: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 380: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
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

  case 381: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 382: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 383: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 384: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 385: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 386: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 387: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 388: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 389: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 390: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 391: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 392: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 393: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 394: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 402: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 404: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 405: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 406: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 407: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 408: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 409: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 410: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 411: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 412: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 413: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 414: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 415: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 416: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 417: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 418: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 419: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 420: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 421: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 422: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 423: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 424: /* expr: '(' expr_list optional_comma ')'  */
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

  case 425: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 426: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 427: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 428: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 429: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 430: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 431: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 432: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 433: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 435: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 436: /* expr: expr_generator  */
                                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 437: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 439: /* $@27: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 440: /* $@28: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 441: /* expr: expr "is" "type" '<' $@27 type_declaration_no_options '>' $@28  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 442: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 443: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 444: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 445: /* $@29: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 446: /* $@30: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 447: /* expr: expr "as" "type" '<' $@29 type_declaration '>' $@30  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 448: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 449: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 450: /* $@31: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 451: /* $@32: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 452: /* expr: expr '?' "as" "type" '<' $@31 type_declaration '>' $@32  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 453: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 454: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 455: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 456: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 457: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 458: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 459: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 460: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 461: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 462: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 463: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 464: /* expr: expr_call_pipe  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 465: /* expr: "unsafe" '(' expr ')'  */
                                         {
            (yyvsp[-1].pExpression)->alwaysSafe = true;
            (yyvsp[-1].pExpression)->userSaidItsSafe = true;
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        }
    break;

  case 466: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 467: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 468: /* expr_generator: "generator" '<' type_declaration_no_options '>' optional_capture_list optional_semis expression_block  */
                                                                                                                                             {
        auto closure = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        ((ExprBlock *)(yyvsp[0].pExpression))->returnType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),closure,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 469: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 470: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 471: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 472: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 473: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 474: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 475: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 476: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 477: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 478: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 479: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 480: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 481: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 482: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 483: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 484: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 485: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 486: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 487: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 488: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 489: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 490: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 491: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 492: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 493: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 494: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 495: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 496: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 497: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 498: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 499: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 500: /* struct_variable_declaration_list: struct_variable_declaration_list $@33 structure_variable_declaration "end of expression"  */
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

  case 501: /* $@34: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 502: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@34 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 503: /* $@35: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 504: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@35 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 505: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 506: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 507: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 508: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 509: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 510: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 511: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 512: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 513: /* tuple_alias_type_list: %empty  */
      {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 514: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 515: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
                                                          {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
        /*
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,@decl);
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *($decl->pNameList) ) {
                    crd->afterVariantEntry(nl.name.c_str(), nl.at);
                }
            }
        }
        */
    }
    break;

  case 516: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 517: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 518: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 519: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 520: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 521: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
                                                                {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[-1].pVarDecl)->pNameList) ) {
                    crd->afterVariantEntry(nl.name.c_str(), nl.at);
                }
            }
        }
    }
    break;

  case 522: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 523: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 524: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 525: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 526: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 527: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 528: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 529: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 530: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 531: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 532: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 533: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 534: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 535: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 536: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 537: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 538: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 539: /* global_let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 540: /* global_let_variable_name_with_pos_list: global_let_variable_name_with_pos_list ',' "name"  */
                                                                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 541: /* variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 542: /* variable_declaration_list: variable_declaration_list "end of expression"  */
                                            {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 543: /* variable_declaration_list: variable_declaration_list let_variable_declaration  */
                                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
        (yyvsp[-1].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 544: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 545: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 546: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 547: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 548: /* global_let_variable_declaration: global_let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                         {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 549: /* global_let_variable_declaration: global_let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                                 {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 550: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 551: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 552: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 553: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 554: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 555: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 556: /* global_variable_declaration_list: global_variable_declaration_list "end of expression"  */
                                                   {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 557: /* $@36: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 558: /* global_variable_declaration_list: global_variable_declaration_list $@36 optional_field_annotation let_variable_declaration  */
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

  case 559: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 560: /* $@37: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 561: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@37 optional_field_annotation global_let_variable_declaration  */
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

  case 562: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        delete (yyvsp[0].s);
    }
    break;

  case 563: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
        delete (yyvsp[-2].s);
    }
    break;

  case 566: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 567: /* enum_list: enum_expression  */
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

  case 568: /* enum_list: enum_list commas enum_expression  */
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

  case 569: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 570: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 571: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 572: /* $@38: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 573: /* single_alias: optional_public_or_private_alias "name" $@38 '=' type_declaration  */
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

  case 575: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 576: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 577: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 578: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 579: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 580: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 583: /* $@39: %empty  */
                                                     {
        yyextra->das_indent_char = ',';
        yyextra->das_indent_level++;
    }
    break;

  case 584: /* $@40: %empty  */
                                                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 585: /* $@41: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        yyextra->das_indent_level--;
        yyextra->das_indent_char = ';';
    }
    break;

  case 586: /* enum_declaration: optional_annotation_list "enum" $@39 optional_public_or_private_enum enum_name optional_enum_basic_type_declaration "begin of code block" $@40 enum_list optional_commas $@41 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-11].faList),tokAt(scanner,(yylsp[-11])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-3].pEnum),(yyvsp[-6].type));
    }
    break;

  case 587: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 588: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 589: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 590: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 591: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 592: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 593: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 594: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 595: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 596: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 597: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 598: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 599: /* $@42: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 600: /* $@43: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 601: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@42 structure_name $@43 optional_struct_variable_declaration_list  */
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

  case 602: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 603: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 604: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 605: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 606: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 607: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 608: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 609: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 610: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 611: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 612: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 613: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 614: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 615: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 616: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 617: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 618: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 619: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 620: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 621: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 622: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 623: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 624: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 625: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 626: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 627: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 628: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 629: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 630: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 631: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 632: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 633: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 634: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 635: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 636: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 637: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 638: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 639: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 640: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 641: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 642: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 643: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 644: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 645: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 646: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 647: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 648: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 649: /* bitfield_alias_bits: "name"  */
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

  case 650: /* bitfield_alias_bits: bitfield_alias_bits ',' "name"  */
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

  case 651: /* $@44: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 652: /* $@45: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 653: /* bitfield_type_declaration: "bitfield" '<' $@44 bitfield_bits '>' $@45  */
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

  case 656: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 657: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 658: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 659: /* dim_list: '[' ']'  */
                {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 660: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 661: /* dim_list: dim_list '[' ']'  */
                              {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), nullptr);
    }
    break;

  case 662: /* type_declaration_no_options: type_declaration_no_options_no_dim  */
                                                     {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 663: /* type_declaration_no_options: type_declaration_no_options_no_dim dim_list  */
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

  case 664: /* type_declaration_no_options_no_dim: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 665: /* type_declaration_no_options_no_dim: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 666: /* type_declaration_no_options_no_dim: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 667: /* type_declaration_no_options_no_dim: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 668: /* $@46: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 669: /* $@47: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 670: /* type_declaration_no_options_no_dim: "type" '<' $@46 type_declaration '>' $@47  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 671: /* type_declaration_no_options_no_dim: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 672: /* type_declaration_no_options_no_dim: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 673: /* $@48: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 674: /* type_declaration_no_options_no_dim: '$' name_in_namespace '<' $@48 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 675: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '[' ']'  */
                                                                 {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 676: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "explicit"  */
                                                                  {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 677: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "const"  */
                                                               {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 678: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' "const"  */
                                                                   {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 679: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '&'  */
                                                         {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 680: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '&'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 681: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '#'  */
                                                         {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 682: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "implicit"  */
                                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 683: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '-' '#'  */
                                                             {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 684: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" "const"  */
                                                                      {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 685: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "==" '&'  */
                                                                {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 686: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim '?'  */
                                                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 687: /* $@49: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 688: /* $@50: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 689: /* type_declaration_no_options_no_dim: "smart_ptr" '<' $@49 type_declaration '>' $@50  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 690: /* type_declaration_no_options_no_dim: type_declaration_no_options_no_dim "??"  */
                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 691: /* $@51: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 692: /* $@52: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 693: /* type_declaration_no_options_no_dim: "array" '<' $@51 type_declaration '>' $@52  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 694: /* $@53: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 695: /* $@54: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 696: /* type_declaration_no_options_no_dim: "table" '<' $@53 table_type_pair '>' $@54  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 697: /* $@55: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 698: /* $@56: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 699: /* type_declaration_no_options_no_dim: "iterator" '<' $@55 type_declaration '>' $@56  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 700: /* type_declaration_no_options_no_dim: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 701: /* $@57: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 702: /* $@58: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 703: /* type_declaration_no_options_no_dim: "block" '<' $@57 type_declaration '>' $@58  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 704: /* $@59: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 705: /* $@60: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 706: /* type_declaration_no_options_no_dim: "block" '<' $@59 optional_function_argument_list optional_function_type '>' $@60  */
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

  case 707: /* type_declaration_no_options_no_dim: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 708: /* $@61: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 709: /* $@62: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 710: /* type_declaration_no_options_no_dim: "function" '<' $@61 type_declaration '>' $@62  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 711: /* $@63: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 712: /* $@64: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 713: /* type_declaration_no_options_no_dim: "function" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 714: /* type_declaration_no_options_no_dim: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 715: /* $@65: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 716: /* $@66: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 717: /* type_declaration_no_options_no_dim: "lambda" '<' $@65 type_declaration '>' $@66  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 718: /* $@67: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 719: /* $@68: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 720: /* type_declaration_no_options_no_dim: "lambda" '<' $@67 optional_function_argument_list optional_function_type '>' $@68  */
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

  case 721: /* $@69: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 722: /* $@70: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 723: /* type_declaration_no_options_no_dim: "tuple" '<' $@69 tuple_type_list '>' $@70  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 724: /* $@71: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 725: /* $@72: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 726: /* type_declaration_no_options_no_dim: "variant" '<' $@71 variant_type_list '>' $@72  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 727: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 728: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 729: /* type_declaration: type_declaration '|' '#'  */
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

  case 730: /* $@73: %empty  */
                                                                      {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 731: /* $@74: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 732: /* $@75: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 733: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias "name" $@73 "begin of code block" $@74 tuple_alias_type_list $@75 "end of code block"  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tTuple);
        vtype->alias = *(yyvsp[-6].s);
        vtype->at = tokAt(scanner,(yylsp[-6]));
        vtype->isPrivateAlias = !(yyvsp[-7].b);
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-6].s),tokAt(scanner,(yylsp[-6])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTuple((yyvsp[-6].s)->c_str(),atvname);
        }
        delete (yyvsp[-6].s);
    }
    break;

  case 734: /* $@76: %empty  */
                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 735: /* $@77: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 736: /* $@78: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 737: /* variant_alias_declaration: "variant" optional_public_or_private_alias "name" $@76 "begin of code block" $@77 variant_alias_type_list $@78 "end of code block"  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-6].s);
        vtype->at = tokAt(scanner,(yylsp[-6]));
        vtype->isPrivateAlias = !(yyvsp[-7].b);
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-6].s),tokAt(scanner,(yylsp[-6])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-6].s)->c_str(),atvname);
        }
        delete (yyvsp[-6].s);
    }
    break;

  case 738: /* $@79: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 739: /* $@80: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 740: /* $@81: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 741: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias "name" $@79 "begin of code block" $@80 bitfield_alias_bits optional_comma $@81 "end of code block"  */
          {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-7].s);
        btype->at = tokAt(scanner,(yylsp[-7]));
        btype->argNames = *(yyvsp[-3].pNameList);
        btype->isPrivateAlias = !(yyvsp[-8].b);
        if ( btype->argNames.size()>32 ) {
            das2_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-7])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-7].s),tokAt(scanner,(yylsp[-7])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-7].s)->c_str(),atvname);
        }
        delete (yyvsp[-7].s);
        delete (yyvsp[-3].pNameList);
    }
    break;

  case 742: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 743: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 744: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 745: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 746: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 747: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 748: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 749: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 750: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 751: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 752: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 753: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 754: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 755: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 756: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 757: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 758: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 759: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 760: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 761: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 762: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 763: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 764: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 765: /* $@82: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 766: /* $@83: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 767: /* make_struct_decl: "struct" '<' $@82 type_declaration_no_options '>' $@83 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 768: /* $@84: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 769: /* $@85: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 770: /* make_struct_decl: "class" '<' $@84 type_declaration_no_options '>' $@85 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 771: /* $@86: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 772: /* $@87: %empty  */
                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 773: /* make_struct_decl: "variant" '<' $@86 type_declaration_no_options '>' $@87 '(' make_variant_dim ')'  */
                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 774: /* $@88: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 775: /* $@89: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 776: /* make_struct_decl: "default" '<' $@88 type_declaration_no_options '>' $@89 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 777: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 778: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 779: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 780: /* $@90: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 781: /* $@91: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 782: /* make_tuple_call: "tuple" '<' $@90 type_declaration_no_options '>' $@91 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                      {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 783: /* make_dim_decl: '[' expr_list optional_comma ']'  */
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

  case 784: /* $@92: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 785: /* $@93: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 786: /* make_dim_decl: "array" "struct" '<' $@92 type_declaration_no_options '>' $@93 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 787: /* $@94: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 788: /* $@95: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 789: /* make_dim_decl: "array" "tuple" '<' $@94 type_declaration_no_options '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 790: /* $@96: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 791: /* $@97: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 792: /* make_dim_decl: "array" "variant" '<' $@96 type_declaration_no_options '>' $@97 '(' make_variant_dim ')'  */
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

  case 793: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 794: /* $@98: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 795: /* $@99: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 796: /* make_dim_decl: "array" '<' $@98 type_declaration_no_options '>' $@99 '(' optional_expr_list ')'  */
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

  case 797: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 798: /* $@100: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 799: /* $@101: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 800: /* make_dim_decl: "fixed_array" '<' $@100 type_declaration_no_options '>' $@101 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 801: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 802: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 803: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 804: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 805: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 806: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 807: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 808: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 809: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 810: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 811: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                           {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 812: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                        {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 813: /* array_comprehension: "begin of code block" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
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


