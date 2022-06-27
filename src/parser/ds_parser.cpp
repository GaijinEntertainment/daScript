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
  YYSYMBOL_BRABRAB = 147,                  /* "[["  */
  YYSYMBOL_BRACBRB = 148,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 149,                  /* "{{"  */
  YYSYMBOL_INTEGER = 150,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 151,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 152,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 153,    /* "unsigned long integer constant"  */
  YYSYMBOL_FLOAT = 154,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 155,                   /* "double constant"  */
  YYSYMBOL_NAME = 156,                     /* "name"  */
  YYSYMBOL_BEGIN_STRING = 157,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 158,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 159,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 160,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 161,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 162,          /* "}"  */
  YYSYMBOL_END_OF_READ = 163,              /* "end of failed eader macro"  */
  YYSYMBOL_164_ = 164,                     /* ','  */
  YYSYMBOL_165_ = 165,                     /* '='  */
  YYSYMBOL_166_ = 166,                     /* '?'  */
  YYSYMBOL_167_ = 167,                     /* ':'  */
  YYSYMBOL_168_ = 168,                     /* '|'  */
  YYSYMBOL_169_ = 169,                     /* '^'  */
  YYSYMBOL_170_ = 170,                     /* '&'  */
  YYSYMBOL_171_ = 171,                     /* '<'  */
  YYSYMBOL_172_ = 172,                     /* '>'  */
  YYSYMBOL_173_ = 173,                     /* '-'  */
  YYSYMBOL_174_ = 174,                     /* '+'  */
  YYSYMBOL_175_ = 175,                     /* '*'  */
  YYSYMBOL_176_ = 176,                     /* '/'  */
  YYSYMBOL_177_ = 177,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 178,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 179,               /* UNARY_PLUS  */
  YYSYMBOL_180_ = 180,                     /* '~'  */
  YYSYMBOL_181_ = 181,                     /* '!'  */
  YYSYMBOL_PRE_INC = 182,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 183,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 184,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 185,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 186,                    /* DEREF  */
  YYSYMBOL_187_ = 187,                     /* '.'  */
  YYSYMBOL_188_ = 188,                     /* '['  */
  YYSYMBOL_189_ = 189,                     /* ']'  */
  YYSYMBOL_190_ = 190,                     /* '('  */
  YYSYMBOL_191_ = 191,                     /* ')'  */
  YYSYMBOL_192_ = 192,                     /* '$'  */
  YYSYMBOL_193_ = 193,                     /* '@'  */
  YYSYMBOL_194_ = 194,                     /* ';'  */
  YYSYMBOL_195_ = 195,                     /* '{'  */
  YYSYMBOL_196_ = 196,                     /* '}'  */
  YYSYMBOL_197_ = 197,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 198,                 /* $accept  */
  YYSYMBOL_program = 199,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 200, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 201,              /* module_name  */
  YYSYMBOL_module_declaration = 202,       /* module_declaration  */
  YYSYMBOL_character_sequence = 203,       /* character_sequence  */
  YYSYMBOL_string_constant = 204,          /* string_constant  */
  YYSYMBOL_string_builder_body = 205,      /* string_builder_body  */
  YYSYMBOL_string_builder = 206,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 207, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 208,              /* expr_reader  */
  YYSYMBOL_209_1 = 209,                    /* $@1  */
  YYSYMBOL_options_declaration = 210,      /* options_declaration  */
  YYSYMBOL_require_declaration = 211,      /* require_declaration  */
  YYSYMBOL_require_module_name = 212,      /* require_module_name  */
  YYSYMBOL_require_module = 213,           /* require_module  */
  YYSYMBOL_is_public_module = 214,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 215,       /* expect_declaration  */
  YYSYMBOL_expect_list = 216,              /* expect_list  */
  YYSYMBOL_expect_error = 217,             /* expect_error  */
  YYSYMBOL_expression_label = 218,         /* expression_label  */
  YYSYMBOL_expression_goto = 219,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 220,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 221,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 222,          /* if_or_static_if  */
  YYSYMBOL_expression_if_then_else = 223,  /* expression_if_then_else  */
  YYSYMBOL_expression_for_loop = 224,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 225,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 226,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 227,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 228,    /* expression_with_alias  */
  YYSYMBOL_229_2 = 229,                    /* $@2  */
  YYSYMBOL_annotation_argument_value = 230, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 231, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 232, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 233,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 234, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 235, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration = 236,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 237,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 238, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 239, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 240,   /* optional_function_type  */
  YYSYMBOL_function_name = 241,            /* function_name  */
  YYSYMBOL_global_function_declaration = 242, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 243, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 244, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 245,     /* function_declaration  */
  YYSYMBOL_246_3 = 246,                    /* $@3  */
  YYSYMBOL_expression_block = 247,         /* expression_block  */
  YYSYMBOL_expression_any = 248,           /* expression_any  */
  YYSYMBOL_expressions = 249,              /* expressions  */
  YYSYMBOL_expr_pipe = 250,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 251,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 252,        /* expression_delete  */
  YYSYMBOL_expr_new = 253,                 /* expr_new  */
  YYSYMBOL_expression_break = 254,         /* expression_break  */
  YYSYMBOL_expression_continue = 255,      /* expression_continue  */
  YYSYMBOL_expression_return = 256,        /* expression_return  */
  YYSYMBOL_expression_yield = 257,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 258,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 259,                  /* kwd_let  */
  YYSYMBOL_expression_let = 260,           /* expression_let  */
  YYSYMBOL_expr_cast = 261,                /* expr_cast  */
  YYSYMBOL_262_4 = 262,                    /* $@4  */
  YYSYMBOL_263_5 = 263,                    /* $@5  */
  YYSYMBOL_264_6 = 264,                    /* $@6  */
  YYSYMBOL_265_7 = 265,                    /* $@7  */
  YYSYMBOL_266_8 = 266,                    /* $@8  */
  YYSYMBOL_267_9 = 267,                    /* $@9  */
  YYSYMBOL_expr_type_decl = 268,           /* expr_type_decl  */
  YYSYMBOL_269_10 = 269,                   /* $@10  */
  YYSYMBOL_270_11 = 270,                   /* $@11  */
  YYSYMBOL_expr_type_info = 271,           /* expr_type_info  */
  YYSYMBOL_expr_list = 272,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 273,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 274,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 275,            /* capture_entry  */
  YYSYMBOL_capture_list = 276,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 277,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 278,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 279,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 280,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 281,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 282,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 283,         /* expr_method_call  */
  YYSYMBOL_func_addr_expr = 284,           /* func_addr_expr  */
  YYSYMBOL_285_12 = 285,                   /* $@12  */
  YYSYMBOL_286_13 = 286,                   /* $@13  */
  YYSYMBOL_287_14 = 287,                   /* $@14  */
  YYSYMBOL_288_15 = 288,                   /* $@15  */
  YYSYMBOL_expr_field = 289,               /* expr_field  */
  YYSYMBOL_290_16 = 290,                   /* $@16  */
  YYSYMBOL_291_17 = 291,                   /* $@17  */
  YYSYMBOL_expr = 292,                     /* expr  */
  YYSYMBOL_293_18 = 293,                   /* $@18  */
  YYSYMBOL_294_19 = 294,                   /* $@19  */
  YYSYMBOL_295_20 = 295,                   /* $@20  */
  YYSYMBOL_296_21 = 296,                   /* $@21  */
  YYSYMBOL_297_22 = 297,                   /* $@22  */
  YYSYMBOL_298_23 = 298,                   /* $@23  */
  YYSYMBOL_expr_mtag = 299,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 300, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 301,        /* optional_override  */
  YYSYMBOL_optional_constant = 302,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 303, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 304, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 305, /* struct_variable_declaration_list  */
  YYSYMBOL_306_24 = 306,                   /* $@24  */
  YYSYMBOL_307_25 = 307,                   /* $@25  */
  YYSYMBOL_308_26 = 308,                   /* $@26  */
  YYSYMBOL_function_argument_declaration = 309, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 310,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 311,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 312,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 313,             /* variant_type  */
  YYSYMBOL_variant_type_list = 314,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 315,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 316,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 317,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 318,             /* optional_ref  */
  YYSYMBOL_let_variable_declaration = 319, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 320, /* global_variable_declaration_list  */
  YYSYMBOL_321_27 = 321,                   /* $@27  */
  YYSYMBOL_optional_shared = 322,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 323, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 324,               /* global_let  */
  YYSYMBOL_325_28 = 325,                   /* $@28  */
  YYSYMBOL_enum_list = 326,                /* enum_list  */
  YYSYMBOL_single_alias = 327,             /* single_alias  */
  YYSYMBOL_328_29 = 328,                   /* $@29  */
  YYSYMBOL_alias_list = 329,               /* alias_list  */
  YYSYMBOL_alias_declaration = 330,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 331, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 332,                /* enum_name  */
  YYSYMBOL_enum_declaration = 333,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 334, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 335,          /* optional_sealed  */
  YYSYMBOL_structure_name = 336,           /* structure_name  */
  YYSYMBOL_class_or_struct = 337,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 338, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 339,    /* structure_declaration  */
  YYSYMBOL_340_30 = 340,                   /* $@30  */
  YYSYMBOL_341_31 = 341,                   /* $@31  */
  YYSYMBOL_variable_name_with_pos_list = 342, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 343,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 344, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 345, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 346,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 347,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 348, /* bitfield_type_declaration  */
  YYSYMBOL_349_32 = 349,                   /* $@32  */
  YYSYMBOL_350_33 = 350,                   /* $@33  */
  YYSYMBOL_table_type_pair = 351,          /* table_type_pair  */
  YYSYMBOL_type_declaration = 352,         /* type_declaration  */
  YYSYMBOL_353_34 = 353,                   /* $@34  */
  YYSYMBOL_354_35 = 354,                   /* $@35  */
  YYSYMBOL_355_36 = 355,                   /* $@36  */
  YYSYMBOL_356_37 = 356,                   /* $@37  */
  YYSYMBOL_357_38 = 357,                   /* $@38  */
  YYSYMBOL_358_39 = 358,                   /* $@39  */
  YYSYMBOL_359_40 = 359,                   /* $@40  */
  YYSYMBOL_360_41 = 360,                   /* $@41  */
  YYSYMBOL_361_42 = 361,                   /* $@42  */
  YYSYMBOL_362_43 = 362,                   /* $@43  */
  YYSYMBOL_363_44 = 363,                   /* $@44  */
  YYSYMBOL_364_45 = 364,                   /* $@45  */
  YYSYMBOL_365_46 = 365,                   /* $@46  */
  YYSYMBOL_366_47 = 366,                   /* $@47  */
  YYSYMBOL_367_48 = 367,                   /* $@48  */
  YYSYMBOL_368_49 = 368,                   /* $@49  */
  YYSYMBOL_369_50 = 369,                   /* $@50  */
  YYSYMBOL_370_51 = 370,                   /* $@51  */
  YYSYMBOL_371_52 = 371,                   /* $@52  */
  YYSYMBOL_372_53 = 372,                   /* $@53  */
  YYSYMBOL_373_54 = 373,                   /* $@54  */
  YYSYMBOL_374_55 = 374,                   /* $@55  */
  YYSYMBOL_375_56 = 375,                   /* $@56  */
  YYSYMBOL_376_57 = 376,                   /* $@57  */
  YYSYMBOL_variant_alias_declaration = 377, /* variant_alias_declaration  */
  YYSYMBOL_378_58 = 378,                   /* $@58  */
  YYSYMBOL_379_59 = 379,                   /* $@59  */
  YYSYMBOL_bitfield_alias_declaration = 380, /* bitfield_alias_declaration  */
  YYSYMBOL_381_60 = 381,                   /* $@60  */
  YYSYMBOL_make_decl = 382,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 383,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 384,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 385,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 386,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 387,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 388,           /* make_map_tuple  */
  YYSYMBOL_make_any_tuple = 389,           /* make_any_tuple  */
  YYSYMBOL_make_dim = 390,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 391,            /* make_dim_decl  */
  YYSYMBOL_make_table = 392,               /* make_table  */
  YYSYMBOL_make_table_decl = 393,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 394, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 395       /* array_comprehension  */
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
#define YYLAST   9181

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  198
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  198
/* YYNRULES -- Number of rules.  */
#define YYNRULES  611
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1072

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   425


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
       2,     2,     2,   181,     2,   197,   192,   177,   170,     2,
     190,   191,   175,   174,   164,   173,   187,   176,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   167,   194,
     171,   165,   172,   166,   193,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   188,     2,   189,   169,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   195,   168,   196,   180,     2,     2,     2,
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
     155,   156,   157,   158,   159,   160,   161,   162,   163,   178,
     179,   182,   183,   184,   185,   186
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   468,   468,   469,   470,   471,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   484,   485,   486,   490,   491,
     495,   513,   514,   515,   516,   520,   524,   529,   538,   546,
     562,   567,   575,   575,   605,   626,   630,   633,   637,   643,
     652,   655,   661,   662,   666,   670,   671,   675,   678,   684,
     690,   693,   699,   700,   704,   705,   706,   715,   716,   720,
     729,   735,   743,   753,   762,   762,   769,   770,   771,   772,
     773,   774,   778,   783,   791,   792,   793,   797,   798,   799,
     800,   801,   802,   803,   804,   810,   813,   819,   820,   821,
     825,   833,   846,   850,   857,   858,   862,   863,   864,   868,
     871,   878,   882,   883,   884,   885,   886,   887,   888,   889,
     890,   891,   892,   893,   894,   895,   896,   897,   898,   899,
     900,   901,   902,   903,   904,   905,   906,   907,   908,   909,
     910,   911,   912,   913,   914,   915,   916,   917,   918,   919,
     920,   921,   922,   923,   924,   925,   926,   927,   928,   929,
     930,   931,   932,   933,   934,   938,   960,   961,   962,   966,
     972,   972,   989,   993,  1004,  1005,  1006,  1007,  1008,  1009,
    1010,  1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,
    1020,  1021,  1022,  1023,  1027,  1032,  1038,  1044,  1078,  1081,
    1087,  1088,  1099,  1103,  1109,  1112,  1115,  1119,  1125,  1129,
    1133,  1136,  1139,  1144,  1147,  1155,  1158,  1163,  1166,  1174,
    1180,  1181,  1185,  1191,  1191,  1191,  1194,  1194,  1194,  1199,
    1199,  1199,  1207,  1207,  1207,  1213,  1223,  1234,  1249,  1252,
    1258,  1259,  1266,  1277,  1278,  1279,  1283,  1284,  1285,  1286,
    1290,  1295,  1303,  1304,  1308,  1313,  1320,  1321,  1322,  1323,
    1324,  1325,  1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,
    1337,  1338,  1339,  1340,  1341,  1342,  1343,  1344,  1345,  1346,
    1347,  1351,  1352,  1353,  1354,  1358,  1369,  1374,  1384,  1388,
    1388,  1388,  1395,  1395,  1395,  1409,  1413,  1417,  1417,  1417,
    1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,
    1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,  1443,
    1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,  1453,
    1454,  1455,  1456,  1457,  1458,  1459,  1465,  1466,  1467,  1468,
    1469,  1470,  1471,  1472,  1473,  1474,  1475,  1476,  1477,  1481,
    1485,  1488,  1491,  1492,  1493,  1494,  1497,  1500,  1501,  1504,
    1504,  1504,  1507,  1512,  1516,  1520,  1520,  1520,  1525,  1528,
    1532,  1532,  1532,  1537,  1540,  1541,  1542,  1543,  1544,  1545,
    1546,  1547,  1548,  1549,  1550,  1558,  1559,  1560,  1561,  1562,
    1563,  1567,  1574,  1575,  1579,  1580,  1581,  1585,  1586,  1590,
    1591,  1592,  1596,  1606,  1609,  1609,  1628,  1627,  1642,  1641,
    1654,  1663,  1668,  1676,  1677,  1681,  1684,  1693,  1694,  1698,
    1707,  1708,  1713,  1714,  1718,  1724,  1730,  1733,  1737,  1743,
    1752,  1753,  1754,  1758,  1759,  1763,  1766,  1771,  1776,  1784,
    1795,  1798,  1798,  1818,  1819,  1823,  1824,  1825,  1829,  1832,
    1832,  1851,  1854,  1863,  1876,  1876,  1897,  1898,  1902,  1903,
    1907,  1908,  1909,  1913,  1923,  1930,  1940,  1941,  1945,  1946,
    1950,  1956,  1957,  1961,  1962,  1963,  1967,  1972,  1967,  1982,
    1989,  1998,  2004,  2015,  2016,  2017,  2018,  2019,  2020,  2021,
    2022,  2023,  2024,  2025,  2026,  2027,  2028,  2029,  2030,  2031,
    2032,  2033,  2034,  2035,  2036,  2037,  2038,  2039,  2043,  2044,
    2045,  2046,  2047,  2048,  2052,  2063,  2067,  2074,  2086,  2093,
    2102,  2102,  2102,  2115,  2119,  2126,  2127,  2128,  2129,  2130,
    2144,  2150,  2154,  2158,  2163,  2168,  2173,  2178,  2182,  2186,
    2191,  2195,  2200,  2200,  2200,  2206,  2213,  2213,  2213,  2218,
    2218,  2218,  2224,  2224,  2224,  2229,  2233,  2233,  2233,  2238,
    2238,  2238,  2247,  2251,  2251,  2251,  2256,  2256,  2256,  2265,
    2269,  2269,  2269,  2274,  2274,  2274,  2283,  2283,  2283,  2289,
    2289,  2289,  2298,  2298,  2298,  2322,  2322,  2342,  2343,  2344,
    2345,  2349,  2356,  2363,  2369,  2378,  2383,  2390,  2391,  2395,
    2401,  2408,  2416,  2423,  2431,  2443,  2446,  2460,  2466,  2472,
    2473,  2477,  2482,  2489,  2494,  2504,  2509,  2516,  2528,  2529,
    2533,  2536
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
  "\"$a\"", "\"$t\"", "\"$c\"", "\"[[\"", "\"[{\"", "\"{{\"",
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
  "$@20", "$@21", "$@22", "$@23", "expr_mtag", "optional_field_annotation",
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
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,    44,    61,    63,    58,   124,    94,
      38,    60,    62,    45,    43,    42,    47,    37,   419,   420,
     126,    33,   421,   422,   423,   424,   425,    46,    91,    93,
      40,    41,    36,    64,    59,   123,   125,    35
};
#endif

