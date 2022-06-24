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
  YYSYMBOL_MTAG = 140,                     /* "$$"  */
  YYSYMBOL_BRABRAB = 141,                  /* "[["  */
  YYSYMBOL_BRACBRB = 142,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 143,                  /* "{{"  */
  YYSYMBOL_INTEGER = 144,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 145,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 146,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 147,    /* "unsigned long integer constant"  */
  YYSYMBOL_FLOAT = 148,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 149,                   /* "double constant"  */
  YYSYMBOL_NAME = 150,                     /* "name"  */
  YYSYMBOL_BEGIN_STRING = 151,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 152,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 153,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 154,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 155,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 156,          /* "}"  */
  YYSYMBOL_END_OF_READ = 157,              /* "end of failed eader macro"  */
  YYSYMBOL_158_ = 158,                     /* ','  */
  YYSYMBOL_159_ = 159,                     /* '='  */
  YYSYMBOL_160_ = 160,                     /* '?'  */
  YYSYMBOL_161_ = 161,                     /* ':'  */
  YYSYMBOL_162_ = 162,                     /* '|'  */
  YYSYMBOL_163_ = 163,                     /* '^'  */
  YYSYMBOL_164_ = 164,                     /* '&'  */
  YYSYMBOL_165_ = 165,                     /* '<'  */
  YYSYMBOL_166_ = 166,                     /* '>'  */
  YYSYMBOL_167_ = 167,                     /* '-'  */
  YYSYMBOL_168_ = 168,                     /* '+'  */
  YYSYMBOL_169_ = 169,                     /* '*'  */
  YYSYMBOL_170_ = 170,                     /* '/'  */
  YYSYMBOL_171_ = 171,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 172,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 173,               /* UNARY_PLUS  */
  YYSYMBOL_174_ = 174,                     /* '~'  */
  YYSYMBOL_175_ = 175,                     /* '!'  */
  YYSYMBOL_PRE_INC = 176,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 177,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 178,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 179,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 180,                    /* DEREF  */
  YYSYMBOL_181_ = 181,                     /* '.'  */
  YYSYMBOL_182_ = 182,                     /* '['  */
  YYSYMBOL_183_ = 183,                     /* ']'  */
  YYSYMBOL_184_ = 184,                     /* '('  */
  YYSYMBOL_185_ = 185,                     /* ')'  */
  YYSYMBOL_186_ = 186,                     /* '$'  */
  YYSYMBOL_187_ = 187,                     /* '@'  */
  YYSYMBOL_188_ = 188,                     /* ';'  */
  YYSYMBOL_189_ = 189,                     /* '{'  */
  YYSYMBOL_190_ = 190,                     /* '}'  */
  YYSYMBOL_191_ = 191,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 192,                 /* $accept  */
  YYSYMBOL_program = 193,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 194, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 195,              /* module_name  */
  YYSYMBOL_module_declaration = 196,       /* module_declaration  */
  YYSYMBOL_character_sequence = 197,       /* character_sequence  */
  YYSYMBOL_string_constant = 198,          /* string_constant  */
  YYSYMBOL_string_builder_body = 199,      /* string_builder_body  */
  YYSYMBOL_string_builder = 200,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 201, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 202,              /* expr_reader  */
  YYSYMBOL_203_1 = 203,                    /* $@1  */
  YYSYMBOL_options_declaration = 204,      /* options_declaration  */
  YYSYMBOL_require_declaration = 205,      /* require_declaration  */
  YYSYMBOL_require_module_name = 206,      /* require_module_name  */
  YYSYMBOL_require_module = 207,           /* require_module  */
  YYSYMBOL_is_public_module = 208,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 209,       /* expect_declaration  */
  YYSYMBOL_expect_list = 210,              /* expect_list  */
  YYSYMBOL_expect_error = 211,             /* expect_error  */
  YYSYMBOL_expression_label = 212,         /* expression_label  */
  YYSYMBOL_expression_goto = 213,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 214,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 215,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 216,          /* if_or_static_if  */
  YYSYMBOL_expression_if_then_else = 217,  /* expression_if_then_else  */
  YYSYMBOL_expression_for_loop = 218,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 219,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 220,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 221,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 222,    /* expression_with_alias  */
  YYSYMBOL_223_2 = 223,                    /* $@2  */
  YYSYMBOL_annotation_argument_value = 224, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 225, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 226, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 227,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 228, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 229, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration = 230,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 231,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 232, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 233, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 234,   /* optional_function_type  */
  YYSYMBOL_function_name = 235,            /* function_name  */
  YYSYMBOL_global_function_declaration = 236, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 237, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 238, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 239,     /* function_declaration  */
  YYSYMBOL_240_3 = 240,                    /* $@3  */
  YYSYMBOL_expression_block = 241,         /* expression_block  */
  YYSYMBOL_expression_any = 242,           /* expression_any  */
  YYSYMBOL_expressions = 243,              /* expressions  */
  YYSYMBOL_expr_pipe = 244,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 245,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 246,        /* expression_delete  */
  YYSYMBOL_expr_new = 247,                 /* expr_new  */
  YYSYMBOL_expression_break = 248,         /* expression_break  */
  YYSYMBOL_expression_continue = 249,      /* expression_continue  */
  YYSYMBOL_expression_return = 250,        /* expression_return  */
  YYSYMBOL_expression_yield = 251,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 252,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 253,                  /* kwd_let  */
  YYSYMBOL_expression_let = 254,           /* expression_let  */
  YYSYMBOL_expr_cast = 255,                /* expr_cast  */
  YYSYMBOL_256_4 = 256,                    /* $@4  */
  YYSYMBOL_257_5 = 257,                    /* $@5  */
  YYSYMBOL_258_6 = 258,                    /* $@6  */
  YYSYMBOL_259_7 = 259,                    /* $@7  */
  YYSYMBOL_260_8 = 260,                    /* $@8  */
  YYSYMBOL_261_9 = 261,                    /* $@9  */
  YYSYMBOL_expr_type_decl = 262,           /* expr_type_decl  */
  YYSYMBOL_263_10 = 263,                   /* $@10  */
  YYSYMBOL_264_11 = 264,                   /* $@11  */
  YYSYMBOL_expr_type_info = 265,           /* expr_type_info  */
  YYSYMBOL_expr_list = 266,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 267,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 268,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 269,            /* capture_entry  */
  YYSYMBOL_capture_list = 270,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 271,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 272,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 273,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 274,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 275,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 276,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 277,         /* expr_method_call  */
  YYSYMBOL_func_addr_expr = 278,           /* func_addr_expr  */
  YYSYMBOL_279_12 = 279,                   /* $@12  */
  YYSYMBOL_280_13 = 280,                   /* $@13  */
  YYSYMBOL_281_14 = 281,                   /* $@14  */
  YYSYMBOL_282_15 = 282,                   /* $@15  */
  YYSYMBOL_expr_field = 283,               /* expr_field  */
  YYSYMBOL_284_16 = 284,                   /* $@16  */
  YYSYMBOL_285_17 = 285,                   /* $@17  */
  YYSYMBOL_expr = 286,                     /* expr  */
  YYSYMBOL_287_18 = 287,                   /* $@18  */
  YYSYMBOL_288_19 = 288,                   /* $@19  */
  YYSYMBOL_289_20 = 289,                   /* $@20  */
  YYSYMBOL_290_21 = 290,                   /* $@21  */
  YYSYMBOL_291_22 = 291,                   /* $@22  */
  YYSYMBOL_292_23 = 292,                   /* $@23  */
  YYSYMBOL_optional_field_annotation = 293, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 294,        /* optional_override  */
  YYSYMBOL_optional_constant = 295,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 296, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 297, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 298, /* struct_variable_declaration_list  */
  YYSYMBOL_299_24 = 299,                   /* $@24  */
  YYSYMBOL_300_25 = 300,                   /* $@25  */
  YYSYMBOL_301_26 = 301,                   /* $@26  */
  YYSYMBOL_function_argument_declaration = 302, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 303,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 304,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 305,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 306,             /* variant_type  */
  YYSYMBOL_variant_type_list = 307,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 308,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 309,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 310,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 311,             /* optional_ref  */
  YYSYMBOL_let_variable_declaration = 312, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 313, /* global_variable_declaration_list  */
  YYSYMBOL_314_27 = 314,                   /* $@27  */
  YYSYMBOL_optional_shared = 315,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 316, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 317,               /* global_let  */
  YYSYMBOL_318_28 = 318,                   /* $@28  */
  YYSYMBOL_enum_list = 319,                /* enum_list  */
  YYSYMBOL_single_alias = 320,             /* single_alias  */
  YYSYMBOL_321_29 = 321,                   /* $@29  */
  YYSYMBOL_alias_list = 322,               /* alias_list  */
  YYSYMBOL_alias_declaration = 323,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 324, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 325,                /* enum_name  */
  YYSYMBOL_enum_declaration = 326,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 327, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 328,          /* optional_sealed  */
  YYSYMBOL_structure_name = 329,           /* structure_name  */
  YYSYMBOL_class_or_struct = 330,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 331, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 332,    /* structure_declaration  */
  YYSYMBOL_333_30 = 333,                   /* $@30  */
  YYSYMBOL_334_31 = 334,                   /* $@31  */
  YYSYMBOL_variable_name_with_pos_list = 335, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 336,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 337, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 338, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 339,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 340,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 341, /* bitfield_type_declaration  */
  YYSYMBOL_342_32 = 342,                   /* $@32  */
  YYSYMBOL_343_33 = 343,                   /* $@33  */
  YYSYMBOL_table_type_pair = 344,          /* table_type_pair  */
  YYSYMBOL_type_declaration = 345,         /* type_declaration  */
  YYSYMBOL_346_34 = 346,                   /* $@34  */
  YYSYMBOL_347_35 = 347,                   /* $@35  */
  YYSYMBOL_348_36 = 348,                   /* $@36  */
  YYSYMBOL_349_37 = 349,                   /* $@37  */
  YYSYMBOL_350_38 = 350,                   /* $@38  */
  YYSYMBOL_351_39 = 351,                   /* $@39  */
  YYSYMBOL_352_40 = 352,                   /* $@40  */
  YYSYMBOL_353_41 = 353,                   /* $@41  */
  YYSYMBOL_354_42 = 354,                   /* $@42  */
  YYSYMBOL_355_43 = 355,                   /* $@43  */
  YYSYMBOL_356_44 = 356,                   /* $@44  */
  YYSYMBOL_357_45 = 357,                   /* $@45  */
  YYSYMBOL_358_46 = 358,                   /* $@46  */
  YYSYMBOL_359_47 = 359,                   /* $@47  */
  YYSYMBOL_360_48 = 360,                   /* $@48  */
  YYSYMBOL_361_49 = 361,                   /* $@49  */
  YYSYMBOL_362_50 = 362,                   /* $@50  */
  YYSYMBOL_363_51 = 363,                   /* $@51  */
  YYSYMBOL_364_52 = 364,                   /* $@52  */
  YYSYMBOL_365_53 = 365,                   /* $@53  */
  YYSYMBOL_366_54 = 366,                   /* $@54  */
  YYSYMBOL_367_55 = 367,                   /* $@55  */
  YYSYMBOL_368_56 = 368,                   /* $@56  */
  YYSYMBOL_369_57 = 369,                   /* $@57  */
  YYSYMBOL_variant_alias_declaration = 370, /* variant_alias_declaration  */
  YYSYMBOL_371_58 = 371,                   /* $@58  */
  YYSYMBOL_372_59 = 372,                   /* $@59  */
  YYSYMBOL_bitfield_alias_declaration = 373, /* bitfield_alias_declaration  */
  YYSYMBOL_374_60 = 374,                   /* $@60  */
  YYSYMBOL_make_decl = 375,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 376,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 377,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 378,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 379,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 380,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 381,           /* make_map_tuple  */
  YYSYMBOL_make_any_tuple = 382,           /* make_any_tuple  */
  YYSYMBOL_make_dim = 383,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 384,            /* make_dim_decl  */
  YYSYMBOL_make_table = 385,               /* make_table  */
  YYSYMBOL_make_table_decl = 386,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 387, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 388       /* array_comprehension  */
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
#define YYLAST   8369

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  192
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  197
/* YYNRULES -- Number of rules.  */
#define YYNRULES  604
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1047

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   419


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
       2,     2,     2,   175,     2,   191,   186,   171,   164,     2,
     184,   185,   169,   168,   158,   167,   181,   170,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   161,   188,
     165,   159,   166,   160,   187,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   182,     2,   183,   163,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   189,   162,   190,   174,     2,     2,     2,
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
     155,   156,   157,   172,   173,   176,   177,   178,   179,   180
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   461,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   477,   478,   479,   483,   484,
     488,   506,   507,   508,   509,   513,   517,   522,   531,   539,
     555,   560,   568,   568,   598,   619,   623,   626,   630,   636,
     645,   648,   654,   655,   659,   663,   664,   668,   671,   677,
     683,   686,   692,   693,   697,   698,   699,   708,   709,   713,
     722,   728,   736,   746,   755,   755,   762,   763,   764,   765,
     766,   767,   771,   776,   784,   785,   786,   790,   791,   792,
     793,   794,   795,   796,   797,   803,   806,   812,   813,   814,
     818,   826,   839,   843,   850,   851,   855,   856,   857,   861,
     864,   871,   875,   876,   877,   878,   879,   880,   881,   882,
     883,   884,   885,   886,   887,   888,   889,   890,   891,   892,
     893,   894,   895,   896,   897,   898,   899,   900,   901,   902,
     903,   904,   905,   906,   907,   908,   909,   910,   911,   912,
     913,   914,   915,   916,   917,   918,   919,   920,   921,   922,
     923,   924,   925,   926,   927,   931,   953,   954,   955,   959,
     965,   965,   982,   986,   997,   998,   999,  1000,  1001,  1002,
    1003,  1004,  1005,  1006,  1007,  1008,  1009,  1010,  1011,  1012,
    1013,  1014,  1015,  1016,  1020,  1025,  1031,  1037,  1071,  1074,
    1080,  1081,  1092,  1096,  1102,  1105,  1108,  1112,  1118,  1122,
    1126,  1129,  1132,  1137,  1140,  1148,  1151,  1156,  1159,  1167,
    1173,  1174,  1178,  1184,  1184,  1184,  1187,  1187,  1187,  1192,
    1192,  1192,  1200,  1200,  1200,  1206,  1216,  1227,  1242,  1245,
    1251,  1252,  1259,  1270,  1271,  1272,  1276,  1277,  1278,  1279,
    1283,  1288,  1296,  1297,  1301,  1306,  1313,  1314,  1315,  1316,
    1317,  1318,  1322,  1323,  1324,  1325,  1326,  1327,  1328,  1329,
    1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,
    1340,  1344,  1345,  1346,  1347,  1351,  1362,  1367,  1377,  1381,
    1381,  1381,  1388,  1388,  1388,  1402,  1406,  1410,  1410,  1410,
    1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,  1425,  1426,
    1427,  1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1436,
    1437,  1438,  1439,  1440,  1441,  1442,  1443,  1444,  1445,  1446,
    1447,  1448,  1449,  1450,  1451,  1457,  1458,  1459,  1460,  1461,
    1462,  1463,  1464,  1465,  1466,  1467,  1468,  1469,  1473,  1477,
    1480,  1483,  1484,  1485,  1486,  1489,  1492,  1493,  1496,  1496,
    1496,  1499,  1504,  1508,  1512,  1512,  1512,  1517,  1520,  1524,
    1524,  1524,  1529,  1532,  1533,  1534,  1535,  1536,  1537,  1538,
    1539,  1540,  1541,  1542,  1547,  1548,  1552,  1553,  1557,  1558,
    1559,  1563,  1564,  1568,  1569,  1570,  1574,  1584,  1587,  1587,
    1606,  1605,  1620,  1619,  1632,  1641,  1646,  1654,  1655,  1659,
    1662,  1671,  1672,  1676,  1685,  1686,  1691,  1692,  1696,  1702,
    1708,  1711,  1715,  1721,  1730,  1731,  1732,  1736,  1737,  1741,
    1744,  1749,  1754,  1762,  1773,  1776,  1776,  1796,  1797,  1801,
    1802,  1803,  1807,  1810,  1810,  1829,  1832,  1841,  1854,  1854,
    1875,  1876,  1880,  1881,  1885,  1886,  1887,  1891,  1901,  1908,
    1918,  1919,  1923,  1924,  1928,  1934,  1935,  1939,  1940,  1941,
    1945,  1950,  1945,  1960,  1967,  1976,  1982,  1993,  1994,  1995,
    1996,  1997,  1998,  1999,  2000,  2001,  2002,  2003,  2004,  2005,
    2006,  2007,  2008,  2009,  2010,  2011,  2012,  2013,  2014,  2015,
    2016,  2017,  2021,  2022,  2023,  2024,  2025,  2026,  2030,  2041,
    2045,  2055,  2062,  2071,  2071,  2071,  2084,  2088,  2095,  2096,
    2097,  2098,  2099,  2113,  2119,  2123,  2127,  2132,  2137,  2142,
    2147,  2151,  2155,  2160,  2164,  2169,  2169,  2169,  2175,  2182,
    2182,  2182,  2187,  2187,  2187,  2193,  2193,  2193,  2198,  2202,
    2202,  2202,  2207,  2207,  2207,  2216,  2220,  2220,  2220,  2225,
    2225,  2225,  2234,  2238,  2238,  2238,  2243,  2243,  2243,  2252,
    2252,  2252,  2258,  2258,  2258,  2267,  2267,  2267,  2291,  2291,
    2311,  2312,  2313,  2314,  2318,  2325,  2332,  2338,  2347,  2352,
    2359,  2360,  2364,  2370,  2377,  2385,  2392,  2400,  2412,  2415,
    2429,  2435,  2441,  2442,  2446,  2451,  2458,  2463,  2473,  2478,
    2485,  2497,  2498,  2502,  2505
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
  "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"[[\"", "\"[{\"", "\"{{\"",
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
  "expr_named_call", "expr_method_call", "func_addr_expr", "$@12", "$@13",
  "$@14", "$@15", "expr_field", "$@16", "$@17", "expr", "$@18", "$@19",
  "$@20", "$@21", "$@22", "$@23", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@24", "$@25", "$@26", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_declaration", "global_variable_declaration_list", "$@27",
  "optional_shared", "optional_public_or_private_variable", "global_let",
  "$@28", "enum_list", "single_alias", "$@29", "alias_list",
  "alias_declaration", "optional_public_or_private_enum", "enum_name",
  "enum_declaration", "optional_structure_parent", "optional_sealed",
  "structure_name", "class_or_struct",
  "optional_public_or_private_structure", "structure_declaration", "$@30",
  "$@31", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_type_declaration",
  "$@32", "$@33", "table_type_pair", "type_declaration", "$@34", "$@35",
  "$@36", "$@37", "$@38", "$@39", "$@40", "$@41", "$@42", "$@43", "$@44",
  "$@45", "$@46", "$@47", "$@48", "$@49", "$@50", "$@51", "$@52", "$@53",
  "$@54", "$@55", "$@56", "$@57", "variant_alias_declaration", "$@58",
  "$@59", "bitfield_alias_declaration", "$@60", "make_decl",
  "make_struct_fields", "make_struct_dim", "optional_block",
  "make_struct_decl", "make_tuple", "make_map_tuple", "make_any_tuple",
  "make_dim", "make_dim_decl", "make_table", "make_table_decl",
  "array_comprehension_where", "array_comprehension", YY_NULLPTR
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
     405,   406,   407,   408,   409,   410,   411,   412,    44,    61,
      63,    58,   124,    94,    38,    60,    62,    45,    43,    42,
      47,    37,   413,   414,   126,    33,   415,   416,   417,   418,
     419,    46,    91,    93,    40,    41,    36,    64,    59,   123,
     125,    35
};
#endif

#define YYPACT_NINF (-893)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-557)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -893,    29,  -893,  -893,    21,   -85,   -44,   -89,  -893,   -75,
    -893,  -893,     3,  -893,  -893,  -893,  -893,  -893,   277,  -893,
      60,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,
     -92,  -893,    58,   -90,   118,  -893,  -893,   -44,    11,  -893,
    -893,   156,  -893,  -893,  -893,    60,   183,   185,  -893,  -893,
     200,   220,   190,  -893,   -26,  -893,  -893,  -893,    70,   160,
     207,  -893,   209,    10,    21,    -6,   -85,   186,   233,  -893,
     243,   251,  -893,   248,   221,   -97,   265,   229,  -893,  -893,
     274,    21,     3,  -893,  -893,  -893,  -893,  -893,  -893,  -893,
     278,  -893,  -893,  -893,  -893,  -893,   257,  -893,  -893,  -893,
    -893,  -893,   169,   101,  -893,  -893,  -893,  -893,   375,  -893,
    -893,  8132,  -893,  -893,   244,  -893,  -893,  -893,   307,   259,
    -893,    37,  -893,   -13,  -893,  -105,   361,  -893,   318,  -893,
    -893,   302,  -893,  -893,  -893,  -893,  -893,  -893,  -893,    73,
    -893,   299,   300,   305,   306,  -893,  -893,  -893,   279,  -893,
    -893,  -893,  -893,  -893,   308,  -893,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,  -893,   310,  -893,  -893,  -893,   311,   312,
    -893,  -893,  -893,  -893,   313,   315,  -893,  -893,  -893,  -893,
    -893,   344,  -893,  -893,   295,   335,  -893,  6834,   452,   453,
    -893,   304,   301,   377,  -893,  -893,   339,  -893,   303,    21,
     342,  -893,  -893,  -893,   101,  -893,  -893,  -893,  -893,  -893,
     348,  -893,   152,   162,   179,  -893,  -893,  -893,  -893,  -893,
    -893,   462,  -893,  -893,     6,  3022,  -893,   -31,   340,  -893,
     319,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,  -893,   317,  -893,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,  -893,  -893,  -893,  -893,   481,  -893,  -893,
    -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,   323,  -893,  -893,   -87,   351,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,  -893,   320,   -21,   352,   325,  -893,   318,
      96,   474,  -893,   204,  -893,  8132,  8132,  8132,  8132,   331,
     307,  8132,   304,  8132,   304,  8132,   304,  8219,   335,  -893,
    -893,  -893,   341,  -893,  -893,  -893,  8045,   334,   358,  -893,
     346,   360,   362,   349,   369,   353,  -893,   371,  4712,  4712,
     -20,  7724,  7811,  4712,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,  4712,  4712,  4712,   211,  4712,  4712,  -893,  4712,  -893,
     363,  -893,  -893,  -893,    12,  -893,  -893,  -893,  -893,   356,
    -893,  -893,  -893,  -893,  -893,  -893,  6225,   365,  -893,  -893,
    -893,  -893,  -893,  -893,  -893,  8132,   -14,  -893,  -893,  -893,
    -893,   -17,  -893,   241,  8132,  -893,  1316,  -893,   -25,  -893,
     211,  -893,  -893,   342,   364,   395,   401,  8132,  -893,   -52,
     543,   386,  1122,   593,  1256,  -893,  -119,  1541,   351,  1608,
     351,  1770,   351,    57,  -893,    81,   344,   149,  -893,  1932,
    -893,   211,  -893,  4712,  -893,  -893,  4712,  -893,  4712,  8132,
     239,   239,   370,  4712,   342,  1671,   342,  1833,  6904,  -893,
     -43,   262,   239,   239,   -30,  -893,   239,   239,   508,   100,
    -893,  2443,   412,  7842,  7929,  4712,  4712,  -893,  -893,  4712,
    4712,  4712,  4712,   405,  4712,   409,  4712,  4712,  4712,  4712,
    4712,  4712,  4712,  4712,  4712,  3161,  4712,  4712,  4712,  4712,
    4712,  4712,  4712,  4712,  4712,  4712,   -33,  4712,  -893,  3300,
     344,  -893,  -893,   342,  -893,   -67,  -893,   318,   344,  -893,
    4712,  -893,  -893,   342,  2157,  -893,   301,  4712,  4712,   410,
    -893,   373,   418,  3462,   182,  2304,   236,   236,  -893,   527,
     376,   378,  4712,  -893,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,   383,   384,   385,  -893,  -893,  -893,   342,  -893,   -79,
    -893,  6731,     0,  4712,  -893,  -893,    30,  -893,  -893,  -893,
     524,   733,  -893,  -893,  -893,  2605,  -893,  -893,  8132,  -893,
    -893,  -893,   428,  -893,   414,  -893,   415,  -893,   416,  8132,
    -893,  8219,  -893,   335,  3601,  3763,  8132,  5493,  8132,  8132,
    5573,  8132,  5656,  1994,  4712,  5736,    15,   236,    -4,  1995,
    3925,  6904,   425,    -2,   402,   426,  -893,  -893,   225,    25,
    4087,    -2,    65,  4712,  4712,   396,  -893,  4712,   298,   435,
    -893,   188,  -893,   438,  -893,   103,  7138,   -54,   304,   424,
    -893,  -893,   430,  -893,  -893,    59,    59,  7596,  7596,  7529,
    7529,   406,   537,  -893,  6303,   -38,   -38,    59,    59,  7392,
    7360,   866,  7208,  7958,  6986,  7462,  7494,  7564,  7596,  7596,
     423,   423,   537,   537,   537,   441,  4712,  -893,   446,  4712,
     596,  6381,  -893,   105,  -893,  -893,  -893,  8132,  -893,  2605,
    -893,   271,    27,  2605,  -893,  -893,   476,  4804,   586,  7138,
     271,   444,  -893,   445,   463,  7138,  -893,  2605,  -893,  4906,
     432,  -893,  -893,   421,  -893,  -893,   271,  -893,  -893,  -893,
    -893,   236,  -893,  4712,  4712,  4712,  4712,  4712,  4712,  4712,
    4712,  4712,  4712,  2744,  4712,  4712,  4712,  4712,  4712,  4712,
    2883,  -893,   981,     3,  -893,   604,   318,   470,   462,  -893,
    2605,  -893,  5008,  -893,  -893,   344,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,  -893,  -893,   344,  -893,  -893,  -893,  -893,
     117,   480,  5819,  2097,  -893,  2382,  2539,  -893,  2575,  -893,
     412,  5899,  -893,  4712,  -893,  4712,  4712,   -10,   482,   438,
     429,   448,  4712,   451,  4712,  4712,   438,   447,   455,  7138,
    -893,  -893,  7056,  -893,   461,  8132,   304,   -41,   125,  4712,
    -893,   489,   490,   494,   499,  -893,   127,   351,  -893,  -893,
    4249,  -893,   485,  -893,  -893,  4712,  -893,  6459,  -893,  6537,
    -893,  -893,  -893,   107,  -893,  6628,  -893,  4712,  -893,  5110,
    4712,  4712,  -893,   301,  -893,  -893,  -893,  -893,  -893,  5212,
    -893,  -893,  -893,   389,  -893,  7138,  7138,  7138,  7138,  7138,
    7138,  7138,  7138,  7138,  7138,  4712,  4712,  7138,  7138,  7138,
    7138,  7138,  7138,  7138,  4712,  4712,  7138,  -893,   128,   314,
       4,   466,  -893,  -893,  5314,  -893,  -893,  -893,  -893,  -893,
     153,  -893,  -893,  -893,  -893,  -893,   475,  -893,   -46,  7138,
    7138,    -2,   477,    97,   425,   483,  -893,  7138,  -893,  -893,
     -32,    -2,   484,  -893,  -893,  -893,  2822,   351,   478,  7138,
    -893,  -893,  -893,  -893,   -54,   486,   -86,  8132,  8132,  -893,
     119,  -893,  7290,  -893,  -893,  -893,   462,  4712,    45,  -893,
    -893,  4712,  -893,  1506,   301,  -893,  -893,  4712,  -893,  1103,
    7138,  1103,  7138,   492,  -893,  -893,     7,  -893,  -893,   314,
    -893,  -893,  4712,   514,  -893,  4712,  4712,  4712,  4411,  4712,
     500,   501,  4712,  4712,  -893,  4712,   495,  -893,  -893,   521,
    -893,  -893,  -893,  4573,  -893,  -893,  2961,  3100,  -893,  8132,
    7138,   301,  7138,  -893,  -893,   271,  -893,  -893,   659,   342,
    5982,   526,  7596,  7596,  7596,  -893,  6062,  5416,   517,  -893,
    7138,  7138,  5416,   522,   211,  -893,  4712,  7208,  -893,  -893,
    3239,  -893,   389,   -13,  -893,  -893,  -893,  -893,  4712,  -893,
     646,   523,  -893,   518,  -893,  -893,   211,  7290,  -893,  -893,
    -893,  -893,   516,   -13,  6145,  4712,   529,   530,  -893,  -893,
    -893,   301,  -893,  7138,  -893,  -893,  -893
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,    94,     1,   210,     0,     0,     0,     0,   211,     0,
     568,   565,     0,    14,     3,    10,     9,     8,     0,     7,
     427,     6,    11,     5,     4,    12,    13,    75,    76,    74,
      83,    85,    34,    47,    44,    45,    36,     0,    42,    35,
     438,     0,   443,    19,    18,   427,     0,     0,    88,    89,
       0,   190,    90,    92,     0,    87,   456,   455,   156,   444,
     457,   428,   429,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,   566,   192,
       0,     0,     0,    95,   158,   157,   160,   155,   446,   445,
       0,   459,   458,   460,   431,   430,   433,    81,    82,    79,
      80,    78,     0,     0,    77,    86,    48,    46,    42,    39,
      38,     0,   440,   442,     0,    16,    17,    20,     0,     0,
     191,     0,    93,     0,   447,     0,   452,   424,   376,    21,
      22,     0,    70,    71,    68,    69,    67,    66,    72,     0,
      41,     0,     0,     0,     0,   467,   487,   468,   499,   469,
     473,   474,   475,   476,   491,   480,   481,   482,   483,   484,
     485,   486,   488,   489,   538,   472,   479,   490,   545,   552,
     470,   477,   471,   478,     0,     0,   498,   508,   511,   509,
     510,   439,   441,   501,     0,     0,    91,     0,     0,     0,
     101,    96,     0,     0,   435,   453,     0,   461,   425,     0,
       0,    23,    24,    25,     0,    84,   529,   532,   535,   525,
       0,   503,   539,   546,   553,   559,   562,   516,   521,   515,
     528,     0,   524,   518,     0,     0,   520,     0,     0,   404,
       0,   152,   153,   150,   104,   105,   107,   106,   108,   109,
     110,   111,   137,   138,   135,   136,   128,   139,   140,   129,
     126,   127,   151,   148,     0,   149,   141,   142,   143,   144,
     115,   116,   117,   112,   113,   114,   125,     0,   131,   132,
     130,   123,   124,   119,   118,   120,   121,   122,   103,   102,
     147,     0,   133,   134,   376,    99,   184,   161,   492,   495,
     493,   496,   494,   497,     0,     0,   450,     0,   432,   376,
       0,   463,   434,   417,    73,     0,     0,     0,     0,     0,
       0,     0,    96,     0,    96,     0,    96,     0,     0,   523,
     517,   519,     0,   522,   296,   297,     0,     0,     0,   290,
       0,     0,     0,     0,     0,     0,   491,     0,     0,     0,
       0,     0,     0,     0,   246,   248,   247,   249,   250,   251,
      26,     0,     0,     0,     0,     0,     0,   513,     0,   233,
     234,   294,   293,   244,   291,   366,   365,   364,   363,    94,
     369,   292,   368,   367,   336,   298,     0,     0,   295,   570,
     571,   572,   573,   502,   569,     0,     0,   146,   154,   145,
      97,     0,   397,     0,     0,   159,     0,   435,     0,   448,
       0,   454,   387,     0,     0,     0,     0,     0,   418,     0,
       0,     0,   506,     0,     0,   500,     0,     0,    99,     0,
      99,     0,    99,   190,   401,     0,   399,     0,   514,   194,
     197,     0,   222,     0,   213,   216,     0,   219,     0,     0,
     325,   326,     0,     0,     0,   580,     0,     0,   591,   598,
       0,     0,   302,   301,   341,    32,   300,   299,     0,   235,
     372,     0,   242,     0,     0,     0,     0,   327,   328,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   287,     0,   512,     0,
     403,   567,   405,     0,   395,   408,    98,   376,   100,   186,
       0,    57,    58,     0,     0,   198,     0,     0,     0,     0,
     199,     0,     0,     0,     0,     0,     0,     0,   164,   162,
       0,     0,     0,   179,   174,   171,   170,   172,   173,   185,
     165,     0,     0,     0,   177,   178,   180,     0,   169,     0,
     166,   252,     0,     0,   436,   451,   388,   426,   377,   464,
     465,     0,   415,   416,   414,     0,   530,   533,     0,   536,
     526,   504,     0,   540,     0,   547,     0,   554,     0,     0,
     560,     0,   563,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   190,     0,
       0,   588,   578,   580,     0,   592,   593,   594,     0,     0,
       0,   580,     0,     0,     0,     0,    29,     0,    27,     0,
     329,   279,   278,     0,   337,     0,   228,     0,    96,     0,
     352,   351,     0,   353,   357,   303,   304,   316,   317,   314,
     315,     0,   346,   334,     0,   370,   371,   305,   306,   321,
     322,   323,   324,     0,     0,   319,   320,   318,   312,   313,
     308,   307,   309,   310,   311,     0,     0,   285,     0,     0,
       0,     0,   339,     0,   396,   407,   406,     0,   409,     0,
     398,     0,     0,     0,   200,   203,     0,   252,     0,   193,
       0,     0,   183,     0,     0,    51,    61,     0,   207,   252,
     234,   188,   189,     0,   181,   182,     0,   168,   175,   176,
     212,     0,   167,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   449,     0,     0,   462,     0,   376,     0,   414,   419,
       0,   423,   252,   531,   534,   507,   537,   527,   505,   541,
     543,   548,   550,   555,   557,   400,   561,   402,   564,   195,
       0,     0,     0,     0,   342,     0,     0,   343,     0,   373,
     242,     0,   374,     0,   581,     0,     0,   580,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   590,
     599,   600,     0,    30,    33,     0,    96,     0,     0,     0,
     338,     0,     0,     0,     0,   240,     0,    99,   348,   354,
       0,   332,     0,   358,   362,     0,   335,     0,   286,     0,
     288,   330,   340,   410,   413,   412,    62,     0,   204,   252,
       0,     0,   201,     0,    63,    64,    49,    50,   208,   252,
     205,   235,   184,    54,   187,   262,   263,   265,   264,   266,
     256,   257,   258,   267,   268,     0,     0,   254,   255,   269,
     270,   259,   260,   261,     0,     0,   253,   437,     0,   383,
     378,     0,   466,   421,   252,   422,   544,   551,   558,   196,
       0,   225,   223,   214,   217,   220,     0,   375,     0,   575,
     574,   580,     0,     0,   579,     0,   583,   589,   596,   595,
       0,   580,     0,   597,    28,    31,     0,    99,     0,   229,
     238,   239,   237,   236,     0,     0,     0,     0,     0,   276,
       0,   359,   347,   333,   331,   289,   406,     0,     0,   202,
     209,     0,   206,     0,     0,    52,    53,     0,    59,   273,
     274,   271,   272,    95,   384,   385,   378,   379,   380,   383,
     389,   420,     0,     0,   224,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   582,     0,     0,   586,   280,     0,
     275,   241,   243,     0,   230,   245,     0,     0,   277,     0,
     411,     0,    65,   163,    55,     0,   394,   390,   381,     0,
       0,     0,   215,   218,   221,   344,     0,   601,     0,   584,
     577,   576,   601,     0,     0,   283,     0,   231,   349,   355,
       0,    60,    54,     0,   382,   392,   386,   226,     0,   345,
       0,     0,   585,     0,   587,   281,     0,   232,   350,   356,
     360,    56,     0,     0,     0,     0,     0,     0,   284,   361,
     391,     0,   227,   602,   603,   604,   393
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -893,  -893,  -893,  -893,  -893,   263,   652,  -893,  -893,  -893,
    -893,  -893,  -893,  -893,   679,  -893,   609,  -893,  -893,   654,
    -893,  -893,  -893,  -290,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,  -893,   519,  -893,  -893,   660,   -53,  -893,   645,    -5,
    -349,  -301,  -410,  -893,  -893,  -893,  -892,  -893,  -893,  -185,
    -893,  -113,  -492,   -12,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,   729,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,  -893,  -476,  -893,  -893,  -182,  -893,   -37,
    -509,  -893,  -393,  -893,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,  -893,   343,  -893,  -893,  -893,  -893,  -893,
    -893,  -127,  -211,  -893,  -213,  -893,  -893,  -893,  -893,  -893,
     230,  -893,   157,  -893,  -370,   422,  -499,  -498,   180,  -893,
    -373,  -893,  -893,   697,  -893,  -893,  -893,   347,   168,  -893,
    -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,  -389,  -107,  -893,  -893,  -893,   433,  -893,
    -893,  -893,  -893,   -69,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,  -893,
    -893,  -893,  -893,   419,  -597,  -433,  -584,  -893,  -893,  -334,
     -36,   309,  -893,  -893,  -893,  -256,  -893
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   117,    45,    14,   131,   137,   451,   361,   794,
     362,   619,    15,    16,    38,    39,    72,    17,    34,    35,
     530,   531,   937,   938,   532,   533,   534,   535,   536,   537,
     538,   931,   138,   139,    30,    31,    32,    52,    53,    54,
      18,   285,   395,   191,    19,    86,   192,    87,   123,   363,
     539,   396,   540,   364,   541,   365,   542,   543,   544,   545,
     546,   547,   548,   366,   588,   955,   589,   956,   591,   957,
     367,   586,   954,   368,   625,   975,   369,   805,   806,   628,
     370,   371,   686,   550,   372,   373,   374,   795,  1004,   796,
    1026,   375,   670,   925,   626,   917,  1028,   918,  1029,   979,
    1039,   391,   949,  1015,   946,   871,   556,   736,  1013,  1033,
     392,   393,   424,   425,   229,   230,   776,   504,   565,   409,
     302,   198,   299,    62,    96,    21,   128,   295,    42,    73,
      75,    22,    90,   125,    23,   401,   196,   197,    60,    93,
      24,   126,   297,   303,   377,   294,   178,   179,   184,   180,
     310,   748,   411,   426,   308,   747,   305,   743,   306,   744,
     307,   746,   311,   749,   312,   876,   313,   751,   314,   877,
     315,   753,   316,   878,   317,   756,   318,   758,    25,    47,
     119,    26,    46,   378,   602,   603,   604,   379,   605,   606,
     607,   608,   380,   450,   381,  1021,   382
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   200,   505,   549,   177,   674,   679,   287,   574,   449,
     576,   418,   578,   420,   611,   422,   502,   701,   702,   780,
     462,   187,   685,   673,    97,    98,   798,   787,   121,     2,
     557,   503,    68,   698,   773,     3,    48,   -94,   320,    27,
      28,   597,   181,   711,   785,   973,   827,   571,   947,   597,
     675,   947,   987,    40,   199,   596,   193,   609,     4,    33,
       5,    40,     6,   801,    49,   562,    69,    63,     7,   572,
      55,    65,   802,   741,   563,    43,   675,     8,   473,   463,
     464,   475,   476,     9,   194,   775,   665,   666,   774,   475,
     476,   406,   676,   113,   677,   188,   189,   678,   390,   176,
      41,   948,    10,   286,   948,   803,    36,   564,   760,   712,
     804,    44,   799,   675,   505,   132,   133,   667,   676,   383,
      61,  1032,   775,    11,   682,    84,   799,    37,    80,   398,
     442,    85,    82,   301,   553,    50,   228,   190,   106,   217,
     797,  1041,   959,   496,   497,   614,   300,   615,   668,   669,
     398,   496,   497,    51,    99,   676,   965,    83,   100,   384,
     101,   102,   460,   554,   443,   218,   219,   467,   468,   399,
     321,    29,   403,   406,   710,   473,   501,   474,   475,   476,
     477,    70,   894,   406,   478,   406,   779,   824,   322,    80,
     731,   828,    71,   892,   103,    64,   461,   323,   177,   177,
     177,   177,   844,   799,   177,   838,   177,   735,   177,    74,
     177,    12,   733,   502,   675,    88,    64,    13,   579,   177,
     734,    89,   186,   962,   675,   220,   491,   492,   493,   494,
     495,   204,    50,   981,   177,   177,   410,   412,   413,   414,
     496,   497,   417,   114,   419,   134,   421,   580,   873,   135,
      51,   136,   102,   784,    64,   788,   676,   429,   205,   463,
     464,   799,    91,   799,    94,   621,   926,   222,    92,   581,
      95,   223,   445,   447,   224,   799,    66,   799,   177,   404,
     790,    56,    57,   778,    58,   914,    82,   177,   800,   225,
     822,   463,   464,   176,   176,   176,   176,   888,   226,   176,
     177,   176,   879,   176,   978,   176,    40,   960,   908,   900,
     915,   943,    59,  -542,   176,   582,   500,   966,  -542,   952,
     115,   129,   130,  -549,   927,   508,   116,   807,  -549,   176,
     176,   688,   177,    77,   920,    78,  -542,   583,   561,   696,
    -556,   953,   455,    50,   891,  -556,  -549,   467,   468,  -282,
      79,   928,    80,   901,  -282,   473,   631,   634,   475,   476,
     477,    51,   406,  -556,   478,   407,   438,    71,   408,   944,
     593,   286,  -282,   176,    81,   945,   217,   465,   466,   467,
     468,   469,   176,   108,   470,   471,   472,   473,   555,   474,
     475,   476,   477,   109,   963,   176,   478,   916,   479,   480,
     934,   110,   218,   219,   481,   482,   483,   111,   783,   112,
     484,   935,   936,   784,   129,   130,   616,   617,   118,   585,
     496,   497,   359,   700,   120,   286,   506,   176,   124,   507,
      69,   485,   182,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   463,   464,   288,   127,   622,   185,   289,
     201,   202,   496,   497,   201,   202,   203,   183,   195,   199,
     286,   177,   220,   210,   206,   207,   290,   291,   292,   293,
     208,   209,   177,   211,   177,   212,   213,   214,   215,   177,
     216,   177,   177,   227,   177,   228,   282,   283,   284,   296,
     286,  1016,   301,   298,   319,   907,   826,   969,   309,   745,
     387,   385,   388,   221,   222,   834,   389,   386,   223,   397,
     755,   224,   394,   400,   402,   405,   415,   763,   431,   765,
     766,   843,   768,   432,   428,   434,   225,   435,   463,   464,
     433,   467,   468,   436,   437,   226,   439,   438,    12,   473,
     549,   474,   475,   476,   477,   559,   814,   558,   478,   499,
     459,   560,   567,   627,   594,   641,   176,   463,   464,   643,
     691,   692,   693,   703,   704,   737,   705,   176,   376,   176,
     177,   707,   708,   709,   176,   217,   176,   176,   383,   176,
     750,   752,   754,   778,   782,   781,   791,   793,   797,   808,
     810,   816,   493,   494,   495,   809,   818,   820,   711,   833,
     505,   218,   219,   835,   496,   497,   836,   837,   823,   870,
     842,   869,   895,   905,   465,   466,   467,   468,   469,   841,
     872,   470,   471,   472,   473,   217,   474,   475,   476,   477,
     880,   896,   893,   478,   898,   479,   480,   902,   903,   910,
     911,   481,   482,   483,   912,   467,   468,   484,   930,   913,
     921,   218,   219,   473,   950,   474,   475,   476,   477,   958,
     961,   220,   478,   970,   991,   176,   964,   967,   485,   972,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     986,   440,   441,   998,   999,  1003,   448,  1005,   177,   496,
     497,  1014,  1018,   620,   452,   453,   454,  1035,   456,   457,
    1022,   458,   221,   222,  1040,  1024,  1036,   223,  1037,   566,
     224,   220,  1044,  1045,   618,   104,    67,   140,   496,   497,
     107,    55,  1031,   304,   105,   225,   906,   122,   868,   933,
      20,   974,   971,   886,   226,   988,   989,   680,   757,   551,
     427,   740,    76,   416,   552,   430,  1023,     0,   899,   984,
       0,     0,   221,   222,     0,     0,   612,   223,     0,   569,
     224,     0,     0,     0,     0,   217,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   225,   587,     0,     0,   590,
       0,   592,     0,   176,   226,     0,   595,     0,   601,     0,
     601,   218,   219,     0,     0,     0,  1011,     0,     0,     0,
    1012,     0,     0,     0,     0,     0,     0,     0,   635,   636,
     177,   177,   637,   638,   639,   640,     0,   642,     0,   644,
     645,   646,   647,   648,   649,   650,   651,   652,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,   664,     0,
     671,     0,     0,     0,     0,     0,     0,     0,   976,   977,
     562,   220,     0,   681,     0,     0,  1046,   687,     0,   563,
     689,   690,     0,     0,     0,     0,   695,     0,   699,     0,
       0,     0,   177,     0,     0,   706,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   463,   464,     0,     0,
       0,     0,   738,   222,     0,     0,   732,   223,     0,     0,
     224,     0,     0,     0,     0,   176,   176,     0,   742,     0,
    1010,     0,     0,     0,     0,   225,     0,     0,     0,     0,
       0,   739,     0,     0,   226,     0,     0,     0,   762,     0,
       0,     0,     0,     0,     0,     0,     0,   771,     0,     0,
       0,     0,   452,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,     0,     0,   789,   448,     0,     0,
     792,     0,     0,     0,     0,     0,     0,   176,     0,     0,
       0,     0,   465,   466,   467,   468,   469,     0,     0,   470,
     471,   472,   473,     0,   474,   475,   476,   477,     0,     0,
       0,   478,  1025,   479,   480,     0,     0,     0,     0,   481,
       0,   463,   464,     0,     0,     0,     0,     0,     0,   817,
       0,     0,   819,     0,  1038,     0,     0,     0,     0,     0,
       0,     0,   825,     0,     0,     0,   829,     0,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,     0,     0,
     839,     0,     0,     0,     0,     0,     0,   496,   497,     0,
       0,     0,     0,     0,     0,     0,   845,   846,   847,   848,
     849,   850,   851,   852,   853,   854,   857,   858,   859,   860,
     861,   862,   863,   866,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   874,     0,     0,     0,   465,   466,   467,
     468,   469,     0,     0,   470,   471,   472,   473,     0,   474,
     475,   476,   477,     0,     0,     0,   478,     0,   479,   480,
       0,     0,     0,     0,   481,   482,   483,     0,   889,   890,
     484,     0,     0,   463,   464,   897,     0,   601,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   485,   909,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,     0,   217,     0,     0,     0,   922,     0,
       0,     0,   496,   497,     0,     0,     0,     0,     0,   867,
       0,     0,     0,   857,   866,     0,     0,     0,     0,     0,
     218,   219,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   939,   940,
       0,     0,     0,     0,     0,     0,     0,   941,   942,   465,
     466,     0,     0,   469,     0,     0,   470,   471,   472,   473,
       0,   474,   475,   476,   477,     0,     0,     0,   478,     0,
     479,   480,     0,     0,     0,     0,   481,   482,   483,     0,
     220,     0,   484,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   485,     0,   486,   487,   488,   489,   490,
     980,     0,     0,   494,   982,     0,   551,     0,     0,     0,
     985,   221,   222,     0,   496,   497,   223,     0,   217,   224,
       0,     0,     0,     0,     0,   990,     0,     0,   992,   993,
     994,   996,   997,     0,   225,  1000,  1001,     0,  1002,     0,
     568,     0,     0,   226,   218,   219,  1007,   509,     0,     0,
       0,     0,     3,     0,   510,   511,   512,     0,   513,     0,
     324,   325,   326,   327,   328,     0,     0,     0,     0,     0,
       0,   514,   329,   515,   516,     0,     0,     0,     0,  1027,
       0,     0,     0,   517,   330,     0,   518,     0,   519,   331,
       0,  1034,   332,     0,     8,   333,   520,     0,   521,   334,
       0,     0,   522,   523,   220,     0,     0,     0,  1043,   524,
     145,   146,   147,     0,   149,   150,   151,   152,   153,   336,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
       0,   337,   525,     0,     0,   221,   222,     0,     0,     0,
     223,     0,   570,   224,   338,   339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   225,   526,
     527,     0,     0,     0,     0,     0,     0,   226,    50,     0,
       0,     0,     0,     0,     0,     0,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,    51,   350,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,   352,   353,     0,   354,     0,     0,
     355,   356,     0,     0,     0,     0,     0,     0,     0,     0,
     358,     0,   359,   360,   528,   286,   529,   509,     0,     0,
       0,     0,     3,     0,   510,   511,   512,     0,   513,     0,
     324,   325,   326,   327,   328,     0,     0,     0,     0,     0,
       0,   514,   329,   515,   516,     0,     0,     0,     0,     0,
       0,     0,     0,   517,   330,     0,   518,     0,   519,   331,
       0,     0,   332,     0,     8,   333,   520,     0,   521,   334,
       0,     0,   522,   523,     0,     0,     0,     0,     0,   524,
     145,   146,   147,   217,   149,   150,   151,   152,   153,   336,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,   218,
     219,   337,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   338,   339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   526,
     527,     0,     0,     0,     0,     0,     0,     0,    50,     0,
     217,     0,     0,     0,     0,     0,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,    51,   350,     0,   220,
       0,     0,     0,     0,     0,     0,   218,   219,     0,     0,
       0,     0,     0,   351,   352,   353,     0,   354,     0,     0,
     355,   356,     0,     0,     0,   324,   325,   326,   327,   328,
     358,     0,   359,   360,   528,   286,   983,   329,     0,     0,
     221,   222,     0,   217,     0,   223,     0,   573,   224,   330,
       0,     0,     0,     0,   331,     0,     0,   332,     0,     0,
     333,     0,   597,   225,   334,     0,   220,     0,     0,   218,
     219,     0,   226,     0,   335,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   336,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   337,   221,   222,     0,
       0,     0,   223,     0,   575,   224,     0,     0,     0,   338,
     339,     0,     0,     0,     0,     0,     0,     0,     0,   220,
     225,     0,     0,     0,     0,     0,     0,     0,     0,   226,
       0,     0,   217,    50,     0,     0,     0,     0,     0,     0,
       0,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   598,   350,     0,     0,     0,     0,     0,   218,   219,
     221,   222,     0,     0,     0,   223,     0,     0,   599,   352,
     353,     0,   354,     0,     0,   355,   356,   324,   325,   326,
     327,   328,     0,   225,     0,   600,     0,   359,   360,   329,
     286,     0,   226,     0,     0,   217,     0,     0,     0,     0,
       0,   330,     0,     0,     0,     0,   331,     0,     0,   332,
       0,     0,   333,     0,     0,     0,   334,     0,   220,     0,
       0,   218,   219,     0,     0,     0,   335,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   336,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   337,   221,
     222,     0,     0,     0,   223,     0,   577,   224,     0,     0,
       0,   338,   339,     0,     0,     0,     0,     0,     0,     0,
       0,   220,   225,     0,     0,     0,     0,     0,     0,     0,
       0,   226,     0,     0,   217,    50,     0,     0,     0,     0,
       0,     0,     0,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   598,   350,     0,     0,     0,     0,     0,
     218,   219,   221,   222,     0,     0,     0,   223,     0,     0,
     599,   352,   353,     0,   354,     0,     0,   355,   356,   324,
     325,   326,   327,   328,     0,   225,     0,   610,     0,   359,
     360,   329,   286,     0,   226,     0,   217,   320,     0,     0,
       0,     0,     0,   330,     0,     0,     0,     0,   331,     0,
       0,   332,     0,     0,   333,     0,     0,     0,   334,     0,
     220,     0,   218,   219,     0,     0,     0,     0,   335,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   336,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     337,   221,   222,     0,     0,     0,   223,     0,     0,   224,
       0,     0,     0,   338,   339,     0,     0,     0,     0,     0,
       0,     0,   220,     0,   225,     0,   584,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,    50,     0,   217,
       0,     0,     0,     0,     0,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,    51,   350,     0,     0,     0,
       0,     0,     0,   221,   222,   218,   219,     0,   223,   321,
     770,   224,   351,   352,   353,     0,   354,     0,     0,   355,
     356,   324,   325,   326,   327,   328,   225,   322,     0,   358,
       0,   359,   360,   329,   286,   226,   323,     0,     0,     0,
       0,     0,     0,     0,     0,   330,     0,     0,     0,     0,
     331,     0,     0,   332,     0,     0,   333,     0,     0,     0,
     334,     0,     0,     0,     0,   220,     0,     0,     0,     0,
     335,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     336,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   337,     0,     0,     0,   221,   222,     0,     0,
       0,   223,     0,   882,   224,   338,   339,     0,     0,     0,
       0,     0,     0,     0,   683,     0,     0,     0,     0,   225,
     526,   527,     0,     0,     0,     0,     0,     0,   226,    50,
       0,     0,     0,     0,     0,     0,     0,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,    51,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   325,
     326,   327,   328,     0,   351,   352,   353,     0,   354,     0,
     329,   355,   356,     0,     0,     0,     0,     0,     0,     0,
       0,   358,   330,   359,   360,   684,   286,   331,     0,     0,
     332,     0,     0,   333,     0,     0,     0,   334,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   335,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   336,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,   337,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   338,   339,   217,     0,     0,     0,     0,     0,
       0,   697,     0,     0,     0,     0,     0,   526,   527,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
     218,   219,     0,     0,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,    51,   350,     0,   324,   325,   326,
     327,   328,     0,     0,     0,     0,     0,     0,     0,   329,
       0,   351,   352,   353,     0,   354,     0,     0,   355,   356,
       0,   330,     0,     0,     0,     0,   331,     0,   358,   332,
     359,   360,   333,   286,     0,     0,   334,     0,     0,     0,
     220,     0,     0,     0,     0,     0,   335,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   336,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   337,     0,
       0,   221,   222,     0,     0,     0,   223,     0,   883,   224,
       0,   338,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   225,     0,     0,     0,     0,     0,
       0,   217,     0,   226,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,    51,   350,     0,     0,   218,   219,     0,
       0,     0,     0,     0,     0,     0,     0,   217,     0,     0,
     351,   352,   353,     0,   354,     0,     0,   355,   356,   324,
     325,   326,   327,   328,     0,   623,     0,   358,   624,   359,
     360,   329,   286,   218,   219,     0,     0,     0,     0,     0,
       0,     0,     0,   330,     0,     0,     0,     0,   331,     0,
       0,   332,     0,     0,   333,     0,     0,   220,   334,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   335,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   336,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,   220,   170,   171,   172,   173,   221,   222,
     337,     0,     0,   223,     0,   884,   224,     0,     0,     0,
       0,     0,     0,   338,   339,     0,     0,     0,     0,     0,
       0,   225,     0,     0,     0,     0,     0,     0,   526,   527,
     226,     0,     0,     0,   221,   222,     0,    50,     0,   223,
       0,   885,   224,     0,     0,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,    51,   350,   225,   324,   325,
     326,   327,   328,     0,     0,     0,   226,     0,     0,     0,
     329,     0,   351,   352,   353,     0,   354,     0,     0,   355,
     356,     0,   330,     0,     0,     0,     0,   331,     0,   358,
     332,   359,   360,   333,   286,     0,     0,   334,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   335,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   336,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,   337,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   338,   339,   217,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   855,   856,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
     218,   219,     0,     0,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,    51,   350,     0,   324,   325,   326,
     327,   328,     0,     0,     0,     0,     0,     0,     0,   329,
       0,   351,   352,   353,     0,   354,     0,     0,   355,   356,
       0,   330,     0,     0,     0,     0,   331,     0,   358,   332,
     359,   360,   333,   286,     0,     0,   334,     0,     0,     0,
     220,     0,     0,     0,     0,     0,   335,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   336,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   337,     0,
       0,   221,   222,     0,     0,     0,   223,     0,   968,   224,
       0,   338,   339,   217,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   225,     0,   864,   865,     0,     0,
       0,     0,     0,   226,     0,    50,     0,     0,     0,   218,
     219,     0,     0,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,    51,   350,     0,   324,   325,   326,   327,
     328,     0,     0,     0,     0,     0,     0,     0,   329,     0,
     351,   352,   353,     0,   354,     0,     0,   355,   356,     0,
     330,     0,     0,     0,     0,   331,     0,   358,   332,   359,
     360,   333,   286,     0,     0,   334,     0,     0,     0,   220,
       0,     0,     0,     0,     0,   335,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   336,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   337,     0,     0,
     221,   222,     0,     0,     0,   223,     0,  1008,   224,     0,
     338,   339,   217,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,     0,     0,     0,     0,     0,     0,
       0,     0,   226,     0,    50,     0,     0,     0,   218,   219,
       0,     0,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,    51,   350,     0,   324,   325,   326,   327,   328,
       0,     0,   653,     0,     0,     0,     0,   329,     0,   351,
     352,   353,     0,   354,     0,     0,   355,   356,     0,   330,
       0,     0,     0,     0,   331,   357,   358,   332,   359,   360,
     333,   286,     0,     0,   334,     0,     0,     0,   220,     0,
       0,     0,     0,     0,   335,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   336,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   337,     0,     0,   221,
     222,     0,     0,     0,   223,     0,  1009,   224,     0,   338,
     339,   217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,   226,     0,    50,     0,     0,     0,   218,   219,     0,
       0,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,    51,   350,     0,   324,   325,   326,   327,   328,     0,
       0,     0,     0,     0,     0,     0,   329,     0,   351,   352,
     353,     0,   354,     0,     0,   355,   356,     0,   330,     0,
       0,     0,     0,   331,     0,   358,   332,   359,   360,   333,
     286,     0,     0,   334,     0,     0,     0,   220,     0,     0,
       0,     0,     0,   335,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   336,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   337,     0,     0,   221,   222,
       0,     0,     0,   223,     0,  1030,   224,     0,   338,   339,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   225,     0,     0,     0,     0,     0,     0,     0,     0,
     226,     0,    50,     0,     0,     0,     0,     0,     0,     0,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
      51,   350,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,   352,   353,
       0,   354,     0,     0,   355,   356,   324,   325,   326,   327,
     328,     0,     0,     0,   358,   672,   359,   360,   329,   286,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     330,     0,     0,     0,     0,   331,     0,     0,   332,     0,
       0,   333,     0,     0,     0,   334,     0,     0,   694,     0,
       0,     0,     0,     0,     0,   335,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   336,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   337,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     338,   339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,    51,   350,     0,   324,   325,   326,   327,   328,
       0,     0,     0,     0,     0,     0,     0,   329,     0,   351,
     352,   353,     0,   354,     0,     0,   355,   356,     0,   330,
       0,     0,     0,     0,   331,     0,   358,   332,   359,   360,
     333,   286,     0,     0,   334,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   335,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   336,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   337,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   338,
     339,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,    51,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
     353,     0,   354,     0,     0,   355,   356,   324,   325,   326,
     327,   328,     0,     0,     0,   358,   759,   359,   360,   329,
     286,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   330,     0,     0,     0,     0,   331,     0,     0,   332,
       0,     0,   333,     0,     0,     0,   334,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   335,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   336,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   337,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   338,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,    51,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   761,     0,
     351,   352,   353,     0,   354,     0,     0,   355,   356,   324,
     325,   326,   327,   328,     0,     0,     0,   358,     0,   359,
     360,   329,   286,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   330,     0,     0,     0,     0,   331,     0,
       0,   332,     0,     0,   333,     0,     0,     0,   334,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   335,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   336,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     337,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   338,   339,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,    51,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,   353,     0,   354,     0,     0,   355,
     356,   324,   325,   326,   327,   328,     0,     0,     0,   358,
     777,   359,   360,   329,   286,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   330,     0,     0,     0,     0,
     331,     0,     0,   332,     0,     0,   333,     0,     0,     0,
     334,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     335,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     336,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   337,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   338,   339,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,    51,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,   353,     0,   354,     0,
       0,   355,   356,   324,   325,   326,   327,   328,     0,     0,
       0,   358,   786,   359,   360,   329,   286,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   330,     0,     0,
       0,     0,   331,     0,     0,   332,     0,     0,   333,     0,
       0,     0,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   335,   145,   146,   147,     0,   149,   150,   151,
     152,   153,   336,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   165,   166,   167,     0,     0,   170,   171,
     172,   173,     0,     0,   337,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   338,   339,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,    51,
     350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,   353,     0,
     354,     0,     0,   355,   356,   324,   325,   326,   327,   328,
       0,     0,     0,   358,   919,   359,   360,   329,   286,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   330,
       0,     0,     0,     0,   331,     0,     0,   332,     0,     0,
     333,     0,     0,     0,   334,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   335,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   336,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   337,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   338,
     339,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,    51,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
     353,     0,   354,     0,     0,   355,   356,   324,   325,   326,
     327,   328,     0,     0,     0,   358,   995,   359,   360,   329,
     286,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   330,     0,     0,     0,     0,   331,     0,     0,   332,
       0,     0,   333,     0,     0,     0,   334,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   335,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   336,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   337,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   338,   339,     0,     0,     0,     0,     0,     0,     0,
    1006,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,    51,   350,     0,   324,   325,   326,   327,
     328,     0,     0,     0,     0,     0,     0,     0,   329,     0,
     351,   352,   353,     0,   354,     0,     0,   355,   356,     0,
     330,     0,     0,     0,     0,   331,     0,   358,   332,   359,
     360,   333,   286,     0,     0,   334,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   335,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   336,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   337,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     338,   339,     0,     0,   463,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,    51,   350,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
     352,   353,     0,   354,     0,     0,   355,   356,     0,     0,
       0,     0,     0,     0,     0,     0,   358,     0,   359,   360,
       0,   286,   713,   714,   715,   716,   717,   718,   719,   720,
     465,   466,   467,   468,   469,   721,   722,   470,   471,   472,
     473,   830,   474,   475,   476,   477,   463,   464,     0,   478,
     724,   479,   480,   725,   726,     0,     0,   481,   482,   483,
     727,   728,   729,   484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   831,   485,     0,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,   497,     0,     0,     0,
       0,     0,   832,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   713,   714,   715,   716,   717,   718,
     719,   720,   465,   466,   467,   468,   469,   721,   722,   470,
     471,   472,   473,   830,   474,   475,   476,   477,   463,   464,
       0,   478,   724,   479,   480,   725,   726,     0,     0,   481,
     482,   483,   727,   728,   729,   484,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   831,   485,     0,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,     0,
       0,     0,     0,     0,   840,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   713,   714,   715,   716,
     717,   718,   719,   720,   465,   466,   467,   468,   469,   721,
     722,   470,   471,   472,   473,   830,   474,   475,   476,   477,
     463,   464,     0,   478,   724,   479,   480,   725,   726,     0,
       0,   481,   482,   483,   727,   728,   729,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   831,   485,     0,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,     0,     0,     0,     0,     0,   875,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   713,   714,
     715,   716,   717,   718,   719,   720,   465,   466,   467,   468,
     469,   721,   722,   470,   471,   472,   473,   830,   474,   475,
     476,   477,   463,   464,     0,   478,   724,   479,   480,   725,
     726,     0,     0,   481,   482,   483,   727,   728,   729,   484,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   831,
     485,     0,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   496,   497,     0,     0,     0,     0,     0,   929,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     713,   714,   715,   716,   717,   718,   719,   720,   465,   466,
     467,   468,   469,   721,   722,   470,   471,   472,   473,   830,
     474,   475,   476,   477,   463,   464,     0,   478,   724,   479,
     480,   725,   726,     0,     0,   481,   482,   483,   727,   728,
     729,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   831,   485,     0,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   497,     0,     0,     0,     0,     0,
     932,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   713,   714,   715,   716,   717,   718,   719,   720,
     465,   466,   467,   468,   469,   721,   722,   470,   471,   472,
     473,   830,   474,   475,   476,   477,   463,   464,     0,   478,
     724,   479,   480,   725,   726,     0,     0,   481,   482,   483,
     727,   728,   729,   484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   831,   485,     0,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,   497,     0,     0,     0,
       0,     0,   951,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   463,   464,     0,     0,     0,     0,     0,
       0,     0,   465,   466,   467,   468,   469,     0,     0,   470,
     471,   472,   473,     0,   474,   475,   476,   477,     0,     0,
       0,   478,     0,   479,   480,     0,     0,     0,     0,   481,
     482,   483,     0,     0,     0,   484,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   485,     0,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,     0,     0,
       0,     0,     0,   463,   464,     0,     0,   496,   497,   465,
     466,   467,   468,   469,  1020,     0,   470,   471,   472,   473,
       0,   474,   475,   476,   477,     0,     0,     0,   478,     0,
     479,   480,     0,     0,     0,     0,   481,   482,   483,     0,
       0,     0,   484,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   485,     0,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   463,   464,   764,   465,
     466,   467,   468,   469,     0,     0,   470,   471,   472,   473,
       0,   474,   475,   476,   477,     0,     0,     0,   478,     0,
     479,   480,     0,     0,     0,     0,   481,   482,   483,     0,
       0,     0,   484,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   485,     0,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   463,   464,   767,     0,
       0,     0,   465,   466,   467,   468,   469,     0,     0,   470,
     471,   472,   473,     0,   474,   475,   476,   477,     0,     0,
       0,   478,     0,   479,   480,     0,     0,     0,     0,   481,
     482,   483,     0,     0,     0,   484,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   485,     0,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,   463,
     464,   769,   465,   466,   467,   468,   469,     0,     0,   470,
     471,   472,   473,     0,   474,   475,   476,   477,     0,     0,
       0,   478,     0,   479,   480,     0,     0,     0,     0,   481,
     482,   483,     0,     0,     0,   484,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   485,     0,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,   463,
     464,   772,     0,     0,     0,   465,   466,   467,   468,   469,
       0,     0,   470,   471,   472,   473,     0,   474,   475,   476,
     477,     0,     0,     0,   478,     0,   479,   480,     0,     0,
       0,     0,   481,   482,   483,     0,     0,     0,   484,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   485,
       0,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   463,   464,   881,   465,   466,   467,   468,   469,
       0,     0,   470,   471,   472,   473,     0,   474,   475,   476,
     477,     0,     0,     0,   478,     0,   479,   480,     0,     0,
       0,     0,   481,   482,   483,     0,     0,     0,   484,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   485,
       0,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   463,   464,   887,     0,     0,     0,   465,   466,
     467,   468,   469,     0,     0,   470,   471,   472,   473,     0,
     474,   475,   476,   477,     0,     0,     0,   478,     0,   479,
     480,     0,     0,     0,     0,   481,   482,   483,     0,     0,
       0,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   485,     0,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   463,   464,  1017,   465,   466,
     467,   468,   469,     0,     0,   470,   471,   472,   473,     0,
     474,   475,   476,   477,     0,     0,     0,   478,     0,   479,
     480,     0,     0,     0,     0,   481,   482,   483,     0,     0,
       0,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   485,     0,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   463,   464,  1019,     0,     0,
       0,   465,   466,   467,   468,   469,     0,     0,   470,   471,
     472,   473,     0,   474,   475,   476,   477,     0,     0,     0,
     478,     0,   479,   480,     0,     0,     0,     0,   481,   482,
     483,     0,     0,     0,   484,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   485,     0,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,     0,     0,     0,
       0,     0,     0,   463,   464,     0,   496,   497,     0,     0,
    1042,   465,   466,   467,   468,   469,     0,     0,   470,   471,
     472,   473,     0,   474,   475,   476,   477,     0,     0,     0,
     478,     0,   479,   480,     0,     0,     0,     0,   481,   482,
     483,     0,     0,     0,   484,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   485,     0,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,     0,     0,     0,
       0,   463,   464,     0,     0,     0,   496,   497,   498,   465,
     466,   467,   468,   469,     0,     0,   470,   471,   472,   473,
       0,   474,   475,   476,   477,     0,     0,     0,   478,     0,
     479,   480,     0,     0,     0,     0,   481,   482,   483,     0,
       0,     0,   484,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   485,     0,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,     0,     0,     0,     0,   463,
     464,     0,     0,     0,   496,   497,   811,   465,   466,   467,
     468,   469,     0,     0,   470,   471,   472,   473,     0,   474,
     475,   476,   477,     0,     0,     0,   478,     0,   479,   480,
       0,     0,     0,     0,   481,   482,   483,     0,     0,     0,
     484,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   485,     0,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,     0,     0,     0,     0,   463,   464,     0,
       0,     0,   496,   497,   821,   465,   466,   467,   468,   469,
       0,     0,   470,   471,   472,   473,     0,   474,   475,   476,
     477,     0,     0,     0,   478,     0,   479,   480,     0,     0,
       0,     0,   481,   482,   483,     0,     0,     0,   484,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   485,
       0,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   923,   465,   466,   467,   468,   469,   463,   464,
     470,   471,   472,   473,     0,   474,   475,   476,   477,     0,
       0,     0,   478,     0,   479,   480,     0,     0,     0,     0,
     481,   482,   483,     0,     0,     0,   484,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   485,     0,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     924,     0,     0,     0,     0,     0,   713,   714,   715,   716,
     717,   718,   719,   720,   465,   466,   467,   468,   469,   721,
     722,   470,   471,   472,   473,   830,   474,   475,   476,   477,
    -252,   463,   464,   478,   724,   479,   480,   725,   726,     0,
       0,   481,   482,   483,   727,   728,   729,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   831,   485,     0,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   713,
     714,   715,   716,   717,   718,   719,   720,   465,   466,   467,
     468,   469,   721,   722,   470,   471,   472,   473,   723,   474,
     475,   476,   477,     0,   231,   232,   478,   724,   479,   480,
     725,   726,     0,     0,   481,   482,   483,   727,   728,   729,
     484,   233,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     730,   485,     0,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   463,   464,     0,     0,     0,     0,
       0,     0,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,     0,   252,   253,   254,     0,     0,     0,     0,     0,
     255,   256,   257,   258,   259,     0,     0,   260,   261,   262,
     263,   264,   265,   266,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   267,     0,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   463,   464,   278,   279,
     465,   466,   467,   468,   469,   280,   281,   470,   471,   472,
     473,     0,   474,   475,   476,   477,     0,     0,     0,   478,
       0,   479,   480,     0,     0,   613,     0,   481,   482,   483,
       0,     0,     0,   484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   485,     0,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   463,   464,     0,     0,
       0,     0,     0,     0,     0,   496,   497,     0,     0,     0,
       0,     0,   465,   466,   467,   468,   469,     0,     0,   470,
     471,   472,   473,     0,   474,   475,   476,   477,     0,     0,
       0,   478,     0,   479,   480,     0,     0,     0,     0,   481,
     482,   483,     0,     0,     0,   484,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   485,   815,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   463,   464,
       0,     0,   465,   466,   467,   468,   469,   496,   497,   470,
     471,   472,   473,     0,   474,   475,   476,   477,     0,     0,
       0,   478,     0,   479,   480,     0,     0,     0,     0,   481,
     482,   483,     0,     0,     0,   484,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   904,     0,     0,     0,   485,     0,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   463,   464,
       0,     0,     0,     0,     0,     0,     0,   496,   497,     0,
       0,     0,     0,     0,   465,   466,   467,   468,   469,     0,
       0,   470,   471,   472,   473,     0,   474,   475,   476,   477,
       0,     0,     0,   478,     0,   479,   480,     0,     0,     0,
       0,   481,   482,   483,     0,     0,     0,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   485,     0,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     463,   464,     0,     0,   465,   466,   467,   468,   469,   496,
     497,   470,   471,   472,   473,     0,   474,   475,   476,   477,
       0,     0,     0,   478,     0,   479,   480,     0,     0,     0,
       0,   481,   482,   483,     0,     0,     0,  -557,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   485,     0,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     463,   464,     0,     0,     0,     0,     0,     0,     0,   496,
     497,     0,     0,     0,     0,     0,   465,   466,   467,   468,
     469,     0,     0,   470,   471,   472,   473,     0,   474,   475,
     476,   477,   463,   464,     0,   478,     0,   479,   480,     0,
       0,     0,     0,   481,   482,   483,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     485,     0,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,     0,     0,     0,     0,   465,   466,   467,   468,
     469,   496,   497,   470,   471,   472,   473,     0,   474,   475,
     476,   477,   463,   464,     0,   478,     0,   479,   480,     0,
       0,     0,     0,   481,     0,   483,     0,     0,   465,   466,
     467,   468,   469,     0,     0,   470,   471,   472,   473,     0,
     474,   475,   476,   477,   463,   464,     0,   478,     0,   479,
     480,     0,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   496,   497,     0,     0,     0,     0,     0,     0,   463,
     464,     0,     0,     0,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,     0,     0,     0,     0,   465,   466,
     467,   468,   469,   496,   497,   470,   471,   472,   473,     0,
     474,   475,   476,   477,   463,   464,     0,   478,     0,   479,
     480,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     465,   466,   467,   468,   469,     0,     0,   470,   471,   472,
     473,     0,   474,   475,   476,   477,   463,   464,     0,   478,
       0,   479,   480,     0,     0,   487,   488,   489,   490,   491,
     492,   493,   494,   495,     0,   465,   466,   467,   468,   469,
       0,     0,   470,   496,   497,   473,     0,   474,   475,   476,
     477,     0,     0,     0,   478,     0,   479,   480,   488,   489,
     490,   491,   492,   493,   494,   495,     0,     0,     0,     0,
     465,   466,   467,   468,   469,   496,   497,   470,   471,   472,
     473,     0,   474,   475,   476,   477,     0,     0,     0,   478,
       0,   479,   480,     0,   489,   490,   491,   492,   493,   494,
     495,     0,   465,   466,   467,   468,     0,     0,     0,     0,
     496,   497,   473,     0,   474,   475,   476,   477,     0,     0,
       0,   478,     0,   479,   480,     0,     0,     0,     0,   489,
     490,   491,   492,   493,   494,   495,   444,     0,     0,     0,
       0,     0,     0,     0,     0,   496,   497,     0,   141,     0,
       0,     0,     0,     0,   142,     0,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,     0,
       0,   143,     0,     0,     0,     0,     0,   496,   497,     0,
       0,     0,     0,     0,     0,     0,   144,     0,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,     0,
       0,     0,     0,   446,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   141,     0,     0,     0,     0,
       0,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,   143,     0,
     629,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   144,    51,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   336,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,   632,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   812,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   630,   145,   146,   147,     0,   149,   150,   151,
     152,   153,   336,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   165,   166,   167,     0,     0,   170,   171,
     172,   173,   145,   146,   147,     0,   149,   150,   151,   152,
     153,   336,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   165,   166,   167,     0,     0,   170,   171,   172,
     173,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   141,
       0,     0,     0,     0,     0,   142,     0,     0,     0,   633,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   143,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   144,   813,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   141,     0,     0,     0,
       0,     0,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,   143,
       0,     0,     0,     0,     0,     0,   341,   342,   343,     0,
       0,     0,     0,     0,   144,    51,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   141,     0,     0,     0,     0,     0,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,   143,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   144,    51,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423
};

