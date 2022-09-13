/* A Bison parser, made by GNU Bison 3.7.6.  */

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
#define YYBISON 30706

/* Bison version string.  */
#define YYBISON_VERSION "3.7.6"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         DAS_YYSTYPE
#define YYLTYPE         DAS_YYLTYPE
/* Substitute the variable and function names.  */
#define yyparse         das_yyparse
#define yylex           das_yylex
#define yyerror         das_yyerror
#define yydebug         das_yydebug
#define yynerrs         das_yynerrs

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

    union DAS_YYSTYPE;
    struct DAS_YYLTYPE;

    #define YY_NO_UNISTD_H
    #include "lex.yy.h"

    void das_yyerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error );
    void das_yyfatalerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr );
    int yylex ( DAS_YYSTYPE *lvalp, DAS_YYLTYPE *llocp, yyscan_t scanner );
    void yybegin ( const char * str );

    void das_yybegin_reader ( yyscan_t yyscanner );
    void das_yyend_reader ( yyscan_t yyscanner );

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

#include "ds_parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_LEXER_ERROR = 3,                /* "lexer error"  */
  YYSYMBOL_DAS_STRUCT = 4,                 /* "struct"  */
  YYSYMBOL_DAS_CLASS = 5,                  /* "class"  */
  YYSYMBOL_DAS_LET = 6,                    /* "let"  */
  YYSYMBOL_DAS_DEF = 7,                    /* "def"  */
  YYSYMBOL_DAS_WHILE = 8,                  /* "while"  */
  YYSYMBOL_DAS_IF = 9,                     /* "if"  */
  YYSYMBOL_DAS_STATIC_IF = 10,             /* "static_if"  */
  YYSYMBOL_DAS_ELSE = 11,                  /* "else"  */
  YYSYMBOL_DAS_FOR = 12,                   /* "for"  */
  YYSYMBOL_DAS_CATCH = 13,                 /* "recover"  */
  YYSYMBOL_DAS_TRUE = 14,                  /* "true"  */
  YYSYMBOL_DAS_FALSE = 15,                 /* "false"  */
  YYSYMBOL_DAS_NEWT = 16,                  /* "new"  */
  YYSYMBOL_DAS_TYPEINFO = 17,              /* "typeinfo"  */
  YYSYMBOL_DAS_TYPE = 18,                  /* "type"  */
  YYSYMBOL_DAS_IN = 19,                    /* "in"  */
  YYSYMBOL_DAS_IS = 20,                    /* "is"  */
  YYSYMBOL_DAS_AS = 21,                    /* "as"  */
  YYSYMBOL_DAS_ELIF = 22,                  /* "elif"  */
  YYSYMBOL_DAS_STATIC_ELIF = 23,           /* "static_elif"  */
  YYSYMBOL_DAS_ARRAY = 24,                 /* "array"  */
  YYSYMBOL_DAS_RETURN = 25,                /* "return"  */
  YYSYMBOL_DAS_NULL = 26,                  /* "null"  */
  YYSYMBOL_DAS_BREAK = 27,                 /* "break"  */
  YYSYMBOL_DAS_TRY = 28,                   /* "try"  */
  YYSYMBOL_DAS_OPTIONS = 29,               /* "options"  */
  YYSYMBOL_DAS_TABLE = 30,                 /* "table"  */
  YYSYMBOL_DAS_EXPECT = 31,                /* "expect"  */
  YYSYMBOL_DAS_CONST = 32,                 /* "const"  */
  YYSYMBOL_DAS_REQUIRE = 33,               /* "require"  */
  YYSYMBOL_DAS_OPERATOR = 34,              /* "operator"  */
  YYSYMBOL_DAS_ENUM = 35,                  /* "enum"  */
  YYSYMBOL_DAS_FINALLY = 36,               /* "finally"  */
  YYSYMBOL_DAS_DELETE = 37,                /* "delete"  */
  YYSYMBOL_DAS_DEREF = 38,                 /* "deref"  */
  YYSYMBOL_DAS_TYPEDEF = 39,               /* "typedef"  */
  YYSYMBOL_DAS_WITH = 40,                  /* "with"  */
  YYSYMBOL_DAS_AKA = 41,                   /* "aka"  */
  YYSYMBOL_DAS_ASSUME = 42,                /* "assume"  */
  YYSYMBOL_DAS_CAST = 43,                  /* "cast"  */
  YYSYMBOL_DAS_OVERRIDE = 44,              /* "override"  */
  YYSYMBOL_DAS_ABSTRACT = 45,              /* "abstract"  */
  YYSYMBOL_DAS_UPCAST = 46,                /* "upcast"  */
  YYSYMBOL_DAS_ITERATOR = 47,              /* "iterator"  */
  YYSYMBOL_DAS_VAR = 48,                   /* "var"  */
  YYSYMBOL_DAS_ADDR = 49,                  /* "addr"  */
  YYSYMBOL_DAS_CONTINUE = 50,              /* "continue"  */
  YYSYMBOL_DAS_WHERE = 51,                 /* "where"  */
  YYSYMBOL_DAS_PASS = 52,                  /* "pass"  */
  YYSYMBOL_DAS_REINTERPRET = 53,           /* "reinterpret"  */
  YYSYMBOL_DAS_MODULE = 54,                /* "module"  */
  YYSYMBOL_DAS_PUBLIC = 55,                /* "public"  */
  YYSYMBOL_DAS_LABEL = 56,                 /* "label"  */
  YYSYMBOL_DAS_GOTO = 57,                  /* "goto"  */
  YYSYMBOL_DAS_IMPLICIT = 58,              /* "implicit"  */
  YYSYMBOL_DAS_EXPLICIT = 59,              /* "explicit"  */
  YYSYMBOL_DAS_SHARED = 60,                /* "shared"  */
  YYSYMBOL_DAS_PRIVATE = 61,               /* "private"  */
  YYSYMBOL_DAS_SMART_PTR = 62,             /* "smart_ptr"  */
  YYSYMBOL_DAS_UNSAFE = 63,                /* "unsafe"  */
  YYSYMBOL_DAS_TBOOL = 64,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 65,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 66,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 67,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 68,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 69,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 70,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 71,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 72,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 73,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 74,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 75,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 76,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 77,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 78,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 79,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 80,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 81,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 82,               /* "urange"  */
  YYSYMBOL_DAS_TBLOCK = 83,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 84,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 85,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 86,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 87,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 88,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 89,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 90,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 91,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 92,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 93,                /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 94,              /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 95,             /* "generator"  */
  YYSYMBOL_DAS_YIELD = 96,                 /* "yield"  */
  YYSYMBOL_DAS_SEALED = 97,                /* "sealed"  */
  YYSYMBOL_ADDEQU = 98,                    /* "+="  */
  YYSYMBOL_SUBEQU = 99,                    /* "-="  */
  YYSYMBOL_DIVEQU = 100,                   /* "/="  */
  YYSYMBOL_MULEQU = 101,                   /* "*="  */
  YYSYMBOL_MODEQU = 102,                   /* "%="  */
  YYSYMBOL_ANDEQU = 103,                   /* "&="  */
  YYSYMBOL_OREQU = 104,                    /* "|="  */
  YYSYMBOL_XOREQU = 105,                   /* "^="  */
  YYSYMBOL_SHL = 106,                      /* "<<"  */
  YYSYMBOL_SHR = 107,                      /* ">>"  */
  YYSYMBOL_ADDADD = 108,                   /* "++"  */
  YYSYMBOL_SUBSUB = 109,                   /* "--"  */
  YYSYMBOL_LEEQU = 110,                    /* "<="  */
  YYSYMBOL_SHLEQU = 111,                   /* "<<="  */
  YYSYMBOL_SHREQU = 112,                   /* ">>="  */
  YYSYMBOL_GREQU = 113,                    /* ">="  */
  YYSYMBOL_EQUEQU = 114,                   /* "=="  */
  YYSYMBOL_NOTEQU = 115,                   /* "!="  */
  YYSYMBOL_RARROW = 116,                   /* "->"  */
  YYSYMBOL_LARROW = 117,                   /* "<-"  */
  YYSYMBOL_QQ = 118,                       /* "??"  */
  YYSYMBOL_QDOT = 119,                     /* "?."  */
  YYSYMBOL_QBRA = 120,                     /* "?["  */
  YYSYMBOL_LPIPE = 121,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 122,                   /* " <|"  */
  YYSYMBOL_LAPIPE = 123,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 124,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 125,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 126,                 /* ":="  */
  YYSYMBOL_ROTL = 127,                     /* "<<<"  */
  YYSYMBOL_ROTR = 128,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 129,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 130,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 131,                    /* "=>"  */
  YYSYMBOL_COLCOL = 132,                   /* "::"  */
  YYSYMBOL_ANDAND = 133,                   /* "&&"  */
  YYSYMBOL_OROR = 134,                     /* "||"  */
  YYSYMBOL_XORXOR = 135,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 136,                /* "&&="  */
  YYSYMBOL_OROREQU = 137,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 138,                /* "^^="  */
  YYSYMBOL_DOTDOT = 139,                   /* ".."  */
  YYSYMBOL_MTAG_E = 140,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 141,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 142,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 143,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 144,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 145,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 146,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 147,                   /* "$f"  */
  YYSYMBOL_BRABRAB = 148,                  /* "[["  */
  YYSYMBOL_BRACBRB = 149,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 150,                  /* "{{"  */
  YYSYMBOL_INTEGER = 151,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 152,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 153,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 154,    /* "unsigned long integer constant"  */
  YYSYMBOL_FLOAT = 155,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 156,                   /* "double constant"  */
  YYSYMBOL_NAME = 157,                     /* "name"  */
  YYSYMBOL_BEGIN_STRING = 158,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 159,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 160,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 161,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 162,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 163,          /* "}"  */
  YYSYMBOL_END_OF_READ = 164,              /* "end of failed eader macro"  */
  YYSYMBOL_165_ = 165,                     /* ','  */
  YYSYMBOL_166_ = 166,                     /* '='  */
  YYSYMBOL_167_ = 167,                     /* '?'  */
  YYSYMBOL_168_ = 168,                     /* ':'  */
  YYSYMBOL_169_ = 169,                     /* '|'  */
  YYSYMBOL_170_ = 170,                     /* '^'  */
  YYSYMBOL_171_ = 171,                     /* '&'  */
  YYSYMBOL_172_ = 172,                     /* '<'  */
  YYSYMBOL_173_ = 173,                     /* '>'  */
  YYSYMBOL_174_ = 174,                     /* '-'  */
  YYSYMBOL_175_ = 175,                     /* '+'  */
  YYSYMBOL_176_ = 176,                     /* '*'  */
  YYSYMBOL_177_ = 177,                     /* '/'  */
  YYSYMBOL_178_ = 178,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 179,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 180,               /* UNARY_PLUS  */
  YYSYMBOL_181_ = 181,                     /* '~'  */
  YYSYMBOL_182_ = 182,                     /* '!'  */
  YYSYMBOL_PRE_INC = 183,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 184,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 185,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 186,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 187,                    /* DEREF  */
  YYSYMBOL_188_ = 188,                     /* '.'  */
  YYSYMBOL_189_ = 189,                     /* '['  */
  YYSYMBOL_190_ = 190,                     /* ']'  */
  YYSYMBOL_191_ = 191,                     /* '('  */
  YYSYMBOL_192_ = 192,                     /* ')'  */
  YYSYMBOL_193_ = 193,                     /* '$'  */
  YYSYMBOL_194_ = 194,                     /* '@'  */
  YYSYMBOL_195_ = 195,                     /* ';'  */
  YYSYMBOL_196_ = 196,                     /* '{'  */
  YYSYMBOL_197_ = 197,                     /* '}'  */
  YYSYMBOL_198_ = 198,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 199,                 /* $accept  */
  YYSYMBOL_program = 200,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 201, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 202,              /* module_name  */
  YYSYMBOL_module_declaration = 203,       /* module_declaration  */
  YYSYMBOL_character_sequence = 204,       /* character_sequence  */
  YYSYMBOL_string_constant = 205,          /* string_constant  */
  YYSYMBOL_string_builder_body = 206,      /* string_builder_body  */
  YYSYMBOL_string_builder = 207,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 208, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 209,              /* expr_reader  */
  YYSYMBOL_210_1 = 210,                    /* $@1  */
  YYSYMBOL_options_declaration = 211,      /* options_declaration  */
  YYSYMBOL_require_declaration = 212,      /* require_declaration  */
  YYSYMBOL_require_module_name = 213,      /* require_module_name  */
  YYSYMBOL_require_module = 214,           /* require_module  */
  YYSYMBOL_is_public_module = 215,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 216,       /* expect_declaration  */
  YYSYMBOL_expect_list = 217,              /* expect_list  */
  YYSYMBOL_expect_error = 218,             /* expect_error  */
  YYSYMBOL_expression_label = 219,         /* expression_label  */
  YYSYMBOL_expression_goto = 220,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 221,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 222,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 223,          /* if_or_static_if  */
  YYSYMBOL_expression_if_then_else = 224,  /* expression_if_then_else  */
  YYSYMBOL_expression_for_loop = 225,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 226,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 227,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 228,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 229,    /* expression_with_alias  */
  YYSYMBOL_230_2 = 230,                    /* $@2  */
  YYSYMBOL_annotation_argument_value = 231, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 232, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 233, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 234,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 235, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 236, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration = 237,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 238,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 239, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 240, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 241,   /* optional_function_type  */
  YYSYMBOL_function_name = 242,            /* function_name  */
  YYSYMBOL_global_function_declaration = 243, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 244, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 245, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 246,     /* function_declaration  */
  YYSYMBOL_247_3 = 247,                    /* $@3  */
  YYSYMBOL_expression_block = 248,         /* expression_block  */
  YYSYMBOL_expression_any = 249,           /* expression_any  */
  YYSYMBOL_expressions = 250,              /* expressions  */
  YYSYMBOL_expr_pipe = 251,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 252,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 253,        /* expression_delete  */
  YYSYMBOL_expr_new = 254,                 /* expr_new  */
  YYSYMBOL_expression_break = 255,         /* expression_break  */
  YYSYMBOL_expression_continue = 256,      /* expression_continue  */
  YYSYMBOL_expression_return = 257,        /* expression_return  */
  YYSYMBOL_258_4 = 258,                    /* $@4  */
  YYSYMBOL_expression_yield = 259,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 260,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 261,                  /* kwd_let  */
  YYSYMBOL_expression_let = 262,           /* expression_let  */
  YYSYMBOL_expr_cast = 263,                /* expr_cast  */
  YYSYMBOL_264_5 = 264,                    /* $@5  */
  YYSYMBOL_265_6 = 265,                    /* $@6  */
  YYSYMBOL_266_7 = 266,                    /* $@7  */
  YYSYMBOL_267_8 = 267,                    /* $@8  */
  YYSYMBOL_268_9 = 268,                    /* $@9  */
  YYSYMBOL_269_10 = 269,                   /* $@10  */
  YYSYMBOL_expr_type_decl = 270,           /* expr_type_decl  */
  YYSYMBOL_271_11 = 271,                   /* $@11  */
  YYSYMBOL_272_12 = 272,                   /* $@12  */
  YYSYMBOL_expr_type_info = 273,           /* expr_type_info  */
  YYSYMBOL_expr_list = 274,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 275,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 276,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 277,            /* capture_entry  */
  YYSYMBOL_capture_list = 278,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 279,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 280,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 281,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 282,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 283,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 284,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 285,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 286,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 287,           /* func_addr_expr  */
  YYSYMBOL_288_13 = 288,                   /* $@13  */
  YYSYMBOL_289_14 = 289,                   /* $@14  */
  YYSYMBOL_290_15 = 290,                   /* $@15  */
  YYSYMBOL_291_16 = 291,                   /* $@16  */
  YYSYMBOL_expr_field = 292,               /* expr_field  */
  YYSYMBOL_293_17 = 293,                   /* $@17  */
  YYSYMBOL_294_18 = 294,                   /* $@18  */
  YYSYMBOL_expr = 295,                     /* expr  */
  YYSYMBOL_296_19 = 296,                   /* $@19  */
  YYSYMBOL_297_20 = 297,                   /* $@20  */
  YYSYMBOL_298_21 = 298,                   /* $@21  */
  YYSYMBOL_299_22 = 299,                   /* $@22  */
  YYSYMBOL_300_23 = 300,                   /* $@23  */
  YYSYMBOL_301_24 = 301,                   /* $@24  */
  YYSYMBOL_expr_mtag = 302,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 303, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 304,        /* optional_override  */
  YYSYMBOL_optional_constant = 305,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 306, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 307, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 308, /* struct_variable_declaration_list  */
  YYSYMBOL_309_25 = 309,                   /* $@25  */
  YYSYMBOL_310_26 = 310,                   /* $@26  */
  YYSYMBOL_311_27 = 311,                   /* $@27  */
  YYSYMBOL_function_argument_declaration = 312, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 313,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 314,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 315,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 316,             /* variant_type  */
  YYSYMBOL_variant_type_list = 317,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 318,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 319,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 320,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 321,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 322, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 323, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 324, /* global_variable_declaration_list  */
  YYSYMBOL_325_28 = 325,                   /* $@28  */
  YYSYMBOL_optional_shared = 326,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 327, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 328,               /* global_let  */
  YYSYMBOL_329_29 = 329,                   /* $@29  */
  YYSYMBOL_enum_list = 330,                /* enum_list  */
  YYSYMBOL_single_alias = 331,             /* single_alias  */
  YYSYMBOL_332_30 = 332,                   /* $@30  */
  YYSYMBOL_alias_list = 333,               /* alias_list  */
  YYSYMBOL_alias_declaration = 334,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 335, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 336,                /* enum_name  */
  YYSYMBOL_enum_declaration = 337,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 338, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 339,          /* optional_sealed  */
  YYSYMBOL_structure_name = 340,           /* structure_name  */
  YYSYMBOL_class_or_struct = 341,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 342, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 343,    /* structure_declaration  */
  YYSYMBOL_344_31 = 344,                   /* $@31  */
  YYSYMBOL_345_32 = 345,                   /* $@32  */
  YYSYMBOL_variable_name_with_pos_list = 346, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 347,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 348, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 349, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 350,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 351,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 352, /* bitfield_type_declaration  */
  YYSYMBOL_353_33 = 353,                   /* $@33  */
  YYSYMBOL_354_34 = 354,                   /* $@34  */
  YYSYMBOL_table_type_pair = 355,          /* table_type_pair  */
  YYSYMBOL_type_declaration = 356,         /* type_declaration  */
  YYSYMBOL_357_35 = 357,                   /* $@35  */
  YYSYMBOL_358_36 = 358,                   /* $@36  */
  YYSYMBOL_359_37 = 359,                   /* $@37  */
  YYSYMBOL_360_38 = 360,                   /* $@38  */
  YYSYMBOL_361_39 = 361,                   /* $@39  */
  YYSYMBOL_362_40 = 362,                   /* $@40  */
  YYSYMBOL_363_41 = 363,                   /* $@41  */
  YYSYMBOL_364_42 = 364,                   /* $@42  */
  YYSYMBOL_365_43 = 365,                   /* $@43  */
  YYSYMBOL_366_44 = 366,                   /* $@44  */
  YYSYMBOL_367_45 = 367,                   /* $@45  */
  YYSYMBOL_368_46 = 368,                   /* $@46  */
  YYSYMBOL_369_47 = 369,                   /* $@47  */
  YYSYMBOL_370_48 = 370,                   /* $@48  */
  YYSYMBOL_371_49 = 371,                   /* $@49  */
  YYSYMBOL_372_50 = 372,                   /* $@50  */
  YYSYMBOL_373_51 = 373,                   /* $@51  */
  YYSYMBOL_374_52 = 374,                   /* $@52  */
  YYSYMBOL_375_53 = 375,                   /* $@53  */
  YYSYMBOL_376_54 = 376,                   /* $@54  */
  YYSYMBOL_377_55 = 377,                   /* $@55  */
  YYSYMBOL_378_56 = 378,                   /* $@56  */
  YYSYMBOL_379_57 = 379,                   /* $@57  */
  YYSYMBOL_380_58 = 380,                   /* $@58  */
  YYSYMBOL_variant_alias_declaration = 381, /* variant_alias_declaration  */
  YYSYMBOL_382_59 = 382,                   /* $@59  */
  YYSYMBOL_383_60 = 383,                   /* $@60  */
  YYSYMBOL_bitfield_alias_declaration = 384, /* bitfield_alias_declaration  */
  YYSYMBOL_385_61 = 385,                   /* $@61  */
  YYSYMBOL_make_decl = 386,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 387,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 388,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 389,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 390,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 391,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 392,           /* make_map_tuple  */
  YYSYMBOL_make_any_tuple = 393,           /* make_any_tuple  */
  YYSYMBOL_make_dim = 394,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 395,            /* make_dim_decl  */
  YYSYMBOL_make_table = 396,               /* make_table  */
  YYSYMBOL_make_table_decl = 397,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 398, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 399       /* array_comprehension  */
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

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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
         || (defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL \
             && defined DAS_YYSTYPE_IS_TRIVIAL && DAS_YYSTYPE_IS_TRIVIAL)))

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
#define YYLAST   10657

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  199
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  201
/* YYNRULES -- Number of rules.  */
#define YYNRULES  653
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1155

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   426


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
       2,     2,     2,   182,     2,   198,   193,   178,   171,     2,
     191,   192,   176,   175,   165,   174,   188,   177,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   168,   195,
     172,   166,   173,   167,   194,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   189,     2,   190,   170,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   196,   169,   197,   181,     2,     2,     2,
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
     179,   180,   183,   184,   185,   186,   187
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   470,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   486,   487,   488,   492,   493,
     497,   515,   516,   517,   518,   522,   526,   531,   540,   548,
     564,   569,   577,   577,   607,   628,   632,   635,   639,   645,
     654,   657,   663,   664,   668,   672,   673,   677,   680,   686,
     692,   695,   701,   702,   706,   707,   708,   717,   718,   722,
     731,   737,   745,   755,   764,   764,   771,   772,   773,   774,
     775,   776,   780,   785,   793,   794,   795,   799,   800,   801,
     802,   803,   804,   805,   806,   812,   815,   821,   822,   823,
     827,   835,   848,   852,   859,   860,   864,   865,   866,   870,
     873,   880,   884,   885,   886,   887,   888,   889,   890,   891,
     892,   893,   894,   895,   896,   897,   898,   899,   900,   901,
     902,   903,   904,   905,   906,   907,   908,   909,   910,   911,
     912,   913,   914,   915,   916,   917,   918,   919,   920,   921,
     922,   923,   924,   925,   926,   927,   928,   929,   930,   931,
     932,   933,   934,   935,   936,   937,   938,   939,   940,   941,
     942,   943,   944,   945,   946,   947,   948,   949,   950,   951,
     952,   953,   954,   955,   956,   957,   958,   959,   964,   986,
     987,   988,   992,   998,   998,  1015,  1019,  1030,  1031,  1032,
    1033,  1034,  1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,
    1043,  1044,  1045,  1046,  1047,  1048,  1049,  1053,  1058,  1064,
    1070,  1104,  1107,  1113,  1114,  1125,  1129,  1135,  1138,  1141,
    1145,  1151,  1155,  1159,  1162,  1165,  1170,  1173,  1178,  1178,
    1185,  1188,  1193,  1196,  1204,  1210,  1211,  1215,  1221,  1221,
    1221,  1224,  1224,  1224,  1229,  1229,  1229,  1237,  1237,  1237,
    1243,  1253,  1264,  1279,  1282,  1288,  1289,  1296,  1307,  1308,
    1309,  1313,  1314,  1315,  1316,  1320,  1325,  1333,  1334,  1338,
    1343,  1350,  1351,  1352,  1353,  1354,  1355,  1359,  1360,  1361,
    1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,  1371,
    1372,  1373,  1374,  1375,  1376,  1377,  1381,  1382,  1383,  1384,
    1388,  1399,  1404,  1414,  1418,  1425,  1428,  1428,  1428,  1433,
    1433,  1433,  1446,  1450,  1454,  1454,  1454,  1461,  1462,  1463,
    1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,
    1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,  1483,
    1484,  1485,  1486,  1487,  1488,  1489,  1490,  1491,  1492,  1493,
    1494,  1495,  1496,  1502,  1503,  1504,  1505,  1506,  1507,  1508,
    1509,  1510,  1511,  1512,  1513,  1514,  1518,  1522,  1525,  1528,
    1529,  1530,  1531,  1534,  1537,  1538,  1541,  1541,  1541,  1544,
    1549,  1553,  1557,  1557,  1557,  1562,  1565,  1569,  1569,  1569,
    1574,  1577,  1578,  1579,  1580,  1581,  1582,  1583,  1584,  1585,
    1586,  1587,  1595,  1596,  1597,  1598,  1599,  1600,  1604,  1608,
    1612,  1616,  1620,  1624,  1628,  1632,  1636,  1643,  1644,  1648,
    1649,  1650,  1654,  1655,  1659,  1660,  1661,  1665,  1675,  1678,
    1678,  1697,  1696,  1711,  1710,  1723,  1732,  1737,  1742,  1752,
    1753,  1757,  1760,  1769,  1770,  1774,  1783,  1784,  1789,  1790,
    1794,  1800,  1806,  1809,  1813,  1819,  1828,  1829,  1830,  1834,
    1835,  1839,  1846,  1851,  1860,  1866,  1877,  1880,  1885,  1890,
    1898,  1909,  1912,  1912,  1932,  1933,  1937,  1938,  1939,  1943,
    1946,  1946,  1965,  1968,  1977,  1990,  1990,  2011,  2012,  2016,
    2017,  2021,  2022,  2023,  2027,  2037,  2044,  2054,  2055,  2059,
    2060,  2064,  2070,  2071,  2075,  2076,  2077,  2081,  2086,  2081,
    2096,  2103,  2108,  2117,  2123,  2134,  2135,  2136,  2137,  2138,
    2139,  2140,  2141,  2142,  2143,  2144,  2145,  2146,  2147,  2148,
    2149,  2150,  2151,  2152,  2153,  2154,  2155,  2156,  2157,  2158,
    2162,  2163,  2164,  2165,  2166,  2167,  2171,  2182,  2186,  2193,
    2205,  2212,  2221,  2221,  2221,  2234,  2238,  2245,  2246,  2247,
    2248,  2249,  2263,  2269,  2273,  2277,  2282,  2287,  2292,  2297,
    2301,  2305,  2310,  2314,  2319,  2319,  2319,  2325,  2332,  2332,
    2332,  2337,  2337,  2337,  2343,  2343,  2343,  2348,  2352,  2352,
    2352,  2357,  2357,  2357,  2366,  2370,  2370,  2370,  2375,  2375,
    2375,  2384,  2388,  2388,  2388,  2393,  2393,  2393,  2402,  2402,
    2402,  2408,  2408,  2408,  2417,  2417,  2417,  2441,  2441,  2461,
    2462,  2463,  2464,  2468,  2475,  2482,  2488,  2497,  2502,  2509,
    2510,  2514,  2520,  2527,  2535,  2542,  2550,  2562,  2565,  2579,
    2585,  2591,  2592,  2596,  2601,  2608,  2613,  2623,  2628,  2635,
    2647,  2648,  2652,  2655
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
  "\"struct\"", "\"class\"", "\"let\"", "\"def\"", "\"while\"", "\"if\"",
  "\"static_if\"", "\"else\"", "\"for\"", "\"recover\"", "\"true\"",
  "\"false\"", "\"new\"", "\"typeinfo\"", "\"type\"", "\"in\"", "\"is\"",
  "\"as\"", "\"elif\"", "\"static_elif\"", "\"array\"", "\"return\"",
  "\"null\"", "\"break\"", "\"try\"", "\"options\"", "\"table\"",
  "\"expect\"", "\"const\"", "\"require\"", "\"operator\"", "\"enum\"",
  "\"finally\"", "\"delete\"", "\"deref\"", "\"typedef\"", "\"with\"",
  "\"aka\"", "\"assume\"", "\"cast\"", "\"override\"", "\"abstract\"",
  "\"upcast\"", "\"iterator\"", "\"var\"", "\"addr\"", "\"continue\"",
  "\"where\"", "\"pass\"", "\"reinterpret\"", "\"module\"", "\"public\"",
  "\"label\"", "\"goto\"", "\"implicit\"", "\"explicit\"", "\"shared\"",
  "\"private\"", "\"smart_ptr\"", "\"unsafe\"", "\"bool\"", "\"void\"",
  "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"",
  "\"uint\"", "\"bitfield\"", "\"uint2\"", "\"uint3\"", "\"uint4\"",
  "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"",
  "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"",
  "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"",
  "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"",
  "\"sealed\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"",
  "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"",
  "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"",
  "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"",
  "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"",
  "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"",
  "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"start of the string\"", "STRING_CHARACTER", "STRING_CHARACTER_ESC",
  "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "POST_INC", "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('", "')'",
  "'$'", "'@'", "';'", "'{'", "'}'", "'#'", "$accept", "program",
  "optional_public_or_private_module", "module_name", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "reader_character_sequence", "expr_reader", "$@1",
  "options_declaration", "require_declaration", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "expression_else", "if_or_static_if",
  "expression_if_then_else", "expression_for_loop", "expression_unsafe",
  "expression_while_loop", "expression_with", "expression_with_alias",
  "$@2", "annotation_argument_value", "annotation_argument_value_list",
  "annotation_argument_name", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@3", "expression_block", "expression_any",
  "expressions", "expr_pipe", "name_in_namespace", "expression_delete",
  "expr_new", "expression_break", "expression_continue",
  "expression_return", "$@4", "expression_yield", "expression_try_catch",
  "kwd_let", "expression_let", "expr_cast", "$@5", "$@6", "$@7", "$@8",
  "$@9", "$@10", "expr_type_decl", "$@11", "$@12", "expr_type_info",
  "expr_list", "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@13", "$@14", "$@15", "$@16", "expr_field", "$@17",
  "$@18", "expr", "$@19", "$@20", "$@21", "$@22", "$@23", "$@24",
  "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@25", "$@26", "$@27", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@28", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@29", "enum_list",
  "single_alias", "$@30", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_name", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@31", "$@32", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@33", "$@34", "table_type_pair",
  "type_declaration", "$@35", "$@36", "$@37", "$@38", "$@39", "$@40",
  "$@41", "$@42", "$@43", "$@44", "$@45", "$@46", "$@47", "$@48", "$@49",
  "$@50", "$@51", "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58",
  "variant_alias_declaration", "$@59", "$@60",
  "bitfield_alias_declaration", "$@61", "make_decl", "make_struct_fields",
  "make_struct_dim", "optional_block", "make_struct_decl", "make_tuple",
  "make_map_tuple", "make_any_tuple", "make_dim", "make_dim_decl",
  "make_table", "make_table_decl", "array_comprehension_where",
  "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,    44,    61,    63,    58,   124,
      94,    38,    60,    62,    45,    43,    42,    47,    37,   420,
     421,   126,    33,   422,   423,   424,   425,   426,    46,    91,
      93,    40,    41,    36,    64,    59,   123,   125,    35
};
#endif

