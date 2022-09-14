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
  YYSYMBOL_optional_return_result = 224,   /* optional_return_result  */
  YYSYMBOL_expression_else_one_liner = 225, /* expression_else_one_liner  */
  YYSYMBOL_226_2 = 226,                    /* $@2  */
  YYSYMBOL_227_3 = 227,                    /* $@3  */
  YYSYMBOL_expression_if_then_else = 228,  /* expression_if_then_else  */
  YYSYMBOL_229_4 = 229,                    /* $@4  */
  YYSYMBOL_expression_for_loop = 230,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 231,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 232,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 233,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 234,    /* expression_with_alias  */
  YYSYMBOL_235_5 = 235,                    /* $@5  */
  YYSYMBOL_annotation_argument_value = 236, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 237, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 238, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 239,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 240, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 241, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration = 242,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 243,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 244, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 245, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 246,   /* optional_function_type  */
  YYSYMBOL_function_name = 247,            /* function_name  */
  YYSYMBOL_global_function_declaration = 248, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 249, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 250, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 251,     /* function_declaration  */
  YYSYMBOL_252_6 = 252,                    /* $@6  */
  YYSYMBOL_expression_block = 253,         /* expression_block  */
  YYSYMBOL_expression_any = 254,           /* expression_any  */
  YYSYMBOL_expressions = 255,              /* expressions  */
  YYSYMBOL_expr_pipe = 256,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 257,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 258,        /* expression_delete  */
  YYSYMBOL_expr_new = 259,                 /* expr_new  */
  YYSYMBOL_expression_break = 260,         /* expression_break  */
  YYSYMBOL_expression_continue = 261,      /* expression_continue  */
  YYSYMBOL_expression_return = 262,        /* expression_return  */
  YYSYMBOL_263_7 = 263,                    /* $@7  */
  YYSYMBOL_264_8 = 264,                    /* $@8  */
  YYSYMBOL_expression_yield = 265,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 266,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 267,                  /* kwd_let  */
  YYSYMBOL_expression_let = 268,           /* expression_let  */
  YYSYMBOL_expr_cast = 269,                /* expr_cast  */
  YYSYMBOL_270_9 = 270,                    /* $@9  */
  YYSYMBOL_271_10 = 271,                   /* $@10  */
  YYSYMBOL_272_11 = 272,                   /* $@11  */
  YYSYMBOL_273_12 = 273,                   /* $@12  */
  YYSYMBOL_274_13 = 274,                   /* $@13  */
  YYSYMBOL_275_14 = 275,                   /* $@14  */
  YYSYMBOL_expr_type_decl = 276,           /* expr_type_decl  */
  YYSYMBOL_277_15 = 277,                   /* $@15  */
  YYSYMBOL_278_16 = 278,                   /* $@16  */
  YYSYMBOL_expr_type_info = 279,           /* expr_type_info  */
  YYSYMBOL_expr_list = 280,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 281,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 282,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 283,            /* capture_entry  */
  YYSYMBOL_capture_list = 284,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 285,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 286,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 287,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 288,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 289,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 290,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 291,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 292,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 293,           /* func_addr_expr  */
  YYSYMBOL_294_17 = 294,                   /* $@17  */
  YYSYMBOL_295_18 = 295,                   /* $@18  */
  YYSYMBOL_296_19 = 296,                   /* $@19  */
  YYSYMBOL_297_20 = 297,                   /* $@20  */
  YYSYMBOL_expr_field = 298,               /* expr_field  */
  YYSYMBOL_299_21 = 299,                   /* $@21  */
  YYSYMBOL_300_22 = 300,                   /* $@22  */
  YYSYMBOL_expr = 301,                     /* expr  */
  YYSYMBOL_302_23 = 302,                   /* $@23  */
  YYSYMBOL_303_24 = 303,                   /* $@24  */
  YYSYMBOL_304_25 = 304,                   /* $@25  */
  YYSYMBOL_305_26 = 305,                   /* $@26  */
  YYSYMBOL_306_27 = 306,                   /* $@27  */
  YYSYMBOL_307_28 = 307,                   /* $@28  */
  YYSYMBOL_expr_mtag = 308,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 309, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 310,        /* optional_override  */
  YYSYMBOL_optional_constant = 311,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 312, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 313, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 314, /* struct_variable_declaration_list  */
  YYSYMBOL_315_29 = 315,                   /* $@29  */
  YYSYMBOL_316_30 = 316,                   /* $@30  */
  YYSYMBOL_317_31 = 317,                   /* $@31  */
  YYSYMBOL_function_argument_declaration = 318, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 319,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 320,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 321,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 322,             /* variant_type  */
  YYSYMBOL_variant_type_list = 323,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 324,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 325,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 326,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 327,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 328, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 329, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 330, /* global_variable_declaration_list  */
  YYSYMBOL_331_32 = 331,                   /* $@32  */
  YYSYMBOL_optional_shared = 332,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 333, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 334,               /* global_let  */
  YYSYMBOL_335_33 = 335,                   /* $@33  */
  YYSYMBOL_enum_list = 336,                /* enum_list  */
  YYSYMBOL_single_alias = 337,             /* single_alias  */
  YYSYMBOL_338_34 = 338,                   /* $@34  */
  YYSYMBOL_alias_list = 339,               /* alias_list  */
  YYSYMBOL_alias_declaration = 340,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 341, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 342,                /* enum_name  */
  YYSYMBOL_enum_declaration = 343,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 344, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 345,          /* optional_sealed  */
  YYSYMBOL_structure_name = 346,           /* structure_name  */
  YYSYMBOL_class_or_struct = 347,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 348, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 349,    /* structure_declaration  */
  YYSYMBOL_350_35 = 350,                   /* $@35  */
  YYSYMBOL_351_36 = 351,                   /* $@36  */
  YYSYMBOL_variable_name_with_pos_list = 352, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 353,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 354, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 355, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 356,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 357,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 358, /* bitfield_type_declaration  */
  YYSYMBOL_359_37 = 359,                   /* $@37  */
  YYSYMBOL_360_38 = 360,                   /* $@38  */
  YYSYMBOL_table_type_pair = 361,          /* table_type_pair  */
  YYSYMBOL_type_declaration = 362,         /* type_declaration  */
  YYSYMBOL_363_39 = 363,                   /* $@39  */
  YYSYMBOL_364_40 = 364,                   /* $@40  */
  YYSYMBOL_365_41 = 365,                   /* $@41  */
  YYSYMBOL_366_42 = 366,                   /* $@42  */
  YYSYMBOL_367_43 = 367,                   /* $@43  */
  YYSYMBOL_368_44 = 368,                   /* $@44  */
  YYSYMBOL_369_45 = 369,                   /* $@45  */
  YYSYMBOL_370_46 = 370,                   /* $@46  */
  YYSYMBOL_371_47 = 371,                   /* $@47  */
  YYSYMBOL_372_48 = 372,                   /* $@48  */
  YYSYMBOL_373_49 = 373,                   /* $@49  */
  YYSYMBOL_374_50 = 374,                   /* $@50  */
  YYSYMBOL_375_51 = 375,                   /* $@51  */
  YYSYMBOL_376_52 = 376,                   /* $@52  */
  YYSYMBOL_377_53 = 377,                   /* $@53  */
  YYSYMBOL_378_54 = 378,                   /* $@54  */
  YYSYMBOL_379_55 = 379,                   /* $@55  */
  YYSYMBOL_380_56 = 380,                   /* $@56  */
  YYSYMBOL_381_57 = 381,                   /* $@57  */
  YYSYMBOL_382_58 = 382,                   /* $@58  */
  YYSYMBOL_383_59 = 383,                   /* $@59  */
  YYSYMBOL_384_60 = 384,                   /* $@60  */
  YYSYMBOL_385_61 = 385,                   /* $@61  */
  YYSYMBOL_386_62 = 386,                   /* $@62  */
  YYSYMBOL_variant_alias_declaration = 387, /* variant_alias_declaration  */
  YYSYMBOL_388_63 = 388,                   /* $@63  */
  YYSYMBOL_389_64 = 389,                   /* $@64  */
  YYSYMBOL_bitfield_alias_declaration = 390, /* bitfield_alias_declaration  */
  YYSYMBOL_391_65 = 391,                   /* $@65  */
  YYSYMBOL_make_decl = 392,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 393,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 394,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 395,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 396,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 397,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 398,           /* make_map_tuple  */
  YYSYMBOL_make_any_tuple = 399,           /* make_any_tuple  */
  YYSYMBOL_make_dim = 400,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 401,            /* make_dim_decl  */
  YYSYMBOL_make_table = 402,               /* make_table  */
  YYSYMBOL_make_table_decl = 403,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 404, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 405       /* array_comprehension  */
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
#define YYLAST   10799

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  199
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  207
/* YYNRULES -- Number of rules.  */
#define YYNRULES  664
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1172

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
       0,   472,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   488,   489,   490,   494,   495,
     499,   517,   518,   519,   520,   524,   528,   533,   542,   550,
     566,   571,   579,   579,   609,   630,   634,   637,   641,   647,
     656,   659,   665,   666,   670,   674,   675,   679,   682,   688,
     694,   697,   703,   704,   708,   709,   710,   719,   720,   724,
     725,   728,   729,   729,   732,   732,   738,   744,   744,   750,
     756,   764,   774,   783,   783,   790,   791,   792,   793,   794,
     795,   799,   804,   812,   813,   814,   818,   819,   820,   821,
     822,   823,   824,   825,   831,   834,   840,   841,   842,   846,
     854,   867,   871,   878,   879,   883,   884,   885,   889,   892,
     899,   903,   904,   905,   906,   907,   908,   909,   910,   911,
     912,   913,   914,   915,   916,   917,   918,   919,   920,   921,
     922,   923,   924,   925,   926,   927,   928,   929,   930,   931,
     932,   933,   934,   935,   936,   937,   938,   939,   940,   941,
     942,   943,   944,   945,   946,   947,   948,   949,   950,   951,
     952,   953,   954,   955,   956,   957,   958,   959,   960,   961,
     962,   963,   964,   965,   966,   967,   968,   969,   970,   971,
     972,   973,   974,   975,   976,   977,   978,   983,  1005,  1006,
    1007,  1011,  1017,  1017,  1034,  1038,  1049,  1050,  1051,  1052,
    1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,
    1063,  1064,  1065,  1066,  1067,  1068,  1072,  1077,  1083,  1089,
    1123,  1126,  1132,  1133,  1144,  1148,  1154,  1157,  1160,  1164,
    1170,  1174,  1178,  1181,  1184,  1189,  1192,  1197,  1197,  1201,
    1201,  1208,  1211,  1216,  1219,  1227,  1233,  1234,  1238,  1244,
    1244,  1244,  1247,  1247,  1247,  1252,  1252,  1252,  1260,  1260,
    1260,  1266,  1276,  1287,  1302,  1305,  1311,  1312,  1319,  1330,
    1331,  1332,  1336,  1337,  1338,  1339,  1343,  1348,  1356,  1357,
    1361,  1366,  1373,  1374,  1375,  1376,  1377,  1378,  1382,  1383,
    1384,  1385,  1386,  1387,  1388,  1389,  1390,  1391,  1392,  1393,
    1394,  1395,  1396,  1397,  1398,  1399,  1400,  1404,  1405,  1406,
    1407,  1411,  1422,  1427,  1437,  1441,  1448,  1451,  1451,  1451,
    1456,  1456,  1456,  1469,  1473,  1477,  1477,  1477,  1484,  1485,
    1486,  1487,  1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,
    1496,  1497,  1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,
    1506,  1507,  1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,
    1516,  1517,  1518,  1519,  1525,  1526,  1527,  1528,  1529,  1530,
    1531,  1532,  1533,  1534,  1535,  1536,  1537,  1541,  1545,  1548,
    1551,  1552,  1553,  1554,  1557,  1560,  1561,  1564,  1564,  1564,
    1567,  1572,  1576,  1580,  1580,  1580,  1585,  1588,  1592,  1592,
    1592,  1597,  1600,  1601,  1602,  1603,  1604,  1605,  1606,  1607,
    1608,  1609,  1610,  1618,  1619,  1620,  1621,  1622,  1623,  1627,
    1631,  1635,  1639,  1643,  1647,  1651,  1655,  1659,  1666,  1667,
    1671,  1672,  1673,  1677,  1678,  1682,  1683,  1684,  1688,  1698,
    1701,  1701,  1720,  1719,  1734,  1733,  1746,  1755,  1760,  1765,
    1775,  1776,  1780,  1783,  1792,  1793,  1797,  1806,  1807,  1812,
    1813,  1817,  1823,  1829,  1832,  1836,  1842,  1851,  1852,  1853,
    1857,  1858,  1862,  1869,  1874,  1883,  1889,  1900,  1903,  1908,
    1913,  1921,  1932,  1935,  1935,  1955,  1956,  1960,  1961,  1962,
    1966,  1969,  1969,  1988,  1991,  2000,  2013,  2013,  2034,  2035,
    2039,  2040,  2044,  2045,  2046,  2050,  2060,  2067,  2077,  2078,
    2082,  2083,  2087,  2093,  2094,  2098,  2099,  2100,  2104,  2109,
    2104,  2119,  2126,  2131,  2140,  2146,  2157,  2158,  2159,  2160,
    2161,  2162,  2163,  2164,  2165,  2166,  2167,  2168,  2169,  2170,
    2171,  2172,  2173,  2174,  2175,  2176,  2177,  2178,  2179,  2180,
    2181,  2185,  2186,  2187,  2188,  2189,  2190,  2194,  2205,  2209,
    2216,  2228,  2235,  2244,  2244,  2244,  2257,  2261,  2268,  2269,
    2270,  2271,  2272,  2286,  2292,  2296,  2300,  2305,  2310,  2315,
    2320,  2324,  2328,  2333,  2337,  2342,  2342,  2342,  2348,  2355,
    2355,  2355,  2360,  2360,  2360,  2366,  2366,  2366,  2371,  2375,
    2375,  2375,  2380,  2380,  2380,  2389,  2393,  2393,  2393,  2398,
    2398,  2398,  2407,  2411,  2411,  2411,  2416,  2416,  2416,  2425,
    2425,  2425,  2431,  2431,  2431,  2440,  2440,  2440,  2464,  2464,
    2484,  2485,  2486,  2487,  2491,  2498,  2505,  2511,  2520,  2525,
    2532,  2533,  2537,  2543,  2550,  2558,  2565,  2573,  2585,  2588,
    2602,  2608,  2614,  2615,  2619,  2624,  2631,  2636,  2646,  2651,
    2658,  2670,  2671,  2675,  2678
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
  "optional_return_result", "expression_else_one_liner", "$@2", "$@3",
  "expression_if_then_else", "$@4", "expression_for_loop",
  "expression_unsafe", "expression_while_loop", "expression_with",
  "expression_with_alias", "$@5", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "annotation_declaration_name", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@6", "expression_block", "expression_any",
  "expressions", "expr_pipe", "name_in_namespace", "expression_delete",
  "expr_new", "expression_break", "expression_continue",
  "expression_return", "$@7", "$@8", "expression_yield",
  "expression_try_catch", "kwd_let", "expression_let", "expr_cast", "$@9",
  "$@10", "$@11", "$@12", "$@13", "$@14", "expr_type_decl", "$@15", "$@16",
  "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_name", "func_addr_expr", "$@17", "$@18", "$@19", "$@20",
  "expr_field", "$@21", "$@22", "expr", "$@23", "$@24", "$@25", "$@26",
  "$@27", "$@28", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@29", "$@30", "$@31", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@32", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@33", "enum_list",
  "single_alias", "$@34", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_name", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@35", "$@36", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@37", "$@38", "table_type_pair",
  "type_declaration", "$@39", "$@40", "$@41", "$@42", "$@43", "$@44",
  "$@45", "$@46", "$@47", "$@48", "$@49", "$@50", "$@51", "$@52", "$@53",
  "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62",
  "variant_alias_declaration", "$@63", "$@64",
  "bitfield_alias_declaration", "$@65", "make_decl", "make_struct_fields",
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

