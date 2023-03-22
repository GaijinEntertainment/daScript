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
  YYSYMBOL_LAPIPE = 126,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 127,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 128,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 129,                 /* ":="  */
  YYSYMBOL_ROTL = 130,                     /* "<<<"  */
  YYSYMBOL_ROTR = 131,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 132,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 133,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 134,                    /* "=>"  */
  YYSYMBOL_COLCOL = 135,                   /* "::"  */
  YYSYMBOL_ANDAND = 136,                   /* "&&"  */
  YYSYMBOL_OROR = 137,                     /* "||"  */
  YYSYMBOL_XORXOR = 138,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 139,                /* "&&="  */
  YYSYMBOL_OROREQU = 140,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 141,                /* "^^="  */
  YYSYMBOL_DOTDOT = 142,                   /* ".."  */
  YYSYMBOL_MTAG_E = 143,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 144,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 145,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 146,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 147,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 148,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 149,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 150,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 151,           /* "..."  */
  YYSYMBOL_BRABRAB = 152,                  /* "[["  */
  YYSYMBOL_BRACBRB = 153,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 154,                  /* "{{"  */
  YYSYMBOL_INTEGER = 155,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 156,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 157,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 158,    /* "unsigned long integer constant"  */
  YYSYMBOL_FLOAT = 159,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 160,                   /* "double constant"  */
  YYSYMBOL_NAME = 161,                     /* "name"  */
  YYSYMBOL_KEYWORD = 162,                  /* "keyword"  */
  YYSYMBOL_BEGIN_STRING = 163,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 164,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 165,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 166,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 167,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 168,          /* "}"  */
  YYSYMBOL_END_OF_READ = 169,              /* "end of failed eader macro"  */
  YYSYMBOL_170_ = 170,                     /* ','  */
  YYSYMBOL_171_ = 171,                     /* '='  */
  YYSYMBOL_172_ = 172,                     /* '?'  */
  YYSYMBOL_173_ = 173,                     /* ':'  */
  YYSYMBOL_174_ = 174,                     /* '|'  */
  YYSYMBOL_175_ = 175,                     /* '^'  */
  YYSYMBOL_176_ = 176,                     /* '&'  */
  YYSYMBOL_177_ = 177,                     /* '<'  */
  YYSYMBOL_178_ = 178,                     /* '>'  */
  YYSYMBOL_179_ = 179,                     /* '-'  */
  YYSYMBOL_180_ = 180,                     /* '+'  */
  YYSYMBOL_181_ = 181,                     /* '*'  */
  YYSYMBOL_182_ = 182,                     /* '/'  */
  YYSYMBOL_183_ = 183,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 184,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 185,               /* UNARY_PLUS  */
  YYSYMBOL_186_ = 186,                     /* '~'  */
  YYSYMBOL_187_ = 187,                     /* '!'  */
  YYSYMBOL_PRE_INC = 188,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 189,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 190,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 191,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 192,                    /* DEREF  */
  YYSYMBOL_193_ = 193,                     /* '.'  */
  YYSYMBOL_194_ = 194,                     /* '['  */
  YYSYMBOL_195_ = 195,                     /* ']'  */
  YYSYMBOL_196_ = 196,                     /* '('  */
  YYSYMBOL_197_ = 197,                     /* ')'  */
  YYSYMBOL_198_ = 198,                     /* '$'  */
  YYSYMBOL_199_ = 199,                     /* '@'  */
  YYSYMBOL_200_ = 200,                     /* ';'  */
  YYSYMBOL_201_ = 201,                     /* '{'  */
  YYSYMBOL_202_ = 202,                     /* '}'  */
  YYSYMBOL_203_ = 203,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 204,                 /* $accept  */
  YYSYMBOL_program = 205,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 206, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 207,              /* module_name  */
  YYSYMBOL_module_declaration = 208,       /* module_declaration  */
  YYSYMBOL_character_sequence = 209,       /* character_sequence  */
  YYSYMBOL_string_constant = 210,          /* string_constant  */
  YYSYMBOL_string_builder_body = 211,      /* string_builder_body  */
  YYSYMBOL_string_builder = 212,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 213, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 214,              /* expr_reader  */
  YYSYMBOL_215_1 = 215,                    /* $@1  */
  YYSYMBOL_options_declaration = 216,      /* options_declaration  */
  YYSYMBOL_require_declaration = 217,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 218,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 219,      /* require_module_name  */
  YYSYMBOL_require_module = 220,           /* require_module  */
  YYSYMBOL_is_public_module = 221,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 222,       /* expect_declaration  */
  YYSYMBOL_expect_list = 223,              /* expect_list  */
  YYSYMBOL_expect_error = 224,             /* expect_error  */
  YYSYMBOL_expression_label = 225,         /* expression_label  */
  YYSYMBOL_expression_goto = 226,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 227,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 228,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 229,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 230, /* expression_else_one_liner  */
  YYSYMBOL_231_2 = 231,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 232,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 233,  /* expression_if_then_else  */
  YYSYMBOL_234_3 = 234,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 235,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 236,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 237,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 238,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 239,    /* expression_with_alias  */
  YYSYMBOL_240_4 = 240,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 241, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 242, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 243, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 244,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 245, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 246, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 247, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 248,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 249,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 250, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 251, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 252,   /* optional_function_type  */
  YYSYMBOL_function_name = 253,            /* function_name  */
  YYSYMBOL_global_function_declaration = 254, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 255, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 256, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 257,     /* function_declaration  */
  YYSYMBOL_258_5 = 258,                    /* $@5  */
  YYSYMBOL_expression_block = 259,         /* expression_block  */
  YYSYMBOL_expression_any = 260,           /* expression_any  */
  YYSYMBOL_expressions = 261,              /* expressions  */
  YYSYMBOL_expr_keyword = 262,             /* expr_keyword  */
  YYSYMBOL_expression_keyword = 263,       /* expression_keyword  */
  YYSYMBOL_264_6 = 264,                    /* $@6  */
  YYSYMBOL_265_7 = 265,                    /* $@7  */
  YYSYMBOL_expr_pipe = 266,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 267,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 268,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 269,     /* new_type_declaration  */
  YYSYMBOL_270_8 = 270,                    /* $@8  */
  YYSYMBOL_271_9 = 271,                    /* $@9  */
  YYSYMBOL_expr_new = 272,                 /* expr_new  */
  YYSYMBOL_expression_break = 273,         /* expression_break  */
  YYSYMBOL_expression_continue = 274,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 275, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 276,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 277, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 278,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 279,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 280,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 281,        /* optional_in_scope  */
  YYSYMBOL_expression_let = 282,           /* expression_let  */
  YYSYMBOL_expr_cast = 283,                /* expr_cast  */
  YYSYMBOL_284_10 = 284,                   /* $@10  */
  YYSYMBOL_285_11 = 285,                   /* $@11  */
  YYSYMBOL_286_12 = 286,                   /* $@12  */
  YYSYMBOL_287_13 = 287,                   /* $@13  */
  YYSYMBOL_288_14 = 288,                   /* $@14  */
  YYSYMBOL_289_15 = 289,                   /* $@15  */
  YYSYMBOL_expr_type_decl = 290,           /* expr_type_decl  */
  YYSYMBOL_291_16 = 291,                   /* $@16  */
  YYSYMBOL_292_17 = 292,                   /* $@17  */
  YYSYMBOL_expr_type_info = 293,           /* expr_type_info  */
  YYSYMBOL_expr_list = 294,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 295,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 296,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 297,            /* capture_entry  */
  YYSYMBOL_capture_list = 298,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 299,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 300,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 301,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 302,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 303,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 304,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 305,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 306,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 307,           /* func_addr_expr  */
  YYSYMBOL_308_18 = 308,                   /* $@18  */
  YYSYMBOL_309_19 = 309,                   /* $@19  */
  YYSYMBOL_310_20 = 310,                   /* $@20  */
  YYSYMBOL_311_21 = 311,                   /* $@21  */
  YYSYMBOL_expr_field = 312,               /* expr_field  */
  YYSYMBOL_313_22 = 313,                   /* $@22  */
  YYSYMBOL_314_23 = 314,                   /* $@23  */
  YYSYMBOL_expr = 315,                     /* expr  */
  YYSYMBOL_316_24 = 316,                   /* $@24  */
  YYSYMBOL_317_25 = 317,                   /* $@25  */
  YYSYMBOL_318_26 = 318,                   /* $@26  */
  YYSYMBOL_319_27 = 319,                   /* $@27  */
  YYSYMBOL_320_28 = 320,                   /* $@28  */
  YYSYMBOL_321_29 = 321,                   /* $@29  */
  YYSYMBOL_expr_mtag = 322,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 323, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 324,        /* optional_override  */
  YYSYMBOL_optional_constant = 325,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 326, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 327, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 328, /* struct_variable_declaration_list  */
  YYSYMBOL_329_30 = 329,                   /* $@30  */
  YYSYMBOL_330_31 = 330,                   /* $@31  */
  YYSYMBOL_331_32 = 331,                   /* $@32  */
  YYSYMBOL_function_argument_declaration = 332, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 333,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 334,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 335,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 336,             /* variant_type  */
  YYSYMBOL_variant_type_list = 337,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 338,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 339,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 340,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 341,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 342, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 343, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 344, /* global_variable_declaration_list  */
  YYSYMBOL_345_33 = 345,                   /* $@33  */
  YYSYMBOL_optional_shared = 346,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 347, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 348,               /* global_let  */
  YYSYMBOL_349_34 = 349,                   /* $@34  */
  YYSYMBOL_enum_list = 350,                /* enum_list  */
  YYSYMBOL_single_alias = 351,             /* single_alias  */
  YYSYMBOL_352_35 = 352,                   /* $@35  */
  YYSYMBOL_alias_list = 353,               /* alias_list  */
  YYSYMBOL_alias_declaration = 354,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 355, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 356,                /* enum_name  */
  YYSYMBOL_enum_declaration = 357,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 358, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 359,          /* optional_sealed  */
  YYSYMBOL_structure_name = 360,           /* structure_name  */
  YYSYMBOL_class_or_struct = 361,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 362, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 363,    /* structure_declaration  */
  YYSYMBOL_364_36 = 364,                   /* $@36  */
  YYSYMBOL_365_37 = 365,                   /* $@37  */
  YYSYMBOL_variable_name_with_pos_list = 366, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 367,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 368, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 369, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 370,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 371,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 372, /* bitfield_type_declaration  */
  YYSYMBOL_373_38 = 373,                   /* $@38  */
  YYSYMBOL_374_39 = 374,                   /* $@39  */
  YYSYMBOL_table_type_pair = 375,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 376, /* type_declaration_no_options  */
  YYSYMBOL_377_40 = 377,                   /* $@40  */
  YYSYMBOL_378_41 = 378,                   /* $@41  */
  YYSYMBOL_379_42 = 379,                   /* $@42  */
  YYSYMBOL_380_43 = 380,                   /* $@43  */
  YYSYMBOL_381_44 = 381,                   /* $@44  */
  YYSYMBOL_382_45 = 382,                   /* $@45  */
  YYSYMBOL_383_46 = 383,                   /* $@46  */
  YYSYMBOL_384_47 = 384,                   /* $@47  */
  YYSYMBOL_385_48 = 385,                   /* $@48  */
  YYSYMBOL_386_49 = 386,                   /* $@49  */
  YYSYMBOL_387_50 = 387,                   /* $@50  */
  YYSYMBOL_388_51 = 388,                   /* $@51  */
  YYSYMBOL_389_52 = 389,                   /* $@52  */
  YYSYMBOL_390_53 = 390,                   /* $@53  */
  YYSYMBOL_391_54 = 391,                   /* $@54  */
  YYSYMBOL_392_55 = 392,                   /* $@55  */
  YYSYMBOL_393_56 = 393,                   /* $@56  */
  YYSYMBOL_394_57 = 394,                   /* $@57  */
  YYSYMBOL_395_58 = 395,                   /* $@58  */
  YYSYMBOL_396_59 = 396,                   /* $@59  */
  YYSYMBOL_397_60 = 397,                   /* $@60  */
  YYSYMBOL_398_61 = 398,                   /* $@61  */
  YYSYMBOL_399_62 = 399,                   /* $@62  */
  YYSYMBOL_400_63 = 400,                   /* $@63  */
  YYSYMBOL_type_declaration = 401,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 402, /* variant_alias_declaration  */
  YYSYMBOL_403_64 = 403,                   /* $@64  */
  YYSYMBOL_404_65 = 404,                   /* $@65  */
  YYSYMBOL_bitfield_alias_declaration = 405, /* bitfield_alias_declaration  */
  YYSYMBOL_406_66 = 406,                   /* $@66  */
  YYSYMBOL_make_decl = 407,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 408,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 409,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 410,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 411,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 412,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 413,           /* make_map_tuple  */
  YYSYMBOL_make_dim = 414,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 415,            /* make_dim_decl  */
  YYSYMBOL_make_table = 416,               /* make_table  */
  YYSYMBOL_make_table_decl = 417,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 418, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 419       /* array_comprehension  */
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
#define YYLAST   11473

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  204
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  216
/* YYNRULES -- Number of rules.  */
#define YYNRULES  698
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1226

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   431


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
       2,     2,     2,   187,     2,   203,   198,   183,   176,     2,
     196,   197,   181,   180,   170,   179,   193,   182,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   173,   200,
     177,   171,   178,   172,   199,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   194,     2,   195,   175,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   201,   174,   202,   186,     2,     2,     2,
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
     165,   166,   167,   168,   169,   184,   185,   188,   189,   190,
     191,   192
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   489,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   505,   506,   507,   511,   512,
     516,   534,   535,   536,   537,   541,   545,   550,   559,   567,
     583,   588,   596,   596,   626,   647,   651,   652,   656,   659,
     663,   669,   678,   681,   687,   688,   692,   696,   697,   701,
     704,   710,   716,   719,   725,   726,   730,   731,   732,   741,
     742,   746,   747,   747,   753,   754,   755,   756,   757,   761,
     767,   767,   773,   779,   787,   797,   806,   806,   813,   814,
     815,   816,   817,   818,   822,   827,   835,   836,   837,   841,
     842,   843,   844,   845,   846,   847,   848,   854,   857,   863,
     864,   865,   869,   877,   890,   893,   901,   912,   923,   934,
     940,   944,   951,   952,   956,   957,   958,   962,   965,   972,
     976,   977,   978,   979,   980,   981,   982,   983,   984,   985,
     986,   987,   988,   989,   990,   991,   992,   993,   994,   995,
     996,   997,   998,   999,  1000,  1001,  1002,  1003,  1004,  1005,
    1006,  1007,  1008,  1009,  1010,  1011,  1012,  1013,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,  1035,
    1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,  1044,  1045,
    1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,  1054,  1055,
    1056,  1057,  1058,  1063,  1085,  1086,  1087,  1091,  1097,  1097,
    1114,  1118,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,
    1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,
    1147,  1148,  1149,  1153,  1158,  1164,  1170,  1181,  1181,  1181,
    1192,  1226,  1229,  1235,  1236,  1247,  1251,  1254,  1262,  1262,
    1262,  1265,  1271,  1274,  1277,  1281,  1287,  1291,  1295,  1298,
    1301,  1309,  1312,  1315,  1323,  1326,  1334,  1337,  1340,  1348,
    1354,  1355,  1359,  1360,  1364,  1370,  1370,  1370,  1373,  1373,
    1373,  1378,  1378,  1378,  1386,  1386,  1386,  1392,  1402,  1413,
    1428,  1431,  1437,  1438,  1445,  1456,  1457,  1458,  1462,  1463,
    1464,  1465,  1469,  1474,  1482,  1483,  1487,  1492,  1499,  1500,
    1501,  1502,  1503,  1504,  1508,  1509,  1510,  1511,  1512,  1513,
    1514,  1515,  1516,  1517,  1518,  1519,  1520,  1521,  1522,  1523,
    1524,  1525,  1526,  1530,  1531,  1532,  1533,  1537,  1544,  1556,
    1561,  1571,  1575,  1582,  1585,  1585,  1585,  1590,  1590,  1590,
    1603,  1607,  1611,  1611,  1611,  1618,  1619,  1620,  1621,  1622,
    1623,  1624,  1625,  1626,  1627,  1628,  1629,  1630,  1631,  1632,
    1633,  1634,  1635,  1636,  1637,  1638,  1639,  1640,  1641,  1642,
    1643,  1644,  1645,  1646,  1647,  1648,  1649,  1650,  1651,  1652,
    1653,  1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,
    1668,  1669,  1670,  1671,  1675,  1679,  1682,  1685,  1686,  1687,
    1688,  1691,  1694,  1695,  1698,  1698,  1698,  1701,  1706,  1710,
    1714,  1714,  1714,  1719,  1722,  1726,  1726,  1726,  1731,  1734,
    1735,  1736,  1737,  1738,  1739,  1740,  1741,  1742,  1743,  1744,
    1749,  1753,  1754,  1755,  1756,  1757,  1758,  1759,  1763,  1767,
    1771,  1775,  1779,  1783,  1787,  1791,  1795,  1802,  1803,  1807,
    1808,  1809,  1813,  1814,  1818,  1819,  1820,  1824,  1834,  1837,
    1837,  1856,  1855,  1870,  1869,  1882,  1891,  1897,  1902,  1912,
    1913,  1917,  1920,  1929,  1930,  1934,  1943,  1944,  1949,  1950,
    1954,  1960,  1966,  1969,  1973,  1979,  1988,  1989,  1990,  1994,
    1995,  1999,  2006,  2011,  2020,  2026,  2037,  2040,  2045,  2050,
    2058,  2069,  2072,  2072,  2092,  2093,  2097,  2098,  2099,  2103,
    2106,  2106,  2125,  2128,  2137,  2150,  2150,  2171,  2172,  2176,
    2177,  2181,  2182,  2183,  2187,  2197,  2204,  2214,  2215,  2219,
    2220,  2224,  2230,  2231,  2235,  2236,  2237,  2241,  2246,  2241,
    2256,  2263,  2268,  2277,  2283,  2294,  2295,  2296,  2297,  2298,
    2299,  2300,  2301,  2302,  2303,  2304,  2305,  2306,  2307,  2308,
    2309,  2310,  2311,  2312,  2313,  2314,  2315,  2316,  2317,  2318,
    2319,  2320,  2324,  2325,  2326,  2327,  2328,  2329,  2333,  2344,
    2348,  2355,  2367,  2374,  2383,  2383,  2383,  2396,  2400,  2407,
    2408,  2409,  2410,  2411,  2425,  2431,  2435,  2439,  2444,  2449,
    2454,  2459,  2463,  2467,  2472,  2476,  2480,  2485,  2485,  2485,
    2491,  2498,  2498,  2498,  2503,  2503,  2503,  2509,  2509,  2509,
    2514,  2518,  2518,  2518,  2523,  2523,  2523,  2532,  2536,  2536,
    2536,  2541,  2541,  2541,  2550,  2554,  2554,  2554,  2559,  2559,
    2559,  2568,  2568,  2568,  2574,  2574,  2574,  2583,  2586,  2597,
    2613,  2613,  2613,  2637,  2637,  2657,  2658,  2659,  2660,  2664,
    2671,  2678,  2684,  2693,  2698,  2705,  2706,  2710,  2716,  2723,
    2731,  2738,  2746,  2758,  2761,  2767,  2781,  2787,  2793,  2798,
    2805,  2810,  2820,  2825,  2832,  2844,  2845,  2849,  2852
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
  "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"", "\"@@ <|\"", "\"|>\"",
  "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"",
  "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"", "\"^^=\"",
  "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"", "\"$t\"",
  "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"keyword\"", "\"start of the string\"", "STRING_CHARACTER",
  "STRING_CHARACTER_ESC", "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "POST_INC", "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('", "')'",
  "'$'", "'@'", "';'", "'{'", "'}'", "'#'", "$accept", "program",
  "optional_public_or_private_module", "module_name", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "reader_character_sequence", "expr_reader", "$@1",
  "options_declaration", "require_declaration", "keyword_or_name",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "expression_else",
  "if_or_static_if", "expression_else_one_liner", "$@2",
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