#define YYPACT_NINF (-968)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-564)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -968,    24,  -968,  -968,    21,   -21,   185,  -100,  -968,   -53,
    -968,  -968,    31,  -968,  -968,  -968,  -968,  -968,   221,  -968,
      73,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,
     -24,  -968,   117,    28,   179,  -968,  -968,   185,    30,  -968,
    -968,   196,  -968,  -968,  -968,    73,   240,   244,  -968,  -968,
     251,   225,   197,  -968,    22,  -968,  -968,  -968,   135,   239,
     309,  -968,   353,    14,    21,   268,   -21,   228,   264,  -968,
     266,   269,  -968,   286,   235,   -77,   388,   260,  -968,  -968,
     278,    21,    31,  -968,  -968,  -968,  -968,  -968,  -968,  -968,
     314,  -968,  -968,  -968,  -968,  -968,   263,  -968,  -968,  -968,
    -968,  -968,    90,    35,  -968,  -968,  -968,  -968,   410,  -968,
    -968,  8932,  -968,  -968,   277,  -968,  -968,  -968,   325,   281,
    -968,   -47,  -968,     0,  -968,    68,   385,  -968,   337,  -968,
    -968,   320,  -968,  -968,  -968,  -968,  -968,  -968,  -968,   -40,
    -968,   315,   316,   317,   318,  -968,  -968,  -968,   301,  -968,
    -968,  -968,  -968,  -968,   323,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,   326,  -968,  -968,  -968,   327,   328,
    -968,  -968,  -968,  -968,   329,   330,   312,  -968,  -968,  -968,
    -968,  -968,  3320,  -968,  -968,   310,   350,  -968,  7742,   474,
     478,  -968,   336,   332,   341,  -968,  -968,   357,  -968,   319,
      21,   364,  -968,  -968,  -968,    35,  -968,  -968,  -968,  -968,
    -968,   372,  -968,   186,   231,   234,  -968,  -968,  5100,  -968,
    -968,  -968,  -968,   497,  -968,  -968,    62,  3176,  -968,   -58,
     363,  -968,   338,  -968,  -968,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,  -968,  -968,   342,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,   512,
    -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,   349,  -968,  -968,   -59,   370,  -968,  -968,
    -968,  -968,  -968,  -968,  -968,  -968,   344,   -43,   373,   346,
    -968,   337,    80,   501,  -968,   280,  -968,  8932,  8932,  8932,
    8932,   352,   325,  8932,   336,  8932,   336,  8932,   336,  9025,
     350,  -968,  -968,  8839,   354,   374,  -968,   356,   376,   379,
     361,   383,   368,  -968,   391,  5100,  5100,   375,   377,   378,
     381,   382,   384,  8653,  8746,  5100,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,  5100,  5100,  5100,   180,  5100,  5100,  5100,
    -968,   371,  -968,  -968,  -968,     3,  -968,  -968,  -968,  -968,
     389,  -968,  -968,  -968,  -968,  -968,  -968,  1200,  -968,   386,
    -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,   367,  -968,
    -968,  7084,  -968,  -968,  8932,   -31,  -968,  -968,  -968,  -968,
      -7,  -968,    86,  8932,  -968,  1404,  -968,    82,  -968,   180,
    -968,  -968,   364,   380,   407,   417,  8932,  -968,   -65,   387,
     394,   555,   759,   823,  -968,  -107,  1098,   370,  1533,   370,
    1703,   370,   -39,  -968,   141,  3320,   156,  1872,  -968,   180,
    -968,  5100,  -968,  -968,  5100,  -968,  5100,  8932,    53,    53,
    5100,  5100,  5100,  5100,  5100,  5100,   364,  1771,   364,  1939,
    7818,  -968,    96,   302,    53,    53,   -61,  -968,    53,    53,
    5810,   104,  -968,  2573,   431,  5882,  6064,  5100,  5100,  -968,
    -968,  5100,  5100,  5100,  5100,   423,  5100,   424,  5100,  5100,
    5100,  5100,  5100,  5100,  5100,  5100,  5100,  3321,  5100,  5100,
    5100,  5100,  5100,  5100,  5100,  5100,  5100,  5100,   -76,  5100,
    -968,  3466,  -968,  -968,  3320,  -968,  -968,   364,  -968,   -63,
    -968,   337,  3320,  -968,  5100,  -968,  -968,   364,  2275,  -968,
     332,  5100,  5100,   426,  -968,   392,   439,  3634,    56,  2428,
     282,   282,  -968,   556,   400,   401,  5100,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,  -968,   402,   403,   404,  -968,  -968,
    -968,   364,  -968,   -80,  -968,  7639,    -2,  5100,  -968,  -968,
      20,  -968,  -968,  -968,   558,   229,  -968,  -968,  -968,  2741,
    -968,  -968,  8932,  -968,  -968,  -968,   444,  -968,   429,  -968,
     430,  -968,   432,  8932,  -968,  9025,  -968,   350,  3779,  3947,
    8932,  5901,  8932,  8932,  5992,  8932,  6083,  2040,  6174,  6265,
    6356,  6447,  6538,  6629,    16,   282,   -49,  2107,  4115,  7818,
     441,   -18,   414,   442,  -968,  -968,   -52,    18,  4283,   -18,
     133,  5100,  5100,   411,  -968,  5100,   201,   450,  -968,   238,
    -968,   454,  -968,    79,  8079,   -50,   336,   440,  -968,  -968,
     445,  -968,  -968,   348,   348,   146,   146,  1169,  1169,   422,
      27,  -968,  7168,   -30,   -30,   348,   348,  8423,  8314,  8347,
    8155,  6246,  7920,  8449,   661,   265,   146,   146,   637,   637,
      27,    27,    27,   459,  5100,  -968,   462,  5100,   618,  7259,
    -968,   118,  -968,  -968,  -968,  8932,  -968,  2741,  -968,   528,
      19,  2741,  -968,  -968,   498,  5198,   608,  8079,   528,   457,
    -968,   456,   475,  8079,  -968,  2741,  -968,  5300,   433,  -968,
    -968,   434,  -968,  -968,   528,  -968,  -968,  -968,  -968,   282,
    -968,  5100,  5100,  5100,  5100,  5100,  5100,  5100,  5100,  5100,
    5100,  2886,  5100,  5100,  5100,  5100,  5100,  5100,  3031,  -968,
     940,    31,  -968,   620,   337,   476,   497,  -968,  2741,  -968,
    5402,  -968,  -968,  3320,  -968,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,  3320,  -968,  -968,  -968,  -968,   119,   483,
    6720,  2106,  -968,  2208,  2506,  -968,  2572,  -968,   431,  -968,
    -968,  -968,  -968,  -968,   455,  5100,  -968,  5100,  5100,    -6,
     484,   454,   461,   463,  5100,   465,  5100,  5100,   454,   435,
     470,  8079,  -968,  -968,  7996,  -968,   472,  8932,   336,   -42,
     115,  5100,  -968,   495,   504,   508,   510,  -968,   160,   370,
    -968,  -968,  4451,  -968,   499,  -968,  -968,  5100,  -968,  7343,
    -968,  7434,  -968,  -968,  -968,  2674,  -968,  7536,  -968,  5100,
    -968,  5504,  5100,  5100,  -968,   332,  -968,  -968,  -968,  -968,
    -968,  5606,  -968,  -968,  -968,   366,  -968,  8079,  8079,  8079,
    8079,  8079,  8079,  8079,  8079,  8079,  8079,  5100,  5100,  8079,
    8079,  8079,  8079,  8079,  8079,  8079,  5100,  5100,  8079,  -968,
     162,   393,    25,   477,  -968,  -968,  5708,  -968,  -968,  -968,
    -968,  -968,   173,  -968,  -968,  -968,  -968,  -968,   479,  4619,
     -34,  8079,  8079,   -18,   485,   113,   441,   486,  -968,  8079,
    -968,  -968,    74,   -18,   487,  -968,  -968,  -968,  2819,   370,
     481,  8079,  -968,  -968,  -968,  -968,   -50,   488,   -85,  8932,
    8932,  -968,   125,  -968,  8238,  -968,  -968,  -968,   497,  5100,
      78,  -968,  -968,  5100,  -968,  1600,   332,  -968,  -968,  5100,
    -968,  8525,  8079,  8525,  8079,   490,  -968,  -968,    37,  -968,
    -968,   393,  -968,  -968,  5100,   522,  -968,  5100,  5100,  5100,
    4787,  -968,   127,  5100,   491,   496,  5100,  5100,  -968,  5100,
     492,  -968,  -968,   507,  -968,  -968,  -968,  4955,  -968,  -968,
    2964,  3109,  -968,  8932,  8079,   332,  8079,  -968,  -968,   528,
    -968,  -968,   636,   364,  6811,   514,   146,   146,   146,  -968,
    6902,  -968,  1060,   500,  -968,  8079,  8079,  1060,   502,   180,
    -968,  5100,  8155,  -968,  -968,  3254,  -968,   366,     0,  -968,
    -968,  -968,  -968,  5100,  -968,   639,   503,  -968,   513,  -968,
    -968,   180,  8238,  -968,  -968,  -968,  -968,   493,     0,  6993,
    5100,   506,   517,  -968,  -968,  -968,   332,  -968,  8079,  -968,
    -968,  -968
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,    94,     1,   210,     0,     0,     0,     0,   211,     0,
     575,   572,     0,    14,     3,    10,     9,     8,     0,     7,
     433,     6,    11,     5,     4,    12,    13,    75,    76,    74,
      83,    85,    34,    47,    44,    45,    36,     0,    42,    35,
     444,     0,   449,    19,    18,   433,     0,     0,    88,    89,
       0,   190,    90,    92,     0,    87,   462,   461,   156,   450,
     463,   434,   435,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,   573,   192,
       0,     0,     0,    95,   158,   157,   160,   155,   452,   451,
       0,   465,   464,   466,   437,   436,   439,    81,    82,    79,
      80,    78,     0,     0,    77,    86,    48,    46,    42,    39,
      38,     0,   446,   448,     0,    16,    17,    20,     0,     0,
     191,     0,    93,     0,   453,     0,   458,   430,   382,    21,
      22,     0,    70,    71,    68,    69,    67,    66,    72,     0,
      41,     0,     0,     0,     0,   473,   493,   474,   505,   475,
     479,   480,   481,   482,   497,   486,   487,   488,   489,   490,
     491,   492,   494,   495,   545,   478,   485,   496,   552,   559,
     476,   483,   477,   484,     0,     0,     0,   504,   515,   518,
     516,   517,   445,   447,   508,     0,     0,    91,     0,     0,
       0,   101,    96,     0,     0,   441,   459,     0,   467,   431,
       0,     0,    23,    24,    25,     0,    84,   536,   539,   542,
     532,     0,   510,   546,   553,   560,   566,   569,     0,   523,
     528,   522,   535,     0,   531,   525,     0,     0,   527,     0,
       0,   410,     0,   152,   153,   150,   104,   105,   107,   106,
     108,   109,   110,   111,   137,   138,   135,   136,   128,   139,
     140,   129,   126,   127,   151,   148,     0,   149,   141,   142,
     143,   144,   115,   116,   117,   112,   113,   114,   125,     0,
     131,   132,   130,   123,   124,   119,   118,   120,   121,   122,
     103,   102,   147,     0,   133,   134,   382,    99,   184,   161,
     498,   501,   499,   502,   500,   503,     0,     0,   456,     0,
     438,   382,     0,   469,   440,   423,    73,     0,     0,     0,
       0,     0,     0,     0,    96,     0,    96,     0,    96,     0,
       0,   296,   297,     0,     0,     0,   290,     0,     0,     0,
       0,     0,     0,   497,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,   248,   247,   249,
     250,   251,    26,     0,     0,     0,     0,     0,     0,     0,
     233,   234,   294,   293,   244,   291,   367,   366,   365,   364,
      94,   370,   292,   369,   368,   337,   298,     0,   299,     0,
     295,   577,   578,   579,   580,   530,   524,   526,     0,   529,
     520,     0,   509,   576,     0,     0,   146,   154,   145,    97,
       0,   403,     0,     0,   159,     0,   441,     0,   454,     0,
     460,   393,     0,     0,     0,     0,     0,   424,     0,     0,
       0,   513,     0,     0,   506,     0,     0,    99,     0,    99,
       0,    99,   190,   407,     0,   405,     0,   194,   197,     0,
     222,     0,   213,   216,     0,   219,     0,     0,   326,   327,
       0,     0,     0,     0,     0,     0,     0,   587,     0,     0,
     598,   605,     0,     0,   303,   302,   342,    32,   301,   300,
       0,   235,   373,     0,   242,     0,     0,     0,     0,   328,
     329,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   287,     0,
     507,     0,   521,   519,   409,   574,   411,     0,   401,   414,
      98,   382,   100,   186,     0,    57,    58,     0,     0,   198,
       0,     0,     0,     0,   199,     0,     0,     0,     0,     0,
       0,     0,   164,   162,     0,     0,     0,   179,   174,   171,
     170,   172,   173,   185,   165,     0,     0,     0,   177,   178,
     180,     0,   169,     0,   166,   252,     0,     0,   442,   457,
     394,   432,   383,   470,   471,     0,   421,   422,   420,     0,
     537,   540,     0,   543,   533,   511,     0,   547,     0,   554,
       0,   561,     0,     0,   567,     0,   570,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   190,     0,     0,   595,
     585,   587,     0,   599,   600,   601,     0,     0,     0,   587,
       0,     0,     0,     0,    29,     0,    27,     0,   330,   279,
     278,     0,   338,     0,   228,     0,    96,     0,   353,   352,
       0,   354,   358,   304,   305,   317,   318,   315,   316,     0,
     347,   335,     0,   371,   372,   306,   307,   322,   323,   324,
     325,     0,     0,   320,   321,   319,   313,   314,   309,   308,
     310,   311,   312,     0,     0,   285,     0,     0,     0,     0,
     340,     0,   402,   413,   412,     0,   415,     0,   404,     0,
       0,     0,   200,   203,     0,   252,     0,   193,     0,     0,
     183,     0,     0,    51,    61,     0,   207,   252,   234,   188,
     189,     0,   181,   182,     0,   168,   175,   176,   212,     0,
     167,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   455,
       0,     0,   468,     0,   382,     0,   420,   425,     0,   429,
     252,   538,   541,   514,   544,   534,   512,   548,   550,   555,
     557,   562,   564,   406,   568,   408,   571,   195,     0,     0,
       0,     0,   343,     0,     0,   344,     0,   374,   242,   375,
     376,   377,   378,   379,     0,     0,   588,     0,     0,   587,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,   606,   607,     0,    30,    33,     0,    96,     0,
       0,     0,   339,     0,     0,     0,     0,   240,     0,    99,
     349,   355,     0,   333,     0,   359,   363,     0,   336,     0,
     286,     0,   288,   331,   341,   416,   419,   418,    62,     0,
     204,   252,     0,     0,   201,     0,    63,    64,    49,    50,
     208,   252,   205,   235,   184,    54,   187,   262,   263,   265,
     264,   266,   256,   257,   258,   267,   268,     0,     0,   254,
     255,   269,   270,   259,   260,   261,     0,     0,   253,   443,
       0,   389,   384,     0,   472,   427,   252,   428,   551,   558,
     565,   196,     0,   225,   223,   214,   217,   220,     0,     0,
       0,   582,   581,   587,     0,     0,   586,     0,   590,   596,
     603,   602,     0,   587,     0,   604,    28,    31,     0,    99,
       0,   229,   238,   239,   237,   236,     0,     0,     0,     0,
       0,   276,     0,   360,   348,   334,   332,   289,   412,     0,
       0,   202,   209,     0,   206,     0,     0,    52,    53,     0,
      59,   273,   274,   271,   272,    95,   390,   391,   384,   385,
     386,   389,   395,   426,     0,     0,   224,     0,     0,     0,
       0,   380,     0,     0,     0,     0,     0,     0,   589,     0,
       0,   593,   280,     0,   275,   241,   243,     0,   230,   245,
       0,     0,   277,     0,   417,     0,    65,   163,    55,     0,
     400,   396,   387,     0,     0,     0,   215,   218,   221,   345,
       0,   381,   608,     0,   591,   584,   583,   608,     0,     0,
     283,     0,   231,   350,   356,     0,    60,    54,     0,   388,
     398,   392,   226,     0,   346,     0,     0,   592,     0,   594,
     281,     0,   232,   351,   357,   361,    56,     0,     0,     0,
       0,     0,     0,   284,   362,   397,     0,   227,   609,   610,
     611,   399
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -968,  -968,  -968,  -968,  -968,   230,   645,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,   673,  -968,   603,  -968,  -968,   646,
    -968,  -968,  -968,  -324,  -968,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,   509,  -968,  -968,   653,   -45,  -968,   640,   -33,
    -350,  -308,  -418,  -968,  -968,  -968,  -967,  -968,  -968,  -190,
    -968,  -145,  -513,   -12,  -968,  -968,  -968,  -968,  -968,  -968,
    -968,   723,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,  -493,  -968,  -968,  -210,  -968,   -56,
    -519,  -968,  -400,  -968,  -968,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,   406,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -126,  -241,  -968,  -242,  -968,  -968,  -968,  -968,
    -968,   209,  -968,   138,  -968,  -378,   415,  -503,  -510,   159,
    -968,  -380,  -968,  -968,   691,  -968,  -968,  -968,   331,    65,
    -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,  -396,  -110,  -968,  -968,  -968,   428,
    -968,  -968,  -968,  -968,   -86,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,  -968,
    -968,  -968,  -968,  -968,   421,  -622,  -445,  -598,  -968,  -968,
    -333,   -68,   288,  -968,  -968,  -968,  -277,  -968
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   117,    45,    14,   131,   137,   463,   362,   816,
     363,   637,    15,    16,    38,    39,    72,    17,    34,    35,
     544,   545,   959,   960,   546,   547,   548,   549,   550,   551,
     552,   953,   138,   139,    30,    31,    32,    52,    53,    54,
      18,   287,   404,   192,    19,    86,   193,    87,   123,   364,
     553,   405,   554,   365,   555,   366,   556,   557,   558,   559,
     560,   561,   562,   367,   602,   977,   603,   978,   605,   979,
     368,   600,   976,   369,   643,   999,   370,   827,   828,   646,
     371,   372,   704,   564,   373,   374,   375,   817,  1029,   818,
    1051,   376,   688,   947,   644,   939,  1053,   940,  1054,  1003,
    1064,   378,   400,   971,  1040,   968,   893,   570,   754,  1038,
    1058,   401,   402,   433,   434,   231,   232,   798,   518,   579,
     418,   304,   199,   301,    62,    96,    21,   128,   297,    42,
      73,    75,    22,    90,   125,    23,   410,   197,   198,    60,
      93,    24,   126,   299,   305,   379,   296,   179,   180,   185,
     181,   312,   766,   420,   435,   310,   765,   307,   761,   308,
     762,   309,   764,   313,   767,   314,   898,   315,   769,   316,
     899,   317,   771,   318,   900,   319,   774,   320,   776,    25,
      47,   119,    26,    46,   380,   620,   621,   622,   381,   623,
     624,   625,   626,   382,   462,   383,  1046,   384
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   178,   201,   289,   519,   563,   427,   692,   429,   588,
     431,   590,   461,   592,   629,   703,   697,   516,   691,   820,
     474,   719,   720,   802,     2,   182,   716,   -94,    97,    98,
       3,   809,   571,   615,   188,   795,   121,   807,   849,    27,
      28,   517,   729,   683,   684,   615,   997,   475,   476,   132,
     133,    68,   576,     4,   693,     5,    40,     6,   487,   488,
     614,   577,   627,     7,    48,   585,   759,   823,   693,   969,
      55,  1057,     8,   475,   476,   693,   824,   797,     9,    40,
     685,   969,  1011,    80,   797,    69,   485,   586,   200,   487,
     488,  1066,    49,    80,   386,    41,   796,    10,   392,   177,
     578,   415,   694,    43,   695,   778,    74,   696,   189,   190,
     288,   686,   687,   407,   730,   825,   694,    64,    11,   113,
     826,   519,   970,   694,   205,   230,   508,   509,   593,    33,
     821,   700,   399,    61,   970,   479,   480,   805,   393,    44,
     114,    63,   806,   485,   187,   486,   487,   488,   489,   303,
     819,   206,   490,   408,   407,   302,   191,   508,   509,   472,
     983,   479,   480,    50,    99,   515,   475,   476,   100,   485,
     101,   102,   487,   488,   489,   412,   801,    29,   490,   916,
     415,   728,   415,   415,   846,   134,    82,    51,   850,   135,
      84,   136,   102,   473,   749,    65,    85,   178,   178,   178,
     178,   914,   860,   178,   103,   178,    70,   178,   751,   178,
     866,    83,    12,   178,   508,   509,   752,    71,    13,   516,
     753,   419,   421,   422,   423,    56,    57,   426,    58,   428,
     693,   430,   387,   178,   178,   194,    50,   437,   821,   986,
     508,   509,   821,   821,    64,   895,   446,   567,   129,   130,
     388,   288,   477,   478,   479,   480,    59,   457,   459,   389,
      51,   219,   485,   195,   486,   487,   488,   489,   989,   413,
     822,   490,  1005,   491,   492,   639,   568,   520,   694,   800,
     521,    64,   821,   821,   178,   475,   476,   220,   221,   821,
     632,   821,   633,   178,    88,   177,   177,   177,   177,   812,
      89,   177,   910,   177,   930,   177,   178,   177,   514,   844,
     901,   177,    50,   594,   922,   984,  1002,   522,  1021,   503,
     504,   505,   506,   507,   936,   990,    82,   806,   596,   810,
     575,   177,   177,   508,   509,   595,    51,   178,   829,   942,
     706,    36,   949,    66,   467,   974,   576,   222,   714,   937,
     597,   965,    40,  -549,   913,   577,   950,    80,  -549,   202,
     203,   607,    37,   923,    91,   649,   652,   975,   475,   476,
      92,   477,   478,   479,   480,   481,  -549,   956,   482,   483,
     484,   485,   177,   486,   487,   488,   489,    81,   957,   958,
     490,   177,   491,   492,   756,   224,    77,   569,  -556,   225,
      78,  -563,   226,  -556,   177,  -282,  -563,    79,    94,   290,
    -282,   938,   987,   291,    95,    71,   982,   227,   106,   219,
     108,  -556,   109,   757,  -563,   110,   228,   599,  -282,   112,
     292,   293,   294,   295,   120,   177,   501,   502,   503,   504,
     505,   506,   507,   115,   415,   220,   221,   416,   966,   116,
     417,   111,   508,   509,   967,   118,   479,   480,   127,   640,
     129,   130,   634,   635,   485,    69,   486,   487,   488,   489,
     124,   183,   178,   490,   360,   718,   186,   288,   202,   203,
     204,   184,   196,   178,   200,   178,   207,   208,   209,   210,
     178,   211,   178,   178,   212,   178,   763,   213,   214,   215,
     216,   217,   218,  1041,   229,   222,   230,   773,   284,   848,
     929,   993,   285,   298,   781,   300,   783,   784,   856,   786,
     303,   503,   504,   505,   506,   507,   286,   288,   311,   385,
     394,   396,   395,   397,   865,   508,   509,   403,   398,   406,
     409,   411,   414,   424,   439,   440,   441,   442,   475,   476,
     443,   444,   223,   224,   445,   563,   512,   225,   446,   580,
     226,   836,   447,   573,   471,   450,   581,   451,   452,   572,
     177,   453,   454,   574,   455,   227,   511,    12,   645,   659,
     661,   177,   709,   177,   228,   178,   710,   219,   177,   711,
     177,   177,   721,   177,   722,   723,   725,   726,   727,   755,
     392,   768,   770,   803,   772,   800,   804,   813,   815,   845,
     819,   830,   832,   220,   221,   838,   831,   519,   840,   842,
     729,   855,   857,   858,   377,   859,   863,   891,   892,   864,
     927,   924,   894,   391,   477,   478,   479,   480,   481,   902,
     915,   482,   483,   484,   485,   909,   486,   487,   488,   489,
     917,   932,   918,   490,   920,   491,   492,   475,   476,   925,
     933,   493,   494,   495,   934,   952,   935,   496,  1039,   980,
     943,   972,   994,   222,   985,   988,   991,   996,  1015,  1030,
    1023,   475,   476,   177,  1010,  1024,  1043,  1065,  1028,  1047,
    1060,  1049,  1061,   636,   497,  1069,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,  1070,   178,   104,  1062,
      67,   140,   107,  1056,   306,   508,   509,   105,   890,   955,
     223,   224,   122,   288,    20,   225,   995,  1012,   226,  1013,
     698,   928,   908,   775,   758,   436,    76,   566,   921,    55,
     425,   448,   449,   227,   438,   479,   480,   630,   998,   582,
    1048,   460,   228,   485,     0,   486,   487,   488,   489,   464,
     465,   466,   490,   468,   469,   470,  1008,   477,   478,   479,
     480,   481,     0,     0,   482,   483,   484,   485,     0,   486,
     487,   488,   489,     0,     0,     0,   490,     0,   491,   492,
       0,   219,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   177,     0,     0,     0,     0,
       0,   565,   505,   506,   507,  1036,     0,   220,   221,  1037,
       0,     0,     0,     0,   508,   509,     0,     0,     0,   178,
     178,   500,   501,   502,   503,   504,   505,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,   601,   508,   509,
     604,     0,   606,  1000,  1001,   219,   608,   609,   610,   611,
     612,   613,     0,   619,     0,   619,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1071,   222,     0,     0,
       0,   220,   221,   653,   654,     0,     0,   655,   656,   657,
     658,     0,   660,   178,   662,   663,   664,   665,   666,   667,
     668,   669,   670,   672,   673,   674,   675,   676,   677,   678,
     679,   680,   681,   682,     0,   689,     0,  1035,     0,     0,
       0,     0,     0,     0,   223,   224,     0,   177,   177,   225,
     699,   583,   226,     0,   705,     0,     0,   707,   708,     0,
       0,   222,     0,   713,     0,   717,     0,   227,     0,     0,
       0,     0,   724,     0,     0,     0,   228,     0,     0,     0,
     475,   476,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   760,     0,     0,   223,   224,
       0,   177,     0,   225,     0,   584,   226,     0,     0,     0,
       0,     0,     0,     0,     0,   780,     0,     0,     0,     0,
       0,   227,     0,     0,     0,     0,     0,  1050,     0,     0,
     228,     0,     0,   464,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   470,     0,     0,   811,   460,  1063,
       0,   814,     0,     0,     0,     0,   477,   478,   479,   480,
     481,     0,     0,   482,   483,   484,   485,     0,   486,   487,
     488,   489,     0,     0,     0,   490,     0,   491,   492,     0,
       0,     0,     0,   493,   494,   495,     0,     0,     0,   496,
     475,   476,     0,     0,     0,     0,     0,     0,     0,     0,
     839,     0,     0,   841,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   847,     0,     0,   497,   851,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,     0,     0,
       0,   861,     0,     0,     0,     0,     0,   508,   509,     0,
     219,     0,     0,     0,   889,     0,     0,   867,   868,   869,
     870,   871,   872,   873,   874,   875,   876,   879,   880,   881,
     882,   883,   884,   885,   888,     0,   220,   221,     0,     0,
       0,     0,     0,     0,   896,     0,   477,   478,   479,   480,
     481,     0,     0,   482,   483,   484,   485,     0,   486,   487,
     488,   489,     0,     0,     0,   490,     0,   491,   492,   475,
     476,     0,     0,   493,   494,   495,     0,     0,     0,   496,
       0,     0,     0,   911,   912,     0,     0,     0,     0,     0,
     919,     0,   619,     0,     0,     0,   222,     0,     0,     0,
     475,   476,     0,     0,     0,     0,   497,   931,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,     0,     0,
       0,     0,     0,   944,     0,     0,     0,   508,   509,     0,
       0,     0,     0,     0,  1045,     0,     0,     0,   879,   888,
       0,     0,     0,   223,   224,     0,     0,     0,   225,     0,
     587,   226,     0,     0,     0,   477,   478,   479,   480,   481,
       0,     0,   482,   961,   962,   485,   227,   486,   487,   488,
     489,     0,   963,   964,   490,   228,   491,   492,     0,     0,
       0,     0,     0,     0,     0,     0,   477,   478,   479,   480,
     481,     0,     0,   482,   483,   484,   485,     0,   486,   487,
     488,   489,     0,     0,     0,   490,     0,   491,   492,     0,
       0,     0,     0,   493,   494,   495,     0,     0,     0,   496,
     501,   502,   503,   504,   505,   506,   507,     0,     0,     0,
       0,     0,     0,     0,     0,  1004,   508,   509,     0,  1006,
       0,   565,     0,     0,     0,  1009,   497,     0,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,     0,     0,
    1014,     0,     0,  1016,  1017,  1018,  1020,   508,   509,  1022,
       0,   510,  1025,  1026,     0,  1027,     0,     0,     0,     0,
       0,     0,     0,  1032,     0,   523,     0,     0,     0,     0,
       3,     0,   524,   525,   526,     0,   527,     0,   321,   322,
     323,   324,   325,     0,     0,     0,     0,     0,     0,   528,
     326,   529,   530,     0,     0,     0,     0,  1052,     0,     0,
       0,   531,   327,     0,   532,     0,   533,   328,     0,  1059,
     329,     0,     8,   330,   534,     0,   535,   331,     0,     0,
     536,   537,     0,     0,     0,     0,  1068,   538,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   333,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,   334,
     539,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   335,   336,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   540,   541,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,   337,   338,   339,   340,   341,     0,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
      51,   352,     0,     0,     0,   219,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   353,   354,   355,
       0,   356,     0,     0,   357,   358,     0,     0,     0,     0,
       0,   220,   221,     0,   359,     0,   360,   361,   542,   288,
     543,   523,     0,     0,     0,     0,     3,     0,   524,   525,
     526,     0,   527,     0,   321,   322,   323,   324,   325,     0,
       0,     0,     0,     0,     0,   528,   326,   529,   530,     0,
       0,     0,     0,     0,     0,     0,     0,   531,   327,     0,
     532,     0,   533,   328,     0,     0,   329,     0,     8,   330,
     534,   222,   535,   331,     0,     0,   536,   537,     0,     0,
       0,     0,     0,   538,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   333,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   334,   539,     0,   223,   224,
       0,     0,     0,   225,     0,   589,   226,     0,   335,   336,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   227,     0,   540,   541,     0,     0,     0,     0,     0,
     228,     0,    50,     0,     0,   219,     0,     0,     0,     0,
     337,   338,   339,   340,   341,     0,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,    51,   352,     0,     0,
       0,   220,   221,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   353,   354,   355,     0,   356,     0,     0,
     357,   358,     0,     0,     0,   321,   322,   323,   324,   325,
     359,     0,   360,   361,   542,   288,  1007,   326,     0,     0,
       0,     0,     0,   219,     0,     0,     0,     0,     0,   327,
       0,     0,     0,     0,   328,     0,     0,   329,     0,     0,
     330,   222,   615,     0,   331,     0,     0,     0,     0,   220,
     221,     0,     0,     0,   332,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   333,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   334,     0,   223,   224,
       0,     0,     0,   225,     0,   591,   226,     0,     0,   335,
     336,     0,     0,     0,     0,     0,     0,     0,     0,   222,
       0,   227,     0,     0,     0,     0,     0,     0,     0,     0,
     228,     0,     0,    50,   219,     0,     0,     0,     0,     0,
       0,   337,   338,   339,   340,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   616,   352,     0,
     220,   221,     0,     0,     0,     0,   223,   224,     0,     0,
       0,   225,     0,     0,   617,   354,   355,     0,   356,     0,
       0,   357,   358,   321,   322,   323,   324,   325,     0,   227,
       0,   618,     0,   360,   361,   326,   288,     0,   228,     0,
       0,   219,     0,     0,     0,     0,     0,   327,     0,     0,
       0,     0,   328,     0,     0,   329,     0,     0,   330,     0,
     222,     0,   331,     0,     0,     0,     0,   220,   221,     0,
       0,     0,   332,   145,   146,   147,     0,   149,   150,   151,
     152,   153,   333,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   165,   166,   167,     0,     0,   170,   171,
     172,   173,     0,     0,   334,     0,     0,   223,   224,     0,
       0,     0,   225,     0,     0,   226,     0,   335,   336,     0,
       0,     0,     0,     0,     0,     0,     0,   222,     0,     0,
     227,     0,   598,     0,     0,     0,     0,     0,     0,   228,
       0,    50,   219,     0,     0,     0,     0,     0,     0,   337,
     338,   339,   340,   341,     0,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   616,   352,     0,   220,   221,
       0,     0,     0,     0,   223,   224,     0,     0,     0,   225,
       0,     0,   617,   354,   355,     0,   356,     0,     0,   357,
     358,   321,   322,   323,   324,   325,     0,   227,     0,   628,
       0,   360,   361,   326,   288,     0,   228,     0,   219,   386,
       0,     0,     0,     0,     0,   327,     0,     0,     0,     0,
     328,     0,     0,   329,     0,     0,   330,     0,   222,     0,
     331,     0,     0,     0,   220,   221,     0,     0,     0,     0,
     332,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     333,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   334,     0,     0,   223,   224,     0,     0,     0,
     225,     0,   788,   226,     0,   335,   336,     0,     0,     0,
       0,     0,     0,     0,   222,     0,     0,     0,   227,     0,
       0,     0,     0,     0,     0,     0,     0,   228,     0,    50,
     219,     0,     0,     0,     0,     0,     0,   337,   338,   339,
     340,   341,     0,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,    51,   352,     0,   220,   221,     0,     0,
       0,   223,   224,     0,     0,     0,   225,   387,   904,   226,
     353,   354,   355,     0,   356,     0,     0,   357,   358,   321,
     322,   323,   324,   325,   227,   388,     0,   359,     0,   360,
     361,   326,   288,   228,   389,     0,     0,     0,     0,     0,
       0,     0,     0,   327,     0,     0,     0,     0,   328,     0,
       0,   329,     0,     0,   330,     0,   222,     0,   331,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   333,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     334,     0,     0,   223,   224,     0,     0,     0,   225,     0,
     905,   226,     0,   335,   336,     0,     0,     0,     0,     0,
       0,     0,   701,     0,     0,     0,   227,     0,   540,   541,
       0,     0,     0,     0,     0,   228,     0,    50,     0,     0,
       0,     0,     0,     0,     0,   337,   338,   339,   340,   341,
       0,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,    51,   352,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   321,   322,   323,   324,   325,     0,   353,   354,
     355,     0,   356,     0,   326,   357,   358,     0,     0,     0,
       0,     0,     0,     0,     0,   359,   327,   360,   361,   702,
     288,   328,     0,     0,   329,     0,     0,   330,     0,     0,
       0,   331,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   332,   145,   146,   147,     0,   149,   150,   151,   152,
     153,   333,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   165,   166,   167,     0,     0,   170,   171,   172,
     173,     0,     0,   334,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   335,   336,   219,     0,
       0,     0,     0,     0,     0,   715,     0,     0,     0,     0,
       0,   540,   541,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,   220,   221,     0,     0,   337,   338,
     339,   340,   341,     0,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,    51,   352,     0,   321,   322,   323,
     324,   325,     0,     0,     0,     0,     0,     0,     0,   326,
       0,   353,   354,   355,   219,   356,     0,     0,   357,   358,
       0,   327,     0,     0,     0,     0,   328,     0,   359,   329,
     360,   361,   330,   288,   222,     0,   331,     0,     0,     0,
     220,   221,     0,     0,     0,     0,   332,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   333,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   334,     0,
       0,   223,   224,     0,     0,     0,   225,     0,   906,   226,
       0,   335,   336,     0,     0,     0,     0,     0,     0,     0,
     222,     0,     0,     0,   227,     0,     0,     0,     0,     0,
       0,     0,     0,   228,     0,    50,   219,     0,     0,     0,
       0,     0,     0,   337,   338,   339,   340,   341,     0,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,    51,
     352,     0,   220,   221,     0,     0,     0,   223,   224,     0,
       0,     0,   225,     0,   907,   226,   353,   354,   355,     0,
     356,     0,     0,   357,   358,   321,   322,   323,   324,   325,
     227,   641,     0,   359,   642,   360,   361,   326,   288,   228,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   327,
       0,     0,     0,     0,   328,     0,     0,   329,     0,     0,
     330,   693,   222,     0,   331,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   333,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   334,     0,     0,   948,
     224,     0,     0,     0,   225,     0,     0,   226,     0,   335,
     336,   219,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   227,     0,   540,   541,     0,     0,     0,     0,
       0,   228,     0,    50,     0,     0,     0,   220,   221,     0,
       0,   337,   338,   339,   340,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,    51,   352,     0,
     321,   322,   323,   324,   325,     0,     0,     0,     0,     0,
       0,     0,   326,     0,   353,   354,   355,     0,   356,     0,
       0,   357,   358,     0,   327,     0,     0,     0,     0,   328,
       0,   359,   329,   360,   361,   330,   288,   222,     0,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
     145,   146,   147,     0,   149,   150,   151,   152,   153,   333,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
       0,   334,     0,     0,   223,   224,     0,     0,     0,   225,
       0,   992,   226,     0,   335,   336,   219,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   227,     0,   877,
     878,     0,     0,     0,     0,     0,   228,     0,    50,     0,
       0,     0,   220,   221,     0,     0,   337,   338,   339,   340,
     341,     0,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,    51,   352,     0,   321,   322,   323,   324,   325,
       0,     0,     0,     0,     0,     0,     0,   326,     0,   353,
     354,   355,     0,   356,     0,     0,   357,   358,     0,   327,
       0,     0,     0,     0,   328,     0,   359,   329,   360,   361,
     330,   288,   222,     0,   331,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   333,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   334,     0,     0,   223,
     224,     0,     0,     0,   225,     0,  1033,   226,     0,   335,
     336,   219,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   227,     0,   886,   887,     0,     0,     0,     0,
       0,   228,     0,    50,     0,     0,     0,   220,   221,     0,
       0,   337,   338,   339,   340,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,    51,   352,     0,
     321,   322,   323,   324,   325,     0,     0,     0,     0,     0,
       0,     0,   326,     0,   353,   354,   355,     0,   356,     0,
       0,   357,   358,     0,   327,     0,     0,     0,     0,   328,
       0,   359,   329,   360,   361,   330,   288,   222,     0,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
     145,   146,   147,     0,   149,   150,   151,   152,   153,   333,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
       0,   334,     0,     0,   223,   224,     0,     0,     0,   225,
       0,  1034,   226,     0,   335,   336,   219,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   227,     0,     0,
       0,     0,     0,     0,     0,     0,   228,     0,    50,     0,
       0,     0,   220,   221,     0,     0,   337,   338,   339,   340,
     341,     0,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,    51,   352,     0,   321,   322,   323,   324,   325,
       0,     0,   671,     0,     0,     0,     0,   326,     0,   353,
     354,   355,   219,   356,     0,     0,   357,   358,     0,   327,
       0,     0,     0,     0,   328,   390,   359,   329,   360,   361,
     330,   288,   222,     0,   331,     0,     0,     0,   220,   221,
       0,     0,     0,     0,   332,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   333,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   334,     0,     0,   223,
     224,     0,     0,     0,   225,     0,  1055,   226,     0,   335,
     336,     0,     0,     0,     0,     0,     0,     0,   222,     0,
       0,     0,   227,     0,     0,     0,     0,     0,     0,     0,
       0,   228,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   337,   338,   339,   340,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,    51,   352,     0,
     321,   322,   323,   324,   325,   223,   224,     0,     0,     0,
     225,     0,   326,   226,   353,   354,   355,     0,   356,     0,
       0,   357,   358,     0,   327,     0,     0,     0,   227,   328,
       0,   359,   329,   360,   361,   330,   288,   228,     0,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
     145,   146,   147,     0,   149,   150,   151,   152,   153,   333,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
       0,   334,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   335,   336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,   337,   338,   339,   340,
     341,     0,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,    51,   352,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
     354,   355,     0,   356,     0,     0,   357,   358,   321,   322,
     323,   324,   325,     0,     0,     0,   359,   690,   360,   361,
     326,   288,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   327,     0,     0,     0,     0,   328,     0,     0,
     329,     0,     0,   330,     0,     0,     0,   331,     0,     0,
     712,     0,     0,     0,     0,     0,     0,   332,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   333,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,   334,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   335,   336,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,   337,   338,   339,   340,   341,     0,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
      51,   352,     0,   321,   322,   323,   324,   325,     0,     0,
       0,     0,     0,     0,     0,   326,     0,   353,   354,   355,
       0,   356,     0,     0,   357,   358,     0,   327,     0,     0,
       0,     0,   328,     0,   359,   329,   360,   361,   330,   288,
       0,     0,   331,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   332,   145,   146,   147,     0,   149,   150,   151,
     152,   153,   333,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   165,   166,   167,     0,     0,   170,   171,
     172,   173,     0,     0,   334,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   335,   336,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   337,
     338,   339,   340,   341,     0,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,    51,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   353,   354,   355,     0,   356,     0,     0,   357,
     358,   321,   322,   323,   324,   325,     0,     0,     0,   359,
     777,   360,   361,   326,   288,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   327,     0,     0,     0,     0,
     328,     0,     0,   329,     0,     0,   330,     0,     0,     0,
     331,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     333,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   335,   336,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   337,   338,   339,
     340,   341,     0,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,    51,   352,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   779,     0,
     353,   354,   355,     0,   356,     0,     0,   357,   358,   321,
     322,   323,   324,   325,     0,     0,     0,   359,     0,   360,
     361,   326,   288,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   327,     0,     0,     0,     0,   328,     0,
       0,   329,     0,     0,   330,     0,     0,     0,   331,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   333,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     334,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   335,   336,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,   337,   338,   339,   340,   341,
       0,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,    51,   352,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   353,   354,
     355,     0,   356,     0,     0,   357,   358,   321,   322,   323,
     324,   325,     0,     0,     0,   359,   799,   360,   361,   326,
     288,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   327,     0,     0,     0,     0,   328,     0,     0,   329,
       0,     0,   330,     0,     0,     0,   331,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   333,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   334,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   335,   336,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   337,   338,   339,   340,   341,     0,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,    51,
     352,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   353,   354,   355,     0,
     356,     0,     0,   357,   358,   321,   322,   323,   324,   325,
       0,     0,     0,   359,   808,   360,   361,   326,   288,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   327,
       0,     0,     0,     0,   328,     0,     0,   329,     0,     0,
     330,     0,     0,     0,   331,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   332,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   333,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   334,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   335,
     336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   337,   338,   339,   340,   341,     0,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,    51,   352,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   353,   354,   355,     0,   356,     0,
       0,   357,   358,   321,   322,   323,   324,   325,     0,     0,
       0,   359,   941,   360,   361,   326,   288,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   327,     0,     0,
       0,     0,   328,     0,     0,   329,     0,     0,   330,     0,
       0,     0,   331,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   332,   145,   146,   147,     0,   149,   150,   151,
     152,   153,   333,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   165,   166,   167,     0,     0,   170,   171,
     172,   173,     0,     0,   334,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   335,   336,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   337,
     338,   339,   340,   341,     0,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,    51,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   353,   354,   355,     0,   356,     0,     0,   357,
     358,   321,   322,   323,   324,   325,     0,     0,     0,   359,
     981,   360,   361,   326,   288,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   327,     0,     0,     0,     0,
     328,     0,     0,   329,     0,     0,   330,     0,     0,     0,
     331,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     333,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   335,   336,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   337,   338,   339,
     340,   341,     0,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,    51,   352,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     353,   354,   355,     0,   356,     0,     0,   357,   358,   321,
     322,   323,   324,   325,     0,     0,     0,   359,  1019,   360,
     361,   326,   288,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   327,     0,     0,     0,     0,   328,     0,
       0,   329,     0,     0,   330,     0,     0,     0,   331,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   333,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     334,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   335,   336,     0,     0,     0,     0,     0,
       0,     0,  1031,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,   337,   338,   339,   340,   341,
       0,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,    51,   352,     0,   321,   322,   323,   324,   325,     0,
       0,     0,     0,     0,     0,     0,   326,     0,   353,   354,
     355,     0,   356,     0,     0,   357,   358,     0,   327,     0,
       0,     0,     0,   328,     0,   359,   329,   360,   361,   330,
     288,     0,     0,   331,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   333,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   334,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   335,   336,
       0,     0,     0,     0,     0,     0,     0,     0,   475,   476,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
     337,   338,   339,   340,   341,     0,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,    51,   352,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   353,   354,   355,     0,   356,     0,     0,
     357,   358,     0,     0,     0,     0,     0,     0,     0,     0,
     359,     0,   360,   361,     0,   288,   731,   732,   733,   734,
     735,   736,   737,   738,   477,   478,   479,   480,   481,   739,
     740,   482,   483,   484,   485,   852,   486,   487,   488,   489,
     475,   476,     0,   490,   742,   491,   492,   743,   744,     0,
       0,   493,   494,   495,   745,   746,   747,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   853,   497,     0,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,   509,     0,     0,     0,
       0,     0,   854,     0,     0,     0,     0,     0,   731,   732,
     733,   734,   735,   736,   737,   738,   477,   478,   479,   480,
     481,   739,   740,   482,   483,   484,   485,   852,   486,   487,
     488,   489,   475,   476,     0,   490,   742,   491,   492,   743,
     744,     0,     0,   493,   494,   495,   745,   746,   747,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   853,   497,     0,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   508,   509,     0,
       0,     0,     0,     0,   862,     0,     0,     0,     0,     0,
     731,   732,   733,   734,   735,   736,   737,   738,   477,   478,
     479,   480,   481,   739,   740,   482,   483,   484,   485,   852,
     486,   487,   488,   489,   475,   476,     0,   490,   742,   491,
     492,   743,   744,     0,     0,   493,   494,   495,   745,   746,
     747,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   853,   497,     0,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   508,
     509,     0,     0,     0,     0,     0,   897,     0,     0,     0,
       0,     0,   731,   732,   733,   734,   735,   736,   737,   738,
     477,   478,   479,   480,   481,   739,   740,   482,   483,   484,
     485,   852,   486,   487,   488,   489,   475,   476,     0,   490,
     742,   491,   492,   743,   744,     0,     0,   493,   494,   495,
     745,   746,   747,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   853,
     497,     0,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,   509,     0,     0,     0,     0,     0,   951,     0,
       0,     0,     0,     0,   731,   732,   733,   734,   735,   736,
     737,   738,   477,   478,   479,   480,   481,   739,   740,   482,
     483,   484,   485,   852,   486,   487,   488,   489,   475,   476,
       0,   490,   742,   491,   492,   743,   744,     0,     0,   493,
     494,   495,   745,   746,   747,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   853,   497,     0,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   508,   509,     0,     0,     0,     0,     0,
     954,     0,     0,     0,     0,     0,   731,   732,   733,   734,
     735,   736,   737,   738,   477,   478,   479,   480,   481,   739,
     740,   482,   483,   484,   485,   852,   486,   487,   488,   489,
     475,   476,     0,   490,   742,   491,   492,   743,   744,     0,
       0,   493,   494,   495,   745,   746,   747,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   853,   497,     0,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,   509,     0,     0,     0,
     647,     0,   973,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   477,   478,   479,   480,
     481,   475,   476,   482,   483,   484,   485,     0,   486,   487,
     488,   489,     0,     0,     0,   490,     0,   491,   492,     0,
       0,     0,     0,   493,   494,   495,   145,   146,   147,   496,
     149,   150,   151,   152,   153,   333,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,   497,     0,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   508,   509,     0,
       0,   638,     0,     0,     0,     0,     0,   477,   478,   479,
     480,   481,   475,   476,   482,   483,   484,   485,     0,   486,
     487,   488,   489,     0,     0,     0,   490,     0,   491,   492,
       0,     0,     0,     0,   493,   494,   495,     0,   648,     0,
     496,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   497,     0,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,     0,
       0,     0,   650,     0,     0,     0,     0,     0,   508,   509,
       0,     0,   782,     0,     0,     0,     0,     0,   477,   478,
     479,   480,   481,   475,   476,   482,   483,   484,   485,     0,
     486,   487,   488,   489,     0,     0,     0,   490,     0,   491,
     492,     0,     0,     0,     0,   493,   494,   495,   145,   146,
     147,   496,   149,   150,   151,   152,   153,   333,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,   497,     0,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   508,
     509,     0,     0,   785,     0,     0,     0,     0,     0,   477,
     478,   479,   480,   481,   475,   476,   482,   483,   484,   485,
       0,   486,   487,   488,   489,     0,     0,     0,   490,     0,
     491,   492,     0,     0,     0,     0,   493,   494,   495,     0,
     651,     0,   496,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   497,
       0,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,     0,     0,     0,   834,     0,     0,     0,     0,     0,
     508,   509,     0,     0,   787,     0,     0,     0,     0,     0,
     477,   478,   479,   480,   481,   475,   476,   482,   483,   484,
     485,     0,   486,   487,   488,   489,     0,     0,     0,   490,
       0,   491,   492,     0,     0,     0,     0,   493,   494,   495,
     145,   146,   147,   496,   149,   150,   151,   152,   153,   333,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
     497,     0,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,   509,     0,     0,   789,     0,     0,     0,     0,
       0,   477,   478,   479,   480,   481,   475,   476,   482,   483,
     484,   485,     0,   486,   487,   488,   489,     0,     0,     0,
     490,     0,   491,   492,     0,     0,     0,     0,   493,   494,
     495,     0,   835,     0,   496,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   497,     0,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   508,   509,     0,     0,   790,     0,     0,     0,
       0,     0,   477,   478,   479,   480,   481,   475,   476,   482,
     483,   484,   485,     0,   486,   487,   488,   489,     0,     0,
       0,   490,     0,   491,   492,     0,     0,     0,     0,   493,
     494,   495,     0,     0,     0,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,     0,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   508,   509,     0,     0,   791,     0,     0,
       0,     0,     0,   477,   478,   479,   480,   481,   475,   476,
     482,   483,   484,   485,     0,   486,   487,   488,   489,     0,
       0,     0,   490,     0,   491,   492,     0,     0,     0,     0,
     493,   494,   495,     0,     0,     0,   496,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   497,     0,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   508,   509,     0,     0,   792,     0,
       0,     0,     0,     0,   477,   478,   479,   480,   481,   475,
     476,   482,   483,   484,   485,     0,   486,   487,   488,   489,
       0,     0,     0,   490,     0,   491,   492,     0,     0,     0,
       0,   493,   494,   495,     0,     0,     0,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,     0,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,   509,     0,     0,   793,
       0,     0,     0,     0,     0,   477,   478,   479,   480,   481,
     475,   476,   482,   483,   484,   485,     0,   486,   487,   488,
     489,     0,     0,     0,   490,     0,   491,   492,     0,     0,
       0,     0,   493,   494,   495,     0,     0,     0,   496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   497,     0,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,   509,     0,     0,
     794,     0,     0,     0,     0,     0,   477,   478,   479,   480,
     481,   475,   476,   482,   483,   484,   485,     0,   486,   487,
     488,   489,     0,     0,     0,   490,     0,   491,   492,     0,
       0,     0,     0,   493,   494,   495,     0,     0,     0,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,     0,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   508,   509,     0,
       0,   903,     0,     0,     0,     0,     0,   477,   478,   479,
     480,   481,   475,   476,   482,   483,   484,   485,     0,   486,
     487,   488,   489,     0,     0,     0,   490,     0,   491,   492,
       0,     0,     0,     0,   493,   494,   495,     0,     0,     0,
     496,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   497,     0,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   508,   509,
       0,     0,  1042,     0,     0,     0,     0,     0,   477,   478,
     479,   480,   481,   475,   476,   482,   483,   484,   485,     0,
     486,   487,   488,   489,     0,     0,     0,   490,     0,   491,
     492,     0,     0,     0,     0,   493,   494,   495,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,     0,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   508,
     509,     0,     0,  1044,     0,     0,     0,     0,     0,   477,
     478,   479,   480,   481,   475,   476,   482,   483,   484,   485,
       0,   486,   487,   488,   489,     0,     0,     0,   490,     0,
     491,   492,     0,     0,     0,     0,   493,   494,   495,     0,
       0,     0,   496,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   497,
       0,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     508,   509,     0,     0,  1067,     0,     0,     0,   475,   476,
     477,   478,   479,   480,   481,     0,     0,   482,   483,   484,
     485,     0,   486,   487,   488,   489,     0,     0,     0,   490,
       0,   491,   492,     0,     0,     0,     0,   493,   494,   495,
       0,     0,     0,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,     0,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,   509,   513,   477,   478,   479,   480,   481,   475,
     476,   482,   483,   484,   485,     0,   486,   487,   488,   489,
       0,     0,     0,   490,     0,   491,   492,     0,     0,     0,
       0,   493,   494,   495,     0,     0,     0,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,     0,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,   509,   833,     0,     0,
       0,     0,     0,   475,   476,   477,   478,   479,   480,   481,
       0,     0,   482,   483,   484,   485,     0,   486,   487,   488,
     489,     0,     0,     0,   490,     0,   491,   492,     0,     0,
       0,     0,   493,   494,   495,     0,     0,     0,   496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   497,     0,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,   509,   843,   477,
     478,   479,   480,   481,   475,   476,   482,   483,   484,   485,
       0,   486,   487,   488,   489,     0,     0,     0,   490,     0,
     491,   492,     0,     0,     0,     0,   493,   494,   495,     0,
       0,     0,   496,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   497,
       0,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     508,   509,   945,     0,     0,     0,     0,     0,     0,     0,
     477,   478,   479,   480,   481,     0,     0,   482,   483,   484,
     485,     0,   486,   487,   488,   489,   475,   476,     0,   490,
       0,   491,   492,     0,     0,     0,     0,   493,   494,   495,
       0,     0,     0,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,     0,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,   509,   946,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   731,   732,   733,   734,   735,   736,
     737,   738,   477,   478,   479,   480,   481,   739,   740,   482,
     483,   484,   485,   852,   486,   487,   488,   489,  -252,   475,
     476,   490,   742,   491,   492,   743,   744,     0,     0,   493,
     494,   495,   745,   746,   747,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   853,   497,     0,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   508,   509,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   731,   732,   733,
     734,   735,   736,   737,   738,   477,   478,   479,   480,   481,
     739,   740,   482,   483,   484,   485,   741,   486,   487,   488,
     489,     0,   233,   234,   490,   742,   491,   492,   743,   744,
       0,     0,   493,   494,   495,   745,   746,   747,   496,   235,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   748,   497,     0,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   508,   509,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   475,   476,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,     0,
     254,   255,   256,     0,     0,     0,     0,     0,   257,   258,
     259,   260,   261,     0,     0,   262,   263,   264,   265,   266,
     267,   268,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   269,     0,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
       0,     0,   280,   281,   477,   478,   479,   480,   481,   282,
     283,   482,   483,   484,   485,     0,   486,   487,   488,   489,
     475,   476,     0,   490,     0,   491,   492,     0,     0,   631,
       0,   493,   494,   495,     0,     0,     0,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,     0,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,   509,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   475,   476,     0,     0,
       0,     0,     0,     0,     0,     0,   477,   478,   479,   480,
     481,     0,     0,   482,   483,   484,   485,     0,   486,   487,
     488,   489,     0,     0,     0,   490,     0,   491,   492,     0,
       0,     0,     0,   493,   494,   495,     0,     0,     0,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,   837,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,     0,   475,
     476,     0,   477,   478,   479,   480,   481,   508,   509,   482,
     483,   484,   485,     0,   486,   487,   488,   489,     0,     0,
       0,   490,     0,   491,   492,     0,     0,     0,     0,   493,
     494,   495,     0,     0,     0,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   926,     0,
       0,     0,   497,     0,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,     0,   475,   476,     0,     0,     0,
       0,     0,     0,   508,   509,   477,   478,   479,   480,   481,
       0,     0,   482,   483,   484,   485,     0,   486,   487,   488,
     489,     0,     0,     0,   490,     0,   491,   492,     0,     0,
       0,     0,   493,   494,   495,     0,     0,     0,   496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   497,     0,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,     0,   475,   476,
       0,   477,   478,   479,   480,   481,   508,   509,   482,   483,
     484,   485,     0,   486,   487,   488,   489,     0,     0,     0,
     490,     0,   491,   492,     0,     0,     0,     0,   493,   494,
     495,     0,     0,     0,  -564,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   497,     0,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,     0,   475,   476,     0,     0,     0,     0,
       0,     0,   508,   509,   477,   478,   479,   480,   481,     0,
       0,   482,   483,   484,   485,     0,   486,   487,   488,   489,
       0,     0,     0,   490,     0,   491,   492,   475,   476,     0,
       0,   493,   494,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,     0,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,     0,     0,     0,     0,
     477,   478,   479,   480,   481,   508,   509,   482,   483,   484,
     485,     0,   486,   487,   488,   489,     0,     0,     0,   490,
       0,   491,   492,   475,   476,     0,     0,   493,     0,   495,
       0,     0,     0,   477,   478,   479,   480,   481,     0,     0,
     482,   483,   484,   485,     0,   486,   487,   488,   489,   475,
     476,     0,   490,     0,   491,   492,     0,     0,     0,     0,
     493,     0,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   508,   509,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,     0,     0,     0,     0,   477,
     478,   479,   480,   481,   508,   509,   482,   483,   484,   485,
       0,   486,   487,   488,   489,   475,   476,     0,   490,     0,
     491,   492,     0,     0,     0,   477,   478,   479,   480,   481,
       0,     0,   482,   483,   484,   485,     0,   486,   487,   488,
     489,     0,     0,     0,   490,     0,   491,   492,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     508,   509,     0,     0,     0,     0,     0,     0,   499,   500,
     501,   502,   503,   504,   505,   506,   507,     0,     0,     0,
       0,   477,   478,     0,     0,   481,   508,   509,   482,   483,
     484,   485,     0,   486,   487,   488,   489,     0,     0,     0,
     490,     0,   491,   492,     0,     0,     0,     0,   493,   494,
     495,     0,     0,     0,   496,   456,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   141,     0,     0,
       0,     0,     0,   142,     0,     0,     0,     0,     0,     0,
       0,   497,     0,   498,   499,   500,   501,   502,     0,     0,
     143,   506,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   508,   509,     0,   144,     0,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   458,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     141,     0,     0,     0,     0,     0,   142,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   143,     0,     0,     0,     0,   176,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   144,    51,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   141,     0,     0,     0,     0,     0,   142,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,   143,     0,     0,     0,
       0,   176,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   144,    51,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   141,     0,     0,     0,
       0,     0,   142,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   143,
       0,     0,     0,     0,   176,     0,   343,   344,   345,     0,
       0,     0,     0,     0,   144,    51,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   141,
       0,     0,     0,     0,     0,   142,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   143,     0,     0,     0,     0,   176,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   144,    51,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     176,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432
};

