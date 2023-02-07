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
  YYSYMBOL_MTAG_DOTDOTDOT = 148,           /* "..."  */
  YYSYMBOL_BRABRAB = 149,                  /* "[["  */
  YYSYMBOL_BRACBRB = 150,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 151,                  /* "{{"  */
  YYSYMBOL_INTEGER = 152,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 153,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 154,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 155,    /* "unsigned long integer constant"  */
  YYSYMBOL_FLOAT = 156,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 157,                   /* "double constant"  */
  YYSYMBOL_NAME = 158,                     /* "name"  */
  YYSYMBOL_KEYWORD = 159,                  /* "keyword"  */
  YYSYMBOL_BEGIN_STRING = 160,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 161,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 162,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 163,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 164,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 165,          /* "}"  */
  YYSYMBOL_END_OF_READ = 166,              /* "end of failed eader macro"  */
  YYSYMBOL_167_ = 167,                     /* ','  */
  YYSYMBOL_168_ = 168,                     /* '='  */
  YYSYMBOL_169_ = 169,                     /* '?'  */
  YYSYMBOL_170_ = 170,                     /* ':'  */
  YYSYMBOL_171_ = 171,                     /* '|'  */
  YYSYMBOL_172_ = 172,                     /* '^'  */
  YYSYMBOL_173_ = 173,                     /* '&'  */
  YYSYMBOL_174_ = 174,                     /* '<'  */
  YYSYMBOL_175_ = 175,                     /* '>'  */
  YYSYMBOL_176_ = 176,                     /* '-'  */
  YYSYMBOL_177_ = 177,                     /* '+'  */
  YYSYMBOL_178_ = 178,                     /* '*'  */
  YYSYMBOL_179_ = 179,                     /* '/'  */
  YYSYMBOL_180_ = 180,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 181,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 182,               /* UNARY_PLUS  */
  YYSYMBOL_183_ = 183,                     /* '~'  */
  YYSYMBOL_184_ = 184,                     /* '!'  */
  YYSYMBOL_PRE_INC = 185,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 186,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 187,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 188,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 189,                    /* DEREF  */
  YYSYMBOL_190_ = 190,                     /* '.'  */
  YYSYMBOL_191_ = 191,                     /* '['  */
  YYSYMBOL_192_ = 192,                     /* ']'  */
  YYSYMBOL_193_ = 193,                     /* '('  */
  YYSYMBOL_194_ = 194,                     /* ')'  */
  YYSYMBOL_195_ = 195,                     /* '$'  */
  YYSYMBOL_196_ = 196,                     /* '@'  */
  YYSYMBOL_197_ = 197,                     /* ';'  */
  YYSYMBOL_198_ = 198,                     /* '{'  */
  YYSYMBOL_199_ = 199,                     /* '}'  */
  YYSYMBOL_200_ = 200,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 201,                 /* $accept  */
  YYSYMBOL_program = 202,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 203, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 204,              /* module_name  */
  YYSYMBOL_module_declaration = 205,       /* module_declaration  */
  YYSYMBOL_character_sequence = 206,       /* character_sequence  */
  YYSYMBOL_string_constant = 207,          /* string_constant  */
  YYSYMBOL_string_builder_body = 208,      /* string_builder_body  */
  YYSYMBOL_string_builder = 209,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 210, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 211,              /* expr_reader  */
  YYSYMBOL_212_1 = 212,                    /* $@1  */
  YYSYMBOL_options_declaration = 213,      /* options_declaration  */
  YYSYMBOL_require_declaration = 214,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 215,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 216,      /* require_module_name  */
  YYSYMBOL_require_module = 217,           /* require_module  */
  YYSYMBOL_is_public_module = 218,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 219,       /* expect_declaration  */
  YYSYMBOL_expect_list = 220,              /* expect_list  */
  YYSYMBOL_expect_error = 221,             /* expect_error  */
  YYSYMBOL_expression_label = 222,         /* expression_label  */
  YYSYMBOL_expression_goto = 223,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 224,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 225,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 226,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 227, /* expression_else_one_liner  */
  YYSYMBOL_228_2 = 228,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 229,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 230,  /* expression_if_then_else  */
  YYSYMBOL_231_3 = 231,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 232,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 233,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 234,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 235,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 236,    /* expression_with_alias  */
  YYSYMBOL_237_4 = 237,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 238, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 239, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 240, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 241,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 242, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 243, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 244, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 245,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 246,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 247, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 248, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 249,   /* optional_function_type  */
  YYSYMBOL_function_name = 250,            /* function_name  */
  YYSYMBOL_global_function_declaration = 251, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 252, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 253, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 254,     /* function_declaration  */
  YYSYMBOL_255_5 = 255,                    /* $@5  */
  YYSYMBOL_expression_block = 256,         /* expression_block  */
  YYSYMBOL_expression_any = 257,           /* expression_any  */
  YYSYMBOL_expressions = 258,              /* expressions  */
  YYSYMBOL_expr_keyword = 259,             /* expr_keyword  */
  YYSYMBOL_expr_pipe = 260,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 261,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 262,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 263,     /* new_type_declaration  */
  YYSYMBOL_264_6 = 264,                    /* $@6  */
  YYSYMBOL_265_7 = 265,                    /* $@7  */
  YYSYMBOL_expr_new = 266,                 /* expr_new  */
  YYSYMBOL_expression_break = 267,         /* expression_break  */
  YYSYMBOL_expression_continue = 268,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 269, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 270,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 271, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 272,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 273,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 274,                  /* kwd_let  */
  YYSYMBOL_expression_let = 275,           /* expression_let  */
  YYSYMBOL_expr_cast = 276,                /* expr_cast  */
  YYSYMBOL_277_8 = 277,                    /* $@8  */
  YYSYMBOL_278_9 = 278,                    /* $@9  */
  YYSYMBOL_279_10 = 279,                   /* $@10  */
  YYSYMBOL_280_11 = 280,                   /* $@11  */
  YYSYMBOL_281_12 = 281,                   /* $@12  */
  YYSYMBOL_282_13 = 282,                   /* $@13  */
  YYSYMBOL_expr_type_decl = 283,           /* expr_type_decl  */
  YYSYMBOL_284_14 = 284,                   /* $@14  */
  YYSYMBOL_285_15 = 285,                   /* $@15  */
  YYSYMBOL_expr_type_info = 286,           /* expr_type_info  */
  YYSYMBOL_expr_list = 287,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 288,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 289,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 290,            /* capture_entry  */
  YYSYMBOL_capture_list = 291,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 292,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 293,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 294,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 295,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 296,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 297,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 298,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 299,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 300,           /* func_addr_expr  */
  YYSYMBOL_301_16 = 301,                   /* $@16  */
  YYSYMBOL_302_17 = 302,                   /* $@17  */
  YYSYMBOL_303_18 = 303,                   /* $@18  */
  YYSYMBOL_304_19 = 304,                   /* $@19  */
  YYSYMBOL_expr_field = 305,               /* expr_field  */
  YYSYMBOL_306_20 = 306,                   /* $@20  */
  YYSYMBOL_307_21 = 307,                   /* $@21  */
  YYSYMBOL_expr = 308,                     /* expr  */
  YYSYMBOL_309_22 = 309,                   /* $@22  */
  YYSYMBOL_310_23 = 310,                   /* $@23  */
  YYSYMBOL_311_24 = 311,                   /* $@24  */
  YYSYMBOL_312_25 = 312,                   /* $@25  */
  YYSYMBOL_313_26 = 313,                   /* $@26  */
  YYSYMBOL_314_27 = 314,                   /* $@27  */
  YYSYMBOL_expr_mtag = 315,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 316, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 317,        /* optional_override  */
  YYSYMBOL_optional_constant = 318,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 319, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 320, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 321, /* struct_variable_declaration_list  */
  YYSYMBOL_322_28 = 322,                   /* $@28  */
  YYSYMBOL_323_29 = 323,                   /* $@29  */
  YYSYMBOL_324_30 = 324,                   /* $@30  */
  YYSYMBOL_function_argument_declaration = 325, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 326,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 327,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 328,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 329,             /* variant_type  */
  YYSYMBOL_variant_type_list = 330,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 331,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 332,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 333,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 334,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 335, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 336, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 337, /* global_variable_declaration_list  */
  YYSYMBOL_338_31 = 338,                   /* $@31  */
  YYSYMBOL_optional_shared = 339,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 340, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 341,               /* global_let  */
  YYSYMBOL_342_32 = 342,                   /* $@32  */
  YYSYMBOL_enum_list = 343,                /* enum_list  */
  YYSYMBOL_single_alias = 344,             /* single_alias  */
  YYSYMBOL_345_33 = 345,                   /* $@33  */
  YYSYMBOL_alias_list = 346,               /* alias_list  */
  YYSYMBOL_alias_declaration = 347,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 348, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 349,                /* enum_name  */
  YYSYMBOL_enum_declaration = 350,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 351, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 352,          /* optional_sealed  */
  YYSYMBOL_structure_name = 353,           /* structure_name  */
  YYSYMBOL_class_or_struct = 354,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 355, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 356,    /* structure_declaration  */
  YYSYMBOL_357_34 = 357,                   /* $@34  */
  YYSYMBOL_358_35 = 358,                   /* $@35  */
  YYSYMBOL_variable_name_with_pos_list = 359, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 360,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 361, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 362, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 363,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 364,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 365, /* bitfield_type_declaration  */
  YYSYMBOL_366_36 = 366,                   /* $@36  */
  YYSYMBOL_367_37 = 367,                   /* $@37  */
  YYSYMBOL_table_type_pair = 368,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 369, /* type_declaration_no_options  */
  YYSYMBOL_370_38 = 370,                   /* $@38  */
  YYSYMBOL_371_39 = 371,                   /* $@39  */
  YYSYMBOL_372_40 = 372,                   /* $@40  */
  YYSYMBOL_373_41 = 373,                   /* $@41  */
  YYSYMBOL_374_42 = 374,                   /* $@42  */
  YYSYMBOL_375_43 = 375,                   /* $@43  */
  YYSYMBOL_376_44 = 376,                   /* $@44  */
  YYSYMBOL_377_45 = 377,                   /* $@45  */
  YYSYMBOL_378_46 = 378,                   /* $@46  */
  YYSYMBOL_379_47 = 379,                   /* $@47  */
  YYSYMBOL_380_48 = 380,                   /* $@48  */
  YYSYMBOL_381_49 = 381,                   /* $@49  */
  YYSYMBOL_382_50 = 382,                   /* $@50  */
  YYSYMBOL_383_51 = 383,                   /* $@51  */
  YYSYMBOL_384_52 = 384,                   /* $@52  */
  YYSYMBOL_385_53 = 385,                   /* $@53  */
  YYSYMBOL_386_54 = 386,                   /* $@54  */
  YYSYMBOL_387_55 = 387,                   /* $@55  */
  YYSYMBOL_388_56 = 388,                   /* $@56  */
  YYSYMBOL_389_57 = 389,                   /* $@57  */
  YYSYMBOL_390_58 = 390,                   /* $@58  */
  YYSYMBOL_391_59 = 391,                   /* $@59  */
  YYSYMBOL_392_60 = 392,                   /* $@60  */
  YYSYMBOL_393_61 = 393,                   /* $@61  */
  YYSYMBOL_type_declaration = 394,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 395, /* variant_alias_declaration  */
  YYSYMBOL_396_62 = 396,                   /* $@62  */
  YYSYMBOL_397_63 = 397,                   /* $@63  */
  YYSYMBOL_bitfield_alias_declaration = 398, /* bitfield_alias_declaration  */
  YYSYMBOL_399_64 = 399,                   /* $@64  */
  YYSYMBOL_make_decl = 400,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 401,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 402,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 403,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 404,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 405,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 406,           /* make_map_tuple  */
  YYSYMBOL_make_dim = 407,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 408,            /* make_dim_decl  */
  YYSYMBOL_make_table = 409,               /* make_table  */
  YYSYMBOL_make_table_decl = 410,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 411, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 412       /* array_comprehension  */
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
#define YYLAST   10998

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  201
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  212
/* YYNRULES -- Number of rules.  */
#define YYNRULES  684
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1204

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   428


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
       2,     2,     2,   184,     2,   200,   195,   180,   173,     2,
     193,   194,   178,   177,   167,   176,   190,   179,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   170,   197,
     174,   168,   175,   169,   196,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   191,     2,   192,   172,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   198,   171,   199,   183,     2,     2,     2,
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
     165,   166,   181,   182,   185,   186,   187,   188,   189
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   483,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   499,   500,   501,   505,   506,
     510,   528,   529,   530,   531,   535,   539,   544,   553,   561,
     577,   582,   590,   590,   620,   641,   645,   646,   650,   653,
     657,   663,   672,   675,   681,   682,   686,   690,   691,   695,
     698,   704,   710,   713,   719,   720,   724,   725,   726,   735,
     736,   740,   741,   741,   747,   748,   749,   750,   751,   755,
     761,   761,   767,   773,   781,   791,   800,   800,   807,   808,
     809,   810,   811,   812,   816,   821,   829,   830,   831,   835,
     836,   837,   838,   839,   840,   841,   842,   848,   851,   857,
     858,   859,   863,   871,   884,   887,   895,   906,   917,   928,
     934,   938,   945,   946,   950,   951,   952,   956,   959,   966,
     970,   971,   972,   973,   974,   975,   976,   977,   978,   979,
     980,   981,   982,   983,   984,   985,   986,   987,   988,   989,
     990,   991,   992,   993,   994,   995,   996,   997,   998,   999,
    1000,  1001,  1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,
    1010,  1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,
    1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,
    1030,  1031,  1032,  1033,  1034,  1035,  1036,  1037,  1038,  1039,
    1040,  1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,  1053,
    1075,  1076,  1077,  1081,  1087,  1087,  1104,  1108,  1119,  1120,
    1121,  1122,  1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,
    1131,  1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,  1143,
    1148,  1154,  1160,  1172,  1206,  1209,  1215,  1216,  1227,  1231,
    1234,  1242,  1242,  1242,  1245,  1251,  1254,  1257,  1261,  1267,
    1271,  1275,  1278,  1281,  1289,  1292,  1295,  1303,  1306,  1314,
    1317,  1320,  1328,  1334,  1335,  1339,  1345,  1345,  1345,  1348,
    1348,  1348,  1353,  1353,  1353,  1361,  1361,  1361,  1367,  1377,
    1388,  1403,  1406,  1412,  1413,  1420,  1431,  1432,  1433,  1437,
    1438,  1439,  1440,  1444,  1449,  1457,  1458,  1462,  1467,  1474,
    1475,  1476,  1477,  1478,  1479,  1483,  1484,  1485,  1486,  1487,
    1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,
    1498,  1499,  1500,  1501,  1505,  1506,  1507,  1508,  1512,  1523,
    1528,  1538,  1542,  1549,  1552,  1552,  1552,  1557,  1557,  1557,
    1570,  1574,  1578,  1578,  1578,  1585,  1586,  1587,  1588,  1589,
    1590,  1591,  1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,
    1600,  1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,  1609,
    1610,  1611,  1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,
    1620,  1626,  1627,  1628,  1629,  1630,  1631,  1632,  1633,  1634,
    1635,  1636,  1637,  1638,  1642,  1646,  1649,  1652,  1653,  1654,
    1655,  1658,  1661,  1662,  1665,  1665,  1665,  1668,  1673,  1677,
    1681,  1681,  1681,  1686,  1689,  1693,  1693,  1693,  1698,  1701,
    1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,  1710,  1711,
    1719,  1720,  1721,  1722,  1723,  1724,  1725,  1729,  1733,  1737,
    1741,  1745,  1749,  1753,  1757,  1761,  1768,  1769,  1773,  1774,
    1775,  1779,  1780,  1784,  1785,  1786,  1790,  1800,  1803,  1803,
    1822,  1821,  1836,  1835,  1848,  1857,  1863,  1868,  1878,  1879,
    1883,  1886,  1895,  1896,  1900,  1909,  1910,  1915,  1916,  1920,
    1926,  1932,  1935,  1939,  1945,  1954,  1955,  1956,  1960,  1961,
    1965,  1972,  1977,  1986,  1992,  2003,  2006,  2011,  2016,  2024,
    2035,  2038,  2038,  2058,  2059,  2063,  2064,  2065,  2069,  2072,
    2072,  2091,  2094,  2103,  2116,  2116,  2137,  2138,  2142,  2143,
    2147,  2148,  2149,  2153,  2163,  2170,  2180,  2181,  2185,  2186,
    2190,  2196,  2197,  2201,  2202,  2203,  2207,  2212,  2207,  2222,
    2229,  2234,  2243,  2249,  2260,  2261,  2262,  2263,  2264,  2265,
    2266,  2267,  2268,  2269,  2270,  2271,  2272,  2273,  2274,  2275,
    2276,  2277,  2278,  2279,  2280,  2281,  2282,  2283,  2284,  2288,
    2289,  2290,  2291,  2292,  2293,  2297,  2308,  2312,  2319,  2331,
    2338,  2347,  2347,  2347,  2360,  2364,  2371,  2372,  2373,  2374,
    2375,  2389,  2395,  2399,  2403,  2408,  2413,  2418,  2423,  2427,
    2431,  2436,  2440,  2445,  2445,  2445,  2451,  2458,  2458,  2458,
    2463,  2463,  2463,  2469,  2469,  2469,  2474,  2478,  2478,  2478,
    2483,  2483,  2483,  2492,  2496,  2496,  2496,  2501,  2501,  2501,
    2510,  2514,  2514,  2514,  2519,  2519,  2519,  2528,  2528,  2528,
    2534,  2534,  2534,  2543,  2546,  2557,  2573,  2573,  2573,  2597,
    2597,  2617,  2618,  2619,  2620,  2624,  2631,  2638,  2644,  2653,
    2658,  2665,  2666,  2670,  2676,  2683,  2691,  2698,  2706,  2718,
    2721,  2727,  2741,  2747,  2753,  2758,  2765,  2770,  2780,  2785,
    2792,  2804,  2805,  2809,  2812
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
  "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"", "\"[{\"",
  "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
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
  "expressions", "expr_keyword", "expr_pipe", "name_in_namespace",
  "expression_delete", "new_type_declaration", "$@6", "$@7", "expr_new",
  "expression_break", "expression_continue", "expression_return_no_pipe",
  "expression_return", "expression_yield_no_pipe", "expression_yield",
  "expression_try_catch", "kwd_let", "expression_let", "expr_cast", "$@8",
  "$@9", "$@10", "$@11", "$@12", "$@13", "expr_type_decl", "$@14", "$@15",
  "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_name", "func_addr_expr", "$@16", "$@17", "$@18", "$@19",
  "expr_field", "$@20", "$@21", "expr", "$@22", "$@23", "$@24", "$@25",
  "$@26", "$@27", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@28", "$@29", "$@30", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@31", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@32", "enum_list",
  "single_alias", "$@33", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_name", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@34", "$@35", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@36", "$@37", "table_type_pair",
  "type_declaration_no_options", "$@38", "$@39", "$@40", "$@41", "$@42",
  "$@43", "$@44", "$@45", "$@46", "$@47", "$@48", "$@49", "$@50", "$@51",
  "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60",
  "$@61", "type_declaration", "variant_alias_declaration", "$@62", "$@63",
  "bitfield_alias_declaration", "$@64", "make_decl", "make_struct_fields",
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
     415,   416,   417,   418,   419,   420,   421,    44,    61,    63,
      58,   124,    94,    38,    60,    62,    45,    43,    42,    47,
      37,   422,   423,   126,    33,   424,   425,   426,   427,   428,
      46,    91,    93,    40,    41,    36,    64,    59,   123,   125,
      35
};
#endif

