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
  YYSYMBOL_BRABRAB = 140,                  /* "[["  */
  YYSYMBOL_BRACBRB = 141,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 142,                  /* "{{"  */
  YYSYMBOL_INTEGER = 143,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 144,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 145,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 146,    /* "unsigned long integer constant"  */
  YYSYMBOL_FLOAT = 147,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 148,                   /* "double constant"  */
  YYSYMBOL_NAME = 149,                     /* "name"  */
  YYSYMBOL_BEGIN_STRING = 150,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 151,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 152,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 153,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 154,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 155,          /* "}"  */
  YYSYMBOL_END_OF_READ = 156,              /* "end of failed eader macro"  */
  YYSYMBOL_157_ = 157,                     /* ','  */
  YYSYMBOL_158_ = 158,                     /* '='  */
  YYSYMBOL_159_ = 159,                     /* '?'  */
  YYSYMBOL_160_ = 160,                     /* ':'  */
  YYSYMBOL_161_ = 161,                     /* '|'  */
  YYSYMBOL_162_ = 162,                     /* '^'  */
  YYSYMBOL_163_ = 163,                     /* '&'  */
  YYSYMBOL_164_ = 164,                     /* '<'  */
  YYSYMBOL_165_ = 165,                     /* '>'  */
  YYSYMBOL_166_ = 166,                     /* '-'  */
  YYSYMBOL_167_ = 167,                     /* '+'  */
  YYSYMBOL_168_ = 168,                     /* '*'  */
  YYSYMBOL_169_ = 169,                     /* '/'  */
  YYSYMBOL_170_ = 170,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 171,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 172,               /* UNARY_PLUS  */
  YYSYMBOL_173_ = 173,                     /* '~'  */
  YYSYMBOL_174_ = 174,                     /* '!'  */
  YYSYMBOL_PRE_INC = 175,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 176,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 177,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 178,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 179,                    /* DEREF  */
  YYSYMBOL_180_ = 180,                     /* '.'  */
  YYSYMBOL_181_ = 181,                     /* '['  */
  YYSYMBOL_182_ = 182,                     /* ']'  */
  YYSYMBOL_183_ = 183,                     /* '('  */
  YYSYMBOL_184_ = 184,                     /* ')'  */
  YYSYMBOL_185_ = 185,                     /* '$'  */
  YYSYMBOL_186_ = 186,                     /* '@'  */
  YYSYMBOL_187_ = 187,                     /* ';'  */
  YYSYMBOL_188_ = 188,                     /* '{'  */
  YYSYMBOL_189_ = 189,                     /* '}'  */
  YYSYMBOL_190_ = 190,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 191,                 /* $accept  */
  YYSYMBOL_program = 192,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 193, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 194,              /* module_name  */
  YYSYMBOL_module_declaration = 195,       /* module_declaration  */
  YYSYMBOL_character_sequence = 196,       /* character_sequence  */
  YYSYMBOL_string_constant = 197,          /* string_constant  */
  YYSYMBOL_string_builder_body = 198,      /* string_builder_body  */
  YYSYMBOL_string_builder = 199,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 200, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 201,              /* expr_reader  */
  YYSYMBOL_202_1 = 202,                    /* $@1  */
  YYSYMBOL_options_declaration = 203,      /* options_declaration  */
  YYSYMBOL_require_declaration = 204,      /* require_declaration  */
  YYSYMBOL_require_module_name = 205,      /* require_module_name  */
  YYSYMBOL_require_module = 206,           /* require_module  */
  YYSYMBOL_is_public_module = 207,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 208,       /* expect_declaration  */
  YYSYMBOL_expect_list = 209,              /* expect_list  */
  YYSYMBOL_expect_error = 210,             /* expect_error  */
  YYSYMBOL_expression_label = 211,         /* expression_label  */
  YYSYMBOL_expression_goto = 212,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 213,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 214,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 215,          /* if_or_static_if  */
  YYSYMBOL_expression_if_then_else = 216,  /* expression_if_then_else  */
  YYSYMBOL_expression_for_loop = 217,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 218,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 219,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 220,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 221,    /* expression_with_alias  */
  YYSYMBOL_222_2 = 222,                    /* $@2  */
  YYSYMBOL_annotation_argument_value = 223, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 224, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 225, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 226,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 227, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 228, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration = 229,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 230,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 231, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 232, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 233,   /* optional_function_type  */
  YYSYMBOL_function_name = 234,            /* function_name  */
  YYSYMBOL_global_function_declaration = 235, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 236, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 237, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 238,     /* function_declaration  */
  YYSYMBOL_expression_block = 239,         /* expression_block  */
  YYSYMBOL_expression_any = 240,           /* expression_any  */
  YYSYMBOL_expressions = 241,              /* expressions  */
  YYSYMBOL_expr_pipe = 242,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 243,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 244,        /* expression_delete  */
  YYSYMBOL_expr_new = 245,                 /* expr_new  */
  YYSYMBOL_expression_break = 246,         /* expression_break  */
  YYSYMBOL_expression_continue = 247,      /* expression_continue  */
  YYSYMBOL_expression_return = 248,        /* expression_return  */
  YYSYMBOL_expression_yield = 249,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 250,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 251,                  /* kwd_let  */
  YYSYMBOL_expression_let = 252,           /* expression_let  */
  YYSYMBOL_expr_cast = 253,                /* expr_cast  */
  YYSYMBOL_254_3 = 254,                    /* $@3  */
  YYSYMBOL_255_4 = 255,                    /* $@4  */
  YYSYMBOL_256_5 = 256,                    /* $@5  */
  YYSYMBOL_257_6 = 257,                    /* $@6  */
  YYSYMBOL_258_7 = 258,                    /* $@7  */
  YYSYMBOL_259_8 = 259,                    /* $@8  */
  YYSYMBOL_expr_type_decl = 260,           /* expr_type_decl  */
  YYSYMBOL_261_9 = 261,                    /* $@9  */
  YYSYMBOL_262_10 = 262,                   /* $@10  */
  YYSYMBOL_expr_type_info = 263,           /* expr_type_info  */
  YYSYMBOL_expr_list = 264,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 265,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 266,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 267,            /* capture_entry  */
  YYSYMBOL_capture_list = 268,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 269,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 270,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 271,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 272,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 273,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 274,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 275,         /* expr_method_call  */
  YYSYMBOL_func_addr_expr = 276,           /* func_addr_expr  */
  YYSYMBOL_277_11 = 277,                   /* $@11  */
  YYSYMBOL_278_12 = 278,                   /* $@12  */
  YYSYMBOL_279_13 = 279,                   /* $@13  */
  YYSYMBOL_280_14 = 280,                   /* $@14  */
  YYSYMBOL_expr_field = 281,               /* expr_field  */
  YYSYMBOL_282_15 = 282,                   /* $@15  */
  YYSYMBOL_283_16 = 283,                   /* $@16  */
  YYSYMBOL_expr = 284,                     /* expr  */
  YYSYMBOL_285_17 = 285,                   /* $@17  */
  YYSYMBOL_286_18 = 286,                   /* $@18  */
  YYSYMBOL_optional_field_annotation = 287, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 288,        /* optional_override  */
  YYSYMBOL_optional_constant = 289,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 290, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 291, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 292, /* struct_variable_declaration_list  */
  YYSYMBOL_293_19 = 293,                   /* $@19  */
  YYSYMBOL_function_argument_declaration = 294, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 295,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 296,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 297,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 298,             /* variant_type  */
  YYSYMBOL_variant_type_list = 299,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 300,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 301,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 302,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 303,             /* optional_ref  */
  YYSYMBOL_let_variable_declaration = 304, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 305, /* global_variable_declaration_list  */
  YYSYMBOL_optional_shared = 306,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 307, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 308,               /* global_let  */
  YYSYMBOL_309_20 = 309,                   /* $@20  */
  YYSYMBOL_enum_list = 310,                /* enum_list  */
  YYSYMBOL_single_alias = 311,             /* single_alias  */
  YYSYMBOL_alias_list = 312,               /* alias_list  */
  YYSYMBOL_alias_declaration = 313,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 314, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_declaration = 315,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 316, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 317,          /* optional_sealed  */
  YYSYMBOL_structure_name = 318,           /* structure_name  */
  YYSYMBOL_class_or_struct = 319,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 320, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 321,    /* structure_declaration  */
  YYSYMBOL_322_21 = 322,                   /* $@21  */
  YYSYMBOL_variable_name_with_pos_list = 323, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 324,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 325, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 326, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 327,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 328,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 329, /* bitfield_type_declaration  */
  YYSYMBOL_330_22 = 330,                   /* $@22  */
  YYSYMBOL_331_23 = 331,                   /* $@23  */
  YYSYMBOL_type_declaration = 332,         /* type_declaration  */
  YYSYMBOL_333_24 = 333,                   /* $@24  */
  YYSYMBOL_334_25 = 334,                   /* $@25  */
  YYSYMBOL_335_26 = 335,                   /* $@26  */
  YYSYMBOL_336_27 = 336,                   /* $@27  */
  YYSYMBOL_337_28 = 337,                   /* $@28  */
  YYSYMBOL_338_29 = 338,                   /* $@29  */
  YYSYMBOL_339_30 = 339,                   /* $@30  */
  YYSYMBOL_340_31 = 340,                   /* $@31  */
  YYSYMBOL_341_32 = 341,                   /* $@32  */
  YYSYMBOL_342_33 = 342,                   /* $@33  */
  YYSYMBOL_343_34 = 343,                   /* $@34  */
  YYSYMBOL_344_35 = 344,                   /* $@35  */
  YYSYMBOL_345_36 = 345,                   /* $@36  */
  YYSYMBOL_346_37 = 346,                   /* $@37  */
  YYSYMBOL_347_38 = 347,                   /* $@38  */
  YYSYMBOL_348_39 = 348,                   /* $@39  */
  YYSYMBOL_349_40 = 349,                   /* $@40  */
  YYSYMBOL_350_41 = 350,                   /* $@41  */
  YYSYMBOL_351_42 = 351,                   /* $@42  */
  YYSYMBOL_352_43 = 352,                   /* $@43  */
  YYSYMBOL_353_44 = 353,                   /* $@44  */
  YYSYMBOL_354_45 = 354,                   /* $@45  */
  YYSYMBOL_355_46 = 355,                   /* $@46  */
  YYSYMBOL_356_47 = 356,                   /* $@47  */
  YYSYMBOL_variant_alias_declaration = 357, /* variant_alias_declaration  */
  YYSYMBOL_358_48 = 358,                   /* $@48  */
  YYSYMBOL_bitfield_alias_declaration = 359, /* bitfield_alias_declaration  */
  YYSYMBOL_360_49 = 360,                   /* $@49  */
  YYSYMBOL_make_decl = 361,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 362,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 363,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 364,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 365,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 366,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 367,           /* make_map_tuple  */
  YYSYMBOL_make_any_tuple = 368,           /* make_any_tuple  */
  YYSYMBOL_make_dim = 369,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 370,            /* make_dim_decl  */
  YYSYMBOL_make_table = 371,               /* make_table  */
  YYSYMBOL_make_table_decl = 372,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 373, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 374       /* array_comprehension  */
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
#define YYLAST   7891

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  191
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  184
/* YYNRULES -- Number of rules.  */
#define YYNRULES  578
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1010

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   418


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
       2,     2,     2,   174,     2,   190,   185,   170,   163,     2,
     183,   184,   168,   167,   157,   166,   180,   169,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   160,   187,
     164,   158,   165,   159,   186,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   181,     2,   182,   162,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   188,   161,   189,   173,     2,     2,     2,
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
     155,   156,   171,   172,   175,   176,   177,   178,   179
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   451,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   467,   468,   469,   473,   474,
     478,   495,   496,   497,   498,   502,   506,   511,   520,   528,
     544,   549,   557,   557,   587,   608,   612,   615,   619,   625,
     634,   637,   643,   644,   648,   652,   653,   657,   660,   666,
     672,   675,   681,   682,   686,   687,   688,   697,   698,   702,
     711,   717,   725,   735,   744,   744,   751,   752,   753,   754,
     755,   756,   760,   765,   773,   774,   775,   779,   780,   781,
     782,   783,   784,   785,   786,   792,   795,   801,   802,   803,
     807,   815,   828,   832,   839,   840,   844,   845,   846,   850,
     853,   860,   864,   865,   866,   867,   868,   869,   870,   871,
     872,   873,   874,   875,   876,   877,   878,   879,   880,   881,
     882,   883,   884,   885,   886,   887,   888,   889,   890,   891,
     892,   893,   894,   895,   896,   897,   898,   899,   900,   901,
     902,   903,   904,   905,   906,   907,   908,   909,   910,   911,
     912,   916,   938,   939,   940,   944,   950,   958,   962,   973,
     974,   975,   976,   977,   978,   979,   980,   981,   982,   983,
     984,   985,   986,   987,   988,   989,   990,   991,   992,   996,
    1001,  1007,  1013,  1047,  1050,  1056,  1057,  1068,  1072,  1078,
    1081,  1084,  1088,  1094,  1098,  1102,  1105,  1108,  1113,  1116,
    1124,  1127,  1132,  1135,  1143,  1149,  1150,  1154,  1160,  1160,
    1160,  1163,  1163,  1163,  1168,  1168,  1168,  1176,  1176,  1176,
    1182,  1192,  1203,  1218,  1221,  1227,  1228,  1235,  1246,  1247,
    1248,  1252,  1253,  1254,  1255,  1259,  1264,  1272,  1273,  1277,
    1282,  1289,  1290,  1291,  1292,  1293,  1294,  1298,  1299,  1300,
    1301,  1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,  1310,
    1311,  1312,  1313,  1314,  1315,  1316,  1320,  1321,  1322,  1323,
    1327,  1338,  1343,  1353,  1357,  1357,  1357,  1364,  1364,  1364,
    1378,  1382,  1386,  1386,  1386,  1393,  1394,  1395,  1396,  1397,
    1398,  1399,  1400,  1401,  1402,  1403,  1404,  1405,  1406,  1407,
    1408,  1409,  1410,  1411,  1412,  1413,  1414,  1415,  1416,  1417,
    1418,  1419,  1420,  1421,  1422,  1423,  1424,  1425,  1426,  1427,
    1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,
    1443,  1444,  1445,  1449,  1453,  1456,  1459,  1460,  1461,  1462,
    1465,  1468,  1469,  1472,  1472,  1472,  1475,  1479,  1483,  1487,
    1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,
    1505,  1506,  1510,  1511,  1512,  1516,  1517,  1521,  1522,  1523,
    1527,  1537,  1540,  1545,  1544,  1548,  1553,  1562,  1567,  1575,
    1576,  1580,  1583,  1592,  1593,  1597,  1606,  1607,  1612,  1613,
    1617,  1623,  1629,  1632,  1636,  1642,  1651,  1652,  1653,  1657,
    1658,  1662,  1665,  1670,  1675,  1683,  1694,  1697,  1705,  1706,
    1710,  1711,  1712,  1716,  1719,  1719,  1725,  1728,  1737,  1750,
    1762,  1763,  1767,  1768,  1772,  1773,  1774,  1778,  1781,  1787,
    1788,  1792,  1793,  1797,  1803,  1804,  1808,  1809,  1810,  1815,
    1814,  1821,  1828,  1837,  1843,  1854,  1855,  1856,  1857,  1858,
    1859,  1860,  1861,  1862,  1863,  1864,  1865,  1866,  1867,  1868,
    1869,  1870,  1871,  1872,  1873,  1874,  1875,  1876,  1877,  1878,
    1882,  1883,  1884,  1885,  1886,  1887,  1891,  1902,  1906,  1916,
    1923,  1932,  1932,  1932,  1945,  1946,  1947,  1948,  1949,  1963,
    1969,  1973,  1977,  1982,  1987,  1992,  1997,  2001,  2005,  2010,
    2014,  2019,  2019,  2019,  2025,  2032,  2032,  2032,  2037,  2037,
    2037,  2043,  2043,  2043,  2048,  2052,  2052,  2052,  2057,  2057,
    2057,  2066,  2070,  2070,  2070,  2075,  2075,  2075,  2084,  2088,
    2088,  2088,  2093,  2093,  2093,  2102,  2102,  2102,  2108,  2108,
    2108,  2117,  2117,  2132,  2132,  2152,  2153,  2154,  2155,  2159,
    2166,  2173,  2179,  2188,  2193,  2200,  2201,  2205,  2211,  2218,
    2226,  2233,  2241,  2253,  2256,  2270,  2279,  2280,  2284,  2289,
    2296,  2301,  2311,  2316,  2323,  2335,  2336,  2340,  2343
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
  "\"||=\"", "\"^^=\"", "\"..\"", "\"[[\"", "\"[{\"", "\"{{\"",
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
  "function_declaration", "expression_block", "expression_any",
  "expressions", "expr_pipe", "name_in_namespace", "expression_delete",
  "expr_new", "expression_break", "expression_continue",
  "expression_return", "expression_yield", "expression_try_catch",
  "kwd_let", "expression_let", "expr_cast", "$@3", "$@4", "$@5", "$@6",
  "$@7", "$@8", "expr_type_decl", "$@9", "$@10", "expr_type_info",
  "expr_list", "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_expr", "$@11", "$@12",
  "$@13", "$@14", "expr_field", "$@15", "$@16", "expr", "$@17", "$@18",
  "optional_field_annotation", "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@19", "function_argument_declaration", "function_argument_list",
  "tuple_type", "tuple_type_list", "variant_type", "variant_type_list",
  "copy_or_move", "variable_declaration", "copy_or_move_or_clone",
  "optional_ref", "let_variable_declaration",
  "global_variable_declaration_list", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@20", "enum_list",
  "single_alias", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@21", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@22", "$@23", "type_declaration", "$@24",
  "$@25", "$@26", "$@27", "$@28", "$@29", "$@30", "$@31", "$@32", "$@33",
  "$@34", "$@35", "$@36", "$@37", "$@38", "$@39", "$@40", "$@41", "$@42",
  "$@43", "$@44", "$@45", "$@46", "$@47", "variant_alias_declaration",
  "$@48", "bitfield_alias_declaration", "$@49", "make_decl",
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
     405,   406,   407,   408,   409,   410,   411,    44,    61,    63,
      58,   124,    94,    38,    60,    62,    45,    43,    42,    47,
      37,   412,   413,   126,    33,   414,   415,   416,   417,   418,
      46,    91,    93,    40,    41,    36,    64,    59,   123,   125,
      35
};
#endif