static const yytype_int16 yycheck[] =
{
      12,   111,   128,   193,   400,   405,   314,   517,   316,   427,
     318,   429,   345,   431,   459,   528,   519,   395,   511,   641,
     370,   540,   541,   621,     0,   111,   539,     7,    14,    15,
       6,   629,   412,    51,    34,    19,    81,    19,    19,    18,
      19,    48,   122,   119,   120,    51,   131,    20,    21,    14,
      15,    21,   117,    29,   117,    31,   156,    33,   119,   120,
     456,   126,   458,    39,    33,   172,   579,   117,   117,    44,
      82,  1038,    48,    20,    21,   117,   126,   126,    54,   156,
     156,    44,    45,   132,   126,    55,   116,   194,   147,   119,
     120,  1058,    61,   132,    32,   195,   615,    73,   156,   111,
     165,   164,   165,   156,   167,   598,    41,   170,   108,   109,
     195,   187,   188,   156,   194,   165,   165,   164,    94,   196,
     170,   517,    97,   165,   164,   156,   187,   188,   167,   150,
     164,   527,   191,    60,    97,   108,   109,   189,   196,   192,
      75,   165,   194,   116,   191,   118,   119,   120,   121,   156,
     156,   191,   125,   196,   156,   200,   156,   187,   188,   156,
     194,   108,   109,   132,   150,   196,    20,    21,   154,   116,
     156,   157,   119,   120,   121,   301,   194,   156,   125,   801,
     164,   561,   164,   164,   697,   150,   164,   156,   701,   154,
      55,   156,   157,   190,   196,   167,    61,   307,   308,   309,
     310,   799,   715,   313,   190,   315,   176,   317,   188,   319,
     729,   189,   188,   323,   187,   188,   196,   187,   194,   597,
     570,   307,   308,   309,   310,     4,     5,   313,     7,   315,
     117,   317,   170,   343,   344,   167,   132,   323,   164,   126,
     187,   188,   164,   164,   164,   758,   190,   165,   158,   159,
     188,   195,   106,   107,   108,   109,    35,   343,   344,   197,
     156,    32,   116,   195,   118,   119,   120,   121,   194,   189,
     191,   125,   194,   127,   128,   171,   194,   191,   165,   164,
     194,   164,   164,   164,   394,    20,    21,    58,    59,   164,
     194,   164,   196,   403,    55,   307,   308,   309,   310,   632,
      61,   313,   795,   315,   189,   317,   416,   319,   394,   191,
     191,   323,   132,   172,   807,   913,   191,   403,   191,   173,
     174,   175,   176,   177,   164,   923,   164,   194,   172,   196,
     416,   343,   344,   187,   188,   194,   156,   447,   646,   832,
     530,   156,   845,   164,   356,   172,   117,   118,   538,   189,
     194,   189,   156,   167,   799,   126,   849,   132,   172,   158,
     159,   447,   177,   808,    55,   475,   476,   194,    20,    21,
      61,   106,   107,   108,   109,   110,   190,    11,   113,   114,
     115,   116,   394,   118,   119,   120,   121,   190,    22,    23,
     125,   403,   127,   128,   165,   166,   156,   409,   167,   170,
     156,   167,   173,   172,   416,   167,   172,   156,    55,    68,
     172,   829,   915,    72,    61,   187,   909,   188,   150,    32,
     156,   190,   156,   194,   190,   156,   197,   439,   190,   194,
      89,    90,    91,    92,   156,   447,   171,   172,   173,   174,
     175,   176,   177,    55,   164,    58,    59,   167,    55,    61,
     170,   165,   187,   188,    61,   195,   108,   109,   195,   471,
     158,   159,   160,   161,   116,    55,   118,   119,   120,   121,
     156,   194,   582,   125,   192,   193,   195,   195,   158,   159,
     160,   156,    97,   593,   147,   595,   171,   171,   171,   171,
     600,   190,   602,   603,   171,   605,   582,   171,   171,   171,
     171,   171,   190,  1013,   194,   118,   156,   593,    34,   699,
     818,   929,    34,   156,   600,   196,   602,   603,   708,   605,
     156,   173,   174,   175,   176,   177,   190,   195,   156,    32,
     167,   189,   194,    21,   724,   187,   188,   167,   189,   195,
     167,   195,    41,   191,   190,   171,   190,   171,    20,    21,
     171,   190,   165,   166,   171,   955,   189,   170,   190,   172,
     173,   671,   171,   156,   193,   190,   172,   190,   190,   189,
     582,   190,   190,   156,   190,   188,   190,   188,   147,   156,
     156,   593,   156,   595,   197,   695,   194,    32,   600,   150,
     602,   603,    36,   605,   194,   194,   194,   194,   194,    41,
     156,   172,   172,   189,   172,   164,   164,   196,   158,   695,
     156,   171,   190,    58,    59,   156,   171,  1013,   156,     1,
     122,    13,   165,   167,   218,   150,   193,     7,   754,   195,
     158,   196,   156,   227,   106,   107,   108,   109,   110,   156,
     156,   113,   114,   115,   116,   190,   118,   119,   120,   121,
     189,   156,   189,   125,   189,   127,   128,    20,    21,   189,
     156,   133,   134,   135,   156,   855,   156,   139,    32,   190,
     171,   194,   191,   118,   189,   189,   189,   189,   156,   172,
     189,    20,    21,   695,   194,   189,   172,   194,   196,   189,
      51,   189,   189,   463,   166,   189,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   189,   817,    63,   196,
      37,   108,    66,  1037,   205,   187,   188,    64,   751,   864,
     165,   166,    82,   195,     1,   170,   936,   968,   173,   971,
     521,   817,   788,   595,   575,   320,    45,   406,   806,   751,
     312,   335,   336,   188,   323,   108,   109,   459,   938,   194,
    1027,   345,   197,   116,    -1,   118,   119,   120,   121,   353,
     354,   355,   125,   357,   358,   359,   956,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   817,    -1,    -1,    -1,    -1,
      -1,   405,   175,   176,   177,  1005,    -1,    58,    59,  1009,
      -1,    -1,    -1,    -1,   187,   188,    -1,    -1,    -1,   939,
     940,   170,   171,   172,   173,   174,   175,   176,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   441,   187,   188,
     444,    -1,   446,   939,   940,    32,   450,   451,   452,   453,
     454,   455,    -1,   457,    -1,   459,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1066,   118,    -1,    -1,
      -1,    58,    59,   477,   478,    -1,    -1,   481,   482,   483,
     484,    -1,   486,  1003,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,    -1,   509,    -1,  1003,    -1,    -1,
      -1,    -1,    -1,    -1,   165,   166,    -1,   939,   940,   170,
     524,   172,   173,    -1,   528,    -1,    -1,   531,   532,    -1,
      -1,   118,    -1,   537,    -1,   539,    -1,   188,    -1,    -1,
      -1,    -1,   546,    -1,    -1,    -1,   197,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   567,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   579,    -1,    -1,   165,   166,
      -1,  1003,    -1,   170,    -1,   172,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   599,    -1,    -1,    -1,    -1,
      -1,   188,    -1,    -1,    -1,    -1,    -1,  1029,    -1,    -1,
     197,    -1,    -1,   617,   618,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   628,    -1,    -1,   631,   632,  1051,
      -1,   635,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     684,    -1,    -1,   687,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   697,    -1,    -1,   166,   701,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,    -1,    -1,
      -1,   715,    -1,    -1,    -1,    -1,    -1,   187,   188,    -1,
      32,    -1,    -1,    -1,   194,    -1,    -1,   731,   732,   733,
     734,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,    -1,    58,    59,    -1,    -1,
      -1,    -1,    -1,    -1,   758,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    20,
      21,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,   797,   798,    -1,    -1,    -1,    -1,    -1,
     804,    -1,   806,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   166,   821,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,    -1,    -1,
      -1,    -1,    -1,   837,    -1,    -1,    -1,   187,   188,    -1,
      -1,    -1,    -1,    -1,   194,    -1,    -1,    -1,   852,   853,
      -1,    -1,    -1,   165,   166,    -1,    -1,    -1,   170,    -1,
     172,   173,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,   877,   878,   116,   188,   118,   119,   120,
     121,    -1,   886,   887,   125,   197,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
     171,   172,   173,   174,   175,   176,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   949,   187,   188,    -1,   953,
      -1,   955,    -1,    -1,    -1,   959,   166,    -1,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,    -1,    -1,
     974,    -1,    -1,   977,   978,   979,   980,   187,   188,   983,
      -1,   191,   986,   987,    -1,   989,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   997,    -1,     1,    -1,    -1,    -1,    -1,
       6,    -1,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    -1,    -1,    -1,    -1,  1031,    -1,    -1,
      -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,  1043,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    -1,
      56,    57,    -1,    -1,    -1,    -1,  1060,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,   174,   175,
      -1,   177,    -1,    -1,   180,   181,    -1,    -1,    -1,    -1,
      -1,    58,    59,    -1,   190,    -1,   192,   193,   194,   195,
     196,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    -1,    42,    43,    -1,    -1,    46,    -1,    48,    49,
      50,   118,    52,    53,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    96,    -1,   165,   166,
      -1,    -1,    -1,   170,    -1,   172,   173,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
     197,    -1,   132,    -1,    -1,    32,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,   174,   175,    -1,   177,    -1,    -1,
     180,   181,    -1,    -1,    -1,    14,    15,    16,    17,    18,
     190,    -1,   192,   193,   194,   195,   196,    26,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,   118,    51,    -1,    53,    -1,    -1,    -1,    -1,    58,
      59,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,   165,   166,
      -1,    -1,    -1,   170,    -1,   172,   173,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,   132,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      58,    59,    -1,    -1,    -1,    -1,   165,   166,    -1,    -1,
      -1,   170,    -1,    -1,   173,   174,   175,    -1,   177,    -1,
      -1,   180,   181,    14,    15,    16,    17,    18,    -1,   188,
      -1,   190,    -1,   192,   193,    26,   195,    -1,   197,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
     118,    -1,    53,    -1,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    -1,    -1,   165,   166,    -1,
      -1,    -1,   170,    -1,    -1,   173,    -1,   108,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
     188,    -1,   190,    -1,    -1,    -1,    -1,    -1,    -1,   197,
      -1,   132,    32,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    -1,    58,    59,
      -1,    -1,    -1,    -1,   165,   166,    -1,    -1,    -1,   170,
      -1,    -1,   173,   174,   175,    -1,   177,    -1,    -1,   180,
     181,    14,    15,    16,    17,    18,    -1,   188,    -1,   190,
      -1,   192,   193,    26,   195,    -1,   197,    -1,    32,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,   118,    -1,
      53,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,   165,   166,    -1,    -1,    -1,
     170,    -1,   172,   173,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,   132,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    -1,    58,    59,    -1,    -1,
      -1,   165,   166,    -1,    -1,    -1,   170,   170,   172,   173,
     173,   174,   175,    -1,   177,    -1,    -1,   180,   181,    14,
      15,    16,    17,    18,   188,   188,    -1,   190,    -1,   192,
     193,    26,   195,   197,   197,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,   118,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,   165,   166,    -1,    -1,    -1,   170,    -1,
     172,   173,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,   188,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,   197,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,   173,   174,
     175,    -1,   177,    -1,    26,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,    38,   192,   193,   194,
     195,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    58,    59,    -1,    -1,   140,   141,
     142,   143,   144,    -1,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   173,   174,   175,    32,   177,    -1,    -1,   180,   181,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   190,    46,
     192,   193,    49,   195,   118,    -1,    53,    -1,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,   165,   166,    -1,    -1,    -1,   170,    -1,   172,   173,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,    -1,   132,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,    58,    59,    -1,    -1,    -1,   165,   166,    -1,
      -1,    -1,   170,    -1,   172,   173,   173,   174,   175,    -1,
     177,    -1,    -1,   180,   181,    14,    15,    16,    17,    18,
     188,   188,    -1,   190,   191,   192,   193,    26,   195,   197,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,   117,   118,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,   165,
     166,    -1,    -1,    -1,   170,    -1,    -1,   173,    -1,   108,
     109,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   123,   124,    -1,    -1,    -1,    -1,
      -1,   197,    -1,   132,    -1,    -1,    -1,    58,    59,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   173,   174,   175,    -1,   177,    -1,
      -1,   180,   181,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   190,    46,   192,   193,    49,   195,   118,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,   165,   166,    -1,    -1,    -1,   170,
      -1,   172,   173,    -1,   108,   109,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,   197,    -1,   132,    -1,
      -1,    -1,    58,    59,    -1,    -1,   140,   141,   142,   143,
     144,    -1,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   173,
     174,   175,    -1,   177,    -1,    -1,   180,   181,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   190,    46,   192,   193,
      49,   195,   118,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,   165,
     166,    -1,    -1,    -1,   170,    -1,   172,   173,    -1,   108,
     109,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   123,   124,    -1,    -1,    -1,    -1,
      -1,   197,    -1,   132,    -1,    -1,    -1,    58,    59,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   173,   174,   175,    -1,   177,    -1,
      -1,   180,   181,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   190,    46,   192,   193,    49,   195,   118,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,   165,   166,    -1,    -1,    -1,   170,
      -1,   172,   173,    -1,   108,   109,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,   132,    -1,
      -1,    -1,    58,    59,    -1,    -1,   140,   141,   142,   143,
     144,    -1,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,   173,
     174,   175,    32,   177,    -1,    -1,   180,   181,    -1,    38,
      -1,    -1,    -1,    -1,    43,   189,   190,    46,   192,   193,
      49,   195,   118,    -1,    53,    -1,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,   165,
     166,    -1,    -1,    -1,   170,    -1,   172,   173,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      14,    15,    16,    17,    18,   165,   166,    -1,    -1,    -1,
     170,    -1,    26,   173,   173,   174,   175,    -1,   177,    -1,
      -1,   180,   181,    -1,    38,    -1,    -1,    -1,   188,    43,
      -1,   190,    46,   192,   193,    49,   195,   197,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
     174,   175,    -1,   177,    -1,    -1,   180,   181,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   190,   191,   192,   193,
      26,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   173,   174,   175,
      -1,   177,    -1,    -1,   180,   181,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   190,    46,   192,   193,    49,   195,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,   174,   175,    -1,   177,    -1,    -1,   180,
     181,    14,    15,    16,    17,    18,    -1,    -1,    -1,   190,
     191,   192,   193,    26,   195,    -1,    -1,    -1,    -1,    -1,
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
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,
     173,   174,   175,    -1,   177,    -1,    -1,   180,   181,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   190,    -1,   192,
     193,    26,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,   174,
     175,    -1,   177,    -1,    -1,   180,   181,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   190,   191,   192,   193,    26,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,   174,   175,    -1,
     177,    -1,    -1,   180,   181,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   190,   191,   192,   193,    26,   195,    -1,
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
      -1,   140,   141,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,   174,   175,    -1,   177,    -1,
      -1,   180,   181,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   190,   191,   192,   193,    26,   195,    -1,    -1,    -1,
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
     141,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,   174,   175,    -1,   177,    -1,    -1,   180,
     181,    14,    15,    16,    17,    18,    -1,    -1,    -1,   190,
     191,   192,   193,    26,   195,    -1,    -1,    -1,    -1,    -1,
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
     143,   144,    -1,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,   174,   175,    -1,   177,    -1,    -1,   180,   181,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   190,   191,   192,
     193,    26,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   173,   174,
     175,    -1,   177,    -1,    -1,   180,   181,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   190,    46,   192,   193,    49,
     195,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,   174,   175,    -1,   177,    -1,    -1,
     180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,    -1,   192,   193,    -1,   195,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      20,    21,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,   166,    -1,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,   188,    -1,    -1,    -1,
      -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    20,    21,    -1,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,    -1,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,    -1,
      -1,    -1,    -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    20,    21,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,   166,    -1,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,    -1,    -1,    -1,    -1,    -1,   194,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    20,    21,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
     166,    -1,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,    -1,    -1,    -1,    -1,    -1,   194,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    20,    21,
      -1,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,   166,    -1,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,    -1,    -1,    -1,    -1,    -1,
     194,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      20,    21,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,   166,    -1,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,   188,    -1,    -1,    -1,
      18,    -1,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    20,    21,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    64,    65,    66,   139,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,   166,    -1,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,    -1,
      -1,   191,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    20,    21,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,   156,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,   187,   188,
      -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    20,    21,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    64,    65,
      66,   139,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,   166,    -1,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,    -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
     156,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
     187,   188,    -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      64,    65,    66,   139,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
     166,    -1,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,    -1,    -1,   191,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,   156,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,   188,    -1,    -1,   191,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,    -1,    -1,   191,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,   188,    -1,    -1,   191,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,   188,    -1,    -1,   191,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      20,    21,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,   188,    -1,    -1,
     191,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    20,    21,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,    -1,
      -1,   191,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    20,    21,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
      -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    20,    21,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,    -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,    -1,    -1,   191,    -1,    -1,    -1,    20,    21,
     106,   107,   108,   109,   110,    -1,    -1,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,   189,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,   188,   189,    -1,    -1,
      -1,    -1,    -1,    20,    21,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    -1,    -1,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    20,    21,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    20,
      21,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,   166,    -1,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,    20,    21,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,   137,   138,   139,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,   166,    -1,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
      -1,    -1,   180,   181,   106,   107,   108,   109,   110,   187,
     188,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      20,    21,    -1,   125,    -1,   127,   128,    -1,    -1,   131,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,    -1,    20,
      21,    -1,   106,   107,   108,   109,   110,   187,   188,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
      -1,    -1,   166,    -1,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,    -1,    20,    21,
      -1,   106,   107,   108,   109,   110,   187,   188,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   187,   188,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    20,    21,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   187,   188,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    20,    21,    -1,    -1,   133,    -1,   135,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    20,
      21,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   187,   188,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    20,    21,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,    -1,    -1,    -1,    -1,    -1,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,    -1,    -1,    -1,
      -1,   106,   107,    -1,    -1,   110,   187,   188,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,   168,   169,   170,   171,   172,    -1,    -1,
      47,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,   188,    -1,    62,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,   156,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,   156,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,   145,    -1,   147,   148,   149,    -1,
      -1,    -1,    -1,    -1,    62,   156,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,   156,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   199,     0,     6,    29,    31,    33,    39,    48,    54,
      73,    94,   188,   194,   202,   210,   211,   215,   238,   242,
     259,   324,   330,   333,   339,   377,   380,    18,    19,   156,
     232,   233,   234,   150,   216,   217,   156,   177,   212,   213,
     156,   195,   327,   156,   192,   201,   381,   378,    33,    61,
     132,   156,   235,   236,   237,   251,     4,     5,     7,    35,
     337,    60,   322,   165,   164,   167,   164,   212,    21,    55,
     176,   187,   214,   328,   327,   329,   322,   156,   156,   156,
     132,   190,   164,   189,    55,    61,   243,   245,    55,    61,
     331,    55,    61,   338,    55,    61,   323,    14,    15,   150,
     154,   156,   157,   190,   204,   233,   150,   217,   156,   156,
     156,   165,   194,   196,   327,    55,    61,   200,   195,   379,
     156,   234,   236,   246,   156,   332,   340,   195,   325,   158,
     159,   203,    14,    15,   150,   154,   156,   204,   230,   231,
     214,    24,    30,    47,    62,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   145,   251,   343,   345,
     346,   348,   352,   194,   156,   347,   195,   191,    34,   108,
     109,   156,   241,   244,   167,   195,    97,   335,   336,   320,
     147,   300,   158,   159,   160,   164,   191,   171,   171,   171,
     171,   190,   171,   171,   171,   171,   171,   171,   190,    32,
      58,    59,   118,   165,   166,   170,   173,   188,   197,   194,
     156,   313,   314,    20,    21,    37,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   118,   119,   120,   126,   127,   128,
     129,   130,   133,   134,   135,   136,   137,   138,   139,   166,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     180,   181,   187,   188,    34,    34,   190,   239,   195,   247,
      68,    72,    89,    90,    91,    92,   344,   326,   156,   341,
     196,   321,   234,   156,   319,   342,   230,   355,   357,   359,
     353,   156,   349,   361,   363,   365,   367,   369,   371,   373,
     375,    14,    15,    16,    17,    18,    26,    38,    43,    46,
      49,    53,    63,    73,    95,   108,   109,   140,   141,   142,
     143,   144,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   157,   173,   174,   175,   177,   180,   181,   190,
     192,   193,   206,   208,   247,   251,   253,   261,   268,   271,
     274,   278,   279,   282,   283,   284,   289,   292,   299,   343,
     382,   386,   391,   393,   395,    32,    32,   170,   188,   197,
     189,   292,   156,   196,   167,   194,   189,    21,   189,   191,
     300,   309,   310,   167,   240,   249,   195,   156,   196,   167,
     334,   195,   300,   189,    41,   164,   167,   170,   318,   352,
     351,   352,   352,   352,   191,   347,   352,   239,   352,   239,
     352,   239,   156,   311,   312,   352,   314,   352,   382,   190,
     171,   190,   171,   171,   190,   171,   190,   171,   292,   292,
     190,   190,   190,   190,   190,   190,    12,   352,    12,   352,
     292,   388,   392,   205,   292,   292,   292,   251,   292,   292,
     292,   193,   156,   190,   238,    20,    21,   106,   107,   108,
     109,   110,   113,   114,   115,   116,   118,   119,   120,   121,
     125,   127,   128,   133,   134,   135,   139,   166,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   187,   188,
     191,   190,   189,   189,   352,   196,   313,    48,   316,   342,
     191,   194,   352,     1,     8,     9,    10,    12,    25,    27,
      28,    37,    40,    42,    50,    52,    56,    57,    63,    96,
     123,   124,   194,   196,   218,   219,   222,   223,   224,   225,
     226,   227,   228,   248,   250,   252,   254,   255,   256,   257,
     258,   259,   260,   280,   281,   292,   326,   165,   194,   251,
     305,   319,   189,   156,   156,   352,   117,   126,   165,   317,
     172,   172,   194,   172,   172,   172,   194,   172,   240,   172,
     240,   172,   240,   167,   172,   194,   172,   194,   190,   251,
     269,   292,   262,   264,   292,   266,   292,   352,   292,   292,
     292,   292,   292,   292,   342,    51,   156,   173,   190,   292,
     383,   384,   385,   387,   388,   389,   390,   342,   190,   384,
     390,   131,   194,   196,   160,   161,   203,   209,   191,   171,
     251,   188,   191,   272,   292,   147,   277,    18,   156,   343,
      18,   156,   343,   292,   292,   292,   292,   292,   292,   156,
     292,   156,   292,   292,   292,   292,   292,   292,   292,   292,
     292,    21,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   119,   120,   156,   187,   188,   290,   292,
     191,   272,   316,   117,   165,   167,   170,   315,   309,   292,
     342,   117,   194,   250,   280,   292,   247,   292,   292,   156,
     194,   150,    56,   292,   247,   117,   250,   292,   193,   278,
     278,    36,   194,   194,   292,   194,   194,   194,   319,   122,
     194,    98,    99,   100,   101,   102,   103,   104,   105,   111,
     112,   117,   126,   129,   130,   136,   137,   138,   165,   196,
     292,   188,   196,   238,   306,    41,   165,   194,   317,   250,
     292,   356,   358,   352,   360,   354,   350,   362,   172,   366,
     172,   370,   172,   352,   374,   311,   376,   191,   272,   171,
     292,   352,   191,   352,   352,   191,   352,   191,   172,   191,
     191,   191,   191,   191,   191,    19,   278,   126,   315,   191,
     164,   194,   385,   189,   164,   189,   194,    19,   191,   385,
     196,   292,   388,   196,   292,   158,   207,   285,   287,   156,
     383,   164,   191,   117,   126,   165,   170,   275,   276,   239,
     171,   171,   190,   189,    18,   156,   343,   167,   156,   292,
     156,   292,     1,   189,   191,   352,   250,   292,   247,    19,
     250,   292,   117,   165,   194,    13,   247,   165,   167,   150,
     250,   292,   194,   193,   195,   247,   278,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   123,   124,   292,
     292,   292,   292,   292,   292,   292,   123,   124,   292,   194,
     237,     7,   300,   304,   156,   250,   292,   194,   364,   368,
     372,   191,   156,   191,   172,   172,   172,   172,   277,   190,
     272,   292,   292,   384,   385,   156,   383,   189,   189,   292,
     189,   389,   272,   384,   196,   189,   162,   158,   352,   239,
     189,   292,   156,   156,   156,   156,   164,   189,   240,   293,
     295,   191,   272,   171,   292,   189,   189,   291,   165,   315,
     272,   194,   247,   229,   194,   249,    11,    22,    23,   220,
     221,   292,   292,   292,   292,   189,    55,    61,   303,    44,
      97,   301,   194,   194,   172,   194,   270,   263,   265,   267,
     190,   191,   272,   194,   385,   189,   126,   315,   189,   194,
     385,   189,   172,   240,   191,   275,   189,   131,   247,   273,
     352,   352,   191,   297,   292,   194,   292,   196,   247,   292,
     194,    45,   301,   303,   292,   156,   292,   292,   292,   191,
     292,   191,   292,   189,   189,   292,   292,   292,   196,   286,
     172,   117,   292,   172,   172,   352,   247,   247,   307,    32,
     302,   316,   191,   172,   191,   194,   394,   189,   394,   189,
     251,   288,   292,   294,   296,   172,   221,   244,   308,   292,
      51,   189,   196,   251,   298,   194,   244,   191,   292,   189,
     189,   247
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   198,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   200,   200,   200,   201,   201,
     202,   203,   203,   203,   203,   204,   205,   205,   205,   206,
     207,   207,   209,   208,   210,   211,   212,   212,   212,   212,
     213,   213,   214,   214,   215,   216,   216,   217,   217,   218,
     219,   219,   220,   220,   221,   221,   221,   222,   222,   223,
     224,   225,   226,   227,   229,   228,   230,   230,   230,   230,
     230,   230,   231,   231,   232,   232,   232,   233,   233,   233,
     233,   233,   233,   233,   233,   234,   234,   235,   235,   235,
     236,   236,   237,   237,   238,   238,   239,   239,   239,   240,
     240,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   242,   243,   243,   243,   244,
     246,   245,   247,   247,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   249,   249,   249,   250,   250,   250,
     251,   251,   251,   252,   253,   253,   253,   253,   254,   255,
     256,   256,   256,   256,   256,   257,   257,   257,   257,   258,
     259,   259,   260,   262,   263,   261,   264,   265,   261,   266,
     267,   261,   269,   270,   268,   271,   271,   271,   272,   272,
     273,   273,   273,   274,   274,   274,   275,   275,   275,   275,
     276,   276,   277,   277,   278,   278,   279,   279,   279,   279,
     279,   279,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   281,   281,   281,   281,   282,   283,   283,   284,   285,
     286,   284,   287,   288,   284,   289,   289,   290,   291,   289,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   293,
     294,   292,   292,   292,   292,   295,   296,   292,   292,   292,
     297,   298,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   299,   299,   299,   299,   299,
     299,   299,   300,   300,   301,   301,   301,   302,   302,   303,
     303,   303,   304,   305,   306,   305,   307,   305,   308,   305,
     305,   309,   309,   310,   310,   311,   311,   312,   312,   313,
     314,   314,   315,   315,   316,   316,   316,   316,   316,   316,
     317,   317,   317,   318,   318,   319,   319,   319,   319,   319,
     320,   321,   320,   322,   322,   323,   323,   323,   324,   325,
     324,   326,   326,   326,   328,   327,   329,   329,   330,   330,
     331,   331,   331,   332,   333,   333,   334,   334,   335,   335,
     336,   337,   337,   338,   338,   338,   340,   341,   339,   342,
     342,   342,   342,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   344,   344,
     344,   344,   344,   344,   345,   346,   346,   346,   347,   347,
     349,   350,   348,   351,   351,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   353,   354,   352,   352,   355,   356,   352,   357,
     358,   352,   359,   360,   352,   352,   361,   362,   352,   363,
     364,   352,   352,   365,   366,   352,   367,   368,   352,   352,
     369,   370,   352,   371,   372,   352,   373,   374,   352,   375,
     376,   352,   378,   379,   377,   381,   380,   382,   382,   382,
     382,   383,   383,   383,   383,   384,   384,   385,   385,   386,
     386,   386,   386,   386,   386,   387,   387,   388,   388,   389,
     389,   390,   390,   391,   391,   392,   392,   393,   394,   394,
     395,   395
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       3,     4,     5,     4,     5,     3,     4,     1,     3,     4,
       3,     4,     2,     4,     4,     7,     8,     3,     5,     0,
       0,     8,     3,     3,     3,     0,     0,     8,     3,     4,
       0,     0,     9,     4,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     4,     4,     4,     4,     4,     4,
       6,     7,     0,     4,     0,     1,     1,     0,     1,     0,
       1,     1,     4,     0,     0,     4,     0,     8,     0,     9,
       5,     2,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     1,     1,     2,     3,     5,     3,     3,
       1,     1,     1,     0,     1,     4,     6,     5,     5,     4,
       0,     0,     4,     0,     1,     0,     1,     1,     6,     0,
       6,     0,     3,     5,     0,     4,     2,     3,     4,     2,
       0,     1,     1,     1,     7,     9,     0,     2,     0,     1,
       3,     1,     1,     0,     1,     1,     0,     0,     9,     1,
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

  case 299: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 300: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 301: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 302: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 303: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 304: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 305: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 306: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 307: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 308: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 309: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 310: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 311: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 312: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 313: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 314: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 315: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 316: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 317: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 318: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 319: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 320: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 325: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 326: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 329: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 330: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 331: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 332: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 333: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 334: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 335: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 336: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 337: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 338: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 339: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 340: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 341: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 342: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 344: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 345: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' ')'  */
                                                                                                   {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 346: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' expr ')'  */
                                                                                                                 {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 347: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 349: /* $@18: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 350: /* $@19: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 351: /* expr: expr "is" "type" '<' $@18 type_declaration '>' $@19  */
                                                                                                                                            {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 352: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 353: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 354: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 355: /* $@20: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 356: /* $@21: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 357: /* expr: expr "as" "type" '<' $@20 type_declaration '>' $@21  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 358: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 359: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 360: /* $@22: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 361: /* $@23: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 362: /* expr: expr '?' "as" "type" '<' $@22 type_declaration '>' $@23  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 363: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 364: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 366: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 367: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 368: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 369: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 370: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 371: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 372: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) =ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 373: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 374: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 375: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 376: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 377: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 378: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 379: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 380: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 381: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 382: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 383: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 384: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 385: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 386: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 387: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 388: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 389: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 390: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 391: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 392: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 393: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 394: /* $@24: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 395: /* struct_variable_declaration_list: struct_variable_declaration_list $@24 structure_variable_declaration ';'  */
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

  case 396: /* $@25: %empty  */
                                                                                             {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 397: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@25 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
            }
    break;

  case 398: /* $@26: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 399: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@26 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 400: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 401: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 402: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 403: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 404: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 405: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 406: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 407: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 408: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 409: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 410: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 411: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 412: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 413: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 414: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 415: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 416: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 417: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 418: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 419: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 420: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 421: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 422: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 423: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 424: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 425: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration ';'  */
                                                                             {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 426: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr ';'  */
                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 427: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr_pipe  */
                                                                                                                    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 428: /* let_variable_declaration: variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                      {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 429: /* let_variable_declaration: variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 430: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 431: /* $@27: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 432: /* global_variable_declaration_list: global_variable_declaration_list $@27 optional_field_annotation let_variable_declaration  */
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

  case 433: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 434: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 435: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 436: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 437: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 438: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 439: /* $@28: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 440: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@28 optional_field_annotation let_variable_declaration  */
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

  case 441: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 442: /* enum_list: enum_list "name" ';'  */
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

  case 443: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 444: /* $@29: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 445: /* single_alias: "name" $@29 '=' type_declaration  */
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

  case 450: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 451: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 452: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 453: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 454: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].s)->c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pEnum),tokAt(scanner,(yylsp[-1])),Type::tInt);
    }
    break;

  case 455: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                      {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].s)->c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].s),tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pEnum),tokAt(scanner,(yylsp[-1])),(yyvsp[-3].type));
    }
    break;

  case 456: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 457: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 458: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 459: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 460: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 461: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 462: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 463: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 464: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 465: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 466: /* $@30: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 467: /* $@31: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 468: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@30 structure_name $@31 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
        }
    }
    break;

  case 469: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 470: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 471: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 472: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 473: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 474: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 475: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 476: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 477: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 478: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 479: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 480: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 481: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 482: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 483: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 484: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 485: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 486: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 487: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 488: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 489: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 490: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 491: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 492: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 493: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 494: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 495: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 496: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 497: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 498: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 499: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 500: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 501: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 502: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 503: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 504: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 505: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 506: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 507: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 508: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 509: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 510: /* $@32: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 511: /* $@33: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 512: /* bitfield_type_declaration: "bitfield" '<' $@32 bitfield_bits '>' $@33  */
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

  case 513: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 514: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 515: /* type_declaration: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 516: /* type_declaration: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 517: /* type_declaration: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 518: /* type_declaration: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 519: /* type_declaration: type_declaration '[' expr ']'  */
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

  case 520: /* type_declaration: type_declaration '[' ']'  */
                                           {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 521: /* type_declaration: type_declaration '-' '[' ']'  */
                                               {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 522: /* type_declaration: type_declaration "explicit"  */
                                                {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 523: /* type_declaration: type_declaration "const"  */
                                             {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 524: /* type_declaration: type_declaration '-' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 525: /* type_declaration: type_declaration '&'  */
                                       {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 526: /* type_declaration: type_declaration '-' '&'  */
                                           {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 527: /* type_declaration: type_declaration '#'  */
                                       {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 528: /* type_declaration: type_declaration "implicit"  */
                                                {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 529: /* type_declaration: type_declaration '-' '#'  */
                                           {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 530: /* type_declaration: type_declaration '=' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 531: /* type_declaration: type_declaration '?'  */
                                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 532: /* $@34: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 533: /* $@35: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 534: /* type_declaration: "smart_ptr" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 535: /* type_declaration: type_declaration "??"  */
                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 536: /* $@36: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 537: /* $@37: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 538: /* type_declaration: "array" '<' $@36 type_declaration '>' $@37  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 539: /* $@38: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 540: /* $@39: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 541: /* type_declaration: "table" '<' $@38 table_type_pair '>' $@39  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 542: /* $@40: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 543: /* $@41: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 544: /* type_declaration: "iterator" '<' $@40 type_declaration '>' $@41  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 545: /* type_declaration: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 546: /* $@42: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 547: /* $@43: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 548: /* type_declaration: "block" '<' $@42 type_declaration '>' $@43  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 549: /* $@44: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 550: /* $@45: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 551: /* type_declaration: "block" '<' $@44 optional_function_argument_list optional_function_type '>' $@45  */
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

  case 552: /* type_declaration: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 553: /* $@46: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 554: /* $@47: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 555: /* type_declaration: "function" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 556: /* $@48: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 557: /* $@49: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 558: /* type_declaration: "function" '<' $@48 optional_function_argument_list optional_function_type '>' $@49  */
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

  case 559: /* type_declaration: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 560: /* $@50: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 561: /* $@51: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 562: /* type_declaration: "lambda" '<' $@50 type_declaration '>' $@51  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 563: /* $@52: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 564: /* $@53: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 565: /* type_declaration: "lambda" '<' $@52 optional_function_argument_list optional_function_type '>' $@53  */
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

  case 566: /* $@54: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 567: /* $@55: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 568: /* type_declaration: "tuple" '<' $@54 tuple_type_list '>' $@55  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 569: /* $@56: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 570: /* $@57: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 571: /* type_declaration: "variant" '<' $@56 variant_type_list '>' $@57  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 572: /* $@58: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 573: /* $@59: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 574: /* variant_alias_declaration: "variant" $@58 "name" $@59 '{' variant_type_list ';' '}'  */
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

  case 575: /* $@60: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 576: /* bitfield_alias_declaration: "bitfield" $@60 "name" '{' bitfield_bits ';' '}'  */
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

  case 577: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 578: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 579: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 580: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 581: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 582: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 583: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 584: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 585: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 586: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 587: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 588: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 589: /* make_struct_decl: "[[" type_declaration make_struct_dim optional_block ']' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 590: /* make_struct_decl: "[[" type_declaration optional_block ']' ']'  */
                                                                        {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 591: /* make_struct_decl: "[[" type_declaration '(' ')' optional_block ']' ']'  */
                                                                                {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 592: /* make_struct_decl: "[[" type_declaration '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                     {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 593: /* make_struct_decl: "[{" type_declaration make_struct_dim optional_block '}' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 594: /* make_struct_decl: "[{" type_declaration '(' ')' make_struct_dim optional_block '}' ']'  */
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

  case 595: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 596: /* make_tuple: make_tuple ',' expr  */
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

  case 597: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 598: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 599: /* make_any_tuple: make_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 600: /* make_any_tuple: make_map_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 601: /* make_dim: make_any_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 602: /* make_dim: make_dim ';' make_any_tuple  */
                                              {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 603: /* make_dim_decl: "[[" type_declaration make_dim ']' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 604: /* make_dim_decl: "[{" type_declaration make_dim '}' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 605: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 606: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 607: /* make_table_decl: "{{" make_table '}' '}'  */
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

  case 608: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 609: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 610: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 611: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