#define YYPACT_NINF (-1005)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-649)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1005,    21, -1005, -1005,    29,    33,   283,   -85, -1005,   -88,
   -1005, -1005,    35, -1005, -1005, -1005, -1005, -1005,   287, -1005,
     151, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
      53, -1005,    67,   130,   173, -1005, -1005, -1005,   283, -1005,
      19, -1005, -1005,   191, -1005, -1005, -1005,   151,   203,   241,
   -1005, -1005,   248,   300,    35,    35,   262, -1005,   412,    30,
   -1005, -1005, -1005,   382,   431,   434, -1005,   435,    23,    29,
     362,    33,   332,   381, -1005,   348,   348, -1005,   393,   366,
     -91,   439,   367, -1005, -1005,   409, -1005,   -55,    29,    35,
      35,    35,    35, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
     420, -1005, -1005, -1005, -1005, -1005,   383, -1005, -1005, -1005,
   -1005, -1005,    -9,    91, -1005, -1005, -1005, -1005,   527, -1005,
   -1005, 11215, -1005, -1005,   385, -1005, -1005, -1005,   422,   386,
   -1005, -1005,     7, -1005,   402,   450,   412,  6686, -1005,   113,
     488, -1005,   437, -1005, -1005,   410, -1005, -1005, -1005, -1005,
   -1005, -1005, -1005,   123, -1005,   413,   414,   416,   417, -1005,
   -1005, -1005,   399, -1005, -1005, -1005, -1005, -1005,   419, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
     421, -1005, -1005, -1005,   423,   424, -1005, -1005, -1005, -1005,
     442,   445,   401, -1005, -1005, -1005, -1005, -1005,    54,   425,
   -1005, -1005,   426,   459, -1005,  9947, -1005, -1005, -1005, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
   -1005,   591,   596, -1005,   441,   440,   319, -1005, -1005,   477,
   -1005,   438,    29,   185, -1005, -1005, -1005,    91, -1005, -1005,
   -1005, -1005, -1005,   482, -1005,   230,   245,   258, -1005, -1005,
    6215, -1005, -1005, -1005,     9, -1005, -1005, -1005,    13,  3433,
   -1005,  6835,    18,   475, -1005,   449,   492,   495, -1005, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,   496,   464,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
   -1005, -1005,   633, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
   -1005, -1005, -1005, -1005, -1005,   505,   469, -1005, -1005,   -30,
     494, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,   476,
      38,   506,   479, -1005,   437,    43,   485,   637,   308, -1005,
   -1005, 11215, 11215, 11215, 11215,   486,   422, 11215,   441, 11215,
     441, 11215,   441, 11312,   459, -1005, -1005,   120,   497,   507,
   -1005,   498,   512,   514,   499,   515,   500, -1005,   522,  6215,
    6215,   504,   511,   513,   516,   517,   518, -1005, 10894, 10991,
    6215, -1005, -1005, -1005, -1005, -1005, -1005,   524, -1005,  6215,
    6215,  6215,   -77,  6215,  6215,  6215, -1005,   483, -1005, -1005,
   -1005, -1005,  -102, -1005, -1005, -1005, -1005,   508, -1005, -1005,
   -1005, -1005, -1005, -1005,  6825, -1005,   519, -1005, -1005, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005,   525, -1005, -1005,  9374,
   -1005,    54, -1005, -1005, 11215,   109, -1005, -1005, -1005, -1005,
     542, -1005, -1005,   526, -1005,   -14, -1005,    87, 11215, -1005,
    1436, -1005,   -32, -1005,   -77, -1005, -1005,   185,   528,  6215,
     547,   557, 11215, -1005,   -22,   273,   541,   121,   281,   313,
   -1005,   137,   325,   494,   327,   494,   328,   494,   -80, -1005,
     166,   425,   180, -1005,   529, -1005, -1005,   -77, -1005,  6215,
   -1005, -1005,  6215, -1005,  6215, 11215,   177,   177,  6215,  6215,
    6215,  6215,  6215,  6215,   256,  1815,   256,  1989, 10026, -1005,
     277, -1005,   357,   177,   177,   -56, -1005,   177,   177,  6930,
     141, -1005,  2806,   569, 10205, 11025,  6215,  6215, -1005, -1005,
    6215,  6215,  6215,  6215,   565,  6215,   243,  6215,  6215,  6215,
    6215,  6215,  6215,  6215,  6215,  6215,  3584,  6215,  6215,  6215,
    6215,  6215,  6215,  6215,  6215,  6215,  6215,   -35,  6215, -1005,
    3735, -1005, -1005,   425, -1005, -1005, -1005,  6215,   256,   532,
     688, -1005,   136, -1005,    -6,   425, -1005,  6215, -1005, -1005,
     256,  2504, -1005,   440,  3909,  6215,   570, -1005,   533,   579,
    4060,   238,  2655,   338,   338,  4211, -1005,   699,   544,   546,
    6215,   733, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
   -1005,   548,   549,   550,   551, -1005,   552, -1005, -1005,   683,
   -1005,   -79, -1005,  1134,   114,  6215, -1005, -1005,    15, -1005,
   -1005,  7024, -1005,   712,   207, -1005, -1005, -1005,  2980, -1005,
   -1005, 11215, -1005, -1005, -1005,   593, -1005,   577, -1005,   578,
   -1005,   580, 11215, -1005, 11312, -1005,   459, 11215,  4385,  4559,
   11215,  7118, 11215, 11215,  7212, 11215,  7306,   800,  7400,  7494,
    7588,  7682,  7776,  7870,     6,   338,   148,  2163,  4733, 10131,
     587,   -28,   564,   590,   272,    11,  4907,   -28,   351,  6215,
    6215,   559, 11215, -1005,  6215,   394,   598, -1005,   571,   576,
     260, -1005, -1005,   605, -1005,   128, 10375,   -57,   441,   597,
     599, -1005, -1005,   621,   614, -1005, -1005,    12,    12,  1276,
    1276, 10757, 10757,   629,    41,   631, -1005,  9461,   -66,   -66,
      12,    12,   352, 10573,   690, 10461, 11118, 10210,   493, 10652,
   10678,  1276,  1276,   523,   523,    41,    41,    41,   279,  6215,
     635, -1005,   292,  6215,   816,  9555, -1005,   134,  7964, -1005,
    6215,   668, -1005,   673, -1005, 11215, -1005,  2980, -1005,  1239,
      17,  2980, -1005,   710,  9841,   823,  6215, 10375,  1239,   666,
   -1005,   669,   684, 10375, -1005,  2980, -1005,  9841,   642, -1005,
   -1005,  1239,   643, -1005, -1005,  1239, -1005, -1005, -1005, -1005,
   -1005, -1005, -1005,   185,   338, -1005,  6215,  6215,  6215,  6215,
    6215,  6215,  6215,  6215,  6215,  6215,  3131,  6215,  6215,  6215,
    6215,  6215,  6215,  3282, -1005,  6612,    35, -1005,   836,   437,
   -1005,   685, -1005,  2980, -1005,  6402, -1005, -1005,   425, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,   425, -1005,
   -1005, -1005,   333, -1005,   153,   686,  8058,   334, -1005,   821,
     890, -1005,   904, -1005,   569, -1005, -1005, -1005, -1005, -1005,
     649,  6215, -1005,  6215,  6215,    -7,  6215,   687,   605,   654,
     655,  6215,   656,  6215,  6215,   605,   650,   659, 10375, -1005,
   -1005,  1031, 10296, -1005,   691,  6215,  6215, 11215,   441,   -21,
     138,  5081, -1005,   695,   696,   700,   701, -1005,   143,   494,
   -1005,  6215, -1005,  6215,  5240,  6215, -1005,   697,   664, -1005,
   -1005,  6215,   679, -1005,  9642,  6215,   681, -1005,  9736, -1005,
   -1005,  6215, -1005, -1005,  8152, -1005,   837,   -23, -1005,  9841,
   -1005,  6215, -1005,  9841,  6215,  6215,   440, 10375, -1005, -1005,
   -1005, -1005, -1005,  9841, -1005, -1005, -1005,    80,  6215, -1005,
   -1005, 10375, 10375, 10375, 10375, 10375, 10375, 10375, 10375, 10375,
   10375,   338,   338, 10375, 10375, 10375, 10375, 10375, 10375, 10375,
     338,   338, 10375, -1005,   187,   443,    20,   682, -1005, -1005,
    6507, -1005, -1005, -1005, -1005, -1005, -1005,   216, -1005, -1005,
   -1005, -1005, -1005,   689,  5414,   -99, 10375, 10375,   -28,   692,
   10375,    37,   587,   693, -1005, 10375, -1005,   590,    10,   -28,
     694, -1005, -1005, -1005, -1005,  8246,  8340,  1165,   494,   702,
     605, 10375, -1005, -1005, -1005, -1005,   -57,   698,   -69, 11215,
    8434, 11215,  8528, -1005,   156,  8622, -1005,  6215, 10540,  6215,
   -1005,  8716,  6215, -1005, -1005, -1005,   720,  6215,    58, -1005,
    6215,  1638,   440, -1005, -1005,  6215, -1005,  6308, -1005, -1005,
   -1005, -1005,   704, -1005, -1005,    45, -1005, -1005,   443, -1005,
   -1005, -1005,  6215,   725, -1005,  6215,  6215,  6215,  5588, -1005,
     162,  6215,   703,   705,  6215,  6215, -1005,  6215,   706, -1005,
    6215, -1005, -1005, -1005,   713, -1005,   204, -1005, -1005,  5762,
   -1005, -1005,  1568, -1005,   341, -1005, -1005, -1005, 11215,  8810,
    8904, -1005,  8998, -1005, 10375,   440, 10375, -1005, -1005,  1239,
   -1005,   709, -1005, -1005,   858,   256,  9092,   714,  1276,  1276,
    1276, -1005,  9186, -1005,  6698,   707, -1005, 10375, 10375,  6698,
     711,  1276,   254, -1005,   708,  6215, 10461, -1005, -1005,   342,
   -1005, -1005, -1005, -1005,    80,  2353, -1005,  6686, -1005, -1005,
   -1005, -1005,  6215, -1005,   843,   715, -1005,   716, -1005, -1005,
     254, -1005, 10540, -1005, -1005, -1005, -1005,  5913,  6064, -1005,
   -1005, -1005, -1005, -1005, 10375,   719,  6686,  9280,  6215,   717,
     718, -1005, -1005,  6215, 10375,  6215, 10375, -1005,   440, -1005,
   10375, -1005, -1005, 10375, 10375, -1005
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   112,     1,   270,     0,     0,     0,     0,   271,     0,
     663,   660,     0,    14,     3,    10,     9,     8,     0,     7,
     514,     6,    11,     5,     4,    12,    13,    87,    88,    86,
      95,    97,    34,    49,    46,    47,    36,    37,     0,    38,
      44,    35,   525,     0,   530,    19,    18,   514,     0,     0,
     100,   101,     0,   243,     0,     0,   102,   104,   110,     0,
      99,   543,   542,   204,   531,   544,   515,   516,     0,     0,
       0,     0,    39,     0,    45,     0,     0,    42,     0,     0,
       0,    15,     0,   661,   245,     0,   105,     0,     0,     0,
       0,     0,     0,   113,   206,   205,   208,   203,   533,   532,
       0,   546,   545,   547,   518,   517,   520,    93,    94,    91,
      92,    90,     0,     0,    89,    98,    50,    48,    44,    41,
      40,     0,   527,   529,     0,    16,    17,    20,     0,     0,
     244,   109,     0,   106,   107,   108,   111,     0,   534,     0,
     539,   511,   457,    21,    22,     0,    82,    83,    80,    81,
      79,    78,    84,     0,    43,     0,     0,     0,     0,   555,
     575,   556,   589,   557,   561,   562,   563,   564,   581,   568,
     569,   570,   571,   572,   573,   574,   576,   577,   578,   579,
     630,   560,   567,   580,   637,   644,   558,   565,   559,   566,
       0,     0,     0,   588,   599,   602,   600,   601,   657,   526,
     528,   592,     0,     0,   103,     0,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,     0,     0,   119,   114,     0,     0,   522,   540,     0,
     548,   512,     0,     0,    23,    24,    25,     0,    96,   621,
     624,   627,   617,     0,   594,   631,   638,   645,   651,   654,
       0,   607,   612,   606,     0,   620,   616,   609,     0,     0,
     611,     0,     0,     0,   486,     0,   172,   173,   170,   122,
     123,   125,   124,   126,   127,   128,   129,   155,   156,   153,
     154,   146,   157,   158,   147,   144,   145,   171,   166,     0,
     169,   159,   160,   161,   162,   133,   134,   135,   130,   131,
     132,   143,     0,   149,   150,   148,   141,   142,   137,   136,
     138,   139,   140,   121,   120,   165,     0,   151,   152,   457,
     117,   233,   209,   582,   585,   583,   586,   584,   587,     0,
       0,   537,     0,   519,   457,     0,     0,   501,   499,   521,
      85,     0,     0,     0,     0,     0,     0,     0,   114,     0,
     114,     0,   114,     0,     0,   361,   362,     0,     0,     0,
     355,     0,     0,     0,     0,     0,     0,   581,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   446,     0,     0,
       0,   308,   310,   309,   311,   312,   313,     0,    26,     0,
       0,     0,     0,     0,     0,     0,   295,   296,   359,   358,
     306,   440,   356,   432,   431,   430,   429,   112,   435,   357,
     434,   433,   402,   363,     0,   364,     0,   360,   665,   666,
     667,   668,   614,   615,   608,   610,     0,   613,   604,     0,
     659,   658,   593,   664,     0,     0,   174,   175,   168,   164,
     176,   167,   163,     0,   115,     0,   479,     0,     0,   207,
       0,   522,     0,   535,     0,   541,   468,     0,     0,     0,
       0,     0,     0,   500,     0,     0,     0,   597,     0,     0,
     590,     0,     0,   117,     0,   117,     0,   117,   243,   483,
       0,   481,     0,   248,   252,   251,   255,     0,   284,     0,
     275,   278,     0,   281,     0,     0,   391,   392,     0,     0,
       0,     0,     0,     0,     0,   675,     0,     0,   687,   692,
       0,   237,     0,   368,   367,   407,    32,   366,   365,     0,
     297,   438,     0,   304,     0,     0,     0,     0,   393,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   352,     0,   591,
       0,   605,   603,   485,   662,   487,   177,     0,     0,     0,
     550,   476,   490,   116,   457,   118,   235,     0,    59,    60,
       0,   258,   256,     0,     0,     0,     0,   257,     0,     0,
       0,     0,     0,     0,     0,     0,   212,   210,     0,     0,
       0,     0,   228,   223,   220,   219,   221,   222,   234,   214,
     213,     0,    67,    68,    65,   226,    66,   227,   229,   273,
     218,     0,   215,   314,     0,     0,   523,   538,   469,   513,
     458,     0,   503,   504,     0,   497,   498,   496,     0,   622,
     625,     0,   628,   618,   595,     0,   632,     0,   639,     0,
     646,     0,     0,   652,     0,   655,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,     0,   683,
     673,   675,     0,   688,     0,     0,     0,   675,     0,     0,
       0,     0,     0,    29,     0,    27,     0,   395,     0,     0,
     344,   341,   343,     0,   403,     0,   290,     0,   114,     0,
       0,   418,   417,     0,     0,   419,   423,   369,   370,   382,
     383,   380,   381,     0,   412,     0,   400,     0,   436,   437,
     371,   372,   387,   388,   389,   390,     0,     0,   385,   386,
     384,   378,   379,   374,   373,   375,   376,   377,     0,     0,
       0,   350,     0,     0,     0,     0,   405,     0,     0,   477,
       0,     0,   489,     0,   488,     0,   491,     0,   480,     0,
       0,     0,   262,     0,   259,     0,     0,   246,     0,     0,
     232,     0,     0,    53,    73,     0,   267,   264,   296,   241,
     242,     0,     0,   230,   231,     0,    70,   217,   224,   225,
     261,   266,   272,     0,     0,   216,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   536,     0,     0,   549,     0,   457,
     502,     0,   506,     0,   510,   314,   623,   626,   598,   629,
     619,   596,   633,   635,   640,   642,   647,   649,   482,   653,
     484,   656,     0,   253,     0,     0,     0,     0,   408,     0,
       0,   409,     0,   439,   304,   441,   442,   443,   444,   445,
       0,     0,   676,     0,     0,   675,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   686,   693,
     694,     0,     0,    30,    33,     0,     0,     0,   114,     0,
       0,     0,   404,     0,     0,     0,     0,   302,     0,   117,
     414,     0,   420,     0,     0,     0,   398,     0,     0,   424,
     428,     0,     0,   401,     0,     0,     0,   351,     0,   353,
     396,     0,   406,   478,     0,   552,   553,   492,   495,   494,
      74,     0,   263,   260,     0,     0,     0,   247,    75,    76,
      51,    52,   268,   265,   297,   236,   233,    56,     0,   274,
     240,   324,   325,   327,   326,   328,   318,   319,   320,   329,
     330,     0,     0,   316,   317,   331,   332,   321,   322,   323,
       0,     0,   315,   524,     0,   464,   459,     0,   505,   508,
     314,   509,   636,   643,   650,   249,   254,     0,   287,   285,
     276,   279,   282,     0,     0,     0,   670,   669,   675,     0,
     684,     0,   674,     0,   678,   685,   690,   689,     0,   675,
       0,   691,   238,    28,    31,     0,     0,     0,   117,     0,
       0,   291,   300,   301,   299,   298,     0,     0,     0,     0,
       0,     0,     0,   339,     0,     0,   425,     0,   413,     0,
     399,     0,     0,   397,   354,   551,     0,     0,     0,   269,
       0,     0,     0,    54,    55,     0,    69,    61,   335,   336,
     333,   334,   113,   465,   466,   459,   460,   461,   464,   470,
     507,   250,     0,     0,   286,     0,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,   677,     0,     0,   681,
       0,   342,   456,   345,     0,   337,     0,   303,   305,     0,
     292,   307,     0,   455,     0,   453,   340,   450,     0,     0,
       0,   449,     0,   554,   493,     0,    77,   211,    57,     0,
      62,     0,   475,   471,   462,     0,     0,     0,   277,   280,
     283,   410,     0,   448,   695,     0,   679,   672,   671,   695,
       0,   239,     0,   348,     0,     0,   293,   415,   421,     0,
     454,   452,   451,    72,    56,     0,    71,     0,   463,   473,
     467,   288,     0,   411,     0,     0,   680,     0,   682,   346,
       0,   338,   294,   416,   422,   426,    58,   258,     0,    63,
      67,    68,    65,    66,    64,     0,     0,     0,     0,     0,
       0,   349,   427,     0,   259,     0,   264,   472,     0,   289,
     696,   697,   698,   260,   265,   474
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1005, -1005, -1005, -1005, -1005,   374,   833, -1005, -1005, -1005,
   -1005, -1005, -1005, -1005,   503,   873, -1005,   796, -1005, -1005,
     849, -1005, -1005, -1005,  -251, -1005, -1005, -1005,  -250, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005,   721, -1005, -1005,
     857,   -68, -1005, -1005,   330,    93,  -402,  -353,  -471, -1005,
   -1005, -1005, -1004, -1005, -1005,  -232, -1005,   -36, -1005, -1005,
   -1005, -1005,  -563,   -12, -1005, -1005, -1005, -1005, -1005,  -244,
    -242,  -241, -1005,  -240, -1005, -1005,   936, -1005, -1005, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
    -564, -1005, -1005,  -106, -1005,    69,  -585, -1005,  -458, -1005,
   -1005, -1005,  -880, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
   -1005,   228, -1005, -1005, -1005, -1005, -1005, -1005, -1005,  -141,
    -140, -1005,  -144, -1005, -1005, -1005, -1005, -1005,   363, -1005,
     282, -1005,  -421,   586,  -572,  -574,   307, -1005, -1005,  -450,
   -1005, -1005,   905, -1005, -1005, -1005,   509,    49, -1005, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
   -1005, -1005,  -465,  -110, -1005,   588, -1005,   600, -1005, -1005,
   -1005, -1005,  -245, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005, -1005,
   -1005, -1005, -1005, -1005, -1005, -1005, -1005,   -90, -1005, -1005,
   -1005, -1005, -1005,   592,  -705,  -504,  -662, -1005,    60,   257,
     447, -1005, -1005, -1005,  -205, -1005
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   127,    47,    14,   145,   151,   522,   408,   904,
     409,   706,    15,    16,    39,    40,    41,    77,    17,    34,
      35,   608,   609,  1075,  1076,   610,  1141,  1175,   611,   612,
     968,   613,   614,   615,   616,   617,  1070,   152,   153,    30,
      31,    32,    56,    57,    58,    59,    18,   330,   459,   234,
      19,    96,   235,    97,   137,   410,   618,   460,   619,   411,
     702,  1110,   620,   412,   621,   494,   667,  1091,   413,   622,
     623,   624,   625,   626,   627,   628,   629,   813,   630,   414,
     672,  1095,   673,  1096,   675,  1097,   415,   670,  1094,   416,
     715,  1121,   417,   917,   918,   718,   418,   419,   783,   632,
     420,   421,   712,   422,   907,  1162,   908,  1190,   423,   764,
    1064,   716,  1049,  1193,  1051,  1194,  1128,  1212,   425,   455,
    1088,  1179,  1085,   997,   638,   839,  1177,  1206,   456,   457,
     489,   490,   274,   275,   884,   581,   648,   474,   348,   349,
     241,   344,    67,   106,    21,   142,   340,    44,    78,    80,
      22,   100,   139,    23,   465,   239,   240,    65,   103,    24,
     140,   342,   582,   426,   339,   195,   196,   202,   197,   356,
     851,   476,   198,   354,   850,   351,   846,   352,   847,   353,
     849,   357,   852,   358,  1002,   359,   854,   360,  1003,   361,
     856,   362,  1004,   363,   859,   364,   861,   491,    25,    49,
     129,    26,    48,   427,   690,   691,   692,   428,   693,   519,
     694,   429,   520,   430,  1185,   431
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,   243,   631,   332,   769,   483,   767,   485,   910,   487,
     777,   194,   657,   697,   659,   533,   661,   639,   799,   800,
     132,     2,  -112,   685,   575,   881,   441,     3,   782,   889,
     894,   199,   534,   535,   578,   896,   951,   107,   108,   796,
      73,   432,    60,    60,   685,   434,   814,    27,    28,   684,
       4,   695,     5,   544,     6,    85,   546,   547,    52,   531,
       7,   534,   535,   913,  1086,  1119,   546,   547,    50,     8,
      42,   941,   914,    45,    74,     9,    42,    60,    60,    60,
      60,    89,    90,    91,    53,   844,   261,   758,   759,  1086,
    1143,  1072,    79,   662,   532,    10,    51,   772,   645,   772,
     882,  1101,  1073,  1074,   864,   146,   147,   646,   883,   193,
      46,   123,   262,   263,   915,   760,    43,   453,    11,   916,
    1087,   815,   242,   538,   539,   780,   761,   567,   568,   124,
     579,   544,   331,   545,   546,   547,   548,   567,   568,   635,
     549,   453,   131,   515,   517,  1087,   242,   580,   774,   647,
     774,   271,   538,   539,   909,   143,   144,   772,   762,   763,
     544,   194,   545,   546,   547,   548,  1104,   454,   636,   549,
      52,   264,   888,  1205,   345,   265,   773,    69,   109,   442,
     941,   773,   110,  1022,   111,   433,   112,   773,    33,   435,
      29,   562,   563,   564,   565,   566,    53,   534,   535,   462,
      92,    75,  1218,   467,   204,   567,   568,   436,   774,   836,
    1107,    66,    76,    69,   948,    12,   437,   837,   952,   113,
     443,    13,    54,  1019,    68,    93,   266,   644,   941,   970,
     267,    55,   962,   268,   567,   568,   838,    69,   468,   261,
     463,   194,   194,   194,   194,   575,   148,   194,   269,   194,
     149,   194,   150,   194,   112,    52,   772,   270,  1135,   193,
     677,   475,   477,   478,   479,   262,   263,   482,   772,   484,
     273,   486,   388,   389,   390,   462,    52,   883,   194,   194,
     999,    53,  1189,    85,   583,   708,   236,   584,   538,   539,
     709,    61,    62,   247,    63,   271,   544,   493,   911,   546,
     547,   548,    53,    70,   941,   549,   773,   774,   887,   775,
    1211,   574,   776,  1046,   237,   654,   834,  1015,   710,   774,
     248,   651,    64,   941,   264,   912,   941,   645,   265,   346,
    1028,   942,   941,  1039,   194,  1116,   646,   655,  1047,   193,
     193,   193,   193,    71,   663,   193,   347,   193,   194,   193,
    1006,   193,    42,  1126,   573,   193,  1102,    92,   665,  1153,
    1054,   785,   194,   969,    82,   919,   664,  1108,   585,   794,
     567,   568,   534,   535,   887,  1067,   193,   193,   647,   266,
     666,  1018,  1082,   267,    86,    87,   268,  1068,   333,    52,
     526,  1029,   334,   735,  1092,   194,  1078,  1079,   708,  1164,
     579,   269,    83,  -634,   736,  1080,  1081,   842,  -634,    84,
     270,   335,   336,   337,   338,    53,  1093,   580,  -641,   133,
     134,   135,   136,  -641,   722,   726,  -634,   869,   870,   932,
     872,  -648,   193,  -347,   504,    85,  -648,    94,  -347,   331,
     933,  -641,   936,    95,    36,    37,   193,   271,  1048,  1105,
    1100,   649,   637,   937,  -648,   271,  -347,   901,    88,   652,
     193,   536,   537,   538,   539,   540,    38,   892,   541,   542,
     543,   544,   893,   545,   546,   547,   548,   700,   471,   701,
     549,   472,   550,   551,   473,   669,    98,   271,   424,   101,
     104,   653,    99,   193,   125,   102,   105,   439,  1083,   271,
     126,   271,   271,   656,  1084,   658,   660,   271,   271,    36,
      37,  1005,  1009,   534,   535,   271,   271,   116,   711,  1168,
    1195,   143,   144,   703,   704,    76,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,   406,   798,    89,   331,
      91,   194,   118,   534,   535,   567,   568,   950,    89,    90,
      91,   893,   194,   897,   194,  1038,   958,   194,   244,   245,
     194,   848,   194,   194,   121,   194,   122,  1114,   128,   965,
     130,  1180,   858,   967,   244,   245,   246,   862,   119,   120,
     867,   138,    74,   201,   141,   200,    89,   203,   238,   242,
     249,   250,   194,   251,   252,   253,   254,   260,   255,   271,
     256,   257,   536,   537,   538,   539,   540,   506,   507,   541,
     542,   543,   544,   631,   545,   546,   547,   548,   518,   258,
     273,   549,   259,   550,   551,   327,   272,   523,   524,   525,
     328,   527,   528,   529,   538,   539,   930,   329,   341,   193,
     343,   331,   544,   355,   545,   546,   547,   548,   444,   445,
     193,   549,   193,   446,   450,   193,   447,   448,   193,   449,
     193,   193,  1037,   193,   452,   194,   451,   458,   558,   559,
     560,   561,   562,   563,   564,   565,   566,   461,   470,   464,
     466,   469,   530,   480,   498,   947,   567,   568,   633,   500,
     193,   501,   503,   497,   499,   502,   504,   641,   996,   505,
     508,   521,    12,   576,   564,   565,   566,   509,   642,   510,
     534,   535,   511,   512,   513,   570,   567,   568,   643,   650,
     571,   717,   577,   640,  1069,   668,   733,   671,   770,   771,
     674,   789,   676,   790,   791,   802,   678,   679,   680,   681,
     682,   683,   806,   689,   803,   689,   804,   812,   807,   808,
     809,   810,   811,   841,   442,   853,   855,   887,   857,   890,
     891,   900,   903,   193,   727,   728,   909,   905,   729,   730,
     731,   732,   906,   734,   920,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   747,   748,   749,   750,   751,   752,
     753,   754,   755,   756,   757,   921,   765,   194,   922,   536,
     537,   538,   539,   540,  1122,   768,   541,   542,   543,   544,
     923,   545,   546,   547,   548,   779,  1120,   939,   549,   784,
     550,   551,   787,   788,    60,   924,   552,   925,   793,   945,
     797,   935,   261,   801,   946,   814,   956,   959,   805,   961,
    1138,   964,   960,   995,   966,  1014,   998,  1007,  1021,  1023,
    1024,  1026,  1030,   261,  1031,  1034,  1042,  1043,   262,   263,
    1057,  1044,  1045,   835,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,  1056,  1059,   845,  1062,  1066,   262,
     263,  1133,  1089,   567,   568,  1098,  1147,  1103,  1106,  1109,
    1178,  1163,  1182,  1118,  1208,   193,   705,   866,  1155,  1115,
    1156,   114,  1186,  1173,  1142,  1191,  1188,  1174,  1160,  1176,
    1209,    72,  1221,  1222,   154,   523,   529,   264,  1210,  1217,
     117,   265,   261,  1196,   529,  1199,   115,   898,   518,   994,
    1071,  1200,   902,  1201,  1202,  1203,   261,    20,   264,   194,
    1117,   194,   265,  1013,  1145,  1144,   860,   778,   262,   263,
     492,   843,    81,  1027,  1187,   495,   481,   899,     0,   496,
       0,  1124,   262,   263,   698,     0,     0,     0,   350,     0,
     634,     0,   266,     0,     0,     0,   267,     0,   874,   268,
       0,     0,     0,     0,     0,     0,  1225,   934,     0,     0,
       0,   938,     0,   266,   269,     0,     0,   267,   944,  1010,
     268,     0,     0,   270,     0,   949,     0,   264,     0,   953,
       0,   265,     0,     0,   957,   269,     0,     0,   194,     0,
       0,   264,     0,   963,   270,   265,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   193,  1169,   193,
       0,     0,     0,     0,   971,   972,   973,   974,   975,   976,
     977,   978,   979,   980,   983,   984,   985,   986,   987,   988,
     989,   992,   266,   261,     0,     0,   267,     0,  1011,   268,
       0,  1000,     0,     0,     0,     0,   266,     0,     0,     0,
     267,     0,  1012,   268,   269,     0,     0,     0,     0,   262,
     263,     0,     0,   270,     0,     0,     0,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
       0,  1016,  1017,     0,  1020,     0,   193,     0,     0,  1025,
       0,   689,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1035,  1036,     0,     0,     0,     0,  1041,
       0,     0,     0,   -64,     0,     0,     0,     0,   264,  1050,
     711,  1052,   265,  1055,   534,   535,     0,     0,     0,  1058,
       0,     0,     0,  1061,     0,     0,     0,     0,     0,  1041,
       0,     0,     0,     0,     0,     0,     0,     0,   711,     0,
       0,     0,   983,   992,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1077,   261,     0,     0,
       0,     0,     0,   266,     0,     0,     0,   267,     0,  1032,
     268,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   262,   263,   269,     0,     0,     0,     0,
       0,     0,     0,     0,   270,   816,   817,   818,   819,   820,
     821,   822,   823,   536,   537,   538,   539,   540,   824,   825,
     541,   542,   543,   544,   826,   545,   546,   547,   548,   534,
     535,     0,   549,   827,   550,   551,   828,   829,     0,     0,
     552,   553,   554,   830,   831,   832,   555,     0,     0,     0,
       0,     0,   264,     0,     0,  1129,   265,  1130,     0,     0,
    1132,     0,     0,     0,     0,  1134,   534,   535,  1136,   633,
       0,     0,     0,  1139,     0,   833,   556,     0,   557,   558,
     559,   560,   561,   562,   563,   564,   565,   566,     0,     0,
    1146,     0,     0,  1148,  1149,  1150,  1152,   567,   568,  1154,
       0,     0,  1157,  1158,     0,  1159,     0,   266,  1161,     0,
       0,   267,     0,  1113,   268,     0,     0,  1166,   536,   537,
     538,   539,   540,     0,     0,   541,   542,   543,   544,   269,
     545,   546,   547,   548,     0,     0,     0,   549,   270,   550,
     551,     0,     0,     0,     0,   552,   553,   554,     0,     0,
       0,   555,     0,     0,     0,   536,   537,   538,   539,     0,
       0,     0,     0,  1192,     0,   544,     0,   545,   546,   547,
     548,     0,     0,  1204,   549,     0,   550,   551,     0,     0,
    1207,   556,     0,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566,     0,     0,  1214,  1216,     0,     0,     0,
       0,     0,   567,   568,     0,     0,  1220,   586,     0,     0,
     331,  1223,     3,  1224,   587,   588,   589,     0,   590,     0,
     365,   366,   367,   368,   369,   562,   563,   564,   565,   566,
       0,   591,   370,   592,   593,     0,     0,     0,     0,   567,
     568,     0,     0,   594,   371,     0,   595,     0,   596,   372,
       0,     0,   373,     0,     8,   374,   597,     0,   598,   375,
       0,     0,   599,   600,     0,     0,     0,     0,     0,   601,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     377,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,     0,   181,   182,   183,     0,     0,   186,   187,
     188,   189,     0,     0,   378,   602,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   603,   604,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   381,
     382,   383,   384,   385,     0,   386,     0,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,    53,   605,   398,
     261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,   400,   401,     0,   402,
       0,     0,   403,   404,     0,     0,   262,   263,     0,     0,
       0,     0,   405,     0,   406,   407,   606,   331,   607,   586,
       0,     0,     0,     0,     3,     0,   587,   588,   589,     0,
     590,     0,   365,   366,   367,   368,   369,     0,     0,     0,
       0,     0,     0,   591,   370,   592,   593,     0,     0,     0,
       0,     0,     0,     0,     0,   594,   371,     0,   595,     0,
     596,   372,     0,     0,   373,   264,     8,   374,   597,   265,
     598,   375,     0,     0,   599,   600,     0,     0,     0,     0,
       0,   601,     0,   159,   160,   161,     0,   163,   164,   165,
     166,   167,   377,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,   181,   182,   183,     0,     0,
     186,   187,   188,   189,     0,     0,   378,   602,     0,     0,
     266,     0,     0,     0,   267,     0,  1167,   268,     0,   379,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   269,     0,   603,   604,     0,     0,     0,     0,
       0,   270,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   381,   382,   383,   384,   385,     0,   386,     0,   387,
     388,   389,   390,   391,   392,   393,   394,   395,   396,    53,
     605,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,   401,
       0,   402,     0,     0,   403,   404,     0,     0,     0,   365,
     366,   367,   368,   369,   405,     0,   406,   407,   606,   331,
    1137,   370,     0,     0,     0,     0,     0,   261,     0,     0,
       0,     0,     0,   371,     0,     0,     0,     0,   372,     0,
       0,   373,     0,     0,   374,     0,   685,     0,   375,     0,
       0,     0,     0,   262,   263,     0,     0,     0,   376,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   377,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,   181,   182,   183,     0,     0,   186,   187,   188,
     189,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,     0,
       0,     0,   264,     0,     0,     0,   265,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   381,   382,
     383,   384,   385,     0,   386,     0,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   686,   397,   398,     0,
       0,     0,     0,     0,     0,     0,     0,   266,     0,     0,
       0,   267,     0,     0,   687,   400,   401,     0,   402,     0,
       0,   403,   404,   365,   366,   367,   368,   369,     0,   269,
       0,   688,     0,   406,   407,   370,   331,     0,   270,     0,
       0,   261,     0,     0,     0,     0,     0,   371,     0,     0,
       0,     0,   372,     0,     0,   373,     0,     0,   374,     0,
       0,     0,   375,     0,     0,     0,     0,   262,   263,     0,
       0,     0,   376,     0,   159,   160,   161,     0,   163,   164,
     165,   166,   167,   377,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,     0,   181,   182,   183,     0,
       0,   186,   187,   188,   189,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,     0,     0,     0,   264,     0,     0,     0,
     265,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   381,   382,   383,   384,   385,     0,   386,     0,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     686,   397,   398,     0,     0,     0,     0,     0,     0,     0,
       0,   266,     0,     0,     0,   267,     0,     0,   687,   400,
     401,     0,   402,     0,     0,   403,   404,   365,   366,   367,
     368,   369,     0,   269,     0,   696,     0,   406,   407,   370,
     331,     0,   270,     0,     0,   434,     0,     0,     0,     0,
       0,   371,     0,     0,     0,     0,   372,     0,     0,   373,
       0,     0,   374,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,     0,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   377,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,     0,
     181,   182,   183,     0,     0,   186,   187,   188,   189,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   381,   382,   383,   384,
     385,     0,   386,     0,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,    53,   397,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   435,
       0,     0,   399,   400,   401,     0,   402,     0,     0,   403,
     404,     0,     0,     0,     0,     0,     0,   436,     0,   405,
       0,   406,   407,     0,   331,     0,   437,   365,   366,   367,
     368,   369,     0,     0,     0,     0,     0,     0,  1197,   370,
     592,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   371,     0,     0,     0,     0,   372,     0,     0,   373,
       0,     0,   374,   597,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,     0,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   377,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,     0,
     181,   182,   183,     0,     0,   186,   187,   188,   189,     0,
       0,   378,  1198,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   381,   382,   383,   384,
     385,     0,   386,     0,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,    53,   397,   398,     0,   365,   366,
     367,   368,   369,     0,     0,     0,     0,     0,     0,     0,
     370,     0,   399,   400,   401,     0,   402,     0,     0,   403,
     404,     0,   371,     0,     0,     0,     0,   372,     0,   405,
     373,   406,   407,   374,   331,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   376,     0,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   377,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
       0,   181,   182,   183,     0,     0,   186,   187,   188,   189,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,     0,     0,
       0,     0,     0,     0,   781,     0,     0,     0,     0,     0,
     603,   604,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   381,   382,   383,
     384,   385,     0,   386,     0,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,    53,   397,   398,     0,   365,
     366,   367,   368,   369,     0,     0,     0,     0,     0,     0,
       0,   370,     0,   399,   400,   401,     0,   402,     0,     0,
     403,   404,     0,   371,     0,     0,     0,     0,   372,     0,
     405,   373,   406,   407,   374,   331,     0,     0,   375,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   376,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   377,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,   181,   182,   183,     0,     0,   186,   187,   188,
     189,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,     0,
       0,     0,     0,     0,     0,   795,     0,     0,     0,     0,
       0,   603,   604,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   381,   382,
     383,   384,   385,     0,   386,     0,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,    53,   397,   398,     0,
     365,   366,   367,   368,   369,     0,     0,     0,     0,     0,
       0,     0,   370,     0,   399,   400,   401,     0,   402,     0,
       0,   403,   404,     0,   371,     0,     0,     0,     0,   372,
       0,   405,   373,   406,   407,   374,   331,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   376,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     377,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,     0,   181,   182,   183,     0,     0,   186,   187,
     188,   189,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   381,
     382,   383,   384,   385,     0,   386,     0,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,    53,   397,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,   400,   401,     0,   402,
       0,     0,   403,   404,   365,   366,   367,   368,   369,     0,
     713,     0,   405,   714,   406,   407,   370,   331,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   371,     0,
       0,     0,     0,   372,     0,     0,   373,     0,     0,   374,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   376,     0,   159,   160,   161,     0,   163,
     164,   165,   166,   167,   377,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,   181,   182,   183,
       0,     0,   186,   187,   188,   189,     0,     0,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   603,   604,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   381,   382,   383,   384,   385,     0,   386,
       0,   387,   388,   389,   390,   391,   392,   393,   394,   395,
     396,    53,   397,   398,     0,   365,   366,   367,   368,   369,
       0,     0,     0,     0,     0,     0,     0,   370,     0,   399,
     400,   401,     0,   402,     0,     0,   403,   404,     0,   371,
       0,     0,     0,     0,   372,     0,   405,   373,   406,   407,
     374,   331,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   376,     0,   159,   160,   161,     0,
     163,   164,   165,   166,   167,   377,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,     0,   181,   182,
     183,     0,     0,   186,   187,   188,   189,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   981,   982,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   381,   382,   383,   384,   385,     0,
     386,     0,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,    53,   397,   398,     0,   365,   366,   367,   368,
     369,     0,     0,     0,     0,     0,     0,     0,   370,     0,
     399,   400,   401,     0,   402,     0,     0,   403,   404,     0,
     371,     0,     0,     0,     0,   372,     0,   405,   373,   406,
     407,   374,   331,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   376,     0,   159,   160,   161,
       0,   163,   164,   165,   166,   167,   377,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,     0,   181,
     182,   183,     0,     0,   186,   187,   188,   189,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   990,   991,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   381,   382,   383,   384,   385,
       0,   386,     0,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,    53,   397,   398,     0,   365,   366,   367,
     368,   369,     0,     0,     0,     0,     0,     0,     0,   370,
       0,   399,   400,   401,     0,   402,     0,     0,   403,   404,
       0,   371,     0,     0,     0,     0,   372,     0,   405,   373,
     406,   407,   374,   331,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,     0,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   377,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,     0,
     181,   182,   183,     0,     0,   186,   187,   188,   189,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   381,   382,   383,   384,
     385,     0,   386,     0,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,    53,   397,   398,     0,   365,   366,
     367,   368,   369,     0,     0,   746,     0,     0,     0,     0,
     370,     0,   399,   400,   401,     0,   402,     0,     0,   403,
     404,     0,   371,     0,     0,     0,     0,   372,   438,   405,
     373,   406,   407,   374,   331,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   376,     0,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   377,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
       0,   181,   182,   183,     0,     0,   186,   187,   188,   189,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   381,   382,   383,
     384,   385,     0,   386,     0,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,    53,   397,   398,     0,   365,
     366,   367,   368,   369,     0,     0,     0,     0,     0,     0,
       0,   370,     0,   399,   400,   401,     0,   402,     0,     0,
     403,   404,     0,   371,     0,     0,     0,     0,   372,     0,
     405,   373,   406,   407,   374,   331,     0,     0,   375,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   376,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   377,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,   181,   182,   183,     0,     0,   186,   187,   188,
     189,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   381,   382,
     383,   384,   385,     0,   386,     0,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,    53,   397,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,   401,     0,   402,     0,
       0,   403,   404,   365,   366,   367,   368,   369,     0,     0,
       0,   405,   766,   406,   407,   370,   331,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   371,     0,     0,
       0,     0,   372,     0,     0,   373,     0,     0,   374,     0,
       0,     0,   375,     0,     0,     0,     0,     0,   786,     0,
       0,     0,   376,     0,   159,   160,   161,     0,   163,   164,
     165,   166,   167,   377,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,     0,   181,   182,   183,     0,
       0,   186,   187,   188,   189,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   381,   382,   383,   384,   385,     0,   386,     0,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
      53,   397,   398,     0,   365,   366,   367,   368,   369,     0,
       0,     0,     0,     0,     0,     0,   370,     0,   399,   400,
     401,     0,   402,     0,     0,   403,   404,     0,   371,     0,
       0,     0,     0,   372,     0,   405,   373,   406,   407,   374,
     331,     0,     0,   375,     0,     0,   792,     0,     0,     0,
       0,     0,     0,   376,     0,   159,   160,   161,     0,   163,
     164,   165,   166,   167,   377,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,   181,   182,   183,
       0,     0,   186,   187,   188,   189,     0,     0,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   381,   382,   383,   384,   385,     0,   386,
       0,   387,   388,   389,   390,   391,   392,   393,   394,   395,
     396,    53,   397,   398,     0,   365,   366,   367,   368,   369,
       0,     0,     0,     0,     0,     0,     0,   370,     0,   399,
     400,   401,     0,   402,     0,     0,   403,   404,     0,   371,
       0,     0,     0,     0,   372,     0,   405,   373,   406,   407,
     374,   331,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   376,     0,   159,   160,   161,     0,
     163,   164,   165,   166,   167,   377,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,     0,   181,   182,
     183,     0,     0,   186,   187,   188,   189,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   381,   382,   383,   384,   385,     0,
     386,     0,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,    53,   397,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,     0,
     399,   400,   401,     0,   402,     0,     0,   403,   404,   365,
     366,   367,   368,   369,     0,     0,     0,   405,     0,   406,
     407,   370,   331,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   371,     0,     0,     0,     0,   372,     0,
       0,   373,     0,     0,   374,     0,     0,     0,   375,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   376,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   377,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,   181,   182,   183,     0,     0,   186,   187,   188,
     189,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   381,   382,
     383,   384,   385,     0,   386,     0,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,    53,   397,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,   401,     0,   402,     0,
       0,   403,   404,   365,   366,   367,   368,   369,     0,     0,
       0,   405,   863,   406,   407,   370,   331,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   371,     0,     0,
       0,     0,   372,     0,     0,   373,     0,     0,   374,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   376,     0,   159,   160,   161,     0,   163,   164,
     165,   166,   167,   377,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,     0,   181,   182,   183,     0,
       0,   186,   187,   188,   189,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   381,   382,   383,   384,   385,     0,   386,     0,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
      53,   397,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   865,     0,   399,   400,
     401,     0,   402,     0,     0,   403,   404,   365,   366,   367,
     368,   369,     0,     0,     0,   405,     0,   406,   407,   370,
     331,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   371,     0,     0,     0,     0,   372,     0,     0,   373,
       0,     0,   374,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,     0,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   377,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,     0,
     181,   182,   183,     0,     0,   186,   187,   188,   189,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   381,   382,   383,   384,
     385,     0,   386,     0,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,    53,   397,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   400,   401,     0,   402,     0,     0,   403,
     404,   365,   366,   367,   368,   369,     0,     0,     0,   405,
     885,   406,   407,   370,   331,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   371,     0,     0,     0,     0,
     372,     0,     0,   373,     0,     0,   374,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     376,     0,   159,   160,   161,     0,   163,   164,   165,   166,
     167,   377,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,     0,   181,   182,   183,     0,     0,   186,
     187,   188,   189,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     381,   382,   383,   384,   385,     0,   386,     0,   387,   388,
     389,   390,   391,   392,   393,   394,   395,   396,    53,   397,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,   401,     0,
     402,     0,     0,   403,   404,   365,   366,   367,   368,   369,
       0,     0,     0,   405,   895,   406,   407,   370,   331,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   371,
       0,     0,     0,     0,   372,     0,     0,   373,     0,     0,
     374,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   376,     0,   159,   160,   161,     0,
     163,   164,   165,   166,   167,   377,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,     0,   181,   182,
     183,     0,     0,   186,   187,   188,   189,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   381,   382,   383,   384,   385,     0,
     386,     0,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,    53,   397,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,   366,   367,   368,   369,     0,
     399,   400,   401,     0,   402,     0,   370,   403,   404,     0,
       0,     0,     0,     0,     0,  1040,     0,   405,   371,   406,
     407,     0,   331,   372,     0,     0,   373,     0,     0,   374,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   376,     0,   159,   160,   161,     0,   163,
     164,   165,   166,   167,   377,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,   181,   182,   183,
       0,     0,   186,   187,   188,   189,     0,     0,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   381,   382,   383,   384,   385,     0,   386,
       0,   387,   388,   389,   390,   391,   392,   393,   394,   395,
     396,    53,   397,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     400,   401,     0,   402,     0,     0,   403,   404,   365,   366,
     367,   368,   369,     0,     0,     0,   405,  1053,   406,   407,
     370,   331,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   371,     0,     0,     0,     0,   372,     0,     0,
     373,     0,     0,   374,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   376,     0,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   377,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
       0,   181,   182,   183,     0,     0,   186,   187,   188,   189,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   381,   382,   383,
     384,   385,     0,   386,     0,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,    53,   397,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   400,   401,     0,   402,     0,     0,
     403,   404,   365,   366,   367,   368,   369,     0,     0,     0,
     405,  1099,   406,   407,   370,   331,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   371,     0,     0,     0,
       0,   372,     0,     0,   373,     0,     0,   374,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   376,     0,   159,   160,   161,     0,   163,   164,   165,
     166,   167,   377,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,   181,   182,   183,     0,     0,
     186,   187,   188,   189,     0,     0,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   381,   382,   383,   384,   385,     0,   386,     0,   387,
     388,   389,   390,   391,   392,   393,   394,   395,   396,    53,
     397,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,   401,
       0,   402,     0,     0,   403,   404,   365,   366,   367,   368,
     369,     0,     0,     0,   405,  1151,   406,   407,   370,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     371,     0,     0,     0,     0,   372,     0,     0,   373,     0,
       0,   374,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   376,     0,   159,   160,   161,
       0,   163,   164,   165,   166,   167,   377,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,     0,   181,
     182,   183,     0,     0,   186,   187,   188,   189,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,     0,     0,     0,     0,
       0,     0,  1165,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   381,   382,   383,   384,   385,
       0,   386,     0,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,    53,   397,   398,     0,   365,   366,   367,
     368,   369,     0,     0,     0,     0,     0,     0,     0,   370,
       0,   399,   400,   401,     0,   402,     0,     0,   403,   404,
       0,   371,     0,     0,     0,     0,   372,     0,   405,   373,
     406,   407,   374,   331,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,     0,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   377,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,     0,
     181,   182,   183,     0,     0,   186,   187,   188,   189,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,     0,     0,     0,
       0,     0,     0,  1213,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   381,   382,   383,   384,
     385,     0,   386,     0,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,    53,   397,   398,     0,   365,   366,
     367,   368,   369,     0,     0,     0,     0,     0,     0,     0,
     370,     0,   399,   400,   401,     0,   402,     0,     0,   403,
     404,     0,   371,     0,     0,     0,     0,   372,     0,   405,
     373,   406,   407,   374,   331,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   376,     0,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   377,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
       0,   181,   182,   183,     0,     0,   186,   187,   188,   189,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,     0,     0,
       0,     0,     0,     0,  1215,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   381,   382,   383,
     384,   385,     0,   386,     0,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,    53,   397,   398,     0,   365,
     366,   367,   368,   369,     0,     0,     0,     0,     0,     0,
       0,   370,     0,   399,   400,   401,     0,   402,     0,     0,
     403,   404,     0,   371,     0,     0,     0,     0,   372,     0,
     405,   373,   406,   407,   374,   331,     0,     0,   375,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   376,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   377,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,   181,   182,   183,     0,     0,   186,   187,   188,
     189,     0,     0,   378,     0,     0,     0,     0,     0,  1140,
       0,     0,     0,     0,     0,     0,   379,   380,   534,   535,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   381,   382,
     383,   384,   385,     0,   386,     0,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,    53,   397,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,   401,     0,   402,     0,
       0,   403,   404,     0,     0,     0,     0,     0,     0,     0,
       0,   405,     0,   406,   407,     0,   331,   536,   537,   538,
     539,   540,   534,   535,   541,   542,   543,   544,     0,   545,
     546,   547,   548,     0,     0,     0,   549,     0,   550,   551,
       0,     0,     0,     0,   552,   553,   554,     0,     0,     0,
     555,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     556,     0,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   567,   568,   816,   817,   818,   819,   820,   821,   822,
     823,   536,   537,   538,   539,   540,   824,   825,   541,   542,
     543,   544,   954,   545,   546,   547,   548,   534,   535,     0,
     549,   827,   550,   551,   828,   829,     0,     0,   552,   553,
     554,   830,   831,   832,   555,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   955,   556,     0,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   567,   568,     0,     0,     0,
       0,     0,  1001,     0,     0,     0,     0,     0,   816,   817,
     818,   819,   820,   821,   822,   823,   536,   537,   538,   539,
     540,   824,   825,   541,   542,   543,   544,   954,   545,   546,
     547,   548,   534,   535,     0,   549,   827,   550,   551,   828,
     829,     0,     0,   552,   553,   554,   830,   831,   832,   555,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   955,   556,
       0,   557,   558,   559,   560,   561,   562,   563,   564,   565,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     567,   568,     0,     0,     0,     0,     0,  1090,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
     205,   536,   537,   538,   539,   540,     0,     0,   541,   542,
     543,   544,     0,   545,   546,   547,   548,     0,     0,     0,
     549,     0,   550,   551,     0,     0,     0,     0,   552,   553,
     554,   206,     0,   207,   555,   208,   209,   210,   211,   212,
       0,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,     0,   224,   225,   226,     0,     0,   227,   228,
     229,   230,     0,     0,   556,     0,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,     0,   231,   232,     0,
       0,     0,     0,     0,     0,   567,   568,   536,   537,   538,
     539,   540,   993,     0,   541,   542,   543,   544,     0,   545,
     546,   547,   548,     0,     0,     0,   549,     0,   550,   551,
       0,     0,     0,     0,   552,   553,   554,     0,     0,     0,
     555,     0,     0,     0,     0,   534,   535,   233,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   155,
       0,     0,     0,     0,     0,   156,     0,     0,     0,     0,
     556,     0,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   157,     0,     0,     0,     0,     0,     0,     0,
       0,   567,   568,     0,     0,     0,     0,   158,  1184,     0,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,     0,   536,   537,   538,   539,   540,     0,
       0,   541,   542,   543,   544,     0,   545,   546,   547,   548,
     534,   535,     0,   549,     0,   550,   551,     0,     0,     0,
       0,   552,   553,   554,     0,     0,     0,   555,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   192,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,   556,     0,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   567,   568,
       0,     0,   569,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   440,   536,
     537,   538,   539,   540,   534,   535,   541,   542,   543,   544,
       0,   545,   546,   547,   548,     0,     0,     0,   549,     0,
     550,   551,     0,     0,     0,     0,   552,   553,   554,     0,
       0,     0,   555,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   556,     0,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   567,   568,     0,     0,   707,     0,     0,
       0,     0,     0,   536,   537,   538,   539,   540,   534,   535,
     541,   542,   543,   544,     0,   545,   546,   547,   548,     0,
       0,     0,   549,     0,   550,   551,     0,     0,     0,     0,
     552,   553,   554,     0,     0,     0,   555,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   556,     0,   557,   558,
     559,   560,   561,   562,   563,   564,   565,   566,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   567,   568,     0,
       0,   840,     0,     0,     0,     0,     0,   536,   537,   538,
     539,   540,   534,   535,   541,   542,   543,   544,     0,   545,
     546,   547,   548,     0,     0,     0,   549,     0,   550,   551,
       0,     0,     0,     0,   552,   553,   554,     0,     0,     0,
     555,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     556,     0,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   567,   568,     0,     0,   868,     0,     0,     0,     0,
       0,   536,   537,   538,   539,   540,   534,   535,   541,   542,
     543,   544,     0,   545,   546,   547,   548,     0,     0,     0,
     549,     0,   550,   551,     0,     0,     0,     0,   552,   553,
     554,     0,     0,     0,   555,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   556,     0,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   567,   568,     0,     0,   871,
       0,     0,     0,     0,     0,   536,   537,   538,   539,   540,
     534,   535,   541,   542,   543,   544,     0,   545,   546,   547,
     548,     0,     0,     0,   549,     0,   550,   551,     0,     0,
       0,     0,   552,   553,   554,     0,     0,     0,   555,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   556,     0,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   567,
     568,     0,     0,   873,     0,     0,     0,     0,     0,   536,
     537,   538,   539,   540,   534,   535,   541,   542,   543,   544,
       0,   545,   546,   547,   548,     0,     0,     0,   549,     0,
     550,   551,     0,     0,     0,     0,   552,   553,   554,     0,
       0,     0,   555,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   556,     0,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   567,   568,     0,     0,   875,     0,     0,
       0,     0,     0,   536,   537,   538,   539,   540,   534,   535,
     541,   542,   543,   544,     0,   545,   546,   547,   548,     0,
       0,     0,   549,     0,   550,   551,     0,     0,     0,     0,
     552,   553,   554,     0,     0,     0,   555,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   556,     0,   557,   558,
     559,   560,   561,   562,   563,   564,   565,   566,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   567,   568,     0,
       0,   876,     0,     0,     0,     0,     0,   536,   537,   538,
     539,   540,   534,   535,   541,   542,   543,   544,     0,   545,
     546,   547,   548,     0,     0,     0,   549,     0,   550,   551,
       0,     0,     0,     0,   552,   553,   554,     0,     0,     0,
     555,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     556,     0,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   567,   568,     0,     0,   877,     0,     0,     0,     0,
       0,   536,   537,   538,   539,   540,   534,   535,   541,   542,
     543,   544,     0,   545,   546,   547,   548,     0,     0,     0,
     549,     0,   550,   551,     0,     0,     0,     0,   552,   553,
     554,     0,     0,     0,   555,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   556,     0,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   567,   568,     0,     0,   878,
       0,     0,     0,     0,     0,   536,   537,   538,   539,   540,
     534,   535,   541,   542,   543,   544,     0,   545,   546,   547,
     548,     0,     0,     0,   549,     0,   550,   551,     0,     0,
       0,     0,   552,   553,   554,     0,     0,     0,   555,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   556,     0,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   567,
     568,     0,     0,   879,     0,     0,     0,     0,     0,   536,
     537,   538,   539,   540,   534,   535,   541,   542,   543,   544,
       0,   545,   546,   547,   548,     0,     0,     0,   549,     0,
     550,   551,     0,     0,     0,     0,   552,   553,   554,     0,
       0,     0,   555,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   556,     0,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   567,   568,     0,     0,   880,     0,     0,
       0,     0,     0,   536,   537,   538,   539,   540,   534,   535,
     541,   542,   543,   544,     0,   545,   546,   547,   548,     0,
       0,     0,   549,     0,   550,   551,     0,     0,     0,     0,
     552,   553,   554,     0,     0,     0,   555,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   556,     0,   557,   558,
     559,   560,   561,   562,   563,   564,   565,   566,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   567,   568,     0,
       0,   943,     0,     0,     0,     0,     0,   536,   537,   538,
     539,   540,   534,   535,   541,   542,   543,   544,     0,   545,
     546,   547,   548,     0,     0,     0,   549,     0,   550,   551,
       0,     0,     0,     0,   552,   553,   554,     0,     0,     0,
     555,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     556,     0,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   567,   568,     0,     0,  1008,     0,     0,     0,     0,
       0,   536,   537,   538,   539,   540,   534,   535,   541,   542,
     543,   544,     0,   545,   546,   547,   548,     0,     0,     0,
     549,     0,   550,   551,     0,     0,     0,     0,   552,   553,
     554,     0,     0,     0,   555,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   556,     0,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   567,   568,     0,     0,  1065,
       0,     0,     0,     0,     0,   536,   537,   538,   539,   540,
     534,   535,   541,   542,   543,   544,     0,   545,   546,   547,
     548,     0,     0,     0,   549,     0,   550,   551,     0,     0,
       0,     0,   552,   553,   554,     0,     0,     0,   555,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   556,     0,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   567,
     568,     0,     0,  1111,     0,     0,     0,     0,     0,   536,
     537,   538,   539,   540,   534,   535,   541,   542,   543,   544,
       0,   545,   546,   547,   548,     0,     0,     0,   549,     0,
     550,   551,     0,     0,     0,     0,   552,   553,   554,     0,
       0,     0,   555,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   556,     0,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   567,   568,     0,     0,  1112,     0,     0,
       0,     0,     0,   536,   537,   538,   539,   540,   534,   535,
     541,   542,   543,   544,     0,   545,   546,   547,   548,     0,
       0,     0,   549,     0,   550,   551,     0,     0,     0,     0,
     552,   553,   554,     0,     0,     0,   555,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   556,     0,   557,   558,
     559,   560,   561,   562,   563,   564,   565,   566,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   567,   568,     0,
       0,  1123,     0,     0,     0,     0,     0,   536,   537,   538,
     539,   540,   534,   535,   541,   542,   543,   544,     0,   545,
     546,   547,   548,     0,     0,     0,   549,     0,   550,   551,
       0,     0,     0,     0,   552,   553,   554,     0,     0,     0,
     555,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     556,     0,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   567,   568,     0,     0,  1125,     0,     0,     0,     0,
       0,   536,   537,   538,   539,   540,   534,   535,   541,   542,
     543,   544,     0,   545,   546,   547,   548,     0,     0,     0,
     549,     0,   550,   551,     0,     0,     0,     0,   552,   553,
     554,     0,     0,     0,   555,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   556,     0,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   567,   568,     0,     0,  1127,
       0,     0,     0,     0,     0,   536,   537,   538,   539,   540,
     534,   535,   541,   542,   543,   544,     0,   545,   546,   547,
     548,     0,     0,     0,   549,     0,   550,   551,     0,     0,
       0,     0,   552,   553,   554,     0,     0,     0,   555,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   556,     0,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   567,
     568,     0,     0,  1131,     0,     0,     0,     0,     0,   536,
     537,   538,   539,   540,   534,   535,   541,   542,   543,   544,
       0,   545,   546,   547,   548,     0,     0,     0,   549,     0,
     550,   551,     0,     0,     0,     0,   552,   553,   554,     0,
       0,     0,   555,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   556,     0,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   567,   568,     0,     0,  1170,     0,     0,
       0,     0,     0,   536,   537,   538,   539,   540,   534,   535,
     541,   542,   543,   544,     0,   545,   546,   547,   548,     0,
       0,     0,   549,     0,   550,   551,     0,     0,     0,     0,
     552,   553,   554,     0,     0,     0,   555,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   556,     0,   557,   558,
     559,   560,   561,   562,   563,   564,   565,   566,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   567,   568,     0,
       0,  1171,     0,     0,     0,     0,     0,   536,   537,   538,
     539,   540,   534,   535,   541,   542,   543,   544,     0,   545,
     546,   547,   548,     0,     0,     0,   549,     0,   550,   551,
       0,     0,     0,     0,   552,   553,   554,     0,     0,     0,
     555,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     556,     0,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   567,   568,     0,     0,  1172,     0,     0,     0,     0,
       0,   536,   537,   538,   539,   540,   534,   535,   541,   542,
     543,   544,     0,   545,   546,   547,   548,     0,     0,     0,
     549,     0,   550,   551,     0,     0,     0,     0,   552,   553,
     554,     0,     0,     0,   555,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   556,     0,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   567,   568,     0,     0,  1181,
       0,     0,     0,     0,     0,   536,   537,   538,   539,   540,
     534,   535,   541,   542,   543,   544,     0,   545,   546,   547,
     548,     0,     0,     0,   549,     0,   550,   551,     0,     0,
       0,     0,   552,   553,   554,     0,     0,     0,   555,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   556,     0,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   567,
     568,     0,     0,  1183,     0,     0,     0,     0,     0,   536,
     537,   538,   539,   540,   534,   535,   541,   542,   543,   544,
       0,   545,   546,   547,   548,     0,     0,     0,   549,     0,
     550,   551,     0,     0,     0,     0,   552,   553,   554,     0,
       0,     0,   555,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   556,     0,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   567,   568,     0,     0,  1219,     0,     0,
       0,   534,   535,   536,   537,   538,   539,   540,     0,     0,
     541,   542,   543,   544,     0,   545,   546,   547,   548,     0,
       0,     0,   549,     0,   550,   551,     0,     0,     0,     0,
     552,   553,   554,     0,     0,     0,   555,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   556,     0,   557,   558,
     559,   560,   561,   562,   563,   564,   565,   566,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   567,   568,   572,
     536,   537,   538,   539,   540,   534,   535,   541,   542,   543,
     544,     0,   545,   546,   547,   548,     0,     0,     0,   549,
       0,   550,   551,     0,     0,     0,     0,   552,   553,   554,
       0,     0,     0,   555,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   556,     0,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   567,   568,   926,     0,     0,     0,
       0,     0,   534,   535,   536,   537,   538,   539,   540,     0,
       0,   541,   542,   543,   544,     0,   545,   546,   547,   548,
       0,     0,     0,   549,     0,   550,   551,     0,     0,     0,
       0,   552,   553,   554,     0,     0,     0,   555,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   556,     0,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   567,   568,
     940,   536,   537,   538,   539,   540,   534,   535,   541,   542,
     543,   544,     0,   545,   546,   547,   548,     0,     0,     0,
     549,     0,   550,   551,     0,     0,     0,     0,   552,   553,
     554,     0,     0,     0,   555,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   556,     0,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   567,   568,  1060,     0,     0,
       0,     0,     0,     0,     0,   536,   537,   538,   539,   540,
       0,     0,   541,   542,   543,   544,     0,   545,   546,   547,
     548,   534,   535,     0,   549,     0,   550,   551,     0,     0,
       0,     0,   552,   553,   554,     0,     0,     0,   555,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   556,     0,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   567,
     568,  1063,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   816,   817,   818,   819,   820,   821,   822,   823,
     536,   537,   538,   539,   540,   824,   825,   541,   542,   543,
     544,   954,   545,   546,   547,   548,  -314,   276,   277,   549,
     827,   550,   551,   828,   829,     0,     0,   552,   553,   554,
     830,   831,   832,   555,   278,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   955,   556,     0,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   567,   568,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,     0,     0,   297,   298,
     299,     0,     0,     0,     0,     0,   300,   301,   302,   303,
     304,     0,     0,   305,   306,   307,   308,   309,   310,   311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   312,
       0,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,     0,     0,   323,   324,   536,   537,   538,   539,   540,
     325,   326,   541,   542,   543,   544,     0,   545,   546,   547,
     548,   534,   535,     0,   549,     0,   550,   551,     0,     0,
     699,     0,   552,   553,   554,     0,     0,     0,   555,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   556,     0,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   567,
     568,     0,     0,   719,     0,     0,     0,     0,     0,     0,
     534,   535,     0,     0,     0,     0,     0,     0,     0,     0,
     536,   537,   538,   539,   540,     0,     0,   541,   542,   543,
     544,     0,   545,   546,   547,   548,     0,     0,     0,   549,
       0,   550,   551,     0,     0,   886,     0,   552,   553,   554,
     159,   160,   161,   555,   163,   164,   165,   166,   167,   377,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,   181,   182,   183,     0,     0,   186,   187,   188,
     189,     0,     0,   556,     0,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,     0,   534,   535,     0,   536,
     537,   538,   539,   540,   567,   568,   541,   542,   543,   544,
       0,   545,   546,   547,   548,     0,     0,     0,   549,     0,
     550,   551,     0,     0,     0,     0,   552,   553,   554,     0,
       0,     0,   555,     0,     0,   720,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   721,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   556,   931,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,     0,   534,   535,     0,     0,     0,
       0,     0,     0,   567,   568,   536,   537,   538,   539,   540,
       0,     0,   541,   542,   543,   544,     0,   545,   546,   547,
     548,     0,     0,     0,   549,     0,   550,   551,     0,     0,
       0,     0,   552,   553,   554,     0,     0,     0,   555,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1033,     0,     0,     0,   556,     0,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
       0,   534,   535,     0,   536,   537,   538,   539,   540,   567,
     568,   541,   542,   543,   544,     0,   545,   546,   547,   548,
       0,     0,     0,   549,     0,   550,   551,     0,     0,     0,
       0,   552,   553,   554,     0,     0,     0,   555,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   556,     0,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,     0,
     534,   535,     0,     0,     0,     0,     0,     0,   567,   568,
     536,   537,   538,   539,   540,     0,     0,   541,   542,   543,
     544,     0,   545,   546,   547,   548,     0,     0,     0,   549,
       0,   550,   551,   534,   535,     0,     0,   552,   553,   554,
       0,     0,     0,  -649,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   556,     0,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,     0,     0,     0,     0,   536,
     537,   538,   539,   540,   567,   568,   541,   542,   543,   544,
       0,   545,   546,   547,   548,     0,     0,     0,   549,     0,
     550,   551,   534,   535,     0,     0,   552,   553,   554,     0,
       0,     0,   536,   537,   538,   539,   540,     0,     0,   541,
     542,   543,   544,     0,   545,   546,   547,   548,   534,   535,
       0,   549,     0,   550,   551,     0,     0,     0,     0,   552,
       0,   554,   556,     0,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   567,   568,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   557,   558,   559,
     560,   561,   562,   563,   564,   565,   566,     0,     0,     0,
       0,   536,   537,   538,   539,   540,   567,   568,   541,   542,
     543,   544,     0,   545,   546,   547,   548,   534,   535,     0,
     549,     0,   550,   551,     0,     0,     0,   536,   537,   538,
     539,   540,     0,     0,   541,   542,   543,   544,     0,   545,
     546,   547,   548,     0,     0,     0,   549,     0,   550,   551,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   567,   568,     0,     0,     0,
       0,     0,     0,     0,     0,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,   536,   537,   538,   539,
     540,   567,   568,   541,     0,     0,   544,     0,   545,   546,
     547,   548,     0,     0,     0,   549,     0,   550,   551,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   155,     0,
       0,     0,     0,     0,   156,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   560,   561,   562,   563,   564,   565,
     566,   157,     0,     0,     0,     0,     0,     0,     0,     0,
     567,   568,     0,     0,     0,     0,   158,     0,     0,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,     0,     0,     0,     0,
       0,   156,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,   157,     0,
       0,     0,   192,   723,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   158,     0,    53,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   377,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,   181,   182,   183,     0,     0,   186,   187,   188,
     189,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   927,     0,     0,   192,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   724,     0,     0,     0,     0,
       0,     0,     0,   159,   160,   161,   725,   163,   164,   165,
     166,   167,   377,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,   181,   182,   183,     0,     0,
     186,   187,   188,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   155,
       0,     0,     0,     0,     0,   156,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   157,     0,     0,     0,     0,     0,   928,     0,
       0,     0,     0,     0,     0,     0,     0,   158,     0,   929,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   155,     0,     0,     0,
       0,     0,   156,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,   157,
       0,     0,     0,   192,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   158,     0,    53,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     192,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488
};