#define YYPACT_NINF (-1056)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-635)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1056,    52, -1056, -1056,    17,   -83,   219,   -40, -1056,   -10,
   -1056, -1056,    61, -1056, -1056, -1056, -1056, -1056,   267, -1056,
     141, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
     -93, -1056,     2,    51,    74, -1056, -1056, -1056,   219, -1056,
       4, -1056, -1056,   122, -1056, -1056, -1056,   141,   205,   242,
   -1056, -1056,   268,   307,    61,    61,   132, -1056,   467,  -110,
   -1056, -1056, -1056,   374,   406,   407, -1056,   435,    14,    17,
     297,   -83,   260,   330, -1056,   154,   154, -1056,   341,   321,
    -109,   444,   327, -1056, -1056,   370, -1056,   -67,    17,    61,
      61,    61,    61, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
     373, -1056, -1056, -1056, -1056, -1056,   338, -1056, -1056, -1056,
   -1056, -1056,   419,    57, -1056, -1056, -1056, -1056,   492, -1056,
   -1056, 10745, -1056, -1056,   362, -1056, -1056, -1056,   391,   354,
   -1056, -1056,     9, -1056,   221,   429,   467,  1528, -1056,    25,
     468, -1056,   420, -1056, -1056,   458, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056,    22, -1056,   393,   424,   432,   437, -1056,
   -1056, -1056,   397, -1056, -1056, -1056, -1056, -1056,   448, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,   449, -1056,
   -1056, -1056,   450,   453, -1056, -1056, -1056, -1056,   454,   457,
     414, -1056, -1056, -1056, -1056, -1056,  1074,   438, -1056, -1056,
     441,   474, -1056,  9462, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056,   579,   600, -1056,
     442,   443,   364, -1056, -1056,   485, -1056,   447,    17,   262,
   -1056, -1056, -1056,    57, -1056, -1056, -1056, -1056, -1056,   486,
   -1056,     7,   151,   201, -1056, -1056,  5833, -1056, -1056, -1056,
     617, -1056, -1056, -1056,     5,  3429, -1056,  1134,  -104,   482,
   -1056,   459,   496,   497, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056,   465, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,   637, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056,   480, -1056, -1056,    24,   489, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056,   462,   -85,   507,   481, -1056,
     420,   127,   473,   643,   334, -1056, -1056, 10745, 10745, 10745,
   10745,   484,   391, 10745,   442, 10745,   442, 10745,   442, 10840,
     474, -1056, -1056,   315,   494,   517, -1056,   500,   521,   523,
     505,   525,   508, -1056,   526,  5833,  5833,   509,   510,   511,
     512,   513,   514, -1056, 10555, 10650,  5833, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056,  5833,  5833,  5833,   149,  5833,  5833,
    5833, -1056,   515, -1056, -1056, -1056,    70, -1056, -1056, -1056,
   -1056,   518, -1056, -1056, -1056, -1056, -1056, -1056,  6436, -1056,
     519, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,   516,
   -1056, -1056,  8893, -1056,  1074, -1056, -1056, 10745,   -69, -1056,
   -1056, -1056,   552, -1056,   520, -1056,    -1, -1056,   323, 10745,
   -1056,  1632, -1056,   -84, -1056,   149, -1056, -1056,   262,   522,
    5833,   557,   559, 10745, -1056,   -64,   288,   543,   134,   320,
     352, -1056,    62,   355,   489,   383,   489,   408,   489,   -58,
   -1056,   189,   438,   199, -1056,   527, -1056, -1056,   149, -1056,
    5833, -1056, -1056,  5833, -1056,  5833, 10745,   413,   413,  5833,
    5833,  5833,  5833,  5833,  5833,   300,  2005,   300,  2176,  9558,
   -1056,   273,   433,   413,   413,   -56, -1056,   413,   413,  6527,
      88, -1056,  2814,   570,  1271,  6512,  5833,  5833, -1056, -1056,
    5833,  5833,  5833,  5833,   563,  5833,   117,  5833,  5833,  5833,
    5833,  5833,  5833,  5833,  5833,  5833,  3577,  5833,  5833,  5833,
    5833,  5833,  5833,  5833,  5833,  5833,  5833,   -59,  5833, -1056,
    3725, -1056, -1056,   438, -1056, -1056, -1056,  5833,   300,   529,
     682, -1056,   -31, -1056,   -79,   438, -1056,  5833, -1056, -1056,
     300,  2518, -1056,   443,  3896,  5833,   580, -1056,   528,   585,
    4044,   294,  2666,   195,   195,  5833, -1056,   703,   544,   547,
    5833,   739, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056,   553,   558,   562,   564, -1056,   565, -1056, -1056,   262,
   -1056,   -71, -1056,  1358,   -11,  5833, -1056, -1056,    15, -1056,
   -1056,  6618, -1056,   708,   767, -1056, -1056, -1056,  2985, -1056,
   -1056, 10745, -1056, -1056, -1056,   598, -1056,   588, -1056,   589,
   -1056,   590, 10745, -1056, 10840, -1056,   474, 10745,  4192,  4363,
   10745,  6709, 10745, 10745,  6800, 10745,  6891,   269,  6982,  7073,
    7164,  7255,  7346,  7437,    13,   195,   -30,  2347,  4534,  9637,
     593,   -18,   576,   603,   319,    19,  4705,   -18,   342,  5833,
    5833,   573, -1056,  5833,   455,   612, -1056,   581,   582,   282,
   -1056, -1056,   618, -1056,    98,  9920,    -9,   442,   605,   587,
   -1056, -1056,   607,   596, -1056, -1056,   394,   394,   190,   190,
   10421, 10421,   597,    35,   599, -1056,  8984,   -75,   -75,   394,
     394, 10240, 10131, 10210,  9999,  6603,  9739, 10319, 10342,   303,
     190,   190,   170,   170,    35,    35,    35,   175,  5833,   601,
   -1056,   248,  5833,   781,  9075, -1056,    99,  7528, -1056,  5833,
     625, -1056,   633, -1056, 10745, -1056,  2985, -1056,  5926,    20,
    2985, -1056,   671,  9359,   785,  5833,  9920,  5926,   632, -1056,
     631,   650,  9920, -1056,  2985, -1056,  9359,   608, -1056, -1056,
    5926,   609, -1056, -1056,  5926, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056,   195, -1056,  5833,  5833,  5833,  5833,  5833,  5833,
    5833,  5833,  5833,  5833,  3133,  5833,  5833,  5833,  5833,  5833,
    5833,  3281, -1056,  6232,    61, -1056,   796,   420, -1056,   648,
   -1056,  2985, -1056,  6028, -1056, -1056,   438, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056,   438, -1056, -1056, -1056,
     411, -1056,   106,   651,  7619,   416, -1056,   439,   567, -1056,
     578, -1056,   570, -1056, -1056, -1056, -1056, -1056,   621,  5833,
   -1056,  5833,  5833,   -17,  5833,   652,   618,   619,   623,  5833,
     626,  5833,  5833,   618,   622,   630,  9920, -1056, -1056,  9818,
   -1056,   656,  5833,  5833, 10745,   442,    -7,   147,  5833, -1056,
     666,   669,   670,   672, -1056,   163,   489, -1056,  5833, -1056,
    5833,  4876,  5833, -1056,   657,   639, -1056, -1056,  5833,   644,
   -1056,  9166,  5833,   660, -1056,  9257, -1056, -1056, -1056, -1056,
    7710, -1056,   816,   -19, -1056,  9359, -1056,  5833, -1056,  9359,
    5833,  5833,   443,  9920, -1056, -1056, -1056, -1056, -1056,  9359,
   -1056, -1056, -1056,   463,  5833, -1056,  9920,  9920,  9920,  9920,
    9920,  9920,  9920,  9920,  9920,  9920,   195,   195,  9920,  9920,
    9920,  9920,  9920,  9920,  9920,   195,   195,  9920, -1056,   167,
     445,     6,   661, -1056, -1056,  6130, -1056, -1056, -1056, -1056,
   -1056, -1056,   230, -1056, -1056, -1056, -1056, -1056,   674,  5047,
     -34,  9920,  9920,   -18,   676,  9920,    -6,   593,   677, -1056,
    9920, -1056,   603,     0,   -18,   678, -1056, -1056, -1056,  7801,
    7892,   952,   489,   679,  9920, -1056, -1056, -1056, -1056,    -9,
     680,   -91, 10745,  7983, 10745,  8074, -1056,   109,  8165, -1056,
    5833, 10101,  5833, -1056,  8256,  5833, -1056, -1056, -1056,   705,
    5833,    60, -1056,  5833,  1831,   443, -1056, -1056,  5833, -1056,
     555, -1056, -1056, -1056, -1056,   667, -1056, -1056,    48, -1056,
   -1056,   445, -1056, -1056, -1056,  5833,   713, -1056,  5833,  5833,
    5833,  5218, -1056,   110,  5833,   683,   688,  5833,  5833, -1056,
    5833,   675, -1056, -1056, -1056, -1056,   711, -1056, -1056, -1056,
    5389, -1056, -1056,   965, -1056,   417, -1056, -1056, -1056, 10745,
    8347,  8438, -1056,  8529, -1056,  9920,   443,  9920, -1056, -1056,
    5926, -1056,   686, -1056, -1056,   856,   300,  8620,   714,   190,
     190,   190, -1056,  8711, -1056,  6334,   699, -1056,  9920,  9920,
    6334,   700,   129, -1056,  5833,  9999, -1056, -1056,   418, -1056,
   -1056, -1056, -1056,   463,   770, -1056,  1528, -1056, -1056, -1056,
   -1056,  5833, -1056,   847,   712, -1056,   704, -1056, -1056,   129,
   10101, -1056, -1056, -1056, -1056,  5537,  5685, -1056, -1056, -1056,
   -1056, -1056,  9920,   709,  1528,  8802,  5833,   715,   716, -1056,
   -1056,  5833,  9920,  5833,  9920, -1056,   443, -1056,  9920, -1056,
   -1056,  9920,  9920, -1056
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   112,     1,   263,     0,     0,     0,     0,   264,     0,
     649,   646,     0,    14,     3,    10,     9,     8,     0,     7,
     503,     6,    11,     5,     4,    12,    13,    87,    88,    86,
      95,    97,    34,    49,    46,    47,    36,    37,     0,    38,
      44,    35,   514,     0,   519,    19,    18,   503,     0,     0,
     100,   101,     0,   236,     0,     0,   102,   104,   110,     0,
      99,   532,   531,   200,   520,   533,   504,   505,     0,     0,
       0,     0,    39,     0,    45,     0,     0,    42,     0,     0,
       0,    15,     0,   647,   238,     0,   105,     0,     0,     0,
       0,     0,     0,   113,   202,   201,   204,   199,   522,   521,
       0,   535,   534,   536,   507,   506,   509,    93,    94,    91,
      92,    90,     0,     0,    89,    98,    50,    48,    44,    41,
      40,     0,   516,   518,     0,    16,    17,    20,     0,     0,
     237,   109,     0,   106,   107,   108,   111,     0,   523,     0,
     528,   500,   446,    21,    22,     0,    82,    83,    80,    81,
      79,    78,    84,     0,    43,     0,     0,     0,     0,   544,
     564,   545,   576,   546,   550,   551,   552,   553,   568,   557,
     558,   559,   560,   561,   562,   563,   565,   566,   616,   549,
     556,   567,   623,   630,   547,   554,   548,   555,     0,     0,
       0,   575,   586,   589,   587,   588,   643,   515,   517,   579,
       0,     0,   103,     0,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,     0,   119,
     114,     0,     0,   511,   529,     0,   537,   501,     0,     0,
      23,    24,    25,     0,    96,   607,   610,   613,   603,     0,
     581,   617,   624,   631,   637,   640,     0,   594,   599,   593,
       0,   606,   602,   596,     0,     0,   598,     0,     0,     0,
     475,     0,   170,   171,   168,   122,   123,   125,   124,   126,
     127,   128,   129,   155,   156,   153,   154,   146,   157,   158,
     147,   144,   145,   169,   166,     0,   167,   159,   160,   161,
     162,   133,   134,   135,   130,   131,   132,   143,     0,   149,
     150,   148,   141,   142,   137,   136,   138,   139,   140,   121,
     120,   165,     0,   151,   152,   446,   117,   229,   205,   569,
     572,   570,   573,   571,   574,     0,     0,   526,     0,   508,
     446,     0,     0,   490,   488,   510,    85,     0,     0,     0,
       0,     0,     0,     0,   114,     0,   114,     0,   114,     0,
       0,   351,   352,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,   568,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   435,     0,     0,     0,   299,   301,   300,
     302,   303,   304,    26,     0,     0,     0,     0,     0,     0,
       0,   286,   287,   349,   348,   297,   346,   422,   421,   420,
     419,   112,   425,   347,   424,   423,   392,   353,     0,   354,
       0,   350,   651,   652,   653,   654,   601,   595,   597,     0,
     600,   591,     0,   645,   644,   580,   650,     0,     0,   172,
     173,   164,   174,   163,     0,   115,     0,   468,     0,     0,
     203,     0,   511,     0,   524,     0,   530,   457,     0,     0,
       0,     0,     0,     0,   489,     0,     0,     0,   584,     0,
       0,   577,     0,     0,   117,     0,   117,     0,   117,   236,
     472,     0,   470,     0,   241,   245,   244,   248,     0,   275,
       0,   266,   269,     0,   272,     0,     0,   381,   382,     0,
       0,     0,     0,     0,     0,     0,   661,     0,     0,   673,
     678,     0,     0,   358,   357,   397,    32,   356,   355,     0,
     288,   428,     0,   295,     0,     0,     0,     0,   383,   384,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   342,     0,   578,
       0,   592,   590,   474,   648,   476,   175,     0,     0,     0,
     539,   465,   479,   116,   446,   118,   231,     0,    59,    60,
       0,   251,   249,     0,     0,     0,     0,   250,     0,     0,
       0,     0,     0,     0,     0,     0,   208,   206,     0,     0,
       0,     0,   224,   219,   216,   215,   217,   218,   230,   210,
     209,     0,    67,    68,    65,   222,    66,   223,   225,     0,
     214,     0,   211,   305,     0,     0,   512,   527,   458,   502,
     447,     0,   492,   493,     0,   486,   487,   485,     0,   608,
     611,     0,   614,   604,   582,     0,   618,     0,   625,     0,
     632,     0,     0,   638,     0,   641,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   236,     0,     0,   669,
     659,   661,     0,   674,     0,     0,     0,   661,     0,     0,
       0,     0,    29,     0,    27,     0,   385,     0,     0,   334,
     331,   333,     0,   393,     0,   281,     0,   114,     0,     0,
     408,   407,     0,     0,   409,   413,   359,   360,   372,   373,
     370,   371,     0,   402,     0,   390,     0,   426,   427,   361,
     362,   377,   378,   379,   380,     0,     0,   375,   376,   374,
     368,   369,   364,   363,   365,   366,   367,     0,     0,     0,
     340,     0,     0,     0,     0,   395,     0,     0,   466,     0,
       0,   478,     0,   477,     0,   480,     0,   469,     0,     0,
       0,   255,     0,   252,     0,     0,   239,     0,     0,   228,
       0,     0,    53,    73,     0,   260,   257,   287,   234,   235,
       0,     0,   226,   227,     0,    70,   213,   220,   221,   254,
     259,   265,     0,   212,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,     0,     0,   538,     0,   446,   491,     0,
     495,     0,   499,   305,   609,   612,   585,   615,   605,   583,
     619,   621,   626,   628,   633,   635,   471,   639,   473,   642,
       0,   246,     0,     0,     0,     0,   398,     0,     0,   399,
       0,   429,   295,   430,   431,   432,   433,   434,     0,     0,
     662,     0,     0,   661,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   672,   679,   680,     0,
      30,    33,     0,     0,     0,   114,     0,     0,     0,   394,
       0,     0,     0,     0,   293,     0,   117,   404,     0,   410,
       0,     0,     0,   388,     0,     0,   414,   418,     0,     0,
     391,     0,     0,     0,   341,     0,   343,   386,   396,   467,
       0,   541,   542,   481,   484,   483,    74,     0,   256,   253,
       0,     0,     0,   240,    75,    76,    51,    52,   261,   258,
     288,   232,   229,    56,     0,   233,   315,   316,   318,   317,
     319,   309,   310,   311,   320,   321,     0,     0,   307,   308,
     322,   323,   312,   313,   314,     0,     0,   306,   513,     0,
     453,   448,     0,   494,   497,   305,   498,   622,   629,   636,
     242,   247,     0,   278,   276,   267,   270,   273,     0,     0,
       0,   656,   655,   661,     0,   670,     0,   660,     0,   664,
     671,   676,   675,     0,   661,     0,   677,    28,    31,     0,
       0,     0,   117,     0,   282,   291,   292,   290,   289,     0,
       0,     0,     0,     0,     0,     0,   329,     0,     0,   415,
       0,   403,     0,   389,     0,     0,   387,   344,   540,     0,
       0,     0,   262,     0,     0,     0,    54,    55,     0,    69,
      61,   326,   327,   324,   325,   113,   454,   455,   448,   449,
     450,   453,   459,   496,   243,     0,     0,   277,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,   663,
       0,     0,   667,   332,   445,   335,     0,   328,   294,   296,
       0,   283,   298,     0,   444,     0,   442,   330,   439,     0,
       0,     0,   438,     0,   543,   482,     0,    77,   207,    57,
       0,    62,     0,   464,   460,   451,     0,     0,     0,   268,
     271,   274,   400,     0,   437,   681,     0,   665,   658,   657,
     681,     0,     0,   338,     0,   284,   405,   411,     0,   443,
     441,   440,    72,    56,     0,    71,     0,   452,   462,   456,
     279,     0,   401,     0,     0,   666,     0,   668,   336,     0,
     285,   406,   412,   416,    58,   251,     0,    63,    67,    68,
      65,    66,    64,     0,     0,     0,     0,     0,     0,   339,
     417,     0,   252,     0,   257,   461,     0,   280,   682,   683,
     684,   253,   258,   463
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1056, -1056, -1056, -1056, -1056,   421,   837, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056,   502,   871, -1056,   811, -1056, -1056,
     860, -1056, -1056, -1056,  -221, -1056, -1056, -1056,  -220, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056,   695, -1056, -1056,
     870,   -57, -1056, -1056,   193,   118,  -384,  -345,  -466, -1056,
   -1056, -1056, -1055, -1056, -1056,  -226, -1056,    -8, -1056,  -562,
     -12, -1056, -1056, -1056, -1056, -1056,  -213,  -209,  -205, -1056,
    -203, -1056, -1056,   954, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056,  -553, -1056, -1056,   -73,
   -1056,    95,  -578, -1056,  -449, -1056, -1056, -1056,  -971, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056,   501, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056,  -141,  -108, -1056,  -112, -1056,
   -1056, -1056, -1056, -1056,   388, -1056,   317, -1056,  -417,   613,
    -558,  -565,   335, -1056, -1056,  -435, -1056, -1056,   925, -1056,
   -1056, -1056,   524,    85, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,  -459,  -117,
   -1056,   611, -1056,   627, -1056, -1056, -1056, -1056,  -261, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056, -1056,
   -1056, -1056, -1056,   -97, -1056, -1056, -1056, -1056, -1056,   614,
    -684,  -491,  -661, -1056,    94,   290,   470, -1056, -1056, -1056,
    -159, -1056
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   127,    47,    14,   145,   151,   512,   403,   891,
     404,   695,    15,    16,    39,    40,    41,    77,    17,    34,
      35,   598,   599,  1058,  1059,   600,  1122,  1154,   601,   602,
     954,   603,   604,   605,   606,   607,  1053,   152,   153,    30,
      31,    32,    56,    57,    58,    59,    18,   326,   450,   230,
      19,    96,   231,    97,   137,   405,   608,   451,   609,   610,
     406,   611,   485,   657,  1074,   407,   612,   613,   614,   615,
     616,   617,   618,   619,   620,   408,   662,  1078,   663,  1079,
     665,  1080,   409,   660,  1077,   410,   704,  1102,   411,   904,
     905,   707,   412,   413,   772,   622,   414,   415,   701,   416,
     894,  1142,   895,  1169,   417,   753,  1047,   705,  1032,  1171,
    1034,  1172,  1109,  1190,   419,   446,  1071,  1158,  1068,   982,
     628,   827,  1156,  1184,   447,   448,   480,   481,   270,   271,
     872,   571,   638,   465,   344,   345,   237,   340,    67,   106,
      21,   142,   336,    44,    78,    80,    22,   100,   139,    23,
     456,   235,   236,    65,   103,    24,   140,   338,   572,   420,
     335,   193,   194,   200,   195,   352,   839,   467,   196,   350,
     838,   347,   834,   348,   835,   349,   837,   353,   840,   354,
     987,   355,   842,   356,   988,   357,   844,   358,   989,   359,
     847,   360,   849,   482,    25,    49,   129,    26,    48,   421,
     680,   681,   682,   422,   683,   510,   684,   423,   511,   424,
    1164,   425
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,   239,   621,   758,   192,   328,   434,   756,   647,   474,
     649,   476,   651,   478,   766,   788,   789,   687,   897,   771,
     877,   565,  -112,   629,   197,    73,   884,   523,   107,   108,
     785,   132,   869,   675,   675,    27,    28,   427,   882,   937,
    1100,   534,    60,    60,   536,   537,   674,   568,   685,    42,
    1069,   802,     2,   635,   435,   524,   525,    92,     3,    74,
     747,   748,   636,   536,   537,   444,    89,    90,    91,    33,
     238,   146,   147,   453,    85,    68,   832,    60,    60,    60,
      60,     4,    93,     5,   625,     6,   761,   761,   749,   269,
     123,     7,  1069,  1124,    50,   436,   871,   870,   761,   750,
       8,  1183,    85,  1070,   637,   852,     9,   327,   900,   191,
     761,   761,   652,   626,   454,   557,   558,   901,    42,   871,
    1087,   769,    51,   506,   508,    10,   803,   131,    79,  1196,
     564,   751,   752,   898,   557,   558,   762,   763,   763,   764,
     569,   896,   765,   528,   529,  1070,    11,   453,    45,   763,
     192,   534,   267,   535,   536,   537,   538,   570,    43,   902,
     539,   763,   763,  1084,   903,   124,   109,   898,   444,    69,
     110,  1168,   111,   238,   112,    29,    69,  -620,   428,   876,
     762,   341,  -620,    75,   801,    46,   762,   762,   822,   243,
     524,   525,  1007,    52,    76,   232,   429,  1090,  1189,   458,
    -620,    66,   634,   202,   934,   430,   824,   113,   938,   148,
     524,   525,  1004,   149,   825,   150,   244,   112,   445,    53,
      52,    70,   948,   233,   955,   557,   558,   898,   521,   697,
     192,   192,   192,   192,   698,   667,   192,   644,   192,   565,
     192,    71,   192,    12,   826,    54,    53,    86,    87,    13,
     466,   468,   469,   470,    55,   191,   473,  1116,   475,   645,
     477,    52,   699,   522,   724,   898,   898,   192,   192,   984,
     697,    61,    62,   898,    63,   725,   898,   898,   528,   529,
      42,    52,   133,   134,   135,   136,   534,    53,   535,   536,
     537,   538,   899,   928,    69,   539,   526,   527,   528,   529,
     991,   257,    64,  1107,  1134,   267,   534,    53,   535,   536,
     537,   538,    36,    37,   875,   539,  1000,   540,   541,   459,
     192,  -627,   919,   524,   525,    88,  -627,   258,   259,  1013,
    1029,   641,   192,   920,    92,   191,   191,   191,   191,  1023,
     563,   191,  1085,   191,  -627,   191,   192,   191,   554,   555,
     556,   191,   575,  1091,    89,  1030,    91,   774,  1037,  1065,
     557,   558,   906,    82,   653,   783,   552,   553,   554,   555,
     556,  -634,   191,   191,   655,  1050,  -634,    36,    37,   192,
     557,   558,  1003,   260,  1051,   516,   654,   261,  1061,  1062,
     401,   787,  1014,   327,  -634,   923,   656,  1063,  1064,    38,
      83,   857,   858,   342,   860,  1075,   924,   711,   715,   526,
     527,   528,   529,   530,   524,   525,   531,   532,   533,   534,
     343,   535,   536,   537,   538,   191,    84,  1076,   539,    94,
     540,   541,   329,   524,   525,    95,   330,   191,   262,    85,
    1031,   569,   263,   627,   862,   264,  1083,    52,  1088,   116,
      76,   191,  -337,   331,   332,   333,   334,  -337,   570,   267,
     265,    98,   101,   639,   384,   385,   386,    99,   102,   266,
     690,   257,   691,    53,  1055,  -337,   659,   550,   551,   552,
     553,   554,   555,   556,   191,  1056,  1057,   495,   118,   484,
     104,   267,   327,   557,   558,   642,   105,   258,   259,   125,
    1066,   462,   528,   529,   463,   126,  1067,   464,   700,   121,
     534,   880,   535,   536,   537,   538,   881,   573,   122,   539,
     574,   528,   529,   267,   192,   128,   267,   643,   130,   534,
     646,   138,   536,   537,   538,   192,   141,   192,   539,   881,
     192,   885,   936,   192,   836,   192,   192,    74,   192,   199,
    1022,   944,   201,   260,   267,   846,  1096,   261,   648,   198,
     850,  1159,    89,   855,   951,   234,  1121,   245,   953,   238,
     552,   553,   554,   555,   556,   524,   525,   119,   120,   267,
     143,   144,   267,   650,   557,   558,   990,   267,   267,   267,
     249,   994,  1147,  1173,   143,   144,   692,   693,   246,   257,
      89,    90,    91,   557,   558,   621,   247,   256,   262,   267,
     257,   248,   263,   323,   995,   264,   240,   241,   917,   240,
     241,   242,   250,   251,   252,   258,   259,   253,   254,   191,
     265,   255,   269,  1021,   324,   325,   258,   259,   268,   266,
     191,   327,   191,   337,   351,   191,   339,   192,   191,   426,
     191,   191,   437,   191,   439,   440,   438,   441,   442,   449,
     452,   526,   527,   528,   529,   530,   460,   933,   531,   532,
     533,   534,   443,   535,   536,   537,   538,   455,   471,   457,
     539,   260,   540,   541,   461,   261,   981,   488,   542,   543,
     544,   489,   260,   490,   545,   491,   261,   492,   493,   494,
     496,   495,   499,   500,   501,   502,   503,   504,   561,    12,
     566,   520,   560,   567,   630,   632,  1052,   633,   640,   706,
     658,   722,   759,   760,   546,   779,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   262,   780,   778,   791,
     263,   792,   996,   264,   793,   557,   558,   262,   795,   829,
     796,   263,   191,   997,   264,   797,   435,   418,   265,   798,
     875,   799,   800,   841,   843,   845,   432,   266,   878,   265,
     879,  1103,   888,   890,   892,   893,   896,   192,   266,   907,
     908,   909,   926,   931,   361,   362,   363,   364,   365,   910,
     911,   932,   912,   802,   922,  1175,   366,   582,   942,   257,
     945,   946,   947,   980,   950,  1101,   983,   952,   367,   992,
    1006,  1008,    60,   368,   999,  1009,   369,  1018,  1011,   370,
     587,  1015,  1016,   371,  1025,   258,   259,  1026,  1027,  1119,
    1028,  1039,  1040,   372,   159,   160,   161,  1042,   163,   164,
     165,   166,   167,   373,   169,   170,   171,   172,   173,   174,
     175,   176,   177,  1045,   179,   180,   181,  1049,  1072,   184,
     185,   186,   187,  1114,  1123,   374,  1176,  1081,  1086,  1089,
    1092,  1128,  1099,  1097,  1141,  1136,   497,   498,   375,   376,
    1137,   260,   191,  1155,   635,   261,  1143,   509,  1157,  1161,
    1152,  1165,  1167,   636,  1153,   513,   514,   515,  1186,   517,
     518,   519,    52,  1188,  1187,   114,  1195,  1199,  1200,    72,
     377,   378,   379,   380,   381,   192,   382,   192,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,    53,   154,
     393,   117,  1174,   694,  1177,   637,   262,  1105,   346,   115,
     263,  1178,   979,   264,  1054,  1179,   394,   395,   396,  1180,
     397,  1181,   623,   398,   399,    20,  1098,   998,   265,  1126,
    1125,   631,   767,   400,   830,   401,   402,   266,   327,   831,
    1203,   848,    81,   483,   486,  1012,   624,   487,   688,   472,
     887,  1166,     0,     0,   257,     0,     0,     0,     0,     0,
       0,   661,   192,     0,   664,     0,   666,   257,     0,     0,
     668,   669,   670,   671,   672,   673,     0,   679,     0,   679,
     258,   259,  1148,     0,     0,     0,     0,     0,     0,     0,
     191,     0,   191,   258,   259,     0,     0,   716,   717,     0,
       0,   718,   719,   720,   721,     0,   723,     0,   726,   727,
     728,   729,   730,   731,   732,   733,   734,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,     0,   754,
       0,     0,     0,     0,     0,     0,   260,     0,   757,     0,
     261,     0,     0,     0,     0,     0,     0,     0,   768,   260,
       0,     0,   773,   261,     0,   776,   777,     0,     0,     0,
       0,   782,     0,   786,     0,     0,   790,   191,     0,     0,
       0,   794,     0,     0,     0,     0,   257,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,   263,   823,  1095,   264,     0,
     700,     0,   258,   259,   262,     0,     0,     0,   263,   833,
    1146,   264,     0,   265,     0,     0,     0,     0,     0,     0,
       0,     0,   266,     0,     0,     0,   265,   700,   155,     0,
     854,     0,     0,     0,   156,   266,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   513,   519,
       0,   157,     0,     0,     0,     0,     0,   519,   260,     0,
     886,   509,   261,     0,   889,     0,   158,     0,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     0,     0,   263,     0,   921,
     264,     0,     0,   925,     0,     0,     0,     0,     0,     0,
     930,     0,     0,     0,     0,   265,    52,   935,     0,     0,
       0,   939,     0,     0,   266,     0,   943,     0,     0,   190,
       0,     0,     0,     0,     0,   949,     0,     0,     0,   708,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   956,   957,   958,   959,   960,
     961,   962,   963,   964,   965,   968,   969,   970,   971,   972,
     973,   974,   977,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   985,     0,   433,   159,   160,   161,     0,   163,
     164,   165,   166,   167,   373,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,   179,   180,   181,     0,     0,
     184,   185,   186,   187,     0,     0,     0,   -64,     0,     0,
       0,     0,  1001,  1002,     0,  1005,     0,     0,   524,   525,
    1010,     0,   679,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1019,  1020,     0,     0,     0,     0,  1024,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1033,
       0,  1035,     0,  1038,     0,     0,     0,     0,   709,  1041,
       0,     0,     0,  1044,     0,     0,     0,     0,     0,   710,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   968,   977,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1060,   804,   805,   806,   807,
     808,   809,   810,   811,   526,   527,   528,   529,   530,   812,
     813,   531,   532,   533,   534,   814,   535,   536,   537,   538,
       0,     0,     0,   539,   815,   540,   541,   816,   817,     0,
       0,   542,   543,   544,   818,   819,   820,   545,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   821,   546,     0,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,     0,
       0,  1110,     0,  1111,     0,     0,  1113,     0,   557,   558,
       0,  1115,     0,     0,  1117,   623,     0,     0,     0,  1120,
       0,     0,   203,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1127,     0,     0,  1129,
    1130,  1131,  1133,     0,     0,  1135,     0,     0,  1138,  1139,
       0,  1140,   204,     0,   205,     0,   206,   207,   208,   209,
     210,  1145,   211,   212,   213,   214,   215,   216,   217,   218,
     219,     0,   220,   221,   222,     0,     0,   223,   224,   225,
     226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   576,     0,     0,   227,   228,     3,     0,
     577,   578,   579,     0,   580,  1170,   361,   362,   363,   364,
     365,     0,     0,     0,     0,  1182,     0,   581,   366,   582,
     583,     0,  1185,     0,     0,     0,     0,     0,     0,   584,
     367,     0,   585,     0,   586,   368,  1192,  1194,   369,     0,
       8,   370,   587,     0,   588,   371,   229,  1198,   589,   590,
       0,     0,  1201,     0,  1202,   591,   159,   160,   161,     0,
     163,   164,   165,   166,   167,   373,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,   179,   180,   181,     0,
       0,   184,   185,   186,   187,     0,     0,   374,   592,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   593,   594,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   379,   380,   381,     0,   382,     0,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
      53,   595,   393,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,   395,
     396,     0,   397,     0,     0,   398,   399,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,   401,   402,   596,
     327,   597,   576,     0,     0,     0,     0,     3,     0,   577,
     578,   579,     0,   580,     0,   361,   362,   363,   364,   365,
       0,     0,     0,     0,     0,     0,   581,   366,   582,   583,
       0,     0,     0,     0,     0,     0,     0,     0,   584,   367,
       0,   585,     0,   586,   368,     0,     0,   369,     0,     8,
     370,   587,     0,   588,   371,     0,     0,   589,   590,     0,
       0,     0,     0,     0,   591,   159,   160,   161,     0,   163,
     164,   165,   166,   167,   373,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,   179,   180,   181,     0,     0,
     184,   185,   186,   187,     0,     0,   374,   592,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
     376,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   593,   594,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   377,   378,   379,   380,   381,     0,   382,     0,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,    53,
     595,   393,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   394,   395,   396,
       0,   397,     0,     0,   398,   399,     0,     0,     0,   361,
     362,   363,   364,   365,   400,     0,   401,   402,   596,   327,
    1118,   366,     0,     0,     0,     0,     0,   257,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,   368,     0,
       0,   369,     0,     0,   370,     0,   675,     0,   371,     0,
       0,     0,     0,   258,   259,     0,     0,     0,   372,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,   260,
       0,     0,     0,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   676,     0,   393,     0,     0,     0,     0,
       0,     0,     0,     0,   262,     0,     0,     0,   263,     0,
       0,   677,   395,   396,     0,   397,     0,     0,   398,   399,
     361,   362,   363,   364,   365,     0,   265,     0,   678,     0,
     401,   402,   366,   327,     0,   266,     0,     0,   257,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,   368,
       0,     0,   369,     0,     0,   370,     0,     0,     0,   371,
       0,     0,     0,     0,   258,   259,     0,     0,     0,   372,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   376,     0,     0,     0,     0,
     260,     0,     0,     0,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   380,
     381,     0,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,   676,     0,   393,     0,     0,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,   263,
       0,     0,   677,   395,   396,     0,   397,     0,     0,   398,
     399,   361,   362,   363,   364,   365,     0,   265,     0,   686,
       0,   401,   402,   366,   327,     0,   266,     0,     0,   427,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
     368,     0,     0,   369,     0,     0,   370,     0,     0,     0,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,     0,   393,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,     0,     0,   394,   395,   396,     0,   397,     0,     0,
     398,   399,   361,   362,   363,   364,   365,     0,   429,     0,
     400,     0,   401,   402,   366,   327,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,   368,     0,     0,   369,     0,     0,   370,     0,     0,
       0,   371,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   372,   159,   160,   161,     0,   163,   164,   165,   166,
     167,   373,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,   179,   180,   181,     0,     0,   184,   185,   186,
     187,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,   376,     0,     0,
       0,     0,     0,     0,     0,   770,     0,     0,     0,     0,
       0,   593,   594,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   377,   378,
     379,   380,   381,     0,   382,     0,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,    53,     0,   393,     0,
     361,   362,   363,   364,   365,     0,     0,     0,     0,     0,
       0,     0,   366,     0,   394,   395,   396,     0,   397,     0,
       0,   398,   399,     0,   367,     0,     0,     0,     0,   368,
       0,   400,   369,   401,   402,   370,   327,     0,     0,   371,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   372,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   376,     0,     0,     0,     0,
       0,     0,     0,   784,     0,     0,     0,     0,     0,   593,
     594,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   380,
     381,     0,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,    53,     0,   393,     0,   361,   362,
     363,   364,   365,     0,     0,     0,     0,     0,     0,     0,
     366,     0,   394,   395,   396,     0,   397,     0,     0,   398,
     399,     0,   367,     0,     0,     0,     0,   368,     0,   400,
     369,   401,   402,   370,   327,     0,     0,   371,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   372,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   373,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,   179,   180,
     181,     0,     0,   184,   185,   186,   187,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,   376,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   377,   378,   379,   380,   381,     0,
     382,     0,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,    53,     0,   393,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     394,   395,   396,     0,   397,     0,     0,   398,   399,   361,
     362,   363,   364,   365,     0,   702,     0,   400,   703,   401,
     402,   366,   327,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,   368,     0,
       0,   369,     0,     0,   370,     0,     0,     0,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   593,   594,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    53,     0,   393,     0,   361,   362,   363,
     364,   365,     0,     0,     0,     0,     0,     0,     0,   366,
       0,   394,   395,   396,     0,   397,     0,     0,   398,   399,
       0,   367,     0,     0,     0,     0,   368,     0,   400,   369,
     401,   402,   370,   327,     0,     0,   371,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   372,   159,   160,   161,
       0,   163,   164,   165,   166,   167,   373,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,   179,   180,   181,
       0,     0,   184,   185,   186,   187,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   966,   967,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   377,   378,   379,   380,   381,     0,   382,
       0,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,    53,     0,   393,     0,   361,   362,   363,   364,   365,
       0,     0,     0,     0,     0,     0,     0,   366,     0,   394,
     395,   396,     0,   397,     0,     0,   398,   399,     0,   367,
       0,     0,     0,     0,   368,     0,   400,   369,   401,   402,
     370,   327,     0,     0,   371,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   372,   159,   160,   161,     0,   163,
     164,   165,   166,   167,   373,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,   179,   180,   181,     0,     0,
     184,   185,   186,   187,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
     376,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   975,   976,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   377,   378,   379,   380,   381,     0,   382,     0,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,    53,
       0,   393,     0,   361,   362,   363,   364,   365,     0,     0,
       0,     0,     0,     0,     0,   366,     0,   394,   395,   396,
       0,   397,     0,     0,   398,   399,     0,   367,     0,     0,
       0,     0,   368,     0,   400,   369,   401,   402,   370,   327,
       0,     0,   371,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   372,   159,   160,   161,     0,   163,   164,   165,
     166,   167,   373,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,   179,   180,   181,     0,     0,   184,   185,
     186,   187,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,   380,   381,     0,   382,     0,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,    53,     0,   393,
       0,   361,   362,   363,   364,   365,     0,     0,   735,     0,
       0,     0,     0,   366,     0,   394,   395,   396,     0,   397,
       0,     0,   398,   399,     0,   367,     0,     0,     0,     0,
     368,   431,   400,   369,   401,   402,   370,   327,     0,     0,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,     0,   393,     0,   361,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,   366,     0,   394,   395,   396,     0,   397,     0,     0,
     398,   399,     0,   367,     0,     0,     0,     0,   368,     0,
     400,   369,   401,   402,   370,   327,     0,     0,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    53,     0,   393,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   394,   395,   396,     0,   397,     0,     0,   398,   399,
     361,   362,   363,   364,   365,     0,     0,     0,   400,   755,
     401,   402,   366,   327,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,   368,
       0,     0,   369,     0,     0,   370,     0,     0,     0,   371,
       0,     0,     0,     0,     0,   775,     0,     0,     0,   372,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   380,
     381,     0,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,    53,     0,   393,     0,   361,   362,
     363,   364,   365,     0,     0,     0,     0,     0,     0,     0,
     366,     0,   394,   395,   396,     0,   397,     0,     0,   398,
     399,     0,   367,     0,     0,     0,     0,   368,     0,   400,
     369,   401,   402,   370,   327,     0,     0,   371,     0,     0,
     781,     0,     0,     0,     0,     0,     0,   372,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   373,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,   179,   180,
     181,     0,     0,   184,   185,   186,   187,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,   376,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   377,   378,   379,   380,   381,     0,
     382,     0,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,    53,     0,   393,     0,   361,   362,   363,   364,
     365,     0,     0,     0,     0,     0,     0,     0,   366,     0,
     394,   395,   396,     0,   397,     0,     0,   398,   399,     0,
     367,     0,     0,     0,     0,   368,     0,   400,   369,   401,
     402,   370,   327,     0,     0,   371,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   372,   159,   160,   161,     0,
     163,   164,   165,   166,   167,   373,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,   179,   180,   181,     0,
       0,   184,   185,   186,   187,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   379,   380,   381,     0,   382,     0,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
      53,     0,   393,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,   395,
     396,     0,   397,     0,     0,   398,   399,   361,   362,   363,
     364,   365,     0,     0,     0,   400,   851,   401,   402,   366,
     327,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,     0,     0,   368,     0,     0,   369,
       0,     0,   370,     0,     0,     0,   371,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   372,   159,   160,   161,
       0,   163,   164,   165,   166,   167,   373,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,   179,   180,   181,
       0,     0,   184,   185,   186,   187,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   377,   378,   379,   380,   381,     0,   382,
       0,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,    53,     0,   393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   853,     0,   394,
     395,   396,     0,   397,     0,     0,   398,   399,   361,   362,
     363,   364,   365,     0,     0,     0,   400,     0,   401,   402,
     366,   327,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,   368,     0,     0,
     369,     0,     0,   370,     0,     0,     0,   371,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   372,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   373,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,   179,   180,
     181,     0,     0,   184,   185,   186,   187,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,   376,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   377,   378,   379,   380,   381,     0,
     382,     0,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,    53,     0,   393,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     394,   395,   396,     0,   397,     0,     0,   398,   399,   361,
     362,   363,   364,   365,     0,     0,     0,   400,   873,   401,
     402,   366,   327,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,   368,     0,
       0,   369,     0,     0,   370,     0,     0,     0,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    53,     0,   393,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   394,   395,   396,     0,   397,     0,     0,   398,   399,
     361,   362,   363,   364,   365,     0,     0,     0,   400,   883,
     401,   402,   366,   327,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,   368,
       0,     0,   369,     0,     0,   370,     0,     0,     0,   371,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   372,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   380,
     381,     0,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,    53,     0,   393,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   394,   395,   396,     0,   397,     0,     0,   398,
     399,   361,   362,   363,   364,   365,     0,     0,     0,   400,
    1036,   401,   402,   366,   327,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
     368,     0,     0,   369,     0,     0,   370,     0,     0,     0,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,     0,   393,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   394,   395,   396,     0,   397,     0,     0,
     398,   399,   361,   362,   363,   364,   365,     0,     0,     0,
     400,  1082,   401,   402,   366,   327,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,   368,     0,     0,   369,     0,     0,   370,     0,     0,
       0,   371,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   372,   159,   160,   161,     0,   163,   164,   165,   166,
     167,   373,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,   179,   180,   181,     0,     0,   184,   185,   186,
     187,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   377,   378,
     379,   380,   381,     0,   382,     0,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,    53,     0,   393,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   394,   395,   396,     0,   397,     0,
       0,   398,   399,   361,   362,   363,   364,   365,     0,     0,
       0,   400,  1132,   401,   402,   366,   327,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,     0,     0,
       0,     0,   368,     0,     0,   369,     0,     0,   370,     0,
       0,     0,   371,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   372,   159,   160,   161,     0,   163,   164,   165,
     166,   167,   373,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,   179,   180,   181,     0,     0,   184,   185,
     186,   187,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   376,     0,
       0,     0,     0,     0,     0,     0,  1144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,   380,   381,     0,   382,     0,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,    53,     0,   393,
       0,   361,   362,   363,   364,   365,     0,     0,     0,     0,
       0,     0,     0,   366,     0,   394,   395,   396,     0,   397,
       0,     0,   398,   399,     0,   367,     0,     0,     0,     0,
     368,     0,   400,   369,   401,   402,   370,   327,     0,     0,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,  1191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,     0,   393,     0,   361,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,   366,     0,   394,   395,   396,     0,   397,     0,     0,
     398,   399,     0,   367,     0,     0,     0,     0,   368,     0,
     400,   369,   401,   402,   370,   327,     0,     0,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,     0,
       0,     0,  1193,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    53,     0,   393,     0,   361,   362,   363,
     364,   365,     0,     0,     0,     0,     0,     0,     0,   366,
       0,   394,   395,   396,     0,   397,     0,     0,   398,   399,
       0,   367,     0,     0,     0,     0,   368,     0,   400,   369,
     401,   402,   370,   327,     0,     0,   371,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   372,   159,   160,   161,
       0,   163,   164,   165,   166,   167,   373,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,   179,   180,   181,
       0,     0,   184,   185,   186,   187,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   376,     0,     0,     0,   524,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   377,   378,   379,   380,   381,     0,   382,
       0,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,    53,     0,   393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
     395,   396,     0,   397,     0,     0,   398,   399,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,   401,   402,
       0,   327,   526,   527,   528,   529,   530,     0,     0,   531,
     532,   533,   534,     0,   535,   536,   537,   538,   524,   525,
       0,   539,     0,   540,   541,     0,     0,     0,     0,   542,
     543,   544,     0,     0,     0,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   557,   558,     0,     0,
       0,     0,     0,     0,   327,     0,   804,   805,   806,   807,
     808,   809,   810,   811,   526,   527,   528,   529,   530,   812,
     813,   531,   532,   533,   534,   940,   535,   536,   537,   538,
     524,   525,     0,   539,   815,   540,   541,   816,   817,     0,
       0,   542,   543,   544,   818,   819,   820,   545,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   941,   546,     0,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   557,   558,
       0,     0,     0,     0,     0,   986,     0,     0,   804,   805,
     806,   807,   808,   809,   810,   811,   526,   527,   528,   529,
     530,   812,   813,   531,   532,   533,   534,   940,   535,   536,
     537,   538,   524,   525,     0,   539,   815,   540,   541,   816,
     817,     0,     0,   542,   543,   544,   818,   819,   820,   545,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   941,   546,
       0,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     557,   558,     0,     0,     0,     0,     0,  1073,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   526,   527,
     528,   529,   530,     0,     0,   531,   532,   533,   534,     0,
     535,   536,   537,   538,   524,   525,     0,   539,     0,   540,
     541,     0,     0,     0,     0,   542,   543,   544,     0,     0,
       0,   545,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   546,     0,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   557,   558,     0,     0,     0,     0,     0,   978,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     526,   527,   528,   529,   530,     0,     0,   531,   532,   533,
     534,     0,   535,   536,   537,   538,   524,   525,     0,   539,
       0,   540,   541,     0,     0,     0,     0,   542,   543,   544,
       0,     0,     0,   545,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   546,     0,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   557,   558,     0,     0,     0,     0,
     712,  1163,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   526,   527,   528,   529,   530,   524,   525,   531,
     532,   533,   534,     0,   535,   536,   537,   538,     0,     0,
       0,   539,     0,   540,   541,     0,     0,     0,     0,   542,
     543,   544,     0,     0,     0,   545,   159,   160,   161,     0,
     163,   164,   165,   166,   167,   373,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,   179,   180,   181,     0,
       0,   184,   185,   186,   187,   546,     0,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,     0,     0,     0,
       0,   914,     0,     0,     0,     0,   557,   558,     0,     0,
     559,     0,     0,   526,   527,   528,   529,   530,   524,   525,
     531,   532,   533,   534,     0,   535,   536,   537,   538,     0,
       0,     0,   539,     0,   540,   541,     0,     0,     0,   713,
     542,   543,   544,     0,     0,     0,   545,   159,   160,   161,
     714,   163,   164,   165,   166,   167,   373,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,   179,   180,   181,
       0,     0,   184,   185,   186,   187,   546,     0,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   557,   558,     0,
       0,   696,     0,     0,   526,   527,   528,   529,   530,   524,
     525,   531,   532,   533,   534,     0,   535,   536,   537,   538,
       0,     0,     0,   539,     0,   540,   541,     0,     0,     0,
     915,   542,   543,   544,     0,     0,     0,   545,     0,     0,
       0,   916,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   546,     0,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   557,   558,
       0,     0,   828,     0,     0,   526,   527,   528,   529,   530,
     524,   525,   531,   532,   533,   534,     0,   535,   536,   537,
     538,     0,     0,     0,   539,     0,   540,   541,     0,     0,
       0,     0,   542,   543,   544,     0,     0,     0,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   546,     0,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   557,
     558,     0,     0,   856,     0,     0,   526,   527,   528,   529,
     530,   524,   525,   531,   532,   533,   534,     0,   535,   536,
     537,   538,     0,     0,     0,   539,     0,   540,   541,     0,
       0,     0,     0,   542,   543,   544,     0,     0,     0,   545,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     557,   558,     0,     0,   859,     0,     0,   526,   527,   528,
     529,   530,   524,   525,   531,   532,   533,   534,     0,   535,
     536,   537,   538,     0,     0,     0,   539,     0,   540,   541,
       0,     0,     0,     0,   542,   543,   544,     0,     0,     0,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     546,     0,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   557,   558,     0,     0,   861,     0,     0,   526,   527,
     528,   529,   530,   524,   525,   531,   532,   533,   534,     0,
     535,   536,   537,   538,     0,     0,     0,   539,     0,   540,
     541,     0,     0,     0,     0,   542,   543,   544,     0,     0,
       0,   545,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   546,     0,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   557,   558,     0,     0,   863,     0,     0,   526,
     527,   528,   529,   530,   524,   525,   531,   532,   533,   534,
       0,   535,   536,   537,   538,     0,     0,     0,   539,     0,
     540,   541,     0,     0,     0,     0,   542,   543,   544,     0,
       0,     0,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   546,     0,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   557,   558,     0,     0,   864,     0,     0,
     526,   527,   528,   529,   530,   524,   525,   531,   532,   533,
     534,     0,   535,   536,   537,   538,     0,     0,     0,   539,
       0,   540,   541,     0,     0,     0,     0,   542,   543,   544,
       0,     0,     0,   545,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   546,     0,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   557,   558,     0,     0,   865,     0,
       0,   526,   527,   528,   529,   530,   524,   525,   531,   532,
     533,   534,     0,   535,   536,   537,   538,     0,     0,     0,
     539,     0,   540,   541,     0,     0,     0,     0,   542,   543,
     544,     0,     0,     0,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   546,     0,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   557,   558,     0,     0,   866,
       0,     0,   526,   527,   528,   529,   530,   524,   525,   531,
     532,   533,   534,     0,   535,   536,   537,   538,     0,     0,
       0,   539,     0,   540,   541,     0,     0,     0,     0,   542,
     543,   544,     0,     0,     0,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   557,   558,     0,     0,
     867,     0,     0,   526,   527,   528,   529,   530,   524,   525,
     531,   532,   533,   534,     0,   535,   536,   537,   538,     0,
       0,     0,   539,     0,   540,   541,     0,     0,     0,     0,
     542,   543,   544,     0,     0,     0,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   546,     0,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   557,   558,     0,
       0,   868,     0,     0,   526,   527,   528,   529,   530,   524,
     525,   531,   532,   533,   534,     0,   535,   536,   537,   538,
       0,     0,     0,   539,     0,   540,   541,     0,     0,     0,
       0,   542,   543,   544,     0,     0,     0,   545,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   546,     0,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   557,   558,
       0,     0,   929,     0,     0,   526,   527,   528,   529,   530,
     524,   525,   531,   532,   533,   534,     0,   535,   536,   537,
     538,     0,     0,     0,   539,     0,   540,   541,     0,     0,
       0,     0,   542,   543,   544,     0,     0,     0,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   546,     0,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   557,
     558,     0,     0,   993,     0,     0,   526,   527,   528,   529,
     530,   524,   525,   531,   532,   533,   534,     0,   535,   536,
     537,   538,     0,     0,     0,   539,     0,   540,   541,     0,
       0,     0,     0,   542,   543,   544,     0,     0,     0,   545,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     557,   558,     0,     0,  1048,     0,     0,   526,   527,   528,
     529,   530,   524,   525,   531,   532,   533,   534,     0,   535,
     536,   537,   538,     0,     0,     0,   539,     0,   540,   541,
       0,     0,     0,     0,   542,   543,   544,     0,     0,     0,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     546,     0,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   557,   558,     0,     0,  1093,     0,     0,   526,   527,
     528,   529,   530,   524,   525,   531,   532,   533,   534,     0,
     535,   536,   537,   538,     0,     0,     0,   539,     0,   540,
     541,     0,     0,     0,     0,   542,   543,   544,     0,     0,
       0,   545,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   546,     0,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   557,   558,     0,     0,  1094,     0,     0,   526,
     527,   528,   529,   530,   524,   525,   531,   532,   533,   534,
       0,   535,   536,   537,   538,     0,     0,     0,   539,     0,
     540,   541,     0,     0,     0,     0,   542,   543,   544,     0,
       0,     0,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   546,     0,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   557,   558,     0,     0,  1104,     0,     0,
     526,   527,   528,   529,   530,   524,   525,   531,   532,   533,
     534,     0,   535,   536,   537,   538,     0,     0,     0,   539,
       0,   540,   541,     0,     0,     0,     0,   542,   543,   544,
       0,     0,     0,   545,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   546,     0,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   557,   558,     0,     0,  1106,     0,
       0,   526,   527,   528,   529,   530,   524,   525,   531,   532,
     533,   534,     0,   535,   536,   537,   538,     0,     0,     0,
     539,     0,   540,   541,     0,     0,     0,     0,   542,   543,
     544,     0,     0,     0,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   546,     0,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   557,   558,     0,     0,  1108,
       0,     0,   526,   527,   528,   529,   530,   524,   525,   531,
     532,   533,   534,     0,   535,   536,   537,   538,     0,     0,
       0,   539,     0,   540,   541,     0,     0,     0,     0,   542,
     543,   544,     0,     0,     0,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   557,   558,     0,     0,
    1112,     0,     0,   526,   527,   528,   529,   530,   524,   525,
     531,   532,   533,   534,     0,   535,   536,   537,   538,     0,
       0,     0,   539,     0,   540,   541,     0,     0,     0,     0,
     542,   543,   544,     0,     0,     0,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   546,     0,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   557,   558,     0,
       0,  1149,     0,     0,   526,   527,   528,   529,   530,   524,
     525,   531,   532,   533,   534,     0,   535,   536,   537,   538,
       0,     0,     0,   539,     0,   540,   541,     0,     0,     0,
       0,   542,   543,   544,     0,     0,     0,   545,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   546,     0,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   557,   558,
       0,     0,  1150,     0,     0,   526,   527,   528,   529,   530,
     524,   525,   531,   532,   533,   534,     0,   535,   536,   537,
     538,     0,     0,     0,   539,     0,   540,   541,     0,     0,
       0,     0,   542,   543,   544,     0,     0,     0,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   546,     0,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   557,
     558,     0,     0,  1151,     0,     0,   526,   527,   528,   529,
     530,   524,   525,   531,   532,   533,   534,     0,   535,   536,
     537,   538,     0,     0,     0,   539,     0,   540,   541,     0,
       0,     0,     0,   542,   543,   544,     0,     0,     0,   545,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     557,   558,     0,     0,  1160,     0,     0,   526,   527,   528,
     529,   530,   524,   525,   531,   532,   533,   534,     0,   535,
     536,   537,   538,     0,     0,     0,   539,     0,   540,   541,
       0,     0,     0,     0,   542,   543,   544,     0,     0,     0,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     546,     0,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   557,   558,     0,     0,  1162,     0,     0,   526,   527,
     528,   529,   530,   524,   525,   531,   532,   533,   534,     0,
     535,   536,   537,   538,     0,     0,     0,   539,     0,   540,
     541,     0,     0,     0,     0,   542,   543,   544,     0,     0,
       0,   545,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   546,     0,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   557,   558,     0,     0,  1197,     0,     0,   526,
     527,   528,   529,   530,   524,   525,   531,   532,   533,   534,
       0,   535,   536,   537,   538,     0,     0,     0,   539,     0,
     540,   541,     0,     0,     0,     0,   542,   543,   544,     0,
       0,     0,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   546,     0,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   557,   558,   562,     0,     0,     0,     0,
     526,   527,   528,   529,   530,   524,   525,   531,   532,   533,
     534,     0,   535,   536,   537,   538,     0,     0,     0,   539,
       0,   540,   541,     0,     0,     0,     0,   542,   543,   544,
       0,     0,     0,   545,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   546,     0,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   557,   558,   913,     0,     0,     0,
       0,   526,   527,   528,   529,   530,   524,   525,   531,   532,
     533,   534,     0,   535,   536,   537,   538,     0,     0,     0,
     539,     0,   540,   541,     0,     0,     0,     0,   542,   543,
     544,     0,     0,     0,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   546,     0,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   557,   558,   927,     0,     0,
       0,     0,   526,   527,   528,   529,   530,   524,   525,   531,
     532,   533,   534,     0,   535,   536,   537,   538,     0,     0,
       0,   539,     0,   540,   541,     0,     0,     0,     0,   542,
     543,   544,     0,     0,     0,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   557,   558,  1043,     0,
       0,     0,     0,   526,   527,   528,   529,   530,     0,     0,
     531,   532,   533,   534,     0,   535,   536,   537,   538,   524,
     525,     0,   539,     0,   540,   541,     0,     0,     0,     0,
     542,   543,   544,     0,     0,     0,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   546,     0,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   557,   558,  1046,
       0,     0,     0,     0,     0,     0,     0,   804,   805,   806,
     807,   808,   809,   810,   811,   526,   527,   528,   529,   530,
     812,   813,   531,   532,   533,   534,   940,   535,   536,   537,
     538,  -305,   272,   273,   539,   815,   540,   541,   816,   817,
       0,     0,   542,   543,   544,   818,   819,   820,   545,   274,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   941,   546,     0,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   557,
     558,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   524,   525,
     293,   294,   295,     0,     0,     0,     0,     0,   296,   297,
     298,   299,   300,     0,     0,   301,   302,   303,   304,   305,
     306,   307,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,     0,     0,   319,   320,     0,     0,     0,
       0,     0,   321,   322,     0,     0,     0,   524,   525,     0,
       0,     0,     0,     0,   526,   527,   528,   529,   530,     0,
       0,   531,   532,   533,   534,     0,   535,   536,   537,   538,
       0,     0,     0,   539,     0,   540,   541,     0,     0,   689,
       0,   542,   543,   544,     0,     0,     0,   545,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   546,     0,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,     0,
       0,     0,     0,   526,   527,   528,   529,   530,   557,   558,
     531,   532,   533,   534,     0,   535,   536,   537,   538,   524,
     525,     0,   539,     0,   540,   541,     0,     0,   874,     0,
     542,   543,   544,     0,     0,     0,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   546,     0,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   557,   558,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   524,   525,
       0,     0,     0,     0,     0,   526,   527,   528,   529,   530,
       0,     0,   531,   532,   533,   534,     0,   535,   536,   537,
     538,     0,     0,     0,   539,     0,   540,   541,     0,     0,
       0,     0,   542,   543,   544,     0,     0,     0,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   546,   918,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
       0,     0,     0,     0,   526,   527,   528,   529,   530,   557,
     558,   531,   532,   533,   534,     0,   535,   536,   537,   538,
     524,   525,     0,   539,     0,   540,   541,     0,     0,     0,
       0,   542,   543,   544,     0,     0,     0,   545,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1017,     0,     0,     0,   546,     0,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   557,   558,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   524,
     525,     0,     0,     0,     0,     0,   526,   527,   528,   529,
     530,     0,     0,   531,   532,   533,   534,     0,   535,   536,
     537,   538,     0,     0,     0,   539,     0,   540,   541,     0,
       0,     0,     0,   542,   543,   544,     0,     0,     0,   545,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,     0,     0,     0,     0,   526,   527,   528,   529,   530,
     557,   558,   531,   532,   533,   534,     0,   535,   536,   537,
     538,   524,   525,     0,   539,     0,   540,   541,     0,     0,
       0,     0,   542,   543,   544,     0,     0,     0,  -635,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   524,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   546,     0,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   557,
     558,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   526,   527,   528,
     529,   530,     0,     0,   531,   532,   533,   534,     0,   535,
     536,   537,   538,     0,     0,     0,   539,     0,   540,   541,
     524,   525,     0,     0,   542,   543,   544,   526,   527,   528,
     529,   530,     0,     0,   531,   532,   533,   534,     0,   535,
     536,   537,   538,     0,     0,     0,   539,     0,   540,   541,
     524,   525,     0,     0,   542,     0,   544,     0,     0,     0,
     546,     0,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   557,   558,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,     0,     0,     0,     0,   526,   527,   528,   529,
     530,   557,   558,   531,   532,   533,   534,     0,   535,   536,
     537,   538,     0,     0,     0,   539,     0,   540,   541,   524,
     525,     0,     0,   542,     0,     0,   526,   527,   528,   529,
     530,     0,     0,   531,   532,   533,   534,     0,   535,   536,
     537,   538,   524,   525,     0,   539,     0,   540,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     557,   558,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,     0,     0,     0,     0,   526,   527,   528,   529,   530,
     557,   558,   531,   532,   533,   534,     0,   535,   536,   537,
     538,   524,   525,     0,   539,     0,   540,   541,   526,   527,
     528,   529,   530,     0,     0,   531,   532,   533,   534,     0,
     535,   536,   537,   538,     0,     0,     0,   539,     0,   540,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   548,   549,   550,   551,   552,   553,   554,   555,   556,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   557,
     558,     0,     0,     0,     0,   549,   550,   551,   552,   553,
     554,   555,   556,     0,     0,     0,     0,   526,   527,   528,
     529,   530,   557,   558,   531,     0,     0,   534,     0,   535,
     536,   537,   538,     0,     0,     0,   539,     0,   540,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   505,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   155,
       0,     0,     0,     0,     0,   156,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,   551,   552,   553,   554,
     555,   556,   157,     0,     0,     0,     0,     0,     0,     0,
       0,   557,   558,     0,     0,     0,     0,   158,     0,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   507,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,     0,     0,     0,     0,     0,
     156,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   157,     0,     0,
     190,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   158,    53,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   155,
       0,     0,     0,     0,     0,   156,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   157,     0,     0,   190,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   158,    53,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,     0,     0,     0,     0,     0,
     156,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   157,     0,     0,
     190,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   158,    53,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   190,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   479
};

