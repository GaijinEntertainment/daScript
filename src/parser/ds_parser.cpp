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
  YYSYMBOL_expression_yield = 258,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 259,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 260,                  /* kwd_let  */
  YYSYMBOL_expression_let = 261,           /* expression_let  */
  YYSYMBOL_expr_cast = 262,                /* expr_cast  */
  YYSYMBOL_263_4 = 263,                    /* $@4  */
  YYSYMBOL_264_5 = 264,                    /* $@5  */
  YYSYMBOL_265_6 = 265,                    /* $@6  */
  YYSYMBOL_266_7 = 266,                    /* $@7  */
  YYSYMBOL_267_8 = 267,                    /* $@8  */
  YYSYMBOL_268_9 = 268,                    /* $@9  */
  YYSYMBOL_expr_type_decl = 269,           /* expr_type_decl  */
  YYSYMBOL_270_10 = 270,                   /* $@10  */
  YYSYMBOL_271_11 = 271,                   /* $@11  */
  YYSYMBOL_expr_type_info = 272,           /* expr_type_info  */
  YYSYMBOL_expr_list = 273,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 274,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 275,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 276,            /* capture_entry  */
  YYSYMBOL_capture_list = 277,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 278,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 279,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 280,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 281,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 282,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 283,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 284,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 285,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 286,           /* func_addr_expr  */
  YYSYMBOL_287_12 = 287,                   /* $@12  */
  YYSYMBOL_288_13 = 288,                   /* $@13  */
  YYSYMBOL_289_14 = 289,                   /* $@14  */
  YYSYMBOL_290_15 = 290,                   /* $@15  */
  YYSYMBOL_expr_field = 291,               /* expr_field  */
  YYSYMBOL_292_16 = 292,                   /* $@16  */
  YYSYMBOL_293_17 = 293,                   /* $@17  */
  YYSYMBOL_expr = 294,                     /* expr  */
  YYSYMBOL_295_18 = 295,                   /* $@18  */
  YYSYMBOL_296_19 = 296,                   /* $@19  */
  YYSYMBOL_297_20 = 297,                   /* $@20  */
  YYSYMBOL_298_21 = 298,                   /* $@21  */
  YYSYMBOL_299_22 = 299,                   /* $@22  */
  YYSYMBOL_300_23 = 300,                   /* $@23  */
  YYSYMBOL_expr_mtag = 301,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 302, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 303,        /* optional_override  */
  YYSYMBOL_optional_constant = 304,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 305, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 306, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 307, /* struct_variable_declaration_list  */
  YYSYMBOL_308_24 = 308,                   /* $@24  */
  YYSYMBOL_309_25 = 309,                   /* $@25  */
  YYSYMBOL_310_26 = 310,                   /* $@26  */
  YYSYMBOL_function_argument_declaration = 311, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 312,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 313,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 314,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 315,             /* variant_type  */
  YYSYMBOL_variant_type_list = 316,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 317,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 318,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 319,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 320,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 321, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 322, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 323, /* global_variable_declaration_list  */
  YYSYMBOL_324_27 = 324,                   /* $@27  */
  YYSYMBOL_optional_shared = 325,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 326, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 327,               /* global_let  */
  YYSYMBOL_328_28 = 328,                   /* $@28  */
  YYSYMBOL_enum_list = 329,                /* enum_list  */
  YYSYMBOL_single_alias = 330,             /* single_alias  */
  YYSYMBOL_331_29 = 331,                   /* $@29  */
  YYSYMBOL_alias_list = 332,               /* alias_list  */
  YYSYMBOL_alias_declaration = 333,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 334, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 335,                /* enum_name  */
  YYSYMBOL_enum_declaration = 336,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 337, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 338,          /* optional_sealed  */
  YYSYMBOL_structure_name = 339,           /* structure_name  */
  YYSYMBOL_class_or_struct = 340,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 341, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 342,    /* structure_declaration  */
  YYSYMBOL_343_30 = 343,                   /* $@30  */
  YYSYMBOL_344_31 = 344,                   /* $@31  */
  YYSYMBOL_variable_name_with_pos_list = 345, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 346,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 347, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 348, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 349,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 350,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 351, /* bitfield_type_declaration  */
  YYSYMBOL_352_32 = 352,                   /* $@32  */
  YYSYMBOL_353_33 = 353,                   /* $@33  */
  YYSYMBOL_table_type_pair = 354,          /* table_type_pair  */
  YYSYMBOL_type_declaration = 355,         /* type_declaration  */
  YYSYMBOL_356_34 = 356,                   /* $@34  */
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
  YYSYMBOL_variant_alias_declaration = 380, /* variant_alias_declaration  */
  YYSYMBOL_381_58 = 381,                   /* $@58  */
  YYSYMBOL_382_59 = 382,                   /* $@59  */
  YYSYMBOL_bitfield_alias_declaration = 383, /* bitfield_alias_declaration  */
  YYSYMBOL_384_60 = 384,                   /* $@60  */
  YYSYMBOL_make_decl = 385,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 386,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 387,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 388,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 389,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 390,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 391,           /* make_map_tuple  */
  YYSYMBOL_make_any_tuple = 392,           /* make_any_tuple  */
  YYSYMBOL_make_dim = 393,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 394,            /* make_dim_decl  */
  YYSYMBOL_make_table = 395,               /* make_table  */
  YYSYMBOL_make_table_decl = 396,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 397, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 398       /* array_comprehension  */
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
#define YYLAST   10654

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  199
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  200
/* YYNRULES -- Number of rules.  */
#define YYNRULES  651
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1151

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
    1145,  1151,  1155,  1159,  1162,  1165,  1170,  1173,  1181,  1184,
    1189,  1192,  1200,  1206,  1207,  1211,  1217,  1217,  1217,  1220,
    1220,  1220,  1225,  1225,  1225,  1233,  1233,  1233,  1239,  1249,
    1260,  1275,  1278,  1284,  1285,  1292,  1303,  1304,  1305,  1309,
    1310,  1311,  1312,  1316,  1321,  1329,  1330,  1334,  1339,  1346,
    1347,  1348,  1349,  1350,  1351,  1355,  1356,  1357,  1358,  1359,
    1360,  1361,  1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,
    1370,  1371,  1372,  1373,  1377,  1378,  1379,  1380,  1384,  1395,
    1400,  1410,  1414,  1421,  1424,  1424,  1424,  1429,  1429,  1429,
    1442,  1446,  1450,  1450,  1450,  1457,  1458,  1459,  1460,  1461,
    1462,  1463,  1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,
    1472,  1473,  1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,
    1482,  1483,  1484,  1485,  1486,  1487,  1488,  1489,  1490,  1491,
    1492,  1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,
    1507,  1508,  1509,  1510,  1514,  1518,  1521,  1524,  1525,  1526,
    1527,  1530,  1533,  1534,  1537,  1537,  1537,  1540,  1545,  1549,
    1553,  1553,  1553,  1558,  1561,  1565,  1565,  1565,  1570,  1573,
    1574,  1575,  1576,  1577,  1578,  1579,  1580,  1581,  1582,  1583,
    1591,  1592,  1593,  1594,  1595,  1596,  1600,  1604,  1608,  1612,
    1616,  1620,  1624,  1628,  1632,  1639,  1640,  1644,  1645,  1646,
    1650,  1651,  1655,  1656,  1657,  1661,  1671,  1674,  1674,  1693,
    1692,  1707,  1706,  1719,  1728,  1733,  1738,  1748,  1749,  1753,
    1756,  1765,  1766,  1770,  1779,  1780,  1785,  1786,  1790,  1796,
    1802,  1805,  1809,  1815,  1824,  1825,  1826,  1830,  1831,  1835,
    1842,  1847,  1856,  1862,  1873,  1876,  1881,  1886,  1894,  1905,
    1908,  1908,  1928,  1929,  1933,  1934,  1935,  1939,  1942,  1942,
    1961,  1964,  1973,  1986,  1986,  2007,  2008,  2012,  2013,  2017,
    2018,  2019,  2023,  2033,  2040,  2050,  2051,  2055,  2056,  2060,
    2066,  2067,  2071,  2072,  2073,  2077,  2082,  2077,  2092,  2099,
    2104,  2113,  2119,  2130,  2131,  2132,  2133,  2134,  2135,  2136,
    2137,  2138,  2139,  2140,  2141,  2142,  2143,  2144,  2145,  2146,
    2147,  2148,  2149,  2150,  2151,  2152,  2153,  2154,  2158,  2159,
    2160,  2161,  2162,  2163,  2167,  2178,  2182,  2189,  2201,  2208,
    2217,  2217,  2217,  2230,  2234,  2241,  2242,  2243,  2244,  2245,
    2259,  2265,  2269,  2273,  2278,  2283,  2288,  2293,  2297,  2301,
    2306,  2310,  2315,  2315,  2315,  2321,  2328,  2328,  2328,  2333,
    2333,  2333,  2339,  2339,  2339,  2344,  2348,  2348,  2348,  2353,
    2353,  2353,  2362,  2366,  2366,  2366,  2371,  2371,  2371,  2380,
    2384,  2384,  2384,  2389,  2389,  2389,  2398,  2398,  2398,  2404,
    2404,  2404,  2413,  2413,  2413,  2437,  2437,  2457,  2458,  2459,
    2460,  2464,  2471,  2478,  2484,  2493,  2498,  2505,  2506,  2510,
    2516,  2523,  2531,  2538,  2546,  2558,  2561,  2575,  2581,  2587,
    2588,  2592,  2597,  2604,  2609,  2619,  2624,  2631,  2643,  2644,
    2648,  2651
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
  "expression_return", "expression_yield", "expression_try_catch",
  "kwd_let", "expression_let", "expr_cast", "$@4", "$@5", "$@6", "$@7",
  "$@8", "$@9", "expr_type_decl", "$@10", "$@11", "expr_type_info",
  "expr_list", "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@12", "$@13", "$@14", "$@15", "expr_field", "$@16",
  "$@17", "expr", "$@18", "$@19", "$@20", "$@21", "$@22", "$@23",
  "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@24", "$@25", "$@26", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@27", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@28", "enum_list",
  "single_alias", "$@29", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_name", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@30", "$@31", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@32", "$@33", "table_type_pair",
  "type_declaration", "$@34", "$@35", "$@36", "$@37", "$@38", "$@39",
  "$@40", "$@41", "$@42", "$@43", "$@44", "$@45", "$@46", "$@47", "$@48",
  "$@49", "$@50", "$@51", "$@52", "$@53", "$@54", "$@55", "$@56", "$@57",
  "variant_alias_declaration", "$@58", "$@59",
  "bitfield_alias_declaration", "$@60", "make_decl", "make_struct_fields",
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