#define YYPACT_NINF (-571)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-533)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -571,    25,  -571,  -571,    26,   -74,   -84,     2,  -571,   -98,
    -571,  -571,    17,  -571,  -571,  -571,  -571,  -571,   252,  -571,
     107,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,
      88,  -571,   -77,    93,   103,  -571,  -571,   -84,    27,  -571,
     152,   195,  -571,  -571,  -571,   107,   196,   219,  -571,  -571,
     220,   250,   203,  -571,    90,  -571,  -571,  -571,    91,   245,
     317,  -571,   324,    15,    26,   253,   -74,   226,   265,  -571,
     279,   286,  -571,  7656,   256,   -94,   328,   241,   257,  -571,
     291,    26,    17,  -571,  -571,  -571,    12,  -571,  -571,  -571,
     295,  -571,  -571,   349,  -571,  -571,   259,  -571,  -571,  -571,
    -571,  -571,    51,    77,  -571,  -571,  -571,  -571,   393,  -571,
    -571,   287,   290,   299,   300,  -571,  -571,  -571,   266,  -571,
    -571,  -571,  -571,  -571,   301,  -571,  -571,  -571,  -571,  -571,
    -571,  -571,  -571,  -571,   302,  -571,  -571,  -571,   303,   304,
    -571,  -571,  -571,  -571,   305,   307,  -571,  -571,  -571,  -571,
    -571,  2421,  -571,  -571,   263,  -571,  -571,  -571,   306,   330,
    -571,     3,  -571,  7270,   443,   444,  -571,   297,   293,    34,
    -571,   334,  -571,  -571,   344,  -571,  -571,   285,  -571,  -571,
    -571,  -571,  -571,  -571,  -571,    60,  -571,  -571,  -571,  -571,
    -571,   347,  -571,   149,   174,   176,  -571,  -571,  -571,  -571,
    -571,  -571,   461,  -571,  -571,     5,  2836,  -571,  -571,  -571,
     311,   339,  -571,   313,  -571,  -571,  -571,  -571,  -571,  -571,
    -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,
    -571,  -571,  -571,  -571,  -571,   320,  -571,  -571,  -571,  -571,
    -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,
    -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,
    -571,   321,  -571,  -571,   -91,   345,  -571,  -571,   284,  -571,
     346,   316,   -93,    26,   360,  -571,  -571,  -571,    77,  -571,
    7656,  7656,  7656,  7656,   332,   306,  7656,   297,  7656,   297,
    7656,   297,  7742,   330,  -571,  -571,  -571,   335,  -571,  -571,
    -571,  7570,   336,   348,  -571,   337,   357,   358,   340,   368,
     341,  -571,   370,  4515,  4515,  7398,  7484,  4515,  -571,  -571,
    -571,  -571,  -571,  -571,  -571,  4515,  4515,  4515,   -64,  4515,
    4515,  -571,  4515,  -571,   350,  -571,  -571,  -571,   -89,  -571,
    -571,  -571,  -571,   354,  -571,  -571,  -571,  -571,  -571,  -571,
    5976,   355,  -571,  -571,  -571,  -571,  -571,   -82,  7656,   -52,
    -571,  -571,  -571,    -5,  -571,   153,  7656,  -571,  1141,  -571,
    -571,  -571,  -571,  -571,  -571,   351,   -47,   -64,  -571,  -571,
    -571,   360,   104,   500,  -571,   164,  -571,   580,   919,   744,
    1757,  -571,   -99,  1815,   345,  1917,   345,  2201,   345,   106,
    -571,   166,  2421,   184,  -571,   936,  -571,   -64,  -571,  4515,
    -571,  -571,  4515,  -571,  4515,  7656,   272,   272,   360,  1494,
     360,  1655,  6547,  -571,   108,   280,   272,   272,   -57,  -571,
     272,   272,  5344,   109,  -571,  2261,   403,     8,   395,  4515,
    4515,  -571,  -571,  4515,  4515,  4515,  4515,   396,  4515,   397,
    4515,  4515,  4515,  4515,  4515,  4515,  4515,  4515,  4515,  2974,
    4515,  4515,  4515,  4515,  4515,  4515,  4515,  4515,  4515,  4515,
     162,  4515,  -571,  3112,  -571,  -571,  2421,  -571,  -571,   360,
    -571,   -46,  -571,   344,  2421,  -571,  4515,  -571,  -571,   360,
    1977,  -571,   293,  4515,  4515,   399,  -571,   364,   410,  3273,
      21,  2123,   230,   230,  -571,   518,   371,   372,  4515,  -571,
    -571,  -571,  -571,  -571,  -571,  -571,  -571,   373,   374,   375,
    -571,  -571,  -571,   360,  -571,   -83,  -571,  6478,  -571,   -55,
    -571,  -571,    16,  -571,   383,   406,   408,  7656,  -571,   -42,
    -571,  7656,  -571,  -571,  -571,   417,  -571,   402,  -571,   405,
    -571,   407,  7656,  -571,  7742,  -571,   330,  3411,  3572,  7656,
    5423,  7656,  7656,  5502,  7656,  5581,  2360,    14,   230,   -43,
    1816,  3733,  6547,   411,   -15,   400,   420,  -571,  -571,   243,
      19,  3894,   -15,   237,  4515,  4515,   390,  -571,  4515,   177,
     430,  -571,   244,  -571,   434,  -571,    95,  6780,   125,   297,
     421,  -571,  -571,  1430,  1430,   598,   598,   292,   292,   401,
     242,  -571,  6053,   -11,   -11,  1430,  1430,  7082,   822,  7000,
    6849,   437,  6629,   691,  7109,  7185,   598,   598,   455,   455,
     242,   242,   242,   438,  4515,  -571,   439,  4515,   588,  6130,
    -571,   110,  -571,  -571,  -571,  7656,  -571,  2422,  -571,   900,
      23,  2422,  -571,  -571,   468,  4606,   578,  6780,   900,   435,
    -571,   432,   452,  6780,  -571,  2422,  -571,  4708,   412,  -571,
    -571,   409,  -571,  -571,   900,  -571,  -571,  -571,  -571,   230,
    -571,  4515,  4515,  4515,  4515,  4515,  4515,  4515,  4515,  4515,
    4515,  2560,  4515,  4515,  4515,  4515,  4515,  4515,  2698,    39,
    4515,  -571,    17,  -571,   589,    28,   413,  -571,  -571,   558,
    1082,  -571,  -571,  -571,  2422,  -571,  2500,  -571,  -571,  -571,
    -571,  -571,  -571,  -571,  -571,  -571,  2421,  -571,  -571,  -571,
    -571,   118,   453,  5660,  2638,  -571,  2776,  2914,  -571,  3052,
    -571,   403,  4515,  -571,  4515,  4515,     1,   454,   434,   419,
     422,  4515,   423,  4515,  4515,   434,   418,   424,  6780,  -571,
    -571,  6698,  -571,   457,  7656,   297,   -36,   141,  4515,  -571,
     460,   462,   465,   467,  -571,   144,   345,  -571,  4055,  -571,
    -571,  4515,  -571,  6207,  -571,  6284,  -571,  -571,  -571,  1274,
    -571,  6375,  -571,  4515,  -571,  4810,  4515,  4515,  -571,   293,
    -571,  -571,  -571,  -571,  -571,  4912,  -571,  -571,  -571,   232,
    -571,  6780,  6780,  6780,  6780,  6780,  6780,  6780,  6780,  6780,
    6780,  4515,  4515,  6780,  6780,  6780,  6780,  6780,  6780,  6780,
    4515,  4515,  6780,  -571,  5192,   148,   329,  -571,  -571,   329,
    -571,   472,   461,  -571,  2422,  -571,  5014,  -571,  -571,  -571,
    -571,  -571,   190,  -571,  -571,  -571,  -571,  -571,   445,   -56,
    6780,  6780,   -15,   447,   119,   411,   448,  -571,  6780,  -571,
    -571,   -44,   -15,   449,  -571,  -571,  -571,  3213,   345,   433,
    6780,  -571,  -571,  -571,  -571,   125,   466,   -78,  7656,  -571,
     133,  6931,  -571,  -571,  -571,   461,  4515,     6,  -571,  -571,
    4515,  -571,  1330,   293,  -571,  -571,  4515,  -571,    20,  6780,
      20,  6780,  -571,   440,  -571,  -571,    32,   360,  -571,  -571,
    5116,  -571,  -571,  4515,   477,  -571,  4515,  4515,  4515,  4216,
    4515,   489,   491,  4515,  4515,  -571,  4515,   485,  -571,  -571,
     469,  -571,  -571,  -571,  4377,  -571,  -571,  3243,  -571,  6780,
     293,  6780,  -571,  -571,   900,  -571,  -571,   605,  -571,  -571,
    5739,   478,   598,   598,   598,  -571,  5818,  5268,   493,  -571,
    6780,  6780,  5268,   494,   -64,  -571,  4515,  6849,  -571,  -571,
     232,    12,  -571,    12,  -571,  4515,  -571,   591,   495,  -571,
     490,  -571,  -571,   -64,  6931,  -571,  -571,   463,   293,  5897,
    4515,   496,   498,  -571,  -571,  -571,  -571,  6780,  -571,  -571
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,    94,     1,   205,     0,     0,     0,     0,   206,     0,
     543,   541,     0,    14,     3,    10,     9,     8,     0,     7,
     408,     6,    11,     5,     4,    12,    13,    75,    76,    74,
      83,    85,    34,    47,    44,    45,    36,     0,    42,    35,
       0,     0,   423,    19,    18,   408,     0,     0,    88,    89,
       0,   185,    90,    92,     0,    87,   435,   434,   152,   424,
     436,   409,   410,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,     0,   187,
       0,     0,     0,    95,   154,   153,     0,   151,   426,   425,
       0,   438,   437,   431,   412,   411,   414,    81,    82,    79,
      80,    78,     0,     0,    77,    86,    48,    46,    42,    39,
      38,     0,     0,     0,     0,   445,   465,   446,   477,   447,
     451,   452,   453,   454,   469,   458,   459,   460,   461,   462,
     463,   464,   466,   467,   514,   450,   457,   468,   521,   528,
     448,   455,   449,   456,     0,     0,   476,   484,   487,   485,
     486,   419,   420,   422,     0,    16,    17,    20,     0,     0,
     186,     0,    93,     0,     0,     0,   101,    96,     0,     0,
     432,     0,   439,   406,   360,    21,    22,     0,    70,    71,
      68,    69,    67,    66,    72,     0,    41,   505,   508,   511,
     501,     0,   481,   515,   522,   529,   535,   538,   492,   497,
     491,   504,     0,   500,   494,     0,     0,   496,   421,   479,
       0,     0,   386,     0,    91,   150,   104,   105,   107,   106,
     108,   109,   110,   111,   137,   138,   135,   136,   128,   139,
     140,   129,   126,   127,   148,     0,   149,   141,   142,   143,
     144,   115,   116,   117,   112,   113,   114,   125,   131,   132,
     130,   123,   124,   119,   118,   120,   121,   122,   103,   102,
     147,     0,   133,   134,   360,    99,   179,   156,     0,   416,
     429,     0,   360,     0,     0,    23,    24,    25,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    96,     0,    96,
       0,    96,     0,     0,   499,   493,   495,     0,   498,   291,
     292,     0,     0,     0,   285,     0,     0,     0,     0,     0,
       0,   469,     0,     0,     0,     0,     0,     0,   241,   243,
     242,   244,   245,   246,    26,     0,     0,     0,     0,     0,
       0,   489,     0,   228,   229,   289,   288,   239,   286,   352,
     351,   350,   349,    94,   355,   287,   354,   353,   331,   293,
       0,     0,   290,   545,   546,   547,   548,     0,     0,     0,
     146,   145,    97,     0,   379,     0,     0,   155,     0,   470,
     473,   471,   474,   472,   475,     0,     0,     0,   433,   371,
     413,     0,     0,   441,   415,   399,    73,     0,     0,     0,
       0,   478,     0,     0,    99,     0,    99,     0,    99,   185,
     383,     0,   381,     0,   490,   189,   192,     0,   217,     0,
     208,   211,     0,   214,     0,     0,   320,   321,     0,   555,
       0,     0,     0,   572,     0,     0,   297,   296,   336,    32,
     295,   294,     0,   230,   358,     0,   237,     0,     0,     0,
       0,   322,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     282,     0,   488,     0,   480,   544,   385,   542,   387,     0,
     377,   390,    98,   360,   100,   181,     0,    57,    58,     0,
       0,   193,     0,     0,     0,     0,   194,     0,     0,     0,
       0,     0,     0,     0,   159,   157,     0,     0,     0,   174,
     169,   166,   165,   167,   168,   180,   160,     0,     0,     0,
     172,   173,   175,     0,   164,     0,   161,   247,   416,     0,
     427,   430,   360,   407,     0,     0,     0,     0,   400,     0,
     506,     0,   512,   502,   482,     0,   516,     0,   523,     0,
     530,     0,     0,   536,     0,   539,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   185,
       0,     0,   563,   553,   555,     0,   566,   567,   568,     0,
       0,     0,   555,     0,     0,     0,     0,    29,     0,    27,
       0,   324,   274,   273,     0,   332,     0,   223,     0,    96,
       0,   346,   347,   298,   299,   311,   312,   309,   310,     0,
     341,   329,     0,   356,   357,   300,   301,   316,   317,   318,
     319,     0,     0,   314,   315,   313,   307,   308,   303,   302,
     304,   305,   306,     0,     0,   280,     0,     0,     0,     0,
     334,     0,   378,   389,   388,     0,   391,     0,   380,     0,
       0,     0,   195,   198,     0,   247,     0,   188,     0,     0,
     178,     0,     0,    51,    61,     0,   202,   247,   229,   183,
     184,     0,   176,   177,     0,   163,   170,   171,   207,     0,
     162,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   417,     0,   440,     0,   362,     0,   361,   442,   443,
       0,   397,   398,   396,     0,   507,     0,   513,   503,   483,
     517,   519,   524,   526,   531,   533,   382,   537,   384,   540,
     190,     0,     0,     0,     0,   337,     0,     0,   338,     0,
     359,   237,     0,   556,     0,     0,   555,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   565,   573,
     574,     0,    30,    33,     0,    96,     0,     0,     0,   333,
       0,     0,     0,     0,   235,     0,    99,   343,     0,   327,
     348,     0,   330,     0,   281,     0,   283,   325,   335,   392,
     395,   394,    62,     0,   199,   247,     0,     0,   196,     0,
      63,    64,    49,    50,   203,   247,   200,   230,   179,    54,
     182,   257,   258,   260,   259,   261,   251,   252,   253,   262,
     263,     0,     0,   249,   250,   264,   265,   254,   255,   256,
       0,     0,   248,   428,     0,     0,   367,   363,   364,   367,
     372,     0,   396,   401,     0,   405,   247,   509,   520,   527,
     534,   191,     0,   220,   218,   209,   212,   215,     0,     0,
     550,   549,   555,     0,     0,   554,     0,   558,   564,   570,
     569,     0,   555,     0,   571,    28,    31,     0,    99,     0,
     224,   233,   234,   232,   231,     0,     0,     0,     0,   271,
       0,   342,   328,   326,   284,   388,     0,     0,   197,   204,
       0,   201,     0,     0,    52,    53,     0,    59,   268,   269,
     266,   267,   418,    95,   368,   369,   362,     0,   444,   403,
     247,   404,   510,     0,     0,   219,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   557,     0,     0,   561,   275,
       0,   270,   236,   238,     0,   225,   240,     0,   272,   393,
       0,    65,   158,    55,     0,   376,   373,   365,   370,   402,
       0,     0,   210,   213,   216,   339,     0,   575,     0,   559,
     552,   551,   575,     0,     0,   278,     0,   226,   344,    60,
      54,     0,   366,     0,   221,     0,   340,     0,     0,   560,
       0,   562,   276,     0,   227,   345,    56,     0,     0,     0,
       0,     0,     0,   279,   374,   375,   222,   576,   577,   578
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -571,  -571,  -571,  -571,  -571,   258,   618,  -571,  -571,  -571,
    -571,  -571,  -571,  -571,   645,  -571,   576,  -571,  -571,   619,
    -571,  -571,  -571,  -294,  -571,  -571,  -571,  -571,  -571,  -571,
    -571,  -571,   414,  -571,  -571,   624,   -63,  -571,   607,    -9,
    -324,  -277,  -389,  -571,  -571,  -571,  -542,  -571,  -167,  -571,
    -112,  -474,   -12,  -571,  -571,  -571,  -571,  -571,  -571,  -571,
     698,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,
    -571,  -571,  -571,  -458,  -571,  -571,  -184,  -571,   -38,  -468,
    -571,  -365,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,
    -571,  -571,  -571,   201,  -571,  -571,  -166,  -208,  -571,  -129,
    -571,  -571,  -571,   238,  -571,   161,  -571,  -338,   429,  -470,
    -475,    18,  -571,  -349,  -571,   679,  -571,  -571,  -571,   199,
     158,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,
    -571,  -571,  -361,   -67,  -571,  -571,  -571,   446,  -571,  -571,
    -571,   -51,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,
    -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,
    -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,  -571,
     428,  -570,  -408,  -554,  -571,  -571,  -300,   -23,   312,  -571,
    -571,  -571,  -240,  -571
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   157,    45,    14,   177,   183,   425,   335,   763,
     336,   590,    15,    16,    38,    39,    72,    17,    34,    35,
     506,   507,   906,   907,   508,   509,   510,   511,   512,   513,
     514,   900,   184,   185,    30,    31,    32,    52,    53,    54,
      18,   265,   367,   167,    19,    86,   168,    87,   337,   515,
     368,   516,   338,   517,   339,   518,   519,   520,   521,   522,
     523,   524,   340,   561,   926,   562,   927,   564,   928,   341,
     559,   925,   342,   596,   946,   343,   774,   775,   599,   344,
     345,   654,   526,   346,   347,   348,   764,   974,   765,   993,
     349,   638,   894,   597,   888,   995,   363,   839,   983,   916,
     706,   532,   981,   364,   365,   400,   401,   212,   213,   745,
     480,   714,   539,   384,   272,    62,    96,    21,   174,   376,
      42,    75,    22,    90,    23,   378,   171,   172,    60,    93,
      24,   271,   385,   351,   375,   148,   149,   210,   150,   285,
     719,   402,   283,   718,   280,   715,   281,   922,   282,   717,
     286,   720,   287,   848,   288,   722,   289,   849,   290,   724,
     291,   850,   292,   727,   293,   729,    25,    47,    26,    46,
     352,   573,   574,   575,   353,   576,   577,   578,   579,   354,
     424,   355,   988,   356
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   267,   481,   525,   642,   547,   147,   549,   274,   551,
     394,   647,   396,   582,   398,   641,   653,   423,   161,   436,
     749,   478,   151,   -94,   767,     2,   600,   666,   756,    97,
      98,     3,   533,   742,   669,   670,   568,   295,   754,   679,
     437,   438,   793,   479,    27,    28,   163,   273,    68,   273,
      48,    43,   568,   944,     4,    40,     5,   567,     6,   580,
     434,   146,   449,   450,     7,    36,   544,   474,    50,    33,
      55,   643,   837,     8,   643,   711,   837,   956,    49,     9,
      64,   643,    69,   744,   712,    51,    37,    44,   545,    80,
     744,   178,   179,   362,   435,   153,   380,   211,    10,   731,
     743,   768,   529,   700,   680,   447,   381,   475,   449,   450,
     266,   536,   644,   768,   645,   644,   713,   646,   481,    11,
     164,   165,   644,   470,   471,   838,   439,   440,   650,   838,
     443,   930,   701,   444,   445,   446,   447,   477,   448,   449,
     450,   451,   530,   936,   383,   452,    84,   453,   454,    50,
     766,    40,    85,   455,   456,   457,   273,   601,    99,   458,
      64,   166,   100,   768,   101,   102,    51,    61,   296,   470,
     471,   536,   748,   790,   678,    29,   536,   794,   865,   459,
     536,   460,   461,   462,   463,   464,   297,   214,   529,   468,
      41,   804,   863,   950,   268,   298,    70,   702,   103,    74,
     470,   471,   175,   176,   414,   703,    12,    71,   704,   266,
     382,   810,    13,   147,   147,   147,   147,   278,   478,   147,
     180,   147,   269,   147,   181,   147,   182,   102,   833,   387,
     388,   389,   390,   154,   147,   393,   643,   395,    80,   397,
     845,    50,   770,   903,   279,   933,    63,    82,   147,   147,
     405,   771,   768,    65,   904,   905,    56,    57,    51,    58,
      66,    64,   437,   438,   419,   421,   552,   768,   146,   146,
     146,   146,    83,   592,   146,   768,   146,   644,   146,   769,
     146,   633,   634,   772,   859,   759,   534,    59,   773,   146,
     768,   147,   437,   438,   788,   585,   871,   586,   747,   147,
      88,   885,   851,   146,   146,    82,    89,   476,   931,  -518,
      73,   635,   437,   438,  -518,   484,   429,   948,   937,   896,
     890,   536,   776,   879,   537,   656,   886,   538,   275,   276,
     913,   553,  -518,   664,  -525,   897,  -532,   482,   862,  -525,
     483,  -532,   636,   637,    40,    77,   146,   872,   147,   555,
     441,   442,   369,   554,   146,   923,   370,  -525,   447,  -532,
     448,   449,   450,   451,   566,   531,   705,   452,    78,    79,
     919,   556,    91,   371,   372,   373,   374,   924,    92,    94,
     441,   442,    80,   155,   914,    95,    81,   887,   447,   156,
     915,   449,   450,   451,   934,   558,   106,   452,   439,   440,
     441,   442,   443,   146,  -277,   444,    71,   350,   447,  -277,
     448,   449,   450,   451,   108,   333,   668,   452,   266,   453,
     454,   593,   470,   471,   753,   752,   757,  -277,   109,   158,
     753,   175,   176,   587,   588,   110,   275,   276,   277,   997,
     160,   998,   958,   152,   169,   159,   170,   173,    69,   191,
     208,   187,   470,   471,   188,   209,   463,   464,   465,   466,
     467,   468,   469,   189,   190,   192,   193,   194,   195,   196,
     147,   197,   470,   471,   147,   437,   438,   262,   263,   211,
     264,   266,   792,   270,   273,   147,   710,   147,   878,   940,
     716,   800,   147,   294,   147,   147,   284,   147,   357,   358,
     359,   726,   360,   361,   379,   366,   377,   809,   734,   383,
     736,   737,   408,   739,   416,   417,   391,   404,   422,   407,
     409,   410,   411,   412,   414,   146,   426,   427,   428,   146,
     430,   431,   413,   432,   415,    12,   433,   525,   473,   528,
     146,   535,   146,   598,   602,   609,   611,   146,   659,   146,
     146,   660,   146,   661,   671,   708,   481,   709,   672,   673,
     675,   676,   677,   441,   442,   707,   474,   721,   747,   527,
     723,   447,   725,   448,   449,   450,   451,   751,   147,   760,
     452,   762,   750,   766,   778,   777,   780,   782,   784,   786,
     679,   799,   802,   801,   789,   803,   836,   808,   807,   841,
     840,   866,   852,   864,   867,   869,   874,   873,   876,   881,
     560,   882,   198,   563,   883,   565,   884,   941,   437,   438,
     572,   918,   572,   467,   468,   469,   961,   955,   929,   932,
     935,   938,   899,   146,   975,   470,   471,   982,   199,   200,
     603,   604,  1000,   985,   605,   606,   607,   608,   943,   610,
    1004,   612,   613,   614,   615,   616,   617,   618,   619,   620,
     622,   623,   624,   625,   626,   627,   628,   629,   630,   631,
     632,   968,   639,   969,   973,   989,   991,  1001,  1008,  1002,
    1009,   104,    67,   589,   186,   107,   996,   649,   105,   162,
      55,   655,   386,   835,   657,   658,   902,   147,   201,    20,
     663,   942,   667,   858,   439,   440,   441,   442,   957,   674,
     917,   437,   438,   877,   447,   728,   448,   449,   450,   451,
     945,   648,   403,   452,    76,   453,   454,   699,   844,   406,
     870,   392,   990,   583,     0,     0,   953,     0,   202,   203,
       0,     0,     0,   204,     0,   540,   205,     0,     0,     0,
       0,     0,   146,     0,     0,     0,     0,     0,     0,   733,
       0,   206,     0,     0,   465,   466,   467,   468,   469,     0,
     207,   426,   432,     0,     0,     0,   198,     0,   470,   471,
       0,     0,   432,   979,     0,   758,   422,   980,     0,   761,
       0,     0,     0,     0,     0,     0,     0,   439,   440,   441,
     442,   443,   199,   200,   444,   445,   446,   447,     0,   448,
     449,   450,   451,     0,     0,     0,   452,     0,   453,   454,
       0,   147,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1005,     0,     0,     0,   783,     0,   947,   785,     0,
       0,     0,   437,   438,     0,     0,     0,     0,   791,     0,
       0,     0,   795,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   201,     0,     0,     0,   805,     0,     0,     0,
       0,   470,   471,     0,     0,     0,   146,     0,     0,     0,
       0,     0,   811,   812,   813,   814,   815,   816,   817,   818,
     819,   820,   823,   824,   825,   826,   827,   828,   829,   832,
       0,   834,   202,   203,     0,     0,     0,   204,     0,   542,
     205,     0,     0,     0,     0,   846,     0,     0,     0,     0,
     437,   438,     0,     0,     0,   206,     0,     0,   439,   440,
     441,   442,   443,     0,   207,   444,   445,   446,   447,     0,
     448,   449,   450,   451,     0,   860,   861,   452,     0,   453,
     454,   198,   868,     0,   572,   455,     0,   457,     0,     0,
       0,     0,   992,     0,     0,     0,     0,     0,   198,   880,
       0,     0,     0,     0,     0,     0,     0,   199,   200,     0,
       0,  1003,   891,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,     0,   199,   200,     0,   823,   832,     0,
       0,     0,   470,   471,     0,     0,   439,   440,   441,   442,
     443,     0,     0,   444,   445,   446,   447,     0,   448,   449,
     450,   451,   908,   909,     0,   452,     0,   453,   454,     0,
       0,   910,   911,   455,   456,   457,     0,   201,     0,   458,
       0,     0,     0,     0,     0,   920,     0,     0,     0,     0,
       0,     0,     0,     0,   201,     0,     0,     0,     0,   459,
       0,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,     0,     0,     0,     0,     0,     0,   202,   203,     0,
     470,   471,   204,     0,     0,   205,     0,     0,   266,     0,
       0,     0,     0,     0,   202,   203,     0,   949,     0,   204,
     206,   951,   205,   527,     0,     0,   541,   954,     0,   207,
       0,     0,     0,     0,   198,     0,     0,   206,     0,   557,
       0,     0,     0,     0,   960,     0,   207,   962,   963,   964,
     966,   967,     0,     0,   970,   971,     0,   972,     0,     0,
     199,   200,   485,     0,     0,   977,     0,     3,     0,   486,
     487,   488,     0,   489,     0,   299,   300,   301,   302,   303,
       0,     0,     0,     0,     0,     0,   490,   304,   491,   492,
       0,     0,     0,     0,     0,     0,     0,   994,   493,   305,
       0,   494,     0,   495,   306,     0,   999,   307,     0,     8,
     308,   496,     0,   497,   309,     0,     0,   498,   499,   711,
     201,  1007,     0,     0,   500,   115,   116,   117,   712,   119,
     120,   121,   122,   123,   311,   125,   126,   127,   128,   129,
     130,   131,   132,   133,     0,   135,   136,   137,     0,     0,
     140,   141,   142,   143,     0,     0,   312,   501,     0,     0,
     842,   203,     0,     0,     0,   204,     0,     0,   205,   313,
     314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   206,   502,   503,     0,     0,     0,   843,
       0,     0,   207,    50,     0,     0,     0,     0,     0,     0,
       0,   315,   316,   317,   318,   319,   320,   321,   322,   323,
      51,   324,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   198,   325,   326,   327,
       0,   328,     0,     0,   329,   330,     0,     0,     0,     0,
       0,     0,     0,     0,   332,     0,   333,   334,   504,   266,
     505,   485,   199,   200,     0,     0,     3,     0,   486,   487,
     488,     0,   489,     0,   299,   300,   301,   302,   303,     0,
       0,     0,     0,     0,     0,   490,   304,   491,   492,     0,
       0,     0,     0,     0,     0,     0,     0,   493,   305,     0,
     494,     0,   495,   306,     0,     0,   307,     0,     8,   308,
     496,     0,   497,   309,     0,     0,   498,   499,     0,     0,
       0,   643,   201,   500,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   311,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   312,   501,     0,     0,     0,
       0,     0,   895,   203,     0,     0,     0,   204,   313,   314,
     205,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   502,   503,   206,     0,     0,     0,     0,
       0,     0,    50,     0,   207,     0,     0,     0,     0,     0,
     315,   316,   317,   318,   319,   320,   321,   322,   323,    51,
     324,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   325,   326,   327,     0,
     328,     0,     0,   329,   330,     0,     0,     0,   299,   300,
     301,   302,   303,   332,     0,   333,   334,   504,   266,   952,
     304,     0,     0,     0,     0,     0,   198,     0,     0,     0,
       0,     0,   305,     0,     0,     0,     0,   306,   441,   442,
     307,     0,     0,   308,     0,   568,   447,   309,   448,   449,
     450,   451,   199,   200,     0,   452,     0,   310,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   311,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   312,
       0,     0,     0,     0,     0,     0,   465,   466,   467,   468,
     469,     0,   313,   314,     0,     0,     0,     0,     0,     0,
     470,   471,   201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   569,   324,     0,     0,     0,     0,     0,
       0,     0,   202,   203,     0,     0,     0,   204,     0,     0,
     570,   326,   327,     0,   328,     0,     0,   329,   330,   299,
     300,   301,   302,   303,     0,   206,     0,   571,     0,   333,
     334,   304,   266,     0,   207,     0,     0,   198,     0,     0,
       0,     0,     0,   305,     0,     0,     0,     0,   306,     0,
       0,   307,     0,     0,   308,     0,     0,     0,   309,     0,
       0,     0,     0,   199,   200,     0,     0,     0,   310,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   311,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   314,     0,     0,     0,     0,     0,
       0,     0,     0,   201,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,   198,
       0,     0,     0,     0,     0,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   569,   324,     0,     0,     0,     0,
       0,     0,     0,   202,   203,   199,   200,     0,   204,     0,
       0,   570,   326,   327,     0,   328,     0,     0,   329,   330,
     299,   300,   301,   302,   303,     0,   206,     0,   581,     0,
     333,   334,   304,   266,     0,   207,     0,   198,   295,     0,
       0,     0,     0,     0,   305,     0,     0,     0,     0,   306,
       0,     0,   307,     0,     0,   308,     0,     0,     0,   309,
       0,     0,     0,   199,   200,   201,     0,     0,     0,   310,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   311,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   312,     0,     0,     0,   202,   203,     0,     0,     0,
     204,     0,   543,   205,   313,   314,     0,     0,     0,     0,
       0,     0,     0,   201,     0,     0,     0,     0,   206,     0,
       0,     0,     0,     0,     0,     0,     0,   207,    50,   198,
       0,     0,     0,     0,     0,     0,   315,   316,   317,   318,
     319,   320,   321,   322,   323,    51,   324,     0,     0,     0,
       0,     0,     0,   202,   203,   199,   200,     0,   204,   296,
     546,   205,   325,   326,   327,     0,   328,     0,     0,   329,
     330,   299,   300,   301,   302,   303,   206,   297,     0,   332,
       0,   333,   334,   304,   266,   207,   298,     0,     0,     0,
       0,     0,     0,     0,     0,   305,     0,     0,     0,     0,
     306,     0,     0,   307,     0,     0,   308,     0,     0,     0,
     309,     0,     0,     0,     0,   201,     0,     0,     0,     0,
     310,   115,   116,   117,     0,   119,   120,   121,   122,   123,
     311,   125,   126,   127,   128,   129,   130,   131,   132,   133,
       0,   135,   136,   137,     0,     0,   140,   141,   142,   143,
       0,     0,   312,     0,     0,   202,   203,     0,     0,     0,
     204,     0,   548,   205,     0,   313,   314,     0,     0,     0,
       0,     0,     0,     0,   651,     0,     0,     0,   206,     0,
     502,   503,     0,     0,     0,     0,     0,   207,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   315,   316,   317,
     318,   319,   320,   321,   322,   323,    51,   324,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   299,   300,   301,
     302,   303,     0,   325,   326,   327,     0,   328,     0,   304,
     329,   330,     0,     0,     0,     0,     0,     0,     0,     0,
     332,   305,   333,   334,   652,   266,   306,     0,     0,   307,
       0,     0,   308,     0,     0,     0,   309,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   310,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   311,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,     0,     0,   312,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,   314,   198,     0,     0,     0,     0,     0,     0,
     665,     0,     0,     0,     0,     0,   502,   503,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,   199,
     200,     0,     0,   315,   316,   317,   318,   319,   320,   321,
     322,   323,    51,   324,     0,   299,   300,   301,   302,   303,
       0,     0,     0,     0,     0,     0,     0,   304,     0,   325,
     326,   327,     0,   328,     0,     0,   329,   330,     0,   305,
       0,     0,     0,     0,   306,     0,   332,   307,   333,   334,
     308,   266,     0,     0,   309,     0,     0,     0,     0,   201,
       0,     0,     0,     0,   310,   115,   116,   117,     0,   119,
     120,   121,   122,   123,   311,   125,   126,   127,   128,   129,
     130,   131,   132,   133,     0,   135,   136,   137,     0,     0,
     140,   141,   142,   143,     0,     0,   312,     0,     0,   202,
     203,     0,     0,     0,   204,     0,   550,   205,     0,   313,
     314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   206,     0,     0,     0,     0,     0,     0,     0,
       0,   207,   198,    50,     0,     0,     0,     0,     0,     0,
       0,   315,   316,   317,   318,   319,   320,   321,   322,   323,
      51,   324,     0,     0,     0,     0,     0,     0,   199,   200,
       0,     0,     0,     0,     0,     0,     0,   325,   326,   327,
       0,   328,     0,     0,   329,   330,   299,   300,   301,   302,
     303,     0,   594,     0,   332,   595,   333,   334,   304,   266,
       0,     0,     0,   198,     0,     0,     0,     0,     0,     0,
     305,     0,     0,     0,     0,   306,     0,     0,   307,     0,
       0,   308,     0,     0,     0,   309,     0,     0,   201,   199,
     200,     0,     0,     0,     0,   310,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   311,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,     0,     0,   312,   202,   203,
       0,     0,     0,   204,     0,   741,   205,     0,     0,     0,
     313,   314,   198,     0,     0,     0,     0,     0,     0,   201,
       0,   206,     0,     0,     0,   502,   503,     0,     0,     0,
     207,     0,     0,     0,    50,     0,     0,     0,   199,   200,
       0,     0,   315,   316,   317,   318,   319,   320,   321,   322,
     323,    51,   324,     0,   299,   300,   301,   302,   303,   202,
     203,     0,     0,     0,   204,     0,   304,   205,   325,   326,
     327,     0,   328,     0,     0,   329,   330,     0,   305,     0,
       0,     0,   206,   306,     0,   332,   307,   333,   334,   308,
     266,   207,     0,   309,     0,     0,     0,     0,   201,     0,
       0,     0,     0,   310,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   311,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   312,     0,     0,   202,   203,
       0,     0,     0,   204,     0,   847,   205,     0,   313,   314,
     198,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   206,     0,   821,   822,     0,     0,     0,     0,     0,
     207,     0,    50,     0,     0,     0,   199,   200,     0,     0,
     315,   316,   317,   318,   319,   320,   321,   322,   323,    51,
     324,     0,   299,   300,   301,   302,   303,     0,     0,     0,
       0,     0,     0,     0,   304,     0,   325,   326,   327,     0,
     328,     0,     0,   329,   330,     0,   305,     0,     0,     0,
       0,   306,     0,   332,   307,   333,   334,   308,   266,     0,
       0,   309,     0,     0,     0,     0,   201,     0,     0,     0,
       0,   310,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   311,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,     0,     0,   312,     0,     0,   202,   203,     0,     0,
       0,   204,     0,   854,   205,     0,   313,   314,   198,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   206,
       0,   830,   831,     0,     0,     0,     0,     0,   207,     0,
      50,     0,     0,     0,   199,   200,     0,     0,   315,   316,
     317,   318,   319,   320,   321,   322,   323,    51,   324,     0,
     299,   300,   301,   302,   303,     0,     0,     0,     0,     0,
       0,     0,   304,     0,   325,   326,   327,     0,   328,     0,
       0,   329,   330,     0,   305,     0,     0,     0,     0,   306,
       0,   332,   307,   333,   334,   308,   266,     0,     0,   309,
       0,     0,     0,     0,   201,     0,     0,     0,     0,   310,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   311,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   312,     0,     0,   202,   203,     0,     0,     0,   204,
       0,   855,   205,     0,   313,   314,   198,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   206,     0,     0,
       0,     0,     0,     0,     0,     0,   207,     0,    50,     0,
       0,     0,   199,   200,     0,     0,   315,   316,   317,   318,
     319,   320,   321,   322,   323,    51,   324,     0,   299,   300,
     301,   302,   303,     0,     0,   621,     0,     0,     0,     0,
     304,     0,   325,   326,   327,     0,   328,     0,     0,   329,
     330,     0,   305,     0,     0,     0,     0,   306,   331,   332,
     307,   333,   334,   308,   266,     0,     0,   309,     0,     0,
       0,     0,   201,     0,     0,     0,     0,   310,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   311,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   312,
       0,     0,   202,   203,     0,     0,     0,   204,     0,   856,
     205,     0,   313,   314,   198,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   207,     0,    50,     0,     0,     0,
     199,   200,     0,     0,   315,   316,   317,   318,   319,   320,
     321,   322,   323,    51,   324,     0,   299,   300,   301,   302,
     303,     0,     0,     0,     0,     0,     0,     0,   304,     0,
     325,   326,   327,     0,   328,     0,     0,   329,   330,     0,
     305,     0,     0,     0,     0,   306,     0,   332,   307,   333,
     334,   308,   266,     0,     0,   309,     0,     0,     0,     0,
     201,     0,     0,     0,     0,   310,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   311,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,     0,     0,   312,     0,     0,
     202,   203,     0,     0,     0,   204,     0,   857,   205,     0,
     313,   314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   206,     0,     0,     0,     0,     0,     0,
       0,     0,   207,     0,    50,   198,     0,     0,     0,     0,
       0,     0,   315,   316,   317,   318,   319,   320,   321,   322,
     323,    51,   324,     0,     0,     0,     0,     0,     0,     0,
       0,   199,   200,     0,     0,   198,     0,     0,   325,   326,
     327,     0,   328,     0,     0,   329,   330,   299,   300,   301,
     302,   303,     0,     0,     0,   332,   640,   333,   334,   304,
     266,   199,   200,     0,     0,     0,     0,     0,     0,     0,
       0,   305,     0,     0,     0,     0,   306,     0,     0,   307,
       0,     0,   308,     0,     0,     0,   309,     0,     0,   662,
       0,   201,     0,     0,     0,     0,   310,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   311,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,   201,   140,   141,   142,   143,     0,     0,   312,     0,
       0,   202,   203,     0,     0,     0,   204,     0,   939,   205,
       0,   313,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   206,     0,     0,     0,     0,     0,
       0,   202,   203,   207,     0,    50,   204,     0,   978,   205,
       0,     0,     0,   315,   316,   317,   318,   319,   320,   321,
     322,   323,    51,   324,   206,   299,   300,   301,   302,   303,
       0,     0,     0,   207,     0,     0,     0,   304,     0,   325,
     326,   327,     0,   328,     0,     0,   329,   330,     0,   305,
       0,     0,     0,     0,   306,     0,   332,   307,   333,   334,
     308,   266,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   310,   115,   116,   117,     0,   119,
     120,   121,   122,   123,   311,   125,   126,   127,   128,   129,
     130,   131,   132,   133,     0,   135,   136,   137,     0,     0,
     140,   141,   142,   143,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   313,
     314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   315,   316,   317,   318,   319,   320,   321,   322,   323,
      51,   324,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,   326,   327,
       0,   328,     0,     0,   329,   330,   299,   300,   301,   302,
     303,     0,     0,     0,   332,   730,   333,   334,   304,   266,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,     0,     0,     0,     0,   306,     0,     0,   307,     0,
       0,   308,     0,     0,     0,   309,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   310,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   311,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,     0,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,   314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   315,   316,   317,   318,   319,   320,   321,   322,
     323,    51,   324,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   732,     0,   325,   326,
     327,     0,   328,     0,     0,   329,   330,   299,   300,   301,
     302,   303,     0,     0,     0,   332,     0,   333,   334,   304,
     266,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   305,     0,     0,     0,     0,   306,     0,     0,   307,
       0,     0,   308,     0,     0,     0,   309,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   310,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   311,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,     0,     0,   312,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   315,   316,   317,   318,   319,   320,   321,
     322,   323,    51,   324,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   325,
     326,   327,     0,   328,     0,     0,   329,   330,   299,   300,
     301,   302,   303,     0,     0,     0,   332,   746,   333,   334,
     304,   266,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,     0,     0,     0,     0,   306,     0,     0,
     307,     0,     0,   308,     0,     0,     0,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   310,   115,   116,
     117,     0,   119,   120,   121,   122,   123,   311,   125,   126,
     127,   128,   129,   130,   131,   132,   133,     0,   135,   136,
     137,     0,     0,   140,   141,   142,   143,     0,     0,   312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,   315,   316,   317,   318,   319,   320,
     321,   322,   323,    51,   324,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     325,   326,   327,     0,   328,     0,     0,   329,   330,   299,
     300,   301,   302,   303,     0,     0,     0,   332,   755,   333,
     334,   304,   266,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   305,     0,     0,     0,     0,   306,     0,
       0,   307,     0,     0,   308,     0,     0,     0,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   310,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   311,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,   315,   316,   317,   318,   319,
     320,   321,   322,   323,    51,   324,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   325,   326,   327,     0,   328,     0,     0,   329,   330,
     299,   300,   301,   302,   303,     0,     0,     0,   332,   889,
     333,   334,   304,   266,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,     0,     0,     0,     0,   306,
       0,     0,   307,     0,     0,   308,     0,     0,     0,   309,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   310,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   311,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   313,   314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,   315,   316,   317,   318,
     319,   320,   321,   322,   323,    51,   324,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   325,   326,   327,     0,   328,     0,     0,   329,
     330,   299,   300,   301,   302,   303,     0,     0,     0,   332,
     965,   333,   334,   304,   266,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   305,     0,     0,     0,     0,
     306,     0,     0,   307,     0,     0,   308,     0,     0,     0,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     310,   115,   116,   117,     0,   119,   120,   121,   122,   123,
     311,   125,   126,   127,   128,   129,   130,   131,   132,   133,
       0,   135,   136,   137,     0,     0,   140,   141,   142,   143,
       0,     0,   312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   313,   314,     0,     0,     0,
       0,     0,     0,     0,   976,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   315,   316,   317,
     318,   319,   320,   321,   322,   323,    51,   324,     0,   299,
     300,   301,   302,   303,     0,     0,     0,     0,     0,     0,
       0,   304,     0,   325,   326,   327,     0,   328,     0,     0,
     329,   330,     0,   305,     0,     0,     0,     0,   306,     0,
     332,   307,   333,   334,   308,   266,     0,     0,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   310,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   311,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   314,     0,   437,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,   315,   316,   317,   318,   319,
     320,   321,   322,   323,    51,   324,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   325,   326,   327,     0,   328,     0,     0,   329,   330,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
     333,   334,     0,   266,   681,   682,   683,   684,   685,   686,
     687,   688,   439,   440,   441,   442,   443,   689,   690,   444,
     445,   446,   447,   796,   448,   449,   450,   451,   437,   438,
       0,   452,   692,   453,   454,   693,   694,     0,     0,   455,
     456,   457,   695,   696,   697,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   797,   459,     0,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,   798,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   681,   682,   683,   684,
     685,   686,   687,   688,   439,   440,   441,   442,   443,   689,
     690,   444,   445,   446,   447,   796,   448,   449,   450,   451,
     437,   438,     0,   452,   692,   453,   454,   693,   694,     0,
       0,   455,   456,   457,   695,   696,   697,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   797,   459,     0,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   470,   471,
       0,     0,     0,     0,     0,   806,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   681,   682,
     683,   684,   685,   686,   687,   688,   439,   440,   441,   442,
     443,   689,   690,   444,   445,   446,   447,   796,   448,   449,
     450,   451,   437,   438,     0,   452,   692,   453,   454,   693,
     694,     0,     0,   455,   456,   457,   695,   696,   697,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   797,   459,
       0,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     470,   471,     0,     0,     0,     0,     0,   898,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     681,   682,   683,   684,   685,   686,   687,   688,   439,   440,
     441,   442,   443,   689,   690,   444,   445,   446,   447,   796,
     448,   449,   450,   451,   437,   438,     0,   452,   692,   453,
     454,   693,   694,     0,     0,   455,   456,   457,   695,   696,
     697,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   459,     0,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   470,   471,     0,     0,     0,     0,     0,   901,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   681,   682,   683,   684,   685,   686,   687,   688,
     439,   440,   441,   442,   443,   689,   690,   444,   445,   446,
     447,   796,   448,   449,   450,   451,   437,   438,     0,   452,
     692,   453,   454,   693,   694,     0,     0,   455,   456,   457,
     695,   696,   697,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   797,   459,     0,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   470,   471,     0,     0,     0,     0,
       0,   921,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   438,   681,   682,   683,   684,   685,   686,
     687,   688,   439,   440,   441,   442,   443,   689,   690,   444,
     445,   446,   447,   796,   448,   449,   450,   451,     0,     0,
       0,   452,   692,   453,   454,   693,   694,     0,     0,   455,
     456,   457,   695,   696,   697,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   797,   459,     0,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,     0,   437,   438,
       0,     0,     0,     0,     0,     0,   470,   471,   439,   440,
     441,   442,   443,   959,     0,   444,   445,   446,   447,     0,
     448,   449,   450,   451,     0,     0,     0,   452,     0,   453,
     454,     0,     0,     0,     0,   455,   456,   457,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   459,     0,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,     0,   437,   438,     0,     0,     0,     0,
       0,     0,   470,   471,   439,   440,   441,   442,   443,   912,
       0,   444,   445,   446,   447,     0,   448,   449,   450,   451,
       0,     0,     0,   452,     0,   453,   454,     0,     0,     0,
       0,   455,   456,   457,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   459,     0,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,     0,
       0,     0,     0,   437,   438,     0,     0,     0,   470,   471,
     439,   440,   441,   442,   443,   987,     0,   444,   445,   446,
     447,     0,   448,   449,   450,   451,     0,     0,     0,   452,
       0,   453,   454,     0,     0,     0,     0,   455,   456,   457,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   459,     0,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,     0,     0,     0,     0,     0,
       0,     0,   437,   438,   470,   471,     0,     0,   591,   439,
     440,   441,   442,   443,     0,     0,   444,   445,   446,   447,
       0,   448,   449,   450,   451,     0,     0,     0,   452,     0,
     453,   454,     0,     0,     0,     0,   455,   456,   457,     0,
       0,     0,   458,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   459,     0,   460,   461,   462,   463,   464,   465,
     466,   467,   468,   469,     0,     0,     0,     0,     0,     0,
       0,   437,   438,   470,   471,     0,     0,   735,   439,   440,
     441,   442,   443,     0,     0,   444,   445,   446,   447,     0,
     448,   449,   450,   451,     0,     0,     0,   452,     0,   453,
     454,     0,     0,     0,     0,   455,   456,   457,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   459,     0,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,     0,     0,     0,     0,     0,     0,     0,
     437,   438,   470,   471,     0,     0,   738,   439,   440,   441,
     442,   443,     0,     0,   444,   445,   446,   447,     0,   448,
     449,   450,   451,     0,     0,     0,   452,     0,   453,   454,
       0,     0,     0,     0,   455,   456,   457,     0,     0,     0,
     458,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     459,     0,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,     0,     0,     0,     0,     0,     0,     0,   437,
     438,   470,   471,     0,     0,   740,   439,   440,   441,   442,
     443,     0,     0,   444,   445,   446,   447,     0,   448,   449,
     450,   451,     0,     0,     0,   452,     0,   453,   454,     0,
       0,     0,     0,   455,   456,   457,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   459,
       0,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,     0,     0,     0,     0,     0,     0,     0,   437,   438,
     470,   471,     0,     0,   853,   439,   440,   441,   442,   443,
       0,     0,   444,   445,   446,   447,     0,   448,   449,   450,
     451,     0,     0,     0,   452,     0,   453,   454,     0,     0,
       0,     0,   455,   456,   457,     0,     0,     0,   458,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   459,     0,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
       0,     0,     0,     0,     0,     0,     0,   437,   438,   470,
     471,     0,     0,   984,   439,   440,   441,   442,   443,     0,
       0,   444,   445,   446,   447,     0,   448,   449,   450,   451,
       0,     0,     0,   452,     0,   453,   454,     0,     0,     0,
       0,   455,   456,   457,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   459,     0,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,     0,
       0,     0,     0,     0,     0,     0,   437,   438,   470,   471,
       0,     0,   986,   439,   440,   441,   442,   443,     0,     0,
     444,   445,   446,   447,     0,   448,   449,   450,   451,     0,
       0,     0,   452,     0,   453,   454,     0,     0,     0,     0,
     455,   456,   457,     0,     0,     0,   458,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   459,     0,   460,   461,
     462,   463,   464,   465,   466,   467,   468,   469,     0,     0,
       0,     0,     0,   437,   438,     0,     0,   470,   471,     0,
       0,  1006,   439,   440,   441,   442,   443,     0,     0,   444,
     445,   446,   447,     0,   448,   449,   450,   451,     0,     0,
       0,   452,     0,   453,   454,     0,     0,     0,     0,   455,
     456,   457,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   459,     0,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,     0,     0,     0,
     437,   438,     0,     0,     0,     0,   470,   471,   472,   439,
     440,   441,   442,   443,     0,     0,   444,   445,   446,   447,
       0,   448,   449,   450,   451,     0,     0,     0,   452,     0,
     453,   454,     0,     0,     0,     0,   455,   456,   457,     0,
       0,     0,   458,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   459,     0,   460,   461,   462,   463,   464,   465,
     466,   467,   468,   469,     0,     0,     0,   437,   438,     0,
       0,     0,     0,   470,   471,   779,   439,   440,   441,   442,
     443,     0,     0,   444,   445,   446,   447,     0,   448,   449,
     450,   451,     0,     0,     0,   452,     0,   453,   454,     0,
       0,     0,     0,   455,   456,   457,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   459,
       0,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,     0,     0,     0,   437,   438,     0,     0,     0,     0,
     470,   471,   787,   439,   440,   441,   442,   443,     0,     0,
     444,   445,   446,   447,     0,   448,   449,   450,   451,     0,
       0,     0,   452,     0,   453,   454,     0,     0,     0,     0,
     455,   456,   457,     0,     0,     0,   458,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   459,     0,   460,   461,
     462,   463,   464,   465,   466,   467,   468,   469,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   470,   471,   892,
     439,   440,   441,   442,   443,   437,   438,   444,   445,   446,
     447,     0,   448,   449,   450,   451,     0,     0,     0,   452,
       0,   453,   454,     0,     0,     0,     0,   455,   456,   457,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   459,     0,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   470,   471,   893,     0,     0,     0,
       0,     0,     0,   681,   682,   683,   684,   685,   686,   687,
     688,   439,   440,   441,   442,   443,   689,   690,   444,   445,
     446,   447,   796,   448,   449,   450,   451,  -247,   437,   438,
     452,   692,   453,   454,   693,   694,     0,     0,   455,   456,
     457,   695,   696,   697,   458,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   797,   459,     0,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   470,   471,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
       0,     0,     0,     0,     0,     0,   681,   682,   683,   684,
     685,   686,   687,   688,   439,   440,   441,   442,   443,   689,
     690,   444,   445,   446,   447,   691,   448,   449,   450,   451,
       0,     0,     0,   452,   692,   453,   454,   693,   694,     0,
       0,   455,   456,   457,   695,   696,   697,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   698,   459,     0,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,   437,
     438,     0,     0,   439,   440,   441,   442,   443,   470,   471,
     444,   445,   446,   447,     0,   448,   449,   450,   451,     0,
       0,     0,   452,     0,   453,   454,     0,     0,   584,     0,
     455,   456,   457,     0,     0,     0,   458,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   459,     0,   460,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   437,   438,
       0,     0,     0,     0,     0,     0,     0,   470,   471,     0,
       0,     0,     0,     0,     0,   439,   440,   441,   442,   443,
       0,     0,   444,   445,   446,   447,     0,   448,   449,   450,
     451,     0,     0,     0,   452,     0,   453,   454,     0,     0,
       0,     0,   455,   456,   457,     0,     0,     0,   458,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   459,   781,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     437,   438,     0,     0,   439,   440,   441,   442,   443,   470,
     471,   444,   445,   446,   447,     0,   448,   449,   450,   451,
       0,     0,     0,   452,     0,   453,   454,     0,     0,     0,
       0,   455,   456,   457,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   875,     0,     0,     0,   459,     0,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,   437,
     438,     0,     0,     0,     0,     0,     0,     0,   470,   471,
       0,     0,     0,     0,     0,     0,   439,   440,   441,   442,
     443,     0,     0,   444,   445,   446,   447,     0,   448,   449,
     450,   451,     0,     0,     0,   452,     0,   453,   454,     0,
       0,     0,     0,   455,   456,   457,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   459,
       0,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   437,   438,     0,     0,   439,   440,   441,   442,   443,
     470,   471,   444,   445,   446,   447,     0,   448,   449,   450,
     451,     0,     0,     0,   452,     0,   453,   454,     0,     0,
       0,     0,   455,   456,   457,     0,     0,     0,  -533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   459,     0,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     437,   438,     0,     0,     0,     0,     0,     0,     0,   470,
     471,     0,     0,     0,     0,     0,     0,   439,   440,   441,
     442,   443,     0,     0,   444,   445,   446,   447,     0,   448,
     449,   450,   451,     0,     0,     0,   452,     0,   453,   454,
       0,     0,     0,     0,   455,   456,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     459,     0,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   437,   438,     0,     0,   439,   440,   441,   442,
     443,   470,   471,   444,   445,   446,   447,     0,   448,   449,
     450,   451,     0,     0,     0,   452,     0,   453,   454,   437,
     438,     0,     0,   455,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     470,   471,     0,     0,     0,     0,     0,     0,   439,   440,
     441,   442,   443,     0,     0,   444,   445,   446,   447,     0,
     448,   449,   450,   451,     0,   437,   438,   452,     0,   453,
     454,     0,     0,     0,     0,   439,   440,   441,   442,   443,
       0,     0,   444,   445,   446,   447,     0,   448,   449,   450,
     451,     0,     0,     0,   452,     0,   453,   454,     0,     0,
       0,     0,     0,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   470,   471,     0,     0,     0,     0,     0,     0,
       0,     0,   462,   463,   464,   465,   466,   467,   468,   469,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   470,
     471,   439,   440,   441,   442,   443,     0,     0,   444,   445,
     446,   447,     0,   448,   449,   450,   451,   215,     0,     0,
     452,     0,   453,   454,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   470,   471,     0,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,     0,     0,     0,   234,
     235,     0,     0,     0,     0,     0,   236,   237,   238,   239,
     240,     0,     0,   241,   242,   243,   244,   245,   246,   247,
     418,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,     0,     0,     0,     0,     0,   112,     0,
       0,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,     0,     0,   258,   259,   113,     0,     0,     0,     0,
     260,   261,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,     0,     0,     0,   420,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,     0,
       0,     0,     0,     0,   112,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   114,    51,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,     0,     0,     0,     0,     0,
     112,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,    51,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,     0,     0,     0,     0,     0,   112,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,   113,     0,     0,     0,     0,     0,     0,
     315,   316,   317,     0,     0,     0,     0,     0,   114,    51,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   111,     0,     0,     0,
       0,     0,   112,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,    51,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399
};