#define YYPACT_NINF (-895)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-606)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -895,    20,  -895,  -895,    19,   -65,   -18,   -99,  -895,   -76,
    -895,  -895,    75,  -895,  -895,  -895,  -895,  -895,   256,  -895,
      85,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,
      70,  -895,   161,   126,   180,  -895,  -895,   -18,    22,  -895,
    -895,   128,  -895,  -895,  -895,    85,   242,   272,  -895,  -895,
     277,   293,   267,  -895,   116,  -895,  -895,  -895,    27,   343,
     356,  -895,   359,    17,    19,   319,   -65,   295,   340,  -895,
     341,   344,  -895,   323,   305,   -97,   363,   307,  -895,  -895,
     347,    19,    75,  -895,  -895,  -895,  -895,  -895,  -895,  -895,
     348,  -895,  -895,  -895,  -895,  -895,   310,  -895,  -895,  -895,
    -895,  -895,   288,    97,  -895,  -895,  -895,  -895,   452,  -895,
    -895, 10322,  -895,  -895,   313,  -895,  -895,  -895,   352,   314,
    -895,   105,  -895, 10500,  -895,    29,   414,  -895,   364,  -895,
    -895,   332,  -895,  -895,  -895,  -895,  -895,  -895,  -895,   112,
    -895,   342,   345,   346,   349,  -895,  -895,  -895,   325,  -895,
    -895,  -895,  -895,  -895,   354,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,   355,  -895,  -895,  -895,   357,   361,
    -895,  -895,  -895,  -895,   365,   366,   328,  -895,  -895,  -895,
    -895,  -895,  3385,  -895,  -895,   333,   374,  -895,  8962,  -895,
    -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,   498,   502,  -895,   358,   350,   360,  -895,  -895,
     383,  -895,   351,    19,   143,  -895,  -895,  -895,    97,  -895,
    -895,  -895,  -895,  -895,   386,  -895,   232,   240,   286,  -895,
    -895,  5322,  -895,  -895,  -895,  -895,   521,  -895,  -895,    41,
    3386,  -895,   -95,   388,  -895,   362,  -895,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,   369,
    -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,   533,  -895,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,  -895,  -895,   370,  -895,  -895,   -87,
     396,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,   372,
     -93,   397,   373,  -895,   364,   138,   375,   529,   244,  -895,
    -895, 10322, 10322, 10322, 10322,   379,   352, 10322,   358, 10322,
     358, 10322,   358, 10416,   374,  -895,  -895, 10228,   381,   412,
    -895,   398,   418,   419,   402,   426,   408,  -895,   429,  5322,
    5322,   411,   413,   415,   416,   417,   421,  9919, 10013,  5322,
    -895,  -895,  -895,  -895,  -895,  -895,  -895,  5322,  5322,  5322,
     181,  5322,  5322,  5322,  -895,   409,  -895,  -895,  -895,   -14,
    -895,  -895,  -895,  -895,   420,  -895,  -895,  -895,  -895,  -895,
    -895,    13,  -895,   423,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,   425,  -895,  -895,  8290,  -895,  -895, 10322,   -82,
    -895,  -895,  -895,   428,  -895,    -6,  -895,   274, 10322,  -895,
    1603,  -895,    47,  -895,   181,  -895,  -895,   143,   427,  5322,
     448,   453, 10322,  -895,    53,    98,   443,   301,  1392,  1543,
    -895,  -101,  1733,   396,  1902,   396,  2073,   396,   -26,  -895,
     119,  3385,   129,  1320,  -895,   181,  -895,  5322,  -895,  -895,
    5322,  -895,  5322, 10322,   442,   442,  5322,  5322,  5322,  5322,
    5322,  5322,   166,  1972,   166,  2141,  9058,  -895,   174,   320,
     442,   442,   -79,  -895,   442,   442,  5913,   -56,  -895,  2779,
     474,  9130, 10044,  5322,  5322,  -895,  -895,  5322,  5322,  5322,
    5322,   468,  5322,   -68,  5322,  5322,  5322,  5322,  5322,  5322,
    5322,  5322,  5322,  3532,  5322,  5322,  5322,  5322,  5322,  5322,
    5322,  5322,  5322,  5322,   154,  5322,  -895,  3678,  -895,  -895,
    3385,  -895,  -895,  5322,   166,   433,   586,  -895,   182,  -895,
     -61,  3385,  -895,  5322,  -895,  -895,   166,  2479,  -895,   350,
    5322,  5322,   471,  -895,   434,   481,  3847,   -33,  2633,   280,
     280,  -895,   597,   439,   440,  5322,  -895,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,   451,   454,   455,  -895,  -895,  -895,
     143,  -895,  -100,  -895,  8859,    81,  5322,  -895,  -895,    14,
    -895,  -895,  6015,  -895,   606,  1326,  -895,  -895,  -895,  2948,
    -895,  -895, 10322,  -895,  -895,  -895,   494,  -895,   479,  -895,
     480,  -895,   484, 10322,  -895, 10416,  -895,   374,  3993,  4162,
   10322,  6106, 10322, 10322,  6197, 10322,  6288,  2242,  6379,  6470,
    6561,  6652,  6743,  6834,    16,   280,   -48,  2310,  4331,  9058,
     489,   -22,   469,   493,  -895,  -895,   231,    28,  4500,   -22,
     289,  5322,  5322,   463,  -895,  5322,   335,   503,  -895,   470,
     472,   287,  -895,  -895,   513,  -895,   117,  9296,   124,   358,
     508,   490,  -895,  -895,   510,   496,  -895,  -895,   951,   951,
     835,   835,  5990,  5990,   497,    46,   499,  -895,  8381,   -64,
     -64,   951,   951,   467,  9490,  9567,  9380, 10134,  9135,  9599,
    9710,  9787,   835,   835,  1136,  1136,    46,    46,    46,     2,
    5322,   504,  -895,   136,  5322,   674,  8472,  -895,   147,  6925,
    -895,  5322,   527,  -895,   532,  -895, 10322,  -895,  2948,  -895,
     268,    31,  2948,  -895,  -895,   572,   691,   683,  9296,   268,
     536,  -895,   530,   548,  9296,  -895,  2948,  -895,  1096,   509,
    -895,  -895,   511,  -895,  -895,   268,  -895,  -895,  -895,  -895,
     280,  -895,  5322,  5322,  5322,  5322,  5322,  5322,  5322,  5322,
    5322,  5322,  3094,  5322,  5322,  5322,  5322,  5322,  5322,  3240,
    -895,   558,    75,  -895,   697,   364,  -895,   549,   521,  -895,
    2948,  -895,  5421,  -895,  -895,  3385,  -895,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,  -895,  3385,  -895,  -895,  -895,  -895,
     187,   551,  7016,  2309,  -895,  2411,  2712,  -895,  2778,  -895,
     474,  -895,  -895,  -895,  -895,  -895,   514,  5322,  -895,  5322,
    5322,    -7,   552,   513,   520,   523,  5322,   524,  5322,  5322,
     513,   518,   526,  9296,  -895,  -895,  9219,  -895,   559,  5322,
    5322, 10322,   358,   149,   171,  5322,  -895,   560,   562,   563,
     564,  -895,   172,   396,  -895,  5322,  -895,  5322,  4669,  5322,
    -895,   550,   535,  -895,  -895,  5322,   546,  -895,  8563,  5322,
     547,  -895,  8654,  -895,  -895,  -895,  -895,  7107,  -895,   682,
    1301,  -895,  8756,  -895,  5322,  -895,  5523,  -895,  5322,  5322,
    -895,   350,  -895,  -895,  -895,  -895,  -895,  5625,  -895,  -895,
    -895,    69,  -895,  9296,  9296,  9296,  9296,  9296,  9296,  9296,
    9296,  9296,  9296,  5322,  5322,  9296,  9296,  9296,  9296,  9296,
    9296,  9296,  5322,  5322,  9296,  -895,   176,   410,    10,   544,
    -895,  -895,  5727,  -895,  -895,  -895,  -895,  -895,   185,  -895,
    -895,  -895,  -895,  -895,   553,  4838,    52,  9296,  9296,   -22,
     555,   150,   489,   565,  -895,  9296,  -895,  -895,    91,   -22,
     566,  -895,  -895,  -895,  7198,  7289,  2880,   396,   556,  9296,
    -895,  -895,  -895,  -895,   124,   567,   -83, 10322,  7380, 10322,
    7471,  -895,   202,  7562,  -895,  5322,  9457,  5322,  -895,  7653,
    5322,  -895,  -895,  -895,   583,   521,  5322,   103,  -895,  5322,
    -895,  5322,  -895,  1800,   350,  -895,  -895,  5322,  -895,  9676,
    9296,  9676,  9296,   554,  -895,  -895,     1,  -895,  -895,   410,
    -895,  -895,  5322,   584,  -895,  5322,  5322,  5322,  5007,  -895,
     205,  5322,   568,   569,  5322,  5322,  -895,  5322,   545,  -895,
    -895,  -895,  -895,   570,  -895,  -895,  -895,  5176,  -895,  -895,
    3027,  -895,  3173,  -895,  -895,  -895, 10322,  7744,  7835,  -895,
    7926,  -895,  9296,   350,  1307,  9296,  -895,  -895,   268,  -895,
    -895,   718,   166,  8017,   579,   835,   835,   835,  -895,  8108,
    -895,  5829,   571,  -895,  9296,  9296,  5829,   574,    12,  -895,
    5322,  9380,  -895,  -895,  3319,  -895,  -895,  -895,  -895,  -895,
      69, 10500,  -895,  -895,  -895,  -895,  5322,  -895,   703,   575,
    -895,   573,  -895,  -895,    12,  9457,  -895,  -895,  -895,  -895,
     576, 10500,  8199,  5322,   577,   578,  -895,  -895,  -895,   350,
    -895,  9296,  -895,  -895,  -895
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,    94,     1,   235,     0,     0,     0,     0,   236,     0,
     617,   614,     0,    14,     3,    10,     9,     8,     0,     7,
     474,     6,    11,     5,     4,    12,    13,    75,    76,    74,
      83,    85,    34,    47,    44,    45,    36,     0,    42,    35,
     485,     0,   490,    19,    18,   474,     0,     0,    88,    89,
       0,   213,    90,    92,     0,    87,   503,   502,   179,   491,
     504,   475,   476,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,   615,   215,
       0,     0,     0,    95,   181,   180,   183,   178,   493,   492,
       0,   506,   505,   507,   478,   477,   480,    81,    82,    79,
      80,    78,     0,     0,    77,    86,    48,    46,    42,    39,
      38,     0,   487,   489,     0,    16,    17,    20,     0,     0,
     214,     0,    93,     0,   494,     0,   499,   471,   417,    21,
      22,     0,    70,    71,    68,    69,    67,    66,    72,     0,
      41,     0,     0,     0,     0,   515,   535,   516,   547,   517,
     521,   522,   523,   524,   539,   528,   529,   530,   531,   532,
     533,   534,   536,   537,   587,   520,   527,   538,   594,   601,
     518,   525,   519,   526,     0,     0,     0,   546,   557,   560,
     558,   559,   486,   488,   550,     0,     0,    91,     0,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,     0,   101,    96,     0,     0,   482,   500,
       0,   508,   472,     0,     0,    23,    24,    25,     0,    84,
     578,   581,   584,   574,     0,   552,   588,   595,   602,   608,
     611,     0,   565,   570,   564,   577,     0,   573,   567,     0,
       0,   569,     0,     0,   446,     0,   152,   153,   150,   104,
     105,   107,   106,   108,   109,   110,   111,   137,   138,   135,
     136,   128,   139,   140,   129,   126,   127,   151,   148,     0,
     149,   141,   142,   143,   144,   115,   116,   117,   112,   113,
     114,   125,     0,   131,   132,   130,   123,   124,   119,   118,
     120,   121,   122,   103,   102,   147,     0,   133,   134,   417,
      99,   207,   184,   540,   543,   541,   544,   542,   545,     0,
       0,   497,     0,   479,   417,     0,     0,   461,   459,   481,
      73,     0,     0,     0,     0,     0,     0,     0,    96,     0,
      96,     0,    96,     0,     0,   323,   324,     0,     0,     0,
     317,     0,     0,     0,     0,     0,     0,   539,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     271,   273,   272,   274,   275,   276,    26,     0,     0,     0,
       0,     0,     0,     0,   258,   259,   321,   320,   269,   318,
     394,   393,   392,   391,    94,   397,   319,   396,   395,   364,
     325,     0,   326,     0,   322,   619,   620,   621,   622,   572,
     566,   568,     0,   571,   562,     0,   551,   618,     0,     0,
     146,   154,   145,     0,    97,     0,   439,     0,     0,   182,
       0,   482,     0,   495,     0,   501,   428,     0,     0,     0,
       0,     0,     0,   460,     0,     0,     0,   555,     0,     0,
     548,     0,     0,    99,     0,    99,     0,    99,   213,   443,
       0,   441,     0,   217,   220,     0,   247,     0,   238,   241,
       0,   244,     0,     0,   353,   354,     0,     0,     0,     0,
       0,     0,     0,   629,     0,     0,   640,   647,     0,     0,
     330,   329,   369,    32,   328,   327,     0,   260,   400,     0,
     267,     0,     0,     0,     0,   355,   356,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   314,     0,   549,     0,   563,   561,
     445,   616,   447,     0,     0,     0,   510,   436,   450,    98,
     417,   100,   209,     0,    57,    58,     0,     0,   221,     0,
       0,     0,     0,   222,     0,     0,     0,     0,     0,     0,
       0,   187,   185,     0,     0,     0,   202,   197,   194,   193,
     195,   196,   208,   188,     0,     0,     0,   200,   201,   203,
       0,   192,     0,   189,   277,     0,     0,   483,   498,   429,
     473,   418,     0,   463,   464,     0,   457,   458,   456,     0,
     579,   582,     0,   585,   575,   553,     0,   589,     0,   596,
       0,   603,     0,     0,   609,     0,   612,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   213,     0,     0,   637,
     627,   629,     0,   641,   642,   643,     0,     0,     0,   629,
       0,     0,     0,     0,    29,     0,    27,     0,   357,     0,
       0,   306,   303,   305,     0,   365,     0,   253,     0,    96,
       0,     0,   380,   379,     0,     0,   381,   385,   331,   332,
     344,   345,   342,   343,     0,   374,     0,   362,     0,   398,
     399,   333,   334,   349,   350,   351,   352,     0,     0,   347,
     348,   346,   340,   341,   336,   335,   337,   338,   339,     0,
       0,     0,   312,     0,     0,     0,     0,   367,     0,     0,
     437,     0,     0,   449,     0,   448,     0,   451,     0,   440,
       0,     0,     0,   223,   226,     0,   277,     0,   216,     0,
       0,   206,     0,     0,    51,    61,     0,   232,   277,   259,
     211,   212,     0,   204,   205,     0,   191,   198,   199,   237,
       0,   190,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,     0,     0,   509,     0,   417,   462,     0,   456,   466,
       0,   470,   277,   580,   583,   556,   586,   576,   554,   590,
     592,   597,   599,   604,   606,   442,   610,   444,   613,   218,
       0,     0,     0,     0,   370,     0,     0,   371,     0,   401,
     267,   402,   403,   404,   405,   406,     0,     0,   630,     0,
       0,   629,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   639,   648,   649,     0,    30,    33,     0,
       0,     0,    96,     0,     0,     0,   366,     0,     0,     0,
       0,   265,     0,    99,   376,     0,   382,     0,     0,     0,
     360,     0,     0,   386,   390,     0,     0,   363,     0,     0,
       0,   313,     0,   315,   358,   368,   438,     0,   512,   513,
     452,   455,   454,    62,     0,   227,   277,   228,     0,     0,
     224,     0,    63,    64,    49,    50,   233,   277,   230,   260,
     207,    54,   210,   287,   288,   290,   289,   291,   281,   282,
     283,   292,   293,     0,     0,   279,   280,   294,   295,   284,
     285,   286,     0,     0,   278,   484,     0,   424,   419,     0,
     465,   468,   277,   469,   593,   600,   607,   219,     0,   250,
     248,   239,   242,   245,     0,     0,     0,   624,   623,   629,
       0,     0,   628,     0,   632,   638,   645,   644,     0,   629,
       0,   646,    28,    31,     0,     0,     0,    99,     0,   254,
     263,   264,   262,   261,     0,     0,     0,     0,     0,     0,
       0,   301,     0,     0,   387,     0,   375,     0,   361,     0,
       0,   359,   316,   511,     0,   448,     0,     0,   225,     0,
     234,     0,   231,     0,     0,    52,    53,     0,    59,   298,
     299,   296,   297,    95,   425,   426,   419,   420,   421,   424,
     430,   467,     0,     0,   249,     0,     0,     0,     0,   407,
       0,     0,     0,     0,     0,     0,   631,     0,     0,   635,
     304,   416,   307,     0,   300,   266,   268,     0,   255,   270,
       0,   415,     0,   413,   302,   410,     0,     0,     0,   409,
       0,   514,   453,     0,     0,    65,   186,    55,     0,   435,
     431,   422,     0,     0,     0,   240,   243,   246,   372,     0,
     408,   650,     0,   633,   626,   625,   650,     0,     0,   310,
       0,   256,   377,   383,     0,   414,   412,   411,    60,   229,
      54,     0,   423,   433,   427,   251,     0,   373,     0,     0,
     634,     0,   636,   308,     0,   257,   378,   384,   388,    56,
       0,     0,     0,     0,     0,     0,   311,   389,   432,     0,
     252,   651,   652,   653,   434
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -895,  -895,  -895,  -895,  -895,   271,   699,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,   729,  -895,   661,  -895,  -895,   708,
    -895,  -895,  -895,  -345,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,   557,  -895,  -895,   712,   -45,  -895,   695,   -13,
    -379,  -328,  -446,  -895,  -895,  -895,  -731,  -895,  -895,  -210,
    -895,  -142,  -538,   -12,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,   780,  -895,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,  -895,  -532,  -895,  -895,  -212,  -895,
     -47,  -542,  -895,  -427,  -895,  -895,  -895,  -894,  -895,  -895,
    -895,  -895,  -895,  -895,  -895,  -895,   522,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,  -126,  -252,  -895,  -251,  -895,  -895,
    -895,  -895,  -895,   237,  -895,   188,  -895,  -401,   478,  -535,
    -540,   210,  -895,  -895,  -398,  -895,  -895,   778,  -895,  -895,
    -895,   400,   120,  -895,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,  -895,  -895,  -895,  -419,  -110,  -895,
    -895,  -895,   500,  -895,  -895,  -895,  -895,   -88,  -895,  -895,
    -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,  -895,
    -895,  -895,  -895,  -895,  -895,  -895,  -895,   485,  -649,  -477,
    -635,  -895,  -895,  -352,   -15,   353,  -895,  -895,  -895,  -272,
    -895
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   117,    45,    14,   131,   137,   489,   386,   858,
     387,   667,    15,    16,    38,    39,    72,    17,    34,    35,
     573,   574,  1027,  1028,   575,   576,   577,   578,   579,   580,
     581,  1021,   138,   139,    30,    31,    32,    52,    53,    54,
      18,   310,   429,   215,    19,    86,   216,    87,   123,   388,
     582,   430,   583,   389,   584,   390,   585,   586,   587,  1019,
     588,   589,   590,   591,   391,   632,  1045,   633,  1046,   635,
    1047,   392,   630,  1044,   393,   676,  1069,   394,   871,   872,
     679,   395,   396,   745,   593,   397,   398,   673,   399,   861,
    1108,   862,  1134,   400,   725,  1012,   677,   997,  1136,   999,
    1137,  1076,  1147,   402,   425,  1039,  1123,  1036,   949,   599,
     795,  1121,  1141,   426,   427,   459,   460,   254,   255,   840,
     547,   609,   444,   328,   329,   222,   324,    62,    96,    21,
     128,   320,    42,    73,    75,    22,    90,   125,    23,   435,
     220,   221,    60,    93,    24,   126,   322,   548,   403,   319,
     179,   180,   185,   181,   336,   808,   446,   461,   334,   807,
     331,   803,   332,   804,   333,   806,   337,   809,   338,   954,
     339,   811,   340,   955,   341,   813,   342,   956,   343,   816,
     344,   818,    25,    47,   119,    26,    46,   404,   650,   651,
     652,   405,   653,   654,   655,   656,   406,   488,   407,  1129,
     408
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   178,   224,   592,   730,   728,   312,   618,   659,   620,
     453,   622,   455,   738,   457,   500,   844,   487,   542,   744,
       2,   -94,   770,   182,   851,   864,     3,   760,   761,   645,
     757,    97,    98,   501,   502,   837,   121,    27,    28,   600,
     513,   514,   544,    68,   645,  1037,  1090,   849,  1067,     4,
     904,     5,   511,     6,  1037,   513,   514,   423,    40,     7,
      40,   223,   416,   644,   432,   657,   501,   502,     8,   733,
      55,   801,   615,   410,     9,   253,    50,    69,   839,   696,
    1024,    43,    84,   423,    80,   669,    33,   223,    85,   697,
     670,  1025,  1026,    10,   616,   771,   820,    41,  1038,   177,
     113,    51,   417,   838,   433,   424,    80,  1038,    48,   534,
     535,   132,   133,   311,    11,   541,   671,    44,   735,   503,
     504,   505,   506,   507,   534,   535,   508,   509,   510,   511,
     242,   512,   513,   514,   515,   545,    49,   741,   516,    36,
     517,   518,   623,   498,    50,    61,   519,   520,   521,   886,
     863,   546,   522,   669,   505,   506,   243,   244,   472,   887,
      37,    74,   511,   311,   512,   513,   514,   515,    99,    51,
     606,   516,   100,   843,   101,   102,    29,   499,   325,   607,
     523,   734,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   769,   734,   972,   114,   734,   217,   437,    70,
     901,   534,   535,   792,   905,   536,   970,    50,   103,    12,
      71,   793,   411,   596,  1133,    13,   245,   865,   916,   608,
     794,   178,   178,   178,   178,   218,   542,   178,   922,   178,
     412,   178,    51,   178,   534,   535,    63,   178,   432,   413,
    1146,   867,   597,   445,   447,   448,   449,  1051,   134,   452,
     868,   454,   135,   456,   136,   102,   865,   178,   178,   463,
      56,    57,   951,    58,   246,   247,   733,   733,   865,   248,
      64,   610,   249,   719,   720,   839,  1054,   228,   790,   483,
     485,    82,   865,   890,   326,    40,  1057,   250,   501,   502,
     869,    59,   624,   891,    65,   870,   251,   187,  1083,   733,
     327,   721,   626,    64,   229,   966,    83,   545,   178,   866,
     854,   722,   865,    50,   625,   735,   735,   978,   178,   177,
     177,   177,   177,   546,   627,   177,    64,   177,   438,   177,
     540,   177,   178,   242,  1052,   177,   842,   994,    51,   895,
     551,    82,   723,   724,  1058,    66,  1002,   734,   735,   747,
     736,   873,   865,   737,   605,   177,   177,   755,  1042,   243,
     244,   988,   995,   178,   969,  1016,  1033,   865,   493,   662,
     865,   663,  1017,   979,   503,   504,   505,   506,   507,   957,
    1043,   508,   509,   510,   511,   637,   512,   513,   514,   515,
    1140,   683,   687,   516,  1074,   517,   518,  1100,    88,    77,
    -591,   519,   520,   521,    89,  -591,   177,   522,  -598,   441,
    1149,    91,   442,  -598,    94,   443,   177,    92,   115,   245,
      95,   847,   598,  -591,   116,    80,   848,   996,   313,    78,
     177,  -598,   314,  1050,    79,   523,  1055,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   129,   130,   315,
     316,   317,   318,   629,  -605,  -309,   534,   535,    81,  -605,
    -309,   177,   501,   502,   311,  1034,   549,   246,   247,   550,
     106,  1035,   248,   384,   759,   249,   311,  -605,  -309,   129,
     130,   664,   665,    71,   848,   672,   852,   501,   502,   111,
     250,   225,   226,   227,   225,   226,   612,   108,   109,   251,
     112,   110,   178,   118,   120,   124,   127,    69,   183,   184,
     186,   219,   223,   178,   230,   178,   234,   231,   232,   241,
     178,   233,   178,   178,   805,   178,   235,   236,   252,   237,
     903,   253,   307,   238,   987,   815,   308,   239,   240,   912,
     321,  1063,   823,   335,   825,   826,   311,   828,   323,   309,
     505,   506,  1124,   409,   421,   921,   418,   419,   511,   420,
     422,   513,   514,   515,   428,   434,   439,   516,   431,   436,
     440,   450,   465,   503,   504,   505,   506,   507,   501,   502,
     508,   509,   510,   511,   466,   512,   513,   514,   515,   467,
     468,   469,   516,   470,   517,   518,   592,   884,   471,   472,
     177,   473,   476,   497,   477,   603,   478,   479,   480,    12,
     604,   177,   481,   177,   537,   538,   611,   601,   177,   543,
     177,   177,   678,   177,   731,   694,   178,   732,   750,   751,
     534,   535,   752,   762,   763,   764,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   766,   797,   900,   767,
     768,   416,   810,   812,   842,   534,   535,   814,   846,   845,
     855,   859,   857,   860,   503,   504,   505,   506,   507,   948,
     863,   508,   509,   510,   511,   893,   512,   513,   514,   515,
     874,   875,   876,   516,   898,   517,   518,   877,   878,   899,
     879,   519,   520,   521,   770,   889,   911,   522,   914,   915,
     907,  1020,   913,   919,   947,   965,   950,   920,   958,   971,
     973,   501,   502,   974,   976,   980,   981,   990,   983,   991,
     992,   993,  1004,  1014,   177,   523,  1005,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,  1007,  1010,  1040,
    1081,  1094,  1107,  1109,  1048,  1053,   534,   535,  1064,  1089,
    1122,   178,  1126,   945,  1143,  1056,  1059,  1066,  1102,  1103,
     666,  1130,   104,   401,  1132,  1144,    67,  1152,  1153,   140,
    1145,  1148,   415,   986,   107,  1139,   105,   122,  1023,   946,
      55,    20,  1065,   964,  1091,   330,  1068,   739,  1092,   772,
     773,   774,   775,   776,   777,   778,   779,   503,   504,   505,
     506,   507,   780,   781,   508,   509,   510,   511,   908,   512,
     513,   514,   515,   817,  1087,   800,   516,   783,   517,   518,
     784,   785,   462,    76,   519,   520,   521,   786,   787,   788,
     522,   595,   464,   977,  1131,     0,   451,     0,   660,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   177,
       0,     0,     0,     0,     0,   501,   502,   909,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,  1118,     0,     0,     0,     0,  1120,   534,
     535,   474,   475,     0,     0,     0,   910,   178,     0,   178,
       0,   486,     0,     0,     0,     0,     0,     0,     0,   490,
     491,   492,     0,   494,   495,   496,     0,     0,     0,  1070,
       0,  1072,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1154,
       0,   503,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   511,   594,   512,   513,   514,   515,     0,     0,     0,
     516,   602,   517,   518,     0,     0,   178,     0,     0,     0,
       0,   501,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   177,     0,   177,  1114,   631,
       0,     0,   634,     0,   636,     0,     0,     0,   638,   639,
     640,   641,   642,   643,     0,   649,     0,   649,     0,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,   688,   689,     0,     0,   690,
     691,   692,   693,     0,   695,     0,   698,   699,   700,   701,
     702,   703,   704,   705,   706,   708,   709,   710,   711,   712,
     713,   714,   715,   716,   717,   718,     0,   726,     0,   505,
     506,     0,     0,     0,   177,   729,     0,   511,     0,   512,
     513,   514,   515,     0,     0,   740,   516,     0,     0,   746,
       0,     0,   748,   749,     0,     0,     0,     0,   754,     0,
     758,     0,     0,     0,     0,     0,   672,   765,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   501,   502,   791,     0,
       0,     0,   672,     0,     0,   529,   530,   531,   532,   533,
       0,   802,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   822,     0,     0,     0,     0,   501,   502,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     496,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,     0,     0,   853,   486,     0,     0,   856,     0,     0,
       0,     0,     0,     0,   772,   773,   774,   775,   776,   777,
     778,   779,   503,   504,   505,   506,   507,   780,   781,   508,
     509,   510,   511,   908,   512,   513,   514,   515,     0,     0,
       0,   516,   783,   517,   518,   784,   785,     0,     0,   519,
     520,   521,   786,   787,   788,   522,     0,     0,     0,     0,
       0,     0,   888,     0,   505,   506,   892,     0,     0,     0,
       0,     0,   511,   897,   512,   513,   514,   515,     0,     0,
     902,   516,   909,   523,   906,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,   917,     0,
       0,     0,     0,     0,   534,   535,     0,     0,     0,     0,
       0,   918,     0,     0,   923,   924,   925,   926,   927,   928,
     929,   930,   931,   932,   935,   936,   937,   938,   939,   940,
     941,   944,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,   952,     0,   534,   535,     0,   501,   502,     0,
       0,     0,     0,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,     0,     0,     0,   242,   243,
     244,   967,   968,     0,     0,     0,     0,     0,   975,     0,
     649,     0,     0,     0,     0,     0,     0,     0,   243,   244,
       0,   984,   985,     0,   243,   244,     0,   989,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   998,     0,  1000,
       0,  1003,     0,     0,     0,     0,     0,  1006,     0,     0,
       0,  1009,     0,   503,   504,   505,   506,   507,   733,   245,
     508,   509,   510,   511,   242,   512,   513,   514,   515,     0,
     935,   944,   516,     0,   517,   518,     0,     0,   245,     0,
     519,   520,   521,   606,   245,     0,   522,     0,     0,     0,
     243,   244,   607,     0,     0,  1029,  1030,     0,     0,     0,
       0,     0,     0,     0,  1031,  1032,     0,  1015,   247,     0,
       0,     0,   248,     0,   523,   249,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   246,   247,     0,     0,
     250,   248,   798,   247,   249,   534,   535,   248,     0,   251,
     249,     0,  1119,     0,     0,     0,     0,     0,     0,   250,
     245,   628,     0,     0,     0,   250,     0,     0,   251,     0,
       0,   799,     0,     0,   251,     0,     0,  1077,     0,  1078,
       0,     0,  1080,     0,     0,     0,     0,     0,  1082,     0,
       0,  1084,     0,  1085,     0,   594,     0,     0,     0,  1088,
       0,     0,     0,     0,     0,     0,     0,     0,   246,   247,
       0,     0,     0,   248,  1093,   613,   249,  1095,  1096,  1097,
    1099,     0,     0,  1101,     0,   242,  1104,  1105,     0,  1106,
       0,   250,     0,     0,     0,     0,     0,     0,     0,  1111,
     251,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,   244,     0,   552,     0,     0,     0,     0,     3,
       0,   553,   554,   555,     0,   556,     0,   345,   346,   347,
     348,   349,     0,     0,     0,     0,     0,     0,   557,   350,
     558,   559,  1135,     0,     0,     0,     0,     0,     0,     0,
     560,   351,     0,   561,     0,   562,   352,     0,  1142,   353,
       0,     8,   354,   563,     0,   564,   355,     0,     0,   565,
     566,   245,     0,     0,     0,  1151,   567,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   357,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   358,   568,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
     247,   359,   360,     0,   248,     0,   614,   249,     0,     0,
       0,     0,     0,     0,     0,     0,   569,   570,     0,     0,
       0,     0,   250,     0,     0,    50,     0,     0,     0,     0,
       0,   251,     0,   361,   362,   363,   364,   365,     0,   366,
       0,   367,   368,   369,   370,   371,   372,   373,   374,   375,
      51,   376,     0,     0,     0,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
       0,   380,     0,     0,   381,   382,     0,     0,     0,     0,
       0,   243,   244,     0,   383,     0,   384,   385,   571,   311,
     572,   552,     0,     0,     0,     0,     3,     0,   553,   554,
     555,     0,   556,     0,   345,   346,   347,   348,   349,     0,
       0,     0,     0,     0,     0,   557,   350,   558,   559,     0,
       0,     0,     0,     0,     0,     0,     0,   560,   351,     0,
     561,     0,   562,   352,     0,     0,   353,     0,     8,   354,
     563,   245,   564,   355,     0,     0,   565,   566,     0,     0,
       0,     0,     0,   567,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   357,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   358,   568,     0,     0,   246,
     247,     0,     0,     0,   248,     0,   617,   249,   359,   360,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   250,   569,   570,     0,     0,     0,     0,     0,
       0,   251,    50,     0,   242,     0,     0,     0,     0,     0,
     361,   362,   363,   364,   365,     0,   366,     0,   367,   368,
     369,   370,   371,   372,   373,   374,   375,    51,   376,     0,
     243,   244,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   377,   378,   379,     0,   380,     0,
       0,   381,   382,     0,     0,     0,   345,   346,   347,   348,
     349,   383,     0,   384,   385,   571,   311,  1086,   350,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
     351,     0,     0,     0,     0,   352,     0,     0,   353,     0,
     245,   354,     0,   645,     0,   355,     0,     0,     0,     0,
     243,   244,     0,     0,     0,   356,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   357,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   358,   246,   247,
       0,     0,     0,   248,     0,   619,   249,     0,     0,     0,
     359,   360,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   250,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,     0,     0,    50,   242,     0,     0,     0,     0,
       0,     0,   361,   362,   363,   364,   365,     0,   366,     0,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   646,
     376,   243,   244,     0,     0,     0,     0,     0,   246,   247,
       0,     0,     0,   248,     0,     0,   647,   378,   379,     0,
     380,     0,     0,   381,   382,   345,   346,   347,   348,   349,
       0,   250,     0,   648,     0,   384,   385,   350,   311,     0,
     251,     0,     0,   242,     0,     0,     0,     0,     0,   351,
       0,     0,     0,     0,   352,     0,     0,   353,     0,     0,
     354,   245,     0,     0,   355,     0,     0,     0,     0,   243,
     244,     0,     0,     0,   356,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   357,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   358,     0,     0,   246,
     247,     0,     0,     0,   248,     0,   621,   249,     0,   359,
     360,     0,     0,     0,     0,     0,     0,     0,     0,   245,
       0,     0,   250,     0,     0,     0,     0,     0,     0,     0,
       0,   251,     0,    50,   242,     0,     0,     0,     0,     0,
       0,   361,   362,   363,   364,   365,     0,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   646,   376,
     243,   244,     0,     0,     0,     0,     0,   246,   247,     0,
       0,     0,   248,     0,     0,   647,   378,   379,     0,   380,
       0,     0,   381,   382,   345,   346,   347,   348,   349,     0,
     250,     0,   658,     0,   384,   385,   350,   311,     0,   251,
       0,   242,   410,     0,     0,     0,     0,     0,   351,     0,
       0,     0,     0,   352,     0,     0,   353,     0,     0,   354,
     245,     0,     0,   355,     0,     0,     0,   243,   244,     0,
       0,     0,     0,   356,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   357,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   358,     0,     0,   246,   247,
       0,     0,     0,   248,     0,   830,   249,     0,   359,   360,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,   250,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,    50,   242,     0,     0,     0,     0,     0,     0,
     361,   362,   363,   364,   365,     0,   366,     0,   367,   368,
     369,   370,   371,   372,   373,   374,   375,    51,   376,   243,
     244,     0,     0,     0,     0,   246,   247,     0,     0,     0,
     248,   411,   960,   249,   377,   378,   379,     0,   380,     0,
       0,   381,   382,   345,   346,   347,   348,   349,   250,   412,
       0,   383,     0,   384,   385,   350,   311,   251,   413,     0,
       0,     0,     0,     0,     0,     0,     0,   351,     0,     0,
       0,     0,   352,     0,     0,   353,     0,     0,   354,   245,
       0,     0,   355,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   356,   145,   146,   147,     0,   149,   150,   151,
     152,   153,   357,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   165,   166,   167,     0,     0,   170,   171,
     172,   173,     0,     0,   358,     0,     0,   246,   247,     0,
       0,     0,   248,     0,   961,   249,     0,   359,   360,     0,
       0,     0,     0,     0,     0,     0,   742,     0,     0,     0,
     250,     0,   569,   570,     0,     0,     0,     0,     0,   251,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   361,
     362,   363,   364,   365,     0,   366,     0,   367,   368,   369,
     370,   371,   372,   373,   374,   375,    51,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   345,   346,   347,
     348,   349,     0,   377,   378,   379,     0,   380,     0,   350,
     381,   382,     0,     0,     0,     0,     0,     0,     0,     0,
     383,   351,   384,   385,   743,   311,   352,     0,     0,   353,
       0,     0,   354,     0,     0,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   356,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   357,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   359,   360,     0,   242,     0,     0,     0,     0,     0,
     756,     0,     0,     0,     0,     0,   569,   570,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
     243,   244,     0,   361,   362,   363,   364,   365,     0,   366,
       0,   367,   368,   369,   370,   371,   372,   373,   374,   375,
      51,   376,     0,   345,   346,   347,   348,   349,     0,     0,
       0,     0,     0,     0,     0,   350,     0,   377,   378,   379,
     242,   380,     0,     0,   381,   382,     0,   351,     0,     0,
       0,     0,   352,     0,   383,   353,   384,   385,   354,   311,
     245,     0,   355,     0,     0,     0,   243,   244,     0,     0,
       0,     0,   356,   145,   146,   147,     0,   149,   150,   151,
     152,   153,   357,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   165,   166,   167,     0,     0,   170,   171,
     172,   173,     0,     0,   358,     0,     0,     0,   246,   247,
       0,     0,     0,   248,     0,   962,   249,   359,   360,     0,
       0,     0,     0,     0,     0,     0,   245,     0,     0,     0,
       0,   250,     0,     0,     0,     0,     0,     0,     0,     0,
     251,    50,   242,     0,     0,     0,     0,     0,     0,   361,
     362,   363,   364,   365,     0,   366,     0,   367,   368,   369,
     370,   371,   372,   373,   374,   375,    51,   376,   243,   244,
       0,     0,     0,     0,   246,   247,     0,     0,     0,   248,
       0,   963,   249,   377,   378,   379,     0,   380,     0,     0,
     381,   382,   345,   346,   347,   348,   349,   250,   674,     0,
     383,   675,   384,   385,   350,   311,   251,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,     0,     0,     0,
       0,   352,     0,     0,   353,     0,     0,   354,   245,     0,
       0,   355,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   356,   145,   146,   147,     0,   149,   150,   151,   152,
     153,   357,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   165,   166,   167,     0,     0,   170,   171,   172,
     173,     0,     0,   358,     0,     0,   246,   247,     0,     0,
       0,   248,     0,  1062,   249,     0,   359,   360,     0,   242,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   250,
       0,   569,   570,     0,     0,     0,     0,     0,   251,     0,
      50,     0,     0,     0,     0,   243,   244,     0,   361,   362,
     363,   364,   365,     0,   366,     0,   367,   368,   369,   370,
     371,   372,   373,   374,   375,    51,   376,     0,   345,   346,
     347,   348,   349,     0,     0,     0,     0,     0,     0,     0,
     350,     0,   377,   378,   379,     0,   380,     0,     0,   381,
     382,     0,   351,     0,     0,     0,     0,   352,     0,   383,
     353,   384,   385,   354,   311,   245,     0,   355,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   356,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   357,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,   358,
       0,     0,     0,   246,   247,     0,     0,     0,   248,     0,
    1112,   249,   359,   360,     0,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   250,   933,   934,     0,
       0,     0,     0,     0,     0,   251,    50,     0,     0,     0,
       0,   243,   244,     0,   361,   362,   363,   364,   365,     0,
     366,     0,   367,   368,   369,   370,   371,   372,   373,   374,
     375,    51,   376,     0,   345,   346,   347,   348,   349,     0,
       0,     0,     0,     0,     0,     0,   350,     0,   377,   378,
     379,     0,   380,     0,     0,   381,   382,     0,   351,     0,
       0,     0,     0,   352,     0,   383,   353,   384,   385,   354,
     311,   245,     0,   355,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   356,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   357,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   358,     0,     0,     0,   246,
     247,     0,     0,     0,   248,     0,  1113,   249,   359,   360,
       0,   242,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   250,   942,   943,     0,     0,     0,     0,     0,
       0,   251,    50,     0,     0,     0,     0,   243,   244,     0,
     361,   362,   363,   364,   365,     0,   366,     0,   367,   368,
     369,   370,   371,   372,   373,   374,   375,    51,   376,     0,
     345,   346,   347,   348,   349,     0,     0,     0,     0,     0,
       0,     0,   350,     0,   377,   378,   379,   242,   380,     0,
       0,   381,   382,     0,   351,     0,     0,     0,     0,   352,
       0,   383,   353,   384,   385,   354,   311,   245,     0,   355,
       0,     0,     0,   243,   244,     0,     0,     0,     0,   356,
     145,   146,   147,     0,   149,   150,   151,   152,   153,   357,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
       0,   358,     0,     0,     0,   246,   247,     0,     0,     0,
     248,     0,  1138,   249,   359,   360,     0,     0,     0,     0,
       0,     0,     0,   245,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,   251,    50,     0,
       0,     0,     0,     0,     0,     0,   361,   362,   363,   364,
     365,     0,   366,     0,   367,   368,   369,   370,   371,   372,
     373,   374,   375,    51,   376,     0,   345,   346,   347,   348,
     349,   246,   247,   707,     0,     0,   248,     0,   350,   249,
     377,   378,   379,     0,   380,     0,     0,   381,   382,     0,
     351,     0,     0,     0,   250,   352,   414,   383,   353,   384,
     385,   354,   311,   251,     0,   355,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   356,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   357,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   358,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   360,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   361,   362,   363,   364,   365,     0,   366,     0,
     367,   368,   369,   370,   371,   372,   373,   374,   375,    51,
     376,     0,   345,   346,   347,   348,   349,     0,     0,     0,
       0,     0,     0,     0,   350,     0,   377,   378,   379,     0,
     380,     0,     0,   381,   382,     0,   351,     0,     0,     0,
       0,   352,     0,   383,   353,   384,   385,   354,   311,     0,
       0,   355,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   356,   145,   146,   147,     0,   149,   150,   151,   152,
     153,   357,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   165,   166,   167,     0,     0,   170,   171,   172,
     173,     0,     0,   358,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,   361,   362,
     363,   364,   365,     0,   366,     0,   367,   368,   369,   370,
     371,   372,   373,   374,   375,    51,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   379,     0,   380,     0,     0,   381,
     382,   345,   346,   347,   348,   349,     0,     0,     0,   383,
     727,   384,   385,   350,   311,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,     0,     0,     0,     0,
     352,     0,     0,   353,     0,     0,   354,     0,     0,     0,
     355,     0,     0,   753,     0,     0,     0,     0,     0,     0,
     356,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     357,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   359,   360,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   361,   362,   363,
     364,   365,     0,   366,     0,   367,   368,   369,   370,   371,
     372,   373,   374,   375,    51,   376,     0,   345,   346,   347,
     348,   349,     0,     0,     0,     0,     0,     0,     0,   350,
       0,   377,   378,   379,     0,   380,     0,     0,   381,   382,
       0,   351,     0,     0,     0,     0,   352,     0,   383,   353,
     384,   385,   354,   311,     0,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   356,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   357,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   359,   360,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   361,   362,   363,   364,   365,     0,   366,
       0,   367,   368,   369,   370,   371,   372,   373,   374,   375,
      51,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
       0,   380,     0,     0,   381,   382,   345,   346,   347,   348,
     349,     0,     0,     0,   383,   819,   384,   385,   350,   311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,     0,     0,     0,     0,   352,     0,     0,   353,     0,
       0,   354,     0,     0,     0,   355,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   356,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   357,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   358,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   360,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   361,   362,   363,   364,   365,     0,   366,     0,
     367,   368,   369,   370,   371,   372,   373,   374,   375,    51,
     376,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   821,     0,   377,   378,   379,     0,
     380,     0,     0,   381,   382,   345,   346,   347,   348,   349,
       0,     0,     0,   383,     0,   384,   385,   350,   311,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
       0,     0,     0,     0,   352,     0,     0,   353,     0,     0,
     354,     0,     0,     0,   355,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   356,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   357,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   359,
     360,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   361,   362,   363,   364,   365,     0,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,    51,   376,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,     0,   380,
       0,     0,   381,   382,   345,   346,   347,   348,   349,     0,
       0,     0,   383,   841,   384,   385,   350,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,     0,
       0,     0,     0,   352,     0,     0,   353,     0,     0,   354,
       0,     0,     0,   355,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   356,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   357,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   358,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   359,   360,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
     361,   362,   363,   364,   365,     0,   366,     0,   367,   368,
     369,   370,   371,   372,   373,   374,   375,    51,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   377,   378,   379,     0,   380,     0,
       0,   381,   382,   345,   346,   347,   348,   349,     0,     0,
       0,   383,   850,   384,   385,   350,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,     0,     0,
       0,     0,   352,     0,     0,   353,     0,     0,   354,     0,
       0,     0,   355,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   356,   145,   146,   147,     0,   149,   150,   151,
     152,   153,   357,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   165,   166,   167,     0,     0,   170,   171,
     172,   173,     0,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   359,   360,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   361,
     362,   363,   364,   365,     0,   366,     0,   367,   368,   369,
     370,   371,   372,   373,   374,   375,    51,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   377,   378,   379,     0,   380,     0,     0,
     381,   382,   345,   346,   347,   348,   349,     0,     0,     0,
     383,  1001,   384,   385,   350,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,     0,     0,     0,
       0,   352,     0,     0,   353,     0,     0,   354,     0,     0,
       0,   355,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   356,   145,   146,   147,     0,   149,   150,   151,   152,
     153,   357,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   165,   166,   167,     0,     0,   170,   171,   172,
     173,     0,     0,   358,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,   361,   362,
     363,   364,   365,     0,   366,     0,   367,   368,   369,   370,
     371,   372,   373,   374,   375,    51,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   379,     0,   380,     0,     0,   381,
     382,   345,   346,   347,   348,   349,     0,     0,     0,   383,
    1049,   384,   385,   350,   311,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,     0,     0,     0,     0,
     352,     0,     0,   353,     0,     0,   354,     0,     0,     0,
     355,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     356,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     357,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   359,   360,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   361,   362,   363,
     364,   365,     0,   366,     0,   367,   368,   369,   370,   371,
     372,   373,   374,   375,    51,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,   378,   379,     0,   380,     0,     0,   381,   382,
     345,   346,   347,   348,   349,     0,     0,     0,   383,  1098,
     384,   385,   350,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,     0,     0,     0,     0,   352,
       0,     0,   353,     0,     0,   354,     0,     0,     0,   355,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
     145,   146,   147,     0,   149,   150,   151,   152,   153,   357,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
       0,   358,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,   360,     0,     0,     0,     0,
       0,     0,     0,  1110,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,   361,   362,   363,   364,
     365,     0,   366,     0,   367,   368,   369,   370,   371,   372,
     373,   374,   375,    51,   376,     0,   345,   346,   347,   348,
     349,     0,     0,     0,     0,     0,     0,     0,   350,     0,
     377,   378,   379,     0,   380,     0,     0,   381,   382,     0,
     351,     0,     0,     0,     0,   352,     0,   383,   353,   384,
     385,   354,   311,     0,     0,   355,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   356,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   357,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   358,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     359,   360,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   501,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   361,   362,   363,   364,   365,     0,   366,     0,
     367,   368,   369,   370,   371,   372,   373,   374,   375,    51,
     376,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,     0,
     380,     0,     0,   381,   382,     0,     0,     0,     0,     0,
       0,     0,     0,   383,     0,   384,   385,     0,   311,   772,
     773,   774,   775,   776,   777,   778,   779,   503,   504,   505,
     506,   507,   780,   781,   508,   509,   510,   511,   908,   512,
     513,   514,   515,   501,   502,     0,   516,   783,   517,   518,
     784,   785,     0,     0,   519,   520,   521,   786,   787,   788,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   909,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,     0,     0,     0,   953,     0,     0,     0,
       0,   772,   773,   774,   775,   776,   777,   778,   779,   503,
     504,   505,   506,   507,   780,   781,   508,   509,   510,   511,
     908,   512,   513,   514,   515,   501,   502,     0,   516,   783,
     517,   518,   784,   785,     0,     0,   519,   520,   521,   786,
     787,   788,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   909,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,     0,     0,     0,     0,     0,  1018,     0,
       0,     0,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   503,   504,   505,   506,   507,   780,   781,   508,   509,
     510,   511,   908,   512,   513,   514,   515,   501,   502,     0,
     516,   783,   517,   518,   784,   785,     0,     0,   519,   520,
     521,   786,   787,   788,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   909,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,     0,     0,     0,     0,     0,
    1022,     0,     0,     0,     0,   772,   773,   774,   775,   776,
     777,   778,   779,   503,   504,   505,   506,   507,   780,   781,
     508,   509,   510,   511,   908,   512,   513,   514,   515,   501,
     502,     0,   516,   783,   517,   518,   784,   785,     0,     0,
     519,   520,   521,   786,   787,   788,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   909,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,     0,
       0,     0,  1041,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   501,   502,   503,   504,   505,   506,   507,
       0,     0,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
     501,   502,     0,     0,     0,     0,     0,   534,   535,   503,
     504,   505,   506,   507,  1128,     0,   508,   509,   510,   511,
       0,   512,   513,   514,   515,   501,   502,     0,   516,     0,
     517,   518,     0,     0,     0,     0,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,   503,   504,   505,   506,
     507,   534,   535,   508,     0,   668,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
       0,   503,   504,   505,   506,   507,   501,   502,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,     0,     0,   519,   520,
     521,     0,     0,     0,   522,     0,     0,     0,     0,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
       0,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,     0,     0,   796,     0,     0,
       0,     0,   503,   504,   505,   506,   507,   501,   502,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,   824,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   501,   502,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,     0,     0,     0,
     519,   520,   521,     0,     0,     0,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,   827,
       0,     0,     0,     0,   503,   504,   505,   506,   507,   501,
     502,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
     829,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     501,   502,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,   831,     0,     0,     0,     0,   503,   504,   505,   506,
     507,   501,   502,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
       0,     0,     0,   519,   520,   521,     0,     0,     0,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
       0,     0,   832,     0,     0,     0,     0,   503,   504,   505,
     506,   507,   501,   502,   508,   509,   510,   511,     0,   512,
     513,   514,   515,     0,     0,     0,   516,     0,   517,   518,
       0,     0,     0,     0,   519,   520,   521,     0,     0,     0,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,   833,     0,     0,     0,     0,   503,   504,
     505,   506,   507,   501,   502,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,     0,     0,   834,     0,     0,     0,     0,   503,
     504,   505,   506,   507,   501,   502,   508,   509,   510,   511,
       0,   512,   513,   514,   515,     0,     0,     0,   516,     0,
     517,   518,     0,     0,     0,     0,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,     0,     0,   835,     0,     0,     0,     0,
     503,   504,   505,   506,   507,   501,   502,   508,   509,   510,
     511,     0,   512,   513,   514,   515,     0,     0,     0,   516,
       0,   517,   518,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,   836,     0,     0,     0,
       0,   503,   504,   505,   506,   507,   501,   502,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,     0,     0,   519,   520,
     521,     0,     0,     0,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,     0,     0,   896,     0,     0,
       0,     0,   503,   504,   505,   506,   507,   501,   502,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,   959,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   501,   502,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,     0,     0,     0,
     519,   520,   521,     0,     0,     0,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,  1013,
       0,     0,     0,     0,   503,   504,   505,   506,   507,   501,
     502,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
    1060,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     501,   502,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,  1061,     0,     0,     0,     0,   503,   504,   505,   506,
     507,   501,   502,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
       0,     0,     0,   519,   520,   521,     0,     0,     0,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
       0,     0,  1071,     0,     0,     0,     0,   503,   504,   505,
     506,   507,   501,   502,   508,   509,   510,   511,     0,   512,
     513,   514,   515,     0,     0,     0,   516,     0,   517,   518,
       0,     0,     0,     0,   519,   520,   521,     0,     0,     0,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,  1073,     0,     0,     0,     0,   503,   504,
     505,   506,   507,   501,   502,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,     0,     0,  1075,     0,     0,     0,     0,   503,
     504,   505,   506,   507,   501,   502,   508,   509,   510,   511,
       0,   512,   513,   514,   515,     0,     0,     0,   516,     0,
     517,   518,     0,     0,     0,     0,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,     0,     0,  1079,     0,     0,     0,     0,
     503,   504,   505,   506,   507,   501,   502,   508,   509,   510,
     511,     0,   512,   513,   514,   515,     0,     0,     0,   516,
       0,   517,   518,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,  1115,     0,     0,     0,
       0,   503,   504,   505,   506,   507,   501,   502,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,     0,     0,   519,   520,
     521,     0,     0,     0,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,     0,     0,  1116,     0,     0,
       0,     0,   503,   504,   505,   506,   507,   501,   502,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,  1117,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   501,   502,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,     0,     0,     0,
     519,   520,   521,     0,     0,     0,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,  1125,
       0,     0,     0,     0,   503,   504,   505,   506,   507,   501,
     502,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
    1127,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     501,   502,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,  1150,     0,     0,     0,     0,   503,   504,   505,   506,
     507,   501,   502,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
       0,     0,     0,   519,   520,   521,     0,     0,     0,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
     539,     0,     0,     0,     0,     0,     0,   503,   504,   505,
     506,   507,   501,   502,   508,   509,   510,   511,     0,   512,
     513,   514,   515,     0,     0,     0,   516,     0,   517,   518,
       0,     0,     0,     0,   519,   520,   521,     0,     0,     0,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,   880,     0,     0,     0,     0,     0,     0,   503,   504,
     505,   506,   507,   501,   502,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,   894,     0,     0,     0,     0,     0,     0,   503,
     504,   505,   506,   507,   501,   502,   508,   509,   510,   511,
       0,   512,   513,   514,   515,     0,     0,     0,   516,     0,
     517,   518,     0,     0,     0,     0,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,  1008,     0,     0,     0,     0,     0,     0,
     503,   504,   505,   506,   507,     0,     0,   508,   509,   510,
     511,     0,   512,   513,   514,   515,   501,   502,     0,   516,
       0,   517,   518,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,  1011,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   772,   773,   774,   775,   776,   777,
     778,   779,   503,   504,   505,   506,   507,   780,   781,   508,
     509,   510,   511,   908,   512,   513,   514,   515,  -277,   501,
     502,   516,   783,   517,   518,   784,   785,     0,     0,   519,
     520,   521,   786,   787,   788,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   909,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,   773,   774,
     775,   776,   777,   778,   779,   503,   504,   505,   506,   507,
     780,   781,   508,   509,   510,   511,   782,   512,   513,   514,
     515,     0,   256,   257,   516,   783,   517,   518,   784,   785,
       0,     0,   519,   520,   521,   786,   787,   788,   522,   258,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   789,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   501,   502,
     277,   278,   279,     0,     0,     0,     0,     0,   280,   281,
     282,   283,   284,     0,     0,   285,   286,   287,   288,   289,
     290,   291,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   292,
       0,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,     0,     0,   303,   304,     0,     0,     0,   680,     0,
     305,   306,     0,     0,     0,   501,   502,     0,     0,     0,
       0,     0,     0,     0,   503,   504,   505,   506,   507,     0,
       0,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,   661,
       0,   519,   520,   521,   145,   146,   147,   522,   149,   150,
     151,   152,   153,   357,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,   501,
     502,   503,   504,   505,   506,   507,   534,   535,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,     0,     0,   519,   520,
     521,     0,     0,     0,   522,     0,     0,   681,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   682,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   885,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,   501,   502,     0,     0,
       0,     0,     0,   534,   535,   503,   504,   505,   506,   507,
       0,     0,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   982,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
     501,   502,   503,   504,   505,   506,   507,   534,   535,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,   501,   502,     0,
       0,     0,     0,     0,   534,   535,   503,   504,   505,   506,
     507,     0,     0,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
     501,   502,     0,   519,   520,   521,     0,     0,     0,  -606,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   534,   535,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,   501,   502,     0,
     519,   520,   521,     0,     0,     0,   503,   504,   505,   506,
     507,     0,     0,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,   501,
     502,     0,     0,   519,   523,   521,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   534,   535,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,   501,   502,     0,     0,
     519,     0,     0,     0,     0,   503,   504,   505,   506,   507,
       0,     0,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
     501,   502,     0,     0,     0,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,   503,   504,     0,     0,   507,   534,   535,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,   501,   502,   519,
     520,   521,     0,     0,     0,   522,   503,   504,   505,   506,
     507,     0,     0,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
       0,     0,     0,   532,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   534,   535,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   141,     0,     0,     0,     0,     0,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,   143,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,     0,
       0,   144,     0,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   484,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   141,     0,     0,
       0,     0,     0,   142,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,     0,
     143,     0,   684,     0,   176,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   144,    51,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   357,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,   881,     0,     0,     0,     0,     0,   176,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   685,     0,     0,     0,     0,     0,     0,   145,   146,
     147,   686,   149,   150,   151,   152,   153,   357,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   141,     0,     0,     0,     0,     0,   142,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   143,     0,     0,     0,     0,
       0,   882,     0,     0,     0,     0,     0,     0,     0,     0,
     144,   883,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   141,     0,     0,     0,
       0,     0,   142,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,     0,   143,
       0,     0,     0,   176,     0,     0,   367,   368,   369,     0,
       0,     0,     0,     0,   144,    51,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     141,     0,     0,     0,     0,     0,   142,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,     0,   143,     0,     0,     0,   176,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   144,    51,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   188,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   176,     0,     0,   189,     0,   190,     0,   191,   192,
     193,   194,   195,   458,   196,   197,   198,   199,   200,   201,
     202,   203,   204,     0,   205,   206,   207,     0,     0,   208,
     209,   210,   211,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   214
};

