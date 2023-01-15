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
  YYSYMBOL_expression_else_one_liner = 224, /* expression_else_one_liner  */
  YYSYMBOL_225_2 = 225,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 226,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 227,  /* expression_if_then_else  */
  YYSYMBOL_228_3 = 228,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 229,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 230,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 231,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 232,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 233,    /* expression_with_alias  */
  YYSYMBOL_234_4 = 234,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 235, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 236, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 237, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 238,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 239, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 240, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration = 241,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 242,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 243, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 244, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 245,   /* optional_function_type  */
  YYSYMBOL_function_name = 246,            /* function_name  */
  YYSYMBOL_global_function_declaration = 247, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 248, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 249, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 250,     /* function_declaration  */
  YYSYMBOL_251_5 = 251,                    /* $@5  */
  YYSYMBOL_expression_block = 252,         /* expression_block  */
  YYSYMBOL_expression_any = 253,           /* expression_any  */
  YYSYMBOL_expressions = 254,              /* expressions  */
  YYSYMBOL_expr_pipe = 255,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 256,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 257,        /* expression_delete  */
  YYSYMBOL_expr_new = 258,                 /* expr_new  */
  YYSYMBOL_expression_break = 259,         /* expression_break  */
  YYSYMBOL_expression_continue = 260,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 261, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 262,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 263, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 264,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 265,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 266,                  /* kwd_let  */
  YYSYMBOL_expression_let = 267,           /* expression_let  */
  YYSYMBOL_expr_cast = 268,                /* expr_cast  */
  YYSYMBOL_269_6 = 269,                    /* $@6  */
  YYSYMBOL_270_7 = 270,                    /* $@7  */
  YYSYMBOL_271_8 = 271,                    /* $@8  */
  YYSYMBOL_272_9 = 272,                    /* $@9  */
  YYSYMBOL_273_10 = 273,                   /* $@10  */
  YYSYMBOL_274_11 = 274,                   /* $@11  */
  YYSYMBOL_expr_type_decl = 275,           /* expr_type_decl  */
  YYSYMBOL_276_12 = 276,                   /* $@12  */
  YYSYMBOL_277_13 = 277,                   /* $@13  */
  YYSYMBOL_expr_type_info = 278,           /* expr_type_info  */
  YYSYMBOL_expr_list = 279,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 280,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 281,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 282,            /* capture_entry  */
  YYSYMBOL_capture_list = 283,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 284,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 285,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 286,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 287,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 288,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 289,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 290,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 291,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 292,           /* func_addr_expr  */
  YYSYMBOL_293_14 = 293,                   /* $@14  */
  YYSYMBOL_294_15 = 294,                   /* $@15  */
  YYSYMBOL_295_16 = 295,                   /* $@16  */
  YYSYMBOL_296_17 = 296,                   /* $@17  */
  YYSYMBOL_expr_field = 297,               /* expr_field  */
  YYSYMBOL_298_18 = 298,                   /* $@18  */
  YYSYMBOL_299_19 = 299,                   /* $@19  */
  YYSYMBOL_expr = 300,                     /* expr  */
  YYSYMBOL_301_20 = 301,                   /* $@20  */
  YYSYMBOL_302_21 = 302,                   /* $@21  */
  YYSYMBOL_303_22 = 303,                   /* $@22  */
  YYSYMBOL_304_23 = 304,                   /* $@23  */
  YYSYMBOL_305_24 = 305,                   /* $@24  */
  YYSYMBOL_306_25 = 306,                   /* $@25  */
  YYSYMBOL_expr_mtag = 307,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 308, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 309,        /* optional_override  */
  YYSYMBOL_optional_constant = 310,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 311, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 312, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 313, /* struct_variable_declaration_list  */
  YYSYMBOL_314_26 = 314,                   /* $@26  */
  YYSYMBOL_315_27 = 315,                   /* $@27  */
  YYSYMBOL_316_28 = 316,                   /* $@28  */
  YYSYMBOL_function_argument_declaration = 317, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 318,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 319,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 320,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 321,             /* variant_type  */
  YYSYMBOL_variant_type_list = 322,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 323,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 324,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 325,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 326,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 327, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 328, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 329, /* global_variable_declaration_list  */
  YYSYMBOL_330_29 = 330,                   /* $@29  */
  YYSYMBOL_optional_shared = 331,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 332, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 333,               /* global_let  */
  YYSYMBOL_334_30 = 334,                   /* $@30  */
  YYSYMBOL_enum_list = 335,                /* enum_list  */
  YYSYMBOL_single_alias = 336,             /* single_alias  */
  YYSYMBOL_337_31 = 337,                   /* $@31  */
  YYSYMBOL_alias_list = 338,               /* alias_list  */
  YYSYMBOL_alias_declaration = 339,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 340, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 341,                /* enum_name  */
  YYSYMBOL_enum_declaration = 342,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 343, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 344,          /* optional_sealed  */
  YYSYMBOL_structure_name = 345,           /* structure_name  */
  YYSYMBOL_class_or_struct = 346,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 347, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 348,    /* structure_declaration  */
  YYSYMBOL_349_32 = 349,                   /* $@32  */
  YYSYMBOL_350_33 = 350,                   /* $@33  */
  YYSYMBOL_variable_name_with_pos_list = 351, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 352,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 353, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 354, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 355,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 356,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 357, /* bitfield_type_declaration  */
  YYSYMBOL_358_34 = 358,                   /* $@34  */
  YYSYMBOL_359_35 = 359,                   /* $@35  */
  YYSYMBOL_table_type_pair = 360,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 361, /* type_declaration_no_options  */
  YYSYMBOL_362_36 = 362,                   /* $@36  */
  YYSYMBOL_363_37 = 363,                   /* $@37  */
  YYSYMBOL_364_38 = 364,                   /* $@38  */
  YYSYMBOL_365_39 = 365,                   /* $@39  */
  YYSYMBOL_366_40 = 366,                   /* $@40  */
  YYSYMBOL_367_41 = 367,                   /* $@41  */
  YYSYMBOL_368_42 = 368,                   /* $@42  */
  YYSYMBOL_369_43 = 369,                   /* $@43  */
  YYSYMBOL_370_44 = 370,                   /* $@44  */
  YYSYMBOL_371_45 = 371,                   /* $@45  */
  YYSYMBOL_372_46 = 372,                   /* $@46  */
  YYSYMBOL_373_47 = 373,                   /* $@47  */
  YYSYMBOL_374_48 = 374,                   /* $@48  */
  YYSYMBOL_375_49 = 375,                   /* $@49  */
  YYSYMBOL_376_50 = 376,                   /* $@50  */
  YYSYMBOL_377_51 = 377,                   /* $@51  */
  YYSYMBOL_378_52 = 378,                   /* $@52  */
  YYSYMBOL_379_53 = 379,                   /* $@53  */
  YYSYMBOL_380_54 = 380,                   /* $@54  */
  YYSYMBOL_381_55 = 381,                   /* $@55  */
  YYSYMBOL_382_56 = 382,                   /* $@56  */
  YYSYMBOL_383_57 = 383,                   /* $@57  */
  YYSYMBOL_384_58 = 384,                   /* $@58  */
  YYSYMBOL_385_59 = 385,                   /* $@59  */
  YYSYMBOL_type_declaration = 386,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 387, /* variant_alias_declaration  */
  YYSYMBOL_388_60 = 388,                   /* $@60  */
  YYSYMBOL_389_61 = 389,                   /* $@61  */
  YYSYMBOL_bitfield_alias_declaration = 390, /* bitfield_alias_declaration  */
  YYSYMBOL_391_62 = 391,                   /* $@62  */
  YYSYMBOL_make_decl = 392,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 393,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 394,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 395,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 396,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 397,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 398,           /* make_map_tuple  */
  YYSYMBOL_make_dim = 399,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 400,            /* make_dim_decl  */
  YYSYMBOL_make_table = 401,               /* make_table  */
  YYSYMBOL_make_table_decl = 402,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 403, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 404       /* array_comprehension  */
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
#define YYLAST   11018

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  199
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  206
/* YYNRULES -- Number of rules.  */
#define YYNRULES  666
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1176

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
       0,   474,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   490,   491,   492,   496,   497,
     501,   519,   520,   521,   522,   526,   530,   535,   544,   552,
     568,   573,   581,   581,   611,   632,   636,   639,   643,   649,
     658,   661,   667,   668,   672,   676,   677,   681,   684,   690,
     696,   699,   705,   706,   710,   711,   712,   721,   722,   726,
     727,   727,   733,   734,   735,   736,   737,   741,   747,   747,
     753,   759,   767,   777,   786,   786,   793,   794,   795,   796,
     797,   798,   802,   807,   815,   816,   817,   821,   822,   823,
     824,   825,   826,   827,   828,   834,   837,   843,   844,   845,
     849,   857,   870,   874,   881,   882,   886,   887,   888,   892,
     895,   902,   906,   907,   908,   909,   910,   911,   912,   913,
     914,   915,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,   926,   927,   928,   929,   930,   931,   932,   933,
     934,   935,   936,   937,   938,   939,   940,   941,   942,   943,
     944,   945,   946,   947,   948,   949,   950,   951,   952,   953,
     954,   955,   956,   957,   958,   959,   960,   961,   962,   963,
     964,   965,   966,   967,   968,   969,   970,   971,   972,   973,
     974,   975,   976,   977,   978,   979,   980,   981,   986,  1008,
    1009,  1010,  1014,  1020,  1020,  1037,  1041,  1052,  1053,  1054,
    1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,
    1065,  1066,  1067,  1068,  1069,  1070,  1071,  1075,  1080,  1086,
    1092,  1126,  1129,  1135,  1136,  1147,  1151,  1154,  1162,  1165,
    1168,  1172,  1178,  1182,  1186,  1189,  1192,  1200,  1203,  1206,
    1214,  1217,  1225,  1228,  1231,  1239,  1245,  1246,  1250,  1256,
    1256,  1256,  1259,  1259,  1259,  1264,  1264,  1264,  1272,  1272,
    1272,  1278,  1288,  1299,  1314,  1317,  1323,  1324,  1331,  1342,
    1343,  1344,  1348,  1349,  1350,  1351,  1355,  1360,  1368,  1369,
    1373,  1378,  1385,  1386,  1387,  1388,  1389,  1390,  1394,  1395,
    1396,  1397,  1398,  1399,  1400,  1401,  1402,  1403,  1404,  1405,
    1406,  1407,  1408,  1409,  1410,  1411,  1412,  1416,  1417,  1418,
    1419,  1423,  1434,  1439,  1449,  1453,  1460,  1463,  1463,  1463,
    1468,  1468,  1468,  1481,  1485,  1489,  1489,  1489,  1496,  1497,
    1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,  1507,
    1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,  1516,  1517,
    1518,  1519,  1520,  1521,  1522,  1523,  1524,  1525,  1526,  1527,
    1528,  1529,  1530,  1531,  1537,  1538,  1539,  1540,  1541,  1542,
    1543,  1544,  1545,  1546,  1547,  1548,  1549,  1553,  1557,  1560,
    1563,  1564,  1565,  1566,  1569,  1572,  1573,  1576,  1576,  1576,
    1579,  1584,  1588,  1592,  1592,  1592,  1597,  1600,  1604,  1604,
    1604,  1609,  1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,
    1620,  1621,  1622,  1630,  1631,  1632,  1633,  1634,  1635,  1639,
    1643,  1647,  1651,  1655,  1659,  1663,  1667,  1671,  1678,  1679,
    1683,  1684,  1685,  1689,  1690,  1694,  1695,  1696,  1700,  1710,
    1713,  1713,  1732,  1731,  1746,  1745,  1758,  1767,  1773,  1778,
    1788,  1789,  1793,  1796,  1805,  1806,  1810,  1819,  1820,  1825,
    1826,  1830,  1836,  1842,  1845,  1849,  1855,  1864,  1865,  1866,
    1870,  1871,  1875,  1882,  1887,  1896,  1902,  1913,  1916,  1921,
    1926,  1934,  1945,  1948,  1948,  1968,  1969,  1973,  1974,  1975,
    1979,  1982,  1982,  2001,  2004,  2013,  2026,  2026,  2047,  2048,
    2052,  2053,  2057,  2058,  2059,  2063,  2073,  2080,  2090,  2091,
    2095,  2096,  2100,  2106,  2107,  2111,  2112,  2113,  2117,  2122,
    2117,  2132,  2139,  2144,  2153,  2159,  2170,  2171,  2172,  2173,
    2174,  2175,  2176,  2177,  2178,  2179,  2180,  2181,  2182,  2183,
    2184,  2185,  2186,  2187,  2188,  2189,  2190,  2191,  2192,  2193,
    2194,  2198,  2199,  2200,  2201,  2202,  2203,  2207,  2218,  2222,
    2229,  2241,  2248,  2257,  2257,  2257,  2270,  2274,  2281,  2282,
    2283,  2284,  2285,  2299,  2305,  2309,  2313,  2318,  2323,  2328,
    2333,  2337,  2341,  2346,  2350,  2355,  2355,  2355,  2361,  2368,
    2368,  2368,  2373,  2373,  2373,  2379,  2379,  2379,  2384,  2388,
    2388,  2388,  2393,  2393,  2393,  2402,  2406,  2406,  2406,  2411,
    2411,  2411,  2420,  2424,  2424,  2424,  2429,  2429,  2429,  2438,
    2438,  2438,  2444,  2444,  2444,  2453,  2456,  2467,  2483,  2483,
    2483,  2507,  2507,  2527,  2528,  2529,  2530,  2534,  2541,  2548,
    2554,  2563,  2568,  2575,  2576,  2580,  2586,  2593,  2601,  2608,
    2616,  2628,  2631,  2637,  2651,  2657,  2663,  2668,  2675,  2680,
    2690,  2695,  2702,  2714,  2715,  2719,  2722
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
  "expression_else_one_liner", "$@2", "expression_if_one_liner",
  "expression_if_then_else", "$@3", "expression_for_loop",
  "expression_unsafe", "expression_while_loop", "expression_with",
  "expression_with_alias", "$@4", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "annotation_declaration_name", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@5", "expression_block", "expression_any",
  "expressions", "expr_pipe", "name_in_namespace", "expression_delete",
  "expr_new", "expression_break", "expression_continue",
  "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let", "expression_let", "expr_cast", "$@6", "$@7", "$@8", "$@9",
  "$@10", "$@11", "expr_type_decl", "$@12", "$@13", "expr_type_info",
  "expr_list", "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@14", "$@15", "$@16", "$@17", "expr_field", "$@18",
  "$@19", "expr", "$@20", "$@21", "$@22", "$@23", "$@24", "$@25",
  "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@26", "$@27", "$@28", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@29", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@30", "enum_list",
  "single_alias", "$@31", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_name", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@32", "$@33", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@34", "$@35", "table_type_pair",
  "type_declaration_no_options", "$@36", "$@37", "$@38", "$@39", "$@40",
  "$@41", "$@42", "$@43", "$@44", "$@45", "$@46", "$@47", "$@48", "$@49",
  "$@50", "$@51", "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58",
  "$@59", "type_declaration", "variant_alias_declaration", "$@60", "$@61",
  "bitfield_alias_declaration", "$@62", "make_decl", "make_struct_fields",
  "make_struct_dim", "optional_block", "make_struct_decl", "make_tuple",
  "make_map_tuple", "make_dim", "make_dim_decl", "make_table",
  "make_table_decl", "array_comprehension_where", "array_comprehension", YY_NULLPTR
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