static const yytype_int16 yycheck[] =
{
      12,   142,   451,   568,   121,   231,   267,   560,   474,   354,
     476,   356,   478,   358,   572,   593,   594,   508,   702,   581,
     681,   438,     7,   458,   121,    21,   687,   411,    14,    15,
     592,    88,    19,    51,    51,    18,    19,    32,    19,    19,
     131,   116,    54,    55,   119,   120,   505,    48,   507,   158,
      44,   122,     0,   117,   158,    20,    21,   167,     6,    55,
     119,   120,   126,   119,   120,   144,   133,   134,   135,   152,
     149,    14,    15,   158,   132,   168,   638,    89,    90,    91,
      92,    29,   192,    31,   168,    33,   117,   117,   147,   158,
     199,    39,    44,    45,    33,   199,   126,   675,   117,   158,
      48,  1156,   132,    97,   168,   658,    54,   198,   117,   121,
     117,   117,   170,   197,   199,   190,   191,   126,   158,   126,
     126,   580,    61,   384,   385,    73,   197,   194,    43,  1184,
     199,   190,   191,   167,   190,   191,   167,   168,   168,   170,
     141,   158,   173,   108,   109,    97,    94,   158,   158,   168,
     267,   116,   171,   118,   119,   120,   121,   158,   198,   168,
     125,   168,   168,   197,   173,    80,   152,   167,   144,   167,
     156,  1142,   158,   149,   160,   158,   167,   170,   173,   197,
     167,   238,   175,   179,   619,   195,   167,   167,   199,   167,
      20,    21,   876,   132,   190,   170,   191,   197,  1169,   340,
     193,    60,   463,   194,   766,   200,   191,   193,   770,   152,
      20,    21,   873,   156,   199,   158,   194,   160,   194,   158,
     132,   170,   784,   198,   802,   190,   191,   167,   158,   141,
     347,   348,   349,   350,   146,   496,   353,   175,   355,   656,
     357,   167,   359,   191,   628,   184,   158,    54,    55,   197,
     347,   348,   349,   350,   193,   267,   353,   197,   355,   197,
     357,   132,   174,   193,   147,   167,   167,   384,   385,   831,
     141,     4,     5,   167,     7,   158,   167,   167,   108,   109,
     158,   132,    89,    90,    91,    92,   116,   158,   118,   119,
     120,   121,   194,   194,   167,   125,   106,   107,   108,   109,
     194,    32,    35,   194,   194,   171,   116,   158,   118,   119,
     120,   121,   158,   159,   167,   125,   869,   127,   128,   192,
     437,   170,   147,    20,    21,   193,   175,    58,    59,   882,
     167,   197,   449,   158,   167,   347,   348,   349,   350,   192,
     437,   353,  1003,   355,   193,   357,   463,   359,   178,   179,
     180,   363,   449,  1014,   133,   192,   135,   583,   911,   192,
     190,   191,   707,   158,   175,   591,   176,   177,   178,   179,
     180,   170,   384,   385,   175,   933,   175,   158,   159,   496,
     190,   191,   873,   114,   937,   397,   197,   118,   966,   967,
     195,   196,   883,   198,   193,   147,   197,   975,   976,   180,
     158,   662,   663,   141,   665,   175,   158,   524,   525,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
     158,   118,   119,   120,   121,   437,   158,   197,   125,    55,
     127,   128,    68,    20,    21,    61,    72,   449,   169,   132,
     906,   141,   173,   455,   175,   176,   999,   132,  1006,   152,
     190,   463,   170,    89,    90,    91,    92,   175,   158,   171,
     191,    55,    55,   175,   149,   150,   151,    61,    61,   200,
     197,    32,   199,   158,    11,   193,   488,   174,   175,   176,
     177,   178,   179,   180,   496,    22,    23,   193,   158,   174,
      55,   171,   198,   190,   191,   175,    61,    58,    59,    55,
      55,   167,   108,   109,   170,    61,    61,   173,   520,   168,
     116,   192,   118,   119,   120,   121,   197,   194,   197,   125,
     197,   108,   109,   171,   641,   198,   171,   175,   158,   116,
     175,   158,   119,   120,   121,   652,   198,   654,   125,   197,
     657,   199,   768,   660,   641,   662,   663,    55,   665,   158,
     895,   777,   198,   114,   171,   652,  1022,   118,   175,   197,
     657,  1126,   133,   660,   790,    97,    11,   174,   794,   149,
     176,   177,   178,   179,   180,    20,    21,    75,    76,   171,
     161,   162,   171,   175,   190,   191,   175,   171,   171,   171,
     193,   175,   175,   175,   161,   162,   163,   164,   174,    32,
     133,   134,   135,   190,   191,  1054,   174,   193,   169,   171,
      32,   174,   173,    34,   175,   176,   161,   162,   735,   161,
     162,   163,   174,   174,   174,    58,    59,   174,   174,   641,
     191,   174,   158,   894,    34,   193,    58,    59,   197,   200,
     652,   198,   654,   158,   158,   657,   199,   764,   660,    32,
     662,   663,   170,   665,   158,   158,   197,   192,    21,   170,
     198,   106,   107,   108,   109,   110,   193,   764,   113,   114,
     115,   116,   192,   118,   119,   120,   121,   170,   194,   198,
     125,   114,   127,   128,    41,   118,   827,   193,   133,   134,
     135,   174,   114,   193,   139,   174,   118,   174,   193,   174,
     174,   193,   193,   193,   193,   193,   193,   193,   192,   191,
     158,   196,   193,   193,   192,   158,   942,   158,   175,   149,
     193,   158,   193,    41,   169,   197,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   169,   152,   158,    36,
     173,   197,   175,   176,   197,   190,   191,   169,     9,    41,
     197,   173,   764,   175,   176,   197,   158,   256,   191,   197,
     167,   197,   197,   175,   175,   175,   265,   200,   192,   191,
     167,  1032,   199,   161,   193,   193,   158,   894,   200,   174,
     193,   174,     1,   158,    14,    15,    16,    17,    18,   193,
     193,   158,   193,   122,   193,    25,    26,    27,    13,    32,
     168,   170,   152,     7,   196,  1031,   158,   198,    38,   158,
     158,   192,   824,    43,   193,   192,    46,   161,   192,    49,
      50,   199,   192,    53,   158,    58,    59,   158,   158,  1055,
     158,   174,   193,    63,    64,    65,    66,   193,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,   193,    84,    85,    86,    41,   197,    89,
      90,    91,    92,   158,   197,    95,    96,   193,   192,   192,
     192,   158,   192,   194,   199,   192,   375,   376,   108,   109,
     192,   114,   894,   197,   117,   118,   175,   386,    32,   175,
    1116,   192,   192,   126,  1120,   394,   395,   396,    51,   398,
     399,   400,   132,   199,   192,    68,   197,   192,   192,    38,
     140,   141,   142,   143,   144,  1032,   146,  1034,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   118,
     160,    71,  1153,   512,  1154,   168,   169,  1034,   243,    69,
     173,  1154,   824,   176,   952,  1154,   176,   177,   178,  1154,
     180,  1154,   451,   183,   184,     1,  1029,   862,   191,  1071,
    1068,   460,   574,   193,   197,   195,   196,   200,   198,   634,
    1196,   654,    47,   360,   363,   881,   452,   363,   508,   352,
     690,  1140,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   490,  1109,    -1,   493,    -1,   495,    32,    -1,    -1,
     499,   500,   501,   502,   503,   504,    -1,   506,    -1,   508,
      58,    59,  1109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1032,    -1,  1034,    58,    59,    -1,    -1,   526,   527,    -1,
      -1,   530,   531,   532,   533,    -1,   535,    -1,   537,   538,
     539,   540,   541,   542,   543,   544,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,    -1,   558,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,   567,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   577,   114,
      -1,    -1,   581,   118,    -1,   584,   585,    -1,    -1,    -1,
      -1,   590,    -1,   592,    -1,    -1,   595,  1109,    -1,    -1,
      -1,   600,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    -1,   173,   625,   175,   176,    -1,
    1142,    -1,    58,    59,   169,    -1,    -1,    -1,   173,   638,
     175,   176,    -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,    -1,    -1,    -1,   191,  1169,    24,    -1,
     659,    -1,    -1,    -1,    30,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   677,   678,
      -1,    47,    -1,    -1,    -1,    -1,    -1,   686,   114,    -1,
     689,   690,   118,    -1,   693,    -1,    62,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,   173,    -1,   748,
     176,    -1,    -1,   752,    -1,    -1,    -1,    -1,    -1,    -1,
     759,    -1,    -1,    -1,    -1,   191,   132,   766,    -1,    -1,
      -1,   770,    -1,    -1,   200,    -1,   775,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,   784,    -1,    -1,    -1,    18,
      -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   804,   805,   806,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,   817,   818,
     819,   820,   821,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   831,    -1,   200,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    -1,     9,    -1,    -1,
      -1,    -1,   871,   872,    -1,   874,    -1,    -1,    20,    21,
     879,    -1,   881,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   892,   893,    -1,    -1,    -1,    -1,   898,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   908,
      -1,   910,    -1,   912,    -1,    -1,    -1,    -1,   147,   918,
      -1,    -1,    -1,   922,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   940,   941,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   954,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168,   169,    -1,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,    -1,
      -1,  1040,    -1,  1042,    -1,    -1,  1045,    -1,   190,   191,
      -1,  1050,    -1,    -1,  1053,  1054,    -1,    -1,    -1,  1058,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1075,    -1,    -1,  1078,
    1079,  1080,  1081,    -1,    -1,  1084,    -1,    -1,  1087,  1088,
      -1,  1090,    64,    -1,    66,    -1,    68,    69,    70,    71,
      72,  1100,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,    -1,   108,   109,     6,    -1,
       8,     9,    10,    -1,    12,  1144,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,  1154,    -1,    25,    26,    27,
      28,    -1,  1161,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    -1,    42,    43,  1175,  1176,    46,    -1,
      48,    49,    50,    -1,    52,    53,   158,  1186,    56,    57,
      -1,    -1,  1191,    -1,  1193,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
     178,    -1,   180,    -1,    -1,   183,   184,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    -1,    42,    43,    -1,    -1,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,   178,
      -1,   180,    -1,    -1,   183,   184,    -1,    -1,    -1,    14,
      15,    16,    17,    18,   193,    -1,   195,   196,   197,   198,
     199,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    51,    -1,    53,    -1,
      -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    -1,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,   173,    -1,
      -1,   176,   177,   178,    -1,   180,    -1,    -1,   183,   184,
      14,    15,    16,    17,    18,    -1,   191,    -1,   193,    -1,
     195,   196,    26,   198,    -1,   200,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    -1,   160,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,   173,
      -1,    -1,   176,   177,   178,    -1,   180,    -1,    -1,   183,
     184,    14,    15,    16,    17,    18,    -1,   191,    -1,   193,
      -1,   195,   196,    26,   198,    -1,   200,    -1,    -1,    32,
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
     153,   154,   155,   156,   157,   158,    -1,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,   176,   177,   178,    -1,   180,    -1,    -1,
     183,   184,    14,    15,    16,    17,    18,    -1,   191,    -1,
     193,    -1,   195,   196,    26,   198,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,   146,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,   160,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   176,   177,   178,    -1,   180,    -1,
      -1,   183,   184,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   193,    46,   195,   196,    49,   198,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    -1,   160,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   176,   177,   178,    -1,   180,    -1,    -1,   183,
     184,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   193,
      46,   195,   196,    49,   198,    -1,    -1,    53,    -1,    -1,
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
     156,   157,   158,    -1,   160,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,   178,    -1,   180,    -1,    -1,   183,   184,    14,
      15,    16,    17,    18,    -1,   191,    -1,   193,   194,   195,
     196,    26,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
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
     155,   156,   157,   158,    -1,   160,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   176,   177,   178,    -1,   180,    -1,    -1,   183,   184,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   193,    46,
     195,   196,    49,   198,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,   160,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   176,
     177,   178,    -1,   180,    -1,    -1,   183,   184,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   193,    46,   195,   196,
      49,   198,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,   160,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   176,   177,   178,
      -1,   180,    -1,    -1,   183,   184,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   193,    46,   195,   196,    49,   198,
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
     151,   152,   153,   154,   155,   156,   157,   158,    -1,   160,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    26,    -1,   176,   177,   178,    -1,   180,
      -1,    -1,   183,   184,    -1,    38,    -1,    -1,    -1,    -1,
      43,   192,   193,    46,   195,   196,    49,   198,    -1,    -1,
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
     153,   154,   155,   156,   157,   158,    -1,   160,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   176,   177,   178,    -1,   180,    -1,    -1,
     183,   184,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     193,    46,   195,   196,    49,   198,    -1,    -1,    53,    -1,
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
     155,   156,   157,   158,    -1,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,   178,    -1,   180,    -1,    -1,   183,   184,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   193,   194,
     195,   196,    26,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,   146,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    -1,   160,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   176,   177,   178,    -1,   180,    -1,    -1,   183,
     184,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   193,
      46,   195,   196,    49,   198,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
     146,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,   160,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     176,   177,   178,    -1,   180,    -1,    -1,   183,   184,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   193,    46,   195,
     196,    49,   198,    -1,    -1,    53,    -1,    -1,    -1,    -1,
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
     158,    -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
     178,    -1,   180,    -1,    -1,   183,   184,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   193,   194,   195,   196,    26,
     198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     157,   158,    -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,
     177,   178,    -1,   180,    -1,    -1,   183,   184,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   193,    -1,   195,   196,
      26,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     156,   157,   158,    -1,   160,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   177,   178,    -1,   180,    -1,    -1,   183,   184,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   193,   194,   195,
     196,    26,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     155,   156,   157,   158,    -1,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   177,   178,    -1,   180,    -1,    -1,   183,   184,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   193,   194,
     195,   196,    26,   198,    -1,    -1,    -1,    -1,    -1,    -1,
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
     154,   155,   156,   157,   158,    -1,   160,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,   178,    -1,   180,    -1,    -1,   183,
     184,    14,    15,    16,    17,    18,    -1,    -1,    -1,   193,
     194,   195,   196,    26,   198,    -1,    -1,    -1,    -1,    -1,
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
     153,   154,   155,   156,   157,   158,    -1,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,   178,    -1,   180,    -1,    -1,
     183,   184,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     193,   194,   195,   196,    26,   198,    -1,    -1,    -1,    -1,
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
     152,   153,   154,   155,   156,   157,   158,    -1,   160,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,   177,   178,    -1,   180,    -1,
      -1,   183,   184,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   193,   194,   195,   196,    26,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,   160,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   176,   177,   178,    -1,   180,
      -1,    -1,   183,   184,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   193,    46,   195,   196,    49,   198,    -1,    -1,
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
     153,   154,   155,   156,   157,   158,    -1,   160,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   176,   177,   178,    -1,   180,    -1,    -1,
     183,   184,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     193,    46,   195,   196,    49,   198,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    -1,   160,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   176,   177,   178,    -1,   180,    -1,    -1,   183,   184,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   193,    46,
     195,   196,    49,   198,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,   178,    -1,   180,    -1,    -1,   183,   184,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
      -1,   198,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    20,    21,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      20,    21,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168,   169,    -1,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    20,    21,    -1,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,   169,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    20,    21,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,   191,    -1,    -1,    -1,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    -1,    -1,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    20,    21,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,   191,    -1,    -1,    -1,    -1,
      18,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,   169,    -1,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,   190,   191,    -1,    -1,
     194,    -1,    -1,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,   147,
     133,   134,   135,    -1,    -1,    -1,   139,    64,    65,    66,
     158,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,   169,    -1,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,    -1,
      -1,   194,    -1,    -1,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
     147,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
      -1,    -1,   194,    -1,    -1,   106,   107,   108,   109,   110,
      20,    21,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
     191,    -1,    -1,   194,    -1,    -1,   106,   107,   108,   109,
     110,    20,    21,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,    -1,    -1,   194,    -1,    -1,   106,   107,   108,
     109,   110,    20,    21,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   191,    -1,    -1,   194,    -1,    -1,   106,   107,
     108,   109,   110,    20,    21,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,   191,    -1,    -1,   194,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,   191,    -1,    -1,   194,    -1,    -1,
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,   191,    -1,    -1,   194,    -1,
      -1,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,   191,    -1,    -1,   194,
      -1,    -1,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,    -1,    -1,
     194,    -1,    -1,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,    -1,
      -1,   194,    -1,    -1,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
      -1,    -1,   194,    -1,    -1,   106,   107,   108,   109,   110,
      20,    21,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
     191,    -1,    -1,   194,    -1,    -1,   106,   107,   108,   109,
     110,    20,    21,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,    -1,    -1,   194,    -1,    -1,   106,   107,   108,
     109,   110,    20,    21,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   191,    -1,    -1,   194,    -1,    -1,   106,   107,
     108,   109,   110,    20,    21,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,   191,    -1,    -1,   194,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,   191,    -1,    -1,   194,    -1,    -1,
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,   191,    -1,    -1,   194,    -1,
      -1,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,   191,    -1,    -1,   194,
      -1,    -1,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,    -1,    -1,
     194,    -1,    -1,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,    -1,
      -1,   194,    -1,    -1,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
      -1,    -1,   194,    -1,    -1,   106,   107,   108,   109,   110,
      20,    21,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
     191,    -1,    -1,   194,    -1,    -1,   106,   107,   108,   109,
     110,    20,    21,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,    -1,    -1,   194,    -1,    -1,   106,   107,   108,
     109,   110,    20,    21,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   191,    -1,    -1,   194,    -1,    -1,   106,   107,
     108,   109,   110,    20,    21,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,   191,    -1,    -1,   194,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,   191,   192,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,   191,   192,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,   191,   192,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    20,
      21,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    20,    21,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,   137,   138,   139,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,   169,    -1,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    20,    21,
     118,   119,   120,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,    -1,    -1,   183,   184,    -1,    -1,    -1,
      -1,    -1,   190,   191,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,   131,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   190,   191,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    20,
      21,    -1,   125,    -1,   127,   128,    -1,    -1,   131,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,   190,
     191,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      20,    21,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,   169,    -1,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     190,   191,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    20,    21,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      20,    21,    -1,    -1,   133,   134,   135,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      20,    21,    -1,    -1,   133,    -1,   135,    -1,    -1,    -1,
     169,    -1,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,   190,   191,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    20,
      21,    -1,    -1,   133,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    20,    21,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     190,   191,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    20,    21,    -1,   125,    -1,   127,   128,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
     191,    -1,    -1,    -1,    -1,   173,   174,   175,   176,   177,
     178,   179,   180,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   190,   191,   113,    -1,    -1,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
     179,   180,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   191,    -1,    -1,    -1,    -1,    62,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,   158,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,   158,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,   158,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   202,     0,     6,    29,    31,    33,    39,    48,    54,
      73,    94,   191,   197,   205,   213,   214,   219,   247,   251,
     274,   341,   347,   350,   356,   395,   398,    18,    19,   158,
     240,   241,   242,   152,   220,   221,   158,   159,   180,   215,
     216,   217,   158,   198,   344,   158,   195,   204,   399,   396,
      33,    61,   132,   158,   184,   193,   243,   244,   245,   246,
     261,     4,     5,     7,    35,   354,    60,   339,   168,   167,
     170,   167,   216,    21,    55,   179,   190,   218,   345,   344,
     346,   339,   158,   158,   158,   132,   245,   245,   193,   133,
     134,   135,   167,   192,    55,    61,   252,   254,    55,    61,
     348,    55,    61,   355,    55,    61,   340,    14,    15,   152,
     156,   158,   160,   193,   207,   241,   152,   221,   158,   215,
     215,   168,   197,   199,   344,    55,    61,   203,   198,   397,
     158,   194,   242,   245,   245,   245,   245,   255,   158,   349,
     357,   198,   342,   161,   162,   206,    14,    15,   152,   156,
     158,   207,   238,   239,   218,    24,    30,    47,    62,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
     145,   261,   360,   362,   363,   365,   369,   394,   197,   158,
     364,   198,   194,    34,    64,    66,    68,    69,    70,    71,
      72,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      84,    85,    86,    89,    90,    91,    92,   108,   109,   158,
     250,   253,   170,   198,    97,   352,   353,   337,   149,   316,
     161,   162,   163,   167,   194,   174,   174,   174,   174,   193,
     174,   174,   174,   174,   174,   174,   193,    32,    58,    59,
     114,   118,   169,   173,   176,   191,   200,   171,   197,   158,
     329,   330,    20,    21,    37,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   118,   119,   120,   126,   127,   128,   129,
     130,   133,   134,   135,   136,   137,   138,   139,   169,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   183,
     184,   190,   191,    34,    34,   193,   248,   198,   256,    68,
      72,    89,    90,    91,    92,   361,   343,   158,   358,   199,
     338,   242,   141,   158,   335,   336,   238,   372,   374,   376,
     370,   158,   366,   378,   380,   382,   384,   386,   388,   390,
     392,    14,    15,    16,    17,    18,    26,    38,    43,    46,
      49,    53,    63,    73,    95,   108,   109,   140,   141,   142,
     143,   144,   146,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   160,   176,   177,   178,   180,   183,   184,
     193,   195,   196,   209,   211,   256,   261,   266,   276,   283,
     286,   289,   293,   294,   297,   298,   300,   305,   308,   315,
     360,   400,   404,   408,   410,   412,    32,    32,   173,   191,
     200,   192,   308,   200,   369,   158,   199,   170,   197,   158,
     158,   192,    21,   192,   144,   194,   316,   325,   326,   170,
     249,   258,   198,   158,   199,   170,   351,   198,   316,   192,
     193,    41,   167,   170,   173,   334,   394,   368,   394,   394,
     394,   194,   364,   394,   248,   394,   248,   394,   248,   158,
     327,   328,   394,   330,   174,   263,   362,   400,   193,   174,
     193,   174,   174,   193,   174,   193,   174,   308,   308,   193,
     193,   193,   193,   193,   193,    12,   369,    12,   369,   308,
     406,   409,   208,   308,   308,   308,   261,   308,   308,   308,
     196,   158,   193,   247,    20,    21,   106,   107,   108,   109,
     110,   113,   114,   115,   116,   118,   119,   120,   121,   125,
     127,   128,   133,   134,   135,   139,   169,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   190,   191,   194,
     193,   192,   192,   394,   199,   329,   158,   193,    48,   141,
     158,   332,   359,   194,   197,   394,     1,     8,     9,    10,
      12,    25,    27,    28,    37,    40,    42,    50,    52,    56,
      57,    63,    96,   123,   124,   159,   197,   199,   222,   223,
     226,   229,   230,   232,   233,   234,   235,   236,   257,   259,
     260,   262,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   295,   296,   308,   343,   168,   197,   261,   321,   336,
     192,   308,   158,   158,   369,   117,   126,   168,   333,   175,
     175,   197,   175,   175,   175,   197,   175,   249,   175,   249,
     175,   249,   170,   175,   197,   175,   197,   264,   193,   261,
     284,   308,   277,   279,   308,   281,   308,   369,   308,   308,
     308,   308,   308,   308,   359,    51,   158,   176,   193,   308,
     401,   402,   403,   405,   407,   359,   193,   402,   407,   131,
     197,   199,   163,   164,   206,   212,   194,   141,   146,   174,
     261,   299,   191,   194,   287,   308,   149,   292,    18,   147,
     158,   360,    18,   147,   158,   360,   308,   308,   308,   308,
     308,   308,   158,   308,   147,   158,   308,   308,   308,   308,
     308,   308,   308,   308,   308,    21,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   119,   120,   147,
     158,   190,   191,   306,   308,   194,   287,   308,   332,   193,
      41,   117,   167,   168,   170,   173,   331,   325,   308,   359,
     117,   260,   295,   308,   256,    59,   308,   308,   158,   197,
     152,    56,   308,   256,   117,   260,   308,   196,   293,   293,
     308,    36,   197,   197,   308,     9,   197,   197,   197,   197,
     197,   336,   122,   197,    98,    99,   100,   101,   102,   103,
     104,   105,   111,   112,   117,   126,   129,   130,   136,   137,
     138,   168,   199,   308,   191,   199,   247,   322,   194,    41,
     197,   333,   260,   308,   373,   375,   394,   377,   371,   367,
     379,   175,   383,   175,   387,   175,   394,   391,   327,   393,
     394,   194,   287,   174,   308,   394,   194,   369,   369,   194,
     369,   194,   175,   194,   194,   194,   194,   194,   194,    19,
     293,   126,   331,   194,   131,   167,   197,   403,   192,   167,
     192,   197,    19,   194,   403,   199,   308,   406,   199,   308,
     161,   210,   193,   193,   301,   303,   158,   401,   167,   194,
     117,   126,   168,   173,   290,   291,   248,   174,   193,   174,
     193,   193,   193,   192,    18,   147,   158,   360,   170,   147,
     158,   308,   193,   147,   158,   308,     1,   192,   194,   194,
     308,   158,   158,   394,   260,   308,   256,    19,   260,   308,
     117,   168,    13,   308,   256,   168,   170,   152,   260,   308,
     196,   256,   198,   256,   231,   293,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   123,   124,   308,   308,
     308,   308,   308,   308,   308,   123,   124,   308,   197,   246,
       7,   316,   320,   158,   260,   308,   197,   381,   385,   389,
     175,   194,   158,   194,   175,   175,   175,   175,   292,   193,
     287,   308,   308,   402,   403,   308,   158,   401,   192,   192,
     308,   192,   405,   287,   402,   199,   192,   165,   161,   308,
     308,   369,   248,   192,   308,   158,   158,   158,   158,   167,
     192,   249,   309,   308,   311,   308,   194,   287,   308,   174,
     193,   308,   193,   192,   308,   193,   192,   307,   194,    41,
     331,   287,   256,   237,   258,    11,    22,    23,   224,   225,
     308,   293,   293,   293,   293,   192,    55,    61,   319,    44,
      97,   317,   197,   197,   265,   175,   197,   285,   278,   280,
     282,   193,   194,   287,   197,   403,   192,   126,   331,   192,
     197,   403,   192,   194,   194,   175,   249,   194,   290,   192,
     131,   256,   288,   369,   194,   394,   194,   194,   194,   313,
     308,   308,   194,   308,   158,   308,   197,   308,   199,   256,
     308,    11,   227,   197,    45,   317,   319,   308,   158,   308,
     308,   308,   194,   308,   194,   308,   192,   192,   308,   308,
     308,   199,   302,   175,   117,   308,   175,   175,   394,   194,
     194,   194,   256,   256,   228,   197,   323,    32,   318,   332,
     194,   175,   194,   197,   411,   192,   411,   192,   299,   304,
     308,   310,   312,   175,   225,    25,    96,   229,   267,   268,
     269,   271,   308,   253,   324,   308,    51,   192,   199,   299,
     314,   117,   308,   117,   308,   197,   253,   194,   308,   192,
     192,   308,   308,   256
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   201,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   203,   203,   203,   204,   204,
     205,   206,   206,   206,   206,   207,   208,   208,   208,   209,
     210,   210,   212,   211,   213,   214,   215,   215,   216,   216,
     216,   216,   217,   217,   218,   218,   219,   220,   220,   221,
     221,   222,   223,   223,   224,   224,   225,   225,   225,   226,
     226,   227,   228,   227,   229,   229,   229,   229,   229,   230,
     231,   230,   232,   233,   234,   235,   237,   236,   238,   238,
     238,   238,   238,   238,   239,   239,   240,   240,   240,   241,
     241,   241,   241,   241,   241,   241,   241,   242,   242,   243,
     243,   243,   244,   244,   245,   245,   245,   245,   245,   245,
     246,   246,   247,   247,   248,   248,   248,   249,   249,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   251,
     252,   252,   252,   253,   255,   254,   256,   256,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   258,
     258,   258,   259,   260,   260,   260,   261,   261,   261,   262,
     262,   264,   265,   263,   263,   266,   266,   266,   266,   267,
     268,   269,   269,   269,   270,   270,   270,   271,   271,   272,
     272,   272,   273,   274,   274,   275,   277,   278,   276,   279,
     280,   276,   281,   282,   276,   284,   285,   283,   286,   286,
     286,   287,   287,   288,   288,   288,   289,   289,   289,   290,
     290,   290,   290,   291,   291,   292,   292,   293,   293,   294,
     294,   294,   294,   294,   294,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   296,   296,   296,   296,   297,   298,
     298,   299,   299,   300,   301,   302,   300,   303,   304,   300,
     305,   305,   306,   307,   305,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   309,   310,   308,   308,   308,   308,
     311,   312,   308,   308,   308,   313,   314,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   316,   316,   317,   317,
     317,   318,   318,   319,   319,   319,   320,   321,   322,   321,
     323,   321,   324,   321,   321,   325,   325,   325,   326,   326,
     327,   327,   328,   328,   329,   330,   330,   331,   331,   332,
     332,   332,   332,   332,   332,   333,   333,   333,   334,   334,
     335,   335,   335,   335,   335,   336,   336,   336,   336,   336,
     337,   338,   337,   339,   339,   340,   340,   340,   341,   342,
     341,   343,   343,   343,   345,   344,   346,   346,   347,   347,
     348,   348,   348,   349,   350,   350,   351,   351,   352,   352,
     353,   354,   354,   355,   355,   355,   357,   358,   356,   359,
     359,   359,   359,   359,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   361,
     361,   361,   361,   361,   361,   362,   363,   363,   363,   364,
     364,   366,   367,   365,   368,   368,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   370,   371,   369,   369,   372,   373,   369,
     374,   375,   369,   376,   377,   369,   369,   378,   379,   369,
     380,   381,   369,   369,   382,   383,   369,   384,   385,   369,
     369,   386,   387,   369,   388,   389,   369,   390,   391,   369,
     392,   393,   369,   394,   394,   394,   396,   397,   395,   399,
     398,   400,   400,   400,   400,   401,   401,   401,   401,   402,
     402,   403,   403,   404,   404,   404,   404,   404,   404,   405,
     405,   405,   406,   406,   407,   407,   408,   408,   409,   409,
     410,   411,   411,   412,   412
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
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
       2,     2,     2,     3,     3,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     1,     3,     0,     4,     3,     7,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     2,     2,     2,     0,
       2,     2,     3,     3,     2,     2,     1,     3,     2,     2,
       3,     0,     0,     5,     1,     2,     4,     5,     2,     1,
       1,     1,     2,     3,     2,     2,     3,     2,     3,     2,
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
       4,     4,     4,     4,     4,     1,     6,     7,     6,     6,
       7,     7,     6,     7,     6,     6,     0,     4,     0,     1,
       1,     0,     1,     0,     1,     1,     4,     0,     0,     4,
       0,     8,     0,     9,     5,     2,     3,     4,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     1,     1,     1,
       2,     3,     5,     3,     3,     1,     1,     1,     0,     1,
       1,     4,     3,     3,     5,     4,     6,     5,     5,     4,
       0,     0,     4,     0,     1,     0,     1,     1,     6,     0,
       6,     0,     3,     5,     0,     4,     2,     3,     4,     2,
       0,     1,     1,     1,     7,     9,     0,     2,     0,     1,
       3,     1,     1,     0,     1,     1,     0,     0,     9,     1,
       4,     3,     3,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     1,
       3,     0,     0,     6,     1,     3,     1,     1,     1,     1,
       4,     3,     4,     2,     2,     3,     2,     3,     2,     2,
       3,     3,     2,     0,     0,     6,     2,     0,     0,     6,
       0,     0,     6,     0,     0,     6,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     0,     0,     6,
       0,     0,     6,     1,     3,     3,     0,     0,     8,     0,
       7,     1,     1,     1,     1,     3,     3,     5,     5,     1,
       3,     0,     2,     6,     5,     7,     8,     6,     8,     1,
       3,     3,     3,     1,     1,     3,     5,     5,     1,     3,
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

  case 167: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 168: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 169: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 170: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 171: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 172: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 173: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 174: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 175: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 176: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 177: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 178: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 179: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 180: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 181: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 182: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 183: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 184: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 185: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 186: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 187: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 188: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 189: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 190: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 191: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 192: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 193: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 194: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 195: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 196: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 197: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 198: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 199: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 200: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 201: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 202: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 203: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 204: /* $@5: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 205: /* function_declaration: optional_public_or_private_function $@5 function_declaration_header expression_block  */
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

  case 206: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 207: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 208: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 209: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 210: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 211: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 212: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 213: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 214: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 215: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 216: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 217: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 218: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 219: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 220: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 221: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 222: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 226: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 227: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 228: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 229: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 230: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 231: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 232: /* expr_keyword: "keyword" expr expression_block  */
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

  case 233: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 234: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 235: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 236: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 237: /* name_in_namespace: "name" "::" "name"  */
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

  case 238: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 239: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 240: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 241: /* $@6: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 242: /* $@7: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 243: /* new_type_declaration: '<' $@6 type_declaration '>' $@7  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 244: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 245: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 246: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 247: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 248: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 249: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 250: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 251: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 252: /* expression_return_no_pipe: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 253: /* expression_return_no_pipe: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 254: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 255: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 256: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 257: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 258: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 259: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 260: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 261: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 262: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 263: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 264: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 265: /* expression_let: kwd_let let_variable_declaration  */
                                               {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 266: /* $@8: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 267: /* $@9: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 268: /* expr_cast: "cast" '<' $@8 type_declaration_no_options '>' $@9 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 269: /* $@10: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 270: /* $@11: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 271: /* expr_cast: "upcast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 272: /* $@12: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 273: /* $@13: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 274: /* expr_cast: "reinterpret" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 275: /* $@14: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 276: /* $@15: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 277: /* expr_type_decl: "type" '<' $@14 type_declaration '>' $@15  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 278: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 279: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 280: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 281: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 282: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 283: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 284: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 285: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 286: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 287: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 288: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 289: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 290: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 291: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 292: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 293: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 294: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 295: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 296: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 297: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 298: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 299: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 300: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 301: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 302: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 303: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 304: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 305: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 306: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 307: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 308: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 309: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 310: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 311: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 312: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 313: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 314: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 315: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 316: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 317: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 318: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 319: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 320: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 325: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 326: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 329: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 330: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 331: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 332: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 333: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 334: /* $@16: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 335: /* $@17: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 336: /* func_addr_expr: '@' '@' '<' $@16 type_declaration_no_options '>' $@17 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 337: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 338: /* $@19: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 339: /* func_addr_expr: '@' '@' '<' $@18 optional_function_argument_list optional_function_type '>' $@19 func_addr_name  */
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

  case 340: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 341: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 342: /* $@20: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 343: /* $@21: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 344: /* expr_field: expr '.' $@20 error $@21  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 345: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 346: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 347: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 348: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 349: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 350: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 351: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 352: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 353: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 354: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 355: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 381: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 384: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 385: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 386: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 387: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 388: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 389: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 390: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 391: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 392: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 393: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 394: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 395: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 396: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 397: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 399: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 400: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 401: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 402: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 404: /* $@22: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 405: /* $@23: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 406: /* expr: expr "is" "type" '<' $@22 type_declaration_no_options '>' $@23  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 407: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 408: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 409: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 410: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 411: /* $@25: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 412: /* expr: expr "as" "type" '<' $@24 type_declaration '>' $@25  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 413: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 414: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 415: /* $@26: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 416: /* $@27: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 417: /* expr: expr '?' "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 418: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 419: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 420: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 421: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 422: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 423: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 424: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 425: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 426: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 427: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) =ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 428: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 429: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 430: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 431: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 432: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 433: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 434: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 435: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 436: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 437: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 438: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 439: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 440: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 441: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 442: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 443: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 444: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 445: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 446: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 447: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 448: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 449: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 450: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 451: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 452: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 453: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 454: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 455: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 456: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 457: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 458: /* $@28: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 459: /* struct_variable_declaration_list: struct_variable_declaration_list $@28 structure_variable_declaration ';'  */
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

  case 460: /* $@29: %empty  */
                                                                                             {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 461: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@29 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
            }
    break;

  case 462: /* $@30: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 463: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@30 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 464: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 465: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 466: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 467: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 468: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 469: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 470: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 471: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 472: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 473: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 474: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 475: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 476: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 477: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 478: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 479: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 480: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 481: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 482: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 483: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 484: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 485: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 486: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 487: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 488: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 489: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 490: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 491: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 492: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 493: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 494: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 495: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 496: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 497: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 498: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 499: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 500: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 501: /* $@31: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 502: /* global_variable_declaration_list: global_variable_declaration_list $@31 optional_field_annotation let_variable_declaration  */
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

  case 503: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 504: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 505: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 506: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 507: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 508: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 509: /* $@32: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 510: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@32 optional_field_annotation let_variable_declaration  */
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

  case 511: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 512: /* enum_list: enum_list "name" ';'  */
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

  case 513: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 514: /* $@33: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 515: /* single_alias: "name" $@33 '=' type_declaration  */
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

  case 520: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 521: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 522: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 523: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 524: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 525: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 526: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 527: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 528: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 529: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 530: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 531: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 532: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 533: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 534: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 535: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 536: /* $@34: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 537: /* $@35: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 538: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@34 structure_name $@35 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
        }
    }
    break;

  case 539: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 540: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 541: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 542: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 543: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 544: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 545: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 546: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 547: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 548: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 549: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 550: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 551: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 552: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 553: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 554: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 555: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 556: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 557: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 558: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 559: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 560: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 561: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 562: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 563: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 564: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 565: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 566: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 567: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 568: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 569: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 570: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 571: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 572: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 573: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 574: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 575: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 576: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 577: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 578: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 579: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 580: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 581: /* $@36: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 582: /* $@37: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 583: /* bitfield_type_declaration: "bitfield" '<' $@36 bitfield_bits '>' $@37  */
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

  case 584: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 585: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 586: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 587: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 588: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 589: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 590: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
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

  case 591: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 592: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 593: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 594: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 595: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 596: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 597: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 598: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 599: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 600: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 601: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 602: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 603: /* $@38: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 604: /* $@39: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 605: /* type_declaration_no_options: "smart_ptr" '<' $@38 type_declaration '>' $@39  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 606: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 607: /* $@40: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 608: /* $@41: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 609: /* type_declaration_no_options: "array" '<' $@40 type_declaration '>' $@41  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 610: /* $@42: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 611: /* $@43: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 612: /* type_declaration_no_options: "table" '<' $@42 table_type_pair '>' $@43  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 613: /* $@44: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 614: /* $@45: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 615: /* type_declaration_no_options: "iterator" '<' $@44 type_declaration '>' $@45  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 616: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 617: /* $@46: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 618: /* $@47: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 619: /* type_declaration_no_options: "block" '<' $@46 type_declaration '>' $@47  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 620: /* $@48: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 621: /* $@49: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 622: /* type_declaration_no_options: "block" '<' $@48 optional_function_argument_list optional_function_type '>' $@49  */
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

  case 623: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 624: /* $@50: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 625: /* $@51: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 626: /* type_declaration_no_options: "function" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 627: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 628: /* $@53: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 629: /* type_declaration_no_options: "function" '<' $@52 optional_function_argument_list optional_function_type '>' $@53  */
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

  case 630: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 631: /* $@54: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 632: /* $@55: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 633: /* type_declaration_no_options: "lambda" '<' $@54 type_declaration '>' $@55  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 634: /* $@56: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 635: /* $@57: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 636: /* type_declaration_no_options: "lambda" '<' $@56 optional_function_argument_list optional_function_type '>' $@57  */
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

  case 637: /* $@58: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 638: /* $@59: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 639: /* type_declaration_no_options: "tuple" '<' $@58 tuple_type_list '>' $@59  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 640: /* $@60: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 641: /* $@61: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 642: /* type_declaration_no_options: "variant" '<' $@60 variant_type_list '>' $@61  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 643: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 644: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 645: /* type_declaration: type_declaration '|' '#'  */
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

  case 646: /* $@62: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 647: /* $@63: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 648: /* variant_alias_declaration: "variant" $@62 "name" $@63 '{' variant_type_list ';' '}'  */
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

  case 649: /* $@64: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 650: /* bitfield_alias_declaration: "bitfield" $@64 "name" '{' bitfield_bits ';' '}'  */
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

  case 651: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 652: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 653: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 654: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 655: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 656: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 657: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 658: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 659: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 660: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 661: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 662: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 663: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block ']' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 664: /* make_struct_decl: "[[" type_declaration_no_options optional_block ']' ']'  */
                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 665: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block ']' ']'  */
                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 666: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 667: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block '}' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 668: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block '}' ']'  */
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

  case 669: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 670: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 671: /* make_tuple: make_tuple ',' expr  */
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

  case 672: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 673: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 674: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 675: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 676: /* make_dim_decl: "[[" type_declaration_no_options make_dim ']' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 677: /* make_dim_decl: "[{" type_declaration_no_options make_dim '}' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 678: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 679: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 680: /* make_table_decl: "{{" make_table '}' '}'  */
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

  case 681: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 682: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 683: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 684: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


