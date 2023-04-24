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

    namespace das { class Module; }
    void das_collect_keywords ( das::Module * mod, yyscan_t yyscanner );

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
  YYSYMBOL_DAS_INSCOPE = 64,               /* "inscope"  */
  YYSYMBOL_DAS_TBOOL = 65,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 66,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 67,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 68,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 69,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 70,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 71,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 72,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 73,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 74,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 75,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 76,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 77,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 78,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 79,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 80,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 81,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 82,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 83,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 84,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 85,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 86,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 87,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 88,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 89,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 90,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 91,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 92,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 93,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 94,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 95,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 96,                /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 97,              /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 98,             /* "generator"  */
  YYSYMBOL_DAS_YIELD = 99,                 /* "yield"  */
  YYSYMBOL_DAS_SEALED = 100,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 101,                   /* "+="  */
  YYSYMBOL_SUBEQU = 102,                   /* "-="  */
  YYSYMBOL_DIVEQU = 103,                   /* "/="  */
  YYSYMBOL_MULEQU = 104,                   /* "*="  */
  YYSYMBOL_MODEQU = 105,                   /* "%="  */
  YYSYMBOL_ANDEQU = 106,                   /* "&="  */
  YYSYMBOL_OREQU = 107,                    /* "|="  */
  YYSYMBOL_XOREQU = 108,                   /* "^="  */
  YYSYMBOL_SHL = 109,                      /* "<<"  */
  YYSYMBOL_SHR = 110,                      /* ">>"  */
  YYSYMBOL_ADDADD = 111,                   /* "++"  */
  YYSYMBOL_SUBSUB = 112,                   /* "--"  */
  YYSYMBOL_LEEQU = 113,                    /* "<="  */
  YYSYMBOL_SHLEQU = 114,                   /* "<<="  */
  YYSYMBOL_SHREQU = 115,                   /* ">>="  */
  YYSYMBOL_GREQU = 116,                    /* ">="  */
  YYSYMBOL_EQUEQU = 117,                   /* "=="  */
  YYSYMBOL_NOTEQU = 118,                   /* "!="  */
  YYSYMBOL_RARROW = 119,                   /* "->"  */
  YYSYMBOL_LARROW = 120,                   /* "<-"  */
  YYSYMBOL_QQ = 121,                       /* "??"  */
  YYSYMBOL_QDOT = 122,                     /* "?."  */
  YYSYMBOL_QBRA = 123,                     /* "?["  */
  YYSYMBOL_LPIPE = 124,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 125,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 126,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 127,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 128,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 129,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 130,                 /* ":="  */
  YYSYMBOL_ROTL = 131,                     /* "<<<"  */
  YYSYMBOL_ROTR = 132,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 133,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 134,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 135,                    /* "=>"  */
  YYSYMBOL_COLCOL = 136,                   /* "::"  */
  YYSYMBOL_ANDAND = 137,                   /* "&&"  */
  YYSYMBOL_OROR = 138,                     /* "||"  */
  YYSYMBOL_XORXOR = 139,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 140,                /* "&&="  */
  YYSYMBOL_OROREQU = 141,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 142,                /* "^^="  */
  YYSYMBOL_DOTDOT = 143,                   /* ".."  */
  YYSYMBOL_MTAG_E = 144,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 145,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 146,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 147,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 148,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 149,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 150,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 151,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 152,           /* "..."  */
  YYSYMBOL_BRABRAB = 153,                  /* "[["  */
  YYSYMBOL_BRACBRB = 154,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 155,                  /* "{{"  */
  YYSYMBOL_INTEGER = 156,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 157,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 158,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 159,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 160,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 161,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 162,                   /* "double constant"  */
  YYSYMBOL_NAME = 163,                     /* "name"  */
  YYSYMBOL_KEYWORD = 164,                  /* "keyword"  */
  YYSYMBOL_BEGIN_STRING = 165,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 166,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 167,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 168,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 169,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 170,          /* "}"  */
  YYSYMBOL_END_OF_READ = 171,              /* "end of failed eader macro"  */
  YYSYMBOL_COMMA_SQR_BR = 172,             /* ",]"  */
  YYSYMBOL_COMMA_CUR_BR = 173,             /* ",}"  */
  YYSYMBOL_SEMICOLON_SQR_BR = 174,         /* ";]"  */
  YYSYMBOL_SEMICOLON_CUR_BR = 175,         /* ";}"  */
  YYSYMBOL_176_ = 176,                     /* ','  */
  YYSYMBOL_177_ = 177,                     /* '='  */
  YYSYMBOL_178_ = 178,                     /* '?'  */
  YYSYMBOL_179_ = 179,                     /* ':'  */
  YYSYMBOL_180_ = 180,                     /* '|'  */
  YYSYMBOL_181_ = 181,                     /* '^'  */
  YYSYMBOL_182_ = 182,                     /* '&'  */
  YYSYMBOL_183_ = 183,                     /* '<'  */
  YYSYMBOL_184_ = 184,                     /* '>'  */
  YYSYMBOL_185_ = 185,                     /* '-'  */
  YYSYMBOL_186_ = 186,                     /* '+'  */
  YYSYMBOL_187_ = 187,                     /* '*'  */
  YYSYMBOL_188_ = 188,                     /* '/'  */
  YYSYMBOL_189_ = 189,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 190,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 191,               /* UNARY_PLUS  */
  YYSYMBOL_192_ = 192,                     /* '~'  */
  YYSYMBOL_193_ = 193,                     /* '!'  */
  YYSYMBOL_PRE_INC = 194,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 195,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 196,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 197,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 198,                    /* DEREF  */
  YYSYMBOL_199_ = 199,                     /* '.'  */
  YYSYMBOL_200_ = 200,                     /* '['  */
  YYSYMBOL_201_ = 201,                     /* ']'  */
  YYSYMBOL_202_ = 202,                     /* '('  */
  YYSYMBOL_203_ = 203,                     /* ')'  */
  YYSYMBOL_204_ = 204,                     /* '$'  */
  YYSYMBOL_205_ = 205,                     /* '@'  */
  YYSYMBOL_206_ = 206,                     /* ';'  */
  YYSYMBOL_207_ = 207,                     /* '{'  */
  YYSYMBOL_208_ = 208,                     /* '}'  */
  YYSYMBOL_209_ = 209,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 210,                 /* $accept  */
  YYSYMBOL_program = 211,                  /* program  */
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
  YYSYMBOL_keyword_or_name = 224,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 225,      /* require_module_name  */
  YYSYMBOL_require_module = 226,           /* require_module  */
  YYSYMBOL_is_public_module = 227,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 228,       /* expect_declaration  */
  YYSYMBOL_expect_list = 229,              /* expect_list  */
  YYSYMBOL_expect_error = 230,             /* expect_error  */
  YYSYMBOL_expression_label = 231,         /* expression_label  */
  YYSYMBOL_expression_goto = 232,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 233,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 234,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 235,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 236, /* expression_else_one_liner  */
  YYSYMBOL_237_2 = 237,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 238,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 239,  /* expression_if_then_else  */
  YYSYMBOL_240_3 = 240,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 241,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 242,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 243,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 244,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 245,    /* expression_with_alias  */
  YYSYMBOL_246_4 = 246,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 247, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 248, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 249, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 250,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 251, /* annotation_argument_list  */
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
  YYSYMBOL_264_5 = 264,                    /* $@5  */
  YYSYMBOL_expression_block = 265,         /* expression_block  */
  YYSYMBOL_expression_any = 266,           /* expression_any  */
  YYSYMBOL_expressions = 267,              /* expressions  */
  YYSYMBOL_expr_keyword = 268,             /* expr_keyword  */
  YYSYMBOL_expression_keyword = 269,       /* expression_keyword  */
  YYSYMBOL_270_6 = 270,                    /* $@6  */
  YYSYMBOL_271_7 = 271,                    /* $@7  */
  YYSYMBOL_expr_pipe = 272,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 273,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 274,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 275,     /* new_type_declaration  */
  YYSYMBOL_276_8 = 276,                    /* $@8  */
  YYSYMBOL_277_9 = 277,                    /* $@9  */
  YYSYMBOL_expr_new = 278,                 /* expr_new  */
  YYSYMBOL_expression_break = 279,         /* expression_break  */
  YYSYMBOL_expression_continue = 280,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 281, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 282,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 283, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 284,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 285,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 286,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 287,        /* optional_in_scope  */
  YYSYMBOL_expression_let = 288,           /* expression_let  */
  YYSYMBOL_expr_cast = 289,                /* expr_cast  */
  YYSYMBOL_290_10 = 290,                   /* $@10  */
  YYSYMBOL_291_11 = 291,                   /* $@11  */
  YYSYMBOL_292_12 = 292,                   /* $@12  */
  YYSYMBOL_293_13 = 293,                   /* $@13  */
  YYSYMBOL_294_14 = 294,                   /* $@14  */
  YYSYMBOL_295_15 = 295,                   /* $@15  */
  YYSYMBOL_expr_type_decl = 296,           /* expr_type_decl  */
  YYSYMBOL_297_16 = 297,                   /* $@16  */
  YYSYMBOL_298_17 = 298,                   /* $@17  */
  YYSYMBOL_expr_type_info = 299,           /* expr_type_info  */
  YYSYMBOL_expr_list = 300,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 301,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 302,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 303,            /* capture_entry  */
  YYSYMBOL_capture_list = 304,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 305,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 306,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 307,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 308,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 309,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 310,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 311,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 312,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 313,           /* func_addr_expr  */
  YYSYMBOL_314_18 = 314,                   /* $@18  */
  YYSYMBOL_315_19 = 315,                   /* $@19  */
  YYSYMBOL_316_20 = 316,                   /* $@20  */
  YYSYMBOL_317_21 = 317,                   /* $@21  */
  YYSYMBOL_expr_field = 318,               /* expr_field  */
  YYSYMBOL_319_22 = 319,                   /* $@22  */
  YYSYMBOL_320_23 = 320,                   /* $@23  */
  YYSYMBOL_expr = 321,                     /* expr  */
  YYSYMBOL_322_24 = 322,                   /* $@24  */
  YYSYMBOL_323_25 = 323,                   /* $@25  */
  YYSYMBOL_324_26 = 324,                   /* $@26  */
  YYSYMBOL_325_27 = 325,                   /* $@27  */
  YYSYMBOL_326_28 = 326,                   /* $@28  */
  YYSYMBOL_327_29 = 327,                   /* $@29  */
  YYSYMBOL_expr_mtag = 328,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 329, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 330,        /* optional_override  */
  YYSYMBOL_optional_constant = 331,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 332, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 333, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 334, /* struct_variable_declaration_list  */
  YYSYMBOL_335_30 = 335,                   /* $@30  */
  YYSYMBOL_336_31 = 336,                   /* $@31  */
  YYSYMBOL_337_32 = 337,                   /* $@32  */
  YYSYMBOL_function_argument_declaration = 338, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 339,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 340,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 341,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 342,             /* variant_type  */
  YYSYMBOL_variant_type_list = 343,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 344,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 345,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 346,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 347,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 348, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 349, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 350, /* global_variable_declaration_list  */
  YYSYMBOL_351_33 = 351,                   /* $@33  */
  YYSYMBOL_optional_shared = 352,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 353, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 354,               /* global_let  */
  YYSYMBOL_355_34 = 355,                   /* $@34  */
  YYSYMBOL_enum_list = 356,                /* enum_list  */
  YYSYMBOL_single_alias = 357,             /* single_alias  */
  YYSYMBOL_358_35 = 358,                   /* $@35  */
  YYSYMBOL_alias_list = 359,               /* alias_list  */
  YYSYMBOL_alias_declaration = 360,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 361, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 362,                /* enum_name  */
  YYSYMBOL_enum_declaration = 363,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 364, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 365,          /* optional_sealed  */
  YYSYMBOL_structure_name = 366,           /* structure_name  */
  YYSYMBOL_class_or_struct = 367,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 368, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 369,    /* structure_declaration  */
  YYSYMBOL_370_36 = 370,                   /* $@36  */
  YYSYMBOL_371_37 = 371,                   /* $@37  */
  YYSYMBOL_variable_name_with_pos_list = 372, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 373,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 374, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 375, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 376,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 377,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 378, /* bitfield_type_declaration  */
  YYSYMBOL_379_38 = 379,                   /* $@38  */
  YYSYMBOL_380_39 = 380,                   /* $@39  */
  YYSYMBOL_table_type_pair = 381,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 382, /* type_declaration_no_options  */
  YYSYMBOL_383_40 = 383,                   /* $@40  */
  YYSYMBOL_384_41 = 384,                   /* $@41  */
  YYSYMBOL_385_42 = 385,                   /* $@42  */
  YYSYMBOL_386_43 = 386,                   /* $@43  */
  YYSYMBOL_387_44 = 387,                   /* $@44  */
  YYSYMBOL_388_45 = 388,                   /* $@45  */
  YYSYMBOL_389_46 = 389,                   /* $@46  */
  YYSYMBOL_390_47 = 390,                   /* $@47  */
  YYSYMBOL_391_48 = 391,                   /* $@48  */
  YYSYMBOL_392_49 = 392,                   /* $@49  */
  YYSYMBOL_393_50 = 393,                   /* $@50  */
  YYSYMBOL_394_51 = 394,                   /* $@51  */
  YYSYMBOL_395_52 = 395,                   /* $@52  */
  YYSYMBOL_396_53 = 396,                   /* $@53  */
  YYSYMBOL_397_54 = 397,                   /* $@54  */
  YYSYMBOL_398_55 = 398,                   /* $@55  */
  YYSYMBOL_399_56 = 399,                   /* $@56  */
  YYSYMBOL_400_57 = 400,                   /* $@57  */
  YYSYMBOL_401_58 = 401,                   /* $@58  */
  YYSYMBOL_402_59 = 402,                   /* $@59  */
  YYSYMBOL_403_60 = 403,                   /* $@60  */
  YYSYMBOL_404_61 = 404,                   /* $@61  */
  YYSYMBOL_405_62 = 405,                   /* $@62  */
  YYSYMBOL_406_63 = 406,                   /* $@63  */
  YYSYMBOL_type_declaration = 407,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 408, /* variant_alias_declaration  */
  YYSYMBOL_409_64 = 409,                   /* $@64  */
  YYSYMBOL_410_65 = 410,                   /* $@65  */
  YYSYMBOL_bitfield_alias_declaration = 411, /* bitfield_alias_declaration  */
  YYSYMBOL_412_66 = 412,                   /* $@66  */
  YYSYMBOL_make_decl = 413,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 414,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 415,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 416,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_sqr_br = 417, /* optional_trailing_semicolon_sqr_br  */
  YYSYMBOL_optional_trailing_semicolon_cur_br = 418, /* optional_trailing_semicolon_cur_br  */
  YYSYMBOL_optional_trailing_delim_sqr_br = 419, /* optional_trailing_delim_sqr_br  */
  YYSYMBOL_optional_trailing_delim_cur_br = 420, /* optional_trailing_delim_cur_br  */
  YYSYMBOL_make_struct_decl = 421,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 422,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 423,           /* make_map_tuple  */
  YYSYMBOL_make_dim = 424,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 425,            /* make_dim_decl  */
  YYSYMBOL_make_table = 426,               /* make_table  */
  YYSYMBOL_make_table_decl = 427,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 428, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 429       /* array_comprehension  */
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
#define YYLAST   11075

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  210
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  220
/* YYNRULES -- Number of rules.  */
#define YYNRULES  713
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1246

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   437


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
       2,     2,     2,   193,     2,   209,   204,   189,   182,     2,
     202,   203,   187,   186,   176,   185,   199,   188,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   179,   206,
     183,   177,   184,   178,   205,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   200,     2,   201,   181,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   207,   180,   208,   192,     2,     2,     2,
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
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   190,   191,   194,   195,   196,   197,   198
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   496,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   512,   513,   514,   518,   519,
     523,   541,   542,   543,   544,   548,   552,   557,   566,   574,
     590,   595,   603,   603,   633,   654,   658,   659,   663,   666,
     670,   676,   685,   688,   694,   695,   699,   703,   704,   708,
     711,   717,   723,   726,   732,   733,   737,   738,   739,   748,
     749,   753,   754,   754,   760,   761,   762,   763,   764,   768,
     774,   774,   780,   786,   794,   804,   813,   813,   820,   821,
     822,   823,   824,   825,   829,   834,   842,   843,   844,   848,
     849,   850,   851,   852,   853,   854,   855,   861,   864,   870,
     871,   872,   876,   884,   897,   900,   908,   919,   930,   941,
     944,   951,   955,   962,   963,   967,   968,   969,   973,   976,
     983,   987,   988,   989,   990,   991,   992,   993,   994,   995,
     996,   997,   998,   999,  1000,  1001,  1002,  1003,  1004,  1005,
    1006,  1007,  1008,  1009,  1010,  1011,  1012,  1013,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,  1035,
    1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,  1044,  1045,
    1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,  1054,  1055,
    1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,  1065,
    1066,  1067,  1068,  1069,  1074,  1096,  1097,  1098,  1102,  1108,
    1108,  1125,  1129,  1140,  1141,  1142,  1143,  1144,  1145,  1146,
    1147,  1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,  1156,
    1157,  1158,  1159,  1160,  1164,  1169,  1175,  1181,  1192,  1192,
    1192,  1203,  1237,  1240,  1243,  1249,  1250,  1261,  1265,  1268,
    1276,  1276,  1276,  1279,  1285,  1288,  1291,  1295,  1301,  1305,
    1309,  1312,  1315,  1323,  1326,  1329,  1337,  1340,  1348,  1351,
    1354,  1362,  1368,  1369,  1373,  1374,  1378,  1384,  1384,  1384,
    1387,  1387,  1387,  1392,  1392,  1392,  1400,  1400,  1400,  1406,
    1416,  1427,  1442,  1445,  1451,  1452,  1459,  1470,  1471,  1472,
    1476,  1477,  1478,  1479,  1483,  1488,  1496,  1497,  1501,  1506,
    1513,  1514,  1515,  1516,  1517,  1518,  1519,  1523,  1524,  1525,
    1526,  1527,  1528,  1529,  1530,  1531,  1532,  1533,  1534,  1535,
    1536,  1537,  1538,  1539,  1540,  1541,  1545,  1546,  1547,  1548,
    1549,  1550,  1554,  1561,  1573,  1578,  1588,  1592,  1599,  1602,
    1602,  1602,  1607,  1607,  1607,  1620,  1624,  1628,  1628,  1628,
    1635,  1636,  1637,  1638,  1639,  1640,  1641,  1642,  1643,  1644,
    1645,  1646,  1647,  1648,  1649,  1650,  1651,  1652,  1653,  1654,
    1655,  1656,  1657,  1658,  1659,  1660,  1661,  1662,  1663,  1664,
    1665,  1666,  1667,  1668,  1669,  1670,  1676,  1677,  1678,  1679,
    1680,  1681,  1682,  1683,  1684,  1685,  1686,  1687,  1688,  1692,
    1696,  1699,  1702,  1703,  1704,  1705,  1708,  1711,  1712,  1715,
    1715,  1715,  1718,  1723,  1727,  1731,  1731,  1731,  1736,  1739,
    1743,  1743,  1743,  1748,  1751,  1752,  1753,  1754,  1755,  1756,
    1757,  1758,  1759,  1760,  1761,  1766,  1770,  1771,  1772,  1773,
    1774,  1775,  1776,  1780,  1784,  1788,  1792,  1796,  1800,  1804,
    1808,  1812,  1819,  1820,  1824,  1825,  1826,  1830,  1831,  1835,
    1836,  1837,  1841,  1851,  1854,  1854,  1873,  1872,  1887,  1886,
    1899,  1908,  1914,  1919,  1929,  1930,  1934,  1937,  1946,  1947,
    1951,  1960,  1961,  1966,  1967,  1971,  1977,  1983,  1986,  1990,
    1996,  2005,  2006,  2007,  2011,  2012,  2016,  2023,  2028,  2037,
    2043,  2054,  2057,  2062,  2067,  2075,  2086,  2089,  2089,  2109,
    2110,  2114,  2115,  2116,  2120,  2123,  2123,  2142,  2145,  2154,
    2167,  2167,  2188,  2189,  2193,  2194,  2198,  2199,  2200,  2204,
    2214,  2221,  2231,  2232,  2236,  2237,  2241,  2247,  2248,  2252,
    2253,  2254,  2258,  2263,  2258,  2275,  2282,  2287,  2296,  2302,
    2313,  2314,  2315,  2316,  2317,  2318,  2319,  2320,  2321,  2322,
    2323,  2324,  2325,  2326,  2327,  2328,  2329,  2330,  2331,  2332,
    2333,  2334,  2335,  2336,  2337,  2338,  2339,  2343,  2344,  2345,
    2346,  2347,  2348,  2352,  2363,  2367,  2374,  2386,  2393,  2402,
    2402,  2402,  2415,  2419,  2426,  2427,  2428,  2429,  2430,  2444,
    2450,  2454,  2458,  2463,  2468,  2473,  2478,  2482,  2486,  2491,
    2495,  2499,  2504,  2504,  2504,  2510,  2517,  2517,  2517,  2522,
    2522,  2522,  2528,  2528,  2528,  2533,  2537,  2537,  2537,  2542,
    2542,  2542,  2551,  2555,  2555,  2555,  2560,  2560,  2560,  2569,
    2573,  2573,  2573,  2578,  2578,  2578,  2587,  2587,  2587,  2593,
    2593,  2593,  2602,  2605,  2616,  2632,  2632,  2632,  2656,  2656,
    2676,  2677,  2678,  2679,  2683,  2690,  2697,  2703,  2712,  2717,
    2724,  2725,  2729,  2730,  2734,  2735,  2739,  2740,  2741,  2745,
    2746,  2747,  2752,  2758,  2765,  2773,  2780,  2788,  2800,  2803,
    2809,  2823,  2829,  2835,  2840,  2847,  2852,  2862,  2867,  2874,
    2886,  2887,  2891,  2894
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
  "\"private\"", "\"smart_ptr\"", "\"unsafe\"", "\"inscope\"", "\"bool\"",
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
  "\"?[\"", "\"<|\"", "\" <|\"", "\"$ <|\"", "\"@ <|\"", "\"@@ <|\"",
  "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"",
  "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"",
  "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"",
  "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"keyword\"",
  "\"start of the string\"", "STRING_CHARACTER", "STRING_CHARACTER_ESC",
  "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "\",]\"", "\",}\"", "\";]\"", "\";}\"",
  "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'",
  "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'",
  "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "DEREF", "'.'", "'['",
  "']'", "'('", "')'", "'$'", "'@'", "';'", "'{'", "'}'", "'#'", "$accept",
  "program", "optional_public_or_private_module", "module_name",
  "module_declaration", "character_sequence", "string_constant",
  "string_builder_body", "string_builder", "reader_character_sequence",
  "expr_reader", "$@1", "options_declaration", "require_declaration",
  "keyword_or_name", "require_module_name", "require_module",
  "is_public_module", "expect_declaration", "expect_list", "expect_error",
  "expression_label", "expression_goto", "elif_or_static_elif",
  "expression_else", "if_or_static_if", "expression_else_one_liner", "$@2",
  "expression_if_one_liner", "expression_if_then_else", "$@3",
  "expression_for_loop", "expression_unsafe", "expression_while_loop",
  "expression_with", "expression_with_alias", "$@4",
  "annotation_argument_value", "annotation_argument_value_list",
  "annotation_argument_name", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@5", "expression_block", "expression_any",
  "expressions", "expr_keyword", "expression_keyword", "$@6", "$@7",
  "expr_pipe", "name_in_namespace", "expression_delete",
  "new_type_declaration", "$@8", "$@9", "expr_new", "expression_break",
  "expression_continue", "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let", "optional_in_scope", "expression_let", "expr_cast", "$@10",
  "$@11", "$@12", "$@13", "$@14", "$@15", "expr_type_decl", "$@16", "$@17",
  "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_name", "func_addr_expr", "$@18", "$@19", "$@20", "$@21",
  "expr_field", "$@22", "$@23", "expr", "$@24", "$@25", "$@26", "$@27",
  "$@28", "$@29", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@30", "$@31", "$@32", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@33", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@34", "enum_list",
  "single_alias", "$@35", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_name", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@36", "$@37", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@38", "$@39", "table_type_pair",
  "type_declaration_no_options", "$@40", "$@41", "$@42", "$@43", "$@44",
  "$@45", "$@46", "$@47", "$@48", "$@49", "$@50", "$@51", "$@52", "$@53",
  "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62",
  "$@63", "type_declaration", "variant_alias_declaration", "$@64", "$@65",
  "bitfield_alias_declaration", "$@66", "make_decl", "make_struct_fields",
  "make_struct_dim", "optional_block",
  "optional_trailing_semicolon_sqr_br",
  "optional_trailing_semicolon_cur_br", "optional_trailing_delim_sqr_br",
  "optional_trailing_delim_cur_br", "make_struct_decl", "make_tuple",
  "make_map_tuple", "make_dim", "make_dim_decl", "make_table",
  "make_table_decl", "array_comprehension_where", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1035)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-654)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1035,    19, -1035, -1035,    58,   -24,   219,   -97, -1035,   -80,
   -1035, -1035,   255, -1035, -1035, -1035, -1035, -1035,   463, -1035,
     108, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
      60, -1035,    40,    95,   128, -1035, -1035, -1035,   219, -1035,
      10, -1035, -1035,   118, -1035, -1035, -1035,   108,   150,   165,
   -1035, -1035,   255,   187,   275,   255,   255,   269, -1035,   478,
      38, -1035, -1035, -1035,   341,   445,   452, -1035,   459,    15,
      58,   324,   -24,   313,   380, -1035,   385,   385, -1035,   369,
     345,   -69,   462,   350, -1035, -1035, -1035,   406, -1035,   -52,
      58,   255,   255,   255,   255, -1035, -1035, -1035, -1035, -1035,
   -1035, -1035,   414, -1035, -1035, -1035, -1035, -1035,   381, -1035,
   -1035, -1035, -1035, -1035,   396,   119, -1035, -1035, -1035, -1035,
     535, -1035, -1035, 10724, -1035, -1035,   353, -1035, -1035, -1035,
     431,   391, -1035, -1035,   103, -1035,   222,   488,   478, 10912,
   -1035,  -107,   496, -1035,   467, -1035, -1035,   483, -1035, -1035,
   -1035, -1035, -1035, -1035, -1035,   172, -1035,   443,   449,   451,
     455, -1035, -1035, -1035,   438, -1035, -1035, -1035, -1035, -1035,
     465, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
   -1035, -1035,   469, -1035, -1035, -1035,   470,   472, -1035, -1035,
   -1035, -1035,   475,   480,   444, -1035, -1035, -1035, -1035, -1035,
    1247,   481, -1035, -1035,   477,   506, -1035,  9463, -1035, -1035,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
   -1035, -1035, -1035,   647,   650, -1035,   485,   484,   389, -1035,
   -1035,   522, -1035,   486,    58,   131, -1035, -1035, -1035,   119,
   -1035, -1035, -1035, -1035, -1035,   525, -1035,   -35,   293,   306,
   -1035, -1035,  6710, -1035, -1035, -1035,    30, -1035, -1035, -1035,
       4,  3826, -1035,  1166,   -49,   513, -1035,   487,   534,   537,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
     538,   497, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
   -1035, -1035, -1035, -1035,   681, -1035, -1035, -1035, -1035, -1035,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035,   541,   505, -1035,
   -1035,    98,   529, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
   -1035,   502,   -38,   531,   507, -1035,   467,   123,   509,   671,
     297, -1035, -1035, 10724, 10724, 10724, 10724,   512,   431, 10724,
     485, 10724,   485, 10724,   485, 10823,   506, -1035, -1035,   270,
     514,   536, -1035,   515,   539,   540,   516,   543,   518, -1035,
     544,  6710,  6710,   519,   526,   527,   528,   530,   532, -1035,
   10398, 10497,  6710, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
     548, -1035,  6710,  6710,  6710,   226,  6710,  6710,  6710, -1035,
     533, -1035, -1035, -1035, -1035,   -67, -1035, -1035, -1035, -1035,
     542, -1035, -1035, -1035, -1035, -1035, -1035,  7064, -1035,   546,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035,   545,
   -1035, -1035,  8933, -1035,  1247, -1035, -1035, 10724,   -21, -1035,
   -1035, -1035, -1035,   570, -1035, -1035,   549, -1035,    -9, -1035,
    -111, 10724, -1035,  1747, -1035,   -22, -1035,   226, -1035, -1035,
     131,   551,  6710,   573,   574, 10724, -1035,   -71,   338,   555,
     111,   344,   346, -1035,    39,   347,   529,   349,   529,   352,
     529,   -58, -1035,   186,   481,   238, -1035,   552, -1035, -1035,
     226, -1035,  6710, -1035, -1035,  6710, -1035,  6710, 10724,    50,
      50,  6710,  6710,  6710,  6710,  6710,  6710,   167,  2138,   167,
    2318,  9499, -1035,   -48, -1035,   417,    50,    50,   -34, -1035,
      50,    50,  7100,   311, -1035,  3646,   587,  9672, 10530,  6710,
    6710, -1035, -1035,  6710,  6710,  6710,  6710,   581,  6710,   164,
    6710,  6710,  6710,  6710,  6710,  6710,  6710,  6710,  6710,  3983,
    6710,  6710,  6710,  6710,  6710,  6710,  6710,  6710,  6710,  6710,
     -54,  6710, -1035,  4140, -1035, -1035,   481, -1035, -1035, -1035,
    6710,   167,   553,   700, -1035,   -74, -1035,   339,   481, -1035,
    6710, -1035, -1035,   167,  2678, -1035,   484,  4320,  6710,   582,
   -1035,   547,   601,  4477,   295,  2835,   388,   388,   388,  4634,
   -1035,   722,   560,   561,  6710,   750, -1035, -1035, -1035, -1035,
   -1035, -1035, -1035, -1035, -1035,   566,   567,   569,   571, -1035,
     575, -1035, -1035,   697, -1035,   -83, -1035,   704,   -20,  6710,
   -1035, -1035,    20, -1035, -1035,  7205, -1035,   735,   260, -1035,
   -1035, -1035,  3175, -1035, -1035, 10724, -1035, -1035, -1035,   616,
   -1035,   596, -1035,   599, -1035,   600, 10724, -1035, 10823, -1035,
     506, 10724,  4814,  4994, 10724,  7241, 10724, 10724,  7346, 10724,
    7382,   105,  7487,  7523,  7628,  7664,  7769,  7805,    16,   388,
     -46,  2498,  5174,  9593,   609,   -13,   151,   612,   163,    32,
    5354,   -13,   -25,  6710, -1035,  6710, -1035,   584, 10724, -1035,
    6710,   405,   623, -1035,   588,   591,   307, -1035, -1035,   633,
   -1035,   192,  9817,     0,   485,   614,   597, -1035, -1035,   617,
     602, -1035, -1035,    78,    78,  1095,  1095,   830,   830,   627,
     174,   628, -1035,  9027,   -59,   -59,    78,    78, 10057, 10022,
    1398,  9902, 10625,  9687, 10142, 10170, 10255,  1095,  1095,   210,
     210,   174,   174,   174,   252,  6710,   637, -1035,   283,  6710,
     800,  9121, -1035,   225,  7910, -1035,  6710,   639, -1035,   640,
   -1035, 10724, -1035,  3175, -1035,  1359,    35,  3175, -1035,   715,
    9356,   835,  6710,  9817,  1359,   672, -1035,   673,   698,  9817,
   -1035,  3175, -1035,  9356,   648, -1035, -1035, -1035,  1359,   651,
   -1035, -1035,  1359, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
     131,   388, -1035,  6710,  6710,  6710,  6710,  6710,  6710,  6710,
    6710,  6710,  6710,  3332,  6710,  6710,  6710,  6710,  6710,  6710,
    3489, -1035,  1544,   255, -1035,   848,   467, -1035,   694, -1035,
    3175, -1035,  6817, -1035, -1035,   481, -1035, -1035, -1035, -1035,
   -1035, -1035, -1035, -1035, -1035,   481, -1035, -1035, -1035,   355,
   -1035,   236,   696,  7946,   357, -1035,   382,   457, -1035,   565,
   -1035,   587, -1035, -1035, -1035, -1035, -1035,   658,  6710, -1035,
    6710,  6710,    -6,  6710,   699,   633,   151, -1035, -1035, -1035,
     660,  6710, -1035, -1035,  6710,   662,  6710,   633,   127,   663,
    9817, -1035, -1035,   578,  9781, -1035,   701,  6710,  6710, 10724,
     485,   -29,   135,  5534, -1035,   702,   703,   705,   706, -1035,
     144,   529, -1035,  6710, -1035,  6710,  5699,  6710, -1035,   687,
     669, -1035, -1035,  6710,   670, -1035,  9157,  6710,   674, -1035,
    9251, -1035, -1035,  6710, -1035, -1035,  8051, -1035,   832,   -73,
   -1035,  9356, -1035,  6710, -1035,  9356,  6710,  6710,   484,  9817,
   -1035, -1035, -1035, -1035, -1035,  9356, -1035, -1035, -1035,   482,
    6710, -1035, -1035,  9817,  9817,  9817,  9817,  9817,  9817,  9817,
    9817,  9817,  9817,   388,   388,   388,  9817,  9817,  9817,  9817,
    9817,  9817,  9817,   388,   388,   388,  9817, -1035,   250,   464,
      17,   668, -1035, -1035,  6923, -1035, -1035, -1035, -1035, -1035,
   -1035,   259, -1035, -1035, -1035, -1035, -1035,   675,  5879,   -50,
    9817,  9817,   -13,   151,  9817,     8,   609,   677, -1035,  9817,
     612, -1035,    51,   -13, -1035, -1035, -1035,   678, -1035, -1035,
   -1035, -1035,  8087,  8192,   586,   529,   680,   633,  9817, -1035,
   -1035, -1035, -1035,     0,   679,   -94, 10724,  8228, 10724,  8333,
   -1035,   251,  8369, -1035,  6710,  9937,  6710, -1035,  8474,  6710,
   -1035, -1035, -1035,   712,  6710,    53, -1035,  6710,  1955,   484,
   -1035, -1035,  6710, -1035,   490, -1035, -1035, -1035, -1035, -1035,
   -1035,   688, -1035, -1035,    12, -1035, -1035,   464, -1035, -1035,
   -1035,  6710,   732, -1035,  6710,  6710,  6710,  6059, -1035,   256,
    6710,   151,   695,  6710,  6710, -1035,  6710,   127, -1035,  6710,
   -1035, -1035, -1035,   713, -1035,   274, -1035, -1035,  6239, -1035,
   -1035,   992, -1035,   358, -1035, -1035, -1035, 10724,  8510,  8615,
   -1035,  8651, -1035,  9817,   484,  9817, -1035, -1035,  1359, -1035,
     692, -1035,   867,   867,   167,  8756,   716,  1095,  1095,  1095,
   -1035,  8792, -1035,  6959,   707, -1035,  9817,  9817,  6959,   708,
    1095,   231, -1035,   710,  6710,  9902, -1035, -1035,   360, -1035,
   -1035, -1035, -1035,   482,  3018, -1035, -1035, -1035, -1035, -1035,
   -1035,  6710, -1035,   854,   709, -1035,   711, -1035, -1035,   231,
   -1035,  9937, -1035, -1035, -1035, -1035,  6396,  6553, -1035, -1035,
   -1035, -1035, -1035,  9817, 10912, 10912,  8897,  6710,   714,   717,
   -1035, -1035,  6710,  9817,  6710,  9817,   723,   484, -1035,  9817,
   -1035, -1035,  9817,  9817, -1035, -1035
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   113,     1,   272,     0,     0,     0,     0,   273,     0,
     668,   665,     0,    14,     3,    10,     9,     8,     0,     7,
     519,     6,    11,     5,     4,    12,    13,    87,    88,    86,
      95,    97,    34,    49,    46,    47,    36,    37,     0,    38,
      44,    35,   530,     0,   535,    19,    18,   519,     0,     0,
     100,   101,     0,     0,   245,     0,     0,   102,   104,   111,
       0,    99,   548,   547,   205,   536,   549,   520,   521,     0,
       0,     0,     0,    39,     0,    45,     0,     0,    42,     0,
       0,     0,    15,     0,   666,   110,   247,     0,   105,     0,
       0,     0,     0,     0,     0,   114,   207,   206,   209,   204,
     538,   537,     0,   551,   550,   552,   523,   522,   525,    93,
      94,    91,    92,    90,     0,     0,    89,    98,    50,    48,
      44,    41,    40,     0,   532,   534,     0,    16,    17,    20,
       0,     0,   246,   109,     0,   106,   107,   108,   112,     0,
     539,     0,   544,   516,   462,    21,    22,     0,    82,    83,
      80,    81,    79,    78,    84,     0,    43,     0,     0,     0,
       0,   560,   580,   561,   594,   562,   566,   567,   568,   569,
     586,   573,   574,   575,   576,   577,   578,   579,   581,   582,
     583,   584,   635,   565,   572,   585,   642,   649,   563,   570,
     564,   571,     0,     0,     0,   593,   604,   607,   605,   606,
     662,   531,   533,   597,     0,     0,   103,     0,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,     0,     0,   120,   115,     0,     0,   527,
     545,     0,   553,   517,     0,     0,    23,    24,    25,     0,
      96,   626,   629,   632,   622,     0,   599,   636,   643,   650,
     656,   659,     0,   612,   617,   611,     0,   625,   621,   614,
       0,     0,   616,     0,     0,     0,   491,     0,   173,   174,
     171,   123,   124,   126,   125,   127,   128,   129,   130,   156,
     157,   154,   155,   147,   158,   159,   148,   145,   146,   172,
     167,     0,   170,   160,   161,   162,   163,   134,   135,   136,
     131,   132,   133,   144,     0,   150,   151,   149,   142,   143,
     138,   137,   139,   140,   141,   122,   121,   166,     0,   152,
     153,   462,   118,   234,   210,   587,   590,   588,   591,   589,
     592,     0,     0,   542,     0,   524,   462,     0,     0,   506,
     504,   526,    85,     0,     0,     0,     0,     0,     0,     0,
     115,     0,   115,     0,   115,     0,     0,   366,   367,     0,
       0,     0,   360,     0,     0,     0,     0,     0,     0,   586,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   451,
       0,     0,     0,   310,   312,   311,   313,   314,   315,   316,
       0,    26,     0,     0,     0,     0,     0,     0,     0,   297,
     298,   364,   363,   308,   445,   361,   437,   436,   435,   434,
     113,   440,   362,   439,   438,   407,   368,     0,   369,     0,
     365,   670,   671,   672,   673,   619,   620,   613,   615,     0,
     618,   609,     0,   664,   663,   598,   669,     0,     0,   175,
     176,   169,   165,   177,   168,   164,     0,   116,     0,   484,
       0,     0,   208,     0,   527,     0,   540,     0,   546,   473,
       0,     0,     0,     0,     0,     0,   505,     0,     0,     0,
     602,     0,     0,   595,     0,     0,   118,     0,   118,     0,
     118,   245,   488,     0,   486,     0,   250,   254,   253,   257,
       0,   286,     0,   277,   280,     0,   283,     0,     0,   396,
     397,     0,     0,     0,     0,     0,     0,     0,   680,     0,
       0,   702,   707,     0,   238,     0,   373,   372,   412,    32,
     371,   370,     0,   299,   443,     0,   306,     0,     0,     0,
       0,   398,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     357,     0,   596,     0,   610,   608,   490,   667,   492,   178,
       0,     0,     0,   555,   481,   495,   117,   462,   119,   236,
       0,    59,    60,     0,   260,   258,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     213,   211,     0,     0,     0,     0,   229,   224,   221,   220,
     222,   223,   235,   215,   214,     0,    67,    68,    65,   227,
      66,   228,   230,   275,   219,     0,   216,   317,     0,     0,
     528,   543,   474,   518,   463,     0,   508,   509,     0,   502,
     503,   501,     0,   627,   630,     0,   633,   623,   600,     0,
     637,     0,   644,     0,   651,     0,     0,   657,     0,   660,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,   698,   678,   680,     0,   703,     0,     0,
       0,   680,     0,     0,   685,     0,   684,     0,     0,    29,
       0,    27,     0,   400,     0,     0,   349,   346,   348,     0,
     408,     0,   292,     0,   115,     0,     0,   423,   422,     0,
       0,   424,   428,   374,   375,   387,   388,   385,   386,     0,
     417,     0,   405,     0,   441,   442,   376,   377,   392,   393,
     394,   395,     0,     0,   390,   391,   389,   383,   384,   379,
     378,   380,   381,   382,     0,     0,     0,   355,     0,     0,
       0,     0,   410,     0,     0,   482,     0,     0,   494,     0,
     493,     0,   496,     0,   485,     0,     0,     0,   264,     0,
     261,     0,     0,   248,     0,     0,   233,     0,     0,    53,
      73,     0,   269,   266,   298,   244,   242,   243,     0,     0,
     231,   232,     0,    70,   218,   225,   226,   263,   268,   274,
       0,     0,   217,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,     0,     0,   554,     0,   462,   507,     0,   511,
       0,   515,   317,   628,   631,   603,   634,   624,   601,   638,
     640,   645,   647,   652,   654,   487,   658,   489,   661,     0,
     255,     0,     0,     0,     0,   413,     0,     0,   414,     0,
     444,   306,   446,   447,   448,   449,   450,     0,     0,   681,
       0,     0,   680,     0,     0,     0,     0,   688,   687,   686,
       0,     0,   683,   682,     0,     0,     0,     0,     0,     0,
     701,   708,   709,     0,     0,    30,    33,     0,     0,     0,
     115,     0,     0,     0,   409,     0,     0,     0,     0,   304,
       0,   118,   419,     0,   425,     0,     0,     0,   403,     0,
       0,   429,   433,     0,     0,   406,     0,     0,     0,   356,
       0,   358,   401,     0,   411,   483,     0,   557,   558,   497,
     500,   499,    74,     0,   265,   262,     0,     0,     0,   249,
      75,    76,    51,    52,   270,   267,   299,   237,   234,    56,
       0,   276,   241,   327,   328,   330,   329,   331,   321,   322,
     323,   332,   333,     0,     0,     0,   319,   320,   334,   335,
     324,   325,   326,     0,     0,     0,   318,   529,     0,   469,
     464,     0,   510,   513,   317,   514,   641,   648,   655,   251,
     256,     0,   289,   287,   278,   281,   284,     0,     0,     0,
     675,   674,   680,     0,   699,     0,   679,     0,   693,   700,
     704,   705,     0,   680,   691,   690,   689,     0,   706,   239,
      28,    31,     0,     0,     0,   118,     0,     0,   293,   302,
     303,   301,   300,     0,     0,     0,     0,     0,     0,     0,
     344,     0,     0,   430,     0,   418,     0,   404,     0,     0,
     402,   359,   556,     0,     0,     0,   271,     0,     0,     0,
      54,    55,     0,    69,    61,   341,   339,   340,   338,   336,
     337,   114,   470,   471,   464,   465,   466,   469,   475,   512,
     252,     0,     0,   288,     0,     0,     0,     0,   452,     0,
       0,     0,     0,     0,     0,   692,     0,     0,   696,     0,
     347,   461,   350,     0,   342,     0,   305,   307,     0,   294,
     309,     0,   460,     0,   458,   345,   455,     0,     0,     0,
     454,     0,   559,   498,     0,    77,   212,    57,     0,    62,
       0,   480,   467,   467,     0,     0,     0,   279,   282,   285,
     415,     0,   453,   710,     0,   694,   677,   676,   710,     0,
     240,     0,   353,     0,     0,   295,   420,   426,     0,   459,
     457,   456,    72,    56,     0,    71,   468,   476,   478,   472,
     290,     0,   416,     0,     0,   695,     0,   697,   351,     0,
     343,   296,   421,   427,   431,    58,   260,     0,    63,    67,
      68,    65,    66,    64,     0,     0,     0,     0,     0,     0,
     354,   432,     0,   261,     0,   266,     0,     0,   291,   711,
     712,   713,   262,   267,   477,   479
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1035, -1035, -1035, -1035, -1035,   386,   837, -1035, -1035, -1035,
   -1035, -1035, -1035, -1035,   504,   876, -1035,   796, -1035, -1035,
     845, -1035, -1035, -1035,  -273, -1035, -1035, -1035,  -264, -1035,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035,   682, -1035, -1035,
     862,   -53, -1035, -1035,   216,    90,  -399,  -352,  -479, -1035,
   -1035, -1035,  -620, -1035, -1035,  -233, -1035,   -44, -1035, -1035,
   -1035, -1035,  -577,   -12, -1035, -1035, -1035, -1035, -1035,  -259,
    -258,  -257, -1035,  -256, -1035, -1035,   943, -1035, -1035, -1035,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
    -550, -1035, -1035,  -115, -1035,    69,  -574, -1035,  -458, -1035,
   -1035, -1035, -1034, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
   -1035,   520, -1035, -1035, -1035, -1035, -1035, -1035, -1035,  -141,
    -149,  -207,  -150, -1035, -1035, -1035, -1035, -1035,   371, -1035,
     292, -1035,  -422,   598,  -571,  -575,   315, -1035, -1035,  -455,
   -1035, -1035,   918, -1035, -1035, -1035,   508,   175, -1035, -1035,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
   -1035, -1035,  -464,  -121, -1035,   604, -1035,   608, -1035, -1035,
   -1035, -1035,  -272, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035, -1035,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035,  -101, -1035, -1035,
   -1035, -1035, -1035,   605,  -699,  -507,  -677, -1035,   266,  -880,
    -158, -1035,    66,   271,   458, -1035, -1035, -1035,  -203, -1035
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   129,    47,    14,   147,   153,   525,   411,   916,
     412,   712,    15,    16,    39,    40,    41,    78,    17,    34,
      35,   612,   613,  1092,  1093,   614,  1160,  1194,   615,   616,
     980,   617,   618,   619,   620,   621,  1087,   154,   155,    30,
      31,    32,    57,    58,    59,    60,    18,   332,   462,   236,
      19,    98,   237,    99,   139,   413,   622,   463,   623,   414,
     708,  1129,   624,   415,   625,   497,   671,  1110,   416,   626,
     627,   628,   629,   630,   631,   632,   633,   820,   634,   417,
     676,  1114,   677,  1115,   679,  1116,   418,   674,  1113,   419,
     721,  1140,   420,   929,   930,   724,   421,   422,   789,   636,
     423,   424,   718,   425,   919,  1181,   920,  1209,   426,   770,
    1081,   722,  1066,  1212,  1068,  1213,  1147,  1231,   428,   458,
    1107,  1197,  1104,  1011,   642,   846,  1224,  1225,   459,   460,
     492,   493,   276,   277,   891,   584,   652,   477,   350,   351,
     243,   346,    68,   108,    21,   144,   342,    44,    79,    81,
      22,   102,   141,    23,   468,   241,   242,    66,   105,    24,
     142,   344,   585,   429,   341,   197,   198,   204,   199,   358,
     858,   479,   200,   356,   857,   353,   853,   354,   854,   355,
     856,   359,   859,   360,  1016,   361,   861,   362,  1017,   363,
     863,   364,  1018,   365,   866,   366,   868,   494,    25,    49,
     131,    26,    48,   430,   694,   695,   696,   905,   707,   900,
    1047,   431,   697,   522,   698,   432,   523,   433,  1204,   434
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      61,   444,   196,   245,   334,   635,   775,   661,   486,   663,
     488,   665,   490,   701,   783,   643,  1037,   788,   896,     2,
     922,   536,   201,   773,   908,     3,   578,  -113,   802,   109,
     110,    74,   805,   806,   807,   888,   437,   134,   689,   581,
      61,  1138,   821,    61,    61,   689,   778,   778,     4,   649,
       5,   906,     6,   688,   963,   699,  1105,  1162,     7,   650,
     547,  1105,   435,   549,   550,    75,    42,     8,   764,   765,
     537,   538,   238,     9,   778,   851,    27,    28,    87,    61,
      61,    61,    61,    45,   890,    91,    92,    93,   549,   550,
      87,   778,   586,    10,    42,   587,   534,   766,   537,   538,
     239,   890,   779,   780,   780,   781,   651,   273,   782,   767,
      43,   195,  1106,   333,   445,   889,    11,  1106,   518,   520,
     925,   666,   871,   822,    46,   465,   953,   704,   778,   786,
     926,   780,    33,   148,   149,   535,   582,   263,  1123,   125,
     570,   571,   275,   465,  -639,   768,   769,  1208,   780,  -639,
     704,   133,   196,  1122,   583,   639,  1120,   921,   705,   446,
     706,   541,   542,   264,   265,   570,   571,  -639,    67,   547,
     466,   111,   549,   550,   551,  1230,   112,   927,   113,   552,
     114,   904,   928,   706,   640,   780,   438,   577,   841,   541,
     542,   347,   779,   895,   537,   538,  1036,   547,    76,   548,
     549,   550,   551,   648,   439,   470,   960,   552,   779,    77,
     964,   779,   436,   440,    94,  1033,    70,   115,    80,    12,
     843,    29,   266,   658,   974,    13,   267,   953,   844,   953,
     537,   538,   196,   196,   196,   196,   681,    69,   196,    95,
     196,  1174,   196,   845,   196,   659,   456,   982,   578,   570,
     571,   244,   478,   480,   481,   482,   126,  1126,   485,  1154,
     487,   195,   489,   565,   566,   567,   568,   569,    85,   196,
     196,    88,    89,  1013,    71,   150,   348,   570,   571,    70,
     151,    42,   152,   268,   114,   541,   542,   269,    50,   881,
     270,   273,   263,   547,   349,   548,   549,   550,   551,    70,
    1044,   457,  1045,   552,    72,   271,   206,   135,   136,   137,
     138,   894,   582,    83,   272,   741,    51,   655,   264,   265,
    1063,   541,   542,   897,   471,   898,   196,   742,    84,   547,
     583,   548,   549,   550,   551,  1046,  1056,   902,  1029,   552,
     196,   195,   195,   195,   195,  1064,   576,   195,   249,   195,
      86,   195,   899,   195,   196,  1121,  1042,   195,  1135,    91,
     588,    93,    53,   791,   903,   981,  1127,    53,   923,   904,
     667,   800,   931,   570,   571,   250,   714,   266,   195,   195,
     649,   267,    36,    37,    52,  1032,  1071,   196,  1084,    54,
     650,    53,   668,   529,    54,   924,    96,   567,   568,   569,
    1043,   953,    97,   944,   876,   877,    53,   879,    38,   570,
     571,    87,   953,  1085,   263,   945,   728,   732,    54,  1095,
    1096,  1097,   669,   390,   391,   392,    94,   953,   954,  1098,
    1099,  1100,   953,    54,   948,   195,   913,   651,   268,  1020,
     264,   265,   269,  1111,   670,   270,   949,    53,    55,   195,
     894,  1101,  1065,   496,  1145,   641,   714,    56,   335,  1172,
     271,   715,   336,   195,  1124,  1112,   849,    62,    63,   272,
      64,    90,  -646,   474,    54,  1183,   475,  -646,  1119,   476,
     118,   337,   338,   339,   340,  -653,  -352,   456,   673,   263,
    -653,  -352,   244,  1089,   716,  -646,   195,   507,    65,   266,
     100,  1159,   333,   267,  1090,  1091,   101,   103,  -653,  -352,
     537,   538,    77,   104,   106,   264,   265,   127,   273,  1102,
     107,   717,   653,   128,   273,  1103,   273,   273,   656,   273,
     657,   660,   273,   662,   196,   273,   664,   273,   273,  1019,
     273,  1023,  1187,   120,  1214,   196,   123,   196,    36,    37,
     196,   124,   962,   196,   855,   196,   196,   130,   196,   202,
     268,   970,   145,   146,   269,   865,  1024,   270,  1055,   132,
     869,   246,   247,   874,   266,   977,  1133,   140,   267,   979,
     121,   122,   271,   145,   146,   709,   710,   196,   143,  1199,
      75,   272,   409,   804,   203,   333,   240,   263,   205,   539,
     540,   541,   542,   543,  1236,  1237,   544,   545,   546,   547,
     263,   548,   549,   550,   551,    91,    92,    93,   263,   552,
     244,   553,   554,   264,   265,    91,   251,   555,   556,   557,
     635,   942,   252,   558,   253,   268,   264,   265,   254,   269,
     255,  1025,   270,   195,   264,   265,   262,  1054,   256,   246,
     247,   248,   257,   258,   195,   259,   195,   271,   260,   195,
     196,   273,   195,   261,   195,   195,   272,   195,   559,   275,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     959,   329,   266,   274,   330,   343,   267,   331,   357,   570,
     571,   333,   447,   448,   345,   266,   195,   449,   452,   267,
     450,   451,   453,   266,   454,  1010,   455,   267,   461,   464,
     467,   472,   473,   -64,   469,   483,   500,   502,   505,   501,
     507,   511,   503,   504,   537,   538,   506,   508,   512,   513,
     514,   524,   515,   579,   516,  1086,   646,   647,   533,   654,
     723,   777,    12,   268,   739,   795,   574,   269,   573,  1026,
     270,   580,   644,   796,   672,   776,   268,   797,   809,   813,
     269,   819,  1049,   270,   268,   271,   810,   811,   269,   195,
    1132,   270,   814,   815,   272,   816,   848,   817,   271,   445,
     860,   818,   427,   862,   864,   894,   271,   272,   901,   915,
     917,   442,   912,   918,  1141,   272,   921,   932,   196,   933,
     934,   951,   957,   958,   935,   823,   824,   825,   826,   827,
     828,   829,   830,   539,   540,   541,   542,   543,   831,   832,
     544,   545,   546,   547,   833,   548,   549,   550,   551,   936,
     937,    61,  1139,   552,   834,   553,   554,   835,   836,   947,
     821,   555,   556,   557,   837,   838,   839,   558,   968,   971,
     537,   538,   972,   976,   973,  1009,  1157,  1012,   978,  1021,
    1028,  1038,  1035,  1041,  1048,  1059,  1060,  1051,  1061,  1062,
    1073,  1074,  1076,  1083,  1108,  1152,  1079,  1117,  1125,  1128,
    1137,   840,   559,  1134,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,  1161,  1166,  1175,  1182,  1195,  1196,
    1201,   509,   510,   570,   571,  1227,   116,   195,  1205,  1207,
    1228,   711,   521,  1210,    73,  1240,   156,   119,  1241,  1229,
    1215,  1192,   526,   527,   528,  1193,   530,   531,   532,  1244,
    1218,   352,   117,  1008,  1088,  1219,  1220,  1221,  1222,   539,
     540,   541,   542,   543,    20,   196,   544,   196,  1136,   547,
    1027,   548,   549,   550,   551,  1163,  1198,  1164,   784,   552,
     867,   553,   554,   850,   495,    82,   484,  1143,   909,  1179,
    1040,     0,   638,   498,   499,  1206,   911,     0,   702,     0,
       0,     0,     0,   637,     0,     0,     0,     0,     0,     0,
       0,     0,   645,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1245,     0,     0,     0,     0,     0,
       0,     0,     0,   563,   564,   565,   566,   567,   568,   569,
       0,     0,   675,     0,   263,   678,   196,   680,     0,   570,
     571,   682,   683,   684,   685,   686,   687,     0,   693,     0,
     693,     0,     0,     0,     0,     0,  1188,     0,     0,     0,
     264,   265,     0,     0,   195,     0,   195,     0,     0,   733,
     734,     0,     0,   735,   736,   737,   738,     0,   740,     0,
     743,   744,   745,   746,   747,   748,   749,   750,   751,   753,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
       0,   771,     0,     0,     0,     0,     0,     0,     0,     0,
     774,     0,     0,     0,     0,     0,     0,     0,     0,   266,
     785,     0,     0,   267,   790,   537,   538,   793,   794,     0,
       0,     0,     0,   799,     0,   803,     0,     0,     0,   808,
       0,     0,     0,     0,   812,   195,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   842,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   717,
     268,     0,   852,     0,   269,     0,  1186,   270,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     157,     0,   271,   873,     0,     0,   158,   717,     0,     0,
       0,   272,     0,     0,   539,   540,   541,   542,     0,     0,
       0,   526,   532,   159,   547,     0,   548,   549,   550,   551,
     532,     0,     0,   910,   552,   521,   553,   554,   160,     0,
     914,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
     565,   566,   567,   568,   569,   946,     0,     0,     0,   950,
       0,     0,     0,     0,   570,   571,   956,     0,     0,     0,
       0,     0,    53,   961,     0,   264,   265,   965,     0,     0,
       0,     0,   969,     0,     0,   194,     0,     0,     0,     0,
       0,   975,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   983,   984,   985,   986,   987,   988,   989,
     990,   991,   992,   996,   997,   998,   999,  1000,  1001,  1002,
    1006,     0,     0,     0,   266,     0,     0,     0,   267,     0,
    1014,     0,     0,     0,     0,   443,     0,     0,     0,   537,
     538,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1030,  1031,     0,  1034,     0,     0,     0,     0,   537,   538,
       0,  1039,     0,     0,   693,   268,     0,     0,     0,   269,
       0,     0,   270,     0,     0,     0,     0,  1052,  1053,     0,
       0,     0,     0,  1058,     0,     0,     0,   271,     0,     0,
       0,     0,     0,  1067,     0,  1069,   272,  1072,     0,     0,
       0,     0,     0,  1075,     0,     0,     0,  1078,   539,   540,
     541,   542,   543,  1058,     0,   544,   545,   546,   547,     0,
     548,   549,   550,   551,     0,     0,   996,  1006,   552,     0,
     553,   554,     0,     0,     0,     0,   555,   556,   557,     0,
    1094,     0,   558,     0,     0,     0,     0,   539,   540,   541,
     542,   543,     0,     0,   544,   545,   546,   547,     0,   548,
     549,   550,   551,     0,     0,     0,     0,   552,     0,   553,
     554,     0,     0,     0,     0,   555,     0,   559,     0,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   570,   571,
       0,     0,     0,     0,   537,   538,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,  1148,     0,  1149,   570,   571,  1151,
       0,     0,     0,     0,  1153,     0,     0,  1155,   637,     0,
       0,     0,  1158,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1165,     0,     0,  1167,  1168,  1169,  1171,     0,     0,
    1173,     0,     0,  1176,  1177,     0,  1178,     0,     0,  1180,
       0,     0,     0,   539,   540,   541,   542,   543,  1185,     0,
     544,   545,   546,   547,     0,   548,   549,   550,   551,     0,
       0,     0,     0,   552,     0,   553,   554,     0,     0,     0,
       0,   555,   556,   557,     0,     0,     0,   558,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1223,     0,     0,     0,     0,     0,
       0,  1226,   559,     0,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,     0,     0,  1233,  1235,     0,     0,
       0,     0,     0,   570,   571,     0,     0,  1239,   589,     0,
    1007,     0,  1242,     3,  1243,   590,   591,   592,     0,   593,
       0,   367,   368,   369,   370,   371,     0,     0,     0,     0,
       0,     0,   594,   372,   595,   596,     0,     0,     0,     0,
       0,     0,     0,     0,   597,   373,     0,   598,     0,   599,
     374,     0,     0,   375,     0,     8,   376,   600,     0,   601,
     377,     0,     0,   602,   603,     0,     0,     0,     0,     0,
     604,     0,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   379,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,   183,   184,   185,     0,     0,   188,
     189,   190,   191,     0,     0,   380,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   606,   607,   608,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,   383,   384,   385,   386,   387,     0,   388,     0,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
      54,   609,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,   403,   404,     0,   405,     0,     0,   406,
     407,     0,     0,     0,     0,     0,     0,     0,     0,   408,
       0,   409,   410,   610,   333,   611,   589,     0,     0,     0,
       0,     3,     0,   590,   591,   592,     0,   593,     0,   367,
     368,   369,   370,   371,     0,     0,     0,     0,     0,     0,
     594,   372,   595,   596,     0,     0,     0,     0,     0,     0,
       0,     0,   597,   373,     0,   598,     0,   599,   374,     0,
       0,   375,     0,     8,   376,   600,     0,   601,   377,     0,
       0,   602,   603,     0,     0,     0,     0,     0,   604,     0,
     161,   162,   163,     0,   165,   166,   167,   168,   169,   379,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,   183,   184,   185,     0,     0,   188,   189,   190,
     191,     0,     0,   380,   605,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,   382,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   606,   607,   608,     0,     0,     0,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,   383,
     384,   385,   386,   387,     0,   388,     0,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,    54,   609,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,   403,   404,     0,   405,     0,     0,   406,   407,     0,
       0,     0,   367,   368,   369,   370,   371,   408,     0,   409,
     410,   610,   333,  1156,   372,     0,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   374,     0,     0,   375,     0,     0,   376,     0,   689,
       0,   377,     0,     0,     0,     0,   264,   265,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,   266,     0,     0,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,   690,   400,   401,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,     0,     0,
     269,     0,     0,   691,   403,   404,     0,   405,     0,     0,
     406,   407,   367,   368,   369,   370,   371,     0,   271,     0,
     692,     0,   409,   410,   372,   333,     0,   272,     0,     0,
     263,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   374,     0,     0,   375,     0,     0,   376,     0,     0,
       0,   377,     0,     0,     0,     0,   264,   265,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,   266,     0,     0,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,   690,   400,   401,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,     0,     0,
     269,     0,     0,   691,   403,   404,     0,   405,     0,     0,
     406,   407,   367,   368,   369,   370,   371,     0,   271,     0,
     700,     0,   409,   410,   372,   333,     0,   272,     0,     0,
     437,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   374,     0,     0,   375,     0,     0,   376,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,    54,   400,   401,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     438,     0,     0,   402,   403,   404,     0,   405,     0,     0,
     406,   407,   367,   368,   369,   370,   371,     0,   439,     0,
     408,     0,   409,   410,   372,   333,     0,   440,     0,     0,
       0,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   374,     0,     0,   375,     0,     0,   376,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,     0,     0,     0,   787,     0,
       0,     0,     0,     0,   606,   607,   608,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,    54,   400,   401,     0,     0,     0,     0,     0,   367,
     368,   369,   370,   371,     0,     0,     0,     0,     0,     0,
       0,   372,     0,   402,   403,   404,     0,   405,     0,     0,
     406,   407,     0,   373,     0,     0,     0,     0,   374,     0,
     408,   375,   409,   410,   376,   333,     0,     0,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
     161,   162,   163,     0,   165,   166,   167,   168,   169,   379,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,   183,   184,   185,     0,     0,   188,   189,   190,
     191,     0,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,   382,     0,     0,
       0,     0,     0,     0,     0,   801,     0,     0,     0,     0,
       0,   606,   607,   608,     0,     0,     0,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,   383,
     384,   385,   386,   387,     0,   388,     0,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,    54,   400,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,   403,   404,     0,   405,     0,     0,   406,   407,     0,
       0,     0,   367,   368,   369,   370,   371,   408,     0,   409,
     410,     0,   333,  1216,   372,   595,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   374,     0,     0,   375,     0,     0,   376,   600,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,  1217,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,    54,   400,   401,     0,     0,     0,     0,     0,   367,
     368,   369,   370,   371,     0,     0,     0,     0,     0,     0,
       0,   372,     0,   402,   403,   404,     0,   405,     0,     0,
     406,   407,     0,   373,     0,     0,     0,     0,   374,     0,
     408,   375,   409,   410,   376,   333,     0,     0,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
     161,   162,   163,     0,   165,   166,   167,   168,   169,   379,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,   183,   184,   185,     0,     0,   188,   189,   190,
     191,     0,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,   382,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   606,   607,   608,     0,     0,     0,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,   383,
     384,   385,   386,   387,     0,   388,     0,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,    54,   400,
     401,     0,     0,     0,     0,     0,   367,   368,   369,   370,
     371,     0,     0,     0,     0,     0,     0,     0,   372,     0,
     402,   403,   404,     0,   405,     0,     0,   406,   407,     0,
     373,     0,     0,     0,     0,   374,     0,   408,   375,   409,
     410,   376,   333,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   379,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,   183,
     184,   185,     0,     0,   188,   189,   190,   191,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,   382,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   993,   994,
     995,     0,     0,     0,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,   383,   384,   385,   386,
     387,     0,   388,     0,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,    54,   400,   401,     0,     0,
       0,     0,     0,   367,   368,   369,   370,   371,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   402,   403,   404,
       0,   405,     0,     0,   406,   407,     0,   373,     0,     0,
       0,     0,   374,     0,   408,   375,   409,   410,   376,   333,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1003,  1004,  1005,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
     367,   368,   369,   370,   371,     0,     0,     0,     0,     0,
       0,     0,   372,     0,   402,   403,   404,     0,   405,     0,
       0,   406,   407,     0,   373,     0,     0,     0,     0,   374,
       0,   408,   375,   409,   410,   376,   333,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,   161,   162,   163,     0,   165,   166,   167,   168,   169,
     379,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,   183,   184,   185,     0,     0,   188,   189,
     190,   191,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,   382,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
     383,   384,   385,   386,   387,     0,   388,     0,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,    54,
     400,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   403,   404,     0,   405,     0,     0,   406,   407,
     367,   368,   369,   370,   371,     0,   719,     0,   408,   720,
     409,   410,   372,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   373,     0,     0,     0,     0,   374,
       0,     0,   375,     0,     0,   376,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,   161,   162,   163,     0,   165,   166,   167,   168,   169,
     379,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,   183,   184,   185,     0,     0,   188,   189,
     190,   191,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,   382,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
     383,   384,   385,   386,   387,     0,   388,     0,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,    54,
     400,   401,     0,     0,     0,     0,     0,   367,   368,   369,
     370,   371,     0,     0,   752,     0,     0,     0,     0,   372,
       0,   402,   403,   404,     0,   405,     0,     0,   406,   407,
       0,   373,     0,     0,     0,     0,   374,   441,   408,   375,
     409,   410,   376,   333,     0,     0,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,   161,   162,
     163,     0,   165,   166,   167,   168,   169,   379,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
     183,   184,   185,     0,     0,   188,   189,   190,   191,     0,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,   382,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,   383,   384,   385,
     386,   387,     0,   388,     0,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,    54,   400,   401,     0,
       0,     0,     0,     0,   367,   368,   369,   370,   371,     0,
       0,     0,     0,     0,     0,     0,   372,     0,   402,   403,
     404,     0,   405,     0,     0,   406,   407,     0,   373,     0,
       0,     0,     0,   374,     0,   408,   375,   409,   410,   376,
     333,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,   161,   162,   163,     0,   165,
     166,   167,   168,   169,   379,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,   183,   184,   185,
       0,     0,   188,   189,   190,   191,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,   382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,   383,   384,   385,   386,   387,     0,
     388,     0,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,    54,   400,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,   403,   404,     0,   405,
       0,     0,   406,   407,   367,   368,   369,   370,   371,     0,
       0,     0,   408,   772,   409,   410,   372,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   373,     0,
       0,     0,     0,   374,     0,     0,   375,     0,     0,   376,
       0,     0,     0,   377,     0,     0,     0,     0,     0,   792,
       0,     0,     0,   378,     0,   161,   162,   163,     0,   165,
     166,   167,   168,   169,   379,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,   183,   184,   185,
       0,     0,   188,   189,   190,   191,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,   382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,   383,   384,   385,   386,   387,     0,
     388,     0,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,    54,   400,   401,     0,     0,     0,     0,
       0,   367,   368,   369,   370,   371,     0,     0,     0,     0,
       0,     0,     0,   372,     0,   402,   403,   404,     0,   405,
       0,     0,   406,   407,     0,   373,     0,     0,     0,     0,
     374,     0,   408,   375,   409,   410,   376,   333,     0,     0,
     377,     0,     0,   798,     0,     0,     0,     0,     0,     0,
     378,     0,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   379,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,   183,   184,   185,     0,     0,   188,
     189,   190,   191,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,   383,   384,   385,   386,   387,     0,   388,     0,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
      54,   400,   401,     0,     0,     0,     0,     0,   367,   368,
     369,   370,   371,     0,     0,     0,     0,     0,     0,     0,
     372,     0,   402,   403,   404,     0,   405,     0,     0,   406,
     407,     0,   373,     0,     0,     0,     0,   374,     0,   408,
     375,   409,   410,   376,   333,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   379,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,   183,   184,   185,     0,     0,   188,   189,   190,   191,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,   383,   384,
     385,   386,   387,     0,   388,     0,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,    54,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   524,     0,   402,
     403,   404,     0,   405,     0,     0,   406,   407,   367,   368,
     369,   370,   371,     0,     0,     0,   408,     0,   409,   410,
     372,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,   374,     0,     0,
     375,     0,     0,   376,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   379,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,   183,   184,   185,     0,     0,   188,   189,   190,   191,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,   383,   384,
     385,   386,   387,     0,   388,     0,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,    54,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     403,   404,     0,   405,     0,     0,   406,   407,   367,   368,
     369,   370,   371,     0,     0,     0,   408,   870,   409,   410,
     372,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,   374,     0,     0,
     375,     0,     0,   376,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   379,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,   183,   184,   185,     0,     0,   188,   189,   190,   191,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,   383,   384,
     385,   386,   387,     0,   388,     0,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,    54,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   872,     0,   402,
     403,   404,     0,   405,     0,     0,   406,   407,   367,   368,
     369,   370,   371,     0,     0,     0,   408,     0,   409,   410,
     372,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,   374,     0,     0,
     375,     0,     0,   376,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   379,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,   183,   184,   185,     0,     0,   188,   189,   190,   191,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,   383,   384,
     385,   386,   387,     0,   388,     0,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,    54,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     403,   404,     0,   405,     0,     0,   406,   407,   367,   368,
     369,   370,   371,     0,     0,     0,   408,   892,   409,   410,
     372,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,   374,     0,     0,
     375,     0,     0,   376,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   379,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,   183,   184,   185,     0,     0,   188,   189,   190,   191,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,   383,   384,
     385,   386,   387,     0,   388,     0,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,    54,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     403,   404,     0,   405,     0,     0,   406,   407,   367,   368,
     369,   370,   371,     0,     0,     0,   408,   907,   409,   410,
     372,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,   374,     0,     0,
     375,     0,     0,   376,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   379,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,   183,   184,   185,     0,     0,   188,   189,   190,   191,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,   383,   384,
     385,   386,   387,     0,   388,     0,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,    54,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,   368,   369,   370,   371,     0,   402,
     403,   404,     0,   405,     0,   372,   406,   407,     0,     0,
       0,     0,     0,     0,  1057,     0,   408,   373,   409,   410,
       0,   333,   374,     0,     0,   375,     0,     0,   376,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,   403,   404,     0,   405,     0,
       0,   406,   407,   367,   368,   369,   370,   371,     0,     0,
       0,   408,  1070,   409,   410,   372,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,     0,     0,
       0,     0,   374,     0,     0,   375,     0,     0,   376,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,   403,   404,     0,   405,     0,
       0,   406,   407,   367,   368,   369,   370,   371,     0,     0,
       0,   408,  1118,   409,   410,   372,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,     0,     0,
       0,     0,   374,     0,     0,   375,     0,     0,   376,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,   403,   404,     0,   405,     0,
       0,   406,   407,   367,   368,   369,   370,   371,     0,     0,
       0,   408,  1170,   409,   410,   372,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,     0,     0,
       0,     0,   374,     0,     0,   375,     0,     0,   376,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,  1184,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
     367,   368,   369,   370,   371,     0,     0,     0,     0,     0,
       0,     0,   372,     0,   402,   403,   404,     0,   405,     0,
       0,   406,   407,     0,   373,     0,     0,     0,     0,   374,
       0,   408,   375,   409,   410,   376,   333,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,   161,   162,   163,     0,   165,   166,   167,   168,   169,
     379,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,   183,   184,   185,     0,     0,   188,   189,
     190,   191,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,   382,     0,
       0,     0,     0,     0,     0,     0,  1232,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
     383,   384,   385,   386,   387,     0,   388,     0,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,    54,
     400,   401,     0,     0,     0,     0,     0,   367,   368,   369,
     370,   371,     0,     0,     0,     0,     0,     0,     0,   372,
       0,   402,   403,   404,     0,   405,     0,     0,   406,   407,
       0,   373,     0,     0,     0,     0,   374,     0,   408,   375,
     409,   410,   376,   333,     0,     0,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,   161,   162,
     163,     0,   165,   166,   167,   168,   169,   379,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
     183,   184,   185,     0,     0,   188,   189,   190,   191,     0,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,   382,     0,     0,     0,     0,
       0,     0,     0,  1234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,   383,   384,   385,
     386,   387,     0,   388,     0,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,    54,   400,   401,     0,
       0,     0,     0,     0,   367,   368,   369,   370,   371,     0,
       0,     0,     0,     0,     0,     0,   372,     0,   402,   403,
     404,     0,   405,     0,     0,   406,   407,     0,   373,     0,
       0,     0,     0,   374,     0,   408,   375,   409,   410,   376,
     333,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,   161,   162,   163,     0,   165,
     166,   167,   168,   169,   379,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,   183,   184,   185,
       0,     0,   188,   189,   190,   191,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,   382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   537,   538,     0,
       0,     0,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,   383,   384,   385,   386,   387,     0,
     388,     0,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,    54,   400,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,   403,   404,     0,   405,
       0,     0,   406,   407,     0,     0,     0,     0,     0,     0,
       0,     0,   408,     0,   409,   410,     0,   333,   823,   824,
     825,   826,   827,   828,   829,   830,   539,   540,   541,   542,
     543,   831,   832,   544,   545,   546,   547,   966,   548,   549,
     550,   551,     0,   537,   538,     0,   552,   834,   553,   554,
     835,   836,     0,     0,   555,   556,   557,   837,   838,   839,
     558,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   537,
     538,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   967,   559,     0,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,   571,     0,     0,
       0,     0,     0,  1015,   823,   824,   825,   826,   827,   828,
     829,   830,   539,   540,   541,   542,   543,   831,   832,   544,
     545,   546,   547,   966,   548,   549,   550,   551,     0,     0,
       0,     0,   552,   834,   553,   554,   835,   836,     0,     0,
     555,   556,   557,   837,   838,   839,   558,     0,   539,   540,
     541,   542,   543,     0,     0,   544,   545,   546,   547,     0,
     548,   549,   550,   551,   537,   538,     0,     0,   552,     0,
     553,   554,     0,     0,     0,     0,   555,   556,   557,     0,
     967,   559,   558,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,     0,     0,     0,     0,     0,     0,     0,
     537,   538,   570,   571,     0,     0,     0,     0,     0,  1109,
       0,     0,     0,     0,     0,     0,     0,   559,     0,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   570,   571,
       0,     0,     0,     0,     0,  1203,     0,     0,     0,     0,
       0,     0,     0,   539,   540,   541,   542,   543,     0,     0,
     544,   545,   546,   547,     0,   548,   549,   550,   551,     0,
       0,     0,     0,   552,     0,   553,   554,     0,     0,     0,
       0,   555,   556,   557,     0,     0,     0,   558,     0,   539,
     540,   541,   542,   543,     0,     0,   544,   545,   546,   547,
       0,   548,   549,   550,   551,   537,   538,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,   555,   556,   557,
       0,     0,   559,   558,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,     0,     0,     0,     0,     0,     0,
       0,   537,   538,   570,   571,     0,     0,   572,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
     571,     0,     0,   713,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   539,   540,   541,   542,   543,     0,
       0,   544,   545,   546,   547,     0,   548,   549,   550,   551,
       0,     0,     0,     0,   552,     0,   553,   554,     0,     0,
       0,     0,   555,   556,   557,     0,     0,     0,   558,     0,
     539,   540,   541,   542,   543,     0,     0,   544,   545,   546,
     547,     0,   548,   549,   550,   551,   537,   538,     0,     0,
     552,     0,   553,   554,     0,     0,     0,     0,   555,   556,
     557,     0,     0,   559,   558,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,     0,     0,     0,     0,     0,
       0,     0,   537,   538,   570,   571,     0,     0,   847,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   559,
       0,   560,   561,   562,   563,   564,   565,   566,   567,   568,
     569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,   571,     0,     0,   875,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   539,   540,   541,   542,   543,
       0,     0,   544,   545,   546,   547,     0,   548,   549,   550,
     551,     0,     0,     0,     0,   552,     0,   553,   554,     0,
       0,     0,     0,   555,   556,   557,     0,     0,     0,   558,
       0,   539,   540,   541,   542,   543,     0,     0,   544,   545,
     546,   547,     0,   548,   549,   550,   551,   537,   538,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,     0,     0,   559,   558,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,     0,     0,     0,     0,
       0,     0,     0,   537,   538,   570,   571,     0,     0,   878,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,     0,     0,   880,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   539,   540,   541,   542,
     543,     0,     0,   544,   545,   546,   547,     0,   548,   549,
     550,   551,     0,     0,     0,     0,   552,     0,   553,   554,
       0,     0,     0,     0,   555,   556,   557,     0,     0,     0,
     558,     0,   539,   540,   541,   542,   543,     0,     0,   544,
     545,   546,   547,     0,   548,   549,   550,   551,   537,   538,
       0,     0,   552,     0,   553,   554,     0,     0,     0,     0,
     555,   556,   557,     0,     0,   559,   558,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,     0,
       0,     0,     0,     0,   537,   538,   570,   571,     0,     0,
     882,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   559,     0,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,   571,     0,     0,   883,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   539,   540,   541,
     542,   543,     0,     0,   544,   545,   546,   547,     0,   548,
     549,   550,   551,     0,     0,     0,     0,   552,     0,   553,
     554,     0,     0,     0,     0,   555,   556,   557,     0,     0,
       0,   558,     0,   539,   540,   541,   542,   543,     0,     0,
     544,   545,   546,   547,     0,   548,   549,   550,   551,   537,
     538,     0,     0,   552,     0,   553,   554,     0,     0,     0,
       0,   555,   556,   557,     0,     0,   559,   558,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,     0,   537,   538,   570,   571,     0,
       0,   884,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,   571,     0,     0,   885,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   539,   540,
     541,   542,   543,     0,     0,   544,   545,   546,   547,     0,
     548,   549,   550,   551,     0,     0,     0,     0,   552,     0,
     553,   554,     0,     0,     0,     0,   555,   556,   557,     0,
       0,     0,   558,     0,   539,   540,   541,   542,   543,     0,
       0,   544,   545,   546,   547,     0,   548,   549,   550,   551,
     537,   538,     0,     0,   552,     0,   553,   554,     0,     0,
       0,     0,   555,   556,   557,     0,     0,   559,   558,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,     0,
       0,     0,     0,     0,     0,     0,   537,   538,   570,   571,
       0,     0,   886,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   559,     0,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   570,   571,     0,     0,   887,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   539,
     540,   541,   542,   543,     0,     0,   544,   545,   546,   547,
       0,   548,   549,   550,   551,     0,     0,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,   555,   556,   557,
       0,     0,     0,   558,     0,   539,   540,   541,   542,   543,
       0,     0,   544,   545,   546,   547,     0,   548,   549,   550,
     551,   537,   538,     0,     0,   552,     0,   553,   554,     0,
       0,     0,     0,   555,   556,   557,     0,     0,   559,   558,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
       0,     0,     0,     0,     0,     0,     0,   537,   538,   570,
     571,     0,     0,   955,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,   571,     0,     0,  1022,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     539,   540,   541,   542,   543,     0,     0,   544,   545,   546,
     547,     0,   548,   549,   550,   551,     0,     0,     0,     0,
     552,     0,   553,   554,     0,     0,     0,     0,   555,   556,
     557,     0,     0,     0,   558,     0,   539,   540,   541,   542,
     543,     0,     0,   544,   545,   546,   547,     0,   548,   549,
     550,   551,   537,   538,     0,     0,   552,     0,   553,   554,
       0,     0,     0,     0,   555,   556,   557,     0,     0,   559,
     558,   560,   561,   562,   563,   564,   565,   566,   567,   568,
     569,     0,     0,     0,     0,     0,     0,     0,   537,   538,
     570,   571,     0,     0,  1082,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   559,     0,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,   571,     0,     0,
    1130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   539,   540,   541,   542,   543,     0,     0,   544,   545,
     546,   547,     0,   548,   549,   550,   551,     0,     0,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,     0,     0,     0,   558,     0,   539,   540,   541,
     542,   543,     0,     0,   544,   545,   546,   547,     0,   548,
     549,   550,   551,   537,   538,     0,     0,   552,     0,   553,
     554,     0,     0,     0,     0,   555,   556,   557,     0,     0,
     559,   558,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,   537,
     538,   570,   571,     0,     0,  1131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,   571,     0,
       0,  1142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   539,   540,   541,   542,   543,     0,     0,   544,
     545,   546,   547,     0,   548,   549,   550,   551,     0,     0,
       0,     0,   552,     0,   553,   554,     0,     0,     0,     0,
     555,   556,   557,     0,     0,     0,   558,     0,   539,   540,
     541,   542,   543,     0,     0,   544,   545,   546,   547,     0,
     548,   549,   550,   551,   537,   538,     0,     0,   552,     0,
     553,   554,     0,     0,     0,     0,   555,   556,   557,     0,
       0,   559,   558,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,     0,     0,     0,     0,     0,     0,     0,
     537,   538,   570,   571,     0,     0,  1144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   570,   571,
       0,     0,  1146,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   539,   540,   541,   542,   543,     0,     0,
     544,   545,   546,   547,     0,   548,   549,   550,   551,     0,
       0,     0,     0,   552,     0,   553,   554,     0,     0,     0,
       0,   555,   556,   557,     0,     0,     0,   558,     0,   539,
     540,   541,   542,   543,     0,     0,   544,   545,   546,   547,
       0,   548,   549,   550,   551,   537,   538,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,   555,   556,   557,
       0,     0,   559,   558,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,     0,     0,     0,     0,     0,     0,
       0,   537,   538,   570,   571,     0,     0,  1150,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
     571,     0,     0,  1189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   539,   540,   541,   542,   543,     0,
       0,   544,   545,   546,   547,     0,   548,   549,   550,   551,
       0,     0,     0,     0,   552,     0,   553,   554,     0,     0,
       0,     0,   555,   556,   557,     0,     0,     0,   558,     0,
     539,   540,   541,   542,   543,     0,     0,   544,   545,   546,
     547,     0,   548,   549,   550,   551,   537,   538,     0,     0,
     552,     0,   553,   554,     0,     0,     0,     0,   555,   556,
     557,     0,     0,   559,   558,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,     0,     0,     0,     0,     0,
       0,     0,   537,   538,   570,   571,     0,     0,  1190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   559,
       0,   560,   561,   562,   563,   564,   565,   566,   567,   568,
     569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,   571,     0,     0,  1191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   539,   540,   541,   542,   543,
       0,     0,   544,   545,   546,   547,     0,   548,   549,   550,
     551,     0,     0,     0,     0,   552,     0,   553,   554,     0,
       0,     0,     0,   555,   556,   557,     0,     0,     0,   558,
       0,   539,   540,   541,   542,   543,     0,     0,   544,   545,
     546,   547,     0,   548,   549,   550,   551,   537,   538,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,     0,     0,   559,   558,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,     0,     0,     0,     0,
       0,     0,     0,   537,   538,   570,   571,     0,     0,  1200,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,     0,     0,  1202,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   539,   540,   541,   542,
     543,     0,     0,   544,   545,   546,   547,     0,   548,   549,
     550,   551,     0,     0,     0,     0,   552,     0,   553,   554,
       0,     0,     0,     0,   555,   556,   557,     0,     0,     0,
     558,     0,   539,   540,   541,   542,   543,   537,   538,   544,
     545,   546,   547,     0,   548,   549,   550,   551,     0,     0,
       0,     0,   552,     0,   553,   554,     0,     0,     0,     0,
     555,   556,   557,     0,     0,   559,   558,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,   571,     0,     0,
    1238,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   559,     0,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,   571,   575,     0,   539,   540,   541,   542,
     543,   537,   538,   544,   545,   546,   547,     0,   548,   549,
     550,   551,     0,     0,     0,     0,   552,     0,   553,   554,
       0,     0,     0,     0,   555,   556,   557,     0,     0,     0,
     558,     0,     0,     0,     0,     0,     0,   537,   538,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   559,     0,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,   571,   938,     0,
     539,   540,   541,   542,   543,     0,     0,   544,   545,   546,
     547,     0,   548,   549,   550,   551,     0,     0,     0,     0,
     552,     0,   553,   554,     0,     0,     0,     0,   555,   556,
     557,     0,     0,     0,   558,     0,   539,   540,   541,   542,
     543,   537,   538,   544,   545,   546,   547,     0,   548,   549,
     550,   551,     0,     0,     0,     0,   552,     0,   553,   554,
       0,     0,     0,     0,   555,   556,   557,     0,     0,   559,
     558,   560,   561,   562,   563,   564,   565,   566,   567,   568,
     569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,   571,   952,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   559,     0,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,   571,  1077,     0,
     539,   540,   541,   542,   543,     0,     0,   544,   545,   546,
     547,     0,   548,   549,   550,   551,   537,   538,     0,     0,
     552,     0,   553,   554,     0,     0,     0,     0,   555,   556,
     557,     0,     0,     0,   558,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   559,
       0,   560,   561,   562,   563,   564,   565,   566,   567,   568,
     569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,   571,  1080,     0,     0,     0,     0,   823,   824,   825,
     826,   827,   828,   829,   830,   539,   540,   541,   542,   543,
     831,   832,   544,   545,   546,   547,   966,   548,   549,   550,
     551,  -317,     0,   278,   279,   552,   834,   553,   554,   835,
     836,     0,     0,   555,   556,   557,   837,   838,   839,   558,
     280,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   537,
     538,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   967,   559,     0,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,   571,     0,     0,     0,
       0,     0,     0,     0,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,     0,     0,   299,   300,   301,     0,     0,     0,
       0,     0,     0,   302,   303,   304,   305,   306,     0,     0,
     307,   308,   309,   310,   311,   312,   313,     0,   539,   540,
     541,   542,   543,   537,   538,   544,   545,   546,   547,     0,
     548,   549,   550,   551,     0,     0,     0,     0,   552,     0,
     553,   554,     0,     0,   703,     0,   555,   556,   557,     0,
       0,   314,   558,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,     0,     0,   325,   326,     0,     0,     0,
       0,     0,   327,   328,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,     0,
     725,     0,     0,     0,     0,     0,     0,     0,   570,   571,
       0,     0,   539,   540,   541,   542,   543,   537,   538,   544,
     545,   546,   547,     0,   548,   549,   550,   551,     0,     0,
       0,     0,   552,     0,   553,   554,     0,     0,   893,     0,
     555,   556,   557,     0,     0,     0,   558,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   379,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,   183,
     184,   185,     0,     0,   188,   189,   190,   191,     0,     0,
       0,   559,     0,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,   571,     0,     0,   539,   540,   541,   542,
     543,   537,   538,   544,   545,   546,   547,     0,   548,   549,
     550,   551,     0,     0,     0,     0,   552,     0,   553,   554,
       0,     0,     0,   726,   555,   556,   557,     0,     0,     0,
     558,     0,     0,     0,     0,   727,     0,   537,   538,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   559,   943,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,   571,     0,     0,
     539,   540,   541,   542,   543,     0,     0,   544,   545,   546,
     547,     0,   548,   549,   550,   551,     0,     0,     0,     0,
     552,     0,   553,   554,     0,     0,     0,     0,   555,   556,
     557,     0,   537,   538,   558,     0,   539,   540,   541,   542,
     543,     0,     0,   544,   545,   546,   547,     0,   548,   549,
     550,   551,     0,     0,     0,     0,   552,     0,   553,   554,
       0,  1050,     0,     0,   555,   556,   557,   537,   538,   559,
     558,   560,   561,   562,   563,   564,   565,   566,   567,   568,
     569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,   571,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   559,     0,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,     0,
       0,   539,   540,   541,   542,   543,   570,   571,   544,   545,
     546,   547,     0,   548,   549,   550,   551,     0,     0,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,   537,   538,     0,  -654,   539,   540,   541,   542,
     543,     0,     0,   544,   545,   546,   547,     0,   548,   549,
     550,   551,     0,     0,     0,     0,   552,     0,   553,   554,
       0,     0,     0,     0,   555,   556,   557,   537,   538,     0,
     559,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   559,     0,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,     0,
       0,   539,   540,   541,   542,   543,   570,   571,   544,   545,
     546,   547,     0,   548,   549,   550,   551,     0,     0,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
       0,   557,   537,   538,     0,     0,   539,   540,   541,   542,
     543,     0,     0,   544,   545,   546,   547,     0,   548,   549,
     550,   551,     0,     0,     0,     0,   552,     0,   553,   554,
     537,   538,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,     0,
       0,   539,   540,   541,   542,   543,   570,   571,   544,   545,
     546,   547,     0,   548,   549,   550,   551,     0,     0,     0,
       0,   552,     0,   553,   554,   537,   538,     0,     0,   539,
     540,   541,   542,   543,     0,     0,   544,   545,   546,   547,
       0,   548,   549,   550,   551,     0,     0,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   562,   563,   564,   565,   566,   567,   568,   569,
       0,     0,     0,     0,   539,   540,   541,   542,   543,   570,
     571,   544,   545,   546,   547,     0,   548,   549,   550,   551,
       0,     0,     0,     0,   552,     0,   553,   554,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     517,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   157,     0,     0,     0,     0,     0,   158,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   563,   564,
     565,   566,   567,   568,   569,   159,     0,     0,     0,     0,
       0,     0,     0,     0,   570,   571,     0,     0,     0,     0,
     160,     0,     0,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   519,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   157,     0,     0,     0,     0,     0,   158,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   159,     0,     0,   194,   729,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   160,
       0,    54,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   161,   162,   163,     0,   165,
     166,   167,   168,   169,   379,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,   183,   184,   185,
       0,     0,   188,   189,   190,   191,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   939,     0,     0,   194,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     161,   162,   163,   731,   165,   166,   167,   168,   169,   379,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,   183,   184,   185,     0,     0,   188,   189,   190,
     191,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   157,     0,
       0,     0,     0,     0,   158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   159,     0,     0,     0,     0,   940,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   160,     0,   941,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   157,     0,     0,
       0,     0,     0,   158,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     159,     0,     0,   194,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   160,     0,    54,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   207,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   194,     0,     0,     0,     0,   208,     0,   209,
       0,   210,   211,   212,   213,   214,   491,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,     0,   226,
     227,   228,     0,     0,   229,   230,   231,   232,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   233,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   235
};