#define YYPACT_NINF (-808)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-604)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -808,    43,  -808,  -808,    22,   -66,   -89,   -98,  -808,     5,
    -808,  -808,     4,  -808,  -808,  -808,  -808,  -808,    79,  -808,
     103,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
      59,  -808,    69,   106,   124,  -808,  -808,   -89,    18,  -808,
    -808,   191,  -808,  -808,  -808,   103,   214,   218,  -808,  -808,
     276,   293,   267,  -808,    95,  -808,  -808,  -808,   334,   381,
     396,  -808,   401,     9,    22,   308,   -66,   287,   321,  -808,
     322,   333,  -808,   318,   309,  -107,   421,   310,  -808,  -808,
     348,    22,     4,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
     352,  -808,  -808,  -808,  -808,  -808,   314,  -808,  -808,  -808,
    -808,  -808,   -72,   144,  -808,  -808,  -808,  -808,   456,  -808,
    -808, 10319,  -808,  -808,   317,  -808,  -808,  -808,   357,   320,
    -808,   -21,  -808, 10497,  -808,  -104,   420,  -808,   370,  -808,
    -808,   339,  -808,  -808,  -808,  -808,  -808,  -808,  -808,    49,
    -808,   347,   349,   356,   358,  -808,  -808,  -808,   338,  -808,
    -808,  -808,  -808,  -808,   359,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,   360,  -808,  -808,  -808,   362,   364,
    -808,  -808,  -808,  -808,   365,   368,   350,  -808,  -808,  -808,
    -808,  -808,  2912,  -808,  -808,   329,   385,  -808,  8934,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,   509,   510,  -808,   355,   353,   382,  -808,  -808,
     397,  -808,   361,    22,   197,  -808,  -808,  -808,   144,  -808,
    -808,  -808,  -808,  -808,   398,  -808,   194,   213,   250,  -808,
    -808,  5287,  -808,  -808,  -808,  -808,   521,  -808,  -808,    46,
    3351,  -808,   -80,   388,  -808,   366,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,   367,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,   538,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,   372,  -808,  -808,   -91,
     395,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,   369,
     -44,   399,   373,  -808,   370,   114,   375,   519,   284,  -808,
    -808, 10319, 10319, 10319, 10319,   376,   357, 10319,   355, 10319,
     355, 10319,   355, 10413,   385,  -808,  -808, 10225,   379,   400,
    -808,   380,   402,   403,   391,   404,   410,  -808,   411,  5287,
    5287,   412,   416,   417,   418,   425,   426,  9916, 10010,  5287,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  5287,  5287,  5287,
     171,  5287,  5287,  5287,  -808,   394,  -808,  -808,  -808,   -96,
    -808,  -808,  -808,  -808,   384,  -808,  -808,  -808,  -808,  -808,
    -808,  1275,  -808,   428,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,   374,  -808,  -808,  8262,  -808,  -808, 10319,     8,
    -808,  -808,  -808,   431,  -808,    -6,  -808,   268, 10319,  -808,
    1568,  -808,   -23,  -808,   171,  -808,  -808,   197,   434,  5287,
     436,   437, 10319,  -808,   -55,    98,   429,   631,   907,   924,
    -808,    80,  1288,   395,  1316,   395,  1381,   395,    36,  -808,
     111,  2912,   164,  1867,  -808,   171,  -808,  5287,  -808,  -808,
    5287,  -808,  5287, 10319,   257,   257,  5287,  5287,  5287,  5287,
    5287,  5287,   217,  1937,   217,  2106,  9030,  -808,   212,   327,
     257,   257,   -39,  -808,   257,   257,  5896,   104,  -808,  2744,
     464,  9102, 10041,  5287,  5287,  -808,  -808,  5287,  5287,  5287,
    5287,   468,  5287,   233,  5287,  5287,  5287,  5287,  5287,  5287,
    5287,  5287,  5287,  3497,  5287,  5287,  5287,  5287,  5287,  5287,
    5287,  5287,  5287,  5287,   195,  5287,  -808,  3643,  -808,  -808,
    2912,  -808,  -808,  5287,   217,   438,   586,  -808,   -63,  -808,
     107,  2912,  -808,  5287,  -808,  -808,   217,  2444,  -808,   353,
    5287,  5287,   473,  -808,   433,   480,  3812,    72,  2598,   200,
     200,  -808,   596,   439,   440,  5287,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,   441,   442,   444,  -808,  -808,  -808,
     197,  -808,   -56,  -808,  8831,    21,  5287,  -808,  -808,    14,
    -808,  -808,  5987,  -808,   592,   620,  -808,  -808,  -808,  2913,
    -808,  -808, 10319,  -808,  -808,  -808,   493,  -808,   483,  -808,
     484,  -808,   485, 10319,  -808, 10413,  -808,   385,  3958,  4127,
   10319,  6078, 10319, 10319,  6169, 10319,  6260,  1501,  6351,  6442,
    6533,  6624,  6715,  6806,    11,   200,   -57,  2275,  4296,  9030,
     488,   -22,   472,   496,  -808,  -808,   253,    12,  4465,   -22,
     306,  5287,  5287,   467,  -808,  5287,    10,   506,  -808,   476,
     477,   256,  -808,  -808,   512,  -808,    50,  9268,   -59,   355,
     499,   481,  -808,  -808,   501,   486,  -808,  -808,   319,   319,
    9784,  9784,   292,   292,   489,   172,   490,  -808,  8353,   -64,
     -64,   319,   319,   727,  9462,  9539,  9352, 10131,  9107,  9571,
    9682,  9759,  9784,  9784,   634,   634,   172,   172,   172,   240,
    5287,   492,  -808,   274,  5287,   673,  8444,  -808,    74,  6897,
    -808,  5287,   518,  -808,   525,  -808, 10319,  -808,  2913,  -808,
      13,    16,  2913,  -808,  -808,   554,  1065,   671,  9268,    13,
     520,  -808,   517,   536,  9268,  -808,  2913,  -808,  5386,   494,
    -808,  -808,   495,  -808,  -808,    13,  -808,  -808,  -808,  -808,
     200,  -808,  5287,  5287,  5287,  5287,  5287,  5287,  5287,  5287,
    5287,  5287,  3059,  5287,  5287,  5287,  5287,  5287,  5287,  3205,
    -808,   471,     4,  -808,   685,   370,  -808,   537,   521,  -808,
    2913,  -808,  5488,  -808,  -808,  2912,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  2912,  -808,  -808,  -808,  -808,
      94,   540,  6988,  1698,  -808,  2038,  2207,  -808,  2274,  -808,
     464,  -808,  -808,  -808,  -808,  -808,   502,  5287,  -808,  5287,
    5287,   -15,   541,   512,   505,   515,  5287,   516,  5287,  5287,
     512,   514,   526,  9268,  -808,  -808,  9191,  -808,   558,  5287,
    5287, 10319,   355,   -11,   146,  5287,  -808,   542,   561,   563,
     566,  -808,   151,   395,  -808,  5287,  -808,  5287,  4634,  5287,
    -808,   553,   535,  -808,  -808,  5287,   539,  -808,  8535,  5287,
     543,  -808,  8626,  -808,  -808,  -808,  -808,  7079,  -808,   690,
    1304,  -808,  8728,  -808,  5287,  -808,  5590,  5287,  5287,  -808,
     353,  -808,  -808,  -808,  -808,  -808,  5692,  -808,  -808,  -808,
     392,  -808,  9268,  9268,  9268,  9268,  9268,  9268,  9268,  9268,
    9268,  9268,  5287,  5287,  9268,  9268,  9268,  9268,  9268,  9268,
    9268,  5287,  5287,  9268,  -808,   159,   422,     7,   544,  -808,
    -808,  5794,  -808,  -808,  -808,  -808,  -808,   174,  -808,  -808,
    -808,  -808,  -808,   545,  4803,  -102,  9268,  9268,   -22,   550,
      28,   488,   555,  -808,  9268,  -808,  -808,    45,   -22,   567,
    -808,  -808,  -808,  7170,  7261,  2376,   395,   552,  9268,  -808,
    -808,  -808,  -808,   -59,   568,   -86, 10319,  7352, 10319,  7443,
    -808,   102,  7534,  -808,  5287,  9429,  5287,  -808,  7625,  5287,
    -808,  -808,  -808,   578,   521,  5287,    48,  -808,  -808,  5287,
    -808,  1765,   353,  -808,  -808,  5287,  -808,  9648,  9268,  9648,
    9268,   565,  -808,  -808,     3,  -808,  -808,   422,  -808,  -808,
    5287,   599,  -808,  5287,  5287,  5287,  4972,  -808,   108,  5287,
     572,   583,  5287,  5287,  -808,  5287,   577,  -808,  -808,  -808,
    -808,   602,  -808,  -808,  -808,  5141,  -808,  -808,  2677,  -808,
    2743,  -808,  -808,  -808, 10319,  7716,  7807,  -808,  7898,  -808,
    9268,   353,  9268,  -808,  -808,    13,  -808,  -808,   745,   217,
    7989,   605,  9784,  9784,  9784,  -808,  8080,  -808,   594,   589,
    -808,  9268,  9268,   594,   595,   166,  -808,  5287,  9352,  -808,
    -808,  2845,  -808,  -808,  -808,  -808,   392, 10497,  -808,  -808,
    -808,  -808,  5287,  -808,   733,   598,  -808,   593,  -808,  -808,
     166,  9429,  -808,  -808,  -808,  -808,   597, 10497,  8171,  5287,
     603,   606,  -808,  -808,  -808,   353,  -808,  9268,  -808,  -808,
    -808
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,    94,     1,   233,     0,     0,     0,     0,   234,     0,
     615,   612,     0,    14,     3,    10,     9,     8,     0,     7,
     472,     6,    11,     5,     4,    12,    13,    75,    76,    74,
      83,    85,    34,    47,    44,    45,    36,     0,    42,    35,
     483,     0,   488,    19,    18,   472,     0,     0,    88,    89,
       0,   213,    90,    92,     0,    87,   501,   500,   179,   489,
     502,   473,   474,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,   613,   215,
       0,     0,     0,    95,   181,   180,   183,   178,   491,   490,
       0,   504,   503,   505,   476,   475,   478,    81,    82,    79,
      80,    78,     0,     0,    77,    86,    48,    46,    42,    39,
      38,     0,   485,   487,     0,    16,    17,    20,     0,     0,
     214,     0,    93,     0,   492,     0,   497,   469,   415,    21,
      22,     0,    70,    71,    68,    69,    67,    66,    72,     0,
      41,     0,     0,     0,     0,   513,   533,   514,   545,   515,
     519,   520,   521,   522,   537,   526,   527,   528,   529,   530,
     531,   532,   534,   535,   585,   518,   525,   536,   592,   599,
     516,   523,   517,   524,     0,     0,     0,   544,   555,   558,
     556,   557,   484,   486,   548,     0,     0,    91,     0,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,     0,   101,    96,     0,     0,   480,   498,
       0,   506,   470,     0,     0,    23,    24,    25,     0,    84,
     576,   579,   582,   572,     0,   550,   586,   593,   600,   606,
     609,     0,   563,   568,   562,   575,     0,   571,   565,     0,
       0,   567,     0,     0,   444,     0,   152,   153,   150,   104,
     105,   107,   106,   108,   109,   110,   111,   137,   138,   135,
     136,   128,   139,   140,   129,   126,   127,   151,   148,     0,
     149,   141,   142,   143,   144,   115,   116,   117,   112,   113,
     114,   125,     0,   131,   132,   130,   123,   124,   119,   118,
     120,   121,   122,   103,   102,   147,     0,   133,   134,   415,
      99,   207,   184,   538,   541,   539,   542,   540,   543,     0,
       0,   495,     0,   477,   415,     0,     0,   459,   457,   479,
      73,     0,     0,     0,     0,     0,     0,     0,    96,     0,
      96,     0,    96,     0,     0,   321,   322,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   537,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     269,   271,   270,   272,   273,   274,    26,     0,     0,     0,
       0,     0,     0,     0,   256,   257,   319,   318,   267,   316,
     392,   391,   390,   389,    94,   395,   317,   394,   393,   362,
     323,     0,   324,     0,   320,   617,   618,   619,   620,   570,
     564,   566,     0,   569,   560,     0,   549,   616,     0,     0,
     146,   154,   145,     0,    97,     0,   437,     0,     0,   182,
       0,   480,     0,   493,     0,   499,   426,     0,     0,     0,
       0,     0,     0,   458,     0,     0,     0,   553,     0,     0,
     546,     0,     0,    99,     0,    99,     0,    99,   213,   441,
       0,   439,     0,   217,   220,     0,   245,     0,   236,   239,
       0,   242,     0,     0,   351,   352,     0,     0,     0,     0,
       0,     0,     0,   627,     0,     0,   638,   645,     0,     0,
     328,   327,   367,    32,   326,   325,     0,   258,   398,     0,
     265,     0,     0,     0,     0,   353,   354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   312,     0,   547,     0,   561,   559,
     443,   614,   445,     0,     0,     0,   508,   434,   448,    98,
     415,   100,   209,     0,    57,    58,     0,     0,   221,     0,
       0,     0,     0,   222,     0,     0,     0,     0,     0,     0,
       0,   187,   185,     0,     0,     0,   202,   197,   194,   193,
     195,   196,   208,   188,     0,     0,     0,   200,   201,   203,
       0,   192,     0,   189,   275,     0,     0,   481,   496,   427,
     471,   416,     0,   461,   462,     0,   455,   456,   454,     0,
     577,   580,     0,   583,   573,   551,     0,   587,     0,   594,
       0,   601,     0,     0,   607,     0,   610,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   213,     0,     0,   635,
     625,   627,     0,   639,   640,   641,     0,     0,     0,   627,
       0,     0,     0,     0,    29,     0,    27,     0,   355,     0,
       0,   304,   301,   303,     0,   363,     0,   251,     0,    96,
       0,     0,   378,   377,     0,     0,   379,   383,   329,   330,
     342,   343,   340,   341,     0,   372,     0,   360,     0,   396,
     397,   331,   332,   347,   348,   349,   350,     0,     0,   345,
     346,   344,   338,   339,   334,   333,   335,   336,   337,     0,
       0,     0,   310,     0,     0,     0,     0,   365,     0,     0,
     435,     0,     0,   447,     0,   446,     0,   449,     0,   438,
       0,     0,     0,   223,   226,     0,   275,     0,   216,     0,
       0,   206,     0,     0,    51,    61,     0,   230,   275,   257,
     211,   212,     0,   204,   205,     0,   191,   198,   199,   235,
       0,   190,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     494,     0,     0,   507,     0,   415,   460,     0,   454,   464,
       0,   468,   275,   578,   581,   554,   584,   574,   552,   588,
     590,   595,   597,   602,   604,   440,   608,   442,   611,   218,
       0,     0,     0,     0,   368,     0,     0,   369,     0,   399,
     265,   400,   401,   402,   403,   404,     0,     0,   628,     0,
       0,   627,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   637,   646,   647,     0,    30,    33,     0,
       0,     0,    96,     0,     0,     0,   364,     0,     0,     0,
       0,   263,     0,    99,   374,     0,   380,     0,     0,     0,
     358,     0,     0,   384,   388,     0,     0,   361,     0,     0,
       0,   311,     0,   313,   356,   366,   436,     0,   510,   511,
     450,   453,   452,    62,     0,   227,   275,     0,     0,   224,
       0,    63,    64,    49,    50,   231,   275,   228,   258,   207,
      54,   210,   285,   286,   288,   287,   289,   279,   280,   281,
     290,   291,     0,     0,   277,   278,   292,   293,   282,   283,
     284,     0,     0,   276,   482,     0,   422,   417,     0,   463,
     466,   275,   467,   591,   598,   605,   219,     0,   248,   246,
     237,   240,   243,     0,     0,     0,   622,   621,   627,     0,
       0,   626,     0,   630,   636,   643,   642,     0,   627,     0,
     644,    28,    31,     0,     0,     0,    99,     0,   252,   261,
     262,   260,   259,     0,     0,     0,     0,     0,     0,     0,
     299,     0,     0,   385,     0,   373,     0,   359,     0,     0,
     357,   314,   509,     0,   446,     0,     0,   225,   232,     0,
     229,     0,     0,    52,    53,     0,    59,   296,   297,   294,
     295,    95,   423,   424,   417,   418,   419,   422,   428,   465,
       0,     0,   247,     0,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,   629,     0,     0,   633,   302,   414,
     305,     0,   298,   264,   266,     0,   253,   268,     0,   413,
       0,   411,   300,   408,     0,     0,     0,   407,     0,   512,
     451,     0,    65,   186,    55,     0,   433,   429,   420,     0,
       0,     0,   238,   241,   244,   370,     0,   406,   648,     0,
     631,   624,   623,   648,     0,     0,   308,     0,   254,   375,
     381,     0,   412,   410,   409,    60,    54,     0,   421,   431,
     425,   249,     0,   371,     0,     0,   632,     0,   634,   306,
       0,   255,   376,   382,   386,    56,     0,     0,     0,     0,
       0,     0,   309,   387,   430,     0,   250,   649,   650,   651,
     432
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -808,  -808,  -808,  -808,  -808,   311,   732,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,   762,  -808,   693,  -808,  -808,   737,
    -808,  -808,  -808,  -312,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,   579,  -808,  -808,   742,   -49,  -808,   731,    24,
    -379,  -329,  -447,  -808,  -808,  -808,  -807,  -808,  -808,  -214,
    -808,  -105,  -530,   -12,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,   816,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -532,  -808,  -808,  -174,  -808,    -9,
    -551,  -808,  -426,  -808,  -808,  -808,  -779,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,   491,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -125,  -210,  -808,  -212,  -808,  -808,  -808,
    -808,  -808,   277,  -808,   203,  -808,  -399,   487,  -536,  -537,
     225,  -808,  -808,  -415,  -808,  -808,   787,  -808,  -808,  -808,
     407,   155,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -438,  -110,  -808,  -808,
    -808,   503,  -808,  -808,  -808,  -808,   -85,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,   497,  -646,  -471,  -634,
    -808,  -808,  -353,    15,   371,  -808,  -808,  -808,  -250,  -808
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   117,    45,    14,   131,   137,   489,   386,   858,
     387,   667,    15,    16,    38,    39,    72,    17,    34,    35,
     573,   574,  1025,  1026,   575,   576,   577,   578,   579,   580,
     581,  1019,   138,   139,    30,    31,    32,    52,    53,    54,
      18,   310,   429,   215,    19,    86,   216,    87,   123,   388,
     582,   430,   583,   389,   584,   390,   585,   586,   587,   588,
     589,   590,   591,   391,   632,  1043,   633,  1044,   635,  1045,
     392,   630,  1042,   393,   676,  1067,   394,   871,   872,   679,
     395,   396,   745,   593,   397,   398,   673,   399,   861,  1105,
     862,  1130,   400,   725,  1011,   677,   996,  1132,   998,  1133,
    1074,  1143,   402,   425,  1037,  1119,  1034,   948,   599,   795,
    1117,  1137,   426,   427,   459,   460,   254,   255,   840,   547,
     609,   444,   328,   329,   222,   324,    62,    96,    21,   128,
     320,    42,    73,    75,    22,    90,   125,    23,   435,   220,
     221,    60,    93,    24,   126,   322,   548,   403,   319,   179,
     180,   185,   181,   336,   808,   446,   461,   334,   807,   331,
     803,   332,   804,   333,   806,   337,   809,   338,   953,   339,
     811,   340,   954,   341,   813,   342,   955,   343,   816,   344,
     818,    25,    47,   119,    26,    46,   404,   650,   651,   652,
     405,   653,   654,   655,   656,   406,   488,   407,  1125,   408
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   178,   312,   224,   592,   728,   618,   730,   620,   453,
     622,   455,   738,   457,   659,   500,   487,   844,   760,   761,
     542,   -94,   600,    97,    98,   851,   182,   744,   864,   645,
     837,   849,   121,   501,   502,   904,   645,    48,   757,    68,
      27,    28,   544,     2,   644,  1065,   657,  1035,  1087,     3,
      40,  1035,   511,   423,   733,   513,   514,   223,   867,    40,
     733,   498,   606,   865,   217,    49,   770,   868,    36,   839,
      55,   607,     4,    69,     5,    80,     6,   416,   410,   801,
     513,   514,     7,    56,    57,    33,    58,   129,   130,    37,
     113,     8,   218,  1049,   838,   499,   820,     9,    41,   177,
    1036,   424,   734,   735,  1036,   736,   733,   869,   737,   735,
     311,   608,   870,   432,    59,   839,    10,   417,   741,   503,
     504,   505,   506,   507,   534,   535,   508,   509,   510,   511,
     242,   512,   513,   514,   515,   545,    50,    11,   516,   771,
     517,   518,   863,   596,    64,   733,   519,   520,   521,   534,
     535,   546,   522,   433,  1052,   735,   243,   244,   132,   133,
      99,    51,    43,    61,   100,   253,   101,   102,    80,   225,
     226,   187,   597,   843,   325,   769,   734,   734,   432,    29,
     523,   734,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   501,   502,   735,    70,    74,   971,    44,   437,
     103,   534,   535,   792,   623,   541,    71,   969,   901,   311,
     865,   793,   905,   865,   228,   865,   245,   411,   790,   921,
     794,   178,   178,   178,   178,    63,   915,   178,   542,   178,
     114,   178,    12,   178,    64,   412,    50,   178,    13,   865,
    1055,   229,   866,  1081,   413,   669,   445,   447,   448,   449,
     670,   423,   452,   615,   454,   223,   456,   178,   178,   865,
      82,    51,   463,   472,   246,   247,   895,   865,   311,   248,
     950,   610,   249,   865,    65,   616,   671,   501,   502,    64,
     505,   506,   483,   485,   624,    83,   956,   250,   511,    66,
     512,   513,   514,   515,  1072,   134,   251,   516,    50,   135,
    1097,   136,   102,    50,   438,   965,   625,   669,   178,   854,
    1136,   842,   501,   502,   719,   720,   993,   977,   178,   177,
     177,   177,   177,    51,    82,   177,  1129,   177,    51,   177,
    1145,   177,   178,   540,  1050,   177,   987,   626,   326,   501,
     502,   994,   721,   551,  1056,   747,  1001,  1040,    40,  1031,
     873,  1142,   722,   755,   327,   177,   177,   605,   545,   627,
     534,   535,  -589,   178,  1015,   505,   506,  -589,   493,  1041,
     968,    77,  1016,   511,   546,    78,   513,   514,   515,   978,
     696,  -596,   516,   723,   724,  -589,  -596,   886,   637,    84,
     697,   683,   687,   384,   759,    85,   311,   887,   503,   504,
     505,   506,   507,  1022,  -596,   508,   177,   662,   511,   663,
     512,   513,   514,   515,  1023,  1024,   177,   516,  -603,   517,
     518,   890,   598,  -603,  -307,    80,   995,   505,   506,  -307,
     177,   891,  1048,    79,  1053,   511,    88,   512,   513,   514,
     515,  -603,    89,   847,   516,   534,   535,  -307,   848,   441,
     313,    91,   442,   629,   314,   443,    94,    92,    81,   106,
     549,   177,    95,   550,   527,   528,   529,   530,   531,   532,
     533,   315,   316,   317,   318,    71,   115,  1032,   108,   109,
     534,   535,   116,  1033,   111,   672,   129,   130,   664,   665,
     110,   501,   502,   529,   530,   531,   532,   533,   225,   226,
     227,   848,   178,   852,   112,   120,   118,   534,   535,   124,
     127,    69,   183,   178,   184,   178,   186,   219,   223,   230,
     178,   231,   178,   178,   252,   178,   903,   805,   232,   234,
     233,   235,   236,   986,   237,   911,   238,   239,   815,  1061,
     240,   241,   253,   307,   308,   823,   309,   825,   826,   311,
     828,   920,  1120,   409,   321,   335,   418,   420,   323,   421,
     440,   419,   422,   428,   538,   431,   439,   434,   450,   436,
     465,   467,   466,    12,   468,   469,   471,   503,   504,   505,
     506,   507,   470,   473,   508,   509,   510,   511,   497,   512,
     513,   514,   515,   603,   604,   592,   516,   884,   517,   518,
     177,   472,   611,   476,   519,   520,   521,   477,   478,   479,
     522,   177,   678,   177,   501,   502,   480,   481,   177,   537,
     177,   177,   543,   177,   601,   694,   178,   732,   751,   731,
     750,   752,   762,   797,   763,   764,   766,   767,   523,   768,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
     416,   900,   242,   842,   501,   502,   810,   812,   814,   534,
     535,   846,   845,   242,   855,   857,   944,   859,   860,   863,
     947,   874,   875,   876,   893,   898,   770,   877,   243,   244,
     878,   879,   899,   889,   910,   913,   912,   914,   918,   243,
     244,   919,   946,   964,   949,   972,  1018,   957,   970,   989,
     503,   504,   505,   506,   507,   973,   975,   508,   509,   510,
     511,   979,   512,   513,   514,   515,   980,   982,   990,   516,
     991,   517,   518,   992,   177,  1003,  1004,   519,   520,   521,
    1006,  1013,   401,   522,  1009,  1079,  1046,   606,   245,  1038,
    1051,   415,   505,   506,  1062,  1054,   607,   501,   502,   245,
     511,   178,   512,   513,   514,   515,  1091,  1057,  1064,   516,
    1086,   523,  1099,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,  1100,  1104,  1106,   985,  1118,  1122,  1126,
      55,  1066,   534,   535,  1139,  1128,   798,   247,  1140,  1124,
    1141,   248,  1144,  1148,   249,   104,  1149,   246,   247,    67,
     666,   140,   248,   107,  1135,   249,   105,   330,  1084,   250,
     531,   532,   533,   122,  1021,   799,   945,    20,   251,  1063,
     250,   963,   534,   535,  1088,  1089,   612,   739,   817,   251,
     800,   462,    76,   503,   504,   505,   506,   507,   595,   451,
     508,   509,   510,   511,   464,   512,   513,   514,   515,   177,
     474,   475,   516,  1127,   517,   518,   660,     0,     0,     0,
     486,     0,     0,   976,     0,     0,     0,  1115,   490,   491,
     492,  1116,   494,   495,   496,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   178,     0,   178,     0,
       0,     0,     0,     0,     0,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,  1068,     0,  1070,     0,   534,   535,     0,     0,     0,
       0,   594,     0,     0,     0,     0,     0,     0,     0,     0,
     602,  1150,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,   631,     0,
       0,   634,     0,   636,   178,   243,   244,   638,   639,   640,
     641,   642,   643,     0,   649,     0,   649,     0,     0,     0,
       0,     0,   243,   244,   177,     0,   177,     0,     0,  1111,
       0,     0,     0,     0,   688,   689,     0,     0,   690,   691,
     692,   693,     0,   695,     0,   698,   699,   700,   701,   702,
     703,   704,   705,   706,   708,   709,   710,   711,   712,   713,
     714,   715,   716,   717,   718,   245,   726,     0,     0,     0,
       0,     0,     0,     0,   729,     0,     0,     0,     0,     0,
       0,     0,   245,     0,   740,     0,     0,     0,   746,     0,
       0,   748,   749,     0,     0,     0,     0,   754,     0,   758,
       0,     0,   177,     0,     0,     0,   765,     0,     0,     0,
       0,     0,     0,   246,   247,     0,     0,     0,   248,     0,
     613,   249,     0,     0,     0,   501,   502,   791,     0,     0,
     246,   247,     0,   672,     0,   248,   250,   614,   249,     0,
     802,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,   250,     0,     0,     0,     0,   672,     0,
     822,     0,   251,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   490,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
       0,     0,   853,   486,     0,     0,   856,     0,     0,     0,
       0,     0,     0,   772,   773,   774,   775,   776,   777,   778,
     779,   503,   504,   505,   506,   507,   780,   781,   508,   509,
     510,   511,   907,   512,   513,   514,   515,     0,     0,     0,
     516,   783,   517,   518,   784,   785,     0,     0,   519,   520,
     521,   786,   787,   788,   522,     0,     0,     0,     0,     0,
       0,   888,     0,     0,     0,   892,     0,     0,     0,     0,
       0,     0,   897,     0,     0,     0,     0,     0,     0,   902,
       0,   908,   523,   906,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,   916,     0,     0,
       0,     0,     0,   534,   535,     0,     0,     0,     0,     0,
     909,     0,     0,   922,   923,   924,   925,   926,   927,   928,
     929,   930,   931,   934,   935,   936,   937,   938,   939,   940,
     943,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   951,     0,     0,     0,   501,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     966,   967,     0,     0,     0,     0,   242,   974,     0,   649,
       0,     0,     0,     0,     0,     0,   243,   244,   242,     0,
     983,   984,     0,     0,     0,     0,   988,     0,     0,     0,
       0,     0,   243,   244,     0,     0,   997,     0,   999,     0,
    1002,     0,     0,     0,   243,   244,  1005,     0,     0,     0,
    1008,   503,   504,   505,   506,   507,     0,     0,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,   934,   943,
     516,     0,   517,   518,     0,     0,   245,     0,   519,   520,
     521,     0,     0,   242,   522,     0,     0,     0,     0,     0,
       0,   733,   245,  1027,  1028,     0,     0,     0,     0,     0,
       0,     0,  1029,  1030,   245,     0,     0,     0,     0,   243,
     244,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,   246,   247,     0,     0,     0,   248,
       0,   617,   249,   534,   535,     0,     0,   536,     0,     0,
    1014,   247,     0,     0,     0,   248,     0,   250,   249,     0,
       0,     0,   246,   247,     0,     0,   251,   248,     0,   619,
     249,     0,     0,   250,     0,  1075,     0,  1076,     0,   245,
    1078,     0,   251,     0,     0,   250,  1080,     0,     0,     0,
    1082,     0,   594,     0,   251,     0,  1085,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1090,     0,   242,  1092,  1093,  1094,  1096,     0,     0,
    1098,     0,     0,  1101,  1102,     0,  1103,   246,   247,     0,
       0,     0,   248,     0,   621,   249,  1108,     0,     0,   243,
     244,     0,     0,     0,     0,     0,     0,     0,     0,   552,
     250,     0,     0,     0,     3,     0,   553,   554,   555,   251,
     556,     0,   345,   346,   347,   348,   349,     0,     0,     0,
       0,     0,     0,   557,   350,   558,   559,     0,  1131,     0,
       0,     0,     0,     0,     0,   560,   351,     0,   561,     0,
     562,   352,     0,  1138,   353,     0,     8,   354,   563,   245,
     564,   355,     0,     0,   565,   566,     0,     0,     0,     0,
    1147,   567,   145,   146,   147,     0,   149,   150,   151,   152,
     153,   357,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   165,   166,   167,     0,     0,   170,   171,   172,
     173,     0,     0,   358,   568,     0,     0,   246,   247,     0,
       0,     0,   248,     0,   830,   249,   359,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     250,   569,   570,     0,     0,     0,     0,     0,     0,   251,
      50,     0,     0,     0,     0,     0,     0,     0,   361,   362,
     363,   364,   365,     0,   366,     0,   367,   368,   369,   370,
     371,   372,   373,   374,   375,    51,   376,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   379,     0,   380,     0,     0,   381,
     382,     0,     0,     0,     0,     0,   243,   244,     0,   383,
       0,   384,   385,   571,   311,   572,   552,     0,     0,     0,
       0,     3,     0,   553,   554,   555,     0,   556,     0,   345,
     346,   347,   348,   349,     0,     0,     0,     0,     0,     0,
     557,   350,   558,   559,     0,     0,     0,     0,     0,     0,
       0,     0,   560,   351,     0,   561,     0,   562,   352,     0,
       0,   353,     0,     8,   354,   563,   245,   564,   355,     0,
       0,   565,   566,     0,     0,     0,     0,     0,   567,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   357,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     358,   568,     0,     0,   246,   247,     0,     0,     0,   248,
       0,   959,   249,   359,   360,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   250,   569,   570,
       0,     0,     0,     0,     0,     0,   251,    50,     0,   242,
       0,     0,     0,     0,     0,   361,   362,   363,   364,   365,
       0,   366,     0,   367,   368,   369,   370,   371,   372,   373,
     374,   375,    51,   376,     0,   243,   244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,     0,   380,     0,     0,   381,   382,     0,     0,
       0,   345,   346,   347,   348,   349,   383,     0,   384,   385,
     571,   311,  1083,   350,     0,     0,     0,     0,     0,   242,
       0,     0,     0,     0,     0,   351,     0,     0,     0,     0,
     352,     0,     0,   353,     0,   245,   354,     0,   645,     0,
     355,     0,     0,     0,     0,   243,   244,     0,     0,     0,
     356,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     357,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   358,   246,   247,     0,     0,     0,   248,     0,
       0,   249,     0,     0,     0,   359,   360,     0,     0,     0,
       0,     0,     0,     0,     0,   245,   250,     0,   628,     0,
       0,     0,     0,     0,     0,   251,     0,     0,     0,    50,
     242,     0,     0,     0,     0,     0,     0,   361,   362,   363,
     364,   365,     0,   366,     0,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   646,   376,   243,   244,     0,     0,
       0,     0,     0,   246,   247,     0,     0,     0,   248,     0,
       0,   647,   378,   379,     0,   380,     0,     0,   381,   382,
     345,   346,   347,   348,   349,     0,   250,     0,   648,     0,
     384,   385,   350,   311,     0,   251,     0,     0,   242,     0,
       0,     0,     0,     0,   351,     0,     0,     0,     0,   352,
       0,     0,   353,     0,     0,   354,   245,     0,     0,   355,
       0,     0,     0,     0,   243,   244,     0,     0,     0,   356,
     145,   146,   147,     0,   149,   150,   151,   152,   153,   357,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
       0,   358,     0,     0,   246,   247,     0,     0,     0,   248,
       0,   960,   249,     0,   359,   360,     0,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,   250,     0,     0,
       0,     0,     0,     0,     0,     0,   251,     0,    50,   242,
       0,     0,     0,     0,     0,     0,   361,   362,   363,   364,
     365,     0,   366,     0,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   646,   376,   243,   244,     0,     0,     0,
       0,     0,   246,   247,     0,     0,     0,   248,     0,     0,
     647,   378,   379,     0,   380,     0,     0,   381,   382,   345,
     346,   347,   348,   349,     0,   250,     0,   658,     0,   384,
     385,   350,   311,     0,   251,     0,   242,   410,     0,     0,
       0,     0,     0,   351,     0,     0,     0,     0,   352,     0,
       0,   353,     0,     0,   354,   245,     0,     0,   355,     0,
       0,     0,   243,   244,     0,     0,     0,     0,   356,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   357,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     358,     0,     0,   246,   247,     0,     0,     0,   248,     0,
     961,   249,     0,   359,   360,     0,     0,     0,     0,     0,
       0,     0,   245,     0,     0,     0,   250,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,    50,   242,     0,
       0,     0,     0,     0,     0,   361,   362,   363,   364,   365,
       0,   366,     0,   367,   368,   369,   370,   371,   372,   373,
     374,   375,    51,   376,   243,   244,     0,     0,     0,     0,
     246,   247,     0,     0,     0,   248,   411,   962,   249,   377,
     378,   379,     0,   380,     0,     0,   381,   382,   345,   346,
     347,   348,   349,   250,   412,     0,   383,     0,   384,   385,
     350,   311,   251,   413,     0,     0,     0,     0,     0,     0,
       0,     0,   351,     0,     0,     0,     0,   352,     0,     0,
     353,     0,     0,   354,   245,     0,     0,   355,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   356,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   357,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,   358,
       0,     0,   246,   247,     0,     0,     0,   248,     0,  1060,
     249,     0,   359,   360,     0,     0,     0,     0,     0,     0,
       0,   742,     0,     0,     0,   250,     0,   569,   570,     0,
       0,     0,     0,     0,   251,     0,    50,     0,     0,     0,
       0,     0,     0,     0,   361,   362,   363,   364,   365,     0,
     366,     0,   367,   368,   369,   370,   371,   372,   373,   374,
     375,    51,   376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   345,   346,   347,   348,   349,     0,   377,   378,
     379,     0,   380,     0,   350,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,   383,   351,   384,   385,   743,
     311,   352,     0,     0,   353,     0,     0,   354,     0,     0,
       0,   355,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   356,   145,   146,   147,     0,   149,   150,   151,   152,
     153,   357,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   165,   166,   167,     0,     0,   170,   171,   172,
     173,     0,     0,   358,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,   360,     0,   242,
       0,     0,     0,     0,     0,   756,     0,     0,     0,     0,
       0,   569,   570,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,   243,   244,     0,   361,   362,
     363,   364,   365,     0,   366,     0,   367,   368,   369,   370,
     371,   372,   373,   374,   375,    51,   376,     0,   345,   346,
     347,   348,   349,     0,     0,     0,     0,     0,     0,     0,
     350,     0,   377,   378,   379,   242,   380,     0,     0,   381,
     382,     0,   351,     0,     0,     0,     0,   352,     0,   383,
     353,   384,   385,   354,   311,   245,     0,   355,     0,     0,
       0,   243,   244,     0,     0,     0,     0,   356,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   357,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,   358,
       0,     0,     0,   246,   247,     0,     0,     0,   248,     0,
    1109,   249,   359,   360,     0,     0,     0,     0,     0,     0,
       0,   245,     0,     0,     0,     0,   250,     0,     0,     0,
       0,     0,     0,     0,     0,   251,    50,   242,     0,     0,
       0,     0,     0,     0,   361,   362,   363,   364,   365,     0,
     366,     0,   367,   368,   369,   370,   371,   372,   373,   374,
     375,    51,   376,   243,   244,     0,     0,     0,     0,   246,
     247,     0,     0,     0,   248,     0,  1110,   249,   377,   378,
     379,     0,   380,     0,     0,   381,   382,   345,   346,   347,
     348,   349,   250,   674,     0,   383,   675,   384,   385,   350,
     311,   251,     0,     0,   242,     0,     0,     0,     0,     0,
       0,   351,     0,     0,     0,     0,   352,     0,     0,   353,
       0,     0,   354,   245,     0,     0,   355,     0,     0,     0,
     243,   244,     0,     0,     0,     0,   356,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   357,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   358,     0,
       0,   246,   247,     0,     0,     0,   248,     0,  1134,   249,
       0,   359,   360,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,     0,   250,     0,   569,   570,     0,     0,
       0,     0,     0,   251,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   361,   362,   363,   364,   365,     0,   366,
       0,   367,   368,   369,   370,   371,   372,   373,   374,   375,
      51,   376,     0,   345,   346,   347,   348,   349,   246,   247,
       0,     0,     0,   248,     0,   350,   249,   377,   378,   379,
       0,   380,     0,     0,   381,   382,     0,   351,     0,     0,
       0,   250,   352,     0,   383,   353,   384,   385,   354,   311,
     251,     0,   355,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   356,   145,   146,   147,     0,   149,   150,   151,
     152,   153,   357,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   165,   166,   167,     0,     0,   170,   171,
     172,   173,     0,     0,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   359,   360,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   932,   933,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   361,
     362,   363,   364,   365,     0,   366,     0,   367,   368,   369,
     370,   371,   372,   373,   374,   375,    51,   376,     0,   345,
     346,   347,   348,   349,     0,     0,     0,     0,     0,     0,
       0,   350,     0,   377,   378,   379,     0,   380,     0,     0,
     381,   382,     0,   351,     0,     0,     0,     0,   352,     0,
     383,   353,   384,   385,   354,   311,     0,     0,   355,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   356,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   357,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   359,   360,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   941,   942,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,   361,   362,   363,   364,   365,
       0,   366,     0,   367,   368,   369,   370,   371,   372,   373,
     374,   375,    51,   376,     0,   345,   346,   347,   348,   349,
       0,     0,     0,     0,     0,     0,     0,   350,     0,   377,
     378,   379,     0,   380,     0,     0,   381,   382,     0,   351,
       0,     0,     0,     0,   352,     0,   383,   353,   384,   385,
     354,   311,     0,     0,   355,     0,     0,     0,     0,     0,
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
       0,   345,   346,   347,   348,   349,     0,     0,   707,     0,
       0,     0,     0,   350,     0,   377,   378,   379,     0,   380,
       0,     0,   381,   382,     0,   351,     0,     0,     0,     0,
     352,   414,   383,   353,   384,   385,   354,   311,     0,     0,
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
     349,     0,     0,     0,   383,   727,   384,   385,   350,   311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,     0,     0,     0,     0,   352,     0,     0,   353,     0,
       0,   354,     0,     0,     0,   355,     0,     0,   753,     0,
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
     819,   384,   385,   350,   311,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,   821,
       0,   377,   378,   379,     0,   380,     0,     0,   381,   382,
     345,   346,   347,   348,   349,     0,     0,     0,   383,     0,
     384,   385,   350,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,     0,     0,     0,     0,   352,
       0,     0,   353,     0,     0,   354,     0,     0,     0,   355,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
     145,   146,   147,     0,   149,   150,   151,   152,   153,   357,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
       0,   358,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   359,   360,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,   361,   362,   363,   364,
     365,     0,   366,     0,   367,   368,   369,   370,   371,   372,
     373,   374,   375,    51,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     377,   378,   379,     0,   380,     0,     0,   381,   382,   345,
     346,   347,   348,   349,     0,     0,     0,   383,   841,   384,
     385,   350,   311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,     0,     0,     0,     0,   352,     0,
       0,   353,     0,     0,   354,     0,     0,     0,   355,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   356,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   357,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   359,   360,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,   361,   362,   363,   364,   365,
       0,   366,     0,   367,   368,   369,   370,   371,   372,   373,
     374,   375,    51,   376,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,     0,   380,     0,     0,   381,   382,   345,   346,
     347,   348,   349,     0,     0,     0,   383,   850,   384,   385,
     350,   311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,     0,     0,     0,     0,   352,     0,     0,
     353,     0,     0,   354,     0,     0,     0,   355,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   356,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   357,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,   358,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,   360,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,   361,   362,   363,   364,   365,     0,
     366,     0,   367,   368,   369,   370,   371,   372,   373,   374,
     375,    51,   376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,   378,
     379,     0,   380,     0,     0,   381,   382,   345,   346,   347,
     348,   349,     0,     0,     0,   383,  1000,   384,   385,   350,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,     0,     0,     0,     0,   352,     0,     0,   353,
       0,     0,   354,     0,     0,     0,   355,     0,     0,     0,
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
     349,     0,     0,     0,   383,  1047,   384,   385,   350,   311,
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
       0,     0,     0,     0,     0,     0,   377,   378,   379,     0,
     380,     0,     0,   381,   382,   345,   346,   347,   348,   349,
       0,     0,     0,   383,  1095,   384,   385,   350,   311,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
       0,     0,     0,     0,   352,     0,     0,   353,     0,     0,
     354,     0,     0,     0,   355,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   356,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   357,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   359,
     360,     0,     0,     0,     0,     0,     0,     0,  1107,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   361,   362,   363,   364,   365,     0,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,    51,   376,
       0,   345,   346,   347,   348,   349,     0,     0,     0,     0,
       0,     0,     0,   350,     0,   377,   378,   379,     0,   380,
       0,     0,   381,   382,     0,   351,     0,     0,     0,     0,
     352,     0,   383,   353,   384,   385,   354,   311,     0,     0,
     355,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     356,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     357,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   359,   360,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   501,   502,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   361,   362,   363,
     364,   365,     0,   366,     0,   367,   368,   369,   370,   371,
     372,   373,   374,   375,    51,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,   378,   379,     0,   380,     0,     0,   381,   382,
       0,     0,     0,     0,     0,     0,     0,     0,   383,     0,
     384,   385,     0,   311,   772,   773,   774,   775,   776,   777,
     778,   779,   503,   504,   505,   506,   507,   780,   781,   508,
     509,   510,   511,   907,   512,   513,   514,   515,   501,   502,
       0,   516,   783,   517,   518,   784,   785,     0,     0,   519,
     520,   521,   786,   787,   788,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   908,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,     0,     0,
       0,   917,     0,     0,     0,     0,   772,   773,   774,   775,
     776,   777,   778,   779,   503,   504,   505,   506,   507,   780,
     781,   508,   509,   510,   511,   907,   512,   513,   514,   515,
     501,   502,     0,   516,   783,   517,   518,   784,   785,     0,
       0,   519,   520,   521,   786,   787,   788,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   908,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
       0,     0,     0,   952,     0,     0,     0,     0,   772,   773,
     774,   775,   776,   777,   778,   779,   503,   504,   505,   506,
     507,   780,   781,   508,   509,   510,   511,   907,   512,   513,
     514,   515,   501,   502,     0,   516,   783,   517,   518,   784,
     785,     0,     0,   519,   520,   521,   786,   787,   788,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   908,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
       0,     0,     0,     0,     0,  1017,     0,     0,     0,     0,
     772,   773,   774,   775,   776,   777,   778,   779,   503,   504,
     505,   506,   507,   780,   781,   508,   509,   510,   511,   907,
     512,   513,   514,   515,   501,   502,     0,   516,   783,   517,
     518,   784,   785,     0,     0,   519,   520,   521,   786,   787,
     788,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   908,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,     0,     0,     0,     0,     0,  1020,     0,     0,
       0,     0,   772,   773,   774,   775,   776,   777,   778,   779,
     503,   504,   505,   506,   507,   780,   781,   508,   509,   510,
     511,   907,   512,   513,   514,   515,   501,   502,     0,   516,
     783,   517,   518,   784,   785,     0,     0,   519,   520,   521,
     786,   787,   788,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     908,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,     0,     0,     0,  1039,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   503,   504,   505,   506,   507,   501,   502,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,   668,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   501,   502,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,     0,     0,     0,
     519,   520,   521,     0,     0,     0,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,   796,
       0,     0,     0,     0,   503,   504,   505,   506,   507,   501,
     502,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
     824,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     501,   502,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,   827,     0,     0,     0,     0,   503,   504,   505,   506,
     507,   501,   502,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
       0,     0,     0,   519,   520,   521,     0,     0,     0,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
       0,     0,   829,     0,     0,     0,     0,   503,   504,   505,
     506,   507,   501,   502,   508,   509,   510,   511,     0,   512,
     513,   514,   515,     0,     0,     0,   516,     0,   517,   518,
       0,     0,     0,     0,   519,   520,   521,     0,     0,     0,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,   831,     0,     0,     0,     0,   503,   504,
     505,   506,   507,   501,   502,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,     0,     0,   832,     0,     0,     0,     0,   503,
     504,   505,   506,   507,   501,   502,   508,   509,   510,   511,
       0,   512,   513,   514,   515,     0,     0,     0,   516,     0,
     517,   518,     0,     0,     0,     0,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,     0,     0,   833,     0,     0,     0,     0,
     503,   504,   505,   506,   507,   501,   502,   508,   509,   510,
     511,     0,   512,   513,   514,   515,     0,     0,     0,   516,
       0,   517,   518,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,   834,     0,     0,     0,
       0,   503,   504,   505,   506,   507,   501,   502,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,     0,     0,   519,   520,
     521,     0,     0,     0,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,     0,     0,   835,     0,     0,
       0,     0,   503,   504,   505,   506,   507,   501,   502,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,   836,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   501,   502,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,     0,     0,     0,
     519,   520,   521,     0,     0,     0,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,   896,
       0,     0,     0,     0,   503,   504,   505,   506,   507,   501,
     502,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
     958,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     501,   502,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,  1012,     0,     0,     0,     0,   503,   504,   505,   506,
     507,   501,   502,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
       0,     0,     0,   519,   520,   521,     0,     0,     0,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
       0,     0,  1058,     0,     0,     0,     0,   503,   504,   505,
     506,   507,   501,   502,   508,   509,   510,   511,     0,   512,
     513,   514,   515,     0,     0,     0,   516,     0,   517,   518,
       0,     0,     0,     0,   519,   520,   521,     0,     0,     0,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,  1059,     0,     0,     0,     0,   503,   504,
     505,   506,   507,   501,   502,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,     0,     0,  1069,     0,     0,     0,     0,   503,
     504,   505,   506,   507,   501,   502,   508,   509,   510,   511,
       0,   512,   513,   514,   515,     0,     0,     0,   516,     0,
     517,   518,     0,     0,     0,     0,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,     0,     0,  1071,     0,     0,     0,     0,
     503,   504,   505,   506,   507,   501,   502,   508,   509,   510,
     511,     0,   512,   513,   514,   515,     0,     0,     0,   516,
       0,   517,   518,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,  1073,     0,     0,     0,
       0,   503,   504,   505,   506,   507,   501,   502,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,     0,     0,   519,   520,
     521,     0,     0,     0,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,     0,     0,  1077,     0,     0,
       0,     0,   503,   504,   505,   506,   507,   501,   502,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,  1112,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   501,   502,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,     0,     0,     0,
     519,   520,   521,     0,     0,     0,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,  1113,
       0,     0,     0,     0,   503,   504,   505,   506,   507,   501,
     502,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
    1114,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     501,   502,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,  1121,     0,     0,     0,     0,   503,   504,   505,   506,
     507,   501,   502,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
       0,     0,     0,   519,   520,   521,     0,     0,     0,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
       0,     0,  1123,     0,     0,     0,     0,   503,   504,   505,
     506,   507,   501,   502,   508,   509,   510,   511,     0,   512,
     513,   514,   515,     0,     0,     0,   516,     0,   517,   518,
       0,     0,     0,     0,   519,   520,   521,     0,     0,     0,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,  1146,     0,     0,     0,     0,   503,   504,
     505,   506,   507,   501,   502,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,   539,     0,     0,     0,     0,     0,     0,   503,
     504,   505,   506,   507,   501,   502,   508,   509,   510,   511,
       0,   512,   513,   514,   515,     0,     0,     0,   516,     0,
     517,   518,     0,     0,     0,     0,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,   880,     0,     0,     0,     0,     0,     0,
     503,   504,   505,   506,   507,   501,   502,   508,   509,   510,
     511,     0,   512,   513,   514,   515,     0,     0,     0,   516,
       0,   517,   518,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,   894,     0,     0,     0,     0,     0,
       0,   503,   504,   505,   506,   507,   501,   502,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,     0,     0,   519,   520,
     521,     0,     0,     0,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,  1007,     0,     0,     0,     0,
       0,     0,   503,   504,   505,   506,   507,     0,     0,   508,
     509,   510,   511,     0,   512,   513,   514,   515,   501,   502,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,  1010,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   772,   773,   774,   775,
     776,   777,   778,   779,   503,   504,   505,   506,   507,   780,
     781,   508,   509,   510,   511,   907,   512,   513,   514,   515,
    -275,   501,   502,   516,   783,   517,   518,   784,   785,     0,
       0,   519,   520,   521,   786,   787,   788,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   908,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   772,
     773,   774,   775,   776,   777,   778,   779,   503,   504,   505,
     506,   507,   780,   781,   508,   509,   510,   511,   782,   512,
     513,   514,   515,     0,   256,   257,   516,   783,   517,   518,
     784,   785,     0,     0,   519,   520,   521,   786,   787,   788,
     522,   258,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   789,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     501,   502,   277,   278,   279,     0,     0,     0,     0,     0,
     280,   281,   282,   283,   284,     0,     0,   285,   286,   287,
     288,   289,   290,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   292,     0,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,     0,     0,   303,   304,     0,     0,     0,
     680,     0,   305,   306,     0,     0,     0,   501,   502,     0,
       0,     0,     0,     0,     0,     0,   503,   504,   505,   506,
     507,     0,     0,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
       0,   661,     0,   519,   520,   521,   145,   146,   147,   522,
     149,   150,   151,   152,   153,   357,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   501,   502,   503,   504,   505,   506,   507,   534,   535,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,     0,     0,     0,
     519,   520,   521,     0,     0,     0,   522,     0,     0,   681,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   682,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   885,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,   501,   502,
       0,     0,     0,     0,     0,   534,   535,   503,   504,   505,
     506,   507,     0,     0,   508,   509,   510,   511,     0,   512,
     513,   514,   515,     0,     0,     0,   516,     0,   517,   518,
       0,     0,     0,     0,   519,   520,   521,     0,     0,     0,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   981,     0,     0,     0,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,   501,   502,   503,   504,   505,   506,   507,   534,
     535,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,   501,
     502,     0,     0,     0,     0,     0,   534,   535,   503,   504,
     505,   506,   507,     0,     0,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,     0,   501,   502,     0,   519,   520,   521,     0,     0,
       0,  -604,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     534,   535,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,   501,
     502,     0,   519,   520,   521,     0,     0,     0,   503,   504,
     505,   506,   507,     0,     0,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,   501,   502,     0,     0,   519,   523,   521,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     534,   535,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,   501,   502,
       0,     0,   519,     0,     0,     0,     0,   503,   504,   505,
     506,   507,     0,     0,   508,   509,   510,   511,     0,   512,
     513,   514,   515,     0,     0,     0,   516,     0,   517,   518,
       0,     0,   501,   502,     0,     0,     0,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,   503,   504,     0,     0,   507,   534,
     535,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,   501,
     502,   519,   520,   521,     0,     0,     0,   522,   503,   504,
     505,   506,   507,     0,     0,   508,   509,   510,   511,     0,
     512,   513,   514,   515,   501,   502,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,     0,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     534,   535,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
     503,   504,   505,   506,     0,     0,     0,     0,     0,     0,
     511,     0,   512,   513,   514,   515,     0,     0,     0,   516,
       0,   517,   518,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,     0,
     141,     0,     0,     0,     0,     0,   142,   534,   535,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   529,   530,
     531,   532,   533,   143,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,     0,     0,   144,     0,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   484,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   141,     0,     0,     0,     0,     0,
     142,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,     0,   143,     0,   684,
       0,   176,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   144,    51,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   357,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   881,
       0,     0,     0,     0,     0,   176,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   685,     0,
       0,     0,     0,     0,     0,   145,   146,   147,   686,   149,
     150,   151,   152,   153,   357,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   141,
       0,     0,     0,     0,     0,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   143,     0,     0,     0,     0,     0,   882,     0,
       0,     0,     0,     0,     0,     0,     0,   144,   883,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   141,     0,     0,     0,     0,     0,   142,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,   143,     0,     0,     0,
     176,     0,     0,   367,   368,   369,     0,     0,     0,     0,
       0,   144,    51,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   141,     0,     0,
       0,     0,     0,   142,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,     0,
     143,     0,     0,     0,   176,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   144,    51,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   176,     0,
       0,   189,     0,   190,     0,   191,   192,   193,   194,   195,
     458,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   205,   206,   207,     0,     0,   208,   209,   210,   211,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   214
};