static const yytype_int16 yycheck[] =
{
      12,   168,   363,   368,   479,   394,    73,   396,   174,   398,
     287,   481,   289,   421,   291,   473,   490,   317,    81,   343,
     574,   359,    73,     7,   594,     0,    18,   501,   582,    14,
      15,     6,   381,    19,   502,   503,    51,    32,    19,   122,
      20,    21,    19,    48,    18,    19,    34,   140,    21,   140,
      33,   149,    51,   131,    29,   149,    31,   418,    33,   420,
     149,    73,   119,   120,    39,   149,   165,   149,   132,   143,
      82,   117,    44,    48,   117,   117,    44,    45,    61,    54,
     157,   117,    55,   126,   126,   149,   170,   185,   187,   132,
     126,    14,    15,   184,   183,   189,   189,   149,    73,   557,
     568,   157,   149,   158,   187,   116,   272,   189,   119,   120,
     188,   157,   158,   157,   160,   158,   158,   163,   479,    94,
     108,   109,   158,   180,   181,    97,   106,   107,   489,    97,
     110,   187,   187,   113,   114,   115,   116,   189,   118,   119,
     120,   121,   189,   187,   149,   125,    55,   127,   128,   132,
     149,   149,    61,   133,   134,   135,   140,   149,   143,   139,
     157,   149,   147,   157,   149,   150,   149,    60,   163,   180,
     181,   157,   187,   647,   523,   149,   157,   651,   748,   159,
     157,   161,   162,   163,   164,   165,   181,   184,   149,   169,
     188,   665,   746,   187,   160,   190,   169,   181,   183,    41,
     180,   181,   151,   152,   183,   189,   181,   180,   532,   188,
     273,   679,   187,   280,   281,   282,   283,   157,   556,   286,
     143,   288,   188,   290,   147,   292,   149,   150,   189,   280,
     281,   282,   283,    75,   301,   286,   117,   288,   132,   290,
     714,   132,   117,    11,   184,   126,   158,   157,   315,   316,
     301,   126,   157,   160,    22,    23,     4,     5,   149,     7,
     157,   157,    20,    21,   315,   316,   160,   157,   280,   281,
     282,   283,   182,   164,   286,   157,   288,   158,   290,   184,
     292,   119,   120,   158,   742,   585,   182,    35,   163,   301,
     157,   358,    20,    21,   184,   187,   754,   189,   157,   366,
      55,   157,   184,   315,   316,   157,    61,   358,   862,   160,
     158,   149,    20,    21,   165,   366,   328,   184,   872,   789,
     778,   157,   599,   182,   160,   492,   182,   163,   151,   152,
     182,   165,   183,   500,   160,   793,   160,   184,   746,   165,
     187,   165,   180,   181,   149,   149,   358,   755,   415,   165,
     108,   109,    68,   187,   366,   165,    72,   183,   116,   183,
     118,   119,   120,   121,   415,   377,   532,   125,   149,   149,
     844,   187,    55,    89,    90,    91,    92,   187,    61,    55,
     108,   109,   132,    55,    55,    61,   183,   776,   116,    61,
      61,   119,   120,   121,   864,   407,   143,   125,   106,   107,
     108,   109,   110,   415,   160,   113,   180,   206,   116,   165,
     118,   119,   120,   121,   149,   185,   186,   125,   188,   127,
     128,   433,   180,   181,   187,   182,   189,   183,   149,   188,
     187,   151,   152,   153,   154,   149,   151,   152,   153,   981,
     149,   983,   917,   187,   149,   188,    97,   188,    55,   183,
     187,   164,   180,   181,   164,   149,   164,   165,   166,   167,
     168,   169,   170,   164,   164,   164,   164,   164,   164,   164,
     537,   164,   180,   181,   541,    20,    21,    34,    34,   149,
     183,   188,   649,   149,   140,   552,   537,   554,   765,   878,
     541,   658,   559,    32,   561,   562,   149,   564,   187,   160,
     187,   552,   182,   182,   188,   160,   160,   674,   559,   149,
     561,   562,   164,   564,   313,   314,   184,   182,   317,   183,
     183,   164,   164,   183,   183,   537,   325,   326,   327,   541,
     329,   330,   164,   332,   164,   181,   186,   902,   183,   188,
     552,    41,   554,   140,   149,   149,   149,   559,   149,   561,
     562,   187,   564,   143,    36,   149,   917,   149,   187,   187,
     187,   187,   187,   108,   109,   182,   149,   165,   157,   368,
     165,   116,   165,   118,   119,   120,   121,   157,   645,   189,
     125,   151,   182,   149,   183,   164,   149,   149,   149,     1,
     122,    13,   160,   158,   645,   143,     7,   188,   186,    41,
     187,   182,   149,   149,   182,   182,   182,   189,   151,   149,
     409,   149,    32,   412,   149,   414,   149,   184,    20,    21,
     419,   149,   421,   168,   169,   170,   149,   187,   183,   182,
     182,   182,   799,   645,   165,   180,   181,    32,    58,    59,
     439,   440,    51,   165,   443,   444,   445,   446,   182,   448,
     187,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   182,   471,   182,   189,   182,   182,   182,   182,   189,
     182,    63,    37,   425,   108,    66,   980,   486,    64,    82,
     702,   490,   278,   702,   493,   494,   808,   764,   118,     1,
     499,   885,   501,   741,   106,   107,   108,   109,   916,   508,
     839,    20,    21,   764,   116,   554,   118,   119,   120,   121,
     887,   483,   293,   125,    45,   127,   128,   528,   710,   301,
     753,   285,   972,   421,    -1,    -1,   903,    -1,   158,   159,
      -1,    -1,    -1,   163,    -1,   165,   166,    -1,    -1,    -1,
      -1,    -1,   764,    -1,    -1,    -1,    -1,    -1,    -1,   558,
      -1,   181,    -1,    -1,   166,   167,   168,   169,   170,    -1,
     190,   570,   571,    -1,    -1,    -1,    32,    -1,   180,   181,
      -1,    -1,   581,   950,    -1,   584,   585,   954,    -1,   588,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    58,    59,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,   888,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   998,    -1,    -1,    -1,   634,    -1,   888,   637,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   647,    -1,
      -1,    -1,   651,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   118,    -1,    -1,    -1,   665,    -1,    -1,    -1,
      -1,   180,   181,    -1,    -1,    -1,   888,    -1,    -1,    -1,
      -1,    -1,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,   691,   692,   693,   694,   695,   696,   697,   698,
      -1,   700,   158,   159,    -1,    -1,    -1,   163,    -1,   165,
     166,    -1,    -1,    -1,    -1,   714,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,   181,    -1,    -1,   106,   107,
     108,   109,   110,    -1,   190,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,   744,   745,   125,    -1,   127,
     128,    32,   751,    -1,   753,   133,    -1,   135,    -1,    -1,
      -1,    -1,   974,    -1,    -1,    -1,    -1,    -1,    32,   768,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    -1,
      -1,   993,   781,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    58,    59,    -1,   796,   797,    -1,
      -1,    -1,   180,   181,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,   821,   822,    -1,   125,    -1,   127,   128,    -1,
      -1,   830,   831,   133,   134,   135,    -1,   118,    -1,   139,
      -1,    -1,    -1,    -1,    -1,   844,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,    -1,
     180,   181,   163,    -1,    -1,   166,    -1,    -1,   188,    -1,
      -1,    -1,    -1,    -1,   158,   159,    -1,   896,    -1,   163,
     181,   900,   166,   902,    -1,    -1,   187,   906,    -1,   190,
      -1,    -1,    -1,    -1,    32,    -1,    -1,   181,    -1,   183,
      -1,    -1,    -1,    -1,   923,    -1,   190,   926,   927,   928,
     929,   930,    -1,    -1,   933,   934,    -1,   936,    -1,    -1,
      58,    59,     1,    -1,    -1,   944,    -1,     6,    -1,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   976,    37,    38,
      -1,    40,    -1,    42,    43,    -1,   985,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    -1,    56,    57,   117,
     118,  1000,    -1,    -1,    63,    64,    65,    66,   126,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    96,    -1,    -1,
     158,   159,    -1,    -1,    -1,   163,    -1,    -1,   166,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   123,   124,    -1,    -1,    -1,   187,
      -1,    -1,   190,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,   166,   167,   168,
      -1,   170,    -1,    -1,   173,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,   185,   186,   187,   188,
     189,     1,    58,    59,    -1,    -1,     6,    -1,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    -1,    42,    43,    -1,    -1,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    -1,    56,    57,    -1,    -1,
      -1,   117,   118,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    96,    -1,    -1,    -1,
      -1,    -1,   158,   159,    -1,    -1,    -1,   163,   108,   109,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,   123,   124,   181,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,   190,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,   167,   168,    -1,
     170,    -1,    -1,   173,   174,    -1,    -1,    -1,    14,    15,
      16,    17,    18,   183,    -1,   185,   186,   187,   188,   189,
      26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,   108,   109,
      46,    -1,    -1,    49,    -1,    51,   116,    53,   118,   119,
     120,   121,    58,    59,    -1,   125,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,   166,   167,   168,   169,
     170,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
     180,   181,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,    -1,    -1,    -1,   163,    -1,    -1,
     166,   167,   168,    -1,   170,    -1,    -1,   173,   174,    14,
      15,    16,    17,    18,    -1,   181,    -1,   183,    -1,   185,
     186,    26,   188,    -1,   190,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,    58,    59,    -1,   163,    -1,
      -1,   166,   167,   168,    -1,   170,    -1,    -1,   173,   174,
      14,    15,    16,    17,    18,    -1,   181,    -1,   183,    -1,
     185,   186,    26,   188,    -1,   190,    -1,    32,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    58,    59,   118,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,   158,   159,    -1,    -1,    -1,
     163,    -1,   165,   166,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   132,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,    58,    59,    -1,   163,   163,
     165,   166,   166,   167,   168,    -1,   170,    -1,    -1,   173,
     174,    14,    15,    16,    17,    18,   181,   181,    -1,   183,
      -1,   185,   186,    26,   188,   190,   190,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,   158,   159,    -1,    -1,    -1,
     163,    -1,   165,   166,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   181,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,   190,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,   166,   167,   168,    -1,   170,    -1,    26,
     173,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    38,   185,   186,   187,   188,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    58,
      59,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   166,
     167,   168,    -1,   170,    -1,    -1,   173,   174,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   183,    46,   185,   186,
      49,   188,    -1,    -1,    53,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,   158,
     159,    -1,    -1,    -1,   163,    -1,   165,   166,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    32,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,   167,   168,
      -1,   170,    -1,    -1,   173,   174,    14,    15,    16,    17,
      18,    -1,   181,    -1,   183,   184,   185,   186,    26,   188,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,   118,    58,
      59,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,   158,   159,
      -1,    -1,    -1,   163,    -1,   165,   166,    -1,    -1,    -1,
     108,   109,    32,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,   181,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,
     190,    -1,    -1,    -1,   132,    -1,    -1,    -1,    58,    59,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,    -1,    14,    15,    16,    17,    18,   158,
     159,    -1,    -1,    -1,   163,    -1,    26,   166,   166,   167,
     168,    -1,   170,    -1,    -1,   173,   174,    -1,    38,    -1,
      -1,    -1,   181,    43,    -1,   183,    46,   185,   186,    49,
     188,   190,    -1,    53,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,   158,   159,
      -1,    -1,    -1,   163,    -1,   165,   166,    -1,   108,   109,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
     190,    -1,   132,    -1,    -1,    -1,    58,    59,    -1,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,   166,   167,   168,    -1,
     170,    -1,    -1,   173,   174,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,   183,    46,   185,   186,    49,   188,    -1,
      -1,    53,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    -1,    -1,   158,   159,    -1,    -1,
      -1,   163,    -1,   165,   166,    -1,   108,   109,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,   190,    -1,
     132,    -1,    -1,    -1,    58,    59,    -1,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   166,   167,   168,    -1,   170,    -1,
      -1,   173,   174,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   183,    46,   185,   186,    49,   188,    -1,    -1,    53,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,   158,   159,    -1,    -1,    -1,   163,
      -1,   165,   166,    -1,   108,   109,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   132,    -1,
      -1,    -1,    58,    59,    -1,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      26,    -1,   166,   167,   168,    -1,   170,    -1,    -1,   173,
     174,    -1,    38,    -1,    -1,    -1,    -1,    43,   182,   183,
      46,   185,   186,    49,   188,    -1,    -1,    53,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,   158,   159,    -1,    -1,    -1,   163,    -1,   165,
     166,    -1,   108,   109,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   132,    -1,    -1,    -1,
      58,    59,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     166,   167,   168,    -1,   170,    -1,    -1,   173,   174,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   183,    46,   185,
     186,    49,   188,    -1,    -1,    53,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,
     158,   159,    -1,    -1,    -1,   163,    -1,   165,   166,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   132,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    -1,    -1,    32,    -1,    -1,   166,   167,
     168,    -1,   170,    -1,    -1,   173,   174,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   183,   184,   185,   186,    26,
     188,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    56,
      -1,   118,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,   118,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,   158,   159,    -1,    -1,    -1,   163,    -1,   165,   166,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   190,    -1,   132,   163,    -1,   165,   166,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   181,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   190,    -1,    -1,    -1,    26,    -1,   166,
     167,   168,    -1,   170,    -1,    -1,   173,   174,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   183,    46,   185,   186,
      49,   188,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,   167,   168,
      -1,   170,    -1,    -1,   173,   174,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   183,   184,   185,   186,    26,   188,
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
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,   166,   167,
     168,    -1,   170,    -1,    -1,   173,   174,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   183,    -1,   185,   186,    26,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
     167,   168,    -1,   170,    -1,    -1,   173,   174,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   183,   184,   185,   186,
      26,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,   167,   168,    -1,   170,    -1,    -1,   173,   174,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   183,   184,   185,
     186,    26,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,   167,   168,    -1,   170,    -1,    -1,   173,   174,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   183,   184,
     185,   186,    26,   188,    -1,    -1,    -1,    -1,    -1,    -1,
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
     144,   145,   146,   147,   148,   149,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,   167,   168,    -1,   170,    -1,    -1,   173,
     174,    14,    15,    16,    17,    18,    -1,    -1,    -1,   183,
     184,   185,   186,    26,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   166,   167,   168,    -1,   170,    -1,    -1,
     173,   174,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     183,    46,   185,   186,    49,   188,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,   167,   168,    -1,   170,    -1,    -1,   173,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
     185,   186,    -1,   188,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    20,    21,
      -1,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,    -1,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,    -1,    -1,
      -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      20,    21,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,    -1,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    20,    21,    -1,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
      -1,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,   181,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    20,    21,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,    -1,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    20,    21,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,    -1,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,   181,    -1,    -1,    -1,    -1,
      -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,    -1,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   106,   107,
     108,   109,   110,   187,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   106,   107,   108,   109,   110,   187,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   180,   181,
     106,   107,   108,   109,   110,   187,    -1,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,   180,   181,    -1,    -1,   184,   106,
     107,   108,   109,   110,    -1,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,   180,   181,    -1,    -1,   184,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,   180,   181,    -1,    -1,   184,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,   180,   181,    -1,    -1,   184,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
     180,   181,    -1,    -1,   184,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,   180,
     181,    -1,    -1,   184,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,   180,   181,
      -1,    -1,   184,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,   180,   181,    -1,
      -1,   184,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   180,   181,   182,   106,
     107,   108,   109,   110,    -1,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,   180,   181,   182,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
     180,   181,   182,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,   181,   182,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    20,    21,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,    -1,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,    -1,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,    20,
      21,    -1,    -1,   106,   107,   108,   109,   110,   180,   181,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,   131,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      20,    21,    -1,    -1,   106,   107,   108,   109,   110,   180,
     181,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,   159,    -1,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    20,    21,    -1,    -1,   106,   107,   108,   109,   110,
     180,   181,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,    20,    21,    -1,    -1,   106,   107,   108,   109,
     110,   180,   181,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    20,
      21,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,   181,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    20,    21,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,    -1,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,   164,   165,   166,   167,   168,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
     181,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    37,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
     165,   166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,   181,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,   173,   174,    47,    -1,    -1,    -1,    -1,
     180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,   149,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,   149,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    47,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,    -1,    -1,    -1,    -1,    62,   149,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,   149,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   192,     0,     6,    29,    31,    33,    39,    48,    54,
      73,    94,   181,   187,   195,   203,   204,   208,   231,   235,
     251,   308,   313,   315,   321,   357,   359,    18,    19,   149,
     225,   226,   227,   143,   209,   210,   149,   170,   205,   206,
     149,   188,   311,   149,   185,   194,   360,   358,    33,    61,
     132,   149,   228,   229,   230,   243,     4,     5,     7,    35,
     319,    60,   306,   158,   157,   160,   157,   205,    21,    55,
     169,   180,   207,   158,   311,   312,   306,   149,   149,   149,
     132,   183,   157,   182,    55,    61,   236,   238,    55,    61,
     314,    55,    61,   320,    55,    61,   307,    14,    15,   143,
     147,   149,   150,   183,   197,   226,   143,   210,   149,   149,
     149,    24,    30,    47,    62,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   243,   324,   326,   327,
     329,   332,   187,   189,   311,    55,    61,   193,   188,   188,
     149,   227,   229,    34,   108,   109,   149,   234,   237,   149,
      97,   317,   318,   188,   309,   151,   152,   196,    14,    15,
     143,   147,   149,   197,   223,   224,   207,   164,   164,   164,
     164,   183,   164,   164,   164,   164,   164,   164,    32,    58,
      59,   118,   158,   159,   163,   166,   181,   190,   187,   149,
     328,   149,   298,   299,   184,    37,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   119,   120,   126,   127,   128,   129,
     130,   133,   134,   135,   136,   137,   138,   139,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   173,   174,
     180,   181,    34,    34,   183,   232,   188,   239,   160,   188,
     149,   322,   305,   140,   287,   151,   152,   153,   157,   184,
     335,   337,   339,   333,   149,   330,   341,   343,   345,   347,
     349,   351,   353,   355,    32,    32,   163,   181,   190,    14,
      15,    16,    17,    18,    26,    38,    43,    46,    49,    53,
      63,    73,    95,   108,   109,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   150,   166,   167,   168,   170,   173,
     174,   182,   183,   185,   186,   199,   201,   239,   243,   245,
     253,   260,   263,   266,   270,   271,   274,   275,   276,   281,
     284,   324,   361,   365,   370,   372,   374,   187,   160,   187,
     182,   182,   184,   287,   294,   295,   160,   233,   241,    68,
      72,    89,    90,    91,    92,   325,   310,   160,   316,   188,
     189,   287,   227,   149,   304,   323,   223,   332,   332,   332,
     332,   184,   328,   332,   232,   332,   232,   332,   232,   149,
     296,   297,   332,   299,   182,   332,   361,   183,   164,   183,
     164,   164,   183,   164,   183,   164,   284,   284,    12,   332,
      12,   332,   284,   367,   371,   198,   284,   284,   284,   243,
     284,   284,   284,   186,   149,   183,   231,    20,    21,   106,
     107,   108,   109,   110,   113,   114,   115,   116,   118,   119,
     120,   121,   125,   127,   128,   133,   134,   135,   139,   159,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     180,   181,   182,   183,   149,   189,   332,   189,   298,    48,
     301,   323,   184,   187,   332,     1,     8,     9,    10,    12,
      25,    27,    28,    37,    40,    42,    50,    52,    56,    57,
      63,    96,   123,   124,   187,   189,   211,   212,   215,   216,
     217,   218,   219,   220,   221,   240,   242,   244,   246,   247,
     248,   249,   250,   251,   252,   272,   273,   284,   188,   149,
     189,   243,   292,   304,   182,    41,   157,   160,   163,   303,
     165,   187,   165,   165,   165,   187,   165,   233,   165,   233,
     165,   233,   160,   165,   187,   165,   187,   183,   243,   261,
     284,   254,   256,   284,   258,   284,   332,   323,    51,   149,
     166,   183,   284,   362,   363,   364,   366,   367,   368,   369,
     323,   183,   363,   369,   131,   187,   189,   153,   154,   196,
     202,   184,   164,   243,   181,   184,   264,   284,   140,   269,
      18,   149,   149,   284,   284,   284,   284,   284,   284,   149,
     284,   149,   284,   284,   284,   284,   284,   284,   284,   284,
     284,    21,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   119,   120,   149,   180,   181,   282,   284,
     184,   264,   301,   117,   158,   160,   163,   300,   294,   284,
     323,   117,   187,   242,   272,   284,   239,   284,   284,   149,
     187,   143,    56,   284,   239,   117,   242,   284,   186,   270,
     270,    36,   187,   187,   284,   187,   187,   187,   304,   122,
     187,    98,    99,   100,   101,   102,   103,   104,   105,   111,
     112,   117,   126,   129,   130,   136,   137,   138,   158,   310,
     158,   187,   181,   189,   231,   287,   291,   182,   149,   149,
     332,   117,   126,   158,   302,   336,   332,   340,   334,   331,
     342,   165,   346,   165,   350,   165,   332,   354,   296,   356,
     184,   264,   164,   284,   332,   184,   332,   332,   184,   332,
     184,   165,    19,   270,   126,   300,   184,   157,   187,   364,
     182,   157,   182,   187,    19,   184,   364,   189,   284,   367,
     189,   284,   151,   200,   277,   279,   149,   362,   157,   184,
     117,   126,   158,   163,   267,   268,   232,   164,   183,   182,
     149,   160,   149,   284,   149,   284,     1,   182,   184,   332,
     242,   284,   239,    19,   242,   284,   117,   158,   187,    13,
     239,   158,   160,   143,   242,   284,   187,   186,   188,   239,
     270,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   123,   124,   284,   284,   284,   284,   284,   284,   284,
     123,   124,   284,   189,   284,   230,     7,    44,    97,   288,
     187,    41,   158,   187,   302,   242,   284,   165,   344,   348,
     352,   184,   149,   184,   165,   165,   165,   165,   269,   264,
     284,   284,   363,   364,   149,   362,   182,   182,   284,   182,
     368,   264,   363,   189,   182,   155,   151,   332,   232,   182,
     284,   149,   149,   149,   149,   157,   182,   233,   285,   184,
     264,   284,   182,   182,   283,   158,   300,   264,   187,   239,
     222,   187,   241,    11,    22,    23,   213,   214,   284,   284,
     284,   284,   187,   182,    55,    61,   290,   290,   149,   242,
     284,   187,   338,   165,   187,   262,   255,   257,   259,   183,
     187,   364,   182,   126,   300,   182,   187,   364,   182,   165,
     233,   184,   267,   182,   131,   239,   265,   332,   184,   284,
     187,   284,   189,   239,   284,   187,    45,   288,   301,   187,
     284,   149,   284,   284,   284,   184,   284,   284,   182,   182,
     284,   284,   284,   189,   278,   165,   117,   284,   165,   239,
     239,   293,    32,   289,   184,   165,   184,   187,   373,   182,
     373,   182,   243,   280,   284,   286,   214,   237,   237,   284,
      51,   182,   189,   243,   187,   239,   184,   284,   182,   182
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   191,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   193,   193,   193,   194,   194,
     195,   196,   196,   196,   196,   197,   198,   198,   198,   199,
     200,   200,   202,   201,   203,   204,   205,   205,   205,   205,
     206,   206,   207,   207,   208,   209,   209,   210,   210,   211,
     212,   212,   213,   213,   214,   214,   214,   215,   215,   216,
     217,   218,   219,   220,   222,   221,   223,   223,   223,   223,
     223,   223,   224,   224,   225,   225,   225,   226,   226,   226,
     226,   226,   226,   226,   226,   227,   227,   228,   228,   228,
     229,   229,   230,   230,   231,   231,   232,   232,   232,   233,
     233,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   235,   236,   236,   236,   237,   238,   239,   239,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   241,
     241,   241,   242,   242,   242,   243,   243,   243,   244,   245,
     245,   245,   245,   246,   247,   248,   248,   248,   248,   248,
     249,   249,   249,   249,   250,   251,   251,   252,   254,   255,
     253,   256,   257,   253,   258,   259,   253,   261,   262,   260,
     263,   263,   263,   264,   264,   265,   265,   265,   266,   266,
     266,   267,   267,   267,   267,   268,   268,   269,   269,   270,
     270,   271,   271,   271,   271,   271,   271,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   273,   273,   273,   273,
     274,   275,   275,   276,   277,   278,   276,   279,   280,   276,
     281,   281,   282,   283,   281,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   285,   286,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     287,   287,   288,   288,   288,   289,   289,   290,   290,   290,
     291,   292,   292,   293,   292,   292,   292,   294,   294,   295,
     295,   296,   296,   297,   297,   298,   299,   299,   300,   300,
     301,   301,   301,   301,   301,   301,   302,   302,   302,   303,
     303,   304,   304,   304,   304,   304,   305,   305,   306,   306,
     307,   307,   307,   308,   309,   308,   310,   310,   310,   311,
     312,   312,   313,   313,   314,   314,   314,   315,   315,   316,
     316,   317,   317,   318,   319,   319,   320,   320,   320,   322,
     321,   323,   323,   323,   323,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     325,   325,   325,   325,   325,   325,   326,   327,   327,   328,
     328,   330,   331,   329,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   333,   334,   332,   332,   335,   336,   332,   337,   338,
     332,   339,   340,   332,   332,   341,   342,   332,   343,   344,
     332,   332,   345,   346,   332,   347,   348,   332,   332,   349,
     350,   332,   351,   352,   332,   353,   354,   332,   355,   356,
     332,   358,   357,   360,   359,   361,   361,   361,   361,   362,
     362,   362,   362,   363,   363,   364,   364,   365,   365,   365,
     365,   365,   365,   366,   366,   367,   368,   368,   369,   369,
     370,   370,   371,   371,   372,   373,   373,   374,   374
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
       2,     3,     0,     1,     1,     3,     3,     3,     7,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     2,     2,     2,     0,
       2,     2,     3,     2,     2,     1,     3,     2,     2,     2,
       4,     5,     2,     1,     1,     2,     3,     4,     2,     3,
       3,     4,     2,     3,     4,     1,     1,     2,     0,     0,
       7,     0,     0,     7,     0,     0,     7,     0,     0,     6,
       5,     8,    10,     1,     3,     1,     2,     3,     1,     1,
       2,     2,     2,     2,     2,     1,     3,     0,     4,     1,
       6,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     4,     4,
       6,     5,     6,     3,     0,     0,     8,     0,     0,     9,
       3,     4,     0,     0,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     3,     4,     5,     4,     5,     3,
       4,     1,     3,     4,     3,     4,     2,     4,     4,     7,
       8,     3,     5,     0,     0,     8,     3,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     4,
       0,     4,     0,     1,     1,     0,     1,     0,     1,     1,
       4,     0,     3,     0,     8,     8,     5,     2,     3,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     1,
       1,     2,     3,     5,     3,     3,     1,     1,     1,     0,
       1,     4,     6,     5,     5,     4,     0,     3,     0,     1,
       0,     1,     1,     6,     0,     6,     0,     3,     5,     3,
       2,     3,     4,     2,     0,     1,     1,     7,     9,     0,
       2,     0,     1,     3,     1,     1,     0,     1,     1,     0,
       8,     1,     3,     3,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     1,
       3,     0,     0,     6,     1,     1,     1,     1,     4,     3,
       4,     2,     2,     3,     2,     3,     2,     2,     3,     3,
       2,     0,     0,     6,     2,     0,     0,     6,     0,     0,
       8,     0,     0,     6,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     1,     0,
       0,     6,     0,     0,     7,     0,     0,     6,     0,     0,
       6,     0,     7,     0,     7,     1,     1,     1,     1,     3,
       3,     5,     5,     1,     3,     0,     2,     6,     5,     7,
       8,     6,     8,     1,     3,     3,     1,     1,     1,     3,
       5,     5,     1,     3,     4,     0,     3,    10,    10
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

  case 151: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 152: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 153: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 154: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 155: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 156: /* function_declaration: optional_public_or_private_function function_declaration_header expression_block  */
                                                                                                               {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-2].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
    break;

  case 157: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 158: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 159: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 160: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 161: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 162: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 163: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 164: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 165: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 166: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 167: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 168: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 169: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 170: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 171: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 172: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 173: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 174: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 175: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 176: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 177: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 178: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 179: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 180: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 181: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 182: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 183: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 184: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 185: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 186: /* name_in_namespace: "name" "::" "name"  */
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

  case 187: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 188: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 189: /* expr_new: "new" type_declaration  */
                                                   {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 190: /* expr_new: "new" type_declaration '(' ')'  */
                                                           {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 191: /* expr_new: "new" type_declaration '(' expr_list ')'  */
                                                                                {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 192: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 193: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 194: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 195: /* expression_return: "return" ';'  */
                            {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),nullptr);
    }
    break;

  case 196: /* expression_return: "return" expr ';'  */
                                          {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 197: /* expression_return: "return" "<-" expr ';'  */
                                                 {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 198: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 199: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 200: /* expression_yield: "yield" expr ';'  */
                                         {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 201: /* expression_yield: "yield" "<-" expr ';'  */
                                                {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 202: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 203: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 204: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 205: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 206: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 207: /* expression_let: kwd_let let_variable_declaration  */
                                               {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 208: /* $@3: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 209: /* $@4: %empty  */
                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 210: /* expr_cast: "cast" '<' $@3 type_declaration '>' $@4 expr  */
                                                                                                                                     {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 211: /* $@5: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 212: /* $@6: %empty  */
                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 213: /* expr_cast: "upcast" '<' $@5 type_declaration '>' $@6 expr  */
                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 214: /* $@7: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 215: /* $@8: %empty  */
                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 216: /* expr_cast: "reinterpret" '<' $@7 type_declaration '>' $@8 expr  */
                                                                                                                                            {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 217: /* $@9: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 218: /* $@10: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 219: /* expr_type_decl: "type" '<' $@9 type_declaration '>' $@10  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 220: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 221: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 222: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 223: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 224: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 225: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 226: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 227: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 228: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 229: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 230: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 231: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 232: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 233: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 234: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 235: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 236: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 237: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 238: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 239: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 240: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 241: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 242: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 243: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 244: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 245: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 246: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 247: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 248: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 249: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 250: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 251: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 252: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 253: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 254: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 255: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 256: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 257: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 258: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 259: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 260: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 261: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 262: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 263: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 264: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 265: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 266: /* expr_assign_pipe: expr '=' "@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 267: /* expr_assign_pipe: expr '=' "@@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 268: /* expr_assign_pipe: expr "<-" "@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 269: /* expr_assign_pipe: expr "<-" "@@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 270: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 271: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 272: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 273: /* func_addr_expr: '@' '@' name_in_namespace  */
                                                 {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 274: /* $@11: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 275: /* $@12: %empty  */
                                                                                          { yyextra->das_arrow_depth --; }
    break;

  case 276: /* func_addr_expr: '@' '@' '<' $@11 type_declaration '>' $@12 name_in_namespace  */
                                                                                                                                                   {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
    break;

  case 277: /* $@13: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 278: /* $@14: %empty  */
                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 279: /* func_addr_expr: '@' '@' '<' $@13 optional_function_argument_list optional_function_type '>' $@14 name_in_namespace  */
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

  case 280: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 281: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 282: /* $@15: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 283: /* $@16: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 284: /* expr_field: expr '.' $@15 error $@16  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 285: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 286: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 287: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 288: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 289: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 290: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 291: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 292: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 293: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 294: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 295: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 296: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 297: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 298: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 299: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 300: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 301: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 302: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 303: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 304: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 305: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 306: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 307: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 308: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 309: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 310: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 311: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 312: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 313: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 314: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 315: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 316: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 317: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 318: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 319: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 320: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 323: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 324: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 325: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 326: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 327: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 328: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 329: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 330: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 331: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 332: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 333: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 334: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 335: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 336: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 338: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 339: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' ')'  */
                                                                                                   {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 340: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' expr ')'  */
                                                                                                                 {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 341: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 343: /* $@17: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 344: /* $@18: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 345: /* expr: expr "is" "type" '<' $@17 type_declaration '>' $@18  */
                                                                                                                                            {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 346: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 347: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 348: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 349: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 350: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 351: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 352: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 353: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 354: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 355: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 356: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 357: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) =ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 358: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 359: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 360: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 361: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 362: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 363: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 364: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 365: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 366: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 367: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 368: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 369: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 370: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 371: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 372: /* struct_variable_declaration_list: struct_variable_declaration_list structure_variable_declaration ';'  */
                                                                                        {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
    break;

  case 373: /* $@19: %empty  */
                                                                                             { yyextra->das_force_oxford_comma=true; }
    break;

  case 374: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@19 function_declaration_header ';'  */
                                                                                                                                                                             {
        (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
    }
    break;

  case 375: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant function_declaration_header expression_block  */
                                                                          {
        (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-3].i),(yyvsp[-2].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-5]),(yylsp[0])),tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 376: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 377: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 378: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 379: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 380: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 381: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 382: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 383: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 384: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 385: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 386: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 387: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 388: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 389: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 390: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 391: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 392: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 393: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 394: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 395: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 396: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 397: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 398: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 399: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 400: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 401: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration ';'  */
                                                                             {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 402: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr ';'  */
                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 403: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr_pipe  */
                                                                                                                    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 404: /* let_variable_declaration: variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                      {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 405: /* let_variable_declaration: variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 406: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 407: /* global_variable_declaration_list: global_variable_declaration_list optional_field_annotation let_variable_declaration  */
                                                                                                             {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
    break;

  case 408: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 409: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 410: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 411: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 412: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 413: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 414: /* $@20: %empty  */
                                                                                        { yyextra->das_force_oxford_comma=true; }
    break;

  case 415: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@20 optional_field_annotation let_variable_declaration  */
                                                                                                                                                                                                {
        ast_globalLet(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].aaList),(yyvsp[0].pVarDecl));
    }
    break;

  case 416: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 417: /* enum_list: enum_list "name" ';'  */
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

  case 418: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 419: /* single_alias: "name" '=' type_declaration  */
                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !yyextra->g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
    break;

  case 424: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 425: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 426: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 427: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum "name" '{' enum_list '}'  */
                                                                                                                            {
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pEnum),tokAt(scanner,(yylsp[-1])),Type::tInt);
    }
    break;

  case 428: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum "name" ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                 {
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].s),tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pEnum),tokAt(scanner,(yylsp[-1])),(yyvsp[-3].type));
    }
    break;

  case 429: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 430: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 431: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 432: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 433: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 434: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 435: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 436: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 437: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 438: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 439: /* $@21: %empty  */
        { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
    break;

  case 440: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure structure_name $@21 '{' struct_variable_declaration_list '}'  */
                                                                                                                                  {
        ast_structureDeclaration ( scanner, (yyvsp[-7].faList), tokAt(scanner,(yylsp[-6])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
    }
    break;

  case 441: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 442: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 443: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 444: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 445: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 446: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 447: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 448: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 449: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 450: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 451: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 452: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 453: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 454: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 455: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 456: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 457: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 458: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 459: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 460: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 461: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 462: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 463: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 464: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 465: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 466: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 467: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 468: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 469: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 470: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 471: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 472: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 473: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 474: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 475: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 476: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 477: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 478: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 479: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 480: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 481: /* $@22: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 482: /* $@23: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 483: /* bitfield_type_declaration: "bitfield" '<' $@22 bitfield_bits '>' $@23  */
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

  case 484: /* type_declaration: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 485: /* type_declaration: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 486: /* type_declaration: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 487: /* type_declaration: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 488: /* type_declaration: type_declaration '[' expr ']'  */
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

  case 489: /* type_declaration: type_declaration '[' ']'  */
                                           {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 490: /* type_declaration: type_declaration '-' '[' ']'  */
                                               {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 491: /* type_declaration: type_declaration "explicit"  */
                                                {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 492: /* type_declaration: type_declaration "const"  */
                                             {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 493: /* type_declaration: type_declaration '-' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 494: /* type_declaration: type_declaration '&'  */
                                       {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 495: /* type_declaration: type_declaration '-' '&'  */
                                           {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 496: /* type_declaration: type_declaration '#'  */
                                       {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 497: /* type_declaration: type_declaration "implicit"  */
                                                {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 498: /* type_declaration: type_declaration '-' '#'  */
                                           {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 499: /* type_declaration: type_declaration '=' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 500: /* type_declaration: type_declaration '?'  */
                                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 501: /* $@24: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 502: /* $@25: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 503: /* type_declaration: "smart_ptr" '<' $@24 type_declaration '>' $@25  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 504: /* type_declaration: type_declaration "??"  */
                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 505: /* $@26: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 506: /* $@27: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 507: /* type_declaration: "array" '<' $@26 type_declaration '>' $@27  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 508: /* $@28: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 509: /* $@29: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 510: /* type_declaration: "table" '<' $@28 type_declaration ';' type_declaration '>' $@29  */
                                                                                                                                                                   {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 511: /* $@30: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 512: /* $@31: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 513: /* type_declaration: "iterator" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 514: /* type_declaration: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 515: /* $@32: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 516: /* $@33: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 517: /* type_declaration: "block" '<' $@32 type_declaration '>' $@33  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 518: /* $@34: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 519: /* $@35: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 520: /* type_declaration: "block" '<' $@34 optional_function_argument_list optional_function_type '>' $@35  */
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

  case 521: /* type_declaration: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 522: /* $@36: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 523: /* $@37: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 524: /* type_declaration: "function" '<' $@36 type_declaration '>' $@37  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 525: /* $@38: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 526: /* $@39: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 527: /* type_declaration: "function" '<' $@38 optional_function_argument_list optional_function_type '>' $@39  */
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

  case 528: /* type_declaration: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 529: /* $@40: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 530: /* $@41: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 531: /* type_declaration: "lambda" '<' $@40 type_declaration '>' $@41  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 532: /* $@42: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 533: /* $@43: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 534: /* type_declaration: "lambda" '<' $@42 optional_function_argument_list optional_function_type '>' $@43  */
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

  case 535: /* $@44: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 536: /* $@45: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 537: /* type_declaration: "tuple" '<' $@44 tuple_type_list '>' $@45  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 538: /* $@46: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 539: /* $@47: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 540: /* type_declaration: "variant" '<' $@46 variant_type_list '>' $@47  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 541: /* $@48: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 542: /* variant_alias_declaration: "variant" $@48 "name" '{' variant_type_list ';' '}'  */
                                                                                                               {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-4].s);
        vtype->at = tokAt(scanner,(yylsp[-4]));
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-4].s),tokAt(scanner,(yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
    }
    break;

  case 543: /* $@49: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 544: /* bitfield_alias_declaration: "bitfield" $@49 "name" '{' bitfield_bits ';' '}'  */
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

  case 545: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 546: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 547: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 548: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 549: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 550: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 551: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 552: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 553: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 554: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 555: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 556: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 557: /* make_struct_decl: "[[" type_declaration make_struct_dim optional_block ']' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 558: /* make_struct_decl: "[[" type_declaration optional_block ']' ']'  */
                                                                        {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 559: /* make_struct_decl: "[[" type_declaration '(' ')' optional_block ']' ']'  */
                                                                                {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 560: /* make_struct_decl: "[[" type_declaration '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                     {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 561: /* make_struct_decl: "[{" type_declaration make_struct_dim optional_block '}' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 562: /* make_struct_decl: "[{" type_declaration '(' ')' make_struct_dim optional_block '}' ']'  */
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

  case 563: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 564: /* make_tuple: make_tuple ',' expr  */
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

  case 565: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 566: /* make_any_tuple: make_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 567: /* make_any_tuple: make_map_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 568: /* make_dim: make_any_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 569: /* make_dim: make_dim ';' make_any_tuple  */
                                              {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 570: /* make_dim_decl: "[[" type_declaration make_dim ']' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 571: /* make_dim_decl: "[{" type_declaration make_dim '}' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 572: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 573: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 574: /* make_table_decl: "{{" make_table '}' '}'  */
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

  case 575: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 576: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 577: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 578: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