#define YYPACT_NINF (-821)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-617)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -821,    19,  -821,  -821,    46,    14,   -77,   -89,  -821,   -88,
    -821,  -821,    20,  -821,  -821,  -821,  -821,  -821,   306,  -821,
      31,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
     -32,  -821,   154,    98,   214,  -821,  -821,   -77,     8,  -821,
    -821,   229,  -821,  -821,  -821,    31,   252,   276,  -821,  -821,
     285,   287,   337,  -821,   178,  -821,  -821,  -821,   115,   320,
     422,  -821,   454,    16,    46,   292,    14,   279,   302,  -821,
     378,   387,  -821,   354,   366,  -103,   455,   367,  -821,  -821,
     402,    46,    20,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
     405,  -821,  -821,  -821,  -821,  -821,   369,  -821,  -821,  -821,
    -821,  -821,   340,    60,  -821,  -821,  -821,  -821,   509,  -821,
    -821, 10683,  -821,  -821,   376,  -821,  -821,  -821,   411,   403,
    -821,    61,  -821, 10861,  -821,   -15,   501,  -821,   424,  -821,
    -821,   388,  -821,  -821,  -821,  -821,  -821,  -821,  -821,    79,
    -821,   443,   444,   447,   449,  -821,  -821,  -821,   432,  -821,
    -821,  -821,  -821,  -821,   452,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,   459,  -821,  -821,  -821,   460,   461,
    -821,  -821,  -821,  -821,   462,   464,   438,  -821,  -821,  -821,
    -821,  -821,   724,   468,  -821,  -821,   445,   481,  -821,  9338,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,   605,   609,  -821,   465,   463,   401,  -821,
    -821,   498,  -821,   469,    46,   101,  -821,  -821,  -821,    60,
    -821,  -821,  -821,  -821,  -821,   503,  -821,   192,   256,   262,
    -821,  -821,  5914,  -821,  -821,  -821,   629,  -821,  -821,  -821,
      11,  3540,  -821,   210,   -47,   505,  -821,   482,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,   486,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,   659,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,   493,  -821,
    -821,   -65,   516,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,   489,   -35,   518,   491,  -821,   424,   211,   497,   648,
     346,  -821,  -821, 10683, 10683, 10683, 10683,   499,   411, 10683,
     465, 10683,   465, 10683,   465, 10777,   481,  -821,  -821, 10589,
     502,   520,  -821,   504,   522,   524,   507,   527,   510,  -821,
     528,  5914,  5914,   512,   513,   514,   517,   519,   521, 10370,
   10464,  5914,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  5914,
    5914,  5914,   246,  5914,  5914,  5914,  -821,   496,  -821,  -821,
    -821,     2,  -821,  -821,  -821,  -821,   525,  -821,  -821,  -821,
    -821,  -821,  -821,  6301,  -821,   526,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,   523,  -821,  -821,  8769,  -821,   724,
    -821,  -821, 10683,   -34,  -821,  -821,  -821,   529,  -821,    -9,
    -821,   171, 10683,  -821,  1603,  -821,    70,  -821,   246,  -821,
    -821,   101,   531,  5914,   550,   554, 10683,  -821,    23,   -91,
     542,   149,    86,   159,  -821,   223,   313,   516,   328,   516,
     329,   516,    -4,  -821,   263,   468,   271,     1,  -821,   246,
    -821,  5914,  -821,  -821,  5914,  -821,  5914, 10683,    35,    35,
    5914,  5914,  5914,  5914,  5914,  5914,   104,  1972,   104,  2141,
    9434,  -821,   257,   392,    35,    35,   -58,  -821,    35,    35,
    6403,   -48,  -821,  2933,   568,  1420,  9506,  5914,  5914,  -821,
    -821,  5914,  5914,  5914,  5914,   561,  5914,   168,  5914,  5914,
    5914,  5914,  5914,  5914,  5914,  5914,  5914,  3686,  5914,  5914,
    5914,  5914,  5914,  5914,  5914,  5914,  5914,  5914,   -43,  5914,
    -821,  3832,  -821,  -821,   468,  -821,  -821,  5914,   104,   532,
     678,  -821,   102,  -821,    36,   468,  -821,  5914,  -821,  -821,
     104,  2641,  -821,   463,  4001,  5914,   565,  -821,   533,   573,
    4147,  -101,  2787,   364,   364,  -821,   694,   536,   537,  5914,
     725,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,   538,
     544,   545,   546,  -821,   548,  -821,  -821,   101,  -821,   -75,
    -821,   475,   -28,  5914,  -821,  -821,    15,  -821,  -821,  6494,
    -821,   703,   677,  -821,  -821,  -821,  3102,  -821,  -821, 10683,
    -821,  -821,  -821,   588,  -821,   574,  -821,   575,  -821,   576,
   10683,  -821, 10777,  -821,   481,  4293,  4462, 10683,  6585, 10683,
   10683,  6676, 10683,  6767,   314,  6858,  6949,  7040,  7131,  7222,
    7313,    21,   364,   -60,  2310,  4631,  9511,   581,   -17,   560,
     587,   187,    22,  4800,   -17,   275,  5914,  5914,   557,  -821,
    5914,   363,   596,  -821,   571,   577,   303,  -821,  -821,   616,
    -821,   147,  9756,    -5,   465,   602,   584,  -821,  -821,   608,
     586,  -821,  -821,   330,   330,   286,   286,  6378,  6378,   590,
      17,   601,  -821,  8860,   -31,   -31,   330,   330, 10059,  9950,
   10027,  9833, 10495,  9595, 10136, 10161, 10238,   286,   286,   549,
     549,    17,    17,    17,   268,  5914,   606,  -821,   270,  5914,
     786,  8951,  -821,   155,  7404,  -821,  5914,   636,  -821,   639,
    -821, 10683,  -821,  3102,  -821,  1025,    26,  3102,  -821,   676,
    9235,   787,  5914,  9756,  1025,   633,  -821,   634,   650,  9756,
    -821,  3102,  -821,  9235,   610,  -821,  -821,   612,  -821,  -821,
    1025,  -821,  -821,  -821,  -821,  -821,  -821,  -821,   364,  -821,
    5914,  5914,  5914,  5914,  5914,  5914,  5914,  5914,  5914,  5914,
    3248,  5914,  5914,  5914,  5914,  5914,  5914,  3394,  -821,  1292,
      20,  -821,   798,   424,  -821,   649,  -821,  3102,  -821,  6013,
    -821,  -821,   468,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,   468,  -821,  -821,  -821,  -821,   169,   652,  7495,
     356,  -821,   508,   775,  -821,   882,  -821,   568,  -821,  -821,
    -821,  -821,  -821,   621,  5914,  -821,  5914,  5914,   -16,  5914,
     657,   616,   625,   626,  5914,   627,  5914,  5914,   616,   622,
     640,  9756,  -821,  -821,  9672,  -821,   672,  5914,  5914, 10683,
     465,   143,   232,  5914,  -821,   675,   679,   680,   684,  -821,
     233,   516,  -821,  5914,  -821,  5914,  4969,  5914,  -821,   663,
     654,  -821,  -821,  5914,   655,  -821,  9042,  5914,   656,  -821,
    9133,  -821,  -821,  -821,  -821,  7586,  -821,   808,   -66,  -821,
    9235,  -821,  5914,  -821,  9235,  5914,  5914,   463,  9756,  -821,
    -821,  -821,  -821,  -821,  9235,  -821,  -821,   457,  5914,  -821,
    9756,  9756,  9756,  9756,  9756,  9756,  9756,  9756,  9756,  9756,
    5914,  5914,  9756,  9756,  9756,  9756,  9756,  9756,  9756,  5914,
    5914,  9756,  -821,   251,   466,     5,   658,  -821,  -821,  6115,
    -821,  -821,  -821,  -821,  -821,   283,  -821,  -821,  -821,  -821,
    -821,   661,  5138,    18,  9756,  9756,   -17,   660,  9756,   188,
     581,   664,  -821,  9756,  -821,   587,    55,   -17,   665,  -821,
    -821,  -821,  7677,  7768,  1059,   516,   666,  9756,  -821,  -821,
    -821,  -821,    -5,   669,   -85, 10683,  7859, 10683,  7950,  -821,
     197,  8041,  -821,  5914,  9917,  5914,  -821,  8132,  5914,  -821,
    -821,  -821,   699,  5914,    56,  -821,  5914,  1800,   463,  -821,
    -821,  5914,  -821,   651,   790,   790,   790,   790,   668,  -821,
    -821,    42,  -821,  -821,   466,  -821,  -821,  5914,   707,  -821,
    5914,  5914,  5914,  5307,  -821,   199,  5914,   670,   686,  5914,
    5914,  -821,  5914,   671,  -821,  -821,  -821,  -821,   692,  -821,
    -821,  -821,  5476,  -821,  -821,  1064,  -821,   368,  -821,  -821,
    -821, 10683,  8223,  8314,  -821,  8405,  -821,  9756,   463,  9756,
    -821,  -821,  1025,  -821,   674,  -821,  -821,   835,   104,  8496,
     697,   286,   286,   286,  -821,  8587,  -821,  6217,   688,  -821,
    9756,  9756,  6217,   696,   280,  -821,  5914,  9833,  -821,  -821,
     373,  -821,  -821,  -821,  -821,   457,  2495,  -821, 10861,  -821,
    -821,  -821,  -821,  5914,  -821,   823,   698,  -821,   685,  -821,
    -821,   280,  9917,  -821,  -821,  -821,  -821,  5622,  5768,  -821,
    -821,  -821,  -821,  -821,  9756,   695, 10861,  8678,  5914,   702,
     704,  -821,  -821,  5914,  9756,  5914,  9756,  -821,   463,  -821,
    9756,  -821,  -821,  9756,  9756,  -821
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   104,     1,   246,     0,     0,     0,     0,   247,     0,
     631,   628,     0,    14,     3,    10,     9,     8,     0,     7,
     485,     6,    11,     5,     4,    12,    13,    85,    86,    84,
      93,    95,    34,    47,    44,    45,    36,     0,    42,    35,
     496,     0,   501,    19,    18,   485,     0,     0,    98,    99,
       0,   223,   100,   102,     0,    97,   514,   513,   189,   502,
     515,   486,   487,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,   629,   225,
       0,     0,     0,   105,   191,   190,   193,   188,   504,   503,
       0,   517,   516,   518,   489,   488,   491,    91,    92,    89,
      90,    88,     0,     0,    87,    96,    48,    46,    42,    39,
      38,     0,   498,   500,     0,    16,    17,    20,     0,     0,
     224,     0,   103,     0,   505,     0,   510,   482,   428,    21,
      22,     0,    80,    81,    78,    79,    77,    76,    82,     0,
      41,     0,     0,     0,     0,   526,   546,   527,   558,   528,
     532,   533,   534,   535,   550,   539,   540,   541,   542,   543,
     544,   545,   547,   548,   598,   531,   538,   549,   605,   612,
     529,   536,   530,   537,     0,     0,     0,   557,   568,   571,
     569,   570,   625,   497,   499,   561,     0,     0,   101,     0,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,     0,     0,   111,   106,     0,     0,   493,
     511,     0,   519,   483,     0,     0,    23,    24,    25,     0,
      94,   589,   592,   595,   585,     0,   563,   599,   606,   613,
     619,   622,     0,   576,   581,   575,     0,   588,   584,   578,
       0,     0,   580,     0,     0,     0,   457,     0,   162,   163,
     160,   114,   115,   117,   116,   118,   119,   120,   121,   147,
     148,   145,   146,   138,   149,   150,   139,   136,   137,   161,
     158,     0,   159,   151,   152,   153,   154,   125,   126,   127,
     122,   123,   124,   135,     0,   141,   142,   140,   133,   134,
     129,   128,   130,   131,   132,   113,   112,   157,     0,   143,
     144,   428,   109,   217,   194,   551,   554,   552,   555,   553,
     556,     0,     0,   508,     0,   490,   428,     0,     0,   472,
     470,   492,    83,     0,     0,     0,     0,     0,     0,     0,
     106,     0,   106,     0,   106,     0,     0,   334,   335,     0,
       0,     0,   328,     0,     0,     0,     0,     0,     0,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   282,   284,   283,   285,   286,   287,    26,     0,
       0,     0,     0,     0,     0,     0,   269,   270,   332,   331,
     280,   329,   405,   404,   403,   402,   104,   408,   330,   407,
     406,   375,   336,     0,   337,     0,   333,   633,   634,   635,
     636,   583,   577,   579,     0,   582,   573,     0,   627,   626,
     562,   632,     0,     0,   156,   164,   155,     0,   107,     0,
     450,     0,     0,   192,     0,   493,     0,   506,     0,   512,
     439,     0,     0,     0,     0,     0,     0,   471,     0,     0,
       0,   566,     0,     0,   559,     0,     0,   109,     0,   109,
       0,   109,   223,   454,     0,   452,     0,   228,   231,     0,
     258,     0,   249,   252,     0,   255,     0,     0,   364,   365,
       0,     0,     0,     0,     0,     0,     0,   643,     0,     0,
     655,   660,     0,     0,   341,   340,   380,    32,   339,   338,
       0,   271,   411,     0,   278,     0,     0,     0,     0,   366,
     367,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   325,     0,
     560,     0,   574,   572,   456,   630,   458,     0,     0,     0,
     521,   447,   461,   108,   428,   110,   219,     0,    57,    58,
       0,   234,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   197,   195,     0,     0,     0,
       0,   212,   207,   204,   203,   205,   206,   218,   198,     0,
      65,    66,    63,   210,    64,   211,   213,     0,   202,     0,
     199,   288,     0,     0,   494,   509,   440,   484,   429,     0,
     474,   475,     0,   468,   469,   467,     0,   590,   593,     0,
     596,   586,   564,     0,   600,     0,   607,     0,   614,     0,
       0,   620,     0,   623,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   223,     0,     0,   651,   641,   643,     0,
     656,     0,     0,     0,   643,     0,     0,     0,     0,    29,
       0,    27,     0,   368,     0,     0,   317,   314,   316,     0,
     376,     0,   264,     0,   106,     0,     0,   391,   390,     0,
       0,   392,   396,   342,   343,   355,   356,   353,   354,     0,
     385,     0,   373,     0,   409,   410,   344,   345,   360,   361,
     362,   363,     0,     0,   358,   359,   357,   351,   352,   347,
     346,   348,   349,   350,     0,     0,     0,   323,     0,     0,
       0,     0,   378,     0,     0,   448,     0,     0,   460,     0,
     459,     0,   462,     0,   451,     0,     0,     0,   238,     0,
     235,     0,     0,   226,     0,     0,   216,     0,     0,    51,
      71,     0,   243,   240,   270,   221,   222,     0,   214,   215,
       0,    68,   201,   208,   209,   237,   242,   248,     0,   200,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,     0,
       0,   520,     0,   428,   473,     0,   477,     0,   481,   288,
     591,   594,   567,   597,   587,   565,   601,   603,   608,   610,
     615,   617,   453,   621,   455,   624,   229,     0,     0,     0,
       0,   381,     0,     0,   382,     0,   412,   278,   413,   414,
     415,   416,   417,     0,     0,   644,     0,     0,   643,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   654,   661,   662,     0,    30,    33,     0,     0,     0,
     106,     0,     0,     0,   377,     0,     0,     0,     0,   276,
       0,   109,   387,     0,   393,     0,     0,     0,   371,     0,
       0,   397,   401,     0,     0,   374,     0,     0,     0,   324,
       0,   326,   369,   379,   449,     0,   523,   524,   463,   466,
     465,    72,     0,   239,   236,     0,     0,     0,   227,    73,
      74,    49,    50,   244,   241,   271,   217,    54,     0,   220,
     298,   299,   301,   300,   302,   292,   293,   294,   303,   304,
       0,     0,   290,   291,   305,   306,   295,   296,   297,     0,
       0,   289,   495,     0,   435,   430,     0,   476,   479,   288,
     480,   604,   611,   618,   230,     0,   261,   259,   250,   253,
     256,     0,     0,     0,   638,   637,   643,     0,   652,     0,
     642,     0,   646,   653,   658,   657,     0,   643,     0,   659,
      28,    31,     0,     0,     0,   109,     0,   265,   274,   275,
     273,   272,     0,     0,     0,     0,     0,     0,     0,   312,
       0,     0,   398,     0,   386,     0,   372,     0,     0,   370,
     327,   522,     0,     0,     0,   245,     0,     0,     0,    52,
      53,     0,    67,    59,   309,   310,   307,   308,   105,   436,
     437,   430,   431,   432,   435,   441,   478,     0,     0,   260,
       0,     0,     0,     0,   418,     0,     0,     0,     0,     0,
       0,   645,     0,     0,   649,   315,   427,   318,     0,   311,
     277,   279,     0,   266,   281,     0,   426,     0,   424,   313,
     421,     0,     0,     0,   420,     0,   525,   464,     0,    75,
     196,    55,     0,    60,     0,   446,   442,   433,     0,     0,
       0,   251,   254,   257,   383,     0,   419,   663,     0,   647,
     640,   639,   663,     0,     0,   321,     0,   267,   388,   394,
       0,   425,   423,   422,    70,    54,     0,    69,     0,   434,
     444,   438,   262,     0,   384,     0,     0,   648,     0,   650,
     319,     0,   268,   389,   395,   399,    56,   234,     0,    61,
      65,    66,    63,    64,    62,     0,     0,     0,     0,     0,
       0,   322,   400,     0,   235,     0,   240,   443,     0,   263,
     664,   665,   666,   236,   241,   445
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -821,  -821,  -821,  -821,  -821,   394,   839,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,   870,  -821,   804,  -821,  -821,   850,
    -821,  -821,  -821,  -206,  -821,  -821,  -821,  -205,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,   701,  -821,  -821,   862,
     -45,  -821,   845,   128,  -368,  -332,  -450,  -821,  -821,  -821,
    -820,  -821,  -821,  -215,  -821,     6,  -545,   -12,  -821,  -821,
    -195,  -193,  -191,  -821,  -190,  -821,  -821,   936,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -527,  -821,  -821,   -64,  -821,   107,  -556,  -821,  -430,  -821,
    -821,  -821,  -761,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,   500,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -125,
    -102,  -821,   -99,  -821,  -821,  -821,  -821,  -821,   393,  -821,
     318,  -821,  -402,   619,  -539,  -542,   339,  -821,  -821,  -409,
    -821,  -821,   907,  -821,  -821,  -821,   534,   156,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -444,  -106,  -821,  -821,  -821,   617,  -821,  -821,
    -821,  -821,  -252,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,   -87,  -821,  -821,
    -821,  -821,  -821,   607,  -656,  -474,  -638,  -821,   110,   291,
     479,  -821,  -821,  -821,  -158,  -821
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   117,    45,    14,   131,   137,   493,   388,   866,
     389,   672,    15,    16,    38,    39,    72,    17,    34,    35,
     577,   578,  1031,  1032,   579,  1094,  1126,   580,   581,   928,
     582,   583,   584,   585,   586,  1026,   138,   139,    30,    31,
      32,    52,    53,    54,    18,   312,   433,   216,    19,    86,
     217,    87,   123,   390,   587,   434,   588,   391,   589,   392,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   393,
     639,  1050,   640,  1051,   642,  1052,   394,   637,  1049,   395,
     681,  1074,   396,   879,   880,   684,   397,   398,   749,   600,
     399,   400,   678,   401,   869,  1114,   870,  1141,   402,   730,
    1020,   682,  1005,  1143,  1007,  1144,  1081,  1162,   404,   429,
    1044,  1130,  1041,   956,   606,   803,  1128,  1156,   430,   431,
     463,   464,   256,   257,   847,   551,   616,   448,   330,   331,
     223,   326,    62,    96,    21,   128,   322,    42,    73,    75,
      22,    90,   125,    23,   439,   221,   222,    60,    93,    24,
     126,   324,   552,   405,   321,   179,   180,   186,   181,   338,
     815,   450,   182,   336,   814,   333,   810,   334,   811,   335,
     813,   339,   816,   340,   961,   341,   818,   342,   962,   343,
     820,   344,   963,   345,   823,   346,   825,   465,    25,    47,
     119,    26,    46,   406,   657,   658,   659,   407,   660,   491,
     661,   408,   492,   409,  1136,   410
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   419,   314,   225,   599,   178,   735,   625,   457,   627,
     459,   629,   461,   743,   733,   664,   748,   765,   766,     2,
     852,   546,  -104,   872,   183,     3,   859,   762,   504,    68,
      97,    98,   607,   243,   652,   652,   121,   505,   506,   548,
     844,   857,   651,   412,   662,   912,  1072,   778,     4,  1042,
       5,   738,     6,    48,    40,   505,   506,   738,     7,   244,
     245,   517,   518,    69,    27,    28,   846,     8,    40,    43,
      55,   808,    80,     9,   132,   133,   724,   725,   253,   427,
      36,    49,   617,   224,    50,   515,  1042,  1096,   517,   518,
     476,    61,    10,   674,   113,   313,   845,   467,   675,   177,
     740,    37,  1043,   253,   726,    44,   740,    41,   827,    51,
     420,   313,   875,    11,   727,   246,   746,   487,   489,   247,
     779,   876,   436,   255,   676,   509,   510,   428,    80,   436,
     538,   539,   549,   515,    63,   516,   517,   518,   519,  1043,
     613,   871,   520,   509,   510,   728,   729,   178,   550,   614,
     421,   515,    50,   218,   517,   518,   519,   538,   539,   502,
     520,   877,   437,   545,   630,    33,   878,    99,   248,   798,
      84,   100,   249,   101,   102,   250,    85,    51,   851,   327,
     427,   219,   413,   873,   224,    70,   739,   739,   777,   615,
     251,   739,   635,   503,   612,   980,    71,    74,   909,   252,
     414,   441,   913,    29,   800,   538,   539,   103,    12,   415,
     977,   134,   801,  1056,    13,   135,   923,   136,   102,   738,
     873,   873,   929,   538,   539,   644,    64,   178,   178,   178,
     178,   114,   546,   178,   141,   178,   603,   178,   802,   178,
     142,   177,   328,   178,   229,   549,   449,   451,   452,   453,
    1062,  1088,   456,   188,   458,   253,   460,   143,   329,   620,
     738,   550,   958,   178,   178,   604,    65,   739,   740,   846,
     741,   230,   144,   742,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   738,   505,   506,  1155,   740,
      56,    57,   873,    58,  1059,   701,   178,   973,   253,    64,
     873,   177,   177,   177,   177,   702,   178,   177,   253,   177,
     986,   177,   621,   177,   873,   544,  1168,   177,  1057,   874,
     178,    59,    50,    82,   619,   555,   243,   903,   751,  1063,
     505,   506,   881,  1140,   740,   176,   760,   177,   177,  1010,
    -602,   964,   873,   553,   873,  -602,   554,    51,    83,  1023,
     497,   178,   244,   245,   976,    88,    64,   855,    50,    66,
    1161,    89,   856,  -602,   987,  1024,    40,   832,   833,  1079,
     835,  1106,   507,   508,   509,   510,   622,   850,  1002,   688,
     692,   442,   515,    51,   516,   517,   518,   519,   418,    77,
     177,   520,    50,   521,   522,   894,    82,   898,   623,    80,
     177,   674,   996,  1003,  -609,   895,   605,   899,   246,  -609,
    -616,  1004,   247,    78,   177,  -616,   631,    51,   509,   510,
    1060,  1038,    79,   106,   633,  1055,   515,  -609,   516,   517,
     518,   519,   667,  -616,   668,   520,  1047,   636,   632,   108,
     533,   534,   535,   536,   537,   177,   634,    71,  1028,   315,
     856,  -320,   860,   316,   538,   539,  -320,    91,  1048,  1029,
    1030,   248,   253,    92,   -62,   249,   624,   837,   250,   677,
     317,   318,   319,   320,  -320,   505,   506,   253,   253,   129,
     130,   626,   628,   251,   533,   534,   535,   536,   537,    94,
     115,   445,   252,   178,   446,    95,   116,   447,   538,   539,
     111,  1039,   226,   227,   178,   253,   178,  1040,    81,   967,
     911,   178,   812,   178,   178,   109,   178,   253,   995,   919,
     243,  1119,   253,   822,   110,  1068,  1145,   226,   227,   228,
     830,   129,   130,   669,   670,   927,  1131,   386,   764,   120,
     313,   112,   124,   118,    69,   127,   244,   245,   185,   505,
     506,   184,   224,   780,   781,   782,   783,   784,   785,   786,
     787,   507,   508,   509,   510,   511,   788,   789,   512,   513,
     514,   515,   790,   516,   517,   518,   519,   599,   220,   187,
     520,   791,   521,   522,   792,   793,   892,   177,   523,   524,
     525,   794,   795,   796,   526,   231,   232,   994,   177,   233,
     177,   234,   246,   235,   236,   177,   247,   177,   177,   242,
     177,   237,   238,   239,   240,   178,   241,   253,   255,   309,
     254,   797,   527,   310,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   908,   323,   311,   509,   510,   313,
     337,   411,  1093,   538,   539,   515,   325,   516,   517,   518,
     519,   505,   506,   422,   520,   248,   424,   423,   955,   249,
     425,   968,   250,   426,   432,   435,   438,   440,   443,   444,
     501,   454,   470,   469,   472,   471,   473,   251,   474,   475,
     477,   476,  1025,   480,   481,   482,   252,   610,   483,   243,
     484,   611,   485,   542,    12,   618,   683,   541,   699,   737,
     547,   608,   755,   736,   757,   535,   536,   537,   756,   177,
     767,   768,   769,   772,   771,   244,   245,   538,   539,   773,
     774,   775,   403,   776,   805,   420,   850,   817,   819,   821,
     853,   417,   854,  1075,   863,   865,   243,   507,   508,   509,
     510,   511,   867,   178,   512,   513,   514,   515,   868,   516,
     517,   518,   519,   871,   882,   883,   520,   885,   521,   522,
     884,   886,   244,   245,   523,   524,   525,   901,    55,  1073,
     526,   246,   887,   906,   613,   247,   907,   897,   778,   920,
     917,   922,   921,   614,   925,   954,   957,   243,   926,   965,
     505,   506,   972,  1091,   979,   981,   982,   984,   527,   988,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     989,   991,   998,   244,   245,  1012,   999,  1000,   246,   538,
     539,  1001,   247,   615,   248,  1013,  1015,  1018,   249,  1022,
    1058,   250,  1053,  1045,  1061,  1064,  1086,   177,  1069,  1071,
    1108,   478,   479,  1095,  1100,  1115,   251,  1129,  1113,  1127,
    1133,   490,   806,  1124,  1158,   252,  1109,  1125,  1137,   494,
     495,   496,  1160,   498,   499,   500,  1139,   671,  1159,   246,
    1167,   248,  1171,   247,  1172,   249,   507,   508,   250,   178,
     511,   178,   104,   512,   513,   514,   515,    67,   516,   517,
     518,   519,   140,   251,   243,   520,   107,   521,   522,  1146,
    1077,  1149,   252,   523,   524,   525,   105,   122,   953,   526,
     332,  1150,  1027,  1151,   601,  1152,  1153,    20,  1070,  1097,
     244,   245,   248,   609,   971,  1098,   249,   744,   969,   250,
     824,   807,    76,  1175,  1138,   455,   468,   527,   862,   528,
     529,   530,   531,   532,   251,   466,   985,   536,   665,   602,
       0,   638,     0,   252,   641,   178,   643,     0,   538,   539,
     645,   646,   647,   648,   649,   650,     0,   656,     0,   656,
       0,     0,     0,   177,  1120,   177,   246,     0,     0,     0,
     247,     0,     0,     0,     0,     0,     0,   693,   694,     0,
       0,   695,   696,   697,   698,     0,   700,     0,   703,   704,
     705,   706,   707,   708,   709,   710,   711,   713,   714,   715,
     716,   717,   718,   719,   720,   721,   722,   723,     0,   731,
       0,     0,     0,     0,     0,   505,   506,   734,     0,   248,
       0,     0,     0,   249,     0,   970,   250,   745,     0,     0,
       0,   750,     0,     0,   753,   754,     0,     0,     0,   177,
     759,   251,   763,     0,     0,     0,     0,     0,     0,   770,
     252,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,     0,     0,     0,     0,   243,     0,     0,     0,
       0,     0,   677,   799,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   809,   244,   245,     0,
       0,     0,   244,   245,     0,     0,     0,     0,     0,   677,
       0,   507,   508,   509,   510,   511,   829,     0,   512,   513,
     514,   515,     0,   516,   517,   518,   519,     0,     0,     0,
     520,     0,   521,   522,   494,   500,     0,     0,   523,   524,
     525,     0,     0,   500,   526,     0,   861,   490,     0,     0,
     864,     0,     0,   246,     0,     0,     0,   247,   246,     0,
       0,     0,   247,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,     0,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   539,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,   896,   248,     0,     0,   900,
     249,   248,  1067,   250,     0,   249,   905,  1118,   250,     0,
       0,     0,     0,   910,     0,     0,     0,   914,   251,     0,
       0,     0,   918,   251,     0,     0,     0,   252,     0,     0,
       0,   924,   252,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     930,   931,   932,   933,   934,   935,   936,   937,   938,   939,
     942,   943,   944,   945,   946,   947,   948,   951,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   959,     0,     0,
       0,     0,   505,   506,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   974,   975,     0,   978,
       0,     0,     0,     0,   983,     0,   656,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   992,   993,     0,
       0,     0,     0,   997,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1006,     0,  1008,     0,  1011,     0,     0,
       0,     0,     0,  1014,     0,     0,     0,  1017,   507,   508,
     509,   510,   511,     0,     0,   512,   513,   514,   515,     0,
     516,   517,   518,   519,     0,   942,   951,   520,     0,   521,
     522,     0,     0,     0,     0,   523,   524,   525,  1033,     0,
       0,   526,     0,     0,     0,     0,     0,     0,   685,     0,
    1034,  1035,     0,     0,     0,     0,     0,     0,     0,  1036,
    1037,     0,     0,     0,     0,     0,     0,     0,     0,   527,
       0,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     538,   539,     0,     0,   145,   146,   147,   952,   149,   150,
     151,   152,   153,   359,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,  1082,     0,  1083,     0,     0,  1085,     0,
       0,     0,     0,  1087,     0,     0,  1089,   601,     0,     0,
       0,  1092,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1099,     0,     0,
    1101,  1102,  1103,  1105,     0,     0,  1107,     0,     0,  1110,
    1111,     0,  1112,     0,     0,     0,     0,   686,     0,     0,
       0,     0,  1117,     0,     0,     0,     0,   687,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   556,     0,     0,     0,     0,     3,
       0,   557,   558,   559,     0,   560,  1142,   347,   348,   349,
     350,   351,     0,     0,     0,     0,  1154,     0,   561,   352,
     562,   563,     0,  1157,     0,     0,     0,     0,     0,     0,
     564,   353,     0,   565,     0,   566,   354,  1164,  1166,   355,
       0,     8,   356,   567,     0,   568,   357,     0,  1170,   569,
     570,     0,     0,  1173,     0,  1174,   571,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   359,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   360,   572,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   361,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   573,   574,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   363,   364,   365,   366,   367,     0,   368,
       0,   369,   370,   371,   372,   373,   374,   375,   376,   377,
      51,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,   381,
       0,   382,     0,     0,   383,   384,     0,     0,     0,     0,
       0,     0,     0,     0,   385,     0,   386,   387,   575,   313,
     576,   556,     0,     0,     0,     0,     3,     0,   557,   558,
     559,     0,   560,     0,   347,   348,   349,   350,   351,     0,
       0,     0,     0,     0,     0,   561,   352,   562,   563,     0,
       0,     0,     0,     0,     0,     0,     0,   564,   353,     0,
     565,     0,   566,   354,     0,     0,   355,     0,     8,   356,
     567,     0,   568,   357,     0,     0,   569,   570,     0,     0,
       0,     0,     0,   571,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   359,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   360,   572,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   361,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   573,   574,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
     363,   364,   365,   366,   367,     0,   368,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,    51,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,   381,     0,   382,     0,
       0,   383,   384,     0,     0,     0,   347,   348,   349,   350,
     351,   385,     0,   386,   387,   575,   313,  1090,   352,     0,
       0,     0,     0,     0,   243,     0,     0,     0,     0,     0,
     353,     0,     0,     0,     0,   354,     0,     0,   355,     0,
       0,   356,     0,   652,     0,   357,     0,     0,     0,     0,
     244,   245,     0,     0,     0,   358,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   359,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     361,   362,     0,     0,     0,     0,   246,     0,     0,     0,
     247,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   363,   364,   365,   366,   367,     0,   368,     0,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   653,
     378,     0,     0,     0,     0,     0,     0,     0,     0,   248,
       0,     0,     0,   249,     0,     0,   654,   380,   381,     0,
     382,     0,     0,   383,   384,   347,   348,   349,   350,   351,
       0,   251,     0,   655,     0,   386,   387,   352,   313,     0,
     252,     0,     0,   243,     0,     0,     0,     0,     0,   353,
       0,     0,     0,     0,   354,     0,     0,   355,     0,     0,
     356,     0,     0,     0,   357,     0,     0,     0,     0,   244,
     245,     0,     0,     0,   358,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   359,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   360,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   361,
     362,     0,     0,     0,     0,   246,     0,     0,     0,   247,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   363,   364,   365,   366,   367,     0,   368,     0,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   653,   378,
       0,     0,     0,     0,     0,     0,     0,     0,   248,     0,
       0,     0,   249,     0,     0,   654,   380,   381,     0,   382,
       0,     0,   383,   384,   347,   348,   349,   350,   351,     0,
     251,     0,   663,     0,   386,   387,   352,   313,     0,   252,
       0,     0,   412,     0,     0,     0,     0,     0,   353,     0,
       0,     0,     0,   354,     0,     0,   355,     0,     0,   356,
       0,     0,     0,   357,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   358,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   359,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   360,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   361,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
     363,   364,   365,   366,   367,     0,   368,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,    51,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   413,     0,     0,   379,   380,   381,     0,   382,     0,
       0,   383,   384,     0,     0,     0,     0,     0,     0,   414,
       0,   385,     0,   386,   387,     0,   313,     0,   415,   347,
     348,   349,   350,   351,     0,     0,     0,     0,     0,     0,
    1147,   352,   562,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   353,     0,     0,     0,     0,   354,     0,
       0,   355,     0,     0,   356,   567,     0,     0,   357,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   358,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   359,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     360,  1148,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   361,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,   363,   364,   365,   366,   367,
       0,   368,     0,   369,   370,   371,   372,   373,   374,   375,
     376,   377,    51,   378,     0,   347,   348,   349,   350,   351,
       0,     0,     0,     0,     0,     0,     0,   352,     0,   379,
     380,   381,     0,   382,     0,     0,   383,   384,     0,   353,
       0,     0,     0,     0,   354,     0,   385,   355,   386,   387,
     356,   313,     0,     0,   357,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   358,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   359,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   360,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   361,
     362,     0,     0,     0,     0,     0,     0,     0,   747,     0,
       0,     0,     0,     0,   573,   574,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   363,   364,   365,   366,   367,     0,   368,     0,   369,
     370,   371,   372,   373,   374,   375,   376,   377,    51,   378,
       0,   347,   348,   349,   350,   351,     0,     0,     0,     0,
       0,     0,     0,   352,     0,   379,   380,   381,     0,   382,
       0,     0,   383,   384,     0,   353,     0,     0,     0,     0,
     354,     0,   385,   355,   386,   387,   356,   313,     0,     0,
     357,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     358,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     359,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   360,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   361,   362,     0,     0,     0,
       0,     0,     0,     0,   761,     0,     0,     0,     0,     0,
     573,   574,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   363,   364,   365,
     366,   367,     0,   368,     0,   369,   370,   371,   372,   373,
     374,   375,   376,   377,    51,   378,     0,   347,   348,   349,
     350,   351,     0,     0,     0,     0,     0,     0,     0,   352,
       0,   379,   380,   381,     0,   382,     0,     0,   383,   384,
       0,   353,     0,     0,     0,     0,   354,     0,   385,   355,
     386,   387,   356,   313,     0,     0,   357,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   358,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   359,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   360,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   361,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   363,   364,   365,   366,   367,     0,   368,
       0,   369,   370,   371,   372,   373,   374,   375,   376,   377,
      51,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,   381,
       0,   382,     0,     0,   383,   384,   347,   348,   349,   350,
     351,     0,   679,     0,   385,   680,   386,   387,   352,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     353,     0,     0,     0,     0,   354,     0,     0,   355,     0,
       0,   356,     0,     0,     0,   357,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   358,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   359,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     361,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   573,   574,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   363,   364,   365,   366,   367,     0,   368,     0,
     369,   370,   371,   372,   373,   374,   375,   376,   377,    51,
     378,     0,   347,   348,   349,   350,   351,     0,     0,     0,
       0,     0,     0,     0,   352,     0,   379,   380,   381,     0,
     382,     0,     0,   383,   384,     0,   353,     0,     0,     0,
       0,   354,     0,   385,   355,   386,   387,   356,   313,     0,
       0,   357,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   358,   145,   146,   147,     0,   149,   150,   151,   152,
     153,   359,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   165,   166,   167,     0,     0,   170,   171,   172,
     173,     0,     0,   360,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   361,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   940,   941,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,   363,   364,
     365,   366,   367,     0,   368,     0,   369,   370,   371,   372,
     373,   374,   375,   376,   377,    51,   378,     0,   347,   348,
     349,   350,   351,     0,     0,     0,     0,     0,     0,     0,
     352,     0,   379,   380,   381,     0,   382,     0,     0,   383,
     384,     0,   353,     0,     0,     0,     0,   354,     0,   385,
     355,   386,   387,   356,   313,     0,     0,   357,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   358,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   359,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,   360,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   361,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   949,   950,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,   363,   364,   365,   366,   367,     0,
     368,     0,   369,   370,   371,   372,   373,   374,   375,   376,
     377,    51,   378,     0,   347,   348,   349,   350,   351,     0,
       0,     0,     0,     0,     0,     0,   352,     0,   379,   380,
     381,     0,   382,     0,     0,   383,   384,     0,   353,     0,
       0,     0,     0,   354,     0,   385,   355,   386,   387,   356,
     313,     0,     0,   357,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   358,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   359,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   360,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   361,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
     363,   364,   365,   366,   367,     0,   368,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,    51,   378,     0,
     347,   348,   349,   350,   351,     0,     0,   712,     0,     0,
       0,     0,   352,     0,   379,   380,   381,     0,   382,     0,
       0,   383,   384,     0,   353,     0,     0,     0,     0,   354,
     416,   385,   355,   386,   387,   356,   313,     0,     0,   357,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   358,
     145,   146,   147,     0,   149,   150,   151,   152,   153,   359,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
       0,   360,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   361,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,   363,   364,   365,   366,
     367,     0,   368,     0,   369,   370,   371,   372,   373,   374,
     375,   376,   377,    51,   378,     0,   347,   348,   349,   350,
     351,     0,     0,     0,     0,     0,     0,     0,   352,     0,
     379,   380,   381,     0,   382,     0,     0,   383,   384,     0,
     353,     0,     0,     0,     0,   354,     0,   385,   355,   386,
     387,   356,   313,     0,     0,   357,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   358,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   359,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     361,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   363,   364,   365,   366,   367,     0,   368,     0,
     369,   370,   371,   372,   373,   374,   375,   376,   377,    51,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,   381,     0,
     382,     0,     0,   383,   384,   347,   348,   349,   350,   351,
       0,     0,     0,   385,   732,   386,   387,   352,   313,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,     0,     0,     0,   354,     0,     0,   355,     0,     0,
     356,     0,     0,     0,   357,     0,     0,     0,     0,     0,
     752,     0,     0,     0,   358,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   359,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   360,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   361,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   363,   364,   365,   366,   367,     0,   368,     0,   369,
     370,   371,   372,   373,   374,   375,   376,   377,    51,   378,
       0,   347,   348,   349,   350,   351,     0,     0,     0,     0,
       0,     0,     0,   352,     0,   379,   380,   381,     0,   382,
       0,     0,   383,   384,     0,   353,     0,     0,     0,     0,
     354,     0,   385,   355,   386,   387,   356,   313,     0,     0,
     357,     0,     0,   758,     0,     0,     0,     0,     0,     0,
     358,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     359,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   360,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   361,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   363,   364,   365,
     366,   367,     0,   368,     0,   369,   370,   371,   372,   373,
     374,   375,   376,   377,    51,   378,     0,   347,   348,   349,
     350,   351,     0,     0,     0,     0,     0,     0,     0,   352,
       0,   379,   380,   381,     0,   382,     0,     0,   383,   384,
       0,   353,     0,     0,     0,     0,   354,     0,   385,   355,
     386,   387,   356,   313,     0,     0,   357,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   358,   145,   146,   147,
       0,   149,   150,   151,   152,   153,   359,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,   165,   166,   167,
       0,     0,   170,   171,   172,   173,     0,     0,   360,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   361,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   363,   364,   365,   366,   367,     0,   368,
       0,   369,   370,   371,   372,   373,   374,   375,   376,   377,
      51,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,   381,
       0,   382,     0,     0,   383,   384,   347,   348,   349,   350,
     351,     0,     0,     0,   385,   826,   386,   387,   352,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     353,     0,     0,     0,     0,   354,     0,     0,   355,     0,
       0,   356,     0,     0,     0,   357,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   358,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   359,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     361,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   363,   364,   365,   366,   367,     0,   368,     0,
     369,   370,   371,   372,   373,   374,   375,   376,   377,    51,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   828,     0,   379,   380,   381,     0,
     382,     0,     0,   383,   384,   347,   348,   349,   350,   351,
       0,     0,     0,   385,     0,   386,   387,   352,   313,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,     0,     0,     0,   354,     0,     0,   355,     0,     0,
     356,     0,     0,     0,   357,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   358,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   359,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   360,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   361,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   363,   364,   365,   366,   367,     0,   368,     0,   369,
     370,   371,   372,   373,   374,   375,   376,   377,    51,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,   381,     0,   382,
       0,     0,   383,   384,   347,   348,   349,   350,   351,     0,
       0,     0,   385,   848,   386,   387,   352,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   353,     0,
       0,     0,     0,   354,     0,     0,   355,     0,     0,   356,
       0,     0,     0,   357,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   358,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   359,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   360,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   361,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
     363,   364,   365,   366,   367,     0,   368,     0,   369,   370,
     371,   372,   373,   374,   375,   376,   377,    51,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,   381,     0,   382,     0,
       0,   383,   384,   347,   348,   349,   350,   351,     0,     0,
       0,   385,   858,   386,   387,   352,   313,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   353,     0,     0,
       0,     0,   354,     0,     0,   355,     0,     0,   356,     0,
       0,     0,   357,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   358,   145,   146,   147,     0,   149,   150,   151,
     152,   153,   359,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   165,   166,   167,     0,     0,   170,   171,
     172,   173,     0,     0,   360,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   361,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   363,
     364,   365,   366,   367,     0,   368,     0,   369,   370,   371,
     372,   373,   374,   375,   376,   377,    51,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,   381,     0,   382,     0,     0,
     383,   384,   347,   348,   349,   350,   351,     0,     0,     0,
     385,  1009,   386,   387,   352,   313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   353,     0,     0,     0,
       0,   354,     0,     0,   355,     0,     0,   356,     0,     0,
       0,   357,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   358,   145,   146,   147,     0,   149,   150,   151,   152,
     153,   359,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   165,   166,   167,     0,     0,   170,   171,   172,
     173,     0,     0,   360,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   361,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,   363,   364,
     365,   366,   367,     0,   368,     0,   369,   370,   371,   372,
     373,   374,   375,   376,   377,    51,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,   381,     0,   382,     0,     0,   383,
     384,   347,   348,   349,   350,   351,     0,     0,     0,   385,
    1054,   386,   387,   352,   313,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   353,     0,     0,     0,     0,
     354,     0,     0,   355,     0,     0,   356,     0,     0,     0,
     357,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     358,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     359,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   360,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   361,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   363,   364,   365,
     366,   367,     0,   368,     0,   369,   370,   371,   372,   373,
     374,   375,   376,   377,    51,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,   381,     0,   382,     0,     0,   383,   384,
     347,   348,   349,   350,   351,     0,     0,     0,   385,  1104,
     386,   387,   352,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   353,     0,     0,     0,     0,   354,
       0,     0,   355,     0,     0,   356,     0,     0,     0,   357,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   358,
     145,   146,   147,     0,   149,   150,   151,   152,   153,   359,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
       0,   360,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   361,   362,     0,     0,     0,     0,
       0,     0,     0,  1116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,   363,   364,   365,   366,
     367,     0,   368,     0,   369,   370,   371,   372,   373,   374,
     375,   376,   377,    51,   378,     0,   347,   348,   349,   350,
     351,     0,     0,     0,     0,     0,     0,     0,   352,     0,
     379,   380,   381,     0,   382,     0,     0,   383,   384,     0,
     353,     0,     0,     0,     0,   354,     0,   385,   355,   386,
     387,   356,   313,     0,     0,   357,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   358,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   359,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     361,   362,     0,     0,     0,     0,     0,     0,     0,  1163,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   363,   364,   365,   366,   367,     0,   368,     0,
     369,   370,   371,   372,   373,   374,   375,   376,   377,    51,
     378,     0,   347,   348,   349,   350,   351,     0,     0,     0,
       0,     0,     0,     0,   352,     0,   379,   380,   381,     0,
     382,     0,     0,   383,   384,     0,   353,     0,     0,     0,
       0,   354,     0,   385,   355,   386,   387,   356,   313,     0,
       0,   357,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   358,   145,   146,   147,     0,   149,   150,   151,   152,
     153,   359,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   165,   166,   167,     0,     0,   170,   171,   172,
     173,     0,     0,   360,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   361,   362,     0,     0,
       0,     0,     0,     0,     0,  1165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,   363,   364,
     365,   366,   367,     0,   368,     0,   369,   370,   371,   372,
     373,   374,   375,   376,   377,    51,   378,     0,   347,   348,
     349,   350,   351,     0,     0,     0,     0,     0,     0,     0,
     352,     0,   379,   380,   381,     0,   382,     0,     0,   383,
     384,     0,   353,     0,     0,     0,     0,   354,     0,   385,
     355,   386,   387,   356,   313,     0,     0,   357,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   358,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   359,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,   360,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   361,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,   363,   364,   365,   366,   367,     0,
     368,     0,   369,   370,   371,   372,   373,   374,   375,   376,
     377,    51,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
     381,     0,   382,     0,     0,   383,   384,     0,     0,     0,
       0,     0,     0,     0,     0,   385,     0,   386,   387,     0,
     313,   780,   781,   782,   783,   784,   785,   786,   787,   507,
     508,   509,   510,   511,   788,   789,   512,   513,   514,   515,
     915,   516,   517,   518,   519,   505,   506,     0,   520,   791,
     521,   522,   792,   793,     0,     0,   523,   524,   525,   794,
     795,   796,   526,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   916,
     527,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,     0,     0,     0,     0,     0,   960,     0,
       0,     0,     0,   780,   781,   782,   783,   784,   785,   786,
     787,   507,   508,   509,   510,   511,   788,   789,   512,   513,
     514,   515,   915,   516,   517,   518,   519,   505,   506,     0,
     520,   791,   521,   522,   792,   793,     0,     0,   523,   524,
     525,   794,   795,   796,   526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   916,   527,     0,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   539,     0,     0,     0,     0,     0,
    1046,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   505,   506,   507,   508,   509,   510,   511,     0,     0,
     512,   513,   514,   515,     0,   516,   517,   518,   519,     0,
       0,     0,   520,     0,   521,   522,     0,     0,     0,     0,
     523,   524,   525,     0,     0,     0,   526,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,     0,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,     0,     0,   505,   506,
       0,     0,     0,     0,     0,   538,   539,   507,   508,   509,
     510,   511,  1135,     0,   512,   513,   514,   515,     0,   516,
     517,   518,   519,   505,   506,     0,   520,     0,   521,   522,
       0,     0,     0,     0,   523,   524,   525,     0,     0,     0,
     526,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,     0,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
       0,     0,     0,     0,   507,   508,   509,   510,   511,   538,
     539,   512,     0,   540,   515,     0,   516,   517,   518,   519,
       0,     0,     0,   520,     0,   521,   522,     0,     0,   507,
     508,   509,   510,   511,   505,   506,   512,   513,   514,   515,
       0,   516,   517,   518,   519,     0,     0,     0,   520,     0,
     521,   522,     0,     0,     0,     0,   523,   524,   525,     0,
       0,     0,   526,     0,     0,     0,     0,     0,     0,     0,
     531,   532,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   538,   539,     0,     0,
     527,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,     0,     0,   673,     0,     0,     0,     0,
     507,   508,   509,   510,   511,   505,   506,   512,   513,   514,
     515,     0,   516,   517,   518,   519,     0,     0,     0,   520,
       0,   521,   522,     0,     0,     0,     0,   523,   524,   525,
       0,     0,     0,   526,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   527,     0,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   538,   539,     0,     0,   804,     0,     0,     0,
       0,   507,   508,   509,   510,   511,   505,   506,   512,   513,
     514,   515,     0,   516,   517,   518,   519,     0,     0,     0,
     520,     0,   521,   522,     0,     0,     0,     0,   523,   524,
     525,     0,     0,     0,   526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,     0,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   539,     0,     0,   831,     0,     0,
       0,     0,   507,   508,   509,   510,   511,   505,   506,   512,
     513,   514,   515,     0,   516,   517,   518,   519,     0,     0,
       0,   520,     0,   521,   522,     0,     0,     0,     0,   523,
     524,   525,     0,     0,     0,   526,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   527,     0,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   538,   539,     0,     0,   834,     0,
       0,     0,     0,   507,   508,   509,   510,   511,   505,   506,
     512,   513,   514,   515,     0,   516,   517,   518,   519,     0,
       0,     0,   520,     0,   521,   522,     0,     0,     0,     0,
     523,   524,   525,     0,     0,     0,   526,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,     0,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   538,   539,     0,     0,   836,
       0,     0,     0,     0,   507,   508,   509,   510,   511,   505,
     506,   512,   513,   514,   515,     0,   516,   517,   518,   519,
       0,     0,     0,   520,     0,   521,   522,     0,     0,     0,
       0,   523,   524,   525,     0,     0,     0,   526,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   527,     0,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   538,   539,     0,     0,
     838,     0,     0,     0,     0,   507,   508,   509,   510,   511,
     505,   506,   512,   513,   514,   515,     0,   516,   517,   518,
     519,     0,     0,     0,   520,     0,   521,   522,     0,     0,
       0,     0,   523,   524,   525,     0,     0,     0,   526,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,     0,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   538,   539,     0,
       0,   839,     0,     0,     0,     0,   507,   508,   509,   510,
     511,   505,   506,   512,   513,   514,   515,     0,   516,   517,
     518,   519,     0,     0,     0,   520,     0,   521,   522,     0,
       0,     0,     0,   523,   524,   525,     0,     0,     0,   526,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   527,     0,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   538,   539,
       0,     0,   840,     0,     0,     0,     0,   507,   508,   509,
     510,   511,   505,   506,   512,   513,   514,   515,     0,   516,
     517,   518,   519,     0,     0,     0,   520,     0,   521,   522,
       0,     0,     0,     0,   523,   524,   525,     0,     0,     0,
     526,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,     0,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   538,
     539,     0,     0,   841,     0,     0,     0,     0,   507,   508,
     509,   510,   511,   505,   506,   512,   513,   514,   515,     0,
     516,   517,   518,   519,     0,     0,     0,   520,     0,   521,
     522,     0,     0,     0,     0,   523,   524,   525,     0,     0,
       0,   526,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   527,
       0,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     538,   539,     0,     0,   842,     0,     0,     0,     0,   507,
     508,   509,   510,   511,   505,   506,   512,   513,   514,   515,
       0,   516,   517,   518,   519,     0,     0,     0,   520,     0,
     521,   522,     0,     0,     0,     0,   523,   524,   525,     0,
       0,     0,   526,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,     0,     0,   843,     0,     0,     0,     0,
     507,   508,   509,   510,   511,   505,   506,   512,   513,   514,
     515,     0,   516,   517,   518,   519,     0,     0,     0,   520,
       0,   521,   522,     0,     0,     0,     0,   523,   524,   525,
       0,     0,     0,   526,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   527,     0,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   538,   539,     0,     0,   904,     0,     0,     0,
       0,   507,   508,   509,   510,   511,   505,   506,   512,   513,
     514,   515,     0,   516,   517,   518,   519,     0,     0,     0,
     520,     0,   521,   522,     0,     0,     0,     0,   523,   524,
     525,     0,     0,     0,   526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,     0,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   539,     0,     0,   966,     0,     0,
       0,     0,   507,   508,   509,   510,   511,   505,   506,   512,
     513,   514,   515,     0,   516,   517,   518,   519,     0,     0,
       0,   520,     0,   521,   522,     0,     0,     0,     0,   523,
     524,   525,     0,     0,     0,   526,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   527,     0,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   538,   539,     0,     0,  1021,     0,
       0,     0,     0,   507,   508,   509,   510,   511,   505,   506,
     512,   513,   514,   515,     0,   516,   517,   518,   519,     0,
       0,     0,   520,     0,   521,   522,     0,     0,     0,     0,
     523,   524,   525,     0,     0,     0,   526,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,     0,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   538,   539,     0,     0,  1065,
       0,     0,     0,     0,   507,   508,   509,   510,   511,   505,
     506,   512,   513,   514,   515,     0,   516,   517,   518,   519,
       0,     0,     0,   520,     0,   521,   522,     0,     0,     0,
       0,   523,   524,   525,     0,     0,     0,   526,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   527,     0,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   538,   539,     0,     0,
    1066,     0,     0,     0,     0,   507,   508,   509,   510,   511,
     505,   506,   512,   513,   514,   515,     0,   516,   517,   518,
     519,     0,     0,     0,   520,     0,   521,   522,     0,     0,
       0,     0,   523,   524,   525,     0,     0,     0,   526,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,     0,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   538,   539,     0,
       0,  1076,     0,     0,     0,     0,   507,   508,   509,   510,
     511,   505,   506,   512,   513,   514,   515,     0,   516,   517,
     518,   519,     0,     0,     0,   520,     0,   521,   522,     0,
       0,     0,     0,   523,   524,   525,     0,     0,     0,   526,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   527,     0,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   538,   539,
       0,     0,  1078,     0,     0,     0,     0,   507,   508,   509,
     510,   511,   505,   506,   512,   513,   514,   515,     0,   516,
     517,   518,   519,     0,     0,     0,   520,     0,   521,   522,
       0,     0,     0,     0,   523,   524,   525,     0,     0,     0,
     526,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,     0,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   538,
     539,     0,     0,  1080,     0,     0,     0,     0,   507,   508,
     509,   510,   511,   505,   506,   512,   513,   514,   515,     0,
     516,   517,   518,   519,     0,     0,     0,   520,     0,   521,
     522,     0,     0,     0,     0,   523,   524,   525,     0,     0,
       0,   526,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   527,
       0,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     538,   539,     0,     0,  1084,     0,     0,     0,     0,   507,
     508,   509,   510,   511,   505,   506,   512,   513,   514,   515,
       0,   516,   517,   518,   519,     0,     0,     0,   520,     0,
     521,   522,     0,     0,     0,     0,   523,   524,   525,     0,
       0,     0,   526,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,     0,     0,  1121,     0,     0,     0,     0,
     507,   508,   509,   510,   511,   505,   506,   512,   513,   514,
     515,     0,   516,   517,   518,   519,     0,     0,     0,   520,
       0,   521,   522,     0,     0,     0,     0,   523,   524,   525,
       0,     0,     0,   526,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   527,     0,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   538,   539,     0,     0,  1122,     0,     0,     0,
       0,   507,   508,   509,   510,   511,   505,   506,   512,   513,
     514,   515,     0,   516,   517,   518,   519,     0,     0,     0,
     520,     0,   521,   522,     0,     0,     0,     0,   523,   524,
     525,     0,     0,     0,   526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,     0,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   539,     0,     0,  1123,     0,     0,
       0,     0,   507,   508,   509,   510,   511,   505,   506,   512,
     513,   514,   515,     0,   516,   517,   518,   519,     0,     0,
       0,   520,     0,   521,   522,     0,     0,     0,     0,   523,
     524,   525,     0,     0,     0,   526,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   527,     0,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   538,   539,     0,     0,  1132,     0,
       0,     0,     0,   507,   508,   509,   510,   511,   505,   506,
     512,   513,   514,   515,     0,   516,   517,   518,   519,     0,
       0,     0,   520,     0,   521,   522,     0,     0,     0,     0,
     523,   524,   525,     0,     0,     0,   526,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,     0,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   538,   539,     0,     0,  1134,
       0,     0,     0,     0,   507,   508,   509,   510,   511,   505,
     506,   512,   513,   514,   515,     0,   516,   517,   518,   519,
       0,     0,     0,   520,     0,   521,   522,     0,     0,     0,
       0,   523,   524,   525,     0,     0,     0,   526,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   527,     0,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   538,   539,     0,     0,
    1169,     0,     0,     0,     0,   507,   508,   509,   510,   511,
     505,   506,   512,   513,   514,   515,     0,   516,   517,   518,
     519,     0,     0,     0,   520,     0,   521,   522,     0,     0,
       0,     0,   523,   524,   525,     0,     0,     0,   526,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,     0,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   538,   539,   543,
       0,     0,     0,     0,     0,     0,   507,   508,   509,   510,
     511,   505,   506,   512,   513,   514,   515,     0,   516,   517,
     518,   519,     0,     0,     0,   520,     0,   521,   522,     0,
       0,     0,     0,   523,   524,   525,     0,     0,     0,   526,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   527,     0,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   538,   539,
     888,     0,     0,     0,     0,     0,     0,   507,   508,   509,
     510,   511,   505,   506,   512,   513,   514,   515,     0,   516,
     517,   518,   519,     0,     0,     0,   520,     0,   521,   522,
       0,     0,     0,     0,   523,   524,   525,     0,     0,     0,
     526,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,     0,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   538,
     539,   902,     0,     0,     0,     0,     0,     0,   507,   508,
     509,   510,   511,   505,   506,   512,   513,   514,   515,     0,
     516,   517,   518,   519,     0,     0,     0,   520,     0,   521,
     522,     0,     0,     0,     0,   523,   524,   525,     0,     0,
       0,   526,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   527,
       0,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     538,   539,  1016,     0,     0,     0,     0,     0,     0,   507,
     508,   509,   510,   511,     0,     0,   512,   513,   514,   515,
       0,   516,   517,   518,   519,   505,   506,     0,   520,     0,
     521,   522,     0,     0,     0,     0,   523,   524,   525,     0,
       0,     0,   526,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,  1019,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   780,   781,   782,   783,   784,   785,   786,
     787,   507,   508,   509,   510,   511,   788,   789,   512,   513,
     514,   515,   915,   516,   517,   518,   519,  -288,   258,   259,
     520,   791,   521,   522,   792,   793,     0,     0,   523,   524,
     525,   794,   795,   796,   526,   260,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   916,   527,     0,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   539,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   505,   506,   279,   280,   281,     0,
       0,     0,     0,     0,   282,   283,   284,   285,   286,     0,
       0,   287,   288,   289,   290,   291,   292,   293,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   294,     0,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,     0,     0,   305,
     306,     0,     0,     0,   689,     0,   307,   308,     0,     0,
       0,   505,   506,     0,     0,     0,     0,     0,     0,     0,
     507,   508,   509,   510,   511,     0,     0,   512,   513,   514,
     515,     0,   516,   517,   518,   519,     0,     0,     0,   520,
       0,   521,   522,     0,     0,   666,     0,   523,   524,   525,
     145,   146,   147,   526,   149,   150,   151,   152,   153,   359,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     165,   166,   167,     0,     0,   170,   171,   172,   173,     0,
       0,   527,     0,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,     0,     0,   505,   506,   507,   508,   509,
     510,   511,   538,   539,   512,   513,   514,   515,     0,   516,
     517,   518,   519,     0,     0,     0,   520,     0,   521,   522,
       0,     0,   849,     0,   523,   524,   525,     0,     0,     0,
     526,     0,     0,   690,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   691,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,     0,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
       0,     0,   505,   506,     0,     0,     0,     0,     0,   538,
     539,   507,   508,   509,   510,   511,     0,     0,   512,   513,
     514,   515,     0,   516,   517,   518,   519,     0,     0,     0,
     520,     0,   521,   522,     0,     0,     0,     0,   523,   524,
     525,     0,     0,     0,   526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,   893,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,     0,     0,   505,   506,   507,   508,
     509,   510,   511,   538,   539,   512,   513,   514,   515,     0,
     516,   517,   518,   519,     0,     0,     0,   520,     0,   521,
     522,     0,     0,     0,     0,   523,   524,   525,     0,     0,
       0,   526,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   990,     0,     0,     0,   527,
       0,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,     0,     0,   505,   506,     0,     0,     0,     0,     0,
     538,   539,   507,   508,   509,   510,   511,     0,     0,   512,
     513,   514,   515,     0,   516,   517,   518,   519,     0,     0,
       0,   520,     0,   521,   522,     0,     0,     0,     0,   523,
     524,   525,     0,     0,     0,   526,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   527,     0,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,     0,     0,   505,   506,   507,
     508,   509,   510,   511,   538,   539,   512,   513,   514,   515,
       0,   516,   517,   518,   519,     0,     0,     0,   520,     0,
     521,   522,     0,     0,     0,     0,   523,   524,   525,     0,
     505,   506,  -617,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,     0,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   538,   539,   507,   508,   509,   510,   511,     0,     0,
     512,   513,   514,   515,     0,   516,   517,   518,   519,     0,
       0,     0,   520,     0,   521,   522,     0,   505,   506,     0,
     523,   524,   525,     0,     0,     0,   507,   508,   509,   510,
     511,     0,     0,   512,   513,   514,   515,     0,   516,   517,
     518,   519,     0,     0,     0,   520,     0,   521,   522,   505,
     506,     0,     0,   523,   527,   525,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   538,   539,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,     0,
       0,     0,     0,   507,   508,   509,   510,   511,   538,   539,
     512,   513,   514,   515,     0,   516,   517,   518,   519,     0,
       0,     0,   520,     0,   521,   522,   505,   506,     0,     0,
     523,     0,     0,     0,     0,   507,   508,   509,   510,   511,
       0,     0,   512,   513,   514,   515,     0,   516,   517,   518,
     519,   505,   506,     0,   520,     0,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   538,   539,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,     0,     0,
       0,     0,   507,   508,   509,   510,   511,   538,   539,   512,
     513,   514,   515,     0,   516,   517,   518,   519,   505,   506,
       0,   520,     0,   521,   522,     0,     0,   507,   508,   509,
     510,   511,     0,     0,   512,   513,   514,   515,     0,   516,
     517,   518,   519,     0,     0,     0,   520,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   529,   530,   531,   532,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   538,   539,     0,     0,     0,     0,
       0,     0,   530,   531,   532,   533,   534,   535,   536,   537,
       0,     0,     0,     0,   507,   508,   509,   510,   511,   538,
     539,   512,   513,   514,   515,     0,   516,   517,   518,   519,
       0,     0,     0,   520,     0,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   141,     0,     0,     0,     0,     0,
     142,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     531,   532,   533,   534,   535,   536,   537,   143,     0,     0,
       0,     0,     0,     0,     0,     0,   538,   539,     0,     0,
       0,     0,   144,     0,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   488,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   141,     0,
       0,     0,     0,     0,   142,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
       0,   143,     0,   889,     0,   176,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   144,    51,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   359,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,   141,     0,     0,     0,     0,     0,   142,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   143,     0,     0,     0,
       0,     0,   890,     0,     0,     0,     0,     0,     0,     0,
       0,   144,   891,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   141,     0,     0,
       0,     0,     0,   142,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,     0,
     143,     0,     0,     0,   176,     0,     0,   369,   370,   371,
       0,     0,     0,     0,     0,   144,    51,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   141,     0,     0,     0,     0,     0,   142,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,   143,     0,     0,     0,   176,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   144,
      51,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   176,     0,     0,   190,     0,   191,     0,   192,
     193,   194,   195,   196,   462,   197,   198,   199,   200,   201,
     202,   203,   204,   205,     0,   206,   207,   208,     0,     0,
     209,   210,   211,   212,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   213,
     214,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215
};