static const yytype_int16 yycheck[] =
{
      12,   111,   216,   128,   430,   537,   453,   544,   455,   338,
     457,   340,   548,   342,   485,   394,   369,   651,   569,   570,
     419,     7,   437,    14,    15,   659,   111,   557,   674,    51,
      19,    19,    81,    20,    21,    19,    51,    33,   568,    21,
      18,    19,    48,     0,   482,   131,   484,    44,    45,     6,
     157,    44,   116,   144,   117,   119,   120,   148,   117,   157,
     117,   157,   117,   165,   168,    61,   122,   126,   157,   126,
      82,   126,    29,    55,    31,   132,    33,   157,    32,   609,
     119,   120,    39,     4,     5,   151,     7,   159,   160,   178,
     197,    48,   196,   195,   645,   191,   628,    54,   196,   111,
      97,   192,   165,   166,    97,   168,   117,   166,   171,   166,
     196,   166,   171,   157,    35,   126,    73,   197,   556,   106,
     107,   108,   109,   110,   188,   189,   113,   114,   115,   116,
      32,   118,   119,   120,   121,   141,   132,    94,   125,   195,
     127,   128,   157,   166,   165,   117,   133,   134,   135,   188,
     189,   157,   139,   197,   126,   166,    58,    59,    14,    15,
     151,   157,   157,    60,   155,   157,   157,   158,   132,   159,
     160,   192,   195,   195,   223,   590,   165,   165,   157,   157,
     167,   165,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    20,    21,   166,   177,    41,   843,   193,   324,
     191,   188,   189,   189,   168,   197,   188,   841,   738,   196,
     165,   197,   742,   165,   165,   165,   118,   171,   197,   770,
     599,   331,   332,   333,   334,   166,   756,   337,   627,   339,
      75,   341,   189,   343,   165,   189,   132,   347,   195,   165,
     195,   192,   192,   195,   198,   141,   331,   332,   333,   334,
     146,   144,   337,   173,   339,   148,   341,   367,   368,   165,
     165,   157,   347,   191,   166,   167,   192,   165,   196,   171,
     800,   173,   174,   165,   168,   195,   172,    20,    21,   165,
     108,   109,   367,   368,   173,   190,   192,   189,   116,   165,
     118,   119,   120,   121,   192,   151,   198,   125,   132,   155,
     192,   157,   158,   132,   190,   837,   195,   141,   418,   662,
    1117,   165,    20,    21,   119,   120,   165,   849,   428,   331,
     332,   333,   334,   157,   165,   337,  1105,   339,   157,   341,
    1137,   343,   442,   418,   968,   347,   190,   173,   141,    20,
      21,   190,   147,   428,   978,   559,   878,   173,   157,   190,
     679,  1130,   157,   567,   157,   367,   368,   442,   141,   195,
     188,   189,   168,   473,   900,   108,   109,   173,   380,   195,
     841,   157,   904,   116,   157,   157,   119,   120,   121,   850,
     147,   168,   125,   188,   189,   191,   173,   147,   473,    55,
     157,   501,   502,   193,   194,    61,   196,   157,   106,   107,
     108,   109,   110,    11,   191,   113,   418,   195,   116,   197,
     118,   119,   120,   121,    22,    23,   428,   125,   168,   127,
     128,   147,   434,   173,   168,   132,   873,   108,   109,   173,
     442,   157,   964,   157,   970,   116,    55,   118,   119,   120,
     121,   191,    61,   190,   125,   188,   189,   191,   195,   165,
      68,    55,   168,   465,    72,   171,    55,    61,   191,   151,
     192,   473,    61,   195,   172,   173,   174,   175,   176,   177,
     178,    89,    90,    91,    92,   188,    55,    55,   157,   157,
     188,   189,    61,    61,   166,   497,   159,   160,   161,   162,
     157,    20,    21,   174,   175,   176,   177,   178,   159,   160,
     161,   195,   612,   197,   195,   157,   196,   188,   189,   157,
     196,    55,   195,   623,   157,   625,   196,    97,   148,   172,
     630,   172,   632,   633,   195,   635,   740,   612,   172,   191,
     172,   172,   172,   862,   172,   749,   172,   172,   623,   986,
     172,   191,   157,    34,    34,   630,   191,   632,   633,   196,
     635,   765,  1089,    32,   157,   157,   168,   190,   197,    21,
      41,   195,   190,   168,   190,   196,   191,   168,   192,   196,
     191,   191,   172,   189,   172,   172,   172,   106,   107,   108,
     109,   110,   191,   172,   113,   114,   115,   116,   194,   118,
     119,   120,   121,   157,   157,  1021,   125,   707,   127,   128,
     612,   191,   173,   191,   133,   134,   135,   191,   191,   191,
     139,   623,   148,   625,    20,    21,   191,   191,   630,   191,
     632,   633,   191,   635,   190,   157,   736,    41,   195,   191,
     157,   151,    36,    41,   195,   195,   195,   195,   167,   195,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     157,   736,    32,   165,    20,    21,   173,   173,   173,   188,
     189,   165,   190,    32,   197,   159,   195,   191,   191,   157,
     795,   172,   191,   172,     1,   157,   122,   191,    58,    59,
     191,   191,   157,   191,    13,   168,   166,   151,   194,    58,
      59,   196,     7,   191,   157,   190,   910,   157,   157,   157,
     106,   107,   108,   109,   110,   190,   190,   113,   114,   115,
     116,   197,   118,   119,   120,   121,   190,   159,   157,   125,
     157,   127,   128,   157,   736,   172,   191,   133,   134,   135,
     191,    41,   241,   139,   191,   157,   191,   117,   118,   195,
     190,   250,   108,   109,   192,   190,   126,    20,    21,   118,
     116,   861,   118,   119,   120,   121,   157,   190,   190,   125,
     195,   167,   190,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   190,   197,   173,   861,    32,   173,   190,
     792,   995,   188,   189,    51,   190,   166,   167,   190,   195,
     197,   171,   195,   190,   174,    63,   190,   166,   167,    37,
     489,   108,   171,    66,  1116,   174,    64,   228,  1022,   189,
     176,   177,   178,    82,   919,   195,   792,     1,   198,   993,
     189,   830,   188,   189,  1034,  1037,   195,   550,   625,   198,
     605,   344,    45,   106,   107,   108,   109,   110,   431,   336,
     113,   114,   115,   116,   347,   118,   119,   120,   121,   861,
     359,   360,   125,  1103,   127,   128,   485,    -1,    -1,    -1,
     369,    -1,    -1,   848,    -1,    -1,    -1,  1081,   377,   378,
     379,  1085,   381,   382,   383,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   996,    -1,   998,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,
      -1,   996,    -1,   998,    -1,   188,   189,    -1,    -1,    -1,
      -1,   430,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     439,  1145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   467,    -1,
      -1,   470,    -1,   472,  1074,    58,    59,   476,   477,   478,
     479,   480,   481,    -1,   483,    -1,   485,    -1,    -1,    -1,
      -1,    -1,    58,    59,   996,    -1,   998,    -1,    -1,  1074,
      -1,    -1,    -1,    -1,   503,   504,    -1,    -1,   507,   508,
     509,   510,    -1,   512,    -1,   514,   515,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,   118,   535,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   543,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,   553,    -1,    -1,    -1,   557,    -1,
      -1,   560,   561,    -1,    -1,    -1,    -1,   566,    -1,   568,
      -1,    -1,  1074,    -1,    -1,    -1,   575,    -1,    -1,    -1,
      -1,    -1,    -1,   166,   167,    -1,    -1,    -1,   171,    -1,
     173,   174,    -1,    -1,    -1,    20,    21,   596,    -1,    -1,
     166,   167,    -1,  1105,    -1,   171,   189,   173,   174,    -1,
     609,    -1,    -1,    -1,    -1,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,  1130,    -1,
     629,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   647,   648,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   658,
      -1,    -1,   661,   662,    -1,    -1,   665,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,   720,    -1,    -1,    -1,   724,    -1,    -1,    -1,    -1,
      -1,    -1,   731,    -1,    -1,    -1,    -1,    -1,    -1,   738,
      -1,   166,   167,   742,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    -1,   756,    -1,    -1,
      -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,
     195,    -1,    -1,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
     789,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   800,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     839,   840,    -1,    -1,    -1,    -1,    32,   846,    -1,   848,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    32,    -1,
     859,   860,    -1,    -1,    -1,    -1,   865,    -1,    -1,    -1,
      -1,    -1,    58,    59,    -1,    -1,   875,    -1,   877,    -1,
     879,    -1,    -1,    -1,    58,    59,   885,    -1,    -1,    -1,
     889,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,   907,   908,
     125,    -1,   127,   128,    -1,    -1,   118,    -1,   133,   134,
     135,    -1,    -1,    32,   139,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,   932,   933,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   941,   942,   118,    -1,    -1,    -1,    -1,    58,
      59,    -1,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   166,   167,    -1,    -1,    -1,   171,
      -1,   173,   174,   188,   189,    -1,    -1,   192,    -1,    -1,
     166,   167,    -1,    -1,    -1,   171,    -1,   189,   174,    -1,
      -1,    -1,   166,   167,    -1,    -1,   198,   171,    -1,   173,
     174,    -1,    -1,   189,    -1,  1004,    -1,  1006,    -1,   118,
    1009,    -1,   198,    -1,    -1,   189,  1015,    -1,    -1,    -1,
    1019,    -1,  1021,    -1,   198,    -1,  1025,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1040,    -1,    32,  1043,  1044,  1045,  1046,    -1,    -1,
    1049,    -1,    -1,  1052,  1053,    -1,  1055,   166,   167,    -1,
      -1,    -1,   171,    -1,   173,   174,  1065,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
     189,    -1,    -1,    -1,     6,    -1,     8,     9,    10,   198,
      12,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    -1,  1107,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,
      42,    43,    -1,  1122,    46,    -1,    48,    49,    50,   118,
      52,    53,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,
    1139,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    96,    -1,    -1,   166,   167,    -1,
      -1,    -1,   171,    -1,   173,   174,   108,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,   198,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,
     182,    -1,    -1,    -1,    -1,    -1,    58,    59,    -1,   191,
      -1,   193,   194,   195,   196,   197,     1,    -1,    -1,    -1,
      -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    46,    -1,    48,    49,    50,   118,    52,    53,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    96,    -1,    -1,   166,   167,    -1,    -1,    -1,   171,
      -1,   173,   174,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,   198,   132,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,    -1,   178,    -1,    -1,   181,   182,    -1,    -1,
      -1,    14,    15,    16,    17,    18,   191,    -1,   193,   194,
     195,   196,   197,    26,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,   118,    49,    -1,    51,    -1,
      53,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    95,   166,   167,    -1,    -1,    -1,   171,    -1,
      -1,   174,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   189,    -1,   191,    -1,
      -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,    -1,   132,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    58,    59,    -1,    -1,
      -1,    -1,    -1,   166,   167,    -1,    -1,    -1,   171,    -1,
      -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,   182,
      14,    15,    16,    17,    18,    -1,   189,    -1,   191,    -1,
     193,   194,    26,   196,    -1,   198,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,   118,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,   166,   167,    -1,    -1,    -1,   171,
      -1,   173,   174,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,   132,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    58,    59,    -1,    -1,    -1,
      -1,    -1,   166,   167,    -1,    -1,    -1,   171,    -1,    -1,
     174,   175,   176,    -1,   178,    -1,    -1,   181,   182,    14,
      15,    16,    17,    18,    -1,   189,    -1,   191,    -1,   193,
     194,    26,   196,    -1,   198,    -1,    32,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,   118,    -1,    -1,    53,    -1,
      -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,   166,   167,    -1,    -1,    -1,   171,    -1,
     173,   174,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   198,    -1,   132,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    58,    59,    -1,    -1,    -1,    -1,
     166,   167,    -1,    -1,    -1,   171,   171,   173,   174,   174,
     175,   176,    -1,   178,    -1,    -1,   181,   182,    14,    15,
      16,    17,    18,   189,   189,    -1,   191,    -1,   193,   194,
      26,   196,   198,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,   118,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,   166,   167,    -1,    -1,    -1,   171,    -1,   173,
     174,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,   189,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,   198,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,   174,   175,
     176,    -1,   178,    -1,    26,   181,   182,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   191,    38,   193,   194,   195,
     196,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    58,    59,    -1,   140,   141,
     142,   143,   144,    -1,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   174,   175,   176,    32,   178,    -1,    -1,   181,
     182,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   191,
      46,   193,   194,    49,   196,   118,    -1,    53,    -1,    -1,
      -1,    58,    59,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    -1,   166,   167,    -1,    -1,    -1,   171,    -1,
     173,   174,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   198,   132,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    58,    59,    -1,    -1,    -1,    -1,   166,
     167,    -1,    -1,    -1,   171,    -1,   173,   174,   174,   175,
     176,    -1,   178,    -1,    -1,   181,   182,    14,    15,    16,
      17,    18,   189,   189,    -1,   191,   192,   193,   194,    26,
     196,   198,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,   118,    -1,    -1,    53,    -1,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,   166,   167,    -1,    -1,    -1,   171,    -1,   173,   174,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,   189,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,   198,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    14,    15,    16,    17,    18,   166,   167,
      -1,    -1,    -1,   171,    -1,    26,   174,   174,   175,   176,
      -1,   178,    -1,    -1,   181,   182,    -1,    38,    -1,    -1,
      -1,   189,    43,    -1,   191,    46,   193,   194,    49,   196,
     198,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   174,   175,   176,    -1,   178,    -1,    -1,
     181,   182,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     191,    46,   193,   194,    49,   196,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   174,
     175,   176,    -1,   178,    -1,    -1,   181,   182,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   191,    46,   193,   194,
      49,   196,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
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
      -1,    14,    15,    16,    17,    18,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    26,    -1,   174,   175,   176,    -1,   178,
      -1,    -1,   181,   182,    -1,    38,    -1,    -1,    -1,    -1,
      43,   190,   191,    46,   193,   194,    49,   196,    -1,    -1,
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
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    56,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,   182,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   191,    -1,
     193,   194,    26,   196,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   175,   176,    -1,   178,    -1,    -1,   181,   182,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   191,   192,   193,
     194,    26,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,    -1,   178,    -1,    -1,   181,   182,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   191,   192,   193,   194,
      26,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,
     176,    -1,   178,    -1,    -1,   181,   182,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   191,   192,   193,   194,    26,
     196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,    -1,
     178,    -1,    -1,   181,   182,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   191,   192,   193,   194,    26,   196,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   174,   175,   176,    -1,   178,
      -1,    -1,   181,   182,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   191,    46,   193,   194,    49,   196,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,   182,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,    -1,
     193,   194,    -1,   196,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    20,    21,
      -1,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,
      -1,   195,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      20,    21,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,   167,    -1,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,
      -1,    -1,    -1,   195,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    20,    21,    -1,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,   167,    -1,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,
      -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    20,    21,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,   167,
      -1,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    20,    21,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,   167,    -1,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,   190,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    20,    21,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,   190,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    20,    21,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,   167,    -1,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,    20,    21,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,   137,   138,
     139,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      20,    21,   118,   119,   120,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,   181,   182,    -1,    -1,    -1,
      18,    -1,   188,   189,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,   131,    -1,   133,   134,   135,    64,    65,    66,   139,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,   167,    -1,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,    20,    21,   106,   107,   108,   109,   110,   188,   189,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,   188,   189,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    20,    21,   106,   107,   108,   109,   110,   188,
     189,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,   188,   189,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    20,    21,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     188,   189,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    20,
      21,    -1,   133,   134,   135,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    20,    21,    -1,    -1,   133,   167,   135,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     188,   189,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    20,    21,
      -1,    -1,   133,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    -1,    -1,   106,   107,    -1,    -1,   110,   188,
     189,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    20,
      21,   133,   134,   135,    -1,    -1,    -1,   139,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    20,    21,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,
     172,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     188,   189,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
     106,   107,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,   188,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,
     176,   177,   178,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    62,    -1,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    18,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,   157,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    65,    66,   157,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,   157,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,
     145,    -1,    -1,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    62,   157,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,   157,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    70,    71,    72,
     157,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   200,     0,     6,    29,    31,    33,    39,    48,    54,
      73,    94,   189,   195,   203,   211,   212,   216,   239,   243,
     260,   327,   333,   336,   342,   380,   383,    18,    19,   157,
     233,   234,   235,   151,   217,   218,   157,   178,   213,   214,
     157,   196,   330,   157,   193,   202,   384,   381,    33,    61,
     132,   157,   236,   237,   238,   252,     4,     5,     7,    35,
     340,    60,   325,   166,   165,   168,   165,   213,    21,    55,
     177,   188,   215,   331,   330,   332,   325,   157,   157,   157,
     132,   191,   165,   190,    55,    61,   244,   246,    55,    61,
     334,    55,    61,   341,    55,    61,   326,    14,    15,   151,
     155,   157,   158,   191,   205,   234,   151,   218,   157,   157,
     157,   166,   195,   197,   330,    55,    61,   201,   196,   382,
     157,   235,   237,   247,   157,   335,   343,   196,   328,   159,
     160,   204,    14,    15,   151,   155,   157,   205,   231,   232,
     215,    24,    30,    47,    62,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   145,   252,   346,   348,
     349,   351,   355,   195,   157,   350,   196,   192,    34,    64,
      66,    68,    69,    70,    71,    72,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    84,    85,    86,    89,    90,
      91,    92,   108,   109,   157,   242,   245,   168,   196,    97,
     338,   339,   323,   148,   302,   159,   160,   161,   165,   192,
     172,   172,   172,   172,   191,   172,   172,   172,   172,   172,
     172,   191,    32,    58,    59,   118,   166,   167,   171,   174,
     189,   198,   195,   157,   315,   316,    20,    21,    37,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   118,   119,   120,
     126,   127,   128,   129,   130,   133,   134,   135,   136,   137,
     138,   139,   167,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   181,   182,   188,   189,    34,    34,   191,
     240,   196,   248,    68,    72,    89,    90,    91,    92,   347,
     329,   157,   344,   197,   324,   235,   141,   157,   321,   322,
     231,   358,   360,   362,   356,   157,   352,   364,   366,   368,
     370,   372,   374,   376,   378,    14,    15,    16,    17,    18,
      26,    38,    43,    46,    49,    53,    63,    73,    95,   108,
     109,   140,   141,   142,   143,   144,   146,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   158,   174,   175,   176,
     178,   181,   182,   191,   193,   194,   207,   209,   248,   252,
     254,   262,   269,   272,   275,   279,   280,   283,   284,   286,
     291,   294,   301,   346,   385,   389,   394,   396,   398,    32,
      32,   171,   189,   198,   190,   294,   157,   197,   168,   195,
     190,    21,   190,   144,   192,   302,   311,   312,   168,   241,
     250,   196,   157,   197,   168,   337,   196,   302,   190,   191,
      41,   165,   168,   171,   320,   355,   354,   355,   355,   355,
     192,   350,   355,   240,   355,   240,   355,   240,   157,   313,
     314,   355,   316,   355,   385,   191,   172,   191,   172,   172,
     191,   172,   191,   172,   294,   294,   191,   191,   191,   191,
     191,   191,    12,   355,    12,   355,   294,   391,   395,   206,
     294,   294,   294,   252,   294,   294,   294,   194,   157,   191,
     239,    20,    21,   106,   107,   108,   109,   110,   113,   114,
     115,   116,   118,   119,   120,   121,   125,   127,   128,   133,
     134,   135,   139,   167,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   188,   189,   192,   191,   190,   190,
     355,   197,   315,   191,    48,   141,   157,   318,   345,   192,
     195,   355,     1,     8,     9,    10,    12,    25,    27,    28,
      37,    40,    42,    50,    52,    56,    57,    63,    96,   123,
     124,   195,   197,   219,   220,   223,   224,   225,   226,   227,
     228,   229,   249,   251,   253,   255,   256,   257,   258,   259,
     260,   261,   281,   282,   294,   329,   166,   195,   252,   307,
     322,   190,   294,   157,   157,   355,   117,   126,   166,   319,
     173,   173,   195,   173,   173,   173,   195,   173,   241,   173,
     241,   173,   241,   168,   173,   195,   173,   195,   191,   252,
     270,   294,   263,   265,   294,   267,   294,   355,   294,   294,
     294,   294,   294,   294,   345,    51,   157,   174,   191,   294,
     386,   387,   388,   390,   391,   392,   393,   345,   191,   387,
     393,   131,   195,   197,   161,   162,   204,   210,   192,   141,
     146,   172,   252,   285,   189,   192,   273,   294,   148,   278,
      18,   147,   157,   346,    18,   147,   157,   346,   294,   294,
     294,   294,   294,   294,   157,   294,   147,   157,   294,   294,
     294,   294,   294,   294,   294,   294,   294,    21,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   119,
     120,   147,   157,   188,   189,   292,   294,   192,   273,   294,
     318,   191,    41,   117,   165,   166,   168,   171,   317,   311,
     294,   345,   117,   195,   251,   281,   294,   248,   294,   294,
     157,   195,   151,    56,   294,   248,   117,   251,   294,   194,
     279,   279,    36,   195,   195,   294,   195,   195,   195,   322,
     122,   195,    98,    99,   100,   101,   102,   103,   104,   105,
     111,   112,   117,   126,   129,   130,   136,   137,   138,   166,
     197,   294,   189,   197,   239,   308,   192,    41,   166,   195,
     319,   251,   294,   359,   361,   355,   363,   357,   353,   365,
     173,   369,   173,   373,   173,   355,   377,   313,   379,   192,
     273,   172,   294,   355,   192,   355,   355,   192,   355,   192,
     173,   192,   192,   192,   192,   192,   192,    19,   279,   126,
     317,   192,   165,   195,   388,   190,   165,   190,   195,    19,
     192,   388,   197,   294,   391,   197,   294,   159,   208,   191,
     191,   287,   289,   157,   386,   165,   192,   117,   126,   166,
     171,   276,   277,   240,   172,   191,   172,   191,   191,   191,
     190,    18,   147,   157,   346,   168,   147,   157,   294,   191,
     147,   157,   294,     1,   190,   192,   192,   294,   157,   157,
     355,   251,   294,   248,    19,   251,   294,   117,   166,   195,
      13,   248,   166,   168,   151,   251,   294,   195,   194,   196,
     248,   279,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   123,   124,   294,   294,   294,   294,   294,   294,
     294,   123,   124,   294,   195,   238,     7,   302,   306,   157,
     251,   294,   195,   367,   371,   375,   192,   157,   192,   173,
     173,   173,   173,   278,   191,   273,   294,   294,   387,   388,
     157,   386,   190,   190,   294,   190,   392,   273,   387,   197,
     190,   163,   159,   294,   294,   355,   240,   190,   294,   157,
     157,   157,   157,   165,   190,   241,   295,   294,   297,   294,
     192,   273,   294,   172,   191,   294,   191,   190,   294,   191,
     190,   293,   192,    41,   166,   317,   273,   195,   248,   230,
     195,   250,    11,    22,    23,   221,   222,   294,   294,   294,
     294,   190,    55,    61,   305,    44,    97,   303,   195,   195,
     173,   195,   271,   264,   266,   268,   191,   192,   273,   195,
     388,   190,   126,   317,   190,   195,   388,   190,   192,   192,
     173,   241,   192,   276,   190,   131,   248,   274,   355,   192,
     355,   192,   192,   192,   299,   294,   294,   192,   294,   157,
     294,   195,   294,   197,   248,   294,   195,    45,   303,   305,
     294,   157,   294,   294,   294,   192,   294,   192,   294,   190,
     190,   294,   294,   294,   197,   288,   173,   117,   294,   173,
     173,   355,   192,   192,   192,   248,   248,   309,    32,   304,
     318,   192,   173,   192,   195,   397,   190,   397,   190,   285,
     290,   294,   296,   298,   173,   222,   245,   310,   294,    51,
     190,   197,   285,   300,   195,   245,   192,   294,   190,   190,
     248
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
     254,   255,   256,   257,   257,   257,   257,   257,   258,   258,
     258,   258,   259,   260,   260,   261,   263,   264,   262,   265,
     266,   262,   267,   268,   262,   270,   271,   269,   272,   272,
     272,   273,   273,   274,   274,   274,   275,   275,   275,   276,
     276,   276,   276,   277,   277,   278,   278,   279,   279,   280,
     280,   280,   280,   280,   280,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   282,   282,   282,   282,   283,   284,
     284,   285,   285,   286,   287,   288,   286,   289,   290,   286,
     291,   291,   292,   293,   291,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   295,   296,   294,   294,   294,   294,
     297,   298,   294,   294,   294,   299,   300,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   302,   302,   303,   303,   303,
     304,   304,   305,   305,   305,   306,   307,   308,   307,   309,
     307,   310,   307,   307,   311,   311,   311,   312,   312,   313,
     313,   314,   314,   315,   316,   316,   317,   317,   318,   318,
     318,   318,   318,   318,   319,   319,   319,   320,   320,   321,
     321,   321,   321,   321,   322,   322,   322,   322,   322,   323,
     324,   323,   325,   325,   326,   326,   326,   327,   328,   327,
     329,   329,   329,   331,   330,   332,   332,   333,   333,   334,
     334,   334,   335,   336,   336,   337,   337,   338,   338,   339,
     340,   340,   341,   341,   341,   343,   344,   342,   345,   345,
     345,   345,   345,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   347,   347,
     347,   347,   347,   347,   348,   349,   349,   349,   350,   350,
     352,   353,   351,   354,   354,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   356,   357,   355,   355,   358,   359,   355,   360,
     361,   355,   362,   363,   355,   355,   364,   365,   355,   366,
     367,   355,   355,   368,   369,   355,   370,   371,   355,   355,
     372,   373,   355,   374,   375,   355,   376,   377,   355,   378,
     379,   355,   381,   382,   380,   384,   383,   385,   385,   385,
     385,   386,   386,   386,   386,   387,   387,   388,   388,   389,
     389,   389,   389,   389,   389,   390,   390,   391,   391,   392,
     392,   393,   393,   394,   394,   395,   395,   396,   397,   397,
     398,   398
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
       2,     1,     1,     2,     3,     4,     2,     3,     3,     4,
       2,     3,     4,     1,     1,     2,     0,     0,     7,     0,
       0,     7,     0,     0,     7,     0,     0,     6,     5,     8,
      10,     1,     3,     1,     2,     3,     1,     1,     2,     2,
       2,     2,     2,     1,     3,     0,     4,     1,     6,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     4,     4,     4,     4,     6,     5,
       6,     1,     4,     3,     0,     0,     8,     0,     0,     9,
       3,     4,     0,     0,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     3,     4,     5,     4,     5,
       3,     4,     1,     3,     4,     3,     4,     2,     4,     4,
       7,     8,     3,     5,     0,     0,     8,     3,     3,     3,
       0,     0,     8,     3,     4,     0,     0,     9,     4,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     4,
       4,     4,     4,     4,     4,     6,     7,     6,     6,     7,
       7,     6,     7,     6,     6,     0,     4,     0,     1,     1,
       0,     1,     0,     1,     1,     4,     0,     0,     4,     0,
       8,     0,     9,     5,     2,     3,     4,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     1,     1,     2,
       3,     5,     3,     3,     1,     1,     1,     0,     1,     1,
       4,     3,     3,     5,     4,     6,     5,     5,     4,     0,
       0,     4,     0,     1,     0,     1,     1,     6,     0,     6,
       0,     3,     5,     0,     4,     2,     3,     4,     2,     0,
       1,     1,     1,     7,     9,     0,     2,     0,     1,     3,
       1,     1,     0,     1,     1,     0,     0,     9,     1,     4,
       3,     3,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     1,     3,
       0,     0,     6,     1,     3,     1,     1,     1,     1,     4,
       3,     4,     2,     2,     3,     2,     3,     2,     2,     3,
       3,     2,     0,     0,     6,     2,     0,     0,     6,     0,
       0,     6,     0,     0,     6,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     0,     0,     6,     0,
       0,     6,     0,     0,     8,     0,     7,     1,     1,     1,
       1,     3,     3,     5,     5,     1,     3,     0,     2,     6,
       5,     7,     8,     6,     8,     1,     3,     3,     1,     1,
       1,     1,     3,     5,     5,     1,     3,     4,     0,     3,
      10,    10
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

  case 228: /* expression_yield: "yield" expr ';'  */
                                         {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 229: /* expression_yield: "yield" "<-" expr ';'  */
                                                {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 230: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 231: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 232: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 233: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 234: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 235: /* expression_let: kwd_let let_variable_declaration  */
                                               {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 236: /* $@4: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 237: /* $@5: %empty  */
                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 238: /* expr_cast: "cast" '<' $@4 type_declaration '>' $@5 expr  */
                                                                                                                                     {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 239: /* $@6: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 240: /* $@7: %empty  */
                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 241: /* expr_cast: "upcast" '<' $@6 type_declaration '>' $@7 expr  */
                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 242: /* $@8: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 243: /* $@9: %empty  */
                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 244: /* expr_cast: "reinterpret" '<' $@8 type_declaration '>' $@9 expr  */
                                                                                                                                            {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 245: /* $@10: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 246: /* $@11: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 247: /* expr_type_decl: "type" '<' $@10 type_declaration '>' $@11  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 248: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 249: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 250: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 251: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 252: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 253: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 254: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 255: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 256: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 257: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 258: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 259: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 260: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 261: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 262: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 263: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 264: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 265: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 266: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 267: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 268: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 269: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 270: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 271: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 272: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 273: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 274: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 275: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 276: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 277: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 278: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 279: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 280: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 281: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 282: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 283: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 284: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 285: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 286: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 287: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 288: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 289: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 290: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 291: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 292: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 293: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 294: /* expr_assign_pipe: expr '=' "@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 295: /* expr_assign_pipe: expr '=' "@@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 296: /* expr_assign_pipe: expr "<-" "@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 297: /* expr_assign_pipe: expr "<-" "@@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 298: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 299: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 300: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 301: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 302: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 303: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 304: /* $@12: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 305: /* $@13: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 306: /* func_addr_expr: '@' '@' '<' $@12 type_declaration '>' $@13 func_addr_name  */
                                                                                                                                            {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 307: /* $@14: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 308: /* $@15: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 309: /* func_addr_expr: '@' '@' '<' $@14 optional_function_argument_list optional_function_type '>' $@15 func_addr_name  */
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

  case 310: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 311: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 312: /* $@16: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 313: /* $@17: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 314: /* expr_field: expr '.' $@16 error $@17  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 315: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 316: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 317: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 318: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 319: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 320: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 321: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 322: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 323: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 324: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 325: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 326: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 335: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 344: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 345: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 351: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 354: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 355: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 356: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 357: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 358: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 359: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 360: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 361: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 362: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 363: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 364: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 365: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 366: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 367: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 369: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 370: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' ')'  */
                                                                                                   {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 371: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' expr ')'  */
                                                                                                                 {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 372: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 374: /* $@18: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 375: /* $@19: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 376: /* expr: expr "is" "type" '<' $@18 type_declaration '>' $@19  */
                                                                                                                                            {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 377: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 378: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 379: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 380: /* $@20: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 381: /* $@21: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 382: /* expr: expr "as" "type" '<' $@20 type_declaration '>' $@21  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 383: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 384: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 385: /* $@22: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 386: /* $@23: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 387: /* expr: expr '?' "as" "type" '<' $@22 type_declaration '>' $@23  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 388: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 389: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 390: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 391: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 392: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 393: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 394: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 395: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 396: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 397: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) =ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 398: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 399: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 400: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 401: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 402: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 403: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 404: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 405: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 406: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 407: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 408: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 409: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 410: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 411: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 412: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 413: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 414: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 415: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 416: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 417: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 418: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 419: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 420: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 421: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 422: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 423: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 424: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 425: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 426: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 427: /* $@24: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 428: /* struct_variable_declaration_list: struct_variable_declaration_list $@24 structure_variable_declaration ';'  */
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

  case 429: /* $@25: %empty  */
                                                                                             {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 430: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@25 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
            }
    break;

  case 431: /* $@26: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 432: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@26 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 433: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 434: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 435: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 436: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 437: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 438: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 439: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 440: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 441: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 442: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 443: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 444: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 445: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 446: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 447: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 448: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 449: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 450: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 451: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 452: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 453: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 454: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 455: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 456: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 457: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 458: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 459: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 460: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 461: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 462: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 463: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 464: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration ';'  */
                                                                                 {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 465: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr ';'  */
                                                                                                                       {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 466: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr_pipe  */
                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 467: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 468: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 469: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 470: /* $@27: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 471: /* global_variable_declaration_list: global_variable_declaration_list $@27 optional_field_annotation let_variable_declaration  */
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

  case 472: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 473: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 474: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 475: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 476: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 477: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 478: /* $@28: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 479: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@28 optional_field_annotation let_variable_declaration  */
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

  case 480: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 481: /* enum_list: enum_list "name" ';'  */
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

  case 482: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 483: /* $@29: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 484: /* single_alias: "name" $@29 '=' type_declaration  */
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

  case 489: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 490: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 491: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 492: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 493: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 494: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 495: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 496: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 497: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 498: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 499: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 500: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 501: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 502: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 503: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 504: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 505: /* $@30: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 506: /* $@31: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 507: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@30 structure_name $@31 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
        }
    }
    break;

  case 508: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 509: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 510: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 511: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 512: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 513: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 514: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 515: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 516: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 517: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 518: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 519: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 520: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 521: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 522: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 523: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 524: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 525: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 526: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 527: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 528: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 529: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 530: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 531: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 532: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 533: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 534: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 535: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 536: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 537: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 538: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 539: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 540: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 541: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 542: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 543: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 544: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 545: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 546: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 547: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 548: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 549: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 550: /* $@32: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 551: /* $@33: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 552: /* bitfield_type_declaration: "bitfield" '<' $@32 bitfield_bits '>' $@33  */
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

  case 553: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 554: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 555: /* type_declaration: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 556: /* type_declaration: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 557: /* type_declaration: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 558: /* type_declaration: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 559: /* type_declaration: type_declaration '[' expr ']'  */
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

  case 560: /* type_declaration: type_declaration '[' ']'  */
                                           {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 561: /* type_declaration: type_declaration '-' '[' ']'  */
                                               {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 562: /* type_declaration: type_declaration "explicit"  */
                                                {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 563: /* type_declaration: type_declaration "const"  */
                                             {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 564: /* type_declaration: type_declaration '-' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 565: /* type_declaration: type_declaration '&'  */
                                       {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 566: /* type_declaration: type_declaration '-' '&'  */
                                           {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 567: /* type_declaration: type_declaration '#'  */
                                       {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 568: /* type_declaration: type_declaration "implicit"  */
                                                {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 569: /* type_declaration: type_declaration '-' '#'  */
                                           {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 570: /* type_declaration: type_declaration '=' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 571: /* type_declaration: type_declaration '?'  */
                                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 572: /* $@34: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 573: /* $@35: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 574: /* type_declaration: "smart_ptr" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 575: /* type_declaration: type_declaration "??"  */
                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 576: /* $@36: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 577: /* $@37: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 578: /* type_declaration: "array" '<' $@36 type_declaration '>' $@37  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 579: /* $@38: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 580: /* $@39: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 581: /* type_declaration: "table" '<' $@38 table_type_pair '>' $@39  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 582: /* $@40: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 583: /* $@41: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 584: /* type_declaration: "iterator" '<' $@40 type_declaration '>' $@41  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 585: /* type_declaration: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 586: /* $@42: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 587: /* $@43: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 588: /* type_declaration: "block" '<' $@42 type_declaration '>' $@43  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 589: /* $@44: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 590: /* $@45: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 591: /* type_declaration: "block" '<' $@44 optional_function_argument_list optional_function_type '>' $@45  */
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

  case 592: /* type_declaration: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 593: /* $@46: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 594: /* $@47: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 595: /* type_declaration: "function" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 596: /* $@48: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 597: /* $@49: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 598: /* type_declaration: "function" '<' $@48 optional_function_argument_list optional_function_type '>' $@49  */
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

  case 599: /* type_declaration: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 600: /* $@50: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 601: /* $@51: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 602: /* type_declaration: "lambda" '<' $@50 type_declaration '>' $@51  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 603: /* $@52: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 604: /* $@53: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 605: /* type_declaration: "lambda" '<' $@52 optional_function_argument_list optional_function_type '>' $@53  */
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

  case 606: /* $@54: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 607: /* $@55: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 608: /* type_declaration: "tuple" '<' $@54 tuple_type_list '>' $@55  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 609: /* $@56: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 610: /* $@57: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 611: /* type_declaration: "variant" '<' $@56 variant_type_list '>' $@57  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 612: /* $@58: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 613: /* $@59: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 614: /* variant_alias_declaration: "variant" $@58 "name" $@59 '{' variant_type_list ';' '}'  */
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

  case 615: /* $@60: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 616: /* bitfield_alias_declaration: "bitfield" $@60 "name" '{' bitfield_bits ';' '}'  */
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

  case 617: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 618: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 619: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 620: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 621: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 622: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 623: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 624: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 625: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 626: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 627: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 628: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 629: /* make_struct_decl: "[[" type_declaration make_struct_dim optional_block ']' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 630: /* make_struct_decl: "[[" type_declaration optional_block ']' ']'  */
                                                                        {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 631: /* make_struct_decl: "[[" type_declaration '(' ')' optional_block ']' ']'  */
                                                                                {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 632: /* make_struct_decl: "[[" type_declaration '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                     {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 633: /* make_struct_decl: "[{" type_declaration make_struct_dim optional_block '}' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 634: /* make_struct_decl: "[{" type_declaration '(' ')' make_struct_dim optional_block '}' ']'  */
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

  case 635: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 636: /* make_tuple: make_tuple ',' expr  */
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

  case 637: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 638: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 639: /* make_any_tuple: make_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 640: /* make_any_tuple: make_map_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 641: /* make_dim: make_any_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 642: /* make_dim: make_dim ';' make_any_tuple  */
                                              {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 643: /* make_dim_decl: "[[" type_declaration make_dim ']' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 644: /* make_dim_decl: "[{" type_declaration make_dim '}' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 645: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 646: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 647: /* make_table_decl: "{{" make_table '}' '}'  */
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

  case 648: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 649: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 650: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 651: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