static const yytype_int16 yycheck[] =
{
      12,   111,   128,   430,   544,   537,   216,   453,   485,   455,
     338,   457,   340,   548,   342,   394,   651,   369,   419,   557,
       0,     7,   122,   111,   659,   674,     6,   569,   570,    51,
     568,    14,    15,    20,    21,    19,    81,    18,    19,   437,
     119,   120,    48,    21,    51,    44,    45,    19,   131,    29,
      19,    31,   116,    33,    44,   119,   120,   144,   157,    39,
     157,   148,   157,   482,   157,   484,    20,    21,    48,   117,
      82,   609,   173,    32,    54,   157,   132,    55,   126,   147,
      11,   157,    55,   144,   132,   141,   151,   148,    61,   157,
     146,    22,    23,    73,   195,   195,   628,   196,    97,   111,
     197,   157,   197,   645,   197,   192,   132,    97,    33,   188,
     189,    14,    15,   196,    94,   197,   172,   193,   166,   106,
     107,   108,   109,   110,   188,   189,   113,   114,   115,   116,
      32,   118,   119,   120,   121,   141,    61,   556,   125,   157,
     127,   128,   168,   157,   132,    60,   133,   134,   135,   147,
     157,   157,   139,   141,   108,   109,    58,    59,   191,   157,
     178,    41,   116,   196,   118,   119,   120,   121,   151,   157,
     117,   125,   155,   195,   157,   158,   157,   191,   223,   126,
     167,   165,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   590,   165,   843,    75,   165,   168,   324,   177,
     738,   188,   189,   189,   742,   192,   841,   132,   191,   189,
     188,   197,   171,   166,  1108,   195,   118,   165,   756,   166,
     599,   331,   332,   333,   334,   196,   627,   337,   770,   339,
     189,   341,   157,   343,   188,   189,   166,   347,   157,   198,
    1134,   117,   195,   331,   332,   333,   334,   195,   151,   337,
     126,   339,   155,   341,   157,   158,   165,   367,   368,   347,
       4,     5,   800,     7,   166,   167,   117,   117,   165,   171,
     165,   173,   174,   119,   120,   126,   126,   165,   197,   367,
     368,   165,   165,   147,   141,   157,   195,   189,    20,    21,
     166,    35,   173,   157,   168,   171,   198,   192,   195,   117,
     157,   147,   173,   165,   192,   837,   190,   141,   418,   192,
     662,   157,   165,   132,   195,   166,   166,   849,   428,   331,
     332,   333,   334,   157,   195,   337,   165,   339,   190,   341,
     418,   343,   442,    32,   969,   347,   165,   165,   157,   192,
     428,   165,   188,   189,   979,   165,   878,   165,   166,   559,
     168,   679,   165,   171,   442,   367,   368,   567,   173,    58,
      59,   190,   190,   473,   841,   900,   190,   165,   380,   195,
     165,   197,   904,   850,   106,   107,   108,   109,   110,   192,
     195,   113,   114,   115,   116,   473,   118,   119,   120,   121,
    1121,   501,   502,   125,   192,   127,   128,   192,    55,   157,
     168,   133,   134,   135,    61,   173,   418,   139,   168,   165,
    1141,    55,   168,   173,    55,   171,   428,    61,    55,   118,
      61,   190,   434,   191,    61,   132,   195,   873,    68,   157,
     442,   191,    72,   965,   157,   167,   971,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   159,   160,    89,
      90,    91,    92,   465,   168,   168,   188,   189,   191,   173,
     173,   473,    20,    21,   196,    55,   192,   166,   167,   195,
     151,    61,   171,   193,   194,   174,   196,   191,   191,   159,
     160,   161,   162,   188,   195,   497,   197,    20,    21,   166,
     189,   159,   160,   161,   159,   160,   195,   157,   157,   198,
     195,   157,   612,   196,   157,   157,   196,    55,   195,   157,
     196,    97,   148,   623,   172,   625,   191,   172,   172,   191,
     630,   172,   632,   633,   612,   635,   172,   172,   195,   172,
     740,   157,    34,   172,   862,   623,    34,   172,   172,   749,
     157,   987,   630,   157,   632,   633,   196,   635,   197,   191,
     108,   109,  1092,    32,    21,   765,   168,   195,   116,   190,
     190,   119,   120,   121,   168,   168,   191,   125,   196,   196,
      41,   192,   191,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,   172,   118,   119,   120,   121,   191,
     172,   172,   125,   191,   127,   128,  1023,   707,   172,   191,
     612,   172,   191,   194,   191,   157,   191,   191,   191,   189,
     157,   623,   191,   625,   191,   190,   173,   190,   630,   191,
     632,   633,   148,   635,   191,   157,   736,    41,   157,   195,
     188,   189,   151,    36,   195,   195,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   195,    41,   736,   195,
     195,   157,   173,   173,   165,   188,   189,   173,   165,   190,
     197,   191,   159,   191,   106,   107,   108,   109,   110,   795,
     157,   113,   114,   115,   116,     1,   118,   119,   120,   121,
     172,   191,   172,   125,   157,   127,   128,   191,   191,   157,
     191,   133,   134,   135,   122,   191,    13,   139,   168,   151,
       9,   911,   166,   194,     7,   191,   157,   196,   157,   157,
     190,    20,    21,   190,   190,   197,   190,   157,   159,   157,
     157,   157,   172,    41,   736,   167,   191,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   191,   191,   195,
     157,   157,   197,   173,   191,   190,   188,   189,   192,   195,
      32,   861,   173,   195,    51,   190,   190,   190,   190,   190,
     489,   190,    63,   241,   190,   190,    37,   190,   190,   108,
     197,   195,   250,   861,    66,  1120,    64,    82,   920,   792,
     792,     1,   994,   830,  1036,   228,   996,   550,  1039,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   625,  1024,   605,   125,   126,   127,   128,
     129,   130,   344,    45,   133,   134,   135,   136,   137,   138,
     139,   431,   347,   848,  1106,    -1,   336,    -1,   485,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   861,
      -1,    -1,    -1,    -1,    -1,    20,    21,   166,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    -1,  1083,    -1,    -1,    -1,    -1,  1088,   188,
     189,   359,   360,    -1,    -1,    -1,   195,   997,    -1,   999,
      -1,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   377,
     378,   379,    -1,   381,   382,   383,    -1,    -1,    -1,   997,
      -1,   999,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1149,
      -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   430,   118,   119,   120,   121,    -1,    -1,    -1,
     125,   439,   127,   128,    -1,    -1,  1076,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   997,    -1,   999,  1076,   467,
      -1,    -1,   470,    -1,   472,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,    -1,   483,    -1,   485,    -1,   174,
     175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,   503,   504,    -1,    -1,   507,
     508,   509,   510,    -1,   512,    -1,   514,   515,   516,   517,
     518,   519,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,    -1,   535,    -1,   108,
     109,    -1,    -1,    -1,  1076,   543,    -1,   116,    -1,   118,
     119,   120,   121,    -1,    -1,   553,   125,    -1,    -1,   557,
      -1,    -1,   560,   561,    -1,    -1,    -1,    -1,   566,    -1,
     568,    -1,    -1,    -1,    -1,    -1,  1108,   575,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,   596,    -1,
      -1,    -1,  1134,    -1,    -1,   174,   175,   176,   177,   178,
      -1,   609,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   629,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   647,
     648,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     658,    -1,    -1,   661,   662,    -1,    -1,   665,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,   720,    -1,   108,   109,   724,    -1,    -1,    -1,
      -1,    -1,   116,   731,   118,   119,   120,   121,    -1,    -1,
     738,   125,   166,   167,   742,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,   756,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,
      -1,   195,    -1,    -1,   772,   773,   774,   775,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   786,   787,
     788,   789,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   800,    -1,   188,   189,    -1,    20,    21,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    32,    58,
      59,   839,   840,    -1,    -1,    -1,    -1,    -1,   846,    -1,
     848,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
      -1,   859,   860,    -1,    58,    59,    -1,   865,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   875,    -1,   877,
      -1,   879,    -1,    -1,    -1,    -1,    -1,   885,    -1,    -1,
      -1,   889,    -1,   106,   107,   108,   109,   110,   117,   118,
     113,   114,   115,   116,    32,   118,   119,   120,   121,    -1,
     908,   909,   125,    -1,   127,   128,    -1,    -1,   118,    -1,
     133,   134,   135,   117,   118,    -1,   139,    -1,    -1,    -1,
      58,    59,   126,    -1,    -1,   933,   934,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   942,   943,    -1,   166,   167,    -1,
      -1,    -1,   171,    -1,   167,   174,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   166,   167,    -1,    -1,
     189,   171,   166,   167,   174,   188,   189,   171,    -1,   198,
     174,    -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,   189,
     118,   191,    -1,    -1,    -1,   189,    -1,    -1,   198,    -1,
      -1,   195,    -1,    -1,   198,    -1,    -1,  1005,    -1,  1007,
      -1,    -1,  1010,    -1,    -1,    -1,    -1,    -1,  1016,    -1,
      -1,  1019,    -1,  1021,    -1,  1023,    -1,    -1,    -1,  1027,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,   167,
      -1,    -1,    -1,   171,  1042,   173,   174,  1045,  1046,  1047,
    1048,    -1,    -1,  1051,    -1,    32,  1054,  1055,    -1,  1057,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1067,
     198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    -1,     1,    -1,    -1,    -1,    -1,     6,
      -1,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,  1110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,  1126,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    -1,    56,
      57,   118,    -1,    -1,    -1,  1143,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
     167,   108,   109,    -1,   171,    -1,   173,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,
      -1,    -1,   189,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,   198,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
      -1,   178,    -1,    -1,   181,   182,    -1,    -1,    -1,    -1,
      -1,    58,    59,    -1,   191,    -1,   193,   194,   195,   196,
     197,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    -1,    42,    43,    -1,    -1,    46,    -1,    48,    49,
      50,   118,    52,    53,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    96,    -1,    -1,   166,
     167,    -1,    -1,    -1,   171,    -1,   173,   174,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,   198,   132,    -1,    32,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   175,   176,    -1,   178,    -1,
      -1,   181,   182,    -1,    -1,    -1,    14,    15,    16,    17,
      18,   191,    -1,   193,   194,   195,   196,   197,    26,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
     118,    49,    -1,    51,    -1,    53,    -1,    -1,    -1,    -1,
      58,    59,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,   166,   167,
      -1,    -1,    -1,   171,    -1,   173,   174,    -1,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,    -1,    -1,    -1,   132,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    58,    59,    -1,    -1,    -1,    -1,    -1,   166,   167,
      -1,    -1,    -1,   171,    -1,    -1,   174,   175,   176,    -1,
     178,    -1,    -1,   181,   182,    14,    15,    16,    17,    18,
      -1,   189,    -1,   191,    -1,   193,   194,    26,   196,    -1,
     198,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,   118,    -1,    -1,    53,    -1,    -1,    -1,    -1,    58,
      59,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,   166,
     167,    -1,    -1,    -1,   171,    -1,   173,   174,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    -1,   132,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      58,    59,    -1,    -1,    -1,    -1,    -1,   166,   167,    -1,
      -1,    -1,   171,    -1,    -1,   174,   175,   176,    -1,   178,
      -1,    -1,   181,   182,    14,    15,    16,    17,    18,    -1,
     189,    -1,   191,    -1,   193,   194,    26,   196,    -1,   198,
      -1,    32,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
     118,    -1,    -1,    53,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,   166,   167,
      -1,    -1,    -1,   171,    -1,   173,   174,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,    -1,   132,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    58,
      59,    -1,    -1,    -1,    -1,   166,   167,    -1,    -1,    -1,
     171,   171,   173,   174,   174,   175,   176,    -1,   178,    -1,
      -1,   181,   182,    14,    15,    16,    17,    18,   189,   189,
      -1,   191,    -1,   193,   194,    26,   196,   198,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,   118,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    -1,    -1,   166,   167,    -1,
      -1,    -1,   171,    -1,   173,   174,    -1,   108,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
     189,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,   198,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,   174,   175,   176,    -1,   178,    -1,    26,
     181,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,    38,   193,   194,   195,   196,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    32,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      58,    59,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   174,   175,   176,
      32,   178,    -1,    -1,   181,   182,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   191,    46,   193,   194,    49,   196,
     118,    -1,    53,    -1,    -1,    -1,    58,    59,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    -1,    -1,    -1,   166,   167,
      -1,    -1,    -1,   171,    -1,   173,   174,   108,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,   132,    32,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    58,    59,
      -1,    -1,    -1,    -1,   166,   167,    -1,    -1,    -1,   171,
      -1,   173,   174,   174,   175,   176,    -1,   178,    -1,    -1,
     181,   182,    14,    15,    16,    17,    18,   189,   189,    -1,
     191,   192,   193,   194,    26,   196,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,   118,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    -1,    -1,   166,   167,    -1,    -1,
      -1,   171,    -1,   173,   174,    -1,   108,   109,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,   198,    -1,
     132,    -1,    -1,    -1,    -1,    58,    59,    -1,   140,   141,
     142,   143,   144,    -1,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,
     182,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   191,
      46,   193,   194,    49,   196,   118,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    -1,   166,   167,    -1,    -1,    -1,   171,    -1,
     173,   174,   108,   109,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,   198,   132,    -1,    -1,    -1,
      -1,    58,    59,    -1,   140,   141,   142,   143,   144,    -1,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   174,   175,
     176,    -1,   178,    -1,    -1,   181,   182,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   191,    46,   193,   194,    49,
     196,   118,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,   166,
     167,    -1,    -1,    -1,   171,    -1,   173,   174,   108,   109,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,   198,   132,    -1,    -1,    -1,    -1,    58,    59,    -1,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   174,   175,   176,    32,   178,    -1,
      -1,   181,   182,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   191,    46,   193,   194,    49,   196,   118,    -1,    53,
      -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,   166,   167,    -1,    -1,    -1,
     171,    -1,   173,   174,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    -1,    14,    15,    16,    17,
      18,   166,   167,    21,    -1,    -1,   171,    -1,    26,   174,
     174,   175,   176,    -1,   178,    -1,    -1,   181,   182,    -1,
      38,    -1,    -1,    -1,   189,    43,   190,   191,    46,   193,
     194,    49,   196,   198,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,   174,   175,   176,    -1,
     178,    -1,    -1,   181,   182,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,   191,    46,   193,   194,    49,   196,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,
     182,    14,    15,    16,    17,    18,    -1,    -1,    -1,   191,
     192,   193,   194,    26,   196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,   182,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   191,    46,
     193,   194,    49,   196,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
      -1,   178,    -1,    -1,   181,   182,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   191,   192,   193,   194,    26,   196,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,   174,   175,   176,    -1,
     178,    -1,    -1,   181,   182,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   191,    -1,   193,   194,    26,   196,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,    -1,   178,
      -1,    -1,   181,   182,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   191,   192,   193,   194,    26,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   175,   176,    -1,   178,    -1,
      -1,   181,   182,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   191,   192,   193,   194,    26,   196,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,    -1,   178,    -1,    -1,
     181,   182,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     191,   192,   193,   194,    26,   196,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,
     182,    14,    15,    16,    17,    18,    -1,    -1,    -1,   191,
     192,   193,   194,    26,   196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,   182,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   191,   192,
     193,   194,    26,   196,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     174,   175,   176,    -1,   178,    -1,    -1,   181,   182,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   191,    46,   193,
     194,    49,   196,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,    -1,
     178,    -1,    -1,   181,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,    -1,   193,   194,    -1,   196,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    20,    21,    -1,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,   137,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,    -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    20,    21,    -1,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
     137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,    -1,    -1,    -1,    -1,    -1,   195,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    20,    21,    -1,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,
     195,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    20,
      21,    -1,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,   137,   138,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,   167,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,
      -1,    -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,   188,   189,   106,
     107,   108,   109,   110,   195,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    20,    21,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,   188,   189,   113,    -1,   192,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,
      -1,    -1,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,    -1,    -1,   192,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,   192,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,   192,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,
     192,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      20,    21,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,
      -1,   192,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    20,    21,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,
      -1,    -1,   192,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    20,    21,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,    -1,    -1,   192,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    20,    21,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,   189,    -1,    -1,   192,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,    -1,    -1,   192,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,    -1,    -1,   192,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,    -1,    -1,   192,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,   192,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,   192,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,
     192,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      20,    21,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,
      -1,   192,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    20,    21,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,
      -1,    -1,   192,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    20,    21,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,    -1,    -1,   192,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    20,    21,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,   189,    -1,    -1,   192,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,    -1,    -1,   192,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,    -1,    -1,   192,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,    -1,    -1,   192,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,   192,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,   192,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,
     192,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      20,    21,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,
      -1,   192,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    20,    21,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,
     190,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    20,    21,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    20,    21,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    -1,    -1,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    20,    21,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    20,
      21,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,    20,    21,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,   137,   138,   139,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,   167,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    20,    21,
     118,   119,   120,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,   181,   182,    -1,    -1,    -1,    18,    -1,
     188,   189,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,   131,
      -1,   133,   134,   135,    64,    65,    66,   139,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,   167,    -1,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,    20,
      21,   106,   107,   108,   109,   110,   188,   189,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,   188,   189,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,    -1,    -1,   167,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      20,    21,   106,   107,   108,   109,   110,   188,   189,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,   188,   189,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      20,    21,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   188,   189,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    20,    21,    -1,
     133,   134,   135,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    20,
      21,    -1,    -1,   133,   167,   135,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   188,   189,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    20,    21,    -1,    -1,
     133,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,   110,   188,   189,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    20,    21,   133,
     134,   135,    -1,    -1,    -1,   139,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,   173,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   188,   189,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
     173,   174,   175,   176,   177,   178,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,
      -1,    62,    -1,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    18,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,   157,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,   157,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,   157,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,   145,    -1,    -1,   148,   149,   150,    -1,
      -1,    -1,    -1,    -1,    62,   157,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,   157,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      70,    71,    72,   157,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   200,     0,     6,    29,    31,    33,    39,    48,    54,
      73,    94,   189,   195,   203,   211,   212,   216,   239,   243,
     261,   328,   334,   337,   343,   381,   384,    18,    19,   157,
     233,   234,   235,   151,   217,   218,   157,   178,   213,   214,
     157,   196,   331,   157,   193,   202,   385,   382,    33,    61,
     132,   157,   236,   237,   238,   252,     4,     5,     7,    35,
     341,    60,   326,   166,   165,   168,   165,   213,    21,    55,
     177,   188,   215,   332,   331,   333,   326,   157,   157,   157,
     132,   191,   165,   190,    55,    61,   244,   246,    55,    61,
     335,    55,    61,   342,    55,    61,   327,    14,    15,   151,
     155,   157,   158,   191,   205,   234,   151,   218,   157,   157,
     157,   166,   195,   197,   331,    55,    61,   201,   196,   383,
     157,   235,   237,   247,   157,   336,   344,   196,   329,   159,
     160,   204,    14,    15,   151,   155,   157,   205,   231,   232,
     215,    24,    30,    47,    62,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   145,   252,   347,   349,
     350,   352,   356,   195,   157,   351,   196,   192,    34,    64,
      66,    68,    69,    70,    71,    72,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    84,    85,    86,    89,    90,
      91,    92,   108,   109,   157,   242,   245,   168,   196,    97,
     339,   340,   324,   148,   303,   159,   160,   161,   165,   192,
     172,   172,   172,   172,   191,   172,   172,   172,   172,   172,
     172,   191,    32,    58,    59,   118,   166,   167,   171,   174,
     189,   198,   195,   157,   316,   317,    20,    21,    37,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   118,   119,   120,
     126,   127,   128,   129,   130,   133,   134,   135,   136,   137,
     138,   139,   167,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   181,   182,   188,   189,    34,    34,   191,
     240,   196,   248,    68,    72,    89,    90,    91,    92,   348,
     330,   157,   345,   197,   325,   235,   141,   157,   322,   323,
     231,   359,   361,   363,   357,   157,   353,   365,   367,   369,
     371,   373,   375,   377,   379,    14,    15,    16,    17,    18,
      26,    38,    43,    46,    49,    53,    63,    73,    95,   108,
     109,   140,   141,   142,   143,   144,   146,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   158,   174,   175,   176,
     178,   181,   182,   191,   193,   194,   207,   209,   248,   252,
     254,   263,   270,   273,   276,   280,   281,   284,   285,   287,
     292,   295,   302,   347,   386,   390,   395,   397,   399,    32,
      32,   171,   189,   198,   190,   295,   157,   197,   168,   195,
     190,    21,   190,   144,   192,   303,   312,   313,   168,   241,
     250,   196,   157,   197,   168,   338,   196,   303,   190,   191,
      41,   165,   168,   171,   321,   356,   355,   356,   356,   356,
     192,   351,   356,   240,   356,   240,   356,   240,   157,   314,
     315,   356,   317,   356,   386,   191,   172,   191,   172,   172,
     191,   172,   191,   172,   295,   295,   191,   191,   191,   191,
     191,   191,    12,   356,    12,   356,   295,   392,   396,   206,
     295,   295,   295,   252,   295,   295,   295,   194,   157,   191,
     239,    20,    21,   106,   107,   108,   109,   110,   113,   114,
     115,   116,   118,   119,   120,   121,   125,   127,   128,   133,
     134,   135,   139,   167,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   188,   189,   192,   191,   190,   190,
     356,   197,   316,   191,    48,   141,   157,   319,   346,   192,
     195,   356,     1,     8,     9,    10,    12,    25,    27,    28,
      37,    40,    42,    50,    52,    56,    57,    63,    96,   123,
     124,   195,   197,   219,   220,   223,   224,   225,   226,   227,
     228,   229,   249,   251,   253,   255,   256,   257,   259,   260,
     261,   262,   282,   283,   295,   330,   166,   195,   252,   308,
     323,   190,   295,   157,   157,   356,   117,   126,   166,   320,
     173,   173,   195,   173,   173,   173,   195,   173,   241,   173,
     241,   173,   241,   168,   173,   195,   173,   195,   191,   252,
     271,   295,   264,   266,   295,   268,   295,   356,   295,   295,
     295,   295,   295,   295,   346,    51,   157,   174,   191,   295,
     387,   388,   389,   391,   392,   393,   394,   346,   191,   388,
     394,   131,   195,   197,   161,   162,   204,   210,   192,   141,
     146,   172,   252,   286,   189,   192,   274,   295,   148,   279,
      18,   147,   157,   347,    18,   147,   157,   347,   295,   295,
     295,   295,   295,   295,   157,   295,   147,   157,   295,   295,
     295,   295,   295,   295,   295,   295,   295,    21,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   119,
     120,   147,   157,   188,   189,   293,   295,   192,   274,   295,
     319,   191,    41,   117,   165,   166,   168,   171,   318,   312,
     295,   346,   117,   195,   251,   282,   295,   248,   295,   295,
     157,   195,   151,    56,   295,   248,   117,   251,   295,   194,
     280,   280,    36,   195,   195,   295,   195,   195,   195,   323,
     122,   195,    98,    99,   100,   101,   102,   103,   104,   105,
     111,   112,   117,   126,   129,   130,   136,   137,   138,   166,
     197,   295,   189,   197,   239,   309,   192,    41,   166,   195,
     320,   251,   295,   360,   362,   356,   364,   358,   354,   366,
     173,   370,   173,   374,   173,   356,   378,   314,   380,   192,
     274,   172,   295,   356,   192,   356,   356,   192,   356,   192,
     173,   192,   192,   192,   192,   192,   192,    19,   280,   126,
     318,   192,   165,   195,   389,   190,   165,   190,   195,    19,
     192,   389,   197,   295,   392,   197,   295,   159,   208,   191,
     191,   288,   290,   157,   387,   165,   192,   117,   126,   166,
     171,   277,   278,   240,   172,   191,   172,   191,   191,   191,
     190,    18,   147,   157,   347,   168,   147,   157,   295,   191,
     147,   157,   295,     1,   190,   192,   192,   295,   157,   157,
     356,   251,   295,   248,    19,   251,   295,     9,   117,   166,
     195,    13,   248,   166,   168,   151,   251,   295,   195,   194,
     196,   248,   280,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   123,   124,   295,   295,   295,   295,   295,
     295,   295,   123,   124,   295,   195,   238,     7,   303,   307,
     157,   251,   295,   195,   368,   372,   376,   192,   157,   192,
     173,   173,   173,   173,   279,   191,   274,   295,   295,   388,
     389,   157,   387,   190,   190,   295,   190,   393,   274,   388,
     197,   190,   163,   159,   295,   295,   356,   240,   190,   295,
     157,   157,   157,   157,   165,   190,   241,   296,   295,   298,
     295,   192,   274,   295,   172,   191,   295,   191,   190,   295,
     191,   190,   294,   192,    41,   166,   318,   274,   195,   258,
     248,   230,   195,   250,    11,    22,    23,   221,   222,   295,
     295,   295,   295,   190,    55,    61,   306,    44,    97,   304,
     195,   195,   173,   195,   272,   265,   267,   269,   191,   192,
     274,   195,   389,   190,   126,   318,   190,   195,   389,   190,
     192,   192,   173,   241,   192,   277,   190,   131,   248,   275,
     356,   192,   356,   192,   192,   192,   300,   295,   295,   192,
     295,   157,   295,   195,   295,   295,   197,   248,   295,   195,
      45,   304,   306,   295,   157,   295,   295,   295,   192,   295,
     192,   295,   190,   190,   295,   295,   295,   197,   289,   173,
     117,   295,   173,   173,   356,   192,   192,   192,   248,   195,
     248,   310,    32,   305,   319,   192,   173,   192,   195,   398,
     190,   398,   190,   286,   291,   295,   297,   299,   173,   222,
     245,   311,   295,    51,   190,   197,   286,   301,   195,   245,
     192,   295,   190,   190,   248
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   199,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   201,   201,   201,   202,   202,
     203,   204,   204,   204,   204,   205,   206,   206,   206,   207,
     208,   208,   210,   209,   211,   212,   213,   213,   213,   213,
     214,   214,   215,   215,   216,   217,   217,   218,   218,   219,
     220,   220,   221,   221,   222,   222,   222,   223,   223,   224,
     225,   226,   227,   228,   230,   229,   231,   231,   231,   231,
     231,   231,   232,   232,   233,   233,   233,   234,   234,   234,
     234,   234,   234,   234,   234,   235,   235,   236,   236,   236,
     237,   237,   238,   238,   239,   239,   240,   240,   240,   241,
     241,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   243,   244,
     244,   244,   245,   247,   246,   248,   248,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   250,   250,   250,
     251,   251,   251,   252,   252,   252,   253,   254,   254,   254,
     254,   255,   256,   257,   257,   257,   257,   257,   258,   257,
     259,   259,   259,   259,   260,   261,   261,   262,   264,   265,
     263,   266,   267,   263,   268,   269,   263,   271,   272,   270,
     273,   273,   273,   274,   274,   275,   275,   275,   276,   276,
     276,   277,   277,   277,   277,   278,   278,   279,   279,   280,
     280,   281,   281,   281,   281,   281,   281,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   283,   283,   283,   283,
     284,   285,   285,   286,   286,   287,   288,   289,   287,   290,
     291,   287,   292,   292,   293,   294,   292,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   296,   297,   295,   295,
     295,   295,   298,   299,   295,   295,   295,   300,   301,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   303,   303,   304,
     304,   304,   305,   305,   306,   306,   306,   307,   308,   309,
     308,   310,   308,   311,   308,   308,   312,   312,   312,   313,
     313,   314,   314,   315,   315,   316,   317,   317,   318,   318,
     319,   319,   319,   319,   319,   319,   320,   320,   320,   321,
     321,   322,   322,   322,   322,   322,   323,   323,   323,   323,
     323,   324,   325,   324,   326,   326,   327,   327,   327,   328,
     329,   328,   330,   330,   330,   332,   331,   333,   333,   334,
     334,   335,   335,   335,   336,   337,   337,   338,   338,   339,
     339,   340,   341,   341,   342,   342,   342,   344,   345,   343,
     346,   346,   346,   346,   346,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     348,   348,   348,   348,   348,   348,   349,   350,   350,   350,
     351,   351,   353,   354,   352,   355,   355,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   357,   358,   356,   356,   359,   360,
     356,   361,   362,   356,   363,   364,   356,   356,   365,   366,
     356,   367,   368,   356,   356,   369,   370,   356,   371,   372,
     356,   356,   373,   374,   356,   375,   376,   356,   377,   378,
     356,   379,   380,   356,   382,   383,   381,   385,   384,   386,
     386,   386,   386,   387,   387,   387,   387,   388,   388,   389,
     389,   390,   390,   390,   390,   390,   390,   391,   391,   392,
     392,   393,   393,   394,   394,   395,   395,   396,   396,   397,
     398,   398,   399,   399
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     0,     1,     1,     1,     1,
       4,     1,     1,     2,     2,     3,     0,     2,     4,     3,
       1,     2,     0,     4,     2,     2,     1,     2,     3,     3,
       2,     4,     0,     1,     2,     1,     3,     1,     3,     3,
       3,     2,     1,     1,     0,     2,     4,     1,     1,     4,
       6,     2,     3,     3,     0,     5,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     1,     5,     1,     3,     1,     1,     1,
       1,     4,     1,     3,     0,     3,     0,     2,     3,     0,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     2,     2,     2,
       2,     2,     2,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     1,     3,     0,     4,     3,     7,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     2,     2,     2,     0,     2,     2,
       3,     2,     2,     1,     3,     2,     2,     2,     4,     5,
       2,     1,     1,     2,     3,     4,     2,     3,     0,     6,
       3,     4,     2,     3,     4,     1,     1,     2,     0,     0,
       7,     0,     0,     7,     0,     0,     7,     0,     0,     6,
       5,     8,    10,     1,     3,     1,     2,     3,     1,     1,
       2,     2,     2,     2,     2,     1,     3,     0,     4,     1,
       6,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     4,     4,
       6,     5,     6,     1,     4,     3,     0,     0,     8,     0,
       0,     9,     3,     4,     0,     0,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     3,     4,     5,
       4,     5,     3,     4,     1,     3,     4,     3,     4,     2,
       4,     4,     7,     8,     3,     5,     0,     0,     8,     3,
       3,     3,     0,     0,     8,     3,     4,     0,     0,     9,
       4,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     4,     4,     4,     4,     4,     4,     6,     7,     6,
       6,     7,     7,     6,     7,     6,     6,     0,     4,     0,
       1,     1,     0,     1,     0,     1,     1,     4,     0,     0,
       4,     0,     8,     0,     9,     5,     2,     3,     4,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     1,
       1,     2,     3,     5,     3,     3,     1,     1,     1,     0,
       1,     1,     4,     3,     3,     5,     4,     6,     5,     5,
       4,     0,     0,     4,     0,     1,     0,     1,     1,     6,
       0,     6,     0,     3,     5,     0,     4,     2,     3,     4,
       2,     0,     1,     1,     1,     7,     9,     0,     2,     0,
       1,     3,     1,     1,     0,     1,     1,     0,     0,     9,
       1,     4,     3,     3,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     3,     0,     0,     6,     1,     3,     1,     1,     1,
       1,     4,     3,     4,     2,     2,     3,     2,     3,     2,
       2,     3,     3,     2,     0,     0,     6,     2,     0,     0,
       6,     0,     0,     6,     0,     0,     6,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     0,     0,
       6,     0,     0,     6,     0,     0,     8,     0,     7,     1,
       1,     1,     1,     3,     3,     5,     5,     1,     3,     0,
       2,     6,     5,     7,     8,     6,     8,     1,     3,     3,
       1,     1,     1,     1,     3,     5,     5,     1,     3,     4,
       0,     3,    10,    10
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = DAS_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == DAS_YYEMPTY)                                        \
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
   Use DAS_YYerror or DAS_YYUNDEF. */