static const yytype_int16 yycheck[] =
{
      12,   273,   123,   144,   237,   463,   581,   486,   360,   488,
     362,   490,   364,   520,   585,   470,   896,   594,   695,     0,
     719,   420,   123,   573,   701,     6,   448,     7,   605,    14,
      15,    21,   606,   607,   608,    19,    32,    90,    51,    48,
      52,   135,   125,    55,    56,    51,   120,   120,    29,   120,
      31,    19,    33,   517,    19,   519,    44,    45,    39,   130,
     119,    44,    32,   122,   123,    55,   163,    48,   122,   123,
      20,    21,   179,    54,   120,   652,    18,    19,   136,    91,
      92,    93,    94,   163,   130,   137,   138,   139,   122,   123,
     136,   120,   203,    74,   163,   206,   163,   151,    20,    21,
     207,   130,   176,   177,   177,   179,   177,   180,   182,   163,
     207,   123,   100,   207,   163,   689,    97,   100,   390,   391,
     120,   179,   672,   206,   204,   163,   176,   175,   120,   593,
     130,   177,   156,    14,    15,   202,   145,    32,   130,   208,
     199,   200,   163,   163,   179,   199,   200,  1181,   177,   184,
     175,   203,   273,  1033,   163,   177,   206,   163,   206,   208,
     208,   111,   112,    58,    59,   199,   200,   202,    60,   119,
     208,   156,   122,   123,   124,  1209,   161,   177,   163,   129,
     165,   206,   182,   208,   206,   177,   182,   208,   208,   111,
     112,   244,   176,   206,    20,    21,   895,   119,   188,   121,
     122,   123,   124,   475,   200,   346,   783,   129,   176,   199,
     787,   176,   182,   209,   176,   892,   176,   202,    43,   200,
     200,   163,   117,   184,   801,   206,   121,   176,   208,   176,
      20,    21,   353,   354,   355,   356,   508,   177,   359,   201,
     361,  1121,   363,   642,   365,   206,   148,   821,   670,   199,
     200,   153,   353,   354,   355,   356,    81,   206,   359,   206,
     361,   273,   363,   185,   186,   187,   188,   189,    52,   390,
     391,    55,    56,   850,   179,   156,   145,   199,   200,   176,
     161,   163,   163,   178,   165,   111,   112,   182,    33,   184,
     185,   180,    32,   119,   163,   121,   122,   123,   124,   176,
     173,   203,   175,   129,   176,   200,   203,    91,    92,    93,
      94,   176,   145,   163,   209,   151,    61,   206,    58,    59,
     176,   111,   112,   172,   201,   174,   447,   163,   163,   119,
     163,   121,   122,   123,   124,   208,   201,   174,   888,   129,
     461,   353,   354,   355,   356,   201,   447,   359,   176,   361,
     163,   363,   201,   365,   475,  1032,   906,   369,  1057,   137,
     461,   139,   136,   596,   201,   820,  1043,   136,   176,   206,
     184,   604,   724,   199,   200,   203,   145,   117,   390,   391,
     120,   121,   163,   164,   129,   892,   936,   508,   959,   163,
     130,   136,   206,   405,   163,   203,    55,   187,   188,   189,
     907,   176,    61,   151,   676,   677,   136,   679,   189,   199,
     200,   136,   176,   963,    32,   163,   537,   538,   163,   993,
     994,   995,   184,   153,   154,   155,   176,   176,   203,  1003,
    1004,  1005,   176,   163,   151,   447,   708,   177,   178,   203,
      58,    59,   182,   184,   206,   185,   163,   136,   193,   461,
     176,   201,   931,   183,   203,   467,   145,   202,    69,   203,
     200,   150,    73,   475,  1035,   206,   206,     4,     5,   209,
       7,   202,   179,   176,   163,   201,   179,   184,  1028,   182,
     156,    92,    93,    94,    95,   179,   179,   148,   500,    32,
     184,   184,   153,    11,   183,   202,   508,   202,    35,   117,
      55,    11,   207,   121,    22,    23,    61,    55,   202,   202,
      20,    21,   199,    61,    55,    58,    59,    55,   180,    55,
      61,   533,   184,    61,   180,    61,   180,   180,   184,   180,
     184,   184,   180,   184,   655,   180,   184,   180,   180,   184,
     180,   184,   184,   163,   184,   666,   177,   668,   163,   164,
     671,   206,   785,   674,   655,   676,   677,   207,   679,   206,
     178,   794,   166,   167,   182,   666,   184,   185,   920,   163,
     671,   166,   167,   674,   117,   808,  1055,   163,   121,   812,
      76,    77,   200,   166,   167,   168,   169,   708,   207,  1164,
      55,   209,   204,   205,   163,   207,   100,    32,   207,   109,
     110,   111,   112,   113,  1224,  1225,   116,   117,   118,   119,
      32,   121,   122,   123,   124,   137,   138,   139,    32,   129,
     153,   131,   132,    58,    59,   137,   183,   137,   138,   139,
    1088,   752,   183,   143,   183,   178,    58,    59,   183,   182,
     202,   184,   185,   655,    58,    59,   202,   919,   183,   166,
     167,   168,   183,   183,   666,   183,   668,   200,   183,   671,
     781,   180,   674,   183,   676,   677,   209,   679,   178,   163,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     781,    34,   117,   206,    34,   163,   121,   202,   163,   199,
     200,   207,   179,   206,   208,   117,   708,   163,   201,   121,
     163,   163,    21,   117,   163,   846,   201,   121,   179,   207,
     179,   202,    41,     9,   207,   203,   202,   202,   202,   183,
     202,   202,   183,   183,    20,    21,   183,   183,   202,   202,
     202,   183,   202,   163,   202,   968,   163,   163,   205,   184,
     153,    41,   200,   178,   163,   163,   201,   182,   202,   184,
     185,   202,   201,   206,   202,   202,   178,   156,    36,     9,
     182,    64,   184,   185,   178,   200,   206,   206,   182,   781,
     184,   185,   206,   206,   209,   206,    41,   206,   200,   163,
     184,   206,   262,   184,   184,   176,   200,   209,   176,   166,
     202,   271,   208,   202,  1066,   209,   163,   183,   919,   202,
     183,     1,   163,   163,   202,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   202,
     202,   843,  1065,   129,   130,   131,   132,   133,   134,   202,
     125,   137,   138,   139,   140,   141,   142,   143,    13,   177,
      20,    21,   179,   205,   156,     7,  1089,   163,   207,   163,
     202,   201,   163,   201,   201,   163,   163,   166,   163,   163,
     183,   202,   202,    41,   206,   163,   202,   202,   201,   201,
     201,   177,   178,   203,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   206,   163,   201,   184,   206,    32,
     184,   381,   382,   199,   200,    51,    69,   919,   201,   201,
     201,   525,   392,   203,    38,   201,   120,    72,   201,   208,
    1193,  1154,   402,   403,   404,  1158,   406,   407,   408,   206,
    1194,   249,    70,   843,   978,  1194,  1194,  1194,  1194,   109,
     110,   111,   112,   113,     1,  1066,   116,  1068,  1063,   119,
     881,   121,   122,   123,   124,  1104,  1163,  1107,   587,   129,
     668,   131,   132,   648,   366,    47,   358,  1068,   702,  1127,
     904,    -1,   464,   369,   369,  1178,   705,    -1,   520,    -1,
      -1,    -1,    -1,   463,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   472,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1237,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,   187,   188,   189,
      -1,    -1,   502,    -1,    32,   505,  1147,   507,    -1,   199,
     200,   511,   512,   513,   514,   515,   516,    -1,   518,    -1,
     520,    -1,    -1,    -1,    -1,    -1,  1147,    -1,    -1,    -1,
      58,    59,    -1,    -1,  1066,    -1,  1068,    -1,    -1,   539,
     540,    -1,    -1,   543,   544,   545,   546,    -1,   548,    -1,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
      -1,   571,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     580,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     590,    -1,    -1,   121,   594,    20,    21,   597,   598,    -1,
      -1,    -1,    -1,   603,    -1,   605,    -1,    -1,    -1,   609,
      -1,    -1,    -1,    -1,   614,  1147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   639,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1181,
     178,    -1,   652,    -1,   182,    -1,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,   200,   673,    -1,    -1,    30,  1209,    -1,    -1,
      -1,   209,    -1,    -1,   109,   110,   111,   112,    -1,    -1,
      -1,   691,   692,    47,   119,    -1,   121,   122,   123,   124,
     700,    -1,    -1,   703,   129,   705,   131,   132,    62,    -1,
     710,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
     185,   186,   187,   188,   189,   765,    -1,    -1,    -1,   769,
      -1,    -1,    -1,    -1,   199,   200,   776,    -1,    -1,    -1,
      -1,    -1,   136,   783,    -1,    58,    59,   787,    -1,    -1,
      -1,    -1,   792,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,   801,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   823,   824,   825,   826,   827,   828,   829,
     830,   831,   832,   833,   834,   835,   836,   837,   838,   839,
     840,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,    -1,
     850,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     890,   891,    -1,   893,    -1,    -1,    -1,    -1,    20,    21,
      -1,   901,    -1,    -1,   904,   178,    -1,    -1,    -1,   182,
      -1,    -1,   185,    -1,    -1,    -1,    -1,   917,   918,    -1,
      -1,    -1,    -1,   923,    -1,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,   933,    -1,   935,   209,   937,    -1,    -1,
      -1,    -1,    -1,   943,    -1,    -1,    -1,   947,   109,   110,
     111,   112,   113,   953,    -1,   116,   117,   118,   119,    -1,
     121,   122,   123,   124,    -1,    -1,   966,   967,   129,    -1,
     131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,
     980,    -1,   143,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,    -1,   178,    -1,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,   200,
      -1,    -1,    -1,    -1,    20,    21,   207,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,    -1,    -1,
      -1,    -1,    -1,    -1,  1074,    -1,  1076,   199,   200,  1079,
      -1,    -1,    -1,    -1,  1084,    -1,    -1,  1087,  1088,    -1,
      -1,    -1,  1092,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1111,    -1,    -1,  1114,  1115,  1116,  1117,    -1,    -1,
    1120,    -1,    -1,  1123,  1124,    -1,  1126,    -1,    -1,  1129,
      -1,    -1,    -1,   109,   110,   111,   112,   113,  1138,    -1,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1184,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1194,    -1,    -1,    -1,    -1,    -1,
      -1,  1201,   178,    -1,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,    -1,    -1,  1216,  1217,    -1,    -1,
      -1,    -1,    -1,   199,   200,    -1,    -1,  1227,     1,    -1,
     206,    -1,  1232,     6,  1234,     8,     9,    10,    -1,    12,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,   147,   148,    -1,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,   187,    -1,   189,    -1,    -1,   192,
     193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
      -1,   204,   205,   206,   207,   208,     1,    -1,    -1,    -1,
      -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,   146,   147,   148,    -1,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,   187,    -1,   189,    -1,    -1,   192,   193,    -1,
      -1,    -1,    14,    15,    16,    17,    18,   202,    -1,   204,
     205,   206,   207,   208,    26,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    51,
      -1,    53,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
     182,    -1,    -1,   185,   186,   187,    -1,   189,    -1,    -1,
     192,   193,    14,    15,    16,    17,    18,    -1,   200,    -1,
     202,    -1,   204,   205,    26,   207,    -1,   209,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
     182,    -1,    -1,   185,   186,   187,    -1,   189,    -1,    -1,
     192,   193,    14,    15,    16,    17,    18,    -1,   200,    -1,
     202,    -1,   204,   205,    26,   207,    -1,   209,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,    -1,    -1,   185,   186,   187,    -1,   189,    -1,    -1,
     192,   193,    14,    15,    16,    17,    18,    -1,   200,    -1,
     202,    -1,   204,   205,    26,   207,    -1,   209,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   185,   186,   187,    -1,   189,    -1,    -1,
     192,   193,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     202,    46,   204,   205,    49,   207,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,   146,   147,   148,    -1,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,   187,    -1,   189,    -1,    -1,   192,   193,    -1,
      -1,    -1,    14,    15,    16,    17,    18,   202,    -1,   204,
     205,    -1,   207,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   185,   186,   187,    -1,   189,    -1,    -1,
     192,   193,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     202,    46,   204,   205,    49,   207,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,   146,   147,   148,    -1,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     185,   186,   187,    -1,   189,    -1,    -1,   192,   193,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   202,    46,   204,
     205,    49,   207,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,
     148,    -1,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   185,   186,   187,
      -1,   189,    -1,    -1,   192,   193,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   202,    46,   204,   205,    49,   207,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   185,   186,   187,    -1,   189,    -1,
      -1,   192,   193,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   202,    46,   204,   205,    49,   207,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,   147,   148,    -1,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,   187,    -1,   189,    -1,    -1,   192,   193,
      14,    15,    16,    17,    18,    -1,   200,    -1,   202,   203,
     204,   205,    26,   207,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,   147,   148,    -1,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,
      -1,   185,   186,   187,    -1,   189,    -1,    -1,   192,   193,
      -1,    38,    -1,    -1,    -1,    -1,    43,   201,   202,    46,
     204,   205,    49,   207,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
     147,   148,    -1,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   185,   186,
     187,    -1,   189,    -1,    -1,   192,   193,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   202,    46,   204,   205,    49,
     207,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,   187,    -1,   189,
      -1,    -1,   192,   193,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   202,   203,   204,   205,    26,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   185,   186,   187,    -1,   189,
      -1,    -1,   192,   193,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   202,    46,   204,   205,    49,   207,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,   147,   148,    -1,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   185,   186,   187,    -1,   189,    -1,    -1,   192,
     193,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   202,
      46,   204,   205,    49,   207,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,   185,
     186,   187,    -1,   189,    -1,    -1,   192,   193,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   202,    -1,   204,   205,
      26,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,   187,    -1,   189,    -1,    -1,   192,   193,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   202,   203,   204,   205,
      26,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,   185,
     186,   187,    -1,   189,    -1,    -1,   192,   193,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   202,    -1,   204,   205,
      26,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,   187,    -1,   189,    -1,    -1,   192,   193,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   202,   203,   204,   205,
      26,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,   187,    -1,   189,    -1,    -1,   192,   193,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   202,   203,   204,   205,
      26,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,   185,
     186,   187,    -1,   189,    -1,    26,   192,   193,    -1,    -1,
      -1,    -1,    -1,    -1,   200,    -1,   202,    38,   204,   205,
      -1,   207,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,   187,    -1,   189,    -1,
      -1,   192,   193,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   202,   203,   204,   205,    26,   207,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,   187,    -1,   189,    -1,
      -1,   192,   193,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   202,   203,   204,   205,    26,   207,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,   187,    -1,   189,    -1,
      -1,   192,   193,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   202,   203,   204,   205,    26,   207,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   185,   186,   187,    -1,   189,    -1,
      -1,   192,   193,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   202,    46,   204,   205,    49,   207,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,   147,   148,    -1,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   185,   186,   187,    -1,   189,    -1,    -1,   192,   193,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   202,    46,
     204,   205,    49,   207,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
     147,   148,    -1,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   185,   186,
     187,    -1,   189,    -1,    -1,   192,   193,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   202,    46,   204,   205,    49,
     207,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,   187,    -1,   189,
      -1,    -1,   192,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,    -1,   204,   205,    -1,   207,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    -1,    20,    21,    -1,   129,   130,   131,   132,
     133,   134,    -1,    -1,   137,   138,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,    -1,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   199,   200,    -1,    -1,
      -1,    -1,    -1,   206,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,   129,   130,   131,   132,   133,   134,    -1,    -1,
     137,   138,   139,   140,   141,   142,   143,    -1,   109,   110,
     111,   112,   113,    -1,    -1,   116,   117,   118,   119,    -1,
     121,   122,   123,   124,    20,    21,    -1,    -1,   129,    -1,
     131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,
     177,   178,   143,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,   199,   200,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,   200,
      -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    -1,    -1,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    20,    21,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    -1,   178,   143,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,   199,   200,    -1,    -1,   203,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,
     200,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,    -1,
      -1,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,   178,   143,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,   199,   200,    -1,    -1,   203,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     199,   200,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,   109,   110,   111,   112,   113,    -1,    -1,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    20,    21,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,   178,   143,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,   199,   200,    -1,    -1,   203,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   199,   200,    -1,    -1,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,    -1,    -1,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
     143,    -1,   109,   110,   111,   112,   113,    -1,    -1,   116,
     117,   118,   119,    -1,   121,   122,   123,   124,    20,    21,
      -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,
     137,   138,   139,    -1,    -1,   178,   143,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,   199,   200,    -1,    -1,
     203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   199,   200,    -1,    -1,   203,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,   143,    -1,   109,   110,   111,   112,   113,    -1,    -1,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    20,
      21,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,   178,   143,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,   199,   200,    -1,
      -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   199,   200,    -1,    -1,   203,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
     111,   112,   113,    -1,    -1,   116,   117,   118,   119,    -1,
     121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
     131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,
      -1,    -1,   143,    -1,   109,   110,   111,   112,   113,    -1,
      -1,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      20,    21,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,   138,   139,    -1,    -1,   178,   143,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,   199,   200,
      -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   199,   200,    -1,    -1,   203,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,   143,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    20,    21,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,   178,   143,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,   199,
     200,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   199,   200,    -1,    -1,   203,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,    -1,   109,   110,   111,   112,
     113,    -1,    -1,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    20,    21,    -1,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,   178,
     143,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
     199,   200,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   199,   200,    -1,    -1,
     203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    -1,    -1,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    20,    21,    -1,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
     178,   143,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,   199,   200,    -1,    -1,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,   200,    -1,
      -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,   111,   112,   113,    -1,    -1,   116,
     117,   118,   119,    -1,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,
     137,   138,   139,    -1,    -1,    -1,   143,    -1,   109,   110,
     111,   112,   113,    -1,    -1,   116,   117,   118,   119,    -1,
     121,   122,   123,   124,    20,    21,    -1,    -1,   129,    -1,
     131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,
      -1,   178,   143,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,   199,   200,    -1,    -1,   203,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,   200,
      -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    -1,    -1,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    20,    21,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    -1,   178,   143,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,   199,   200,    -1,    -1,   203,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,
     200,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,    -1,
      -1,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,   178,   143,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,   199,   200,    -1,    -1,   203,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     199,   200,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,   109,   110,   111,   112,   113,    -1,    -1,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    20,    21,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,   178,   143,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,   199,   200,    -1,    -1,   203,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   199,   200,    -1,    -1,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,    -1,    -1,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
     143,    -1,   109,   110,   111,   112,   113,    20,    21,   116,
     117,   118,   119,    -1,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,
     137,   138,   139,    -1,    -1,   178,   143,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   199,   200,    -1,    -1,
     203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   199,   200,   201,    -1,   109,   110,   111,   112,
     113,    20,    21,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   199,   200,   201,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,    -1,   109,   110,   111,   112,
     113,    20,    21,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,   178,
     143,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   199,   200,   201,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    20,    21,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     199,   200,   201,    -1,    -1,    -1,    -1,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,    20,    21,   129,   130,   131,   132,   133,
     134,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,    -1,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,
     137,   138,   139,   140,   141,   142,   143,    -1,   109,   110,
     111,   112,   113,    20,    21,   116,   117,   118,   119,    -1,
     121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
     131,   132,    -1,    -1,   135,    -1,   137,   138,   139,    -1,
      -1,   178,   143,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,    -1,    -1,   192,   193,    -1,    -1,    -1,
      -1,    -1,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,   200,
      -1,    -1,   109,   110,   111,   112,   113,    20,    21,   116,
     117,   118,   119,    -1,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,   129,    -1,   131,   132,    -1,    -1,   135,    -1,
     137,   138,   139,    -1,    -1,    -1,   143,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      -1,   178,    -1,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   199,   200,    -1,    -1,   109,   110,   111,   112,
     113,    20,    21,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,   151,   137,   138,   139,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,   163,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   199,   200,    -1,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,
     139,    -1,    20,    21,   143,    -1,   109,   110,   111,   112,
     113,    -1,    -1,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
      -1,   170,    -1,    -1,   137,   138,   139,    20,    21,   178,
     143,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   199,   200,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    20,    21,    -1,   143,   109,   110,   111,   112,
     113,    -1,    -1,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,   138,   139,    20,    21,    -1,
     178,    -1,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   199,   200,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
      -1,   139,    20,    21,    -1,    -1,   109,   110,   111,   112,
     113,    -1,    -1,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   199,   200,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,    20,    21,    -1,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,   184,   185,   186,   187,
     188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   199,
     200,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
     185,   186,   187,   188,   189,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   199,   200,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,   149,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,   163,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    66,    67,   163,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,   163,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,   163,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    65,    -1,    67,
      -1,    69,    70,    71,    72,    73,   163,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   163
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   211,     0,     6,    29,    31,    33,    39,    48,    54,
      74,    97,   200,   206,   214,   222,   223,   228,   256,   260,
     286,   354,   360,   363,   369,   408,   411,    18,    19,   163,
     249,   250,   251,   156,   229,   230,   163,   164,   189,   224,
     225,   226,   163,   207,   357,   163,   204,   213,   412,   409,
      33,    61,   129,   136,   163,   193,   202,   252,   253,   254,
     255,   273,     4,     5,     7,    35,   367,    60,   352,   177,
     176,   179,   176,   225,    21,    55,   188,   199,   227,   358,
     357,   359,   352,   163,   163,   254,   163,   136,   254,   254,
     202,   137,   138,   139,   176,   201,    55,    61,   261,   263,
      55,    61,   361,    55,    61,   368,    55,    61,   353,    14,
      15,   156,   161,   163,   165,   202,   216,   250,   156,   230,
     163,   224,   224,   177,   206,   208,   357,    55,    61,   212,
     207,   410,   163,   203,   251,   254,   254,   254,   254,   264,
     163,   362,   370,   207,   355,   166,   167,   215,    14,    15,
     156,   161,   163,   216,   247,   248,   227,    24,    30,    47,
      62,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,   149,   273,   373,   375,   376,   378,
     382,   407,   206,   163,   377,   207,   203,    34,    65,    67,
      69,    70,    71,    72,    73,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    87,    88,    89,    92,
      93,    94,    95,   111,   112,   163,   259,   262,   179,   207,
     100,   365,   366,   350,   153,   329,   166,   167,   168,   176,
     203,   183,   183,   183,   183,   202,   183,   183,   183,   183,
     183,   183,   202,    32,    58,    59,   117,   121,   178,   182,
     185,   200,   209,   180,   206,   163,   342,   343,    20,    21,
      37,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   121,
     122,   123,   130,   131,   132,   133,   134,   137,   138,   139,
     140,   141,   142,   143,   178,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   192,   193,   199,   200,    34,
      34,   202,   257,   207,   265,    69,    73,    92,    93,    94,
      95,   374,   356,   163,   371,   208,   351,   251,   145,   163,
     348,   349,   247,   385,   387,   389,   383,   163,   379,   391,
     393,   395,   397,   399,   401,   403,   405,    14,    15,    16,
      17,    18,    26,    38,    43,    46,    49,    53,    63,    74,
      98,   111,   112,   144,   145,   146,   147,   148,   150,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     164,   165,   185,   186,   187,   189,   192,   193,   202,   204,
     205,   218,   220,   265,   269,   273,   278,   289,   296,   299,
     302,   306,   307,   310,   311,   313,   318,   321,   328,   373,
     413,   421,   425,   427,   429,    32,   182,    32,   182,   200,
     209,   201,   321,   209,   382,   163,   208,   179,   206,   163,
     163,   163,   201,    21,   163,   201,   148,   203,   329,   338,
     339,   179,   258,   267,   207,   163,   208,   179,   364,   207,
     329,   201,   202,    41,   176,   179,   182,   347,   407,   381,
     407,   407,   407,   203,   377,   407,   257,   407,   257,   407,
     257,   163,   340,   341,   407,   343,   183,   275,   375,   413,
     202,   183,   202,   183,   183,   202,   183,   202,   183,   321,
     321,   202,   202,   202,   202,   202,   202,    12,   382,    12,
     382,   321,   423,   426,   183,   217,   321,   321,   321,   273,
     321,   321,   321,   205,   163,   202,   256,    20,    21,   109,
     110,   111,   112,   113,   116,   117,   118,   119,   121,   122,
     123,   124,   129,   131,   132,   137,   138,   139,   143,   178,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     199,   200,   203,   202,   201,   201,   407,   208,   342,   163,
     202,    48,   145,   163,   345,   372,   203,   206,   407,     1,
       8,     9,    10,    12,    25,    27,    28,    37,    40,    42,
      50,    52,    56,    57,    63,    99,   126,   127,   128,   164,
     206,   208,   231,   232,   235,   238,   239,   241,   242,   243,
     244,   245,   266,   268,   272,   274,   279,   280,   281,   282,
     283,   284,   285,   286,   288,   308,   309,   321,   356,   177,
     206,   273,   334,   349,   201,   321,   163,   163,   382,   120,
     130,   177,   346,   184,   184,   206,   184,   184,   184,   206,
     184,   258,   184,   258,   184,   258,   179,   184,   206,   184,
     206,   276,   202,   273,   297,   321,   290,   292,   321,   294,
     321,   382,   321,   321,   321,   321,   321,   321,   372,    51,
     163,   185,   202,   321,   414,   415,   416,   422,   424,   372,
     202,   415,   424,   135,   175,   206,   208,   418,   270,   168,
     169,   215,   221,   203,   145,   150,   183,   273,   312,   200,
     203,   300,   321,   153,   305,    18,   151,   163,   373,    18,
     151,   163,   373,   321,   321,   321,   321,   321,   321,   163,
     321,   151,   163,   321,   321,   321,   321,   321,   321,   321,
     321,   321,    21,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   122,   123,   151,   163,   199,   200,
     319,   321,   203,   300,   321,   345,   202,    41,   120,   176,
     177,   179,   182,   344,   338,   321,   372,   120,   272,   308,
     321,   265,    59,   321,   321,   163,   206,   156,    56,   321,
     265,   120,   272,   321,   205,   306,   306,   306,   321,    36,
     206,   206,   321,     9,   206,   206,   206,   206,   206,    64,
     287,   125,   206,   101,   102,   103,   104,   105,   106,   107,
     108,   114,   115,   120,   130,   133,   134,   140,   141,   142,
     177,   208,   321,   200,   208,   256,   335,   203,    41,   206,
     346,   272,   321,   386,   388,   407,   390,   384,   380,   392,
     184,   396,   184,   400,   184,   407,   404,   340,   406,   407,
     203,   300,   183,   321,   407,   203,   382,   382,   203,   382,
     203,   184,   203,   203,   203,   203,   203,   203,    19,   306,
     130,   344,   203,   135,   176,   206,   416,   172,   174,   201,
     419,   176,   174,   201,   206,   417,    19,   203,   416,   418,
     321,   423,   208,   382,   321,   166,   219,   202,   202,   314,
     316,   163,   414,   176,   203,   120,   130,   177,   182,   303,
     304,   257,   183,   202,   183,   202,   202,   202,   201,    18,
     151,   163,   373,   179,   151,   163,   321,   202,   151,   163,
     321,     1,   201,   176,   203,   203,   321,   163,   163,   407,
     272,   321,   265,    19,   272,   321,   120,   177,    13,   321,
     265,   177,   179,   156,   272,   321,   205,   265,   207,   265,
     240,   349,   306,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   126,   127,   128,   321,   321,   321,   321,
     321,   321,   321,   126,   127,   128,   321,   206,   255,     7,
     329,   333,   163,   272,   321,   206,   394,   398,   402,   184,
     203,   163,   203,   184,   184,   184,   184,   305,   202,   300,
     321,   321,   415,   416,   321,   163,   414,   419,   201,   321,
     422,   201,   300,   415,   173,   175,   208,   420,   201,   184,
     170,   166,   321,   321,   382,   257,   201,   200,   321,   163,
     163,   163,   163,   176,   201,   258,   322,   321,   324,   321,
     203,   300,   321,   183,   202,   321,   202,   201,   321,   202,
     201,   320,   203,    41,   344,   300,   265,   246,   267,    11,
      22,    23,   233,   234,   321,   306,   306,   306,   306,   306,
     306,   201,    55,    61,   332,    44,   100,   330,   206,   206,
     277,   184,   206,   298,   291,   293,   295,   202,   203,   300,
     206,   416,   419,   130,   344,   201,   206,   416,   201,   271,
     203,   203,   184,   258,   203,   414,   303,   201,   135,   265,
     301,   382,   203,   407,   203,   203,   203,   326,   321,   321,
     203,   321,   163,   321,   206,   321,   208,   265,   321,    11,
     236,   206,    45,   330,   332,   321,   163,   321,   321,   321,
     203,   321,   203,   321,   419,   201,   321,   321,   321,   420,
     321,   315,   184,   201,   120,   321,   184,   184,   407,   203,
     203,   203,   265,   265,   237,   206,    32,   331,   331,   345,
     203,   184,   203,   206,   428,   201,   428,   201,   312,   317,
     203,   321,   323,   325,   184,   234,    25,    99,   238,   279,
     280,   281,   283,   321,   336,   337,   321,    51,   201,   208,
     312,   327,   120,   321,   120,   321,   262,   262,   203,   321,
     201,   201,   321,   321,   206,   265
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   210,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   212,   212,   212,   213,   213,
     214,   215,   215,   215,   215,   216,   217,   217,   217,   218,
     219,   219,   221,   220,   222,   223,   224,   224,   225,   225,
     225,   225,   226,   226,   227,   227,   228,   229,   229,   230,
     230,   231,   232,   232,   233,   233,   234,   234,   234,   235,
     235,   236,   237,   236,   238,   238,   238,   238,   238,   239,
     240,   239,   241,   242,   243,   244,   246,   245,   247,   247,
     247,   247,   247,   247,   248,   248,   249,   249,   249,   250,
     250,   250,   250,   250,   250,   250,   250,   251,   251,   252,
     252,   252,   253,   253,   254,   254,   254,   254,   254,   254,
     254,   255,   255,   256,   256,   257,   257,   257,   258,   258,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   260,   261,   261,   261,   262,   264,
     263,   265,   265,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   267,   267,   267,   268,   270,   271,
     269,   272,   272,   272,   272,   273,   273,   273,   274,   274,
     276,   277,   275,   275,   278,   278,   278,   278,   279,   280,
     281,   281,   281,   282,   282,   282,   283,   283,   284,   284,
     284,   285,   286,   286,   287,   287,   288,   290,   291,   289,
     292,   293,   289,   294,   295,   289,   297,   298,   296,   299,
     299,   299,   300,   300,   301,   301,   301,   302,   302,   302,
     303,   303,   303,   303,   304,   304,   305,   305,   306,   306,
     307,   307,   307,   307,   307,   307,   307,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   309,   309,   309,   309,
     309,   309,   310,   310,   311,   311,   312,   312,   313,   314,
     315,   313,   316,   317,   313,   318,   318,   319,   320,   318,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   322,
     323,   321,   321,   321,   321,   324,   325,   321,   321,   321,
     326,   327,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   329,   329,   330,   330,   330,   331,   331,   332,
     332,   332,   333,   334,   335,   334,   336,   334,   337,   334,
     334,   338,   338,   338,   339,   339,   340,   340,   341,   341,
     342,   343,   343,   344,   344,   345,   345,   345,   345,   345,
     345,   346,   346,   346,   347,   347,   348,   348,   348,   348,
     348,   349,   349,   349,   349,   349,   350,   351,   350,   352,
     352,   353,   353,   353,   354,   355,   354,   356,   356,   356,
     358,   357,   359,   359,   360,   360,   361,   361,   361,   362,
     363,   363,   364,   364,   365,   365,   366,   367,   367,   368,
     368,   368,   370,   371,   369,   372,   372,   372,   372,   372,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   374,   374,   374,
     374,   374,   374,   375,   376,   376,   376,   377,   377,   379,
     380,   378,   381,   381,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   383,   384,   382,   382,   385,   386,   382,   387,
     388,   382,   389,   390,   382,   382,   391,   392,   382,   393,
     394,   382,   382,   395,   396,   382,   397,   398,   382,   382,
     399,   400,   382,   401,   402,   382,   403,   404,   382,   405,
     406,   382,   407,   407,   407,   409,   410,   408,   412,   411,
     413,   413,   413,   413,   414,   414,   414,   414,   415,   415,
     416,   416,   417,   417,   418,   418,   419,   419,   419,   420,
     420,   420,   421,   421,   421,   421,   421,   421,   422,   422,
     422,   423,   423,   424,   424,   425,   425,   426,   426,   427,
     428,   428,   429,   429
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     0,     1,     1,     1,     1,
       4,     1,     1,     2,     2,     3,     0,     2,     4,     3,
       1,     2,     0,     4,     2,     2,     1,     1,     1,     2,
       3,     3,     2,     4,     0,     1,     2,     1,     3,     1,
       3,     3,     3,     2,     1,     1,     0,     2,     4,     1,
       1,     0,     0,     3,     1,     1,     1,     1,     1,     4,
       0,     6,     6,     2,     3,     3,     0,     5,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     1,
       1,     1,     1,     4,     1,     2,     3,     3,     3,     3,
       2,     1,     3,     0,     3,     0,     2,     3,     0,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     2,     2,     3,     3,
       2,     2,     2,     2,     2,     3,     3,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     1,     1,     3,     0,
       4,     3,     7,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     2,     2,     2,     0,     2,     2,     3,     0,     0,
       7,     3,     2,     2,     2,     1,     3,     2,     2,     3,
       0,     0,     5,     1,     2,     4,     5,     2,     1,     1,
       1,     2,     3,     2,     2,     3,     2,     3,     2,     2,
       3,     4,     1,     1,     1,     0,     3,     0,     0,     7,
       0,     0,     7,     0,     0,     7,     0,     0,     6,     5,
       8,    10,     1,     3,     1,     2,     3,     1,     1,     2,
       2,     2,     2,     2,     1,     3,     0,     4,     1,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     4,     4,
       4,     4,     6,     8,     5,     6,     1,     4,     3,     0,
       0,     8,     0,     0,     9,     3,     4,     0,     0,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       3,     4,     5,     4,     5,     3,     4,     1,     3,     4,
       3,     4,     2,     4,     4,     7,     8,     3,     5,     0,
       0,     8,     3,     3,     3,     0,     0,     8,     3,     4,
       0,     0,     9,     4,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     4,     1,     4,     4,     4,     4,
       4,     1,     6,     7,     6,     6,     7,     7,     6,     7,
       6,     6,     0,     4,     0,     1,     1,     0,     1,     0,
       1,     1,     4,     0,     0,     4,     0,     9,     0,     9,
       5,     2,     3,     4,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     1,     1,     2,     3,     5,     3,
       3,     1,     1,     1,     0,     1,     1,     4,     3,     3,
       5,     4,     6,     5,     5,     4,     0,     0,     4,     0,
       1,     0,     1,     1,     6,     0,     6,     0,     3,     5,
       0,     4,     2,     3,     4,     2,     0,     1,     1,     1,
       7,     9,     0,     2,     0,     1,     3,     1,     1,     0,
       1,     1,     0,     0,     9,     1,     4,     3,     3,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     1,     3,     0,
       0,     6,     1,     3,     1,     1,     1,     1,     4,     3,
       4,     2,     2,     3,     2,     3,     2,     2,     3,     3,
       3,     2,     0,     0,     6,     2,     0,     0,     6,     0,
       0,     6,     0,     0,     6,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     0,     0,     6,     0,
       0,     6,     1,     3,     3,     0,     0,     8,     0,     7,
       1,     1,     1,     1,     3,     3,     5,     5,     1,     3,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     5,     7,     8,     6,     8,     1,     3,
       3,     3,     1,     1,     3,     5,     5,     1,     3,     4,
       0,     3,    10,    10
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = DAS_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL

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

    case YYSYMBOL_KEYWORD: /* "keyword"  */
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

    case YYSYMBOL_keyword_or_name: /* keyword_or_name  */
            { delete ((*yyvaluep).s); }
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

    case YYSYMBOL_expression_block: /* expression_block  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_any: /* expression_any  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expressions: /* expressions  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_keyword: /* expr_keyword  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expression_keyword: /* expression_keyword  */
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
            { delete ((*yyvaluep).aTypePair).firstType; delete ((*yyvaluep).aTypePair).secondType; }
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

  case 36: /* keyword_or_name: "name"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 37: /* keyword_or_name: "keyword"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 38: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 39: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 40: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 41: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 42: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 43: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 44: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 45: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 49: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 50: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 51: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 52: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 53: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 54: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 55: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 56: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 57: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 58: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 59: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 60: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 61: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 62: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 63: /* expression_else_one_liner: "else" $@2 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 64: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 65: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 66: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 67: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 69: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 70: /* $@3: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 71: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner ';'  */
                                                                                                                                                   {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 72: /* expression_for_loop: "for" variable_name_with_pos_list "in" expr_list ';' expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 73: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 74: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 75: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 76: /* $@4: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 77: /* expression_with_alias: "assume" "name" '=' $@4 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 78: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 79: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 80: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 81: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 82: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 83: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 84: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 85: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 86: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 87: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 88: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 89: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 90: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 91: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 92: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 93: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 94: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 95: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 96: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 97: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 98: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 99: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 100: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 101: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 102: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
    break;

  case 103: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 104: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 105: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 106: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::And,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 107: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
                                                                            {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Or,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 108: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Xor,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 109: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 110: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 111: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 112: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 113: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 114: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 115: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 116: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 117: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 118: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 119: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 120: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 121: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 122: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 123: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 124: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 125: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 126: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 127: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 128: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 129: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 130: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 131: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 132: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 133: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 134: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 135: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 136: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 137: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 138: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 139: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 140: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 141: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 142: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 143: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 144: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 145: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 146: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 147: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 148: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 149: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 150: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 151: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 152: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 153: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 154: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 155: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 156: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 157: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 158: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 159: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 160: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 161: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 162: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 163: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 164: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 165: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 166: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 167: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 168: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 169: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 170: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 171: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 172: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 173: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 174: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 175: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 176: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 177: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 178: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 179: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 180: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 181: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 182: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 183: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 184: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 185: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 186: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 187: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 188: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 189: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 190: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 191: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 192: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 193: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 194: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 195: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 196: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 197: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 198: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 199: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 200: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 201: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 202: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 203: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 204: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 205: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 206: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 207: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 208: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 209: /* $@5: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 210: /* function_declaration: optional_public_or_private_function $@5 function_declaration_header expression_block  */
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

  case 211: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 212: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 213: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 214: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 215: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 216: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 217: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 218: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 219: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 220: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 221: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 222: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 226: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 227: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 232: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 233: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 234: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 235: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 236: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 237: /* expr_keyword: "keyword" expr expression_block  */
                                                           {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        auto resT = new TypeDecl(Type::autoinfer);
        auto blk = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,resT,(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])));
        pCall->arguments.push_back(ExpressionPtr(blk));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 238: /* $@6: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 239: /* $@7: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 240: /* expression_keyword: "keyword" '<' $@6 type_declaration_no_options '>' $@7 expr  */
                                                                                                                                               {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),*(yyvsp[-6].s));
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCall->arguments.push_back(ExpressionPtr(td));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 241: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 242: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 243: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 244: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 245: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 246: /* name_in_namespace: "name" "::" "name"  */
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

  case 247: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 248: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 249: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 250: /* $@8: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 251: /* $@9: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 252: /* new_type_declaration: '<' $@8 type_declaration '>' $@9  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 253: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 254: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 255: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 256: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 257: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 258: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 259: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 260: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 261: /* expression_return_no_pipe: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 262: /* expression_return_no_pipe: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 263: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 264: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 265: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 266: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 267: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 268: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 269: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 270: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 271: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 272: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 273: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 274: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 275: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 276: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 277: /* $@10: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 278: /* $@11: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 279: /* expr_cast: "cast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 280: /* $@12: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 281: /* $@13: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 282: /* expr_cast: "upcast" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 283: /* $@14: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 284: /* $@15: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 285: /* expr_cast: "reinterpret" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 286: /* $@16: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 287: /* $@17: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 288: /* expr_type_decl: "type" '<' $@16 type_declaration '>' $@17  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 289: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 290: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 291: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 292: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 293: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 294: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 295: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 296: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 297: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 298: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 299: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 300: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 301: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 302: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 303: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 304: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 305: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 306: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 307: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 308: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 309: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 310: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 311: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 312: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 313: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 314: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 315: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 316: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 317: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 318: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 319: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 320: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 325: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 326: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 335: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr_assign_pipe: expr '=' "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr_assign_pipe: expr "<-" "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 343: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 344: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 345: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 346: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 347: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 348: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 349: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 350: /* $@19: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 351: /* func_addr_expr: '@' '@' '<' $@18 type_declaration_no_options '>' $@19 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 352: /* $@20: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 353: /* $@21: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 354: /* func_addr_expr: '@' '@' '<' $@20 optional_function_argument_list optional_function_type '>' $@21 func_addr_name  */
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

  case 355: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 356: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 357: /* $@22: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 358: /* $@23: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 359: /* expr_field: expr '.' $@22 error $@23  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 360: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 361: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 362: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 363: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 364: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 366: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 367: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 368: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 369: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 370: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 396: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 399: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 400: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 401: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 402: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 403: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 404: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 405: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 406: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 407: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 408: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 409: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 410: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 411: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 412: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 413: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 414: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 415: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 416: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 417: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 418: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 419: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 420: /* $@25: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 421: /* expr: expr "is" "type" '<' $@24 type_declaration_no_options '>' $@25  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 422: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 423: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 424: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 425: /* $@26: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 426: /* $@27: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 427: /* expr: expr "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 428: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 429: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 430: /* $@28: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 431: /* $@29: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 432: /* expr: expr '?' "as" "type" '<' $@28 type_declaration '>' $@29  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 433: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 434: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 435: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 436: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 437: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 438: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 439: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 440: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 442: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 443: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 444: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 445: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 446: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 447: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 448: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 449: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 450: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 451: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 452: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 453: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 454: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 455: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 456: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 457: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 458: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 459: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 460: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 461: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 462: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 463: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 464: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 465: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 466: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 467: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 468: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 469: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 470: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 471: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 472: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 473: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 474: /* $@30: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 475: /* struct_variable_declaration_list: struct_variable_declaration_list $@30 structure_variable_declaration ';'  */
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

  case 476: /* $@31: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 477: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@31 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 478: /* $@32: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 479: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@32 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 480: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 481: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 482: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 483: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 484: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 485: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 486: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 487: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 488: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 489: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 490: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 491: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 492: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 493: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 494: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 495: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 496: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 497: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 498: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 499: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 500: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 501: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 502: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 503: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 504: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 505: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 506: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 507: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 508: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 509: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 510: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 511: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 512: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 513: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 514: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 515: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 516: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 517: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 518: /* global_variable_declaration_list: global_variable_declaration_list $@33 optional_field_annotation let_variable_declaration  */
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

  case 519: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 520: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 521: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 522: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 523: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 524: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 525: /* $@34: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 526: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@34 optional_field_annotation let_variable_declaration  */
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

  case 527: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 528: /* enum_list: enum_list "name" ';'  */
                                     {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt(scanner,(yylsp[-1]))) ) {
            das_yyerror(scanner,"enumeration already declared " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
    break;

  case 529: /* enum_list: enum_list "name" '=' expr ';'  */
                                                     {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt(scanner,(yylsp[-3]))) ) {
            das_yyerror(scanner,"enumeration value already declared " + *(yyvsp[-3].s), tokAt(scanner,(yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
    break;

  case 530: /* $@35: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 531: /* single_alias: "name" $@35 '=' type_declaration  */
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

  case 536: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 537: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 538: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 539: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 540: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 541: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 542: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 543: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 544: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 545: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 546: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 547: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 548: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 549: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 550: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 551: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 552: /* $@36: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 553: /* $@37: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 554: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@36 structure_name $@37 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        if ( (yyvsp[-4].pStructure) ) {
            ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
            if ( !yyextra->g_CommentReaders.empty() ) {
                auto tak = tokAt(scanner,(yylsp[-7]));
                for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
            }
        }
    }
    break;

  case 555: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 556: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 557: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 558: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 559: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 560: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 561: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 562: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 563: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 564: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 565: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 566: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 567: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 568: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 569: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 570: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 571: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 572: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 573: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 574: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 575: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 576: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 577: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 578: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 579: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 580: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 581: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 582: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 583: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 584: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 585: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 586: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 587: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 588: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 589: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 590: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 591: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 592: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 593: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 594: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 595: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 596: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 597: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 598: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 599: /* $@38: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 600: /* $@39: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 601: /* bitfield_type_declaration: "bitfield" '<' $@38 bitfield_bits '>' $@39  */
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

  case 602: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 603: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 604: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 605: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 606: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 607: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 608: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
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

  case 609: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 610: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 611: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 612: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 613: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 614: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 615: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 616: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 617: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 618: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 619: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 620: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 621: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 622: /* $@40: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 623: /* $@41: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 624: /* type_declaration_no_options: "smart_ptr" '<' $@40 type_declaration '>' $@41  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 625: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 626: /* $@42: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 627: /* $@43: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 628: /* type_declaration_no_options: "array" '<' $@42 type_declaration '>' $@43  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 629: /* $@44: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 630: /* $@45: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 631: /* type_declaration_no_options: "table" '<' $@44 table_type_pair '>' $@45  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 632: /* $@46: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 633: /* $@47: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 634: /* type_declaration_no_options: "iterator" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 635: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 636: /* $@48: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 637: /* $@49: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 638: /* type_declaration_no_options: "block" '<' $@48 type_declaration '>' $@49  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 639: /* $@50: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 640: /* $@51: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 641: /* type_declaration_no_options: "block" '<' $@50 optional_function_argument_list optional_function_type '>' $@51  */
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

  case 642: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 643: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 644: /* $@53: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 645: /* type_declaration_no_options: "function" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 646: /* $@54: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 647: /* $@55: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 648: /* type_declaration_no_options: "function" '<' $@54 optional_function_argument_list optional_function_type '>' $@55  */
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

  case 649: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 650: /* $@56: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 651: /* $@57: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 652: /* type_declaration_no_options: "lambda" '<' $@56 type_declaration '>' $@57  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 653: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 654: /* $@59: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 655: /* type_declaration_no_options: "lambda" '<' $@58 optional_function_argument_list optional_function_type '>' $@59  */
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

  case 656: /* $@60: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 657: /* $@61: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 658: /* type_declaration_no_options: "tuple" '<' $@60 tuple_type_list '>' $@61  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 659: /* $@62: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 660: /* $@63: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 661: /* type_declaration_no_options: "variant" '<' $@62 variant_type_list '>' $@63  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 662: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 663: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 664: /* type_declaration: type_declaration '|' '#'  */
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

  case 665: /* $@64: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 666: /* $@65: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 667: /* variant_alias_declaration: "variant" $@64 "name" $@65 '{' variant_type_list ';' '}'  */
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

  case 668: /* $@66: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 669: /* bitfield_alias_declaration: "bitfield" $@66 "name" '{' bitfield_bits ';' '}'  */
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

  case 670: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 671: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 672: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 673: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 674: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 675: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 676: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 677: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 678: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 679: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 680: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 681: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 692: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_br ']'  */
                                                                                                                                   {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 693: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_br ']'  */
                                                                                                              {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 694: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_br ']'  */
                                                                                                                      {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 695: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_br ']'  */
                                                                                                                                           {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 696: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_br ']'  */
                                                                                                                                   {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 697: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_br ']'  */
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

  case 698: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 699: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 700: /* make_tuple: make_tuple ',' expr  */
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

  case 701: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 702: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 703: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 704: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 705: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_br ']'  */
                                                                                                            {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 706: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_br ']'  */
                                                                                                            {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 707: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 708: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 709: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_br '}'  */
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

  case 710: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 711: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 712: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 713: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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
          YYNOMEM;
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