static const yytype_int16 yycheck[] =
{
      12,   142,   460,   235,   578,   358,   570,   360,   713,   362,
     582,   121,   483,   517,   485,   417,   487,   467,   603,   604,
      88,     0,     7,    51,   445,    19,   271,     6,   591,   691,
      19,   121,    20,    21,    48,   697,    19,    14,    15,   602,
      21,    32,    54,    55,    51,    32,   125,    18,    19,   514,
      29,   516,    31,   119,    33,   135,   122,   123,   135,   161,
      39,    20,    21,   120,    44,   134,   122,   123,    33,    48,
     161,   170,   129,   161,    55,    54,   161,    89,    90,    91,
      92,   136,   137,   138,   161,   648,    32,   122,   123,    44,
      45,    11,    43,   173,   196,    74,    61,   120,   120,   120,
     685,   200,    22,    23,   668,    14,    15,   129,   129,   121,
     198,   202,    58,    59,   171,   150,   201,   147,    97,   176,
     100,   200,   152,   111,   112,   590,   161,   193,   194,    80,
     144,   119,   201,   121,   122,   123,   124,   193,   194,   171,
     128,   147,   197,   388,   389,   100,   152,   161,   171,   171,
     171,   174,   111,   112,   161,   164,   165,   120,   193,   194,
     119,   271,   121,   122,   123,   124,   129,   197,   200,   128,
     135,   117,   200,  1177,   242,   121,   170,   170,   155,   161,
     170,   170,   159,   888,   161,   176,   163,   170,   155,   176,
     161,   179,   180,   181,   182,   183,   161,    20,    21,   161,
     170,   182,  1206,   344,   197,   193,   194,   194,   171,   194,
     200,    60,   193,   170,   777,   194,   203,   202,   781,   196,
     202,   200,   187,   885,   171,   195,   172,   472,   170,   814,
     176,   196,   795,   179,   193,   194,   638,   170,   195,    32,
     202,   351,   352,   353,   354,   666,   155,   357,   194,   359,
     159,   361,   161,   363,   163,   135,   120,   203,   200,   271,
     505,   351,   352,   353,   354,    58,    59,   357,   120,   359,
     161,   361,   152,   153,   154,   161,   135,   129,   388,   389,
     843,   161,  1162,   135,   197,   144,   173,   200,   111,   112,
     149,     4,     5,   170,     7,   174,   119,   177,   170,   122,
     123,   124,   161,   173,   170,   128,   170,   171,   170,   173,
    1190,   202,   176,   170,   201,   178,   202,   881,   177,   171,
     197,   200,    35,   170,   117,   197,   170,   120,   121,   144,
     894,   197,   170,   195,   444,  1040,   129,   200,   195,   351,
     352,   353,   354,   170,   178,   357,   161,   359,   458,   361,
     197,   363,   161,   197,   444,   367,  1018,   170,   178,   197,
     924,   593,   472,   813,   161,   718,   200,  1029,   458,   601,
     193,   194,    20,    21,   170,   947,   388,   389,   171,   172,
     200,   885,   195,   176,    54,    55,   179,   951,    69,   135,
     402,   895,    73,   150,   178,   505,   981,   982,   144,   195,
     144,   194,   161,   173,   161,   990,   991,   200,   178,   161,
     203,    92,    93,    94,    95,   161,   200,   161,   173,    89,
      90,    91,    92,   178,   534,   535,   196,   672,   673,   150,
     675,   173,   444,   173,   196,   135,   178,    55,   178,   201,
     161,   196,   150,    61,   161,   162,   458,   174,   919,  1021,
    1014,   178,   464,   161,   196,   174,   196,   702,   196,   178,
     472,   109,   110,   111,   112,   113,   183,   195,   116,   117,
     118,   119,   200,   121,   122,   123,   124,   200,   170,   202,
     128,   173,   130,   131,   176,   497,    55,   174,   260,    55,
      55,   178,    61,   505,    55,    61,    61,   269,    55,   174,
      61,   174,   174,   178,    61,   178,   178,   174,   174,   161,
     162,   178,   178,    20,    21,   174,   174,   155,   530,   178,
     178,   164,   165,   166,   167,   193,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   198,   199,   136,   201,
     138,   651,   161,    20,    21,   193,   194,   779,   136,   137,
     138,   200,   662,   202,   664,   908,   788,   667,   164,   165,
     670,   651,   672,   673,   171,   675,   200,  1038,   201,   801,
     161,  1145,   662,   805,   164,   165,   166,   667,    75,    76,
     670,   161,    55,   161,   201,   200,   136,   201,   100,   152,
     177,   177,   702,   177,   177,   196,   177,   196,   177,   174,
     177,   177,   109,   110,   111,   112,   113,   379,   380,   116,
     117,   118,   119,  1071,   121,   122,   123,   124,   390,   177,
     161,   128,   177,   130,   131,    34,   200,   399,   400,   401,
      34,   403,   404,   405,   111,   112,   746,   196,   161,   651,
     202,   201,   119,   161,   121,   122,   123,   124,   173,   200,
     662,   128,   664,   161,    21,   667,   161,   161,   670,   195,
     672,   673,   907,   675,   195,   775,   161,   173,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   201,    41,   173,
     201,   196,   199,   197,   177,   775,   193,   194,   460,   177,
     702,   177,   177,   196,   196,   196,   196,   469,   839,   177,
     196,   177,   194,   161,   181,   182,   183,   196,   161,   196,
      20,    21,   196,   196,   196,   196,   193,   194,   161,   178,
     195,   152,   196,   195,   956,   196,   161,   499,   196,    41,
     502,   161,   504,   200,   155,    36,   508,   509,   510,   511,
     512,   513,     9,   515,   200,   517,   200,    64,   200,   200,
     200,   200,   200,    41,   161,   178,   178,   170,   178,   195,
     170,   202,   164,   775,   536,   537,   161,   196,   540,   541,
     542,   543,   196,   545,   177,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   196,   568,   907,   177,   109,
     110,   111,   112,   113,  1049,   577,   116,   117,   118,   119,
     196,   121,   122,   123,   124,   587,  1048,     1,   128,   591,
     130,   131,   594,   595,   836,   196,   136,   196,   600,   161,
     602,   196,    32,   605,   161,   125,    13,   171,   610,   155,
    1072,   199,   173,     7,   201,   196,   161,   161,   161,   195,
     195,   195,   202,    32,   195,   164,   161,   161,    58,    59,
     196,   161,   161,   635,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   177,   196,   648,   196,    41,    58,
      59,   161,   200,   193,   194,   196,   161,   195,   195,   195,
      32,   178,   178,   195,    51,   907,   522,   669,   195,   197,
     195,    68,   195,  1135,   200,   197,   195,  1139,   202,   200,
     195,    38,   195,   195,   118,   687,   688,   117,   202,   200,
      71,   121,    32,  1174,   696,  1175,    69,   699,   700,   836,
     966,  1175,   704,  1175,  1175,  1175,    32,     1,   117,  1049,
    1046,  1051,   121,   874,  1088,  1085,   664,   584,    58,    59,
     364,   644,    47,   893,  1159,   367,   356,   700,    -1,   367,
      -1,  1051,    58,    59,   517,    -1,    -1,    -1,   247,    -1,
     461,    -1,   172,    -1,    -1,    -1,   176,    -1,   178,   179,
      -1,    -1,    -1,    -1,    -1,    -1,  1218,   759,    -1,    -1,
      -1,   763,    -1,   172,   194,    -1,    -1,   176,   770,   178,
     179,    -1,    -1,   203,    -1,   777,    -1,   117,    -1,   781,
      -1,   121,    -1,    -1,   786,   194,    -1,    -1,  1128,    -1,
      -1,   117,    -1,   795,   203,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1049,  1128,  1051,
      -1,    -1,    -1,    -1,   816,   817,   818,   819,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,   830,   831,
     832,   833,   172,    32,    -1,    -1,   176,    -1,   178,   179,
      -1,   843,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,
     176,    -1,   178,   179,   194,    -1,    -1,    -1,    -1,    58,
      59,    -1,    -1,   203,    -1,    -1,    -1,    -1,   194,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,
      -1,   883,   884,    -1,   886,    -1,  1128,    -1,    -1,   891,
      -1,   893,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   905,   906,    -1,    -1,    -1,    -1,   911,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,   117,   921,
    1162,   923,   121,   925,    20,    21,    -1,    -1,    -1,   931,
      -1,    -1,    -1,   935,    -1,    -1,    -1,    -1,    -1,   941,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1190,    -1,
      -1,    -1,   954,   955,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   968,    32,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    -1,    -1,   176,    -1,   178,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,   194,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   203,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    20,
      21,    -1,   128,   129,   130,   131,   132,   133,    -1,    -1,
     136,   137,   138,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,  1057,   121,  1059,    -1,    -1,
    1062,    -1,    -1,    -1,    -1,  1067,    20,    21,  1070,  1071,
      -1,    -1,    -1,  1075,    -1,   171,   172,    -1,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,    -1,
    1092,    -1,    -1,  1095,  1096,  1097,  1098,   193,   194,  1101,
      -1,    -1,  1104,  1105,    -1,  1107,    -1,   172,  1110,    -1,
      -1,   176,    -1,   178,   179,    -1,    -1,  1119,   109,   110,
     111,   112,   113,    -1,    -1,   116,   117,   118,   119,   194,
     121,   122,   123,   124,    -1,    -1,    -1,   128,   203,   130,
     131,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,
      -1,   142,    -1,    -1,    -1,   109,   110,   111,   112,    -1,
      -1,    -1,    -1,  1165,    -1,   119,    -1,   121,   122,   123,
     124,    -1,    -1,  1175,   128,    -1,   130,   131,    -1,    -1,
    1182,   172,    -1,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,    -1,    -1,  1197,  1198,    -1,    -1,    -1,
      -1,    -1,   193,   194,    -1,    -1,  1208,     1,    -1,    -1,
     201,  1213,     6,  1215,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,   179,   180,   181,   182,   183,
      -1,    25,    26,    27,    28,    -1,    -1,    -1,    -1,   193,
     194,    -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,
      -1,    -1,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,   145,   146,   147,    -1,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   179,   180,   181,    -1,   183,
      -1,    -1,   186,   187,    -1,    -1,    58,    59,    -1,    -1,
      -1,    -1,   196,    -1,   198,   199,   200,   201,   202,     1,
      -1,    -1,    -1,    -1,     6,    -1,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,
      42,    43,    -1,    -1,    46,   117,    48,    49,    50,   121,
      52,    53,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    99,    -1,    -1,
     172,    -1,    -1,    -1,   176,    -1,   178,   179,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,    -1,   126,   127,    -1,    -1,    -1,    -1,
      -1,   203,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,   147,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,   180,   181,
      -1,   183,    -1,    -1,   186,   187,    -1,    -1,    -1,    14,
      15,    16,    17,    18,   196,    -1,   198,   199,   200,   201,
     202,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    51,    -1,    53,    -1,
      -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,
      -1,   176,    -1,    -1,   179,   180,   181,    -1,   183,    -1,
      -1,   186,   187,    14,    15,    16,    17,    18,    -1,   194,
      -1,   196,    -1,   198,   199,    26,   201,    -1,   203,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    -1,    -1,   176,    -1,    -1,   179,   180,
     181,    -1,   183,    -1,    -1,   186,   187,    14,    15,    16,
      17,    18,    -1,   194,    -1,   196,    -1,   198,   199,    26,
     201,    -1,   203,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,   179,   180,   181,    -1,   183,    -1,    -1,   186,
     187,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,   196,
      -1,   198,   199,    -1,   201,    -1,   203,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   179,   180,   181,    -1,   183,    -1,    -1,   186,
     187,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   196,
      46,   198,   199,    49,   201,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   179,   180,   181,    -1,   183,    -1,    -1,
     186,   187,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     196,    46,   198,   199,    49,   201,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   179,   180,   181,    -1,   183,    -1,
      -1,   186,   187,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   196,    46,   198,   199,    49,   201,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,   145,   146,   147,    -1,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   179,   180,   181,    -1,   183,
      -1,    -1,   186,   187,    14,    15,    16,    17,    18,    -1,
     194,    -1,   196,   197,   198,   199,    26,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   179,
     180,   181,    -1,   183,    -1,    -1,   186,   187,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   196,    46,   198,   199,
      49,   201,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     179,   180,   181,    -1,   183,    -1,    -1,   186,   187,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   196,    46,   198,
     199,    49,   201,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,
      -1,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   179,   180,   181,    -1,   183,    -1,    -1,   186,   187,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   196,    46,
     198,   199,    49,   201,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      26,    -1,   179,   180,   181,    -1,   183,    -1,    -1,   186,
     187,    -1,    38,    -1,    -1,    -1,    -1,    43,   195,   196,
      46,   198,   199,    49,   201,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   179,   180,   181,    -1,   183,    -1,    -1,
     186,   187,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     196,    46,   198,   199,    49,   201,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,   180,   181,    -1,   183,    -1,
      -1,   186,   187,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   196,   197,   198,   199,    26,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   179,   180,
     181,    -1,   183,    -1,    -1,   186,   187,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   196,    46,   198,   199,    49,
     201,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   179,
     180,   181,    -1,   183,    -1,    -1,   186,   187,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   196,    46,   198,   199,
      49,   201,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
     179,   180,   181,    -1,   183,    -1,    -1,   186,   187,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   196,    -1,   198,
     199,    26,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,   180,   181,    -1,   183,    -1,
      -1,   186,   187,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   196,   197,   198,   199,    26,   201,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   179,   180,
     181,    -1,   183,    -1,    -1,   186,   187,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   196,    -1,   198,   199,    26,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179,   180,   181,    -1,   183,    -1,    -1,   186,
     187,    14,    15,    16,    17,    18,    -1,    -1,    -1,   196,
     197,   198,   199,    26,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,   145,   146,   147,    -1,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,   180,   181,    -1,
     183,    -1,    -1,   186,   187,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   196,   197,   198,   199,    26,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     179,   180,   181,    -1,   183,    -1,    26,   186,   187,    -1,
      -1,    -1,    -1,    -1,    -1,   194,    -1,   196,    38,   198,
     199,    -1,   201,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,
     180,   181,    -1,   183,    -1,    -1,   186,   187,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   196,   197,   198,   199,
      26,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   179,   180,   181,    -1,   183,    -1,    -1,
     186,   187,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     196,   197,   198,   199,    26,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,   147,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,   180,   181,
      -1,   183,    -1,    -1,   186,   187,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   196,   197,   198,   199,    26,   201,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,
      -1,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   179,   180,   181,    -1,   183,    -1,    -1,   186,   187,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   196,    46,
     198,   199,    49,   201,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   179,   180,   181,    -1,   183,    -1,    -1,   186,
     187,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   196,
      46,   198,   199,    49,   201,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   179,   180,   181,    -1,   183,    -1,    -1,
     186,   187,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     196,    46,   198,   199,    49,   201,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,   180,   181,    -1,   183,    -1,
      -1,   186,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   196,    -1,   198,   199,    -1,   201,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,   128,    -1,   130,   131,
      -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   194,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    20,    21,    -1,
     128,   129,   130,   131,   132,   133,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   171,   172,    -1,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   194,    -1,    -1,    -1,
      -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    20,    21,    -1,   128,   129,   130,   131,   132,
     133,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,   172,
      -1,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   194,    -1,    -1,    -1,    -1,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      34,   109,   110,   111,   112,   113,    -1,    -1,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
     128,    -1,   130,   131,    -1,    -1,    -1,    -1,   136,   137,
     138,    65,    -1,    67,   142,    69,    70,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,   172,    -1,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   194,   109,   110,   111,
     112,   113,   200,    -1,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,   128,    -1,   130,   131,
      -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    20,    21,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   194,    -1,    -1,    -1,    -1,    62,   200,    -1,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,   109,   110,   111,   112,   113,    -1,
      -1,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      20,    21,    -1,   128,    -1,   130,   131,    -1,    -1,    -1,
      -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,   172,    -1,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   194,
      -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,   128,    -1,
     130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   194,    -1,    -1,   197,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    20,    21,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,   128,    -1,   130,   131,    -1,    -1,    -1,    -1,
     136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   194,    -1,
      -1,   197,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,   128,    -1,   130,   131,
      -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   194,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
     128,    -1,   130,   131,    -1,    -1,    -1,    -1,   136,   137,
     138,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   194,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,   128,    -1,   130,   131,    -1,    -1,
      -1,    -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     194,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,   128,    -1,
     130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   194,    -1,    -1,   197,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    20,    21,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,   128,    -1,   130,   131,    -1,    -1,    -1,    -1,
     136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   194,    -1,
      -1,   197,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,   128,    -1,   130,   131,
      -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   194,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
     128,    -1,   130,   131,    -1,    -1,    -1,    -1,   136,   137,
     138,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   194,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,   128,    -1,   130,   131,    -1,    -1,
      -1,    -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     194,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,   128,    -1,
     130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   194,    -1,    -1,   197,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    20,    21,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,   128,    -1,   130,   131,    -1,    -1,    -1,    -1,
     136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   194,    -1,
      -1,   197,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,   128,    -1,   130,   131,
      -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   194,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
     128,    -1,   130,   131,    -1,    -1,    -1,    -1,   136,   137,
     138,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   194,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,   128,    -1,   130,   131,    -1,    -1,
      -1,    -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     194,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,   128,    -1,
     130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   194,    -1,    -1,   197,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    20,    21,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,   128,    -1,   130,   131,    -1,    -1,    -1,    -1,
     136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   194,    -1,
      -1,   197,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,   128,    -1,   130,   131,
      -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   194,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
     128,    -1,   130,   131,    -1,    -1,    -1,    -1,   136,   137,
     138,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   194,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,   128,    -1,   130,   131,    -1,    -1,
      -1,    -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     194,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,   128,    -1,
     130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   194,    -1,    -1,   197,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    20,    21,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,   128,    -1,   130,   131,    -1,    -1,    -1,    -1,
     136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   194,    -1,
      -1,   197,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,   128,    -1,   130,   131,
      -1,    -1,    -1,    -1,   136,   137,   138,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   194,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
     128,    -1,   130,   131,    -1,    -1,    -1,    -1,   136,   137,
     138,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   194,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,   128,    -1,   130,   131,    -1,    -1,
      -1,    -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     194,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,   128,    -1,
     130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   194,    -1,    -1,   197,    -1,    -1,
      -1,    20,    21,   109,   110,   111,   112,   113,    -1,    -1,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,   128,    -1,   130,   131,    -1,    -1,    -1,    -1,
     136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   194,   195,
     109,   110,   111,   112,   113,    20,    21,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,   128,
      -1,   130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    20,    21,   109,   110,   111,   112,   113,    -1,
      -1,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      -1,    -1,    -1,   128,    -1,   130,   131,    -1,    -1,    -1,
      -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   194,
     195,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
     128,    -1,   130,   131,    -1,    -1,    -1,    -1,   136,   137,
     138,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    20,    21,    -1,   128,    -1,   130,   131,    -1,    -1,
      -1,    -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    20,    21,   128,
     129,   130,   131,   132,   133,    -1,    -1,   136,   137,   138,
     139,   140,   141,   142,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,   172,    -1,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   194,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,   132,
     133,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,    -1,    -1,   186,   187,   109,   110,   111,   112,   113,
     193,   194,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    20,    21,    -1,   128,    -1,   130,   131,    -1,    -1,
     134,    -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     194,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,   128,
      -1,   130,   131,    -1,    -1,   134,    -1,   136,   137,   138,
      65,    66,    67,   142,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,   172,    -1,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,    -1,    20,    21,    -1,   109,
     110,   111,   112,   113,   193,   194,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,   128,    -1,
     130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,    -1,
      -1,    -1,   142,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   194,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,   128,    -1,   130,   131,    -1,    -1,
      -1,    -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   168,    -1,    -1,    -1,   172,    -1,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,    20,    21,    -1,   109,   110,   111,   112,   113,   193,
     194,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
      -1,    -1,    -1,   128,    -1,   130,   131,    -1,    -1,    -1,
      -1,   136,   137,   138,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   193,   194,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,   128,
      -1,   130,   131,    20,    21,    -1,    -1,   136,   137,   138,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   193,   194,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,   128,    -1,
     130,   131,    20,    21,    -1,    -1,   136,   137,   138,    -1,
      -1,    -1,   109,   110,   111,   112,   113,    -1,    -1,   116,
     117,   118,   119,    -1,   121,   122,   123,   124,    20,    21,
      -1,   128,    -1,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   193,   194,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    20,    21,    -1,
     128,    -1,   130,   131,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,   128,    -1,   130,   131,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   194,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   193,   194,   116,    -1,    -1,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,   128,    -1,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,   182,
     183,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   194,    -1,    -1,    -1,    -1,    62,    -1,    -1,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    -1,   148,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,   161,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,   161,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,   161,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,   161,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   205,     0,     6,    29,    31,    33,    39,    48,    54,
      74,    97,   194,   200,   208,   216,   217,   222,   250,   254,
     280,   348,   354,   357,   363,   402,   405,    18,    19,   161,
     243,   244,   245,   155,   223,   224,   161,   162,   183,   218,
     219,   220,   161,   201,   351,   161,   198,   207,   406,   403,
      33,    61,   135,   161,   187,   196,   246,   247,   248,   249,
     267,     4,     5,     7,    35,   361,    60,   346,   171,   170,
     173,   170,   219,    21,    55,   182,   193,   221,   352,   351,
     353,   346,   161,   161,   161,   135,   248,   248,   196,   136,
     137,   138,   170,   195,    55,    61,   255,   257,    55,    61,
     355,    55,    61,   362,    55,    61,   347,    14,    15,   155,
     159,   161,   163,   196,   210,   244,   155,   224,   161,   218,
     218,   171,   200,   202,   351,    55,    61,   206,   201,   404,
     161,   197,   245,   248,   248,   248,   248,   258,   161,   356,
     364,   201,   349,   164,   165,   209,    14,    15,   155,   159,
     161,   210,   241,   242,   221,    24,    30,    47,    62,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,   148,   267,   367,   369,   370,   372,   376,   401,
     200,   161,   371,   201,   197,    34,    65,    67,    69,    70,
      71,    72,    73,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    87,    88,    89,    92,    93,    94,
      95,   111,   112,   161,   253,   256,   173,   201,   100,   359,
     360,   344,   152,   323,   164,   165,   166,   170,   197,   177,
     177,   177,   177,   196,   177,   177,   177,   177,   177,   177,
     196,    32,    58,    59,   117,   121,   172,   176,   179,   194,
     203,   174,   200,   161,   336,   337,    20,    21,    37,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   121,   122,   123,
     129,   130,   131,   132,   133,   136,   137,   138,   139,   140,
     141,   142,   172,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   186,   187,   193,   194,    34,    34,   196,
     251,   201,   259,    69,    73,    92,    93,    94,    95,   368,
     350,   161,   365,   202,   345,   245,   144,   161,   342,   343,
     241,   379,   381,   383,   377,   161,   373,   385,   387,   389,
     391,   393,   395,   397,   399,    14,    15,    16,    17,    18,
      26,    38,    43,    46,    49,    53,    63,    74,    98,   111,
     112,   143,   144,   145,   146,   147,   149,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   162,   163,   179,
     180,   181,   183,   186,   187,   196,   198,   199,   212,   214,
     259,   263,   267,   272,   283,   290,   293,   296,   300,   301,
     304,   305,   307,   312,   315,   322,   367,   407,   411,   415,
     417,   419,    32,   176,    32,   176,   194,   203,   195,   315,
     203,   376,   161,   202,   173,   200,   161,   161,   161,   195,
      21,   161,   195,   147,   197,   323,   332,   333,   173,   252,
     261,   201,   161,   202,   173,   358,   201,   323,   195,   196,
      41,   170,   173,   176,   341,   401,   375,   401,   401,   401,
     197,   371,   401,   251,   401,   251,   401,   251,   161,   334,
     335,   401,   337,   177,   269,   369,   407,   196,   177,   196,
     177,   177,   196,   177,   196,   177,   315,   315,   196,   196,
     196,   196,   196,   196,    12,   376,    12,   376,   315,   413,
     416,   177,   211,   315,   315,   315,   267,   315,   315,   315,
     199,   161,   196,   250,    20,    21,   109,   110,   111,   112,
     113,   116,   117,   118,   119,   121,   122,   123,   124,   128,
     130,   131,   136,   137,   138,   142,   172,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   193,   194,   197,
     196,   195,   195,   401,   202,   336,   161,   196,    48,   144,
     161,   339,   366,   197,   200,   401,     1,     8,     9,    10,
      12,    25,    27,    28,    37,    40,    42,    50,    52,    56,
      57,    63,    99,   126,   127,   162,   200,   202,   225,   226,
     229,   232,   233,   235,   236,   237,   238,   239,   260,   262,
     266,   268,   273,   274,   275,   276,   277,   278,   279,   280,
     282,   302,   303,   315,   350,   171,   200,   267,   328,   343,
     195,   315,   161,   161,   376,   120,   129,   171,   340,   178,
     178,   200,   178,   178,   178,   200,   178,   252,   178,   252,
     178,   252,   173,   178,   200,   178,   200,   270,   196,   267,
     291,   315,   284,   286,   315,   288,   315,   376,   315,   315,
     315,   315,   315,   315,   366,    51,   161,   179,   196,   315,
     408,   409,   410,   412,   414,   366,   196,   409,   414,   134,
     200,   202,   264,   166,   167,   209,   215,   197,   144,   149,
     177,   267,   306,   194,   197,   294,   315,   152,   299,    18,
     150,   161,   367,    18,   150,   161,   367,   315,   315,   315,
     315,   315,   315,   161,   315,   150,   161,   315,   315,   315,
     315,   315,   315,   315,   315,   315,    21,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   122,   123,
     150,   161,   193,   194,   313,   315,   197,   294,   315,   339,
     196,    41,   120,   170,   171,   173,   176,   338,   332,   315,
     366,   120,   266,   302,   315,   259,    59,   315,   315,   161,
     200,   155,    56,   315,   259,   120,   266,   315,   199,   300,
     300,   315,    36,   200,   200,   315,     9,   200,   200,   200,
     200,   200,    64,   281,   125,   200,   101,   102,   103,   104,
     105,   106,   107,   108,   114,   115,   120,   129,   132,   133,
     139,   140,   141,   171,   202,   315,   194,   202,   250,   329,
     197,    41,   200,   340,   266,   315,   380,   382,   401,   384,
     378,   374,   386,   178,   390,   178,   394,   178,   401,   398,
     334,   400,   401,   197,   294,   177,   315,   401,   197,   376,
     376,   197,   376,   197,   178,   197,   197,   197,   197,   197,
     197,    19,   300,   129,   338,   197,   134,   170,   200,   410,
     195,   170,   195,   200,    19,   197,   410,   202,   315,   413,
     202,   376,   315,   164,   213,   196,   196,   308,   310,   161,
     408,   170,   197,   120,   129,   171,   176,   297,   298,   251,
     177,   196,   177,   196,   196,   196,   195,    18,   150,   161,
     367,   173,   150,   161,   315,   196,   150,   161,   315,     1,
     195,   170,   197,   197,   315,   161,   161,   401,   266,   315,
     259,    19,   266,   315,   120,   171,    13,   315,   259,   171,
     173,   155,   266,   315,   199,   259,   201,   259,   234,   343,
     300,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   126,   127,   315,   315,   315,   315,   315,   315,   315,
     126,   127,   315,   200,   249,     7,   323,   327,   161,   266,
     315,   200,   388,   392,   396,   178,   197,   161,   197,   178,
     178,   178,   178,   299,   196,   294,   315,   315,   409,   410,
     315,   161,   408,   195,   195,   315,   195,   412,   294,   409,
     202,   195,   178,   168,   164,   315,   315,   376,   251,   195,
     194,   315,   161,   161,   161,   161,   170,   195,   252,   316,
     315,   318,   315,   197,   294,   315,   177,   196,   315,   196,
     195,   315,   196,   195,   314,   197,    41,   338,   294,   259,
     240,   261,    11,    22,    23,   227,   228,   315,   300,   300,
     300,   300,   195,    55,    61,   326,    44,   100,   324,   200,
     200,   271,   178,   200,   292,   285,   287,   289,   196,   197,
     294,   200,   410,   195,   129,   338,   195,   200,   410,   195,
     265,   197,   197,   178,   252,   197,   408,   297,   195,   134,
     259,   295,   376,   197,   401,   197,   197,   197,   320,   315,
     315,   197,   315,   161,   315,   200,   315,   202,   259,   315,
      11,   230,   200,    45,   324,   326,   315,   161,   315,   315,
     315,   197,   315,   197,   315,   195,   195,   315,   315,   315,
     202,   315,   309,   178,   195,   120,   315,   178,   178,   401,
     197,   197,   197,   259,   259,   231,   200,   330,    32,   325,
     339,   197,   178,   197,   200,   418,   195,   418,   195,   306,
     311,   197,   315,   317,   319,   178,   228,    25,    99,   232,
     273,   274,   275,   277,   315,   256,   331,   315,    51,   195,
     202,   306,   321,   120,   315,   120,   315,   200,   256,   197,
     315,   195,   195,   315,   315,   259
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   204,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   206,   206,   206,   207,   207,
     208,   209,   209,   209,   209,   210,   211,   211,   211,   212,
     213,   213,   215,   214,   216,   217,   218,   218,   219,   219,
     219,   219,   220,   220,   221,   221,   222,   223,   223,   224,
     224,   225,   226,   226,   227,   227,   228,   228,   228,   229,
     229,   230,   231,   230,   232,   232,   232,   232,   232,   233,
     234,   233,   235,   236,   237,   238,   240,   239,   241,   241,
     241,   241,   241,   241,   242,   242,   243,   243,   243,   244,
     244,   244,   244,   244,   244,   244,   244,   245,   245,   246,
     246,   246,   247,   247,   248,   248,   248,   248,   248,   248,
     249,   249,   250,   250,   251,   251,   251,   252,   252,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   254,   255,   255,   255,   256,   258,   257,
     259,   259,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   261,   261,   261,   262,   264,   265,   263,
     266,   266,   266,   267,   267,   267,   268,   268,   270,   271,
     269,   269,   272,   272,   272,   272,   273,   274,   275,   275,
     275,   276,   276,   276,   277,   277,   278,   278,   278,   279,
     280,   280,   281,   281,   282,   284,   285,   283,   286,   287,
     283,   288,   289,   283,   291,   292,   290,   293,   293,   293,
     294,   294,   295,   295,   295,   296,   296,   296,   297,   297,
     297,   297,   298,   298,   299,   299,   300,   300,   301,   301,
     301,   301,   301,   301,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   303,   303,   303,   303,   304,   304,   305,
     305,   306,   306,   307,   308,   309,   307,   310,   311,   307,
     312,   312,   313,   314,   312,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   316,   317,   315,   315,   315,   315,
     318,   319,   315,   315,   315,   320,   321,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   323,   323,   324,
     324,   324,   325,   325,   326,   326,   326,   327,   328,   329,
     328,   330,   328,   331,   328,   328,   332,   332,   332,   333,
     333,   334,   334,   335,   335,   336,   337,   337,   338,   338,
     339,   339,   339,   339,   339,   339,   340,   340,   340,   341,
     341,   342,   342,   342,   342,   342,   343,   343,   343,   343,
     343,   344,   345,   344,   346,   346,   347,   347,   347,   348,
     349,   348,   350,   350,   350,   352,   351,   353,   353,   354,
     354,   355,   355,   355,   356,   357,   357,   358,   358,   359,
     359,   360,   361,   361,   362,   362,   362,   364,   365,   363,
     366,   366,   366,   366,   366,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   368,   368,   368,   368,   368,   368,   369,   370,
     370,   370,   371,   371,   373,   374,   372,   375,   375,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   377,   378,   376,
     376,   379,   380,   376,   381,   382,   376,   383,   384,   376,
     376,   385,   386,   376,   387,   388,   376,   376,   389,   390,
     376,   391,   392,   376,   376,   393,   394,   376,   395,   396,
     376,   397,   398,   376,   399,   400,   376,   401,   401,   401,
     403,   404,   402,   406,   405,   407,   407,   407,   407,   408,
     408,   408,   408,   409,   409,   410,   410,   411,   411,   411,
     411,   411,   411,   412,   412,   412,   413,   413,   414,   414,
     415,   415,   416,   416,   417,   418,   418,   419,   419
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
       1,     3,     0,     3,     0,     2,     3,     0,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     2,     2,     3,     3,     2,
       2,     2,     2,     2,     3,     3,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     1,     1,     3,     0,     4,
       3,     7,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     2,     0,     2,     2,     3,     0,     0,     7,
       3,     2,     2,     1,     3,     2,     2,     3,     0,     0,
       5,     1,     2,     4,     5,     2,     1,     1,     1,     2,
       3,     2,     2,     3,     2,     3,     2,     2,     3,     4,
       1,     1,     1,     0,     3,     0,     0,     7,     0,     0,
       7,     0,     0,     7,     0,     0,     6,     5,     8,    10,
       1,     3,     1,     2,     3,     1,     1,     2,     2,     2,
       2,     2,     1,     3,     0,     4,     1,     6,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     4,     4,     6,     8,     5,
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
       1,     4,     4,     4,     4,     4,     1,     6,     7,     6,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     1,     3,     0,     0,     6,     1,     3,     1,
       1,     1,     1,     4,     3,     4,     2,     2,     3,     2,
       3,     2,     2,     3,     3,     3,     2,     0,     0,     6,
       2,     0,     0,     6,     0,     0,     6,     0,     0,     6,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     0,     0,     6,     0,     0,     6,     1,     3,     3,
       0,     0,     8,     0,     7,     1,     1,     1,     1,     3,
       3,     5,     5,     1,     3,     0,     2,     6,     5,     7,
       8,     6,     8,     1,     3,     3,     3,     1,     1,     3,
       5,     5,     1,     3,     4,     0,     3,    10,    10
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

  case 110: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 111: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 112: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 113: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 114: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 115: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 116: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 117: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 118: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 119: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 120: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 121: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 122: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 123: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 124: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 125: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 126: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 127: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 128: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 129: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 130: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 131: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 132: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 133: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 134: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 135: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 136: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 137: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 138: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 139: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 140: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 141: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 142: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 143: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 144: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 145: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 146: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 147: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 148: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 149: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 150: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 151: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 152: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 153: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 154: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 155: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 156: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 157: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 158: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 159: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 160: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 161: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 162: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 163: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 164: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 165: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 166: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 167: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 168: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 169: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 170: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 171: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 172: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 173: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 174: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 175: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 176: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 177: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 178: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 179: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 180: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 181: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 182: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 183: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 184: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 185: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 186: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 187: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 188: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 189: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 190: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 191: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 192: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 193: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 194: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 195: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 196: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 197: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 198: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 199: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 200: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 201: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 202: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 203: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 204: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 205: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 206: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 207: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 208: /* $@5: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 209: /* function_declaration: optional_public_or_private_function $@5 function_declaration_header expression_block  */
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

  case 210: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 211: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 212: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 213: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 214: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 215: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 216: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 217: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 218: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 219: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 220: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 221: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 222: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 225: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 226: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 227: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 231: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 232: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 233: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 234: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 235: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 236: /* expr_keyword: "keyword" expr expression_block  */
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

  case 237: /* $@6: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 238: /* $@7: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 239: /* expression_keyword: "keyword" '<' $@6 type_declaration_no_options '>' $@7 expr  */
                                                                                                                                               {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),*(yyvsp[-6].s));
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCall->arguments.push_back(ExpressionPtr(td));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 240: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 241: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 242: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 243: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 244: /* name_in_namespace: "name" "::" "name"  */
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

  case 245: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 246: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 247: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 248: /* $@8: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 249: /* $@9: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 250: /* new_type_declaration: '<' $@8 type_declaration '>' $@9  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 251: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 252: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 253: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 254: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 255: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 256: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 257: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 258: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 259: /* expression_return_no_pipe: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 260: /* expression_return_no_pipe: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 261: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 262: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 263: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 264: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 265: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 266: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 267: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 268: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 269: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 270: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 271: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 272: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 273: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 274: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 275: /* $@10: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 276: /* $@11: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 277: /* expr_cast: "cast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 278: /* $@12: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 279: /* $@13: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 280: /* expr_cast: "upcast" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 281: /* $@14: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 282: /* $@15: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 283: /* expr_cast: "reinterpret" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 284: /* $@16: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 285: /* $@17: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 286: /* expr_type_decl: "type" '<' $@16 type_declaration '>' $@17  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 287: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 288: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 289: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 290: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 291: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 292: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 293: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 294: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 295: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 296: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 297: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 298: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 299: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 300: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 301: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 302: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 303: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 304: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 305: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 306: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 307: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 308: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 309: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 310: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 311: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 312: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 313: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 314: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 315: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 316: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 317: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 318: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 319: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 320: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 325: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 326: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 335: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 338: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 339: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 340: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 341: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 342: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 343: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 344: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 345: /* $@19: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 346: /* func_addr_expr: '@' '@' '<' $@18 type_declaration_no_options '>' $@19 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 347: /* $@20: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 348: /* $@21: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 349: /* func_addr_expr: '@' '@' '<' $@20 optional_function_argument_list optional_function_type '>' $@21 func_addr_name  */
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

  case 350: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 351: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 352: /* $@22: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 353: /* $@23: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 354: /* expr_field: expr '.' $@22 error $@23  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 355: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 356: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 357: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 358: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 359: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 360: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 361: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 362: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 363: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 364: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 391: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 394: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 395: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 396: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 397: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 398: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 399: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 400: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 401: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 402: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 403: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 404: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 405: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 406: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 407: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 408: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 409: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 410: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 411: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 412: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 413: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 414: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 415: /* $@25: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 416: /* expr: expr "is" "type" '<' $@24 type_declaration_no_options '>' $@25  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 417: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 418: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 419: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 420: /* $@26: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 421: /* $@27: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 422: /* expr: expr "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 423: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 424: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 425: /* $@28: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 426: /* $@29: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 427: /* expr: expr '?' "as" "type" '<' $@28 type_declaration '>' $@29  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 428: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 429: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 430: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 431: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 432: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 433: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 434: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 435: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 436: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 437: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 438: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 439: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 440: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 442: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 443: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 444: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 445: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 446: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 447: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 448: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 449: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 450: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 451: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 452: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 453: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 454: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 455: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 456: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 457: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 458: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 459: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 460: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 461: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 462: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 463: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 464: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 465: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 466: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 467: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 468: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 469: /* $@30: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 470: /* struct_variable_declaration_list: struct_variable_declaration_list $@30 structure_variable_declaration ';'  */
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

  case 471: /* $@31: %empty  */
                                                                                             {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 472: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@31 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
            }
    break;

  case 473: /* $@32: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 474: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@32 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 475: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 476: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 477: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 478: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 479: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 480: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 481: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 482: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 483: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 484: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 485: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 486: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 487: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 488: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 489: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 490: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 491: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 492: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 493: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 494: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 495: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 496: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 497: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 498: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 499: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 500: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 501: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 502: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 503: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 504: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 505: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 506: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 507: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 508: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 509: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 510: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 511: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 512: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 513: /* global_variable_declaration_list: global_variable_declaration_list $@33 optional_field_annotation let_variable_declaration  */
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

  case 514: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 515: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 516: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 517: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 518: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 519: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 520: /* $@34: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 521: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@34 optional_field_annotation let_variable_declaration  */
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

  case 522: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 523: /* enum_list: enum_list "name" ';'  */
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

  case 524: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 525: /* $@35: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 526: /* single_alias: "name" $@35 '=' type_declaration  */
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

  case 531: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 532: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 533: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 534: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 535: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 536: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 537: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 538: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 539: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 540: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 541: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 542: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 543: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 544: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 545: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 546: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 547: /* $@36: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 548: /* $@37: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 549: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@36 structure_name $@37 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
        }
    }
    break;

  case 550: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 551: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 552: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 553: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 554: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 555: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 556: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 557: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 558: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 559: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 560: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 561: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 562: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 563: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 564: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 565: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 566: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 567: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 568: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 569: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 570: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 571: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 572: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 573: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 574: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 575: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 576: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 577: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 578: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 579: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 580: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 581: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 582: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 583: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 584: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 585: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 586: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 587: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 588: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 589: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 590: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 591: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 592: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 593: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 594: /* $@38: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 595: /* $@39: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 596: /* bitfield_type_declaration: "bitfield" '<' $@38 bitfield_bits '>' $@39  */
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

  case 597: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 598: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 599: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 600: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 601: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 602: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 603: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
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

  case 604: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 605: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 606: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 607: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 608: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 609: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 610: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 611: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 612: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 613: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 614: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 615: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 616: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 617: /* $@40: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 618: /* $@41: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 619: /* type_declaration_no_options: "smart_ptr" '<' $@40 type_declaration '>' $@41  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 620: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 621: /* $@42: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 622: /* $@43: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 623: /* type_declaration_no_options: "array" '<' $@42 type_declaration '>' $@43  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 624: /* $@44: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 625: /* $@45: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 626: /* type_declaration_no_options: "table" '<' $@44 table_type_pair '>' $@45  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 627: /* $@46: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 628: /* $@47: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 629: /* type_declaration_no_options: "iterator" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 630: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 631: /* $@48: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 632: /* $@49: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 633: /* type_declaration_no_options: "block" '<' $@48 type_declaration '>' $@49  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 634: /* $@50: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 635: /* $@51: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 636: /* type_declaration_no_options: "block" '<' $@50 optional_function_argument_list optional_function_type '>' $@51  */
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

  case 637: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 638: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 639: /* $@53: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 640: /* type_declaration_no_options: "function" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 641: /* $@54: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 642: /* $@55: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 643: /* type_declaration_no_options: "function" '<' $@54 optional_function_argument_list optional_function_type '>' $@55  */
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

  case 644: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 645: /* $@56: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 646: /* $@57: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 647: /* type_declaration_no_options: "lambda" '<' $@56 type_declaration '>' $@57  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 648: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 649: /* $@59: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 650: /* type_declaration_no_options: "lambda" '<' $@58 optional_function_argument_list optional_function_type '>' $@59  */
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

  case 651: /* $@60: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 652: /* $@61: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 653: /* type_declaration_no_options: "tuple" '<' $@60 tuple_type_list '>' $@61  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 654: /* $@62: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 655: /* $@63: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 656: /* type_declaration_no_options: "variant" '<' $@62 variant_type_list '>' $@63  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 657: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 658: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 659: /* type_declaration: type_declaration '|' '#'  */
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

  case 660: /* $@64: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 661: /* $@65: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 662: /* variant_alias_declaration: "variant" $@64 "name" $@65 '{' variant_type_list ';' '}'  */
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

  case 663: /* $@66: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 664: /* bitfield_alias_declaration: "bitfield" $@66 "name" '{' bitfield_bits ';' '}'  */
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

  case 665: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 666: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 667: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 668: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 669: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 670: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 671: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 672: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 673: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 674: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 675: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 676: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 677: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block ']' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 678: /* make_struct_decl: "[[" type_declaration_no_options optional_block ']' ']'  */
                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 679: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block ']' ']'  */
                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 680: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 681: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block '}' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 682: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block '}' ']'  */
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

  case 683: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 684: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 685: /* make_tuple: make_tuple ',' expr  */
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

  case 686: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 687: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 688: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 689: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 690: /* make_dim_decl: "[[" type_declaration_no_options make_dim ']' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 691: /* make_dim_decl: "[{" type_declaration_no_options make_dim '}' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 692: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 693: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 694: /* make_table_decl: "{{" make_table '}' '}'  */
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

  case 695: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 696: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 697: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 698: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