#define YYPACT_NINF (-988)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-617)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -988,    30,  -988,  -988,    25,   -70,   -23,  -106,  -988,   -71,
    -988,  -988,    12,  -988,  -988,  -988,  -988,  -988,   235,  -988,
      93,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
       4,  -988,    63,   129,   109,  -988,  -988,   -23,    10,  -988,
    -988,   148,  -988,  -988,  -988,    93,   219,   226,  -988,  -988,
     233,   265,   236,  -988,   110,  -988,  -988,  -988,   241,   282,
     368,  -988,   370,     5,    25,   275,   -70,   254,   286,  -988,
     295,   306,  -988,   288,   280,  -105,   373,   283,  -988,  -988,
     319,    25,    12,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
     329,  -988,  -988,  -988,  -988,  -988,   291,  -988,  -988,  -988,
    -988,  -988,   -41,    57,  -988,  -988,  -988,  -988,   433,  -988,
    -988, 10548,  -988,  -988,   294,  -988,  -988,  -988,   333,   296,
    -988,    70,  -988,   754,  -988,   -39,   394,  -988,   345,  -988,
    -988,   323,  -988,  -988,  -988,  -988,  -988,  -988,  -988,    76,
    -988,   322,   324,   325,   326,  -988,  -988,  -988,   304,  -988,
    -988,  -988,  -988,  -988,   327,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,   328,  -988,  -988,  -988,   331,   337,
    -988,  -988,  -988,  -988,   338,   339,   310,  -988,  -988,  -988,
    -988,  -988,  3280,  -988,  -988,   317,   357,  -988,  9299,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,   482,   483,  -988,   336,   334,   297,  -988,  -988,
     364,  -988,   332,    25,   160,  -988,  -988,  -988,    57,  -988,
    -988,  -988,  -988,  -988,   374,  -988,   142,   155,   179,  -988,
    -988,  5363,  -988,  -988,  -988,  -988,   500,  -988,  -988,    -6,
    3427,  -988,  -102,   366,  -988,   342,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,   348,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,   518,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,   350,  -988,  -988,   -59,
     375,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,   353,
     -99,   378,   354,  -988,   345,   117,   360,   512,   253,  -988,
    -988, 10548, 10548, 10548, 10548,   349,   333, 10548,   336, 10548,
     336, 10548,   336, 10642,   357,  -988,  -988, 10454,   365,   385,
    -988,   367,   387,   388,   377,   389,   383,  -988,   397,  5363,
    5363,   390,   395,   396,   399,   400,   401, 10145, 10239,  5363,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  5363,  5363,  5363,
     152,  5363,  5363,  5363,  -988,   386,  -988,  -988,  -988,  -103,
    -988,  -988,  -988,  -988,   404,  -988,  -988,  -988,  -988,  -988,
    -988,  6262,  -988,   403,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,   393,  -988,  -988,  8730,  -988,  -988, 10548,   -83,
    -988,  -988,  -988,   405,  -988,   -11,  -988,   252, 10548,  -988,
    1515,  -988,     0,  -988,   152,  -988,  -988,   160,   408,  5363,
     438,   442, 10548,  -988,   -26,   206,   428,   496,   592,  1330,
    -988,    18,  1448,   375,  1645,   375,  1841,   375,   -25,  -988,
     100,  3280,   189,  1269,  -988,   152,  -988,  5363,  -988,  -988,
    5363,  -988,  5363, 10548,   169,   169,  5363,  5363,  5363,  5363,
    5363,  5363,   237,  2167,   237,  2336,  9395,  -988,  -133,   312,
     169,   169,   -37,  -988,   169,   169,  6364,   -45,  -988,  2820,
     455,  9467, 10270,  5363,  5363,  -988,  -988,  5363,  5363,  5363,
    5363,   447,  5363,   157,  5363,  5363,  5363,  5363,  5363,  5363,
    5363,  5363,  5363,  3573,  5363,  5363,  5363,  5363,  5363,  5363,
    5363,  5363,  5363,  5363,   -53,  5363,  -988,  3719,  -988,  -988,
    3280,  -988,  -988,  5363,   237,   414,   565,  -988,    -7,  -988,
      88,  3280,  -988,  5363,  -988,  -988,   237,  1901,  -988,   334,
    5363,  5363,   450,  -988,   413,   458,  3888,   -75,  2674,   266,
     266,  -988,   574,   417,   420,  5363,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,   424,   427,   430,  -988,  -988,  -988,
     160,  -988,   -84,  -988,  5464,   -80,  5363,  -988,  -988,    14,
    -988,  -988,  6455,  -988,   595,   851,  -988,  -988,  -988,  2989,
    -988,  -988, 10548,  -988,  -988,  -988,   480,  -988,   465,  -988,
     466,  -988,   467, 10548,  -988, 10642,  -988,   357,  4034,  4203,
   10548,  6546, 10548, 10548,  6637, 10548,  6728,  2094,  6819,  6910,
    7001,  7092,  7183,  7274,    20,   266,   137,  2505,  4372,  9395,
     476,   -22,   452,   478,  -988,  -988,   250,    21,  4541,   -22,
     212,  5363,  5363,   456,  -988,  5363,   182,   485,  -988,   461,
     463,   191,  -988,  -988,   490,  -988,    91,  9633,    84,   336,
     477,   464,  -988,  -988,   484,   469,  -988,  -988,   596,   596,
     330,   330,   292,   292,   470,    59,   473,  -988,  8821,   -63,
     -63,   596,   596,  9936,  9827,  9904,  9717, 10360,  9472,   457,
    1115, 10013,   330,   330,    29,    29,    59,    59,    59,   187,
    5363,   474,  -988,   224,  5363,   656,  8912,  -988,    95,  7365,
    -988,  5363,   501,  -988,   509,  -988, 10548,  -988,  2989,  -988,
    5566,    23,  -988,  2989,  -988,  -988,   547,  2002,   658,  9633,
    5566,   507,  -988,   511,   529,  9633,  -988,  2989,  -988,  5668,
     498,  -988,  -988,   494,  -988,  -988,  5566,  -988,  -988,  -988,
    -988,   266,  -988,  -988,  5363,  5363,  5363,  5363,  5363,  5363,
    5363,  5363,  5363,  5363,  3135,  5363,  5363,  5363,  5363,  5363,
    5363,  3281,  -988,  1233,    12,  -988,   690,   345,  -988,   541,
     500,  -988,  2989,  -988,  5770,  -988,  -988,  3280,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  3280,  -988,  -988,
    -988,  -988,   101,   542,  7456,  2268,  -988,  2437,  2504,  -988,
    2606,  -988,   455,  -988,  -988,  -988,  -988,  -988,   515,  5363,
    -988,  5363,  5363,   -16,   551,   490,   519,   521,  5363,   523,
    5363,  5363,   490,   522,   528,  9633,  -988,  -988,  9556,  -988,
     561,  5363,  5363, 10548,   336,   -24,   121,  5363,  -988,   566,
     569,   570,   571,  -988,   126,   375,  -988,  5363,  -988,  5363,
    4710,  5363,  -988,   550,   534,  -988,  -988,  5363,   538,  -988,
    9003,  5363,   539,  -988,  9094,  -988,  -988,  -988,  -988,  7547,
    -988,   691,  2819,  -988,  9196,  -988,  5363,  5363,  -988,  5872,
    -988,  5363,  5363,  -988,   334,  -988,  -988,  -988,  -988,  -988,
    5974,  -988,  -988,  -988,   392,  -988,  5363,  9633,  9633,  9633,
    9633,  9633,  9633,  9633,  9633,  9633,  9633,  5363,  5363,  9633,
    9633,  9633,  9633,  9633,  9633,  9633,  5363,  5363,  9633,  -988,
     171,   380,    16,   536,  -988,  -988,  6076,  -988,  -988,  -988,
    -988,  -988,   201,  -988,  -988,  -988,  -988,  -988,   543,  4879,
     -56,  9633,  9633,   -22,   546,     6,   476,   557,  -988,  9633,
    -988,  -988,     9,   -22,   558,  -988,  -988,  -988,  7638,  7729,
    2753,   375,   559,  9633,  -988,  -988,  -988,  -988,    84,   560,
     -90, 10548,  7820, 10548,  7911,  -988,   106,  8002,  -988,  5363,
    9794,  5363,  -988,  8093,  5363,  -988,  -988,  -988,   576,   500,
    5363,    35,   568,  -988,  5363,  -988,  5363,  -988,  1712,   334,
    -988,  -988,  5363,  -988,   568,   981,  9633,   981,  9633,   554,
    -988,  -988,    31,  -988,  -988,   380,  -988,  -988,  5363,   597,
    -988,  5363,  5363,  5363,  5048,  -988,   107,  5363,   562,   572,
    5363,  5363,  -988,  5363,   563,  -988,  -988,  -988,  -988,   582,
    -988,  -988,  -988,  5217,  -988,  -988,  2921,  -988,  3068,  -988,
    -988,  -988, 10548,  8184,  8275,  -988,  8366,  -988,  9633,   334,
     736,   573,   568,  9633,  -988,  -988,  5566,  -988,  -988,  -988,
     732,   237,  8457,   594,   330,   330,   330,  -988,  8548,  -988,
    6178,   579,  -988,  9633,  9633,  6178,   586,   135,  -988,  5363,
    9717,  -988,  -988,  3214,  -988,  -988,  -988,  -988,  5363,   755,
    -988,   584,   392,   754,  -988,  -988,  -988,  -988,  5363,  -988,
     738,   593,  -988,   601,  -988,  -988,   135,  9794,  -988,  -988,
    -988,  9633,  5363,  -988,  -988,   598,   754,  8639,  5363,   602,
     604,  -988,  -988,  -988,  9633,  -988,   334,  -988,  9633,  -988,
    -988,  -988
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   103,     1,   246,     0,     0,     0,     0,   247,     0,
     628,   625,     0,    14,     3,    10,     9,     8,     0,     7,
     485,     6,    11,     5,     4,    12,    13,    84,    85,    83,
      92,    94,    34,    47,    44,    45,    36,     0,    42,    35,
     496,     0,   501,    19,    18,   485,     0,     0,    97,    98,
       0,   222,    99,   101,     0,    96,   514,   513,   188,   502,
     515,   486,   487,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,   626,   224,
       0,     0,     0,   104,   190,   189,   192,   187,   504,   503,
       0,   517,   516,   518,   489,   488,   491,    90,    91,    88,
      89,    87,     0,     0,    86,    95,    48,    46,    42,    39,
      38,     0,   498,   500,     0,    16,    17,    20,     0,     0,
     223,     0,   102,     0,   505,     0,   510,   482,   428,    21,
      22,     0,    79,    80,    77,    78,    76,    75,    81,     0,
      41,     0,     0,     0,     0,   526,   546,   527,   558,   528,
     532,   533,   534,   535,   550,   539,   540,   541,   542,   543,
     544,   545,   547,   548,   598,   531,   538,   549,   605,   612,
     529,   536,   530,   537,     0,     0,     0,   557,   568,   571,
     569,   570,   497,   499,   561,     0,     0,   100,     0,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,     0,     0,   110,   105,     0,     0,   493,   511,
       0,   519,   483,     0,     0,    23,    24,    25,     0,    93,
     589,   592,   595,   585,     0,   563,   599,   606,   613,   619,
     622,     0,   576,   581,   575,   588,     0,   584,   578,     0,
       0,   580,     0,     0,   457,     0,   161,   162,   159,   113,
     114,   116,   115,   117,   118,   119,   120,   146,   147,   144,
     145,   137,   148,   149,   138,   135,   136,   160,   157,     0,
     158,   150,   151,   152,   153,   124,   125,   126,   121,   122,
     123,   134,     0,   140,   141,   139,   132,   133,   128,   127,
     129,   130,   131,   112,   111,   156,     0,   142,   143,   428,
     108,   216,   193,   551,   554,   552,   555,   553,   556,     0,
       0,   508,     0,   490,   428,     0,     0,   472,   470,   492,
      82,     0,     0,     0,     0,     0,     0,     0,   105,     0,
     105,     0,   105,     0,     0,   334,   335,     0,     0,     0,
     328,     0,     0,     0,     0,     0,     0,   550,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     282,   284,   283,   285,   286,   287,    26,     0,     0,     0,
       0,     0,     0,     0,   269,   270,   332,   331,   280,   329,
     405,   404,   403,   402,   103,   408,   330,   407,   406,   375,
     336,     0,   337,     0,   333,   630,   631,   632,   633,   583,
     577,   579,     0,   582,   573,     0,   562,   629,     0,     0,
     155,   163,   154,     0,   106,     0,   450,     0,     0,   191,
       0,   493,     0,   506,     0,   512,   439,     0,     0,     0,
       0,     0,     0,   471,     0,     0,     0,   566,     0,     0,
     559,     0,     0,   108,     0,   108,     0,   108,   222,   454,
       0,   452,     0,   226,   229,     0,   258,     0,   249,   252,
       0,   255,     0,     0,   364,   365,     0,     0,     0,     0,
       0,     0,     0,   640,     0,     0,   651,   658,     0,     0,
     341,   340,   380,    32,   339,   338,     0,   271,   411,     0,
     278,     0,     0,     0,     0,   366,   367,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   325,     0,   560,     0,   574,   572,
     456,   627,   458,     0,     0,     0,   521,   447,   461,   107,
     428,   109,   218,     0,    57,    58,     0,     0,   230,     0,
       0,     0,     0,   231,     0,     0,     0,     0,     0,     0,
       0,   196,   194,     0,     0,     0,   211,   206,   203,   202,
     204,   205,   217,   197,     0,     0,     0,   209,   210,   212,
       0,   201,     0,   198,   288,     0,     0,   494,   509,   440,
     484,   429,     0,   474,   475,     0,   468,   469,   467,     0,
     590,   593,     0,   596,   586,   564,     0,   600,     0,   607,
       0,   614,     0,     0,   620,     0,   623,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,     0,     0,   648,
     638,   640,     0,   652,   653,   654,     0,     0,     0,   640,
       0,     0,     0,     0,    29,     0,    27,     0,   368,     0,
       0,   317,   314,   316,     0,   376,     0,   264,     0,   105,
       0,     0,   391,   390,     0,     0,   392,   396,   342,   343,
     355,   356,   353,   354,     0,   385,     0,   373,     0,   409,
     410,   344,   345,   360,   361,   362,   363,     0,     0,   358,
     359,   357,   351,   352,   347,   346,   348,   349,   350,     0,
       0,     0,   323,     0,     0,     0,     0,   378,     0,     0,
     448,     0,     0,   460,     0,   459,     0,   462,     0,   451,
       0,     0,   237,     0,   232,   235,     0,   288,     0,   225,
       0,     0,   215,     0,     0,    51,    70,     0,   243,   288,
     270,   220,   221,     0,   213,   214,     0,   200,   207,   208,
     248,     0,   199,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   507,     0,     0,   520,     0,   428,   473,     0,
     467,   477,     0,   481,   288,   591,   594,   567,   597,   587,
     565,   601,   603,   608,   610,   615,   617,   453,   621,   455,
     624,   227,     0,     0,     0,     0,   381,     0,     0,   382,
       0,   412,   278,   413,   414,   415,   416,   417,     0,     0,
     641,     0,     0,   640,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   650,   659,   660,     0,    30,
      33,     0,     0,     0,   105,     0,     0,     0,   377,     0,
       0,     0,     0,   276,     0,   108,   387,     0,   393,     0,
       0,     0,   371,     0,     0,   397,   401,     0,     0,   374,
       0,     0,     0,   324,     0,   326,   369,   379,   449,     0,
     523,   524,   463,   466,   465,    71,     0,     0,   236,   288,
     239,     0,     0,   233,     0,    72,    73,    49,    50,   244,
     288,   241,   271,   216,    54,   219,     0,   298,   299,   301,
     300,   302,   292,   293,   294,   303,   304,     0,     0,   290,
     291,   305,   306,   295,   296,   297,     0,     0,   289,   495,
       0,   435,   430,     0,   476,   479,   288,   480,   604,   611,
     618,   228,     0,   261,   259,   250,   253,   256,     0,     0,
       0,   635,   634,   640,     0,     0,   639,     0,   643,   649,
     656,   655,     0,   640,     0,   657,    28,    31,     0,     0,
       0,   108,     0,   265,   274,   275,   273,   272,     0,     0,
       0,     0,     0,     0,     0,   312,     0,     0,   398,     0,
     386,     0,   372,     0,     0,   370,   327,   522,     0,   459,
       0,     0,    61,   234,     0,   245,     0,   242,     0,     0,
      52,    53,     0,    66,    61,   309,   310,   307,   308,   104,
     436,   437,   430,   431,   432,   435,   441,   478,     0,     0,
     260,     0,     0,     0,     0,   418,     0,     0,     0,     0,
       0,     0,   642,     0,     0,   646,   315,   427,   318,     0,
     311,   277,   279,     0,   266,   281,     0,   426,     0,   424,
     313,   421,     0,     0,     0,   420,     0,   525,   464,     0,
      62,     0,    61,    74,   195,    55,     0,    68,   446,   442,
     433,     0,     0,     0,   251,   254,   257,   383,     0,   419,
     661,     0,   644,   637,   636,   661,     0,     0,   321,     0,
     267,   388,   394,     0,   425,   423,   422,    69,     0,     0,
     238,     0,    54,     0,   434,   444,   438,   262,     0,   384,
       0,     0,   645,     0,   647,   319,     0,   268,   389,   395,
     399,    63,    59,   240,    56,     0,     0,     0,     0,     0,
       0,   322,   400,    65,    60,   443,     0,   263,   662,   663,
     664,   445
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -988,  -988,  -988,  -988,  -988,   302,   739,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,   766,  -988,   696,  -988,  -988,   740,
    -988,  -988,  -988,  -327,  -988,  -988,  -987,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,   580,  -988,  -988,
     743,   -47,  -988,   727,    17,  -379,  -331,  -443,  -988,  -988,
    -988,  -962,  -988,  -988,  -214,  -988,  -113,  -541,   -12,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,   811,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -513,  -988,  -988,  -185,  -988,   -18,  -537,  -988,  -426,
    -988,  -988,  -988,  -865,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,   418,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -125,  -226,  -988,  -228,  -988,  -988,  -988,  -988,  -988,   269,
    -988,   196,  -988,  -401,   493,  -542,  -539,   222,  -988,  -988,
    -409,  -988,  -988,   796,  -988,  -988,  -988,   411,    56,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -436,  -110,  -988,  -988,  -988,   513,  -988,
    -988,  -988,  -988,   -88,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,  -988,
    -988,  -988,  -988,   503,  -652,  -477,  -634,  -988,  -988,  -356,
      -3,   369,  -988,  -988,  -988,  -263,  -988
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   117,    45,    14,   131,   137,   489,   386,   860,
     387,   667,    15,    16,    38,    39,    72,    17,    34,    35,
     573,   574,  1032,  1033,   575,  1163,  1091,  1128,  1129,   576,
     926,   577,   578,   579,   580,   581,  1026,   138,   139,    30,
      31,    32,    52,    53,    54,    18,   310,   429,   215,    19,
      86,   216,    87,   123,   388,   582,   430,   583,   389,   584,
     390,   585,   586,   587,   907,  1024,   588,   589,   590,   591,
     391,   632,  1051,   633,  1052,   635,  1053,   392,   630,  1050,
     393,   676,  1075,   394,   873,   874,   679,   395,   396,   746,
     593,   397,   398,   673,   399,   863,  1117,   864,  1146,   400,
     725,  1016,   677,  1001,  1148,  1003,  1149,  1082,  1162,   402,
     425,  1045,  1135,  1042,   953,   599,   797,  1133,  1156,   426,
     427,   459,   460,   254,   255,   842,   547,   609,   444,   328,
     329,   222,   324,    62,    96,    21,   128,   320,    42,    73,
      75,    22,    90,   125,    23,   435,   220,   221,    60,    93,
      24,   126,   322,   548,   403,   319,   179,   180,   185,   181,
     336,   810,   446,   461,   334,   809,   331,   805,   332,   806,
     333,   808,   337,   811,   338,   958,   339,   813,   340,   959,
     341,   815,   342,   960,   343,   818,   344,   820,    25,    47,
     119,    26,    46,   404,   650,   651,   652,   405,   653,   654,
     655,   656,   406,   488,   407,  1141,   408
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   178,   312,   224,   592,   730,   738,   453,   659,   455,
     618,   457,   620,   487,   622,   500,   745,   846,   542,    97,
      98,  -103,   866,   182,   728,   853,   410,   758,   600,   645,
       2,    68,   761,   762,   121,   645,     3,   544,   771,   839,
     851,  1073,   906,    27,    28,    48,   644,  1097,   657,   501,
     502,    40,    40,   511,   498,   416,   513,   514,   432,     4,
    1043,     5,   662,     6,   663,    69,   719,   720,   803,     7,
      55,   132,   133,    49,   253,  1043,  1099,   432,     8,   501,
     502,    33,   513,   514,     9,   423,    43,    50,   499,   223,
      41,   606,   113,   733,   721,   417,   669,    74,   433,   177,
     607,   670,   841,    10,   722,  1131,   311,    80,   840,   867,
     733,   772,    51,  1044,   541,   822,   472,   792,   129,   130,
     741,   311,    44,   733,    11,   534,   535,   671,  1044,   217,
     545,   114,  1060,   424,    36,   723,   724,   505,   506,  1057,
     608,   865,   735,   623,    50,   511,   546,   512,   513,   514,
     515,   534,   535,    61,   516,    37,    99,   218,   734,   735,
     100,   736,   101,   102,   737,   411,   596,   505,   506,    51,
      63,  1155,   735,   845,   867,   511,   325,   512,   513,   514,
     515,   770,    29,   412,   516,   734,   734,    70,   734,   501,
     502,   615,   413,   976,  1166,   597,   103,   903,    71,   437,
     867,   869,   908,   794,  1063,   531,   532,   533,   134,   974,
     870,   795,   135,   616,   136,   102,   919,   534,   535,    12,
     796,   178,   178,   178,   178,    13,   542,   178,    64,   178,
    1089,   178,   423,   178,   925,    64,   223,   178,   242,    56,
      57,   228,    58,   445,   447,   448,   449,   534,   535,   452,
     871,   454,  1145,   456,   733,   872,   867,   178,   178,   463,
     867,   955,   187,   841,   243,   244,   867,    50,   229,    80,
      59,   867,   867,   624,    66,    82,   669,   505,   506,   483,
     485,  1161,    64,   868,    50,   511,   844,   897,   513,   514,
     515,   998,    51,   961,   516,   625,    84,    65,  1080,  1109,
      83,   326,    85,   735,   696,    40,   856,   438,   178,    51,
    -602,   992,   501,   502,   697,  -602,   999,   327,   178,   177,
     177,   177,   177,  -609,   245,   177,   970,   177,  -609,   177,
     540,   177,   178,  -602,   888,   177,    82,    88,   982,  1058,
     551,   225,   226,    89,   889,   748,  -609,  -616,   875,  1064,
     501,   502,  -616,   756,   605,   177,   177,   534,   535,  -320,
    1020,  1039,   626,   178,  -320,   313,   973,  1006,   493,   314,
    -616,   892,   246,   247,  1048,   983,    77,   248,   545,   610,
     249,   893,  -320,    78,   627,   637,   315,   316,   317,   318,
      79,   683,   687,  1021,   546,   250,  1049,    80,   503,   504,
     505,   506,   507,  1029,   251,   508,   177,   850,   511,   854,
     512,   513,   514,   515,  1030,  1031,   177,   516,   441,   517,
     518,   442,   598,    91,   443,    94,   106,    81,   115,    92,
     177,    95,  1000,  1061,   116,  1040,   503,   504,   505,   506,
     849,  1041,    71,   108,   549,   850,   511,   550,   512,   513,
     514,   515,   109,   629,   111,   516,  1056,   517,   518,   384,
     760,   177,   311,   110,   527,   528,   529,   530,   531,   532,
     533,   129,   130,   664,   665,   112,   120,   501,   502,   118,
     534,   535,   225,   226,   227,   672,   124,   127,    69,   183,
     184,   219,   186,   223,   230,   234,   231,   232,   233,   235,
     236,   241,   178,   237,   529,   530,   531,   532,   533,   238,
     239,   240,   252,   178,   253,   178,   307,   308,   534,   535,
     178,   321,   178,   178,   807,   178,   905,   309,   242,   323,
     311,   335,   409,   991,   418,   817,   915,   419,   420,   421,
     422,   450,   825,   428,   827,   828,   434,   830,  1069,   431,
     436,   439,   924,   440,   243,   244,   465,   466,   467,   468,
     469,   471,  1136,   503,   504,   505,   506,   507,   470,   473,
     508,   509,   510,   511,   472,   512,   513,   514,   515,  1090,
     497,   476,   516,   538,   517,   518,   477,   478,   501,   502,
     479,   480,   481,    12,   537,   603,   543,   886,   601,   604,
     177,   611,   592,   678,   694,   731,   732,   751,   752,   753,
     763,   177,   764,   177,   245,   765,   501,   502,   177,   767,
     177,   177,   768,   177,   242,   769,   178,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   799,   416,   812,   814,
     816,   844,   847,   848,   859,   534,   535,   865,   902,   876,
     243,   244,   861,   857,   862,   877,   878,   895,   900,   401,
     879,   880,   246,   247,   881,   891,   901,   248,   415,   771,
     249,   914,   952,   916,   503,   504,   505,   506,   507,   917,
     918,   508,   509,   510,   511,   250,   512,   513,   514,   515,
     923,   612,   922,   516,   251,   517,   518,   951,   954,   962,
    1025,   519,   520,   521,   505,   506,   969,   522,   975,   977,
     245,   978,   511,   980,   512,   513,   514,   515,   985,   984,
     987,   516,  1008,   994,   177,  1009,   995,   996,   997,  1011,
    1014,  1046,  1018,  1087,  1054,   523,  1059,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,  1062,  1065,  1098,
    1072,  1070,  1111,   178,  1103,  1118,   534,   535,   246,   247,
    1116,   -64,  1112,   248,  1134,   613,   249,  1138,  1130,  1142,
     529,   530,   531,   532,   533,   990,  1144,   474,   475,  1153,
    1152,   250,    55,  1159,   534,   535,  1074,   486,   188,  1158,
     251,   666,  1169,  1165,  1170,   490,   491,   492,  1160,   494,
     495,   496,   104,    67,   140,  1154,   107,   105,   330,   122,
    1028,   950,    20,  1071,   968,  1095,  1100,  1101,   189,   739,
     190,   819,   191,   192,   193,   194,   195,   802,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   462,   205,   206,
     207,    76,   595,   208,   209,   210,   211,   981,   594,   451,
     464,   177,  1143,     0,   660,     0,     0,   602,     0,     0,
       0,     0,   212,   213,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1127,     0,     0,     0,     0,
       0,     0,  1132,   242,     0,   631,     0,     0,   634,     0,
     636,   178,     0,   178,   638,   639,   640,   641,   642,   643,
       0,   649,     0,   649,     0,     0,     0,     0,     0,   243,
     244,   214,     0,  1076,     0,  1078,     0,     0,     0,     0,
       0,   688,   689,     0,     0,   690,   691,   692,   693,     0,
     695,     0,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   708,   709,   710,   711,   712,   713,   714,   715,   716,
     717,   718,  1171,   726,     0,     0,     0,     0,     0,     0,
       0,   729,     0,     0,     0,     0,     0,     0,   606,   245,
       0,   740,   178,     0,     0,   747,     0,   607,   749,   750,
       0,     0,     0,     0,   755,     0,   759,     0,     0,   177,
       0,   177,     0,   766,  1123,     0,     0,     0,     0,     0,
       0,   501,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   793,     0,     0,   800,   247,     0,
       0,     0,   248,     0,     0,   249,     0,   804,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     250,     0,     0,     0,     0,     0,   801,   824,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,   496,     0,     0,     0,
     177,     0,     0,     0,     0,     0,   496,     0,     0,   855,
     486,     0,     0,   858,     0,     0,     0,   503,   504,     0,
       0,   507,     0,     0,   508,   509,   510,   511,     0,   512,
     513,   514,   515,     0,     0,   672,   516,     0,   517,   518,
       0,     0,     0,     0,   519,   520,   521,     0,     0,     0,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   672,   501,   502,     0,   890,     0,
       0,     0,   894,     0,     0,     0,     0,     0,   523,   899,
     524,   525,   526,   527,   528,     0,   904,     0,   532,     0,
       0,   909,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,     0,     0,   920,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   927,   928,   929,   930,   931,   932,   933,   934,
     935,   936,   939,   940,   941,   942,   943,   944,   945,   948,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     956,   503,   504,   505,   506,   507,     0,     0,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   501,   502,     0,     0,     0,     0,   971,
     972,     0,     0,     0,     0,     0,   979,     0,   649,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   988,
     989,     0,     0,     0,     0,   993,   526,   527,   528,   529,
     530,   531,   532,   533,     0,  1002,     0,  1004,     0,  1007,
       0,   242,     0,   534,   535,  1010,     0,     0,     0,  1013,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1022,     0,   243,   244,   939,
     948,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,   505,   506,   507,  1034,     0,   508,   509,   510,   511,
       0,   512,   513,   514,   515,  1035,  1036,     0,   516,     0,
     517,   518,   242,     0,  1037,  1038,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,   243,   244,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,     0,     0,     0,     0,  1083,   949,  1084,
       0,     0,  1086,     0,     0,   246,   247,     0,  1088,     0,
     248,     0,  1092,   249,  1093,     0,   594,     0,   245,     0,
    1096,     0,     0,     0,     0,     0,     0,     0,   250,     0,
     628,     0,     0,     0,     0,     0,  1102,   251,     0,  1104,
    1105,  1106,  1108,     0,     0,  1110,     0,     0,  1113,  1114,
     242,  1115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1120,     0,     0,     0,     0,   246,   247,     0,     0,
       0,   248,     0,   614,   249,     0,   243,   244,     0,     0,
       0,     0,     0,     0,     0,     0,   552,     0,     0,   250,
       0,     3,     0,   553,   554,   555,     0,   556,   251,   345,
     346,   347,   348,   349,     0,     0,     0,  1147,     0,     0,
     557,   350,   558,   559,     0,     0,  1151,     0,     0,     0,
       0,     0,   560,   351,     0,   561,  1157,   562,   352,     0,
       0,   353,     0,     8,   354,   563,   245,   564,   355,     0,
    1164,   565,   566,     0,     0,     0,  1168,     0,   567,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   357,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     358,   568,     0,     0,   246,   247,     0,     0,     0,   248,
       0,   617,   249,   359,   360,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   250,   569,   570,
       0,     0,     0,     0,     0,     0,   251,    50,     0,     0,
       0,     0,     0,     0,     0,   361,   362,   363,   364,   365,
       0,   366,     0,   367,   368,   369,   370,   371,   372,   373,
     374,   375,    51,   376,     0,     0,     0,   242,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,     0,   380,     0,     0,   381,   382,     0,     0,
       0,     0,     0,   243,   244,     0,   383,     0,   384,   385,
     571,   311,   572,   552,     0,     0,     0,     0,     3,     0,
     553,   554,   555,     0,   556,     0,   345,   346,   347,   348,
     349,     0,     0,     0,     0,     0,     0,   557,   350,   558,
     559,     0,     0,     0,     0,     0,     0,     0,     0,   560,
     351,     0,   561,     0,   562,   352,     0,     0,   353,     0,
       8,   354,   563,   245,   564,   355,     0,     0,   565,   566,
       0,     0,     0,     0,     0,   567,   145,   146,   147,     0,
     149,   150,   151,   152,   153,   357,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   165,   166,   167,     0,
       0,   170,   171,   172,   173,     0,     0,   358,   568,     0,
       0,   246,   247,     0,     0,     0,   248,     0,   619,   249,
     359,   360,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   250,   569,   570,     0,     0,     0,
       0,     0,     0,   251,    50,     0,     0,     0,     0,     0,
       0,     0,   361,   362,   363,   364,   365,     0,   366,     0,
     367,   368,   369,   370,   371,   372,   373,   374,   375,    51,
     376,     0,     0,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,     0,
     380,     0,     0,   381,   382,     0,     0,     0,     0,   243,
     244,     0,     0,   383,     0,   384,   385,   571,   311,  1094,
     742,     0,     0,     0,     0,   345,   346,   347,   348,   349,
       0,     0,     0,     0,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
       0,     0,     0,     0,   352,     0,     0,   353,     0,     0,
     354,     0,     0,     0,   355,     0,     0,     0,     0,   245,
       0,     0,     0,     0,   356,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   357,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,   247,   359,
     360,   910,   248,     0,   621,   249,     0,     0,   743,     0,
       0,     0,   501,   502,   569,   570,     0,     0,     0,     0,
     250,     0,     0,    50,     0,     0,     0,     0,     0,   251,
       0,   361,   362,   363,   364,   365,     0,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,    51,   376,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,     0,   380,
       0,     0,   381,   382,     0,     0,     0,     0,     0,     0,
       0,     0,   383,     0,   384,   385,   744,   311,     0,     0,
     774,   775,   776,   777,   778,   779,   780,   781,   503,   504,
     505,   506,   507,   782,   783,   508,   509,   510,   511,   911,
     512,   513,   514,   515,     0,     0,   242,   516,   785,   517,
     518,   786,   787,     0,     0,   519,   520,   521,   788,   789,
     790,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   243,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   912,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,   345,   346,   347,   348,   349,     0,     0,     0,     0,
     534,   535,     0,   350,     0,     0,     0,   913,     0,   242,
       0,     0,     0,     0,     0,   351,     0,     0,     0,     0,
     352,     0,   245,   353,     0,     0,   354,     0,   645,     0,
     355,     0,     0,     0,     0,   243,   244,     0,     0,     0,
     356,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     357,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
     246,   247,   358,     0,     0,   248,     0,   832,   249,     0,
       0,     0,     0,     0,     0,   359,   360,     0,     0,     0,
       0,     0,     0,   250,     0,   245,     0,     0,     0,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,    50,
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
       0,   964,   249,     0,   359,   360,     0,     0,     0,     0,
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
     965,   249,     0,   359,   360,     0,     0,     0,     0,     0,
       0,     0,   245,     0,     0,     0,   250,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,    50,   242,     0,
       0,     0,     0,     0,     0,   361,   362,   363,   364,   365,
       0,   366,     0,   367,   368,   369,   370,   371,   372,   373,
     374,   375,    51,   376,   243,   244,     0,     0,     0,     0,
     246,   247,     0,     0,     0,   248,   411,   966,   249,   377,
     378,   379,     0,   380,     0,     0,   381,   382,   345,   346,
     347,   348,   349,   250,   412,     0,   383,     0,   384,   385,
     350,   311,   251,   413,     0,     0,     0,     0,     0,     0,
       0,     0,   351,     0,     0,     0,     0,   352,     0,     0,
     353,     0,     0,   354,   245,     0,     0,   355,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   356,   145,   146,
     147,     0,   149,   150,   151,   152,   153,   357,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,   165,   166,
     167,     0,     0,   170,   171,   172,   173,     0,     0,   358,
       0,     0,   246,   247,     0,     0,     0,   248,     0,   967,
     249,     0,   359,   360,     0,   242,     0,     0,     0,     0,
       0,   757,     0,     0,     0,   250,     0,   569,   570,     0,
       0,     0,     0,     0,   251,     0,    50,     0,     0,     0,
       0,   243,   244,     0,   361,   362,   363,   364,   365,     0,
     366,     0,   367,   368,   369,   370,   371,   372,   373,   374,
     375,    51,   376,     0,   345,   346,   347,   348,   349,     0,
       0,     0,     0,     0,     0,     0,   350,     0,   377,   378,
     379,   242,   380,     0,     0,   381,   382,     0,   351,     0,
       0,     0,     0,   352,     0,   383,   353,   384,   385,   354,
     311,   245,     0,   355,     0,     0,     0,   243,   244,     0,
       0,     0,     0,   356,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   357,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,   358,     0,     0,     0,   246,
     247,     0,     0,     0,   248,     0,  1068,   249,   359,   360,
       0,     0,     0,     0,     0,     0,   733,   245,     0,     0,
       0,     0,   250,     0,     0,     0,     0,     0,     0,     0,
       0,   251,    50,   242,     0,     0,     0,     0,     0,     0,
     361,   362,   363,   364,   365,     0,   366,     0,   367,   368,
     369,   370,   371,   372,   373,   374,   375,    51,   376,   243,
     244,     0,     0,     0,     0,  1019,   247,     0,     0,     0,
     248,     0,     0,   249,   377,   378,   379,     0,   380,     0,
       0,   381,   382,   345,   346,   347,   348,   349,   250,   674,
       0,   383,   675,   384,   385,   350,   311,   251,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,     0,     0,
       0,     0,   352,     0,     0,   353,     0,     0,   354,   245,
       0,     0,   355,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   356,   145,   146,   147,     0,   149,   150,   151,
     152,   153,   357,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   165,   166,   167,     0,     0,   170,   171,
     172,   173,     0,     0,   358,     0,     0,   246,   247,     0,
       0,     0,   248,     0,  1121,   249,     0,   359,   360,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     250,     0,   569,   570,     0,     0,     0,     0,     0,   251,
       0,    50,     0,     0,     0,     0,   243,   244,     0,   361,
     362,   363,   364,   365,     0,   366,     0,   367,   368,   369,
     370,   371,   372,   373,   374,   375,    51,   376,     0,   345,
     346,   347,   348,   349,     0,     0,     0,     0,     0,     0,
       0,   350,     0,   377,   378,   379,     0,   380,     0,     0,
     381,   382,     0,   351,     0,     0,     0,     0,   352,     0,
     383,   353,   384,   385,   354,   311,   245,     0,   355,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   356,   145,
     146,   147,     0,   149,   150,   151,   152,   153,   357,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   165,
     166,   167,     0,     0,   170,   171,   172,   173,     0,     0,
     358,     0,     0,     0,   246,   247,     0,     0,     0,   248,
       0,  1122,   249,   359,   360,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   250,   937,   938,
       0,     0,     0,     0,     0,     0,   251,    50,     0,     0,
       0,     0,   243,   244,     0,   361,   362,   363,   364,   365,
       0,   366,     0,   367,   368,   369,   370,   371,   372,   373,
     374,   375,    51,   376,     0,   345,   346,   347,   348,   349,
       0,     0,     0,     0,     0,     0,     0,   350,     0,   377,
     378,   379,   242,   380,     0,     0,   381,   382,     0,   351,
       0,     0,     0,     0,   352,     0,   383,   353,   384,   385,
     354,   311,   245,     0,   355,     0,     0,     0,   243,   244,
       0,     0,     0,     0,   356,   145,   146,   147,     0,   149,
     150,   151,   152,   153,   357,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   165,   166,   167,     0,     0,
     170,   171,   172,   173,     0,     0,   358,     0,     0,     0,
     246,   247,     0,     0,     0,   248,     0,  1150,   249,   359,
     360,     0,     0,     0,     0,     0,     0,     0,   245,     0,
       0,     0,     0,   250,   946,   947,     0,     0,     0,     0,
       0,     0,   251,    50,     0,     0,     0,     0,     0,     0,
       0,   361,   362,   363,   364,   365,     0,   366,     0,   367,
     368,   369,   370,   371,   372,   373,   374,   375,    51,   376,
       0,   345,   346,   347,   348,   349,   246,   247,     0,     0,
       0,   248,     0,   350,   249,   377,   378,   379,     0,   380,
       0,     0,   381,   382,     0,   351,     0,     0,     0,   250,
     352,     0,   383,   353,   384,   385,   354,   311,   251,     0,
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
     348,   349,     0,     0,   707,     0,     0,     0,     0,   350,
       0,   377,   378,   379,     0,   380,     0,     0,   381,   382,
       0,   351,     0,     0,     0,     0,   352,   414,   383,   353,
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
      51,   376,     0,   345,   346,   347,   348,   349,     0,     0,
       0,     0,     0,     0,     0,   350,     0,   377,   378,   379,
       0,   380,     0,     0,   381,   382,     0,   351,     0,     0,
       0,     0,   352,     0,   383,   353,   384,   385,   354,   311,
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
     383,   727,   384,   385,   350,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,     0,     0,     0,
       0,   352,     0,     0,   353,     0,     0,   354,     0,     0,
       0,   355,     0,     0,   754,     0,     0,     0,     0,     0,
       0,   356,   145,   146,   147,     0,   149,   150,   151,   152,
     153,   357,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   165,   166,   167,     0,     0,   170,   171,   172,
     173,     0,     0,   358,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   359,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,   361,   362,
     363,   364,   365,     0,   366,     0,   367,   368,   369,   370,
     371,   372,   373,   374,   375,    51,   376,     0,   345,   346,
     347,   348,   349,     0,     0,     0,     0,     0,     0,     0,
     350,     0,   377,   378,   379,     0,   380,     0,     0,   381,
     382,     0,   351,     0,     0,     0,     0,   352,     0,   383,
     353,   384,   385,   354,   311,     0,     0,   355,     0,     0,
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
     348,   349,     0,     0,     0,   383,   821,   384,   385,   350,
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
       0,     0,     0,     0,     0,   823,     0,   377,   378,   379,
       0,   380,     0,     0,   381,   382,   345,   346,   347,   348,
     349,     0,     0,     0,   383,     0,   384,   385,   350,   311,
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
       0,     0,     0,   383,   843,   384,   385,   350,   311,     0,
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
       0,     0,   383,   852,   384,   385,   350,   311,     0,     0,
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
       0,   383,  1005,   384,   385,   350,   311,     0,     0,     0,
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
     383,  1055,   384,   385,   350,   311,     0,     0,     0,     0,
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
    1107,   384,   385,   350,   311,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,     0,     0,     0,     0,
     352,     0,     0,   353,     0,     0,   354,     0,     0,     0,
     355,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     356,   145,   146,   147,     0,   149,   150,   151,   152,   153,
     357,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   359,   360,     0,     0,     0,
       0,     0,     0,     0,  1119,     0,     0,     0,     0,     0,
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
       0,   359,   360,   773,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   501,   502,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   361,   362,   363,   364,   365,     0,   366,
       0,   367,   368,   369,   370,   371,   372,   373,   374,   375,
      51,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
       0,   380,     0,     0,   381,   382,     0,     0,     0,     0,
       0,     0,     0,     0,   383,     0,   384,   385,     0,   311,
       0,     0,   774,   775,   776,   777,   778,   779,   780,   781,
     503,   504,   505,   506,   507,   782,   783,   508,   509,   510,
     511,   784,   512,   513,   514,   515,   501,   502,     0,   516,
     785,   517,   518,   786,   787,     0,     0,   519,   520,   521,
     788,   789,   790,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     791,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   503,   504,   505,   506,   507,     0,     0,   508,
     509,   510,   511,     0,   512,   513,   514,   515,   501,   502,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,     0,     0,
       0,     0,   311,     0,     0,     0,   774,   775,   776,   777,
     778,   779,   780,   781,   503,   504,   505,   506,   507,   782,
     783,   508,   509,   510,   511,   911,   512,   513,   514,   515,
     501,   502,     0,   516,   785,   517,   518,   786,   787,     0,
       0,   519,   520,   521,   788,   789,   790,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   912,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
       0,     0,     0,   921,     0,     0,     0,     0,   774,   775,
     776,   777,   778,   779,   780,   781,   503,   504,   505,   506,
     507,   782,   783,   508,   509,   510,   511,   911,   512,   513,
     514,   515,   501,   502,     0,   516,   785,   517,   518,   786,
     787,     0,     0,   519,   520,   521,   788,   789,   790,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   912,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
       0,     0,     0,     0,     0,   957,     0,     0,     0,     0,
     774,   775,   776,   777,   778,   779,   780,   781,   503,   504,
     505,   506,   507,   782,   783,   508,   509,   510,   511,   911,
     512,   513,   514,   515,   501,   502,     0,   516,   785,   517,
     518,   786,   787,     0,     0,   519,   520,   521,   788,   789,
     790,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   912,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,     0,     0,     0,     0,     0,  1023,     0,     0,
       0,     0,   774,   775,   776,   777,   778,   779,   780,   781,
     503,   504,   505,   506,   507,   782,   783,   508,   509,   510,
     511,   911,   512,   513,   514,   515,   501,   502,     0,   516,
     785,   517,   518,   786,   787,     0,     0,   519,   520,   521,
     788,   789,   790,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     912,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,     0,     0,     0,  1027,
       0,     0,     0,     0,   774,   775,   776,   777,   778,   779,
     780,   781,   503,   504,   505,   506,   507,   782,   783,   508,
     509,   510,   511,   911,   512,   513,   514,   515,   501,   502,
       0,   516,   785,   517,   518,   786,   787,     0,     0,   519,
     520,   521,   788,   789,   790,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   912,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,     0,     0,
       0,  1047,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   501,   502,   503,   504,   505,   506,   507,     0,
       0,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,   503,   504,
     505,   506,   507,  1140,     0,   508,   509,   510,   511,     0,
     512,   513,   514,   515,   501,   502,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,     0,     0,   536,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     503,   504,   505,   506,   507,   501,   502,   508,   509,   510,
     511,     0,   512,   513,   514,   515,     0,     0,     0,   516,
       0,   517,   518,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,   668,     0,     0,     0,
       0,   503,   504,   505,   506,   507,   501,   502,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,     0,     0,   519,   520,
     521,     0,     0,     0,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,     0,     0,   798,     0,     0,
       0,     0,   503,   504,   505,   506,   507,   501,   502,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,   826,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   501,   502,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,     0,     0,     0,
     519,   520,   521,     0,     0,     0,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,   829,
       0,     0,     0,     0,   503,   504,   505,   506,   507,   501,
     502,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
     831,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     501,   502,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,   833,     0,     0,     0,     0,   503,   504,   505,   506,
     507,   501,   502,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
       0,     0,     0,   519,   520,   521,     0,     0,     0,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
       0,     0,   834,     0,     0,     0,     0,   503,   504,   505,
     506,   507,   501,   502,   508,   509,   510,   511,     0,   512,
     513,   514,   515,     0,     0,     0,   516,     0,   517,   518,
       0,     0,     0,     0,   519,   520,   521,     0,     0,     0,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,   835,     0,     0,     0,     0,   503,   504,
     505,   506,   507,   501,   502,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,     0,     0,   836,     0,     0,     0,     0,   503,
     504,   505,   506,   507,   501,   502,   508,   509,   510,   511,
       0,   512,   513,   514,   515,     0,     0,     0,   516,     0,
     517,   518,     0,     0,     0,     0,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,     0,     0,   837,     0,     0,     0,     0,
     503,   504,   505,   506,   507,   501,   502,   508,   509,   510,
     511,     0,   512,   513,   514,   515,     0,     0,     0,   516,
       0,   517,   518,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,   838,     0,     0,     0,
       0,   503,   504,   505,   506,   507,   501,   502,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,     0,     0,   519,   520,
     521,     0,     0,     0,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,     0,     0,   898,     0,     0,
       0,     0,   503,   504,   505,   506,   507,   501,   502,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,   963,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   501,   502,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,     0,     0,     0,
     519,   520,   521,     0,     0,     0,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,  1017,
       0,     0,     0,     0,   503,   504,   505,   506,   507,   501,
     502,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
    1066,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     501,   502,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,  1067,     0,     0,     0,     0,   503,   504,   505,   506,
     507,   501,   502,   508,   509,   510,   511,     0,   512,   513,
     514,   515,     0,     0,     0,   516,     0,   517,   518,     0,
       0,     0,     0,   519,   520,   521,     0,     0,     0,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
       0,     0,  1077,     0,     0,     0,     0,   503,   504,   505,
     506,   507,   501,   502,   508,   509,   510,   511,     0,   512,
     513,   514,   515,     0,     0,     0,   516,     0,   517,   518,
       0,     0,     0,     0,   519,   520,   521,     0,     0,     0,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,     0,     0,  1079,     0,     0,     0,     0,   503,   504,
     505,   506,   507,   501,   502,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,     0,     0,  1081,     0,     0,     0,     0,   503,
     504,   505,   506,   507,   501,   502,   508,   509,   510,   511,
       0,   512,   513,   514,   515,     0,     0,     0,   516,     0,
     517,   518,     0,     0,     0,     0,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,     0,     0,  1085,     0,     0,     0,     0,
     503,   504,   505,   506,   507,   501,   502,   508,   509,   510,
     511,     0,   512,   513,   514,   515,     0,     0,     0,   516,
       0,   517,   518,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,  1124,     0,     0,     0,
       0,   503,   504,   505,   506,   507,   501,   502,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,     0,     0,   519,   520,
     521,     0,     0,     0,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,     0,     0,  1125,     0,     0,
       0,     0,   503,   504,   505,   506,   507,   501,   502,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,  1126,     0,
       0,     0,     0,   503,   504,   505,   506,   507,   501,   502,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,     0,     0,     0,
     519,   520,   521,     0,     0,     0,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,     0,  1137,
       0,     0,     0,     0,   503,   504,   505,   506,   507,   501,
     502,   508,   509,   510,   511,     0,   512,   513,   514,   515,
       0,     0,     0,   516,     0,   517,   518,     0,     0,     0,
       0,   519,   520,   521,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,     0,     0,
    1139,     0,     0,     0,     0,   503,   504,   505,   506,   507,
     501,   502,   508,   509,   510,   511,     0,   512,   513,   514,
     515,     0,     0,     0,   516,     0,   517,   518,     0,     0,
       0,     0,   519,   520,   521,     0,     0,     0,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   535,     0,
       0,  1167,     0,     0,     0,     0,   503,   504,   505,   506,
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
     535,   882,     0,     0,     0,     0,     0,     0,   503,   504,
     505,   506,   507,   501,   502,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
       0,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,   896,     0,     0,     0,     0,     0,     0,   503,
     504,   505,   506,   507,   501,   502,   508,   509,   510,   511,
       0,   512,   513,   514,   515,     0,     0,     0,   516,     0,
     517,   518,     0,     0,     0,     0,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,  1012,     0,     0,     0,     0,     0,     0,
     503,   504,   505,   506,   507,     0,     0,   508,   509,   510,
     511,     0,   512,   513,   514,   515,   501,   502,     0,   516,
       0,   517,   518,     0,     0,     0,     0,   519,   520,   521,
       0,     0,     0,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,  1015,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   774,   775,   776,   777,   778,   779,
     780,   781,   503,   504,   505,   506,   507,   782,   783,   508,
     509,   510,   511,   911,   512,   513,   514,   515,  -288,   256,
     257,   516,   785,   517,   518,   786,   787,     0,     0,   519,
     520,   521,   788,   789,   790,   522,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   912,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   501,   502,   277,   278,   279,
       0,     0,     0,     0,     0,   280,   281,   282,   283,   284,
       0,     0,   285,   286,   287,   288,   289,   290,   291,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   292,     0,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,     0,     0,
     303,   304,     0,     0,     0,   680,     0,   305,   306,     0,
       0,     0,   501,   502,     0,     0,     0,     0,     0,     0,
       0,   503,   504,   505,   506,   507,     0,     0,   508,   509,
     510,   511,     0,   512,   513,   514,   515,     0,     0,     0,
     516,     0,   517,   518,     0,     0,   661,     0,   519,   520,
     521,   145,   146,   147,   522,   149,   150,   151,   152,   153,
     357,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   165,   166,   167,     0,     0,   170,   171,   172,   173,
       0,     0,   523,     0,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,     0,     0,   501,   502,   503,   504,
     505,   506,   507,   534,   535,   508,   509,   510,   511,     0,
     512,   513,   514,   515,     0,     0,     0,   516,     0,   517,
     518,     0,     0,     0,     0,   519,   520,   521,     0,     0,
       0,   522,     0,     0,   681,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   682,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
     887,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,     0,     0,   501,   502,     0,     0,     0,     0,     0,
     534,   535,   503,   504,   505,   506,   507,     0,     0,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,   519,
     520,   521,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   986,
       0,     0,     0,   523,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,   501,   502,   503,
     504,   505,   506,   507,   534,   535,   508,   509,   510,   511,
       0,   512,   513,   514,   515,     0,     0,     0,   516,     0,
     517,   518,     0,     0,     0,     0,   519,   520,   521,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,     0,   501,   502,     0,     0,     0,     0,
       0,   534,   535,   503,   504,   505,   506,   507,     0,     0,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,     0,   501,   502,     0,
     519,   520,   521,     0,     0,     0,  -617,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
     503,   504,   505,   506,   507,   534,   535,   508,   509,   510,
     511,     0,   512,   513,   514,   515,     0,     0,     0,   516,
       0,   517,   518,     0,   501,   502,     0,   519,   520,   521,
       0,     0,     0,   503,   504,   505,   506,   507,     0,     0,
     508,   509,   510,   511,     0,   512,   513,   514,   515,     0,
       0,     0,   516,     0,   517,   518,   501,   502,     0,     0,
     519,   523,   521,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,     0,     0,     0,     0,
     503,   504,   505,   506,   507,   534,   535,   508,   509,   510,
     511,     0,   512,   513,   514,   515,     0,     0,     0,   516,
       0,   517,   518,   501,   502,     0,     0,   519,     0,     0,
       0,     0,   503,   504,   505,   506,   507,     0,     0,   508,
     509,   510,   511,     0,   512,   513,   514,   515,     0,     0,
       0,   516,     0,   517,   518,     0,     0,     0,     0,     0,
       0,     0,     0,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,     0,     0,     0,     0,   503,
     504,   505,   506,   507,   534,   535,   508,   509,   510,   511,
       0,   512,   513,   514,   515,     0,     0,     0,   516,     0,
     517,   518,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   141,
       0,     0,     0,     0,     0,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,   143,     0,     0,     0,     0,     0,     0,     0,
       0,   534,   535,     0,     0,     0,     0,   144,     0,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   141,     0,     0,     0,     0,     0,   142,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,   143,     0,   684,     0,
     176,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   144,    51,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   145,   146,   147,     0,   149,   150,
     151,   152,   153,   357,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,   883,     0,
       0,     0,     0,     0,   176,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   685,     0,     0,
       0,     0,     0,     0,   145,   146,   147,   686,   149,   150,
     151,   152,   153,   357,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   165,   166,   167,     0,     0,   170,
     171,   172,   173,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   141,     0,
       0,     0,     0,     0,   142,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,     0,     0,     0,     0,     0,   884,     0,     0,
       0,     0,     0,     0,     0,     0,   144,   885,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   141,     0,     0,     0,     0,     0,   142,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,     0,   143,     0,     0,     0,   176,
       0,     0,   367,   368,   369,     0,     0,     0,     0,     0,
     144,    51,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   141,     0,     0,     0,
       0,     0,   142,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,     0,   143,
       0,     0,     0,   176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   144,    51,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   176,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458
};