#define YYERRCODE DAS_YYUNDEF

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
#if DAS_YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL

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

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


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
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
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

  YY_LOCATION_PRINT (yyo, *yylocationp);
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
#else /* !DAS_YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !DAS_YYDEBUG */


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

    case YYSYMBOL_expression_if_then_else: /* expression_if_then_else  */
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

    case YYSYMBOL_annotation_declaration_name: /* annotation_declaration_name  */
            { delete ((*yyvaluep).s); }
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

    case YYSYMBOL_expression_block: /* expression_block  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_any: /* expression_any  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expressions: /* expressions  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_pipe: /* expr_pipe  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_name_in_namespace: /* name_in_namespace  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_expression_delete: /* expression_delete  */
            { delete ((*yyvaluep).pExpression); }
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

    case YYSYMBOL_expr_block: /* expr_block  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_numeric_const: /* expr_numeric_const  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_assign: /* expr_assign  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_assign_pipe: /* expr_assign_pipe  */
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

    case YYSYMBOL_expr: /* expr  */
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

    case YYSYMBOL_variant_type: /* variant_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_variant_type_list: /* variant_type_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_variable_declaration: /* variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_let_variable_name_with_pos_list: /* let_variable_name_with_pos_list  */
            { delete ((*yyvaluep).pNameWithPosList); }
        break;

    case YYSYMBOL_let_variable_declaration: /* let_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_global_variable_declaration_list: /* global_variable_declaration_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
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

    case YYSYMBOL_bitfield_type_declaration: /* bitfield_type_declaration  */
            { delete ((*yyvaluep).pTypeDecl); }
        break;

    case YYSYMBOL_table_type_pair: /* table_type_pair  */
            { delete ((*yyvaluep).aTypePair).firstType; ((*yyvaluep).aTypePair).secondType; }
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

    case YYSYMBOL_make_struct_dim: /* make_struct_dim  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_optional_block: /* optional_block  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_decl: /* make_struct_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_tuple: /* make_tuple  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_map_tuple: /* make_map_tuple  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_any_tuple: /* make_any_tuple  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_dim: /* make_dim  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_dim_decl: /* make_dim_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_table: /* make_table  */
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
# if defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL
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

  yychar = DAS_YYEMPTY; /* Cause a token to be read.  */
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
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
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
  if (yychar == DAS_YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= DAS_YYEOF)
    {
      yychar = DAS_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == DAS_YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = DAS_YYUNDEF;
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
  yychar = DAS_YYEMPTY;
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
  case 15: /* optional_public_or_private_module: %empty  */
                        { (yyval.b) = yyextra->g_Program->policies.default_module_public; }
    break;

  case 16: /* optional_public_or_private_module: "public"  */
                        { (yyval.b) = true; }
    break;

  case 17: /* optional_public_or_private_module: "private"  */
                        { (yyval.b) = false; }
    break;

  case 18: /* module_name: '$'  */
                    { (yyval.s) = new string("$"); }
    break;

  case 19: /* module_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 20: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module  */
                                                                                                      {
        yyextra->g_Program->thisModuleName = *(yyvsp[-2].s);
        yyextra->g_Program->thisModule->isPublic = (yyvsp[0].b);
        yyextra->g_Program->thisModule->isModule = true;
        if ( yyextra->g_Program->thisModule->name.empty() ) {
            yyextra->g_Program->thisModule->name = *(yyvsp[-2].s);
        } else if ( yyextra->g_Program->thisModule->name != *(yyvsp[-2].s) ){
            das_yyerror(scanner,"this module already has a name " + yyextra->g_Program->thisModule->name,tokAt(scanner,(yylsp[-2])),
                CompilationError::module_already_has_a_name);
        }
        if ( !yyextra->g_Program->policies.ignore_shared_modules ) {
            yyextra->g_Program->promoteToBuiltin = (yyvsp[-1].b);
        }
        delete (yyvsp[-2].s);
    }
    break;

  case 21: /* character_sequence: STRING_CHARACTER  */
                                                            { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 22: /* character_sequence: STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 23: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 24: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 25: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 26: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 27: /* string_builder_body: string_builder_body character_sequence  */
                                                           {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror(scanner,"invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt(scanner,(yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 28: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 29: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 30: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 31: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 32: /* $@1: %empty  */
                                        {
        auto macros = yyextra->g_Program->getReaderMacro(*(yyvsp[0].s));
        if ( macros.size()==0 ) {
            das_yyerror(scanner,"reader macro " + *(yyvsp[0].s) + " not found",tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( macros.size()>1 ) {
            string options;
            for ( auto & x : macros ) {
                options += "\t" + x->module->name + "::" + x->name + "\n";
            }
            das_yyerror(scanner,"too many options for the reader macro " + *(yyvsp[0].s) +  "\n" + options, tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( yychar != '~' ) {
            das_yyerror(scanner,"expecting ~ after the reader macro", tokAt(scanner,(yylsp[0])),
                CompilationError::syntax_error);
        } else {
            yyextra->g_ReaderMacro = macros.back().get();
            yyextra->g_ReaderExpr = new ExprReader(tokAt(scanner,(yylsp[-1])),yyextra->g_ReaderMacro);
            yyclearin ;
            das_yybegin_reader(scanner);
        }
    }
    break;

  case 33: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
                                     {
        yyextra->g_ReaderExpr->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = yyextra->g_ReaderExpr;
        delete (yyvsp[-2].s);
        yyextra->g_ReaderMacro = nullptr;
        yyextra->g_ReaderExpr = nullptr;
    }
    break;

  case 34: /* options_declaration: "options" annotation_argument_list  */
                                                   {
        if ( yyextra->g_Program->options.size() ) {
            yyextra->g_Program->options.insert ( yyextra->g_Program->options.begin(),
                (yyvsp[0].aaList)->begin(), (yyvsp[0].aaList)->end() );
        } else {
            swap ( yyextra->g_Program->options, *(yyvsp[0].aaList) );
        }
        auto opt = yyextra->g_Program->options.find("indenting", tInt);
        if (opt)
        {
            if (opt->iValue != 0 && opt->iValue != 2 && opt->iValue != 4 && opt->iValue != 8)//this is error
                yyextra->das_tab_size = yyextra->das_def_tab_size;
            else
                yyextra->das_tab_size = opt->iValue ? opt->iValue : yyextra->das_def_tab_size;//0 is default
            yyextra->g_FileAccessStack.back()->tabSize = yyextra->das_tab_size;
        }
        delete (yyvsp[0].aaList);
    }
    break;

  case 36: /* require_module_name: "name"  */
                   {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 37: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 38: /* require_module_name: require_module_name '.' "name"  */
                                                {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 39: /* require_module_name: require_module_name '/' "name"  */
                                                {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 40: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 41: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 42: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 43: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 47: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 48: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 49: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 50: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 51: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 52: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 53: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 54: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 55: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 56: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 57: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 58: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 59: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 60: /* expression_for_loop: "for" variable_name_with_pos_list "in" expr_list ';' expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 61: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 62: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 63: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 64: /* $@2: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 65: /* expression_with_alias: "assume" "name" '=' $@2 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 66: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 67: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 68: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 69: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 70: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 71: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 72: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 73: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 74: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 75: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 76: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 77: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 78: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 79: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 80: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 81: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 82: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 83: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 84: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 85: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 86: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 87: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 88: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 89: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 90: /* annotation_declaration: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
    break;

  case 91: /* annotation_declaration: annotation_declaration_name '(' annotation_argument_list ')'  */
                                                                                 {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
    break;

  case 92: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 93: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 94: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 95: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 96: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 97: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 98: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 99: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 100: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 101: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 102: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 103: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 104: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 105: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 106: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 107: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 108: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 109: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 110: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 111: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 112: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 113: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 114: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 115: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 116: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 117: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 118: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 119: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 120: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 121: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 122: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 123: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 124: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 125: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 126: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 127: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 128: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 129: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 130: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 131: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 132: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 133: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 134: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 135: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 136: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 137: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 138: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 139: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 140: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 141: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 142: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 143: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 144: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 145: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 146: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 147: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 148: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 149: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 150: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 151: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 152: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 153: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 154: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 155: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 156: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 157: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 158: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 159: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 160: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 161: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 162: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 163: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 164: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 165: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 166: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 167: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 168: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 169: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 170: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 171: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 172: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 173: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 174: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 175: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 176: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 177: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 178: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
                                                                                {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-1]),(yylsp[0]));
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations(scanner, (yyvsp[0].pFuncDecl), (yyvsp[-2].faList), tokAt(scanner,(yylsp[-2])));
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            if ( !yyextra->g_Program->addGeneric((yyvsp[0].pFuncDecl)) ) {
                das_yyerror(scanner,"generic function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::function_already_declared);
            }
        } else {
            if ( !yyextra->g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das_yyerror(scanner,"function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::function_already_declared);
            }
        }
        (yyvsp[0].pFuncDecl)->delRef();
    }
    break;

  case 179: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 180: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 181: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 182: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 183: /* $@3: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 184: /* function_declaration: optional_public_or_private_function $@3 function_declaration_header expression_block  */
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

  case 185: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 186: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 187: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 188: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 189: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 190: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 191: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 192: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 193: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 194: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 195: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 196: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 197: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 198: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 199: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 200: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 201: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 202: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 203: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 204: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 205: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 206: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 207: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 208: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 209: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 210: /* expr_pipe: expr_assign " <|" expr_block  */
                                                        {
        Expression * pipeCall = (yyvsp[-2].pExpression)->tail();
        if ( pipeCall->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) pipeCall;
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else if ( pipeCall->rtti_isVar() ) {
            // a += b <| c
            auto pVar = (ExprVar *) pipeCall;
            auto pCall = yyextra->g_Program->makeCall(pVar->at,pVar->name);
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            if ( !(yyvsp[-2].pExpression)->swap_tail(pVar,pCall) ) {
                delete pVar;
                (yyval.pExpression) = pCall;
            } else {
                (yyval.pExpression) = (yyvsp[-2].pExpression);
            }
        } else if ( pipeCall->rtti_isMakeStruct() ) {
            auto pMS = (ExprMakeStruct *) pipeCall;
            if ( pMS->block ) {
                das_yyerror(scanner,"can't pipe into [[ make structure ]]. it already has where closure",
                    tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
                delete (yyvsp[0].pExpression);
            } else {
                pMS->block = (yyvsp[0].pExpression);
            }
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das_yyerror(scanner,"can only pipe into function call or [[ make structure ]]",
                tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
            delete (yyvsp[0].pExpression);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
    break;

  case 211: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 212: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 213: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 214: /* name_in_namespace: "name" "::" "name"  */
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

  case 215: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 216: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 217: /* expr_new: "new" type_declaration  */
                                                   {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 218: /* expr_new: "new" type_declaration '(' ')'  */
                                                           {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 219: /* expr_new: "new" type_declaration '(' expr_list ')'  */
                                                                                {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 220: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 221: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 222: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 223: /* expression_return: "return" ';'  */
                            {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),nullptr);
    }
    break;

  case 224: /* expression_return: "return" expr ';'  */
                                          {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 225: /* expression_return: "return" "<-" expr ';'  */
                                                 {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 226: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 227: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 228: /* $@4: %empty  */
                                                    { yyextra->das_need_oxford_comma=true; }
    break;

  case 229: /* expression_return: "return" expr "if" $@4 expr ';'  */
                                                                                                            {
        (yyval.pExpression) = new ExprIfThenElse ( tokAt(scanner,(yylsp[-3])), (yyvsp[-1].pExpression),
            make_smart<ExprReturn>(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-4].pExpression))), nullptr);
    }
    break;

  case 230: /* expression_yield: "yield" expr ';'  */
                                         {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 231: /* expression_yield: "yield" "<-" expr ';'  */
                                                {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 232: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 233: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 234: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 235: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 236: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 237: /* expression_let: kwd_let let_variable_declaration  */
                                               {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 238: /* $@5: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 239: /* $@6: %empty  */
                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 240: /* expr_cast: "cast" '<' $@5 type_declaration '>' $@6 expr  */
                                                                                                                                     {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 241: /* $@7: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 242: /* $@8: %empty  */
                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 243: /* expr_cast: "upcast" '<' $@7 type_declaration '>' $@8 expr  */
                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 244: /* $@9: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 245: /* $@10: %empty  */
                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 246: /* expr_cast: "reinterpret" '<' $@9 type_declaration '>' $@10 expr  */
                                                                                                                                            {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 247: /* $@11: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 248: /* $@12: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 249: /* expr_type_decl: "type" '<' $@11 type_declaration '>' $@12  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 250: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
                                                                         {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            }
            delete (yyvsp[-2].s);
    }
    break;

  case 251: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),ptd->typeexpr,*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            }
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
    break;

  case 252: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
                                                                                                                     {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),ptd->typeexpr,*(yyvsp[-5].s),*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            }
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
    break;

  case 253: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 254: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 255: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 256: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 257: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 258: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 259: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 260: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 261: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 262: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 263: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 264: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 265: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 266: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 267: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 268: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 269: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 270: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 271: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 272: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 273: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 274: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 275: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 276: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 277: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 278: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 279: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 280: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 281: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 282: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 283: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 284: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 285: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 286: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 287: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 288: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 289: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 290: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 291: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 292: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 293: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 294: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 295: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 296: /* expr_assign_pipe: expr '=' "@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 297: /* expr_assign_pipe: expr '=' "@@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 298: /* expr_assign_pipe: expr "<-" "@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 299: /* expr_assign_pipe: expr "<-" "@@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 300: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 301: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 302: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 303: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 304: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 305: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 306: /* $@13: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 307: /* $@14: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 308: /* func_addr_expr: '@' '@' '<' $@13 type_declaration '>' $@14 func_addr_name  */
                                                                                                                                            {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 309: /* $@15: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 310: /* $@16: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 311: /* func_addr_expr: '@' '@' '<' $@15 optional_function_argument_list optional_function_type '>' $@16 func_addr_name  */
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

  case 312: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 313: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 314: /* $@17: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 315: /* $@18: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 316: /* expr_field: expr '.' $@17 error $@18  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 317: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 318: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 319: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 320: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 321: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 322: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 323: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 324: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 325: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 326: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 327: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 335: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 344: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 345: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 353: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 356: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 357: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 358: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 359: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 360: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 361: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 362: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 363: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 364: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 366: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 367: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 368: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 369: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 371: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 372: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' ')'  */
                                                                                                   {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 373: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' expr ')'  */
                                                                                                                 {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 374: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 376: /* $@19: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 377: /* $@20: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 378: /* expr: expr "is" "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                                            {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 379: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 380: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 381: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 382: /* $@21: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 383: /* $@22: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 384: /* expr: expr "as" "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 385: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 386: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 387: /* $@23: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 388: /* $@24: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 389: /* expr: expr '?' "as" "type" '<' $@23 type_declaration '>' $@24  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 390: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 391: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 392: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 393: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 394: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 395: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 396: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 397: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 398: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 399: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) =ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 400: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 401: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 402: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 403: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 404: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 405: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 406: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 407: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 408: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 409: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 410: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 411: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 412: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 413: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 414: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 415: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 416: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 417: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 418: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 419: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 420: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 421: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 422: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 423: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 424: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 425: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 426: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 427: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 428: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 429: /* $@25: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 430: /* struct_variable_declaration_list: struct_variable_declaration_list $@25 structure_variable_declaration ';'  */
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

  case 431: /* $@26: %empty  */
                                                                                             {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 432: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@26 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
            }
    break;

  case 433: /* $@27: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 434: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@27 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 435: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 436: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 437: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 438: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 439: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 440: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 441: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 442: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 443: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 444: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 445: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 446: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 447: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 448: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 449: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 450: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 451: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 452: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 453: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 454: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 455: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 456: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 457: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 458: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 459: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 460: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 461: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 462: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 463: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 464: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 465: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 466: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration ';'  */
                                                                                 {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 467: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr ';'  */
                                                                                                                       {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 468: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr_pipe  */
                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 469: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 470: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 471: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 472: /* $@28: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 473: /* global_variable_declaration_list: global_variable_declaration_list $@28 optional_field_annotation let_variable_declaration  */
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

  case 474: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 475: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 476: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 477: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 478: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 479: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 480: /* $@29: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 481: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@29 optional_field_annotation let_variable_declaration  */
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

  case 482: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 483: /* enum_list: enum_list "name" ';'  */
                                     {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt(scanner,(yylsp[-1]))) ) {
            das_yyerror(scanner,"enumeration alread declared " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
    break;

  case 484: /* enum_list: enum_list "name" '=' expr ';'  */
                                                     {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt(scanner,(yylsp[-3]))) ) {
            das_yyerror(scanner,"enumeration value alread declared " + *(yyvsp[-3].s), tokAt(scanner,(yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
    break;

  case 485: /* $@30: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 486: /* single_alias: "name" $@30 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterAlias((yyvsp[-3].s)->c_str(),pubename);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 491: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 492: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 493: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 494: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 495: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 496: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 497: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 498: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 499: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 500: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 501: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 502: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 503: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 504: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 505: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 506: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 507: /* $@31: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 508: /* $@32: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 509: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@31 structure_name $@32 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
        }
    }
    break;

  case 510: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 511: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 512: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 513: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 514: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 515: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 516: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 517: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 518: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 519: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 520: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 521: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 522: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 523: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 524: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 525: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 526: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 527: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 528: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 529: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 530: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 531: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 532: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 533: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 534: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 535: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 536: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 537: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 538: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 539: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 540: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 541: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 542: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 543: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 544: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 545: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 546: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 547: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 548: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 549: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 550: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 551: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 552: /* $@33: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 553: /* $@34: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 554: /* bitfield_type_declaration: "bitfield" '<' $@33 bitfield_bits '>' $@34  */
                                                                                                                             {
            (yyval.pTypeDecl) = new TypeDecl(Type::tBitfield);
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            if ( (yyval.pTypeDecl)->argNames.size()>32 ) {
                das_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                    CompilationError::invalid_type);
            }
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
    break;

  case 555: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 556: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 557: /* type_declaration: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 558: /* type_declaration: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 559: /* type_declaration: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 560: /* type_declaration: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 561: /* type_declaration: type_declaration '[' expr ']'  */
                                                         {
        int32_t dI = TypeDecl::dimConst;
        if ( (yyvsp[-1].pExpression)->rtti_isConstant() ) {                // note: this shortcut is here so we don`t get extra infer pass on every array
            auto cI = (ExprConst *) (yyvsp[-1].pExpression);
            auto bt = cI->baseType;
            if ( bt==Type::tInt || bt==Type::tUInt ) {
                dI = cast<int32_t>::to(cI->value);
            }
        }
        (yyvsp[-3].pTypeDecl)->dim.push_back(dI);
        (yyvsp[-3].pTypeDecl)->dimExpr.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyvsp[-3].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 562: /* type_declaration: type_declaration '[' ']'  */
                                           {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 563: /* type_declaration: type_declaration '-' '[' ']'  */
                                               {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 564: /* type_declaration: type_declaration "explicit"  */
                                                {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 565: /* type_declaration: type_declaration "const"  */
                                             {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 566: /* type_declaration: type_declaration '-' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 567: /* type_declaration: type_declaration '&'  */
                                       {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 568: /* type_declaration: type_declaration '-' '&'  */
                                           {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 569: /* type_declaration: type_declaration '#'  */
                                       {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 570: /* type_declaration: type_declaration "implicit"  */
                                                {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 571: /* type_declaration: type_declaration '-' '#'  */
                                           {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 572: /* type_declaration: type_declaration '=' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 573: /* type_declaration: type_declaration '?'  */
                                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 574: /* $@35: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 575: /* $@36: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 576: /* type_declaration: "smart_ptr" '<' $@35 type_declaration '>' $@36  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 577: /* type_declaration: type_declaration "??"  */
                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 578: /* $@37: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 579: /* $@38: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 580: /* type_declaration: "array" '<' $@37 type_declaration '>' $@38  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 581: /* $@39: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 582: /* $@40: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 583: /* type_declaration: "table" '<' $@39 table_type_pair '>' $@40  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 584: /* $@41: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 585: /* $@42: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 586: /* type_declaration: "iterator" '<' $@41 type_declaration '>' $@42  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 587: /* type_declaration: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 588: /* $@43: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 589: /* $@44: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 590: /* type_declaration: "block" '<' $@43 type_declaration '>' $@44  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 591: /* $@45: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 592: /* $@46: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 593: /* type_declaration: "block" '<' $@45 optional_function_argument_list optional_function_type '>' $@46  */
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

  case 594: /* type_declaration: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 595: /* $@47: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 596: /* $@48: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 597: /* type_declaration: "function" '<' $@47 type_declaration '>' $@48  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 598: /* $@49: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 599: /* $@50: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 600: /* type_declaration: "function" '<' $@49 optional_function_argument_list optional_function_type '>' $@50  */
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

  case 601: /* type_declaration: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 602: /* $@51: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 603: /* $@52: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 604: /* type_declaration: "lambda" '<' $@51 type_declaration '>' $@52  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 605: /* $@53: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 606: /* $@54: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 607: /* type_declaration: "lambda" '<' $@53 optional_function_argument_list optional_function_type '>' $@54  */
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

  case 608: /* $@55: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 609: /* $@56: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 610: /* type_declaration: "tuple" '<' $@55 tuple_type_list '>' $@56  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 611: /* $@57: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 612: /* $@58: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 613: /* type_declaration: "variant" '<' $@57 variant_type_list '>' $@58  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 614: /* $@59: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 615: /* $@60: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 616: /* variant_alias_declaration: "variant" $@59 "name" $@60 '{' variant_type_list ';' '}'  */
                                          {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-5].s);
        vtype->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-5].s),tokAt(scanner,(yylsp[-5])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-5].s)->c_str(),atvname);
        }
        delete (yyvsp[-5].s);
    }
    break;

  case 617: /* $@61: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 618: /* bitfield_alias_declaration: "bitfield" $@61 "name" '{' bitfield_bits ';' '}'  */
                                                                                                            {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-4].s);
        btype->at = tokAt(scanner,(yylsp[-4]));
        btype->argNames = *(yyvsp[-2].pNameList);
        if ( btype->argNames.size()>32 ) {
            das_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-4])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-4].s),tokAt(scanner,(yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
        delete (yyvsp[-2].pNameList);
    }
    break;

  case 619: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 620: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 621: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 622: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 623: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 624: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 625: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 626: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 627: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 628: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 629: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 630: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 631: /* make_struct_decl: "[[" type_declaration make_struct_dim optional_block ']' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 632: /* make_struct_decl: "[[" type_declaration optional_block ']' ']'  */
                                                                        {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 633: /* make_struct_decl: "[[" type_declaration '(' ')' optional_block ']' ']'  */
                                                                                {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 634: /* make_struct_decl: "[[" type_declaration '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                     {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 635: /* make_struct_decl: "[{" type_declaration make_struct_dim optional_block '}' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 636: /* make_struct_decl: "[{" type_declaration '(' ')' make_struct_dim optional_block '}' ']'  */
                                                                                                     {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 637: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 638: /* make_tuple: make_tuple ',' expr  */
                                      {
        ExprMakeTuple * mt;
        if ( (yyvsp[-2].pExpression)->rtti_isMakeTuple() ) {
            mt = static_cast<ExprMakeTuple *>((yyvsp[-2].pExpression));
        } else {
            mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
            mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        }
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 639: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 640: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 641: /* make_any_tuple: make_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 642: /* make_any_tuple: make_map_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 643: /* make_dim: make_any_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 644: /* make_dim: make_dim ';' make_any_tuple  */
                                              {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 645: /* make_dim_decl: "[[" type_declaration make_dim ']' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 646: /* make_dim_decl: "[{" type_declaration make_dim '}' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 647: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 648: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 649: /* make_table_decl: "{{" make_table '}' '}'  */
                                              {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
    break;

  case 650: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 651: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 652: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 653: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false);
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
  yytoken = yychar == DAS_YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
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
          goto yyexhaustedlab;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= DAS_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == DAS_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner);
          yychar = DAS_YYEMPTY;
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
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != DAS_YYEMPTY)
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



void das_yyfatalerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr ) {
    yyextra->g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
        lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),cerr);
}

void das_yyerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error ) {
    if ( !yyextra->das_supress_errors ) {
        yyextra->g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
            lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),
                CompilationError::syntax_error);
    }
}

LineInfo tokAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
        li.first_column,li.first_line,
        li.last_column,li.last_line);
}

LineInfo tokRangeAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li, const struct DAS_YYLTYPE & lie ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
        li.first_column,li.first_line,
        lie.last_column,lie.last_line);
}