static const yytype_int16 yycheck[] =
{
      12,   253,   217,   128,   434,   111,   548,   457,   340,   459,
     342,   461,   344,   552,   541,   489,   561,   573,   574,     0,
     658,   423,     7,   679,   111,     6,   664,   572,   396,    21,
      14,    15,   441,    32,    51,    51,    81,    20,    21,    48,
      19,    19,   486,    32,   488,    19,   131,   122,    29,    44,
      31,   117,    33,    33,   157,    20,    21,   117,    39,    58,
      59,   119,   120,    55,    18,    19,   126,    48,   157,   157,
      82,   616,   132,    54,    14,    15,   119,   120,   169,   144,
     157,    61,   173,   148,   132,   116,    44,    45,   119,   120,
     191,    60,    73,   141,   197,   196,   652,   349,   146,   111,
     166,   178,    97,   169,   147,   193,   166,   196,   635,   157,
     157,   196,   117,    94,   157,   114,   560,   369,   370,   118,
     195,   126,   157,   157,   172,   108,   109,   192,   132,   157,
     188,   189,   141,   116,   166,   118,   119,   120,   121,    97,
     117,   157,   125,   108,   109,   188,   189,   253,   157,   126,
     197,   116,   132,   168,   119,   120,   121,   188,   189,   157,
     125,   166,   197,   197,   168,   151,   171,   151,   167,   197,
      55,   155,   171,   157,   158,   174,    61,   157,   195,   224,
     144,   196,   171,   165,   148,   177,   165,   165,   597,   166,
     189,   165,   191,   191,   446,   851,   188,    41,   743,   198,
     189,   326,   747,   157,   189,   188,   189,   191,   189,   198,
     848,   151,   197,   195,   195,   155,   761,   157,   158,   117,
     165,   165,   778,   188,   189,   477,   165,   333,   334,   335,
     336,    75,   634,   339,    24,   341,   166,   343,   606,   345,
      30,   253,   141,   349,   165,   141,   333,   334,   335,   336,
     195,   195,   339,   192,   341,   169,   343,    47,   157,   173,
     117,   157,   807,   369,   370,   195,   168,   165,   166,   126,
     168,   192,    62,   171,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,   117,    20,    21,  1128,   166,
       4,     5,   165,     7,   126,   147,   422,   844,   169,   165,
     165,   333,   334,   335,   336,   157,   432,   339,   169,   341,
     857,   343,   173,   345,   165,   422,  1156,   349,   976,   192,
     446,    35,   132,   165,   195,   432,    32,   192,   563,   987,
      20,    21,   684,  1114,   166,   145,   571,   369,   370,   886,
     168,   192,   165,   192,   165,   173,   195,   157,   190,   908,
     382,   477,    58,    59,   848,    55,   165,   190,   132,   165,
    1141,    61,   195,   191,   858,   912,   157,   639,   640,   192,
     642,   192,   106,   107,   108,   109,   173,   165,   165,   505,
     506,   190,   116,   157,   118,   119,   120,   121,   198,   157,
     422,   125,   132,   127,   128,   147,   165,   147,   195,   132,
     432,   141,   190,   190,   168,   157,   438,   157,   114,   173,
     168,   881,   118,   157,   446,   173,   173,   157,   108,   109,
     979,   190,   157,   151,   173,   972,   116,   191,   118,   119,
     120,   121,   195,   191,   197,   125,   173,   469,   195,   157,
     174,   175,   176,   177,   178,   477,   195,   188,    11,    68,
     195,   168,   197,    72,   188,   189,   173,    55,   195,    22,
      23,   167,   169,    61,     9,   171,   173,   173,   174,   501,
      89,    90,    91,    92,   191,    20,    21,   169,   169,   159,
     160,   173,   173,   189,   174,   175,   176,   177,   178,    55,
      55,   165,   198,   619,   168,    61,    61,   171,   188,   189,
     166,    55,   159,   160,   630,   169,   632,    61,   191,   173,
     745,   637,   619,   639,   640,   157,   642,   169,   870,   754,
      32,   173,   169,   630,   157,   995,   173,   159,   160,   161,
     637,   159,   160,   161,   162,   770,  1098,   193,   194,   157,
     196,   195,   157,   196,    55,   196,    58,    59,   157,    20,
      21,   195,   148,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,  1027,    97,   196,
     125,   126,   127,   128,   129,   130,   712,   619,   133,   134,
     135,   136,   137,   138,   139,   172,   172,   869,   630,   172,
     632,   172,   114,   191,   172,   637,   118,   639,   640,   191,
     642,   172,   172,   172,   172,   741,   172,   169,   157,    34,
     195,   166,   167,    34,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   741,   157,   191,   108,   109,   196,
     157,    32,    11,   188,   189,   116,   197,   118,   119,   120,
     121,    20,    21,   168,   125,   167,   190,   195,   803,   171,
      21,   173,   174,   190,   168,   196,   168,   196,   191,    41,
     194,   192,   172,   191,   172,   191,   172,   189,   191,   172,
     172,   191,   917,   191,   191,   191,   198,   157,   191,    32,
     191,   157,   191,   190,   189,   173,   148,   191,   157,    41,
     191,   190,   157,   191,   151,   176,   177,   178,   195,   741,
      36,   195,   195,   195,     9,    58,    59,   188,   189,   195,
     195,   195,   242,   195,    41,   157,   165,   173,   173,   173,
     190,   251,   165,  1005,   197,   159,    32,   106,   107,   108,
     109,   110,   191,   869,   113,   114,   115,   116,   191,   118,
     119,   120,   121,   157,   172,   191,   125,   191,   127,   128,
     172,   191,    58,    59,   133,   134,   135,     1,   800,  1004,
     139,   114,   191,   157,   117,   118,   157,   191,   122,   166,
      13,   151,   168,   126,   194,     7,   157,    32,   196,   157,
      20,    21,   191,  1028,   157,   190,   190,   190,   167,   197,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     190,   159,   157,    58,    59,   172,   157,   157,   114,   188,
     189,   157,   118,   166,   167,   191,   191,   191,   171,    41,
     190,   174,   191,   195,   190,   190,   157,   869,   192,   190,
     190,   361,   362,   195,   157,   173,   189,    32,   197,   195,
     173,   371,   195,  1088,    51,   198,   190,  1092,   190,   379,
     380,   381,   197,   383,   384,   385,   190,   493,   190,   114,
     195,   167,   190,   118,   190,   171,   106,   107,   174,  1005,
     110,  1007,    63,   113,   114,   115,   116,    37,   118,   119,
     120,   121,   108,   189,    32,   125,    66,   127,   128,  1125,
    1007,  1126,   198,   133,   134,   135,    64,    82,   800,   139,
     229,  1126,   926,  1126,   434,  1126,  1126,     1,  1002,  1041,
      58,    59,   167,   443,   837,  1044,   171,   554,   173,   174,
     632,   612,    45,  1168,  1112,   338,   349,   167,   667,   169,
     170,   171,   172,   173,   189,   346,   856,   177,   489,   435,
      -1,   471,    -1,   198,   474,  1081,   476,    -1,   188,   189,
     480,   481,   482,   483,   484,   485,    -1,   487,    -1,   489,
      -1,    -1,    -1,  1005,  1081,  1007,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,    -1,
      -1,   511,   512,   513,   514,    -1,   516,    -1,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,    -1,   539,
      -1,    -1,    -1,    -1,    -1,    20,    21,   547,    -1,   167,
      -1,    -1,    -1,   171,    -1,   173,   174,   557,    -1,    -1,
      -1,   561,    -1,    -1,   564,   565,    -1,    -1,    -1,  1081,
     570,   189,   572,    -1,    -1,    -1,    -1,    -1,    -1,   579,
     198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,  1114,   603,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   616,    58,    59,    -1,
      -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,  1141,
      -1,   106,   107,   108,   109,   110,   636,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,   654,   655,    -1,    -1,   133,   134,
     135,    -1,    -1,   663,   139,    -1,   666,   667,    -1,    -1,
     670,    -1,    -1,   114,    -1,    -1,    -1,   118,   114,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,
      -1,   196,    -1,    -1,    -1,   725,   167,    -1,    -1,   729,
     171,   167,   173,   174,    -1,   171,   736,   173,   174,    -1,
      -1,    -1,    -1,   743,    -1,    -1,    -1,   747,   189,    -1,
      -1,    -1,   752,   189,    -1,    -1,    -1,   198,    -1,    -1,
      -1,   761,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,   797,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   807,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   846,   847,    -1,   849,
      -1,    -1,    -1,    -1,   854,    -1,   856,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   867,   868,    -1,
      -1,    -1,    -1,   873,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   883,    -1,   885,    -1,   887,    -1,    -1,
      -1,    -1,    -1,   893,    -1,    -1,    -1,   897,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,   915,   916,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,   928,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
     940,   941,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   949,
     950,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,   189,    -1,    -1,    64,    65,    66,   195,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,  1013,    -1,  1015,    -1,    -1,  1018,    -1,
      -1,    -1,    -1,  1023,    -1,    -1,  1026,  1027,    -1,    -1,
      -1,  1031,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1047,    -1,    -1,
    1050,  1051,  1052,  1053,    -1,    -1,  1056,    -1,    -1,  1059,
    1060,    -1,  1062,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,  1072,    -1,    -1,    -1,    -1,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,    -1,     6,
      -1,     8,     9,    10,    -1,    12,  1116,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,  1126,    -1,    25,    26,
      27,    28,    -1,  1133,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,  1147,  1148,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,  1158,    56,
      57,    -1,    -1,  1163,    -1,  1165,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
      -1,   178,    -1,    -1,   181,   182,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,    -1,   193,   194,   195,   196,
     197,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    -1,    42,    43,    -1,    -1,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   175,   176,    -1,   178,    -1,
      -1,   181,   182,    -1,    -1,    -1,    14,    15,    16,    17,
      18,   191,    -1,   193,   194,   195,   196,   197,    26,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    51,    -1,    53,    -1,    -1,    -1,    -1,
      58,    59,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,   171,    -1,    -1,   174,   175,   176,    -1,
     178,    -1,    -1,   181,   182,    14,    15,    16,    17,    18,
      -1,   189,    -1,   191,    -1,   193,   194,    26,   196,    -1,
     198,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    58,
      59,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
      -1,    -1,   171,    -1,    -1,   174,   175,   176,    -1,   178,
      -1,    -1,   181,   182,    14,    15,    16,    17,    18,    -1,
     189,    -1,   191,    -1,   193,   194,    26,   196,    -1,   198,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
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
      -1,   171,    -1,    -1,   174,   175,   176,    -1,   178,    -1,
      -1,   181,   182,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,   191,    -1,   193,   194,    -1,   196,    -1,   198,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
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
      18,    -1,   189,    -1,   191,   192,   193,   194,    26,   196,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,
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
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,
     182,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   191,
      46,   193,   194,    49,   196,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   174,   175,
     176,    -1,   178,    -1,    -1,   181,   182,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   191,    46,   193,   194,    49,
     196,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
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
      14,    15,    16,    17,    18,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    26,    -1,   174,   175,   176,    -1,   178,    -1,
      -1,   181,   182,    -1,    38,    -1,    -1,    -1,    -1,    43,
     190,   191,    46,   193,   194,    49,   196,    -1,    -1,    53,
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
      59,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   174,   175,   176,    -1,   178,
      -1,    -1,   181,   182,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   191,    46,   193,   194,    49,   196,    -1,    -1,
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
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
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
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,
     182,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   191,
      46,   193,   194,    49,   196,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,
     176,    -1,   178,    -1,    -1,   181,   182,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   191,    -1,   193,   194,    -1,
     196,    98,    99,   100,   101,   102,   103,   104,   105,   106,
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
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,   188,   189,   106,   107,   108,
     109,   110,   195,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    20,    21,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,   188,
     189,   113,    -1,   192,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
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
     107,   108,   109,   110,    -1,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    20,    21,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    20,    21,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,   137,   138,   139,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    20,    21,   118,   119,   120,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,   181,
     182,    -1,    -1,    -1,    18,    -1,   188,   189,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    -1,    -1,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,   131,    -1,   133,   134,   135,
      64,    65,    66,   139,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,   167,    -1,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,    20,    21,   106,   107,   108,
     109,   110,   188,   189,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,   131,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   188,
     189,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    20,    21,   106,   107,
     108,   109,   110,   188,   189,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,   167,
      -1,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
     188,   189,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    20,    21,   106,
     107,   108,   109,   110,   188,   189,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      20,    21,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,   106,   107,   108,   109,   110,    -1,    -1,
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
     121,    20,    21,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   188,   189,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    20,    21,
      -1,   125,    -1,   127,   128,    -1,    -1,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,
      -1,    -1,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,   188,
     189,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,   173,   174,   175,   176,   177,   178,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,    -1,    -1,
      -1,    -1,    62,    -1,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    18,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,   157,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,   157,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    -1,   145,    -1,    -1,   148,   149,   150,
      -1,    -1,    -1,    -1,    -1,    62,   157,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
     157,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    70,    71,    72,   157,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   200,     0,     6,    29,    31,    33,    39,    48,    54,
      73,    94,   189,   195,   203,   211,   212,   216,   243,   247,
     266,   333,   339,   342,   348,   387,   390,    18,    19,   157,
     237,   238,   239,   151,   217,   218,   157,   178,   213,   214,
     157,   196,   336,   157,   193,   202,   391,   388,    33,    61,
     132,   157,   240,   241,   242,   256,     4,     5,     7,    35,
     346,    60,   331,   166,   165,   168,   165,   213,    21,    55,
     177,   188,   215,   337,   336,   338,   331,   157,   157,   157,
     132,   191,   165,   190,    55,    61,   248,   250,    55,    61,
     340,    55,    61,   347,    55,    61,   332,    14,    15,   151,
     155,   157,   158,   191,   205,   238,   151,   218,   157,   157,
     157,   166,   195,   197,   336,    55,    61,   201,   196,   389,
     157,   239,   241,   251,   157,   341,   349,   196,   334,   159,
     160,   204,    14,    15,   151,   155,   157,   205,   235,   236,
     215,    24,    30,    47,    62,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   145,   256,   352,   354,
     355,   357,   361,   386,   195,   157,   356,   196,   192,    34,
      64,    66,    68,    69,    70,    71,    72,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    84,    85,    86,    89,
      90,    91,    92,   108,   109,   157,   246,   249,   168,   196,
      97,   344,   345,   329,   148,   308,   159,   160,   161,   165,
     192,   172,   172,   172,   172,   191,   172,   172,   172,   172,
     172,   172,   191,    32,    58,    59,   114,   118,   167,   171,
     174,   189,   198,   169,   195,   157,   321,   322,    20,    21,
      37,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   118,
     119,   120,   126,   127,   128,   129,   130,   133,   134,   135,
     136,   137,   138,   139,   167,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   181,   182,   188,   189,    34,
      34,   191,   244,   196,   252,    68,    72,    89,    90,    91,
      92,   353,   335,   157,   350,   197,   330,   239,   141,   157,
     327,   328,   235,   364,   366,   368,   362,   157,   358,   370,
     372,   374,   376,   378,   380,   382,   384,    14,    15,    16,
      17,    18,    26,    38,    43,    46,    49,    53,    63,    73,
      95,   108,   109,   140,   141,   142,   143,   144,   146,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   158,   174,
     175,   176,   178,   181,   182,   191,   193,   194,   207,   209,
     252,   256,   258,   268,   275,   278,   281,   285,   286,   289,
     290,   292,   297,   300,   307,   352,   392,   396,   400,   402,
     404,    32,    32,   171,   189,   198,   190,   300,   198,   361,
     157,   197,   168,   195,   190,    21,   190,   144,   192,   308,
     317,   318,   168,   245,   254,   196,   157,   197,   168,   343,
     196,   308,   190,   191,    41,   165,   168,   171,   326,   386,
     360,   386,   386,   386,   192,   356,   386,   244,   386,   244,
     386,   244,   157,   319,   320,   386,   322,   361,   392,   191,
     172,   191,   172,   172,   191,   172,   191,   172,   300,   300,
     191,   191,   191,   191,   191,   191,    12,   361,    12,   361,
     300,   398,   401,   206,   300,   300,   300,   256,   300,   300,
     300,   194,   157,   191,   243,    20,    21,   106,   107,   108,
     109,   110,   113,   114,   115,   116,   118,   119,   120,   121,
     125,   127,   128,   133,   134,   135,   139,   167,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   188,   189,
     192,   191,   190,   190,   386,   197,   321,   191,    48,   141,
     157,   324,   351,   192,   195,   386,     1,     8,     9,    10,
      12,    25,    27,    28,    37,    40,    42,    50,    52,    56,
      57,    63,    96,   123,   124,   195,   197,   219,   220,   223,
     226,   227,   229,   230,   231,   232,   233,   253,   255,   257,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   287,
     288,   300,   335,   166,   195,   256,   313,   328,   190,   300,
     157,   157,   361,   117,   126,   166,   325,   173,   173,   195,
     173,   173,   173,   195,   173,   245,   173,   245,   173,   245,
     168,   173,   195,   173,   195,   191,   256,   276,   300,   269,
     271,   300,   273,   300,   361,   300,   300,   300,   300,   300,
     300,   351,    51,   157,   174,   191,   300,   393,   394,   395,
     397,   399,   351,   191,   394,   399,   131,   195,   197,   161,
     162,   204,   210,   192,   141,   146,   172,   256,   291,   189,
     192,   279,   300,   148,   284,    18,   147,   157,   352,    18,
     147,   157,   352,   300,   300,   300,   300,   300,   300,   157,
     300,   147,   157,   300,   300,   300,   300,   300,   300,   300,
     300,   300,    21,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   119,   120,   147,   157,   188,   189,
     298,   300,   192,   279,   300,   324,   191,    41,   117,   165,
     166,   168,   171,   323,   317,   300,   351,   117,   255,   287,
     300,   252,    59,   300,   300,   157,   195,   151,    56,   300,
     252,   117,   255,   300,   194,   285,   285,    36,   195,   195,
     300,     9,   195,   195,   195,   195,   195,   328,   122,   195,
      98,    99,   100,   101,   102,   103,   104,   105,   111,   112,
     117,   126,   129,   130,   136,   137,   138,   166,   197,   300,
     189,   197,   243,   314,   192,    41,   195,   325,   255,   300,
     365,   367,   386,   369,   363,   359,   371,   173,   375,   173,
     379,   173,   386,   383,   319,   385,   192,   279,   172,   300,
     386,   192,   361,   361,   192,   361,   192,   173,   192,   192,
     192,   192,   192,   192,    19,   285,   126,   323,   192,   131,
     165,   195,   395,   190,   165,   190,   195,    19,   192,   395,
     197,   300,   398,   197,   300,   159,   208,   191,   191,   293,
     295,   157,   393,   165,   192,   117,   126,   166,   171,   282,
     283,   244,   172,   191,   172,   191,   191,   191,   190,    18,
     147,   157,   352,   168,   147,   157,   300,   191,   147,   157,
     300,     1,   190,   192,   192,   300,   157,   157,   386,   255,
     300,   252,    19,   255,   300,   117,   166,    13,   300,   252,
     166,   168,   151,   255,   300,   194,   196,   252,   228,   285,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     123,   124,   300,   300,   300,   300,   300,   300,   300,   123,
     124,   300,   195,   242,     7,   308,   312,   157,   255,   300,
     195,   373,   377,   381,   192,   157,   192,   173,   173,   173,
     173,   284,   191,   279,   300,   300,   394,   395,   300,   157,
     393,   190,   190,   300,   190,   397,   279,   394,   197,   190,
     163,   159,   300,   300,   361,   244,   190,   300,   157,   157,
     157,   157,   165,   190,   245,   301,   300,   303,   300,   192,
     279,   300,   172,   191,   300,   191,   190,   300,   191,   190,
     299,   192,    41,   323,   279,   252,   234,   254,    11,    22,
      23,   221,   222,   300,   300,   300,   300,   300,   190,    55,
      61,   311,    44,    97,   309,   195,   195,   173,   195,   277,
     270,   272,   274,   191,   192,   279,   195,   395,   190,   126,
     323,   190,   195,   395,   190,   192,   192,   173,   245,   192,
     282,   190,   131,   252,   280,   361,   192,   386,   192,   192,
     192,   305,   300,   300,   192,   300,   157,   300,   195,   300,
     197,   252,   300,    11,   224,   195,    45,   309,   311,   300,
     157,   300,   300,   300,   192,   300,   192,   300,   190,   190,
     300,   300,   300,   197,   294,   173,   117,   300,   173,   173,
     386,   192,   192,   192,   252,   252,   225,   195,   315,    32,
     310,   324,   192,   173,   192,   195,   403,   190,   403,   190,
     291,   296,   300,   302,   304,   173,   222,    25,    96,   226,
     259,   260,   261,   263,   300,   249,   316,   300,    51,   190,
     197,   291,   306,   117,   300,   117,   300,   195,   249,   192,
     300,   190,   190,   300,   300,   252
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
     225,   224,   226,   226,   226,   226,   226,   227,   228,   227,
     229,   230,   231,   232,   234,   233,   235,   235,   235,   235,
     235,   235,   236,   236,   237,   237,   237,   238,   238,   238,
     238,   238,   238,   238,   238,   239,   239,   240,   240,   240,
     241,   241,   242,   242,   243,   243,   244,   244,   244,   245,
     245,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   247,   248,
     248,   248,   249,   251,   250,   252,   252,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   254,   254,   254,
     255,   255,   255,   256,   256,   256,   257,   257,   258,   258,
     258,   258,   259,   260,   261,   261,   261,   262,   262,   262,
     263,   263,   264,   264,   264,   265,   266,   266,   267,   269,
     270,   268,   271,   272,   268,   273,   274,   268,   276,   277,
     275,   278,   278,   278,   279,   279,   280,   280,   280,   281,
     281,   281,   282,   282,   282,   282,   283,   283,   284,   284,
     285,   285,   286,   286,   286,   286,   286,   286,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   288,   288,   288,
     288,   289,   290,   290,   291,   291,   292,   293,   294,   292,
     295,   296,   292,   297,   297,   298,   299,   297,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   301,   302,   300,
     300,   300,   300,   303,   304,   300,   300,   300,   305,   306,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   308,   308,
     309,   309,   309,   310,   310,   311,   311,   311,   312,   313,
     314,   313,   315,   313,   316,   313,   313,   317,   317,   317,
     318,   318,   319,   319,   320,   320,   321,   322,   322,   323,
     323,   324,   324,   324,   324,   324,   324,   325,   325,   325,
     326,   326,   327,   327,   327,   327,   327,   328,   328,   328,
     328,   328,   329,   330,   329,   331,   331,   332,   332,   332,
     333,   334,   333,   335,   335,   335,   337,   336,   338,   338,
     339,   339,   340,   340,   340,   341,   342,   342,   343,   343,
     344,   344,   345,   346,   346,   347,   347,   347,   349,   350,
     348,   351,   351,   351,   351,   351,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   353,   353,   353,   353,   353,   353,   354,   355,   355,
     355,   356,   356,   358,   359,   357,   360,   360,   361,   361,
     361,   361,   361,   361,   361,   361,   361,   361,   361,   361,
     361,   361,   361,   361,   361,   362,   363,   361,   361,   364,
     365,   361,   366,   367,   361,   368,   369,   361,   361,   370,
     371,   361,   372,   373,   361,   361,   374,   375,   361,   376,
     377,   361,   361,   378,   379,   361,   380,   381,   361,   382,
     383,   361,   384,   385,   361,   386,   386,   386,   388,   389,
     387,   391,   390,   392,   392,   392,   392,   393,   393,   393,
     393,   394,   394,   395,   395,   396,   396,   396,   396,   396,
     396,   397,   397,   397,   398,   398,   399,   399,   400,   400,
     401,   401,   402,   403,   403,   404,   404
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     0,     1,     1,     1,     1,
       4,     1,     1,     2,     2,     3,     0,     2,     4,     3,
       1,     2,     0,     4,     2,     2,     1,     2,     3,     3,
       2,     4,     0,     1,     2,     1,     3,     1,     3,     3,
       3,     2,     1,     1,     0,     2,     4,     1,     1,     0,
       0,     3,     1,     1,     1,     1,     1,     4,     0,     6,
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
       3,     2,     2,     1,     3,     2,     2,     3,     2,     4,
       5,     2,     1,     1,     1,     2,     3,     2,     2,     3,
       2,     3,     2,     2,     3,     4,     1,     1,     2,     0,
       0,     7,     0,     0,     7,     0,     0,     7,     0,     0,
       6,     5,     8,    10,     1,     3,     1,     2,     3,     1,
       1,     2,     2,     2,     2,     2,     1,     3,     0,     4,
       1,     6,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     4,
       4,     6,     5,     6,     1,     4,     3,     0,     0,     8,
       0,     0,     9,     3,     4,     0,     0,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     3,     4,
       5,     4,     5,     3,     4,     1,     3,     4,     3,     4,
       2,     4,     4,     7,     8,     3,     5,     0,     0,     8,
       3,     3,     3,     0,     0,     8,     3,     4,     0,     0,
       9,     4,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     2,     4,     4,     4,     4,     4,     4,     6,     7,
       6,     6,     7,     7,     6,     7,     6,     6,     0,     4,
       0,     1,     1,     0,     1,     0,     1,     1,     4,     0,
       0,     4,     0,     8,     0,     9,     5,     2,     3,     4,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       1,     1,     2,     3,     5,     3,     3,     1,     1,     1,
       0,     1,     1,     4,     3,     3,     5,     4,     6,     5,
       5,     4,     0,     0,     4,     0,     1,     0,     1,     1,
       6,     0,     6,     0,     3,     5,     0,     4,     2,     3,
       4,     2,     0,     1,     1,     1,     7,     9,     0,     2,
       0,     1,     3,     1,     1,     0,     1,     1,     0,     0,
       9,     1,     4,     3,     3,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     1,     3,     0,     0,     6,     1,     3,     1,     1,
       1,     1,     4,     3,     4,     2,     2,     3,     2,     3,
       2,     2,     3,     3,     2,     0,     0,     6,     2,     0,
       0,     6,     0,     0,     6,     0,     0,     6,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     0,
       0,     6,     0,     0,     6,     1,     3,     3,     0,     0,
       8,     0,     7,     1,     1,     1,     1,     3,     3,     5,
       5,     1,     3,     0,     2,     6,     5,     7,     8,     6,
       8,     1,     3,     3,     3,     1,     1,     3,     5,     5,
       1,     3,     4,     0,     3,    10,    10
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

    case YYSYMBOL_expression_else_one_liner: /* expression_else_one_liner  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_if_one_liner: /* expression_if_one_liner  */
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

    case YYSYMBOL_expression_return_no_pipe: /* expression_return_no_pipe  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_return: /* expression_return  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_yield_no_pipe: /* expression_yield_no_pipe  */
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

    case YYSYMBOL_type_declaration_no_options: /* type_declaration_no_options  */
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

  case 59: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 60: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 61: /* expression_else_one_liner: "else" $@2 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 62: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 63: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 64: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 65: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 66: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 67: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 68: /* $@3: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 69: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner ';'  */
                                                                                                                                                   {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 70: /* expression_for_loop: "for" variable_name_with_pos_list "in" expr_list ';' expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 71: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 72: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 73: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 74: /* $@4: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 75: /* expression_with_alias: "assume" "name" '=' $@4 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 76: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 77: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 78: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 79: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 80: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 81: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 82: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 83: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 84: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 85: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 86: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 87: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 88: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 89: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 90: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 91: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 92: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 93: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 94: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 95: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 96: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 97: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 98: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 99: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 100: /* annotation_declaration: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
    break;

  case 101: /* annotation_declaration: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 102: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 103: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 104: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 105: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 106: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 107: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 108: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 109: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 110: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 111: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 112: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 113: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 114: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 115: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 116: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 117: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 118: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 119: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 120: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 121: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 122: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 123: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 124: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 125: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 126: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 127: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 128: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 129: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 130: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 131: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 132: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 133: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 134: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 135: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 136: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 137: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 138: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 139: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 140: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 141: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 142: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 143: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 144: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 145: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 146: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 147: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 148: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 149: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 150: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 151: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 152: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 153: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 154: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 155: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 156: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 157: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 158: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 159: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 160: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 161: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 162: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 163: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 164: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 165: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 166: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 167: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 168: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 169: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 170: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 171: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 172: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 173: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 174: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 175: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 176: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 177: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 178: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 179: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 180: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 181: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 182: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 183: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 184: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 185: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 186: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 187: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 188: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 189: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 190: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 191: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 192: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 193: /* $@5: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 194: /* function_declaration: optional_public_or_private_function $@5 function_declaration_header expression_block  */
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

  case 195: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 196: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 197: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 198: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 199: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 200: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 201: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 202: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 203: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 204: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 205: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 206: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 207: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 208: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 209: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 210: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 211: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 212: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 213: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 214: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 215: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 216: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 217: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 218: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 219: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 220: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 221: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 222: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 223: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 224: /* name_in_namespace: "name" "::" "name"  */
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

  case 225: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 226: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 227: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 228: /* expr_new: "new" type_declaration_no_options  */
                                                              {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 229: /* expr_new: "new" type_declaration_no_options '(' ')'  */
                                                                      {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 230: /* expr_new: "new" type_declaration_no_options '(' expr_list ')'  */
                                                                                           {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 231: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 232: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 233: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 234: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 235: /* expression_return_no_pipe: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 236: /* expression_return_no_pipe: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 237: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 238: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 239: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 240: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 241: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 242: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 243: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 244: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 245: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 246: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 247: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 248: /* expression_let: kwd_let let_variable_declaration  */
                                               {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 249: /* $@6: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 250: /* $@7: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 251: /* expr_cast: "cast" '<' $@6 type_declaration_no_options '>' $@7 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 252: /* $@8: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 253: /* $@9: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 254: /* expr_cast: "upcast" '<' $@8 type_declaration_no_options '>' $@9 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 255: /* $@10: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 256: /* $@11: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 257: /* expr_cast: "reinterpret" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 258: /* $@12: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 259: /* $@13: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 260: /* expr_type_decl: "type" '<' $@12 type_declaration '>' $@13  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 261: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 262: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 263: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 264: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 265: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 266: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 267: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 268: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 269: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 270: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 271: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 272: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 273: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 274: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 275: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 276: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 277: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 278: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 279: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 280: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 281: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 282: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 283: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 284: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 285: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 286: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 287: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 288: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 289: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 290: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 291: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 292: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 293: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 294: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 295: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 296: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 297: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 298: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 299: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 300: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 301: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 302: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 303: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 304: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 305: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 306: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 307: /* expr_assign_pipe: expr '=' "@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 308: /* expr_assign_pipe: expr '=' "@@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 309: /* expr_assign_pipe: expr "<-" "@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 310: /* expr_assign_pipe: expr "<-" "@@ <|" expr  */
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 311: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 312: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 313: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 314: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 315: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 316: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 317: /* $@14: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 318: /* $@15: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 319: /* func_addr_expr: '@' '@' '<' $@14 type_declaration_no_options '>' $@15 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 320: /* $@16: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 321: /* $@17: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 322: /* func_addr_expr: '@' '@' '<' $@16 optional_function_argument_list optional_function_type '>' $@17 func_addr_name  */
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

  case 323: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 324: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 325: /* $@18: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 326: /* $@19: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 327: /* expr_field: expr '.' $@18 error $@19  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 328: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 329: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 330: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 331: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 332: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 333: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 334: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 335: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 336: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 337: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 338: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 344: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 345: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 364: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 367: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 368: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 369: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 370: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 371: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 372: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 373: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 374: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 375: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 376: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 377: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 378: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 379: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 380: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 382: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 383: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 384: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 385: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 387: /* $@20: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 388: /* $@21: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 389: /* expr: expr "is" "type" '<' $@20 type_declaration_no_options '>' $@21  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 390: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 391: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 392: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 393: /* $@22: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 394: /* $@23: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 395: /* expr: expr "as" "type" '<' $@22 type_declaration '>' $@23  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 396: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 397: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 398: /* $@24: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 399: /* $@25: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 400: /* expr: expr '?' "as" "type" '<' $@24 type_declaration '>' $@25  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 401: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 402: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 403: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 404: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 405: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 406: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 407: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 408: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 409: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 410: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) =ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 411: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 412: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 413: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 414: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 415: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 416: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 417: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 418: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 419: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 420: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 421: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 422: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 423: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 424: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 425: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 426: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 427: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 428: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 429: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 430: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 431: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 432: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 433: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 434: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 435: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 436: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 437: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 438: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 439: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 440: /* $@26: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 441: /* struct_variable_declaration_list: struct_variable_declaration_list $@26 structure_variable_declaration ';'  */
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

  case 442: /* $@27: %empty  */
                                                                                             {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 443: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@27 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
            }
    break;

  case 444: /* $@28: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 445: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@28 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 446: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 447: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 448: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 449: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 450: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 451: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 452: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 453: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 454: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 455: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 456: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 457: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 458: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 459: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 460: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 461: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 462: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 463: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 464: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 465: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 466: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 467: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 468: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 469: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 470: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 471: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 472: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 473: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 474: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 475: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 476: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 477: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 478: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 479: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 480: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 481: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 482: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 483: /* $@29: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 484: /* global_variable_declaration_list: global_variable_declaration_list $@29 optional_field_annotation let_variable_declaration  */
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

  case 485: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 486: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 487: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 488: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 489: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 490: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 491: /* $@30: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 492: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@30 optional_field_annotation let_variable_declaration  */
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

  case 493: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 494: /* enum_list: enum_list "name" ';'  */
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

  case 495: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 496: /* $@31: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 497: /* single_alias: "name" $@31 '=' type_declaration  */
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

  case 502: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 503: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 504: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 505: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 506: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 507: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 508: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 509: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 510: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 511: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 512: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 513: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 514: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 515: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 516: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 517: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 518: /* $@32: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 519: /* $@33: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 520: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@32 structure_name $@33 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
        }
    }
    break;

  case 521: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 522: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 523: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 524: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 525: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 526: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 527: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 528: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 529: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 530: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 531: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 532: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 533: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 534: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 535: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 536: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 537: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 538: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 539: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 540: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 541: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 542: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 543: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 544: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 545: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 546: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 547: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 548: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 549: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 550: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 551: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 552: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 553: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 554: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 555: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 556: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 557: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 558: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 559: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 560: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 561: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 562: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 563: /* $@34: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 564: /* $@35: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 565: /* bitfield_type_declaration: "bitfield" '<' $@34 bitfield_bits '>' $@35  */
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

  case 566: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 567: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 568: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 569: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 570: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 571: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 572: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
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

  case 573: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 574: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 575: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 576: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 577: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 578: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 579: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 580: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 581: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 582: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 583: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 584: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 585: /* $@36: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 586: /* $@37: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 587: /* type_declaration_no_options: "smart_ptr" '<' $@36 type_declaration '>' $@37  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 588: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 589: /* $@38: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 590: /* $@39: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 591: /* type_declaration_no_options: "array" '<' $@38 type_declaration '>' $@39  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 592: /* $@40: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 593: /* $@41: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 594: /* type_declaration_no_options: "table" '<' $@40 table_type_pair '>' $@41  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 595: /* $@42: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 596: /* $@43: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 597: /* type_declaration_no_options: "iterator" '<' $@42 type_declaration '>' $@43  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 598: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 599: /* $@44: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 600: /* $@45: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 601: /* type_declaration_no_options: "block" '<' $@44 type_declaration '>' $@45  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 602: /* $@46: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 603: /* $@47: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 604: /* type_declaration_no_options: "block" '<' $@46 optional_function_argument_list optional_function_type '>' $@47  */
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

  case 605: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 606: /* $@48: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 607: /* $@49: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 608: /* type_declaration_no_options: "function" '<' $@48 type_declaration '>' $@49  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 609: /* $@50: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 610: /* $@51: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 611: /* type_declaration_no_options: "function" '<' $@50 optional_function_argument_list optional_function_type '>' $@51  */
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

  case 612: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 613: /* $@52: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 614: /* $@53: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 615: /* type_declaration_no_options: "lambda" '<' $@52 type_declaration '>' $@53  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 616: /* $@54: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 617: /* $@55: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 618: /* type_declaration_no_options: "lambda" '<' $@54 optional_function_argument_list optional_function_type '>' $@55  */
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

  case 619: /* $@56: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 620: /* $@57: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 621: /* type_declaration_no_options: "tuple" '<' $@56 tuple_type_list '>' $@57  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 622: /* $@58: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 623: /* $@59: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 624: /* type_declaration_no_options: "variant" '<' $@58 variant_type_list '>' $@59  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 625: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 626: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 627: /* type_declaration: type_declaration '|' '#'  */
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

  case 628: /* $@60: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 629: /* $@61: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 630: /* variant_alias_declaration: "variant" $@60 "name" $@61 '{' variant_type_list ';' '}'  */
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

  case 631: /* $@62: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 632: /* bitfield_alias_declaration: "bitfield" $@62 "name" '{' bitfield_bits ';' '}'  */
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

  case 633: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 634: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 635: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 636: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 637: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 638: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 639: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 640: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 641: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 642: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 643: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 644: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 645: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block ']' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 646: /* make_struct_decl: "[[" type_declaration_no_options optional_block ']' ']'  */
                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 647: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block ']' ']'  */
                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 648: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 649: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block '}' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 650: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block '}' ']'  */
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

  case 651: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 652: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 653: /* make_tuple: make_tuple ',' expr  */
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

  case 654: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 655: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 656: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 657: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 658: /* make_dim_decl: "[[" type_declaration_no_options make_dim ']' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 659: /* make_dim_decl: "[{" type_declaration_no_options make_dim '}' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 660: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 661: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 662: /* make_table_decl: "{{" make_table '}' '}'  */
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

  case 663: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 664: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 665: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 666: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