static const yytype_int16 yycheck[] =
{
      12,   111,   216,   128,   430,   544,   548,   338,   485,   340,
     453,   342,   455,   369,   457,   394,   557,   651,   419,    14,
      15,     7,   674,   111,   537,   659,    32,   568,   437,    51,
       0,    21,   569,   570,    81,    51,     6,    48,   122,    19,
      19,   131,    19,    18,    19,    33,   482,  1034,   484,    20,
      21,   157,   157,   116,   157,   157,   119,   120,   157,    29,
      44,    31,   195,    33,   197,    55,   119,   120,   609,    39,
      82,    14,    15,    61,   157,    44,    45,   157,    48,    20,
      21,   151,   119,   120,    54,   144,   157,   132,   191,   148,
     196,   117,   197,   117,   147,   197,   141,    41,   197,   111,
     126,   146,   126,    73,   157,  1092,   196,   132,   645,   165,
     117,   195,   157,    97,   197,   628,   191,   197,   159,   160,
     556,   196,   193,   117,    94,   188,   189,   172,    97,   168,
     141,    75,   126,   192,   157,   188,   189,   108,   109,   195,
     166,   157,   166,   168,   132,   116,   157,   118,   119,   120,
     121,   188,   189,    60,   125,   178,   151,   196,   165,   166,
     155,   168,   157,   158,   171,   171,   166,   108,   109,   157,
     166,  1133,   166,   195,   165,   116,   223,   118,   119,   120,
     121,   590,   157,   189,   125,   165,   165,   177,   165,    20,
      21,   173,   198,   845,  1156,   195,   191,   738,   188,   324,
     165,   117,   743,   189,   195,   176,   177,   178,   151,   843,
     126,   197,   155,   195,   157,   158,   757,   188,   189,   189,
     599,   331,   332,   333,   334,   195,   627,   337,   165,   339,
     195,   341,   144,   343,   771,   165,   148,   347,    32,     4,
       5,   165,     7,   331,   332,   333,   334,   188,   189,   337,
     166,   339,  1117,   341,   117,   171,   165,   367,   368,   347,
     165,   802,   192,   126,    58,    59,   165,   132,   192,   132,
      35,   165,   165,   173,   165,   165,   141,   108,   109,   367,
     368,  1146,   165,   192,   132,   116,   165,   192,   119,   120,
     121,   165,   157,   192,   125,   195,    55,   168,   192,   192,
     190,   141,    61,   166,   147,   157,   662,   190,   418,   157,
     168,   190,    20,    21,   157,   173,   190,   157,   428,   331,
     332,   333,   334,   168,   118,   337,   839,   339,   173,   341,
     418,   343,   442,   191,   147,   347,   165,    55,   851,   973,
     428,   159,   160,    61,   157,   559,   191,   168,   679,   983,
      20,    21,   173,   567,   442,   367,   368,   188,   189,   168,
     902,   190,   173,   473,   173,    68,   843,   880,   380,    72,
     191,   147,   166,   167,   173,   852,   157,   171,   141,   173,
     174,   157,   191,   157,   195,   473,    89,    90,    91,    92,
     157,   501,   502,   906,   157,   189,   195,   132,   106,   107,
     108,   109,   110,    11,   198,   113,   418,   195,   116,   197,
     118,   119,   120,   121,    22,    23,   428,   125,   165,   127,
     128,   168,   434,    55,   171,    55,   151,   191,    55,    61,
     442,    61,   875,   975,    61,    55,   106,   107,   108,   109,
     190,    61,   188,   157,   192,   195,   116,   195,   118,   119,
     120,   121,   157,   465,   166,   125,   969,   127,   128,   193,
     194,   473,   196,   157,   172,   173,   174,   175,   176,   177,
     178,   159,   160,   161,   162,   195,   157,    20,    21,   196,
     188,   189,   159,   160,   161,   497,   157,   196,    55,   195,
     157,    97,   196,   148,   172,   191,   172,   172,   172,   172,
     172,   191,   612,   172,   174,   175,   176,   177,   178,   172,
     172,   172,   195,   623,   157,   625,    34,    34,   188,   189,
     630,   157,   632,   633,   612,   635,   740,   191,    32,   197,
     196,   157,    32,   864,   168,   623,   750,   195,   190,    21,
     190,   192,   630,   168,   632,   633,   168,   635,   991,   196,
     196,   191,   766,    41,    58,    59,   191,   172,   191,   172,
     172,   172,  1101,   106,   107,   108,   109,   110,   191,   172,
     113,   114,   115,   116,   191,   118,   119,   120,   121,    11,
     194,   191,   125,   190,   127,   128,   191,   191,    20,    21,
     191,   191,   191,   189,   191,   157,   191,   707,   190,   157,
     612,   173,  1028,   148,   157,   191,    41,   157,   195,   151,
      36,   623,   195,   625,   118,   195,    20,    21,   630,   195,
     632,   633,   195,   635,    32,   195,   736,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    41,   157,   173,   173,
     173,   165,   190,   165,   159,   188,   189,   157,   736,   172,
      58,    59,   191,   197,   191,   191,   172,     1,   157,   241,
     191,   191,   166,   167,   191,   191,   157,   171,   250,   122,
     174,    13,   797,   166,   106,   107,   108,   109,   110,   168,
     151,   113,   114,   115,   116,   189,   118,   119,   120,   121,
     196,   195,   194,   125,   198,   127,   128,     7,   157,   157,
     914,   133,   134,   135,   108,   109,   191,   139,   157,   190,
     118,   190,   116,   190,   118,   119,   120,   121,   190,   197,
     159,   125,   172,   157,   736,   191,   157,   157,   157,   191,
     191,   195,    41,   157,   191,   167,   190,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   190,   190,   195,
     190,   192,   190,   863,   157,   173,   188,   189,   166,   167,
     197,    25,   190,   171,    32,   173,   174,   173,   195,   190,
     174,   175,   176,   177,   178,   863,   190,   359,   360,   195,
      25,   189,   794,   190,   188,   189,  1000,   369,    34,    51,
     198,   489,   190,   195,   190,   377,   378,   379,   197,   381,
     382,   383,    63,    37,   108,  1132,    66,    64,   228,    82,
     923,   794,     1,   998,   832,  1029,  1042,  1045,    64,   550,
      66,   625,    68,    69,    70,    71,    72,   605,    74,    75,
      76,    77,    78,    79,    80,    81,    82,   344,    84,    85,
      86,    45,   431,    89,    90,    91,    92,   850,   430,   336,
     347,   863,  1115,    -1,   485,    -1,    -1,   439,    -1,    -1,
      -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1089,    -1,    -1,    -1,    -1,
      -1,    -1,  1096,    32,    -1,   467,    -1,    -1,   470,    -1,
     472,  1001,    -1,  1003,   476,   477,   478,   479,   480,   481,
      -1,   483,    -1,   485,    -1,    -1,    -1,    -1,    -1,    58,
      59,   157,    -1,  1001,    -1,  1003,    -1,    -1,    -1,    -1,
      -1,   503,   504,    -1,    -1,   507,   508,   509,   510,    -1,
     512,    -1,   514,   515,   516,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,  1166,   535,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   543,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,   553,  1082,    -1,    -1,   557,    -1,   126,   560,   561,
      -1,    -1,    -1,    -1,   566,    -1,   568,    -1,    -1,  1001,
      -1,  1003,    -1,   575,  1082,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   596,    -1,    -1,   166,   167,    -1,
      -1,    -1,   171,    -1,    -1,   174,    -1,   609,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,   195,   629,    -1,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   647,   648,    -1,    -1,    -1,
    1082,    -1,    -1,    -1,    -1,    -1,   658,    -1,    -1,   661,
     662,    -1,    -1,   665,    -1,    -1,    -1,   106,   107,    -1,
      -1,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,  1117,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1146,    20,    21,    -1,   720,    -1,
      -1,    -1,   724,    -1,    -1,    -1,    -1,    -1,   167,   731,
     169,   170,   171,   172,   173,    -1,   738,    -1,   177,    -1,
      -1,   743,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,    -1,    -1,    -1,    -1,   757,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   774,   775,   776,   777,   778,   779,   780,   781,
     782,   783,   784,   785,   786,   787,   788,   789,   790,   791,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     802,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   841,
     842,    -1,    -1,    -1,    -1,    -1,   848,    -1,   850,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   861,
     862,    -1,    -1,    -1,    -1,   867,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,   877,    -1,   879,    -1,   881,
      -1,    32,    -1,   188,   189,   887,    -1,    -1,    -1,   891,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   907,    -1,    58,    59,   911,
     912,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   926,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,   937,   938,    -1,   125,    -1,
     127,   128,    32,    -1,   946,   947,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    58,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,    -1,    -1,    -1,    -1,  1009,   195,  1011,
      -1,    -1,  1014,    -1,    -1,   166,   167,    -1,  1020,    -1,
     171,    -1,  1024,   174,  1026,    -1,  1028,    -1,   118,    -1,
    1032,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
     191,    -1,    -1,    -1,    -1,    -1,  1048,   198,    -1,  1051,
    1052,  1053,  1054,    -1,    -1,  1057,    -1,    -1,  1060,  1061,
      32,  1063,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1073,    -1,    -1,    -1,    -1,   166,   167,    -1,    -1,
      -1,   171,    -1,   173,   174,    -1,    58,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,   189,
      -1,     6,    -1,     8,     9,    10,    -1,    12,   198,    14,
      15,    16,    17,    18,    -1,    -1,    -1,  1119,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,  1128,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,  1138,    42,    43,    -1,
      -1,    46,    -1,    48,    49,    50,   118,    52,    53,    -1,
    1152,    56,    57,    -1,    -1,    -1,  1158,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    96,    -1,    -1,   166,   167,    -1,    -1,    -1,   171,
      -1,   173,   174,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,   198,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,    -1,   178,    -1,    -1,   181,   182,    -1,    -1,
      -1,    -1,    -1,    58,    59,    -1,   191,    -1,   193,   194,
     195,   196,   197,     1,    -1,    -1,    -1,    -1,     6,    -1,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    -1,    42,    43,    -1,    -1,    46,    -1,
      48,    49,    50,   118,    52,    53,    -1,    -1,    56,    57,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    96,    -1,
      -1,   166,   167,    -1,    -1,    -1,   171,    -1,   173,   174,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,   198,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,    -1,
     178,    -1,    -1,   181,   182,    -1,    -1,    -1,    -1,    58,
      59,    -1,    -1,   191,    -1,   193,   194,   195,   196,   197,
       9,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,   167,   108,
     109,     9,   171,    -1,   173,   174,    -1,    -1,   117,    -1,
      -1,    -1,    20,    21,   123,   124,    -1,    -1,    -1,    -1,
     189,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,   198,
      -1,   140,   141,   142,   143,   144,    -1,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,    -1,   178,
      -1,    -1,   181,   182,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,    -1,   193,   194,   195,   196,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,    -1,    32,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,   167,
      -1,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
     188,   189,    -1,    26,    -1,    -1,    -1,   195,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   118,    46,    -1,    -1,    49,    -1,    51,    -1,
      53,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
     166,   167,    95,    -1,    -1,   171,    -1,   173,   174,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,   132,
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
     174,    -1,   108,   109,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,   189,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,   198,    -1,   132,    -1,    -1,    -1,
      -1,    58,    59,    -1,   140,   141,   142,   143,   144,    -1,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   174,   175,
     176,    32,   178,    -1,    -1,   181,   182,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   191,    46,   193,   194,    49,
     196,   118,    -1,    53,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,   166,
     167,    -1,    -1,    -1,   171,    -1,   173,   174,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198,   132,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    58,
      59,    -1,    -1,    -1,    -1,   166,   167,    -1,    -1,    -1,
     171,    -1,    -1,   174,   174,   175,   176,    -1,   178,    -1,
      -1,   181,   182,    14,    15,    16,    17,    18,   189,   189,
      -1,   191,   192,   193,   194,    26,   196,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,   118,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    -1,    -1,   166,   167,    -1,
      -1,    -1,   171,    -1,   173,   174,    -1,   108,   109,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,   198,
      -1,   132,    -1,    -1,    -1,    -1,    58,    59,    -1,   140,
     141,   142,   143,   144,    -1,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   174,   175,   176,    -1,   178,    -1,    -1,
     181,   182,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     191,    46,   193,   194,    49,   196,   118,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,    -1,   166,   167,    -1,    -1,    -1,   171,
      -1,   173,   174,   108,   109,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,   198,   132,    -1,    -1,
      -1,    -1,    58,    59,    -1,   140,   141,   142,   143,   144,
      -1,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   174,
     175,   176,    32,   178,    -1,    -1,   181,   182,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   191,    46,   193,   194,
      49,   196,   118,    -1,    53,    -1,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,
     166,   167,    -1,    -1,    -1,   171,    -1,   173,   174,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,   189,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,   198,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,    14,    15,    16,    17,    18,   166,   167,    -1,    -1,
      -1,   171,    -1,    26,   174,   174,   175,   176,    -1,   178,
      -1,    -1,   181,   182,    -1,    38,    -1,    -1,    -1,   189,
      43,    -1,   191,    46,   193,   194,    49,   196,   198,    -1,
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
      17,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,
      -1,   174,   175,   176,    -1,   178,    -1,    -1,   181,   182,
      -1,    38,    -1,    -1,    -1,    -1,    43,   190,   191,    46,
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
     157,   158,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   174,   175,   176,
      -1,   178,    -1,    -1,   181,   182,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   191,    46,   193,   194,    49,   196,
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
      -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   172,    -1,   174,   175,   176,
      -1,   178,    -1,    -1,   181,   182,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   191,    -1,   193,   194,    26,   196,
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
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
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
      -1,   108,   109,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
      -1,   178,    -1,    -1,   181,   182,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,    -1,   193,   194,    -1,   196,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    20,    21,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,   167,    -1,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    20,    21,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,
      -1,    -1,   196,    -1,    -1,    -1,    98,    99,   100,   101,
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
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    20,    21,
      -1,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,
      -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,   106,   107,
     108,   109,   110,   195,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    20,    21,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,   189,    -1,    -1,   192,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     134,   135,   136,   137,   138,   139,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    20,    21,   118,   119,   120,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
     181,   182,    -1,    -1,    -1,    18,    -1,   188,   189,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,   131,    -1,   133,   134,
     135,    64,    65,    66,   139,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    20,    21,   106,   107,
     108,   109,   110,   188,   189,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
     188,   189,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    20,    21,   106,
     107,   108,   109,   110,   188,   189,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,   188,   189,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    20,    21,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   188,   189,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    20,    21,    -1,   133,   134,   135,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    20,    21,    -1,    -1,
     133,   167,   135,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   188,   189,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    20,    21,    -1,    -1,   133,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   188,   189,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,   173,   174,   175,   176,
     177,   178,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,    -1,    -1,    -1,    -1,    62,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    18,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,   157,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    66,   157,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,   157,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,   145,
      -1,    -1,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,
      62,   157,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,   157,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   200,     0,     6,    29,    31,    33,    39,    48,    54,
      73,    94,   189,   195,   203,   211,   212,   216,   244,   248,
     267,   334,   340,   343,   349,   387,   390,    18,    19,   157,
     238,   239,   240,   151,   217,   218,   157,   178,   213,   214,
     157,   196,   337,   157,   193,   202,   391,   388,    33,    61,
     132,   157,   241,   242,   243,   257,     4,     5,     7,    35,
     347,    60,   332,   166,   165,   168,   165,   213,    21,    55,
     177,   188,   215,   338,   337,   339,   332,   157,   157,   157,
     132,   191,   165,   190,    55,    61,   249,   251,    55,    61,
     341,    55,    61,   348,    55,    61,   333,    14,    15,   151,
     155,   157,   158,   191,   205,   239,   151,   218,   157,   157,
     157,   166,   195,   197,   337,    55,    61,   201,   196,   389,
     157,   240,   242,   252,   157,   342,   350,   196,   335,   159,
     160,   204,    14,    15,   151,   155,   157,   205,   236,   237,
     215,    24,    30,    47,    62,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   145,   257,   353,   355,
     356,   358,   362,   195,   157,   357,   196,   192,    34,    64,
      66,    68,    69,    70,    71,    72,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    84,    85,    86,    89,    90,
      91,    92,   108,   109,   157,   247,   250,   168,   196,    97,
     345,   346,   330,   148,   309,   159,   160,   161,   165,   192,
     172,   172,   172,   172,   191,   172,   172,   172,   172,   172,
     172,   191,    32,    58,    59,   118,   166,   167,   171,   174,
     189,   198,   195,   157,   322,   323,    20,    21,    37,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   118,   119,   120,
     126,   127,   128,   129,   130,   133,   134,   135,   136,   137,
     138,   139,   167,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   181,   182,   188,   189,    34,    34,   191,
     245,   196,   253,    68,    72,    89,    90,    91,    92,   354,
     336,   157,   351,   197,   331,   240,   141,   157,   328,   329,
     236,   365,   367,   369,   363,   157,   359,   371,   373,   375,
     377,   379,   381,   383,   385,    14,    15,    16,    17,    18,
      26,    38,    43,    46,    49,    53,    63,    73,    95,   108,
     109,   140,   141,   142,   143,   144,   146,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   158,   174,   175,   176,
     178,   181,   182,   191,   193,   194,   207,   209,   253,   257,
     259,   269,   276,   279,   282,   286,   287,   290,   291,   293,
     298,   301,   308,   353,   392,   396,   401,   403,   405,    32,
      32,   171,   189,   198,   190,   301,   157,   197,   168,   195,
     190,    21,   190,   144,   192,   309,   318,   319,   168,   246,
     255,   196,   157,   197,   168,   344,   196,   309,   190,   191,
      41,   165,   168,   171,   327,   362,   361,   362,   362,   362,
     192,   357,   362,   245,   362,   245,   362,   245,   157,   320,
     321,   362,   323,   362,   392,   191,   172,   191,   172,   172,
     191,   172,   191,   172,   301,   301,   191,   191,   191,   191,
     191,   191,    12,   362,    12,   362,   301,   398,   402,   206,
     301,   301,   301,   257,   301,   301,   301,   194,   157,   191,
     244,    20,    21,   106,   107,   108,   109,   110,   113,   114,
     115,   116,   118,   119,   120,   121,   125,   127,   128,   133,
     134,   135,   139,   167,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   188,   189,   192,   191,   190,   190,
     362,   197,   322,   191,    48,   141,   157,   325,   352,   192,
     195,   362,     1,     8,     9,    10,    12,    25,    27,    28,
      37,    40,    42,    50,    52,    56,    57,    63,    96,   123,
     124,   195,   197,   219,   220,   223,   228,   230,   231,   232,
     233,   234,   254,   256,   258,   260,   261,   262,   265,   266,
     267,   268,   288,   289,   301,   336,   166,   195,   257,   314,
     329,   190,   301,   157,   157,   362,   117,   126,   166,   326,
     173,   173,   195,   173,   173,   173,   195,   173,   246,   173,
     246,   173,   246,   168,   173,   195,   173,   195,   191,   257,
     277,   301,   270,   272,   301,   274,   301,   362,   301,   301,
     301,   301,   301,   301,   352,    51,   157,   174,   191,   301,
     393,   394,   395,   397,   398,   399,   400,   352,   191,   394,
     400,   131,   195,   197,   161,   162,   204,   210,   192,   141,
     146,   172,   257,   292,   189,   192,   280,   301,   148,   285,
      18,   147,   157,   353,    18,   147,   157,   353,   301,   301,
     301,   301,   301,   301,   157,   301,   147,   157,   301,   301,
     301,   301,   301,   301,   301,   301,   301,    21,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   119,
     120,   147,   157,   188,   189,   299,   301,   192,   280,   301,
     325,   191,    41,   117,   165,   166,   168,   171,   324,   318,
     301,   352,     9,   117,   195,   256,   288,   301,   253,   301,
     301,   157,   195,   151,    56,   301,   253,   117,   256,   301,
     194,   286,   286,    36,   195,   195,   301,   195,   195,   195,
     329,   122,   195,     9,    98,    99,   100,   101,   102,   103,
     104,   105,   111,   112,   117,   126,   129,   130,   136,   137,
     138,   166,   197,   301,   189,   197,   244,   315,   192,    41,
     166,   195,   326,   256,   301,   366,   368,   362,   370,   364,
     360,   372,   173,   376,   173,   380,   173,   362,   384,   320,
     386,   192,   280,   172,   301,   362,   192,   362,   362,   192,
     362,   192,   173,   192,   192,   192,   192,   192,   192,    19,
     286,   126,   324,   192,   165,   195,   395,   190,   165,   190,
     195,    19,   192,   395,   197,   301,   398,   197,   301,   159,
     208,   191,   191,   294,   296,   157,   393,   165,   192,   117,
     126,   166,   171,   283,   284,   245,   172,   191,   172,   191,
     191,   191,   190,    18,   147,   157,   353,   168,   147,   157,
     301,   191,   147,   157,   301,     1,   190,   192,   192,   301,
     157,   157,   362,   256,   301,   253,    19,   263,   256,   301,
       9,   117,   166,   195,    13,   253,   166,   168,   151,   256,
     301,   195,   194,   196,   253,   286,   229,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   123,   124,   301,
     301,   301,   301,   301,   301,   301,   123,   124,   301,   195,
     243,     7,   309,   313,   157,   256,   301,   195,   374,   378,
     382,   192,   157,   192,   173,   173,   173,   173,   285,   191,
     280,   301,   301,   394,   395,   157,   393,   190,   190,   301,
     190,   399,   280,   394,   197,   190,   163,   159,   301,   301,
     362,   245,   190,   301,   157,   157,   157,   157,   165,   190,
     246,   302,   301,   304,   301,   192,   280,   301,   172,   191,
     301,   191,   190,   301,   191,   190,   300,   192,    41,   166,
     324,   280,   301,   195,   264,   253,   235,   195,   255,    11,
      22,    23,   221,   222,   301,   301,   301,   301,   301,   190,
      55,    61,   312,    44,    97,   310,   195,   195,   173,   195,
     278,   271,   273,   275,   191,   192,   280,   195,   395,   190,
     126,   324,   190,   195,   395,   190,   192,   192,   173,   246,
     192,   283,   190,   131,   253,   281,   362,   192,   362,   192,
     192,   192,   306,   301,   301,   192,   301,   157,   301,   195,
      11,   225,   301,   301,   197,   253,   301,   225,   195,    45,
     310,   312,   301,   157,   301,   301,   301,   192,   301,   192,
     301,   190,   190,   301,   301,   301,   197,   295,   173,   117,
     301,   173,   173,   362,   192,   192,   192,   253,   226,   227,
     195,   225,   253,   316,    32,   311,   325,   192,   173,   192,
     195,   404,   190,   404,   190,   292,   297,   301,   303,   305,
     173,   301,    25,   195,   222,   250,   317,   301,    51,   190,
     197,   292,   307,   224,   301,   195,   250,   192,   301,   190,
     190,   253
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
     224,   225,   226,   225,   227,   225,   228,   229,   228,   230,
     231,   232,   233,   235,   234,   236,   236,   236,   236,   236,
     236,   237,   237,   238,   238,   238,   239,   239,   239,   239,
     239,   239,   239,   239,   240,   240,   241,   241,   241,   242,
     242,   243,   243,   244,   244,   245,   245,   245,   246,   246,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   248,   249,   249,
     249,   250,   252,   251,   253,   253,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   255,   255,   255,   256,
     256,   256,   257,   257,   257,   258,   259,   259,   259,   259,
     260,   261,   262,   262,   262,   262,   262,   263,   262,   264,
     262,   265,   265,   265,   265,   266,   267,   267,   268,   270,
     271,   269,   272,   273,   269,   274,   275,   269,   277,   278,
     276,   279,   279,   279,   280,   280,   281,   281,   281,   282,
     282,   282,   283,   283,   283,   283,   284,   284,   285,   285,
     286,   286,   287,   287,   287,   287,   287,   287,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   289,   289,   289,
     289,   290,   291,   291,   292,   292,   293,   294,   295,   293,
     296,   297,   293,   298,   298,   299,   300,   298,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   302,   303,   301,
     301,   301,   301,   304,   305,   301,   301,   301,   306,   307,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   309,   309,
     310,   310,   310,   311,   311,   312,   312,   312,   313,   314,
     315,   314,   316,   314,   317,   314,   314,   318,   318,   318,
     319,   319,   320,   320,   321,   321,   322,   323,   323,   324,
     324,   325,   325,   325,   325,   325,   325,   326,   326,   326,
     327,   327,   328,   328,   328,   328,   328,   329,   329,   329,
     329,   329,   330,   331,   330,   332,   332,   333,   333,   333,
     334,   335,   334,   336,   336,   336,   338,   337,   339,   339,
     340,   340,   341,   341,   341,   342,   343,   343,   344,   344,
     345,   345,   346,   347,   347,   348,   348,   348,   350,   351,
     349,   352,   352,   352,   352,   352,   353,   353,   353,   353,
     353,   353,   353,   353,   353,   353,   353,   353,   353,   353,
     353,   353,   353,   353,   353,   353,   353,   353,   353,   353,
     353,   354,   354,   354,   354,   354,   354,   355,   356,   356,
     356,   357,   357,   359,   360,   358,   361,   361,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   363,   364,   362,   362,   365,
     366,   362,   367,   368,   362,   369,   370,   362,   362,   371,
     372,   362,   373,   374,   362,   362,   375,   376,   362,   377,
     378,   362,   362,   379,   380,   362,   381,   382,   362,   383,
     384,   362,   385,   386,   362,   388,   389,   387,   391,   390,
     392,   392,   392,   392,   393,   393,   393,   393,   394,   394,
     395,   395,   396,   396,   396,   396,   396,   396,   397,   397,
     398,   398,   399,   399,   400,   400,   401,   401,   402,   402,
     403,   404,   404,   405,   405
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
       1,     0,     0,     3,     0,     4,     4,     0,     5,     6,
       2,     3,     3,     0,     5,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     1,     5,     1,     3,     1,     1,     1,     1,
       4,     1,     3,     0,     3,     0,     2,     3,     0,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     2,     2,     2,     2,
       2,     2,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       1,     3,     0,     4,     3,     7,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     2,     2,     2,     0,     2,     2,     3,
       2,     2,     1,     3,     2,     2,     2,     4,     5,     2,
       1,     1,     2,     3,     4,     2,     3,     0,     6,     0,
       7,     3,     4,     2,     3,     4,     1,     1,     2,     0,
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

    case YYSYMBOL_optional_return_result: /* optional_return_result  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_else_one_liner: /* expression_else_one_liner  */
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

  case 59: /* optional_return_result: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 60: /* optional_return_result: expr  */
                     { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 61: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 62: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 63: /* expression_else_one_liner: "else" $@2 expr  */
                                                                                   {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 64: /* $@3: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 65: /* expression_else_one_liner: "else" $@3 "return" optional_return_result  */
                                                                                                              {
            (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 66: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 67: /* $@4: %empty  */
                                  { yyextra->das_need_oxford_comma = true; }
    break;

  case 68: /* expression_if_then_else: expr "if" $@4 expr expression_else_one_liner  */
                                                                                                                            {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 69: /* expression_for_loop: "for" variable_name_with_pos_list "in" expr_list ';' expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 70: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 71: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 72: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 73: /* $@5: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 74: /* expression_with_alias: "assume" "name" '=' $@5 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 75: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 76: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 77: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 78: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 79: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 80: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 81: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 82: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 83: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 84: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 85: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 86: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 87: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 88: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 89: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 90: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 91: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 92: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 93: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 94: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 95: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 96: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 97: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 98: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 99: /* annotation_declaration: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
    break;

  case 100: /* annotation_declaration: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 101: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 102: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 103: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 104: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 105: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 106: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 107: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 108: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 109: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 110: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 111: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 112: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 113: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 114: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 115: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 116: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 117: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 118: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 119: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 120: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 121: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 122: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 123: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 124: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 125: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 126: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 127: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 128: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 129: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 130: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 131: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 132: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 133: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 134: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 135: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 136: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 137: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 138: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 139: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 140: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 141: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 142: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 143: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 144: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 145: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 146: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 147: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 148: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 149: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 150: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 151: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 152: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 153: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 154: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 155: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 156: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 157: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 158: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 159: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 160: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 161: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 162: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 163: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 164: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 165: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 166: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 167: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 168: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 169: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 170: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 171: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 172: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 173: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 174: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 175: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 176: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 177: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 178: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 179: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 180: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 181: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 182: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 183: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 184: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 185: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 186: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 187: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 188: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 189: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 190: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 191: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 192: /* $@6: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 193: /* function_declaration: optional_public_or_private_function $@6 function_declaration_header expression_block  */
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

  case 194: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 195: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 196: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 197: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 198: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 199: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 200: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 201: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 202: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 203: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 204: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 205: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 206: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 207: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 208: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 209: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 210: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 211: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 212: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 213: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 214: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 215: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 216: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 217: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 218: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 219: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 220: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 221: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 222: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 223: /* name_in_namespace: "name" "::" "name"  */
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

  case 224: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 225: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 226: /* expr_new: "new" type_declaration  */
                                                   {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 227: /* expr_new: "new" type_declaration '(' ')'  */
                                                           {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 228: /* expr_new: "new" type_declaration '(' expr_list ')'  */
                                                                                {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 229: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 230: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 231: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 232: /* expression_return: "return" ';'  */
                            {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),nullptr);
    }
    break;

  case 233: /* expression_return: "return" expr ';'  */
                                          {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 234: /* expression_return: "return" "<-" expr ';'  */
                                                 {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 235: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 236: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 237: /* $@7: %empty  */
                                      { yyextra->das_need_oxford_comma=true; }
    break;

  case 238: /* expression_return: "return" "if" $@7 expr expression_else_one_liner ';'  */
                                                                                                                                  {
        (yyval.pExpression) = new ExprIfThenElse ( tokAt(scanner,(yylsp[-4])), (yyvsp[-2].pExpression),
            make_smart<ExprReturn>(tokAt(scanner,(yylsp[-5])),nullptr), ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 239: /* $@8: %empty  */
                                                    { yyextra->das_need_oxford_comma=true; }
    break;

  case 240: /* expression_return: "return" expr "if" $@8 expr expression_else_one_liner ';'  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprIfThenElse ( tokAt(scanner,(yylsp[-4])), (yyvsp[-2].pExpression),
            make_smart<ExprReturn>(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-5].pExpression))), ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 241: /* expression_yield: "yield" expr ';'  */
                                         {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 242: /* expression_yield: "yield" "<-" expr ';'  */
                                                {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
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

  case 249: /* $@9: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 250: /* $@10: %empty  */
                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 251: /* expr_cast: "cast" '<' $@9 type_declaration '>' $@10 expr  */
                                                                                                                                     {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 252: /* $@11: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 253: /* $@12: %empty  */
                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 254: /* expr_cast: "upcast" '<' $@11 type_declaration '>' $@12 expr  */
                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 255: /* $@13: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 256: /* $@14: %empty  */
                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 257: /* expr_cast: "reinterpret" '<' $@13 type_declaration '>' $@14 expr  */
                                                                                                                                            {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 258: /* $@15: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 259: /* $@16: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 260: /* expr_type_decl: "type" '<' $@15 type_declaration '>' $@16  */
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

  case 317: /* $@17: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 318: /* $@18: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 319: /* func_addr_expr: '@' '@' '<' $@17 type_declaration '>' $@18 func_addr_name  */
                                                                                                                                            {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 320: /* $@19: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 321: /* $@20: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 322: /* func_addr_expr: '@' '@' '<' $@19 optional_function_argument_list optional_function_type '>' $@20 func_addr_name  */
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

  case 325: /* $@21: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 326: /* $@22: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 327: /* expr_field: expr '.' $@21 error $@22  */
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

  case 383: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' ')'  */
                                                                                                   {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 384: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' expr ')'  */
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

  case 387: /* $@23: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 388: /* $@24: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 389: /* expr: expr "is" "type" '<' $@23 type_declaration '>' $@24  */
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

  case 393: /* $@25: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 394: /* $@26: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 395: /* expr: expr "as" "type" '<' $@25 type_declaration '>' $@26  */
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

  case 398: /* $@27: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 399: /* $@28: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 400: /* expr: expr '?' "as" "type" '<' $@27 type_declaration '>' $@28  */
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

  case 440: /* $@29: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 441: /* struct_variable_declaration_list: struct_variable_declaration_list $@29 structure_variable_declaration ';'  */
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

  case 442: /* $@30: %empty  */
                                                                                             {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 443: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@30 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
            }
    break;

  case 444: /* $@31: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 445: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@31 function_declaration_header expression_block  */
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

  case 477: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration ';'  */
                                                                                 {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 478: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr ';'  */
                                                                                                                       {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 479: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr_pipe  */
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

  case 483: /* $@32: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 484: /* global_variable_declaration_list: global_variable_declaration_list $@32 optional_field_annotation let_variable_declaration  */
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

  case 491: /* $@33: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 492: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@33 optional_field_annotation let_variable_declaration  */
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

  case 496: /* $@34: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 497: /* single_alias: "name" $@34 '=' type_declaration  */
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

  case 518: /* $@35: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 519: /* $@36: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 520: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@35 structure_name $@36 '{' struct_variable_declaration_list '}'  */
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

  case 563: /* $@37: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 564: /* $@38: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 565: /* bitfield_type_declaration: "bitfield" '<' $@37 bitfield_bits '>' $@38  */
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

  case 568: /* type_declaration: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 569: /* type_declaration: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 570: /* type_declaration: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 571: /* type_declaration: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 572: /* type_declaration: type_declaration '[' expr ']'  */
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

  case 573: /* type_declaration: type_declaration '[' ']'  */
                                           {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 574: /* type_declaration: type_declaration '-' '[' ']'  */
                                               {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 575: /* type_declaration: type_declaration "explicit"  */
                                                {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 576: /* type_declaration: type_declaration "const"  */
                                             {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 577: /* type_declaration: type_declaration '-' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 578: /* type_declaration: type_declaration '&'  */
                                       {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 579: /* type_declaration: type_declaration '-' '&'  */
                                           {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 580: /* type_declaration: type_declaration '#'  */
                                       {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 581: /* type_declaration: type_declaration "implicit"  */
                                                {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 582: /* type_declaration: type_declaration '-' '#'  */
                                           {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 583: /* type_declaration: type_declaration '=' "const"  */
                                                 {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 584: /* type_declaration: type_declaration '?'  */
                                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 585: /* $@39: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 586: /* $@40: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 587: /* type_declaration: "smart_ptr" '<' $@39 type_declaration '>' $@40  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 588: /* type_declaration: type_declaration "??"  */
                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 589: /* $@41: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 590: /* $@42: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 591: /* type_declaration: "array" '<' $@41 type_declaration '>' $@42  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 592: /* $@43: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 593: /* $@44: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 594: /* type_declaration: "table" '<' $@43 table_type_pair '>' $@44  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 595: /* $@45: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 596: /* $@46: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 597: /* type_declaration: "iterator" '<' $@45 type_declaration '>' $@46  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 598: /* type_declaration: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 599: /* $@47: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 600: /* $@48: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 601: /* type_declaration: "block" '<' $@47 type_declaration '>' $@48  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 602: /* $@49: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 603: /* $@50: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 604: /* type_declaration: "block" '<' $@49 optional_function_argument_list optional_function_type '>' $@50  */
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

  case 605: /* type_declaration: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 606: /* $@51: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 607: /* $@52: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 608: /* type_declaration: "function" '<' $@51 type_declaration '>' $@52  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 609: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 610: /* $@54: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 611: /* type_declaration: "function" '<' $@53 optional_function_argument_list optional_function_type '>' $@54  */
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

  case 612: /* type_declaration: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 613: /* $@55: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 614: /* $@56: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 615: /* type_declaration: "lambda" '<' $@55 type_declaration '>' $@56  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 616: /* $@57: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 617: /* $@58: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 618: /* type_declaration: "lambda" '<' $@57 optional_function_argument_list optional_function_type '>' $@58  */
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

  case 619: /* $@59: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 620: /* $@60: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 621: /* type_declaration: "tuple" '<' $@59 tuple_type_list '>' $@60  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 622: /* $@61: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 623: /* $@62: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 624: /* type_declaration: "variant" '<' $@61 variant_type_list '>' $@62  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 625: /* $@63: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 626: /* $@64: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 627: /* variant_alias_declaration: "variant" $@63 "name" $@64 '{' variant_type_list ';' '}'  */
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

  case 628: /* $@65: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 629: /* bitfield_alias_declaration: "bitfield" $@65 "name" '{' bitfield_bits ';' '}'  */
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

  case 630: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 631: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 632: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 633: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 634: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 635: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 636: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 637: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 638: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 639: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 640: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 641: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 642: /* make_struct_decl: "[[" type_declaration make_struct_dim optional_block ']' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 643: /* make_struct_decl: "[[" type_declaration optional_block ']' ']'  */
                                                                        {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 644: /* make_struct_decl: "[[" type_declaration '(' ')' optional_block ']' ']'  */
                                                                                {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 645: /* make_struct_decl: "[[" type_declaration '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                     {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 646: /* make_struct_decl: "[{" type_declaration make_struct_dim optional_block '}' ']'  */
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 647: /* make_struct_decl: "[{" type_declaration '(' ')' make_struct_dim optional_block '}' ']'  */
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

  case 648: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 649: /* make_tuple: make_tuple ',' expr  */
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

  case 650: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 651: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 652: /* make_any_tuple: make_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 653: /* make_any_tuple: make_map_tuple  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 654: /* make_dim: make_any_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 655: /* make_dim: make_dim ';' make_any_tuple  */
                                              {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 656: /* make_dim_decl: "[[" type_declaration make_dim ']' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 657: /* make_dim_decl: "[{" type_declaration make_dim '}' ']'  */
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 658: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 659: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 660: /* make_table_decl: "{{" make_table '}' '}'  */
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

  case 661: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 662: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 663: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 664: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