static const yytype_int16 yycheck[] =
{
      12,   128,   391,   396,   111,   503,   505,   192,   418,   343,
     420,   312,   422,   314,   447,   316,   386,   526,   527,   603,
     369,    34,   514,   499,    14,    15,   623,   611,    81,     0,
     403,    48,    21,   525,    19,     6,    33,     7,    32,    18,
      19,    51,   111,   122,    19,   131,    19,   166,    44,    51,
     117,    44,    45,   150,   141,   444,   161,   446,    29,   144,
      31,   150,    33,   117,    61,   117,    55,   159,    39,   188,
      82,   161,   126,   565,   126,   150,   117,    48,   116,    20,
      21,   119,   120,    54,   189,   126,   119,   120,   597,   119,
     120,   158,   159,   190,   161,   108,   109,   164,   185,   111,
     189,    97,    73,   189,    97,   159,   150,   159,   584,   188,
     164,   186,   158,   117,   503,    14,    15,   150,   159,   150,
      60,  1013,   126,    94,   513,    55,   158,   171,   132,   150,
     150,    61,   158,   150,   159,   132,   150,   150,   144,    32,
     150,  1033,   188,   181,   182,   188,   199,   190,   181,   182,
     150,   181,   182,   150,   144,   159,   188,   183,   148,   190,
     150,   151,   150,   188,   184,    58,    59,   108,   109,   190,
     164,   150,   299,   158,   547,   116,   190,   118,   119,   120,
     121,   170,   779,   158,   125,   158,   188,   679,   182,   132,
     190,   683,   181,   777,   184,   158,   184,   191,   305,   306,
     307,   308,   711,   158,   311,   697,   313,   556,   315,    41,
     317,   182,   182,   583,   117,    55,   158,   188,   161,   326,
     190,    61,   185,   126,   117,   118,   167,   168,   169,   170,
     171,   158,   132,   188,   341,   342,   305,   306,   307,   308,
     181,   182,   311,    75,   313,   144,   315,   166,   740,   148,
     150,   150,   151,   188,   158,   190,   159,   326,   185,    20,
      21,   158,    55,   158,    55,   165,   159,   160,    61,   188,
      61,   164,   341,   342,   167,   158,   158,   158,   385,   183,
     614,     4,     5,   158,     7,   158,   158,   394,   185,   182,
     185,    20,    21,   305,   306,   307,   308,   773,   191,   311,
     407,   313,   185,   315,   185,   317,   150,   891,   183,   785,
     183,   183,    35,   161,   326,   166,   385,   901,   166,   166,
      55,   152,   153,   161,   823,   394,    61,   628,   166,   341,
     342,   516,   439,   150,   810,   150,   184,   188,   407,   524,
     161,   188,   354,   132,   777,   166,   184,   108,   109,   161,
     150,   827,   132,   786,   166,   116,   463,   464,   119,   120,
     121,   150,   158,   184,   125,   161,   184,   181,   164,    55,
     439,   189,   184,   385,   184,    61,    32,   106,   107,   108,
     109,   110,   394,   150,   113,   114,   115,   116,   400,   118,
     119,   120,   121,   150,   893,   407,   125,   807,   127,   128,
      11,   150,    58,    59,   133,   134,   135,   159,   183,   188,
     139,    22,    23,   188,   152,   153,   154,   155,   189,   431,
     181,   182,   186,   187,   150,   189,   185,   439,   150,   188,
      55,   160,   188,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    20,    21,    68,   189,   459,   189,    72,
     152,   153,   181,   182,   152,   153,   154,   150,    97,   141,
     189,   568,   118,   184,   165,   165,    89,    90,    91,    92,
     165,   165,   579,   165,   581,   165,   165,   165,   165,   586,
     165,   588,   589,   188,   591,   150,    34,    34,   184,   150,
     189,   989,   150,   190,    32,   796,   681,   907,   150,   568,
     183,   161,    21,   159,   160,   690,   183,   188,   164,   189,
     579,   167,   161,   161,   189,    41,   185,   586,   184,   588,
     589,   706,   591,   165,   183,   165,   182,   165,    20,    21,
     184,   108,   109,   184,   165,   191,   165,   184,   182,   116,
     933,   118,   119,   120,   121,   150,   653,   183,   125,   184,
     187,   150,   166,   141,   184,   150,   568,    20,    21,   150,
     150,   188,   144,    36,   188,    41,   188,   579,   225,   581,
     677,   188,   188,   188,   586,    32,   588,   589,   150,   591,
     166,   166,   166,   158,   158,   183,   190,   152,   150,   165,
     184,   150,   169,   170,   171,   165,   150,     1,   122,    13,
     989,    58,    59,   159,   181,   182,   161,   144,   677,   736,
     189,     7,   183,   152,   106,   107,   108,   109,   110,   187,
     150,   113,   114,   115,   116,    32,   118,   119,   120,   121,
     150,   183,   150,   125,   183,   127,   128,   190,   183,   150,
     150,   133,   134,   135,   150,   108,   109,   139,   833,   150,
     165,    58,    59,   116,   188,   118,   119,   120,   121,   184,
     183,   118,   125,   185,   150,   677,   183,   183,   160,   183,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     188,   338,   339,   183,   183,   190,   343,   166,   795,   181,
     182,    32,   166,   185,   351,   352,   353,    51,   355,   356,
     183,   358,   159,   160,   188,   183,   183,   164,   190,   166,
     167,   118,   183,   183,   451,    63,    37,   108,   181,   182,
      66,   733,  1012,   204,    64,   182,   795,    82,   733,   842,
       1,   916,   914,   770,   191,   946,   949,   507,   581,   396,
     318,   561,    45,   310,   397,   326,  1002,    -1,   784,   934,
      -1,    -1,   159,   160,    -1,    -1,   447,   164,    -1,   166,
     167,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   433,    -1,    -1,   436,
      -1,   438,    -1,   795,   191,    -1,   443,    -1,   445,    -1,
     447,    58,    59,    -1,    -1,    -1,   981,    -1,    -1,    -1,
     985,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   465,   466,
     917,   918,   469,   470,   471,   472,    -1,   474,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,    -1,
     497,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   917,   918,
     117,   118,    -1,   510,    -1,    -1,  1041,   514,    -1,   126,
     517,   518,    -1,    -1,    -1,    -1,   523,    -1,   525,    -1,
      -1,    -1,   979,    -1,    -1,   532,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,   159,   160,    -1,    -1,   553,   164,    -1,    -1,
     167,    -1,    -1,    -1,    -1,   917,   918,    -1,   565,    -1,
     979,    -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,
      -1,   188,    -1,    -1,   191,    -1,    -1,    -1,   585,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   594,    -1,    -1,
      -1,    -1,   599,   600,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   610,    -1,    -1,   613,   614,    -1,    -1,
     617,    -1,    -1,    -1,    -1,    -1,    -1,   979,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,  1004,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   666,
      -1,    -1,   669,    -1,  1026,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   679,    -1,    -1,    -1,   683,    -1,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
     697,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,   723,   724,   725,   726,
     727,   728,   729,   730,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   740,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,   775,   776,
     139,    -1,    -1,    20,    21,   782,    -1,   784,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,   799,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    32,    -1,    -1,    -1,   815,    -1,
      -1,    -1,   181,   182,    -1,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,   830,   831,    -1,    -1,    -1,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   855,   856,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   864,   865,   106,
     107,    -1,    -1,   110,    -1,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
     118,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,   162,   163,   164,   165,   166,
     927,    -1,    -1,   170,   931,    -1,   933,    -1,    -1,    -1,
     937,   159,   160,    -1,   181,   182,   164,    -1,    32,   167,
      -1,    -1,    -1,    -1,    -1,   952,    -1,    -1,   955,   956,
     957,   958,   959,    -1,   182,   962,   963,    -1,   965,    -1,
     188,    -1,    -1,   191,    58,    59,   973,     1,    -1,    -1,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,  1006,
      -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,
      -1,  1018,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    -1,    56,    57,   118,    -1,    -1,    -1,  1035,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    96,    -1,    -1,   159,   160,    -1,    -1,    -1,
     164,    -1,   166,   167,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,   191,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,   168,   169,    -1,   171,    -1,    -1,
     174,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,   186,   187,   188,   189,   190,     1,    -1,    -1,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,
      -1,    -1,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    32,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    58,
      59,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,
      -1,    -1,    -1,   167,   168,   169,    -1,   171,    -1,    -1,
     174,   175,    -1,    -1,    -1,    14,    15,    16,    17,    18,
     184,    -1,   186,   187,   188,   189,   190,    26,    -1,    -1,
     159,   160,    -1,    32,    -1,   164,    -1,   166,   167,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    51,   182,    53,    -1,   118,    -1,    -1,    58,
      59,    -1,   191,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,   159,   160,    -1,
      -1,    -1,   164,    -1,   166,   167,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,
      -1,    -1,    32,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    -1,    -1,    -1,    -1,    58,    59,
     159,   160,    -1,    -1,    -1,   164,    -1,    -1,   167,   168,
     169,    -1,   171,    -1,    -1,   174,   175,    14,    15,    16,
      17,    18,    -1,   182,    -1,   184,    -1,   186,   187,    26,
     189,    -1,   191,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,   118,    -1,
      -1,    58,    59,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,   167,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   191,    -1,    -1,    32,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,    -1,    -1,    -1,
      58,    59,   159,   160,    -1,    -1,    -1,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    -1,   174,   175,    14,
      15,    16,    17,    18,    -1,   182,    -1,   184,    -1,   186,
     187,    26,   189,    -1,   191,    -1,    32,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
     118,    -1,    58,    59,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,   159,   160,    -1,    -1,    -1,   164,    -1,    -1,   167,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,   182,    -1,   184,    -1,    -1,    -1,
      -1,    -1,    -1,   191,    -1,    -1,    -1,   132,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,    58,    59,    -1,   164,   164,
     166,   167,   167,   168,   169,    -1,   171,    -1,    -1,   174,
     175,    14,    15,    16,    17,    18,   182,   182,    -1,   184,
      -1,   186,   187,    26,   189,   191,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,    -1,   159,   160,    -1,    -1,
      -1,   164,    -1,   166,   167,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,   182,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,   191,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,   167,   168,   169,    -1,   171,    -1,
      26,   174,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    38,   186,   187,   188,   189,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      58,    59,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   167,   168,   169,    -1,   171,    -1,    -1,   174,   175,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   184,    46,
     186,   187,    49,   189,    -1,    -1,    53,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,   159,   160,    -1,    -1,    -1,   164,    -1,   166,   167,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,   191,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,    58,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    -1,   174,   175,    14,
      15,    16,    17,    18,    -1,   182,    -1,   184,   185,   186,
     187,    26,   189,    58,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,   118,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,   118,    89,    90,    91,    92,   159,   160,
      95,    -1,    -1,   164,    -1,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
     191,    -1,    -1,    -1,   159,   160,    -1,   132,    -1,   164,
      -1,   166,   167,    -1,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   182,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   191,    -1,    -1,    -1,
      26,    -1,   167,   168,   169,    -1,   171,    -1,    -1,   174,
     175,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   184,
      46,   186,   187,    49,   189,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      58,    59,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   167,   168,   169,    -1,   171,    -1,    -1,   174,   175,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   184,    46,
     186,   187,    49,   189,    -1,    -1,    53,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,   159,   160,    -1,    -1,    -1,   164,    -1,   166,   167,
      -1,   108,   109,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,   191,    -1,   132,    -1,    -1,    -1,    58,
      59,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     167,   168,   169,    -1,   171,    -1,    -1,   174,   175,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   184,    46,   186,
     187,    49,   189,    -1,    -1,    53,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,
     159,   160,    -1,    -1,    -1,   164,    -1,   166,   167,    -1,
     108,   109,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,    -1,   132,    -1,    -1,    -1,    58,    59,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,   167,
     168,   169,    -1,   171,    -1,    -1,   174,   175,    -1,    38,
      -1,    -1,    -1,    -1,    43,   183,   184,    46,   186,   187,
      49,   189,    -1,    -1,    53,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,   159,
     160,    -1,    -1,    -1,   164,    -1,   166,   167,    -1,   108,
     109,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   191,    -1,   132,    -1,    -1,    -1,    58,    59,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   167,   168,
     169,    -1,   171,    -1,    -1,   174,   175,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   184,    46,   186,   187,    49,
     189,    -1,    -1,    53,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,   159,   160,
      -1,    -1,    -1,   164,    -1,   166,   167,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,   168,   169,
      -1,   171,    -1,    -1,   174,   175,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   184,   185,   186,   187,    26,   189,
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
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   167,
     168,   169,    -1,   171,    -1,    -1,   174,   175,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   184,    46,   186,   187,
      49,   189,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,   168,
     169,    -1,   171,    -1,    -1,   174,   175,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   184,   185,   186,   187,    26,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
     167,   168,   169,    -1,   171,    -1,    -1,   174,   175,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   184,    -1,   186,
     187,    26,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     145,   146,   147,   148,   149,   150,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,   168,   169,    -1,   171,    -1,    -1,   174,
     175,    14,    15,    16,    17,    18,    -1,    -1,    -1,   184,
     185,   186,   187,    26,   189,    -1,    -1,    -1,    -1,    -1,
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
     143,   144,   145,   146,   147,   148,   149,   150,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,   168,   169,    -1,   171,    -1,
      -1,   174,   175,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   184,   185,   186,   187,    26,   189,    -1,    -1,    -1,
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
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,   168,   169,    -1,
     171,    -1,    -1,   174,   175,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   184,   185,   186,   187,    26,   189,    -1,
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
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,   168,
     169,    -1,   171,    -1,    -1,   174,   175,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   184,   185,   186,   187,    26,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     167,   168,   169,    -1,   171,    -1,    -1,   174,   175,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   184,    46,   186,
     187,    49,   189,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
     168,   169,    -1,   171,    -1,    -1,   174,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,
      -1,   189,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    20,    21,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,    -1,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,    -1,    -1,    -1,
      -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    20,    21,
      -1,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,   160,    -1,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,    -1,
      -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      20,    21,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,    -1,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
     182,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    20,    21,    -1,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
     160,    -1,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,   182,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    20,    21,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,   160,    -1,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,    -1,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    20,    21,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,    -1,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,    -1,    -1,    -1,
      -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,   181,   182,   106,
     107,   108,   109,   110,   188,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,   182,    20,    21,   185,   106,
     107,   108,   109,   110,    -1,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,   182,    20,    21,   185,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,    20,
      21,   185,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,    20,
      21,   185,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,   182,    20,    21,   185,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,   182,    20,    21,   185,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,    20,    21,   185,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,    20,    21,   185,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,   181,   182,    -1,    -1,
     185,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   181,   182,   183,   106,
     107,   108,   109,   110,    -1,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   181,   182,   183,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,   181,   182,   183,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,   182,   183,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,
     183,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    20,    21,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,    -1,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,    20,    21,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,   137,   138,
     139,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,   160,    -1,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,   182,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,   118,   119,   120,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,    20,    21,   174,   175,
     106,   107,   108,   109,   110,   181,   182,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,   131,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    20,    21,
      -1,    -1,   106,   107,   108,   109,   110,   181,   182,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,    -1,   160,    -1,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      20,    21,    -1,    -1,   106,   107,   108,   109,   110,   181,
     182,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
     182,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    20,    21,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,   181,   182,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    20,    21,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,    -1,    -1,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    20,    21,    -1,   125,    -1,   127,
     128,    -1,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,   182,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,   181,   182,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    20,    21,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    -1,    -1,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    20,    21,    -1,   125,
      -1,   127,   128,    -1,    -1,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,   181,   182,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,   164,   165,
     166,   167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   181,   182,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,   165,   166,   167,   168,   169,   170,
     171,    -1,   106,   107,   108,   109,    -1,    -1,    -1,    -1,
     181,   182,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   165,
     166,   167,   168,   169,   170,   171,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,   181,   182,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    47,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,   150,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,   150,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,    -1,    -1,    62,   150,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,   150,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   193,     0,     6,    29,    31,    33,    39,    48,    54,
      73,    94,   182,   188,   196,   204,   205,   209,   232,   236,
     253,   317,   323,   326,   332,   370,   373,    18,    19,   150,
     226,   227,   228,   144,   210,   211,   150,   171,   206,   207,
     150,   189,   320,   150,   186,   195,   374,   371,    33,    61,
     132,   150,   229,   230,   231,   245,     4,     5,     7,    35,
     330,    60,   315,   159,   158,   161,   158,   206,    21,    55,
     170,   181,   208,   321,   320,   322,   315,   150,   150,   150,
     132,   184,   158,   183,    55,    61,   237,   239,    55,    61,
     324,    55,    61,   331,    55,    61,   316,    14,    15,   144,
     148,   150,   151,   184,   198,   227,   144,   211,   150,   150,
     150,   159,   188,   190,   320,    55,    61,   194,   189,   372,
     150,   228,   230,   240,   150,   325,   333,   189,   318,   152,
     153,   197,    14,    15,   144,   148,   150,   198,   224,   225,
     208,    24,    30,    47,    62,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   245,   336,   338,   339,
     341,   345,   188,   150,   340,   189,   185,    34,   108,   109,
     150,   235,   238,   161,   189,    97,   328,   329,   313,   141,
     293,   152,   153,   154,   158,   185,   165,   165,   165,   165,
     184,   165,   165,   165,   165,   165,   165,    32,    58,    59,
     118,   159,   160,   164,   167,   182,   191,   188,   150,   306,
     307,    20,    21,    37,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   118,   119,   120,   126,   127,   128,   129,   130,
     133,   134,   135,   136,   137,   138,   139,   160,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   174,   175,
     181,   182,    34,    34,   184,   233,   189,   241,    68,    72,
      89,    90,    91,    92,   337,   319,   150,   334,   190,   314,
     228,   150,   312,   335,   224,   348,   350,   352,   346,   150,
     342,   354,   356,   358,   360,   362,   364,   366,   368,    32,
      32,   164,   182,   191,    14,    15,    16,    17,    18,    26,
      38,    43,    46,    49,    53,    63,    73,    95,   108,   109,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     151,   167,   168,   169,   171,   174,   175,   183,   184,   186,
     187,   200,   202,   241,   245,   247,   255,   262,   265,   268,
     272,   273,   276,   277,   278,   283,   286,   336,   375,   379,
     384,   386,   388,   150,   190,   161,   188,   183,    21,   183,
     185,   293,   302,   303,   161,   234,   243,   189,   150,   190,
     161,   327,   189,   293,   183,    41,   158,   161,   164,   311,
     345,   344,   345,   345,   345,   185,   340,   345,   233,   345,
     233,   345,   233,   150,   304,   305,   345,   307,   183,   345,
     375,   184,   165,   184,   165,   165,   184,   165,   184,   165,
     286,   286,   150,   184,    12,   345,    12,   345,   286,   381,
     385,   199,   286,   286,   286,   245,   286,   286,   286,   187,
     150,   184,   232,    20,    21,   106,   107,   108,   109,   110,
     113,   114,   115,   116,   118,   119,   120,   121,   125,   127,
     128,   133,   134,   135,   139,   160,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   181,   182,   183,   184,
     345,   190,   306,    48,   309,   335,   185,   188,   345,     1,
       8,     9,    10,    12,    25,    27,    28,    37,    40,    42,
      50,    52,    56,    57,    63,    96,   123,   124,   188,   190,
     212,   213,   216,   217,   218,   219,   220,   221,   222,   242,
     244,   246,   248,   249,   250,   251,   252,   253,   254,   274,
     275,   286,   319,   159,   188,   245,   298,   312,   183,   150,
     150,   345,   117,   126,   159,   310,   166,   166,   188,   166,
     166,   166,   188,   166,   234,   166,   234,   166,   234,   161,
     166,   188,   166,   188,   184,   245,   263,   286,   256,   258,
     286,   260,   286,   345,   184,   286,   335,    51,   150,   167,
     184,   286,   376,   377,   378,   380,   381,   382,   383,   335,
     184,   377,   383,   131,   188,   190,   154,   155,   197,   203,
     185,   165,   245,   182,   185,   266,   286,   141,   271,    18,
     150,   336,    18,   150,   336,   286,   286,   286,   286,   286,
     286,   150,   286,   150,   286,   286,   286,   286,   286,   286,
     286,   286,   286,    21,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   119,   120,   150,   181,   182,
     284,   286,   185,   266,   309,   117,   159,   161,   164,   308,
     302,   286,   335,   117,   188,   244,   274,   286,   241,   286,
     286,   150,   188,   144,    56,   286,   241,   117,   244,   286,
     187,   272,   272,    36,   188,   188,   286,   188,   188,   188,
     312,   122,   188,    98,    99,   100,   101,   102,   103,   104,
     105,   111,   112,   117,   126,   129,   130,   136,   137,   138,
     159,   190,   286,   182,   190,   232,   299,    41,   159,   188,
     310,   244,   286,   349,   351,   345,   353,   347,   343,   355,
     166,   359,   166,   363,   166,   345,   367,   304,   369,   185,
     266,   165,   286,   345,   185,   345,   345,   185,   345,   185,
     166,   286,   185,    19,   272,   126,   308,   185,   158,   188,
     378,   183,   158,   183,   188,    19,   185,   378,   190,   286,
     381,   190,   286,   152,   201,   279,   281,   150,   376,   158,
     185,   117,   126,   159,   164,   269,   270,   233,   165,   165,
     184,   183,    18,   150,   336,   161,   150,   286,   150,   286,
       1,   183,   185,   345,   244,   286,   241,    19,   244,   286,
     117,   159,   188,    13,   241,   159,   161,   144,   244,   286,
     188,   187,   189,   241,   272,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   123,   124,   286,   286,   286,
     286,   286,   286,   286,   123,   124,   286,   188,   231,     7,
     293,   297,   150,   244,   286,   188,   357,   361,   365,   185,
     150,   185,   166,   166,   166,   166,   271,   185,   266,   286,
     286,   377,   378,   150,   376,   183,   183,   286,   183,   382,
     266,   377,   190,   183,   156,   152,   345,   233,   183,   286,
     150,   150,   150,   150,   158,   183,   234,   287,   289,   185,
     266,   165,   286,   183,   183,   285,   159,   308,   266,   188,
     241,   223,   188,   243,    11,    22,    23,   214,   215,   286,
     286,   286,   286,   183,    55,    61,   296,    44,    97,   294,
     188,   188,   166,   188,   264,   257,   259,   261,   184,   188,
     378,   183,   126,   308,   183,   188,   378,   183,   166,   234,
     185,   269,   183,   131,   241,   267,   345,   345,   185,   291,
     286,   188,   286,   190,   241,   286,   188,    45,   294,   296,
     286,   150,   286,   286,   286,   185,   286,   286,   183,   183,
     286,   286,   286,   190,   280,   166,   117,   286,   166,   166,
     345,   241,   241,   300,    32,   295,   309,   185,   166,   185,
     188,   387,   183,   387,   183,   245,   282,   286,   288,   290,
     166,   215,   238,   301,   286,    51,   183,   190,   245,   292,
     188,   238,   185,   286,   183,   183,   241
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   192,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   194,   194,   194,   195,   195,
     196,   197,   197,   197,   197,   198,   199,   199,   199,   200,
     201,   201,   203,   202,   204,   205,   206,   206,   206,   206,
     207,   207,   208,   208,   209,   210,   210,   211,   211,   212,
     213,   213,   214,   214,   215,   215,   215,   216,   216,   217,
     218,   219,   220,   221,   223,   222,   224,   224,   224,   224,
     224,   224,   225,   225,   226,   226,   226,   227,   227,   227,
     227,   227,   227,   227,   227,   228,   228,   229,   229,   229,
     230,   230,   231,   231,   232,   232,   233,   233,   233,   234,
     234,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   236,   237,   237,   237,   238,
     240,   239,   241,   241,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   243,   243,   243,   244,   244,   244,
     245,   245,   245,   246,   247,   247,   247,   247,   248,   249,
     250,   250,   250,   250,   250,   251,   251,   251,   251,   252,
     253,   253,   254,   256,   257,   255,   258,   259,   255,   260,
     261,   255,   263,   264,   262,   265,   265,   265,   266,   266,
     267,   267,   267,   268,   268,   268,   269,   269,   269,   269,
     270,   270,   271,   271,   272,   272,   273,   273,   273,   273,
     273,   273,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   275,   275,   275,   275,   276,   277,   277,   278,   279,
     280,   278,   281,   282,   278,   283,   283,   284,   285,   283,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   287,   288,
     286,   286,   286,   286,   289,   290,   286,   286,   286,   291,
     292,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   293,   293,   294,   294,
     294,   295,   295,   296,   296,   296,   297,   298,   299,   298,
     300,   298,   301,   298,   298,   302,   302,   303,   303,   304,
     304,   305,   305,   306,   307,   307,   308,   308,   309,   309,
     309,   309,   309,   309,   310,   310,   310,   311,   311,   312,
     312,   312,   312,   312,   313,   314,   313,   315,   315,   316,
     316,   316,   317,   318,   317,   319,   319,   319,   321,   320,
     322,   322,   323,   323,   324,   324,   324,   325,   326,   326,
     327,   327,   328,   328,   329,   330,   330,   331,   331,   331,
     333,   334,   332,   335,   335,   335,   335,   336,   336,   336,
     336,   336,   336,   336,   336,   336,   336,   336,   336,   336,
     336,   336,   336,   336,   336,   336,   336,   336,   336,   336,
     336,   336,   337,   337,   337,   337,   337,   337,   338,   339,
     339,   340,   340,   342,   343,   341,   344,   344,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   346,   347,   345,   345,   348,
     349,   345,   350,   351,   345,   352,   353,   345,   345,   354,
     355,   345,   356,   357,   345,   345,   358,   359,   345,   360,
     361,   345,   345,   362,   363,   345,   364,   365,   345,   366,
     367,   345,   368,   369,   345,   371,   372,   370,   374,   373,
     375,   375,   375,   375,   376,   376,   376,   376,   377,   377,
     378,   378,   379,   379,   379,   379,   379,   379,   380,   380,
     381,   381,   382,   382,   383,   383,   384,   384,   385,   385,
     386,   387,   387,   388,   388
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
       2,     2,     2,     2,     3,     3,     0,     1,     1,     3,
       0,     4,     3,     7,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     2,     2,     2,     0,     2,     2,     3,     2,     2,
       1,     3,     2,     2,     2,     4,     5,     2,     1,     1,
       2,     3,     4,     2,     3,     3,     4,     2,     3,     4,
       1,     1,     2,     0,     0,     7,     0,     0,     7,     0,
       0,     7,     0,     0,     6,     5,     8,    10,     1,     3,
       1,     2,     3,     1,     1,     2,     2,     2,     2,     2,
       1,     3,     0,     4,     1,     6,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     4,     4,     4,     6,     5,     6,     3,     0,
       0,     8,     0,     0,     9,     3,     4,     0,     0,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
       4,     5,     4,     5,     3,     4,     1,     3,     4,     3,
       4,     2,     4,     4,     7,     8,     3,     5,     0,     0,
       8,     3,     3,     3,     0,     0,     8,     3,     4,     0,
       0,     9,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     2,     4,     4,     5,     0,     4,     0,     1,
       1,     0,     1,     0,     1,     1,     4,     0,     0,     4,
       0,     8,     0,     9,     5,     2,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     1,     1,     2,
       3,     5,     3,     3,     1,     1,     1,     0,     1,     4,
       6,     5,     5,     4,     0,     0,     4,     0,     1,     0,
       1,     1,     6,     0,     6,     0,     3,     5,     0,     4,
       2,     3,     4,     2,     0,     1,     1,     1,     7,     9,
       0,     2,     0,     1,     3,     1,     1,     0,     1,     1,
       0,     0,     9,     1,     3,     3,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     3,     0,     0,     6,     1,     3,     1,     1,
       1,     1,     4,     3,     4,     2,     2,     3,     2,     3,
       2,     2,     3,     3,     2,     0,     0,     6,     2,     0,
       0,     6,     0,     0,     6,     0,     0,     6,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     0,
       0,     6,     0,     0,     6,     0,     0,     8,     0,     7,
       1,     1,     1,     1,     3,     3,     5,     5,     1,     3,
       0,     2,     6,     5,     7,     8,     6,     8,     1,     3,
       3,     1,     1,     1,     1,     3,     5,     5,     1,     3,
       4,     0,     3,    10,    10
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

    case YYSYMBOL_func_addr_expr: /* func_addr_expr  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_field: /* expr_field  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr: /* expr  */
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

    case YYSYMBOL_let_variable_declaration: /* let_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_global_variable_declaration_list: /* global_variable_declaration_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
        break;

    case YYSYMBOL_enum_list: /* enum_list  */
            { delete ((*yyvaluep).pEnum); }
        break;

    case YYSYMBOL_enum_name: /* enum_name  */
            { delete ((*yyvaluep).s); }
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

  case 155: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 156: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 157: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 158: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 159: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 160: /* $@3: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 161: /* function_declaration: optional_public_or_private_function $@3 function_declaration_header expression_block  */
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

  case 162: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 163: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 164: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 165: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 166: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 167: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 168: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 169: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 170: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 171: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 172: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 173: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 174: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 175: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 176: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 177: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 178: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 179: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 180: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 181: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 182: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 183: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 184: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 185: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 186: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 187: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 188: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 189: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 190: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 191: /* name_in_namespace: "name" "::" "name"  */
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

  case 192: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 193: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 194: /* expr_new: "new" type_declaration  */
                                                   {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 195: /* expr_new: "new" type_declaration '(' ')'  */
                                                           {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 196: /* expr_new: "new" type_declaration '(' expr_list ')'  */
                                                                                {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 197: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 198: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 199: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 200: /* expression_return: "return" ';'  */
                            {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),nullptr);
    }
    break;

  case 201: /* expression_return: "return" expr ';'  */
                                          {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 202: /* expression_return: "return" "<-" expr ';'  */
                                                 {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 203: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 204: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 205: /* expression_yield: "yield" expr ';'  */
                                         {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 206: /* expression_yield: "yield" "<-" expr ';'  */
                                                {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 207: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 208: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 209: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 210: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 211: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 212: /* expression_let: kwd_let let_variable_declaration  */
                                               {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 213: /* $@4: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 214: /* $@5: %empty  */
                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 215: /* expr_cast: "cast" '<' $@4 type_declaration '>' $@5 expr  */
                                                                                                                                     {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 216: /* $@6: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 217: /* $@7: %empty  */
                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 218: /* expr_cast: "upcast" '<' $@6 type_declaration '>' $@7 expr  */
                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 219: /* $@8: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 220: /* $@9: %empty  */
                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 221: /* expr_cast: "reinterpret" '<' $@8 type_declaration '>' $@9 expr  */
                                                                                                                                            {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 222: /* $@10: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 223: /* $@11: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 224: /* expr_type_decl: "type" '<' $@10 type_declaration '>' $@11  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 225: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 226: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 227: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 228: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 229: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 230: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 232: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 233: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 234: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 235: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 236: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 237: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 238: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 239: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 240: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 241: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 242: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 243: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 244: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 245: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 246: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 247: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 248: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 249: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 250: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 251: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 252: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 253: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 254: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 255: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 256: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 257: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 258: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 259: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 260: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 261: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 262: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 263: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 264: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 265: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 266: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 267: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 268: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 269: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 270: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 271: /* expr_assign_pipe: expr '=' "@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 272: /* expr_assign_pipe: expr '=' "@@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 273: /* expr_assign_pipe: expr "<-" "@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 274: /* expr_assign_pipe: expr "<-" "@@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 275: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 276: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 277: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 278: /* func_addr_expr: '@' '@' name_in_namespace  */
                                                 {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 279: /* $@12: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 280: /* $@13: %empty  */
                                                                                          { yyextra->das_arrow_depth --; }
    break;

  case 281: /* func_addr_expr: '@' '@' '<' $@12 type_declaration '>' $@13 name_in_namespace  */
                                                                                                                                                   {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
    break;

  case 282: /* $@14: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 283: /* $@15: %empty  */
                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 284: /* func_addr_expr: '@' '@' '<' $@14 optional_function_argument_list optional_function_type '>' $@15 name_in_namespace  */
                                                                                                                                                                                            {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-7])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType.get(), (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = expr;
    }
    break;

  case 285: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 286: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 287: /* $@16: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 288: /* $@17: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 289: /* expr_field: expr '.' $@16 error $@17  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 290: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 291: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 292: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 293: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 294: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 295: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 296: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 297: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 298: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 299: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 300: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 301: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 302: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 303: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 304: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 305: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 306: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 307: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 308: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 309: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 310: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 311: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 312: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 313: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 314: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 315: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 316: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 317: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 318: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 319: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 320: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 325: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 326: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 328: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 329: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 330: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 331: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 332: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 333: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 334: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 335: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 336: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 337: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 338: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 339: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 340: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 341: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 343: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 344: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' ')'  */
                                                                                                   {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 345: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' expr ')'  */
                                                                                                                 {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 346: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 348: /* $@18: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 349: /* $@19: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 350: /* expr: expr "is" "type" '<' $@18 type_declaration '>' $@19  */
                                                                                                                                            {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 351: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 352: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 353: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 354: /* $@20: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 355: /* $@21: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 356: /* expr: expr "as" "type" '<' $@20 type_declaration '>' $@21  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 357: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 358: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 359: /* $@22: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 360: /* $@23: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 361: /* expr: expr '?' "as" "type" '<' $@22 type_declaration '>' $@23  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 362: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 363: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 364: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 366: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 367: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 368: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 369: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 370: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 371: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) =ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 372: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 373: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 374: /* expr: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),""); }
    break;

  case 375: /* expr: "$$" "name" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),*(yyvsp[-3].s)); delete (yyvsp[-3].s); }
    break;

  case 376: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 377: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 378: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 379: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 380: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 381: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 382: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 383: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 384: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 385: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 386: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 387: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 388: /* $@24: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 389: /* struct_variable_declaration_list: struct_variable_declaration_list $@24 structure_variable_declaration ';'  */
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

  case 390: /* $@25: %empty  */
                                                                                             {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 391: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@25 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
            }
    break;

  case 392: /* $@26: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 393: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@26 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 394: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 395: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 396: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 397: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 398: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 399: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 400: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 401: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 402: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 403: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 404: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 405: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 406: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 407: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 408: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 409: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 410: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 411: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 412: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 413: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 414: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 415: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 416: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 417: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 418: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 419: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration ';'  */
                                                                             {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 420: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr ';'  */
                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 421: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr_pipe  */
                                                                                                                    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 422: /* let_variable_declaration: variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                      {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 423: /* let_variable_declaration: variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 424: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 425: /* $@27: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 426: /* global_variable_declaration_list: global_variable_declaration_list $@27 optional_field_annotation let_variable_declaration  */
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

  case 427: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 428: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 429: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 430: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 431: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 432: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 433: /* $@28: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 434: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@28 optional_field_annotation let_variable_declaration  */
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

  case 435: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 436: /* enum_list: enum_list "name" ';'  */
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

  case 437: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 438: /* $@29: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 439: /* single_alias: "name" $@29 '=' type_declaration  */
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

  case 444: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 445: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 446: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 447: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 448: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].s)->c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pEnum),tokAt(scanner,(yylsp[-1])),Type::tInt);
    }
    break;

  case 449: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                      {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].s)->c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].s),tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pEnum),tokAt(scanner,(yylsp[-1])),(yyvsp[-3].type));
    }
    break;

  case 450: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 451: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 452: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 453: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 454: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 455: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 456: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 457: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 458: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 459: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 460: /* $@30: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 461: /* $@31: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 462: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@30 structure_name $@31 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
        }
    }
    break;

  case 463: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 464: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 465: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 466: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 467: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 468: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 469: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 470: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 471: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 472: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 473: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 474: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 475: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 476: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 477: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 478: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 479: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 480: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 481: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 482: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 483: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 484: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 485: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 486: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 487: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 488: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 489: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 490: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 491: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 492: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 493: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 494: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 495: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 496: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 497: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 498: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 499: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 500: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 501: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 502: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 503: /* $@32: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 504: /* $@33: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 505: /* bitfield_type_declaration: "bitfield" '<' $@32 bitfield_bits '>' $@33  */
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

  case 506: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 507: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 508: /* type_declaration: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 509: /* type_declaration: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 510: /* type_declaration: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 511: /* type_declaration: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 512: /* type_declaration: type_declaration '[' expr ']'  */
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

  case 513: /* type_declaration: type_declaration '[' ']'  */
                                           {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 514: /* type_declaration: type_declaration '-' '[' ']'  */
                                               {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 515: /* type_declaration: type_declaration "explicit"  */
                                                {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 516: /* type_declaration: type_declaration "const"  */
                                             {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 517: /* type_declaration: type_declaration '-' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 518: /* type_declaration: type_declaration '&'  */
                                       {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 519: /* type_declaration: type_declaration '-' '&'  */
                                           {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 520: /* type_declaration: type_declaration '#'  */
                                       {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 521: /* type_declaration: type_declaration "implicit"  */
                                                {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 522: /* type_declaration: type_declaration '-' '#'  */
                                           {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 523: /* type_declaration: type_declaration '=' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 524: /* type_declaration: type_declaration '?'  */
                                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 525: /* $@34: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 526: /* $@35: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 527: /* type_declaration: "smart_ptr" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 528: /* type_declaration: type_declaration "??"  */
                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 529: /* $@36: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 530: /* $@37: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 531: /* type_declaration: "array" '<' $@36 type_declaration '>' $@37  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 532: /* $@38: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 533: /* $@39: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 534: /* type_declaration: "table" '<' $@38 table_type_pair '>' $@39  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 535: /* $@40: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 536: /* $@41: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 537: /* type_declaration: "iterator" '<' $@40 type_declaration '>' $@41  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 538: /* type_declaration: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 539: /* $@42: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 540: /* $@43: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 541: /* type_declaration: "block" '<' $@42 type_declaration '>' $@43  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 542: /* $@44: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 543: /* $@45: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 544: /* type_declaration: "block" '<' $@44 optional_function_argument_list optional_function_type '>' $@45  */
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

  case 545: /* type_declaration: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 546: /* $@46: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 547: /* $@47: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 548: /* type_declaration: "function" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 549: /* $@48: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 550: /* $@49: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 551: /* type_declaration: "function" '<' $@48 optional_function_argument_list optional_function_type '>' $@49  */
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

  case 552: /* type_declaration: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 553: /* $@50: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 554: /* $@51: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 555: /* type_declaration: "lambda" '<' $@50 type_declaration '>' $@51  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 556: /* $@52: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 557: /* $@53: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 558: /* type_declaration: "lambda" '<' $@52 optional_function_argument_list optional_function_type '>' $@53  */
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

  case 559: /* $@54: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 560: /* $@55: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 561: /* type_declaration: "tuple" '<' $@54 tuple_type_list '>' $@55  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 562: /* $@56: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 563: /* $@57: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 564: /* type_declaration: "variant" '<' $@56 variant_type_list '>' $@57  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 565: /* $@58: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 566: /* $@59: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 567: /* variant_alias_declaration: "variant" $@58 "name" $@59 '{' variant_type_list ';' '}'  */
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

  case 568: /* $@60: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 569: /* bitfield_alias_declaration: "bitfield" $@60 "name" '{' bitfield_bits ';' '}'  */
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

  case 570: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 571: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 572: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 573: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 574: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 575: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 576: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 577: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 578: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 579: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 580: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 581: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 582: /* make_struct_decl: "[[" type_declaration make_struct_dim optional_block ']' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 583: /* make_struct_decl: "[[" type_declaration optional_block ']' ']'  */
                                                                        {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 584: /* make_struct_decl: "[[" type_declaration '(' ')' optional_block ']' ']'  */
                                                                                {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 585: /* make_struct_decl: "[[" type_declaration '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                     {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 586: /* make_struct_decl: "[{" type_declaration make_struct_dim optional_block '}' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 587: /* make_struct_decl: "[{" type_declaration '(' ')' make_struct_dim optional_block '}' ']'  */
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

  case 588: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 589: /* make_tuple: make_tuple ',' expr  */
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

  case 590: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 591: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 592: /* make_any_tuple: make_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 593: /* make_any_tuple: make_map_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 594: /* make_dim: make_any_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 595: /* make_dim: make_dim ';' make_any_tuple  */
                                              {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 596: /* make_dim_decl: "[[" type_declaration make_dim ']' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 597: /* make_dim_decl: "[{" type_declaration make_dim '}' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 598: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 599: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 600: /* make_table_decl: "{{" make_table '}' '}'  */
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

  case 601: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 602: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 603: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 604: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


