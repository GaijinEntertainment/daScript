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
  YYSYMBOL_BEGIN_STRING = 159,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 160,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 161,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 162,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 163,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 164,          /* "}"  */
  YYSYMBOL_END_OF_READ = 165,              /* "end of failed eader macro"  */
  YYSYMBOL_166_ = 166,                     /* ','  */
  YYSYMBOL_167_ = 167,                     /* '='  */
  YYSYMBOL_168_ = 168,                     /* '?'  */
  YYSYMBOL_169_ = 169,                     /* ':'  */
  YYSYMBOL_170_ = 170,                     /* '|'  */
  YYSYMBOL_171_ = 171,                     /* '^'  */
  YYSYMBOL_172_ = 172,                     /* '&'  */
  YYSYMBOL_173_ = 173,                     /* '<'  */
  YYSYMBOL_174_ = 174,                     /* '>'  */
  YYSYMBOL_175_ = 175,                     /* '-'  */
  YYSYMBOL_176_ = 176,                     /* '+'  */
  YYSYMBOL_177_ = 177,                     /* '*'  */
  YYSYMBOL_178_ = 178,                     /* '/'  */
  YYSYMBOL_179_ = 179,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 180,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 181,               /* UNARY_PLUS  */
  YYSYMBOL_182_ = 182,                     /* '~'  */
  YYSYMBOL_183_ = 183,                     /* '!'  */
  YYSYMBOL_PRE_INC = 184,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 185,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 186,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 187,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 188,                    /* DEREF  */
  YYSYMBOL_189_ = 189,                     /* '.'  */
  YYSYMBOL_190_ = 190,                     /* '['  */
  YYSYMBOL_191_ = 191,                     /* ']'  */
  YYSYMBOL_192_ = 192,                     /* '('  */
  YYSYMBOL_193_ = 193,                     /* ')'  */
  YYSYMBOL_194_ = 194,                     /* '$'  */
  YYSYMBOL_195_ = 195,                     /* '@'  */
  YYSYMBOL_196_ = 196,                     /* ';'  */
  YYSYMBOL_197_ = 197,                     /* '{'  */
  YYSYMBOL_198_ = 198,                     /* '}'  */
  YYSYMBOL_199_ = 199,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 200,                 /* $accept  */
  YYSYMBOL_program = 201,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 202, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 203,              /* module_name  */
  YYSYMBOL_module_declaration = 204,       /* module_declaration  */
  YYSYMBOL_character_sequence = 205,       /* character_sequence  */
  YYSYMBOL_string_constant = 206,          /* string_constant  */
  YYSYMBOL_string_builder_body = 207,      /* string_builder_body  */
  YYSYMBOL_string_builder = 208,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 209, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 210,              /* expr_reader  */
  YYSYMBOL_211_1 = 211,                    /* $@1  */
  YYSYMBOL_options_declaration = 212,      /* options_declaration  */
  YYSYMBOL_require_declaration = 213,      /* require_declaration  */
  YYSYMBOL_require_module_name = 214,      /* require_module_name  */
  YYSYMBOL_require_module = 215,           /* require_module  */
  YYSYMBOL_is_public_module = 216,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 217,       /* expect_declaration  */
  YYSYMBOL_expect_list = 218,              /* expect_list  */
  YYSYMBOL_expect_error = 219,             /* expect_error  */
  YYSYMBOL_expression_label = 220,         /* expression_label  */
  YYSYMBOL_expression_goto = 221,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 222,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 223,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 224,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 225, /* expression_else_one_liner  */
  YYSYMBOL_226_2 = 226,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 227,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 228,  /* expression_if_then_else  */
  YYSYMBOL_229_3 = 229,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 230,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 231,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 232,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 233,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 234,    /* expression_with_alias  */
  YYSYMBOL_235_4 = 235,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 236, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 237, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 238, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 239,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 240, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 241, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 242, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 243,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 244,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 245, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 246, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 247,   /* optional_function_type  */
  YYSYMBOL_function_name = 248,            /* function_name  */
  YYSYMBOL_global_function_declaration = 249, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 250, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 251, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 252,     /* function_declaration  */
  YYSYMBOL_253_5 = 253,                    /* $@5  */
  YYSYMBOL_expression_block = 254,         /* expression_block  */
  YYSYMBOL_expression_any = 255,           /* expression_any  */
  YYSYMBOL_expressions = 256,              /* expressions  */
  YYSYMBOL_expr_pipe = 257,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 258,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 259,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 260,     /* new_type_declaration  */
  YYSYMBOL_261_6 = 261,                    /* $@6  */
  YYSYMBOL_262_7 = 262,                    /* $@7  */
  YYSYMBOL_expr_new = 263,                 /* expr_new  */
  YYSYMBOL_expression_break = 264,         /* expression_break  */
  YYSYMBOL_expression_continue = 265,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 266, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 267,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 268, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 269,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 270,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 271,                  /* kwd_let  */
  YYSYMBOL_expression_let = 272,           /* expression_let  */
  YYSYMBOL_expr_cast = 273,                /* expr_cast  */
  YYSYMBOL_274_8 = 274,                    /* $@8  */
  YYSYMBOL_275_9 = 275,                    /* $@9  */
  YYSYMBOL_276_10 = 276,                   /* $@10  */
  YYSYMBOL_277_11 = 277,                   /* $@11  */
  YYSYMBOL_278_12 = 278,                   /* $@12  */
  YYSYMBOL_279_13 = 279,                   /* $@13  */
  YYSYMBOL_expr_type_decl = 280,           /* expr_type_decl  */
  YYSYMBOL_281_14 = 281,                   /* $@14  */
  YYSYMBOL_282_15 = 282,                   /* $@15  */
  YYSYMBOL_expr_type_info = 283,           /* expr_type_info  */
  YYSYMBOL_expr_list = 284,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 285,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 286,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 287,            /* capture_entry  */
  YYSYMBOL_capture_list = 288,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 289,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 290,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 291,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 292,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 293,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 294,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 295,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 296,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 297,           /* func_addr_expr  */
  YYSYMBOL_298_16 = 298,                   /* $@16  */
  YYSYMBOL_299_17 = 299,                   /* $@17  */
  YYSYMBOL_300_18 = 300,                   /* $@18  */
  YYSYMBOL_301_19 = 301,                   /* $@19  */
  YYSYMBOL_expr_field = 302,               /* expr_field  */
  YYSYMBOL_303_20 = 303,                   /* $@20  */
  YYSYMBOL_304_21 = 304,                   /* $@21  */
  YYSYMBOL_expr = 305,                     /* expr  */
  YYSYMBOL_306_22 = 306,                   /* $@22  */
  YYSYMBOL_307_23 = 307,                   /* $@23  */
  YYSYMBOL_308_24 = 308,                   /* $@24  */
  YYSYMBOL_309_25 = 309,                   /* $@25  */
  YYSYMBOL_310_26 = 310,                   /* $@26  */
  YYSYMBOL_311_27 = 311,                   /* $@27  */
  YYSYMBOL_expr_mtag = 312,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 313, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 314,        /* optional_override  */
  YYSYMBOL_optional_constant = 315,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 316, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 317, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 318, /* struct_variable_declaration_list  */
  YYSYMBOL_319_28 = 319,                   /* $@28  */
  YYSYMBOL_320_29 = 320,                   /* $@29  */
  YYSYMBOL_321_30 = 321,                   /* $@30  */
  YYSYMBOL_function_argument_declaration = 322, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 323,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 324,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 325,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 326,             /* variant_type  */
  YYSYMBOL_variant_type_list = 327,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 328,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 329,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 330,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 331,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 332, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 333, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 334, /* global_variable_declaration_list  */
  YYSYMBOL_335_31 = 335,                   /* $@31  */
  YYSYMBOL_optional_shared = 336,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 337, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 338,               /* global_let  */
  YYSYMBOL_339_32 = 339,                   /* $@32  */
  YYSYMBOL_enum_list = 340,                /* enum_list  */
  YYSYMBOL_single_alias = 341,             /* single_alias  */
  YYSYMBOL_342_33 = 342,                   /* $@33  */
  YYSYMBOL_alias_list = 343,               /* alias_list  */
  YYSYMBOL_alias_declaration = 344,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 345, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 346,                /* enum_name  */
  YYSYMBOL_enum_declaration = 347,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 348, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 349,          /* optional_sealed  */
  YYSYMBOL_structure_name = 350,           /* structure_name  */
  YYSYMBOL_class_or_struct = 351,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 352, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 353,    /* structure_declaration  */
  YYSYMBOL_354_34 = 354,                   /* $@34  */
  YYSYMBOL_355_35 = 355,                   /* $@35  */
  YYSYMBOL_variable_name_with_pos_list = 356, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 357,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 358, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 359, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 360,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 361,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 362, /* bitfield_type_declaration  */
  YYSYMBOL_363_36 = 363,                   /* $@36  */
  YYSYMBOL_364_37 = 364,                   /* $@37  */
  YYSYMBOL_table_type_pair = 365,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 366, /* type_declaration_no_options  */
  YYSYMBOL_367_38 = 367,                   /* $@38  */
  YYSYMBOL_368_39 = 368,                   /* $@39  */
  YYSYMBOL_369_40 = 369,                   /* $@40  */
  YYSYMBOL_370_41 = 370,                   /* $@41  */
  YYSYMBOL_371_42 = 371,                   /* $@42  */
  YYSYMBOL_372_43 = 372,                   /* $@43  */
  YYSYMBOL_373_44 = 373,                   /* $@44  */
  YYSYMBOL_374_45 = 374,                   /* $@45  */
  YYSYMBOL_375_46 = 375,                   /* $@46  */
  YYSYMBOL_376_47 = 376,                   /* $@47  */
  YYSYMBOL_377_48 = 377,                   /* $@48  */
  YYSYMBOL_378_49 = 378,                   /* $@49  */
  YYSYMBOL_379_50 = 379,                   /* $@50  */
  YYSYMBOL_380_51 = 380,                   /* $@51  */
  YYSYMBOL_381_52 = 381,                   /* $@52  */
  YYSYMBOL_382_53 = 382,                   /* $@53  */
  YYSYMBOL_383_54 = 383,                   /* $@54  */
  YYSYMBOL_384_55 = 384,                   /* $@55  */
  YYSYMBOL_385_56 = 385,                   /* $@56  */
  YYSYMBOL_386_57 = 386,                   /* $@57  */
  YYSYMBOL_387_58 = 387,                   /* $@58  */
  YYSYMBOL_388_59 = 388,                   /* $@59  */
  YYSYMBOL_389_60 = 389,                   /* $@60  */
  YYSYMBOL_390_61 = 390,                   /* $@61  */
  YYSYMBOL_type_declaration = 391,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 392, /* variant_alias_declaration  */
  YYSYMBOL_393_62 = 393,                   /* $@62  */
  YYSYMBOL_394_63 = 394,                   /* $@63  */
  YYSYMBOL_bitfield_alias_declaration = 395, /* bitfield_alias_declaration  */
  YYSYMBOL_396_64 = 396,                   /* $@64  */
  YYSYMBOL_make_decl = 397,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 398,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 399,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 400,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 401,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 402,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 403,           /* make_map_tuple  */
  YYSYMBOL_make_dim = 404,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 405,            /* make_dim_decl  */
  YYSYMBOL_make_table = 406,               /* make_table  */
  YYSYMBOL_make_table_decl = 407,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 408, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 409       /* array_comprehension  */
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
#define YYLAST   10859

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  200
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  210
/* YYNRULES -- Number of rules.  */
#define YYNRULES  680
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1198

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   427


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
       2,     2,     2,   183,     2,   199,   194,   179,   172,     2,
     192,   193,   177,   176,   166,   175,   189,   178,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   169,   196,
     173,   167,   174,   168,   195,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   190,     2,   191,   171,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   197,   170,   198,   182,     2,     2,     2,
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
     165,   180,   181,   184,   185,   186,   187,   188
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   477,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   493,   494,   495,   499,   500,
     504,   522,   523,   524,   525,   529,   533,   538,   547,   555,
     571,   576,   584,   584,   614,   635,   639,   642,   646,   652,
     661,   664,   670,   671,   675,   679,   680,   684,   687,   693,
     699,   702,   708,   709,   713,   714,   715,   724,   725,   729,
     730,   730,   736,   737,   738,   739,   740,   744,   750,   750,
     756,   762,   770,   780,   789,   789,   796,   797,   798,   799,
     800,   801,   805,   810,   818,   819,   820,   824,   825,   826,
     827,   828,   829,   830,   831,   837,   840,   846,   847,   848,
     852,   860,   873,   876,   884,   895,   906,   917,   923,   927,
     934,   935,   939,   940,   941,   945,   948,   955,   959,   960,
     961,   962,   963,   964,   965,   966,   967,   968,   969,   970,
     971,   972,   973,   974,   975,   976,   977,   978,   979,   980,
     981,   982,   983,   984,   985,   986,   987,   988,   989,   990,
     991,   992,   993,   994,   995,   996,   997,   998,   999,  1000,
    1001,  1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,  1010,
    1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,  1020,
    1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,
    1031,  1032,  1033,  1034,  1035,  1036,  1037,  1042,  1064,  1065,
    1066,  1070,  1076,  1076,  1093,  1097,  1108,  1109,  1110,  1111,
    1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,  1120,  1121,
    1122,  1123,  1124,  1125,  1126,  1127,  1131,  1136,  1142,  1148,
    1182,  1185,  1191,  1192,  1203,  1207,  1210,  1218,  1218,  1218,
    1221,  1227,  1230,  1233,  1237,  1243,  1247,  1251,  1254,  1257,
    1265,  1268,  1271,  1279,  1282,  1290,  1293,  1296,  1304,  1310,
    1311,  1315,  1321,  1321,  1321,  1324,  1324,  1324,  1329,  1329,
    1329,  1337,  1337,  1337,  1343,  1353,  1364,  1379,  1382,  1388,
    1389,  1396,  1407,  1408,  1409,  1413,  1414,  1415,  1416,  1420,
    1425,  1433,  1434,  1438,  1443,  1450,  1451,  1452,  1453,  1454,
    1455,  1459,  1460,  1461,  1462,  1463,  1464,  1465,  1466,  1467,
    1468,  1469,  1470,  1471,  1472,  1473,  1474,  1475,  1476,  1477,
    1481,  1482,  1483,  1484,  1488,  1499,  1504,  1514,  1518,  1525,
    1528,  1528,  1528,  1533,  1533,  1533,  1546,  1550,  1554,  1554,
    1554,  1561,  1562,  1563,  1564,  1565,  1566,  1567,  1568,  1569,
    1570,  1571,  1572,  1573,  1574,  1575,  1576,  1577,  1578,  1579,
    1580,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,
    1590,  1591,  1592,  1593,  1594,  1595,  1596,  1602,  1603,  1604,
    1605,  1606,  1607,  1608,  1609,  1610,  1611,  1612,  1613,  1614,
    1618,  1622,  1625,  1628,  1629,  1630,  1631,  1634,  1637,  1638,
    1641,  1641,  1641,  1644,  1649,  1653,  1657,  1657,  1657,  1662,
    1665,  1669,  1669,  1669,  1674,  1677,  1678,  1679,  1680,  1681,
    1682,  1683,  1684,  1685,  1686,  1687,  1695,  1696,  1697,  1698,
    1699,  1700,  1701,  1705,  1709,  1713,  1717,  1721,  1725,  1729,
    1733,  1737,  1744,  1745,  1749,  1750,  1751,  1755,  1756,  1760,
    1761,  1762,  1766,  1776,  1779,  1779,  1798,  1797,  1812,  1811,
    1824,  1833,  1839,  1844,  1854,  1855,  1859,  1862,  1871,  1872,
    1876,  1885,  1886,  1891,  1892,  1896,  1902,  1908,  1911,  1915,
    1921,  1930,  1931,  1932,  1936,  1937,  1941,  1948,  1953,  1962,
    1968,  1979,  1982,  1987,  1992,  2000,  2011,  2014,  2014,  2034,
    2035,  2039,  2040,  2041,  2045,  2048,  2048,  2067,  2070,  2079,
    2092,  2092,  2113,  2114,  2118,  2119,  2123,  2124,  2125,  2129,
    2139,  2146,  2156,  2157,  2161,  2162,  2166,  2172,  2173,  2177,
    2178,  2179,  2183,  2188,  2183,  2198,  2205,  2210,  2219,  2225,
    2236,  2237,  2238,  2239,  2240,  2241,  2242,  2243,  2244,  2245,
    2246,  2247,  2248,  2249,  2250,  2251,  2252,  2253,  2254,  2255,
    2256,  2257,  2258,  2259,  2260,  2264,  2265,  2266,  2267,  2268,
    2269,  2273,  2284,  2288,  2295,  2307,  2314,  2323,  2323,  2323,
    2336,  2340,  2347,  2348,  2349,  2350,  2351,  2365,  2371,  2375,
    2379,  2384,  2389,  2394,  2399,  2403,  2407,  2412,  2416,  2421,
    2421,  2421,  2427,  2434,  2434,  2434,  2439,  2439,  2439,  2445,
    2445,  2445,  2450,  2454,  2454,  2454,  2459,  2459,  2459,  2468,
    2472,  2472,  2472,  2477,  2477,  2477,  2486,  2490,  2490,  2490,
    2495,  2495,  2495,  2504,  2504,  2504,  2510,  2510,  2510,  2519,
    2522,  2533,  2549,  2549,  2549,  2573,  2573,  2593,  2594,  2595,
    2596,  2600,  2607,  2614,  2620,  2629,  2634,  2641,  2642,  2646,
    2652,  2659,  2667,  2674,  2682,  2694,  2697,  2703,  2717,  2723,
    2729,  2734,  2741,  2746,  2756,  2761,  2768,  2780,  2781,  2785,
    2788
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
  "annotation_declaration_name", "annotation_declaration_basic",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@5", "expression_block", "expression_any",
  "expressions", "expr_pipe", "name_in_namespace", "expression_delete",
  "new_type_declaration", "$@6", "$@7", "expr_new", "expression_break",
  "expression_continue", "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let", "expression_let", "expr_cast", "$@8", "$@9", "$@10", "$@11",
  "$@12", "$@13", "expr_type_decl", "$@14", "$@15", "expr_type_info",
  "expr_list", "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@16", "$@17", "$@18", "$@19", "expr_field", "$@20",
  "$@21", "expr", "$@22", "$@23", "$@24", "$@25", "$@26", "$@27",
  "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
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
     415,   416,   417,   418,   419,   420,    44,    61,    63,    58,
     124,    94,    38,    60,    62,    45,    43,    42,    47,    37,
     421,   422,   126,    33,   423,   424,   425,   426,   427,    46,
      91,    93,    40,    41,    36,    64,    59,   123,   125,    35
};
#endif

#define YYPACT_NINF (-906)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-631)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -906,    65,  -906,  -906,    48,   -68,   106,   -22,  -906,  -115,
    -906,  -906,    24,  -906,  -906,  -906,  -906,  -906,   317,  -906,
      87,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,
      56,  -906,    15,   177,   193,  -906,  -906,   106,     9,  -906,
    -906,   204,  -906,  -906,  -906,    87,   230,   254,  -906,  -906,
     268,   246,    24,    24,   255,  -906,    28,   -27,  -906,  -906,
    -906,   220,   407,   416,  -906,   420,    13,    48,   244,   -68,
     250,   305,  -906,   315,   318,  -906,   288,   291,  -109,   425,
     335,  -906,  -906,   385,  -906,   -73,    48,    24,    24,    24,
      24,  -906,  -906,  -906,  -906,  -906,  -906,  -906,   390,  -906,
    -906,  -906,  -906,  -906,   340,  -906,  -906,  -906,  -906,  -906,
     352,    85,  -906,  -906,  -906,  -906,   502,  -906,  -906, 10606,
    -906,  -906,   362,  -906,  -906,  -906,   403,   365,  -906,  -906,
      -8,  -906,   192,   443,    28,  1518,  -906,    73,   480,  -906,
     433,  -906,  -906,   413,  -906,  -906,  -906,  -906,  -906,  -906,
    -906,   154,  -906,   410,   411,   412,   414,  -906,  -906,  -906,
     401,  -906,  -906,  -906,  -906,  -906,   421,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,  -906,  -906,   426,  -906,  -906,  -906,
     432,   435,  -906,  -906,  -906,  -906,   438,   439,   427,  -906,
    -906,  -906,  -906,  -906,  2518,   436,  -906,  -906,   417,   460,
    -906,  9472,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,  -906,   552,   586,  -906,   430,   429,
     368,  -906,  -906,   465,  -906,   431,    48,   266,  -906,  -906,
    -906,    85,  -906,  -906,  -906,  -906,  -906,   466,  -906,  -100,
     251,   253,  -906,  -906,  5960,  -906,  -906,  -906,   595,  -906,
    -906,  -906,    20,  3571,  -906,   222,   -90,   461,  -906,   437,
     473,   474,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,
    -906,  -906,  -906,   444,  -906,  -906,  -906,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,  -906,  -906,   613,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,
     446,  -906,  -906,   -38,   470,  -906,  -906,  -906,  -906,  -906,
    -906,  -906,  -906,   445,   -88,   471,   448,  -906,   433,   151,
     459,   607,   108,  -906,  -906, 10606, 10606, 10606, 10606,   469,
     403, 10606,   430, 10606,   430, 10606,   430, 10701,   460,  -906,
    -906,   241,   472,   492,  -906,   485,   509,   513,   498,   516,
     499,  -906,   519,  5960,  5960,   505,   506,   507,   508,   510,
     511,  -906, 10385, 10480,  5960,  -906,  -906,  -906,  -906,  -906,
    -906,  -906,  5960,  5960,  5960,   125,  5960,  5960,  5960,  -906,
     512,  -906,  -906,  -906,   -77,  -906,  -906,  -906,  -906,   504,
    -906,  -906,  -906,  -906,  -906,  -906,  6446,  -906,   514,  -906,
    -906,  -906,  -906,  -906,  -906,  -906,  -906,   517,  -906,  -906,
    8903,  -906,  2518,  -906,  -906, 10606,   -31,  -906,  -906,  -906,
     543,  -906,   518,  -906,    -9,  -906,    83, 10606,  -906,  1622,
    -906,   -95,  -906,   125,  -906,  -906,   266,   521,  5960,   546,
     547, 10606,  -906,    57,   314,   535,   161,   330,   341,  -906,
    -123,   348,   470,   349,   470,   351,   470,    -1,  -906,   256,
     436,   260,  -906,   522,  -906,  -906,   125,  -906,  5960,  -906,
    -906,  5960,  -906,  5960, 10606,   345,   345,  5960,  5960,  5960,
    5960,  5960,  5960,   274,  1993,   274,  2163,  9568,  -906,   303,
     409,   345,   345,   -64,  -906,   345,   345,  6537,   228,  -906,
    2960,   564,  9640,  9820,  5960,  5960,  -906,  -906,  5960,  5960,
    5960,  5960,   557,  5960,   120,  5960,  5960,  5960,  5960,  5960,
    5960,  5960,  5960,  5960,  3718,  5960,  5960,  5960,  5960,  5960,
    5960,  5960,  5960,  5960,  5960,   -61,  5960,  -906,  3865,  -906,
    -906,   436,  -906,  -906,  -906,  5960,   274,   524,   676,  -906,
     -15,  -906,    76,   436,  -906,  5960,  -906,  -906,   274,  2666,
    -906,   429,  4035,  5960,   561,  -906,   525,   566,  4182,   131,
    2813,   297,   297,  -906,   684,   526,   527,  5960,   715,  -906,
    -906,  -906,  -906,  -906,  -906,  -906,  -906,   529,   530,   531,
     532,  -906,   533,  -906,  -906,   266,  -906,   -91,  -906,   675,
     -28,  5960,  -906,  -906,    11,  -906,  -906,  6628,  -906,   700,
     708,  -906,  -906,  -906,  3130,  -906,  -906, 10606,  -906,  -906,
    -906,   584,  -906,   569,  -906,   570,  -906,   571, 10606,  -906,
   10701,  -906,   460, 10606,  4329,  4499, 10606,  6719, 10606, 10606,
    6810, 10606,  6901,   801,  6992,  7083,  7174,  7265,  7356,  7447,
      10,   297,   -14,  2333,  4669,  9646,   580,   -19,   556,   587,
     294,    14,  4839,   -19,   331,  5960,  5960,   554,  -906,  5960,
     394,   594,  -906,   563,   565,   277,  -906,  -906,   598,  -906,
     160,  9928,   -79,   430,   585,   567,  -906,  -906,   588,   572,
    -906,  -906,   950,   950,   389,   389, 10252, 10252,   576,    70,
     577,  -906,  8994,   -66,   -66,   950,   950,  1348, 10108,   560,
   10006, 10511,  9748, 10138, 10229,  1218,   389,   389,    25,    25,
      70,    70,    70,   124,  5960,   578,  -906,   284,  5960,   761,
    9085,  -906,   163,  7538,  -906,  5960,   605,  -906,   639,  -906,
   10606,  -906,  3130,  -906,  1309,    18,  3130,  -906,   649,  9369,
     785,  5960,  9928,  1309,   648,  -906,   637,   664,  9928,  -906,
    3130,  -906,  9369,   622,  -906,  -906,   621,  -906,  -906,  1309,
    -906,  -906,  -906,  -906,  -906,  -906,  -906,   297,  -906,  5960,
    5960,  5960,  5960,  5960,  5960,  5960,  5960,  5960,  5960,  3277,
    5960,  5960,  5960,  5960,  5960,  5960,  3424,  -906,  6264,    24,
    -906,   812,   433,  -906,   662,  -906,  3130,  -906,  6060,  -906,
    -906,   436,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,
    -906,   436,  -906,  -906,  -906,   354,  -906,   171,   663,  7629,
     356,  -906,  1074,  1079,  -906,  1923,  -906,   564,  -906,  -906,
    -906,  -906,  -906,   632,  5960,  -906,  5960,  5960,   -16,  5960,
     669,   598,   638,   640,  5960,   641,  5960,  5960,   598,   630,
     644,  9928,  -906,  -906,  9826,  -906,   670,  5960,  5960, 10606,
     430,    96,   238,  5960,  -906,   678,   679,   680,   681,  -906,
     242,   470,  -906,  5960,  -906,  5960,  5009,  5960,  -906,   667,
     652,  -906,  -906,  5960,   665,  -906,  9176,  5960,   666,  -906,
    9267,  -906,  -906,  -906,  -906,  7720,  -906,   800,   -30,  -906,
    9369,  -906,  5960,  -906,  9369,  5960,  5960,   429,  9928,  -906,
    -906,  -906,  -906,  -906,  9369,  -906,  -906,   456,  5960,  -906,
    9928,  9928,  9928,  9928,  9928,  9928,  9928,  9928,  9928,  9928,
     297,   297,  9928,  9928,  9928,  9928,  9928,  9928,  9928,   297,
     297,  9928,  -906,   247,   428,    19,   659,  -906,  -906,  6162,
    -906,  -906,  -906,  -906,  -906,  -906,   276,  -906,  -906,  -906,
    -906,  -906,   671,  5179,     0,  9928,  9928,   -19,   677,  9928,
     101,   580,   682,  -906,  9928,  -906,   587,    51,   -19,   683,
    -906,  -906,  -906,  7811,  7902,  2332,   470,   668,  9928,  -906,
    -906,  -906,  -906,   -79,   687,   -83, 10606,  7993, 10606,  8084,
    -906,   182,  8175,  -906,  5960,  1040,  5960,  -906,  8266,  5960,
    -906,  -906,  -906,   698,  5960,    66,  -906,  5960,  1820,   429,
    -906,  -906,  5960,  -906,   482,  -906,  -906,  -906,  -906,   673,
    -906,  -906,    38,  -906,  -906,   428,  -906,  -906,  -906,  5960,
     704,  -906,  5960,  5960,  5960,  5349,  -906,   202,  5960,   688,
     691,  5960,  5960,  -906,  5960,   674,  -906,  -906,  -906,  -906,
     692,  -906,  -906,  -906,  5519,  -906,  -906,  2959,  -906,   376,
    -906,  -906,  -906, 10606,  8357,  8448,  -906,  8539,  -906,  9928,
     429,  9928,  -906,  -906,  1309,  -906,   689,  -906,  -906,   835,
     274,  8630,   712,   389,   389,   389,  -906,  8721,  -906,  6355,
     696,  -906,  9928,  9928,  6355,   701,   270,  -906,  5960, 10006,
    -906,  -906,   379,  -906,  -906,  -906,  -906,   456,  2519,  -906,
    1518,  -906,  -906,  -906,  -906,  5960,  -906,   845,   706,  -906,
     702,  -906,  -906,   270,  1040,  -906,  -906,  -906,  -906,  5666,
    5813,  -906,  -906,  -906,  -906,  -906,  9928,   703,  1518,  8812,
    5960,   710,   711,  -906,  -906,  5960,  9928,  5960,  9928,  -906,
     429,  -906,  9928,  -906,  -906,  9928,  9928,  -906
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   110,     1,   259,     0,     0,     0,     0,   260,     0,
     645,   642,     0,    14,     3,    10,     9,     8,     0,     7,
     499,     6,    11,     5,     4,    12,    13,    85,    86,    84,
      93,    95,    34,    47,    44,    45,    36,     0,    42,    35,
     510,     0,   515,    19,    18,   499,     0,     0,    98,    99,
       0,   232,     0,     0,   100,   102,   108,     0,    97,   528,
     527,   198,   516,   529,   500,   501,     0,     0,     0,     0,
      37,     0,    43,     0,     0,    40,     0,     0,     0,    15,
       0,   643,   234,     0,   103,     0,     0,     0,     0,     0,
       0,   111,   200,   199,   202,   197,   518,   517,     0,   531,
     530,   532,   503,   502,   505,    91,    92,    89,    90,    88,
       0,     0,    87,    96,    48,    46,    42,    39,    38,     0,
     512,   514,     0,    16,    17,    20,     0,     0,   233,   107,
       0,   104,   105,   106,   109,     0,   519,     0,   524,   496,
     442,    21,    22,     0,    80,    81,    78,    79,    77,    76,
      82,     0,    41,     0,     0,     0,     0,   540,   560,   541,
     572,   542,   546,   547,   548,   549,   564,   553,   554,   555,
     556,   557,   558,   559,   561,   562,   612,   545,   552,   563,
     619,   626,   543,   550,   544,   551,     0,     0,     0,   571,
     582,   585,   583,   584,   639,   511,   513,   575,     0,     0,
     101,     0,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,     0,     0,   117,   112,     0,
       0,   507,   525,     0,   533,   497,     0,     0,    23,    24,
      25,     0,    94,   603,   606,   609,   599,     0,   577,   613,
     620,   627,   633,   636,     0,   590,   595,   589,     0,   602,
     598,   592,     0,     0,   594,     0,     0,     0,   471,     0,
     168,   169,   166,   120,   121,   123,   122,   124,   125,   126,
     127,   153,   154,   151,   152,   144,   155,   156,   145,   142,
     143,   167,   164,     0,   165,   157,   158,   159,   160,   131,
     132,   133,   128,   129,   130,   141,     0,   147,   148,   146,
     139,   140,   135,   134,   136,   137,   138,   119,   118,   163,
       0,   149,   150,   442,   115,   226,   203,   565,   568,   566,
     569,   567,   570,     0,     0,   522,     0,   504,   442,     0,
       0,   486,   484,   506,    83,     0,     0,     0,     0,     0,
       0,     0,   112,     0,   112,     0,   112,     0,     0,   347,
     348,     0,     0,     0,   341,     0,     0,     0,     0,     0,
       0,   564,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,   295,   297,   296,   298,   299,
     300,    26,     0,     0,     0,     0,     0,     0,     0,   282,
     283,   345,   344,   293,   342,   418,   417,   416,   415,   110,
     421,   343,   420,   419,   388,   349,     0,   350,     0,   346,
     647,   648,   649,   650,   597,   591,   593,     0,   596,   587,
       0,   641,   640,   576,   646,     0,     0,   170,   171,   162,
     172,   161,     0,   113,     0,   464,     0,     0,   201,     0,
     507,     0,   520,     0,   526,   453,     0,     0,     0,     0,
       0,     0,   485,     0,     0,     0,   580,     0,     0,   573,
       0,     0,   115,     0,   115,     0,   115,   232,   468,     0,
     466,     0,   237,   241,   240,   244,     0,   271,     0,   262,
     265,     0,   268,     0,     0,   377,   378,     0,     0,     0,
       0,     0,     0,     0,   657,     0,     0,   669,   674,     0,
       0,   354,   353,   393,    32,   352,   351,     0,   284,   424,
       0,   291,     0,     0,     0,     0,   379,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   338,     0,   574,     0,   588,
     586,   470,   644,   472,   173,     0,     0,     0,   535,   461,
     475,   114,   442,   116,   228,     0,    57,    58,     0,   247,
     245,     0,     0,     0,     0,   246,     0,     0,     0,     0,
       0,     0,     0,   206,   204,     0,     0,     0,     0,   221,
     216,   213,   212,   214,   215,   227,   207,     0,    65,    66,
      63,   219,    64,   220,   222,     0,   211,     0,   208,   301,
       0,     0,   508,   523,   454,   498,   443,     0,   488,   489,
       0,   482,   483,   481,     0,   604,   607,     0,   610,   600,
     578,     0,   614,     0,   621,     0,   628,     0,     0,   634,
       0,   637,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,   665,   655,   657,     0,   670,
       0,     0,     0,   657,     0,     0,     0,     0,    29,     0,
      27,     0,   381,     0,     0,   330,   327,   329,     0,   389,
       0,   277,     0,   112,     0,     0,   404,   403,     0,     0,
     405,   409,   355,   356,   368,   369,   366,   367,     0,   398,
       0,   386,     0,   422,   423,   357,   358,   373,   374,   375,
     376,     0,     0,   371,   372,   370,   364,   365,   360,   359,
     361,   362,   363,     0,     0,     0,   336,     0,     0,     0,
       0,   391,     0,     0,   462,     0,     0,   474,     0,   473,
       0,   476,     0,   465,     0,     0,     0,   251,     0,   248,
       0,     0,   235,     0,     0,   225,     0,     0,    51,    71,
       0,   256,   253,   283,   230,   231,     0,   223,   224,     0,
      68,   210,   217,   218,   250,   255,   261,     0,   209,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   521,     0,     0,
     534,     0,   442,   487,     0,   491,     0,   495,   301,   605,
     608,   581,   611,   601,   579,   615,   617,   622,   624,   629,
     631,   467,   635,   469,   638,     0,   242,     0,     0,     0,
       0,   394,     0,     0,   395,     0,   425,   291,   426,   427,
     428,   429,   430,     0,     0,   658,     0,     0,   657,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   668,   675,   676,     0,    30,    33,     0,     0,     0,
     112,     0,     0,     0,   390,     0,     0,     0,     0,   289,
       0,   115,   400,     0,   406,     0,     0,     0,   384,     0,
       0,   410,   414,     0,     0,   387,     0,     0,     0,   337,
       0,   339,   382,   392,   463,     0,   537,   538,   477,   480,
     479,    72,     0,   252,   249,     0,     0,     0,   236,    73,
      74,    49,    50,   257,   254,   284,   226,    54,     0,   229,
     311,   312,   314,   313,   315,   305,   306,   307,   316,   317,
       0,     0,   303,   304,   318,   319,   308,   309,   310,     0,
       0,   302,   509,     0,   449,   444,     0,   490,   493,   301,
     494,   618,   625,   632,   238,   243,     0,   274,   272,   263,
     266,   269,     0,     0,     0,   652,   651,   657,     0,   666,
       0,   656,     0,   660,   667,   672,   671,     0,   657,     0,
     673,    28,    31,     0,     0,     0,   115,     0,   278,   287,
     288,   286,   285,     0,     0,     0,     0,     0,     0,     0,
     325,     0,     0,   411,     0,   399,     0,   385,     0,     0,
     383,   340,   536,     0,     0,     0,   258,     0,     0,     0,
      52,    53,     0,    67,    59,   322,   323,   320,   321,   111,
     450,   451,   444,   445,   446,   449,   455,   492,   239,     0,
       0,   273,     0,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,   659,     0,     0,   663,   328,   441,   331,
       0,   324,   290,   292,     0,   279,   294,     0,   440,     0,
     438,   326,   435,     0,     0,     0,   434,     0,   539,   478,
       0,    75,   205,    55,     0,    60,     0,   460,   456,   447,
       0,     0,     0,   264,   267,   270,   396,     0,   433,   677,
       0,   661,   654,   653,   677,     0,     0,   334,     0,   280,
     401,   407,     0,   439,   437,   436,    70,    54,     0,    69,
       0,   448,   458,   452,   275,     0,   397,     0,     0,   662,
       0,   664,   332,     0,   281,   402,   408,   412,    56,   247,
       0,    61,    65,    66,    63,    64,    62,     0,     0,     0,
       0,     0,     0,   335,   413,     0,   248,     0,   253,   457,
       0,   276,   678,   679,   680,   249,   254,   459
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -906,  -906,  -906,  -906,  -906,   393,   839,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,   869,  -906,   792,  -906,  -906,   841,
    -906,  -906,  -906,  -235,  -906,  -906,  -906,  -234,  -906,  -906,
    -906,  -906,  -906,  -906,  -906,  -906,   672,  -906,  -906,   849,
     -63,  -906,  -906,   329,    98,  -385,  -345,  -464,  -906,  -906,
    -906,  -905,  -906,  -906,  -226,  -906,   -26,  -554,   -12,  -906,
    -906,  -906,  -906,  -906,  -230,  -227,  -225,  -906,  -224,  -906,
    -906,   921,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,  -545,  -906,  -906,   -98,  -906,    69,
    -576,  -906,  -444,  -906,  -906,  -906,  -760,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,  -906,   497,  -906,  -906,  -906,  -906,
    -906,  -906,  -906,  -139,  -135,  -906,  -137,  -906,  -906,  -906,
    -906,  -906,   357,  -906,   280,  -906,  -417,   574,  -556,  -560,
     304,  -906,  -906,  -422,  -906,  -906,   888,  -906,  -906,  -906,
     486,   119,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,  -906,  -906,  -461,  -117,  -906,   579,
    -906,   589,  -906,  -906,  -906,  -906,  -261,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,
    -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,  -906,
    -906,   -97,  -906,  -906,  -906,  -906,  -906,   581,  -677,  -489,
    -657,  -906,    59,   252,   441,  -906,  -906,  -906,  -197,  -906
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   125,    45,    14,   143,   149,   510,   401,   886,
     402,   691,    15,    16,    38,    39,    75,    17,    34,    35,
     595,   596,  1052,  1053,   597,  1116,  1148,   598,   599,   948,
     600,   601,   602,   603,   604,  1047,   150,   151,    30,    31,
      32,    54,    55,    56,    57,    18,   324,   448,   228,    19,
      94,   229,    95,   135,   403,   605,   449,   606,   404,   607,
     483,   653,  1068,   405,   608,   609,   610,   611,   612,   613,
     614,   615,   616,   406,   658,  1072,   659,  1073,   661,  1074,
     407,   656,  1071,   408,   700,  1096,   409,   899,   900,   703,
     410,   411,   768,   618,   412,   413,   697,   414,   889,  1136,
     890,  1163,   415,   749,  1041,   701,  1026,  1165,  1028,  1166,
    1103,  1184,   417,   444,  1065,  1152,  1062,   976,   624,   822,
    1150,  1178,   445,   446,   478,   479,   268,   269,   867,   569,
     634,   463,   342,   343,   235,   338,    65,   104,    21,   140,
     334,    42,    76,    78,    22,    98,   137,    23,   454,   233,
     234,    63,   101,    24,   138,   336,   570,   418,   333,   191,
     192,   198,   193,   350,   834,   465,   194,   348,   833,   345,
     829,   346,   830,   347,   832,   351,   835,   352,   981,   353,
     837,   354,   982,   355,   839,   356,   983,   357,   842,   358,
     844,   480,    25,    47,   127,    26,    46,   419,   676,   677,
     678,   420,   679,   508,   680,   421,   509,   422,  1158,   423
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,   237,   190,   326,   432,   617,   754,   472,   643,   474,
     645,   476,   647,   752,   762,   784,   785,   683,  -110,   563,
     872,   892,   195,   130,   521,   767,   879,   105,   106,   864,
      71,   797,   671,   877,   625,   671,   781,   932,   895,   566,
      58,    58,   670,    43,   681,   522,   523,   896,  1094,    40,
     532,   640,   425,   534,   535,   534,   535,    48,   743,   744,
      87,    88,    89,  1063,    72,     2,    27,    28,   433,  -616,
     451,     3,   621,   641,  -616,    58,    58,    58,    58,    44,
     827,   519,  1063,  1118,    33,    49,   745,   757,   897,   121,
     522,   523,  -616,   898,     4,   865,     5,   746,     6,   144,
     145,   622,   757,   757,     7,   798,   442,   189,   434,   847,
     452,   236,   866,     8,   325,   520,  1064,   765,    83,     9,
     129,   504,   506,   555,   556,   555,   556,   267,   747,   748,
     451,    83,   567,   526,   527,  1064,    40,   759,    10,    90,
     265,   532,   891,   533,   534,   535,   536,    64,   190,   568,
     537,   758,   759,   759,   760,   443,    50,   761,    67,    11,
      77,    87,    88,    89,    91,   107,   893,   562,   648,   108,
     817,   109,   110,   339,   631,    41,   758,   871,   526,   527,
     758,    67,    51,   632,   758,   200,   532,    73,   533,   534,
     535,   536,   426,   796,  1001,   537,  1078,   122,    74,   456,
     630,   819,   552,   553,   554,   111,    29,    52,   929,   820,
     427,   998,   933,   757,   555,   556,    53,   893,   757,   428,
     442,   949,   866,    66,   633,   236,   943,  1081,   190,   190,
     190,   190,   893,   663,   190,   563,   190,   146,   190,   821,
     190,   147,   230,   148,   110,  1177,   153,  1084,   464,   466,
     467,   468,   154,   189,   471,    12,   473,    50,   475,   555,
     556,    13,  1110,   759,    36,   190,   190,   720,   759,   155,
     231,   914,   978,  1190,   460,    92,   571,   461,   721,   572,
     462,    93,   915,    51,   156,    37,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,    67,   190,   994,
     241,    59,    60,   493,    61,    87,   893,    89,   325,   893,
     190,   265,  1007,   189,   189,   189,   189,   893,   561,   189,
    1079,   189,   457,   189,   190,   189,    68,   242,   893,   189,
     573,  1085,    62,   894,    50,   770,   923,   637,   901,    69,
      50,  1031,    40,   779,   985,   522,   523,   188,   893,   693,
     189,   189,  1044,    50,   694,  1101,  1162,   190,    83,   997,
      51,    84,    85,   514,  1055,  1056,    51,  1045,    80,  1008,
     382,   383,   384,  1057,  1058,  1128,   114,   852,   853,    51,
     855,   695,    50,  1183,   870,   707,   711,   340,  1023,   522,
     523,   693,    81,    90,   482,   567,   131,   132,   133,   134,
    -623,   431,  -630,   189,   341,  -623,    82,  -630,    51,  1017,
     649,   918,   568,  1024,   651,   189,   327,  1025,  1059,    74,
     328,   623,   919,  -623,  1082,  -630,  -333,    86,  1077,   189,
    1069,  -333,   650,   526,   527,   119,   652,   329,   330,   331,
     332,   532,    96,   116,   534,   535,   536,  1049,    97,  -333,
     537,    99,  1070,   117,   655,   102,   118,   100,  1050,  1051,
     123,   103,   189,  1060,   265,   875,   124,   120,   635,  1061,
     876,   399,   783,  1115,   325,   524,   525,   526,   527,   686,
     265,   687,   522,   523,   638,   532,   696,   533,   534,   535,
     536,   265,   141,   142,   537,   639,   538,   539,   265,   265,
     190,   265,   642,   644,   265,   646,   265,   876,   984,   880,
     988,   190,   126,   190,   555,   556,   190,   139,   931,   190,
     831,   190,   190,   128,   190,  1016,   265,   939,   136,   265,
    1141,   841,  1090,  1167,   238,   239,   845,    72,   196,   850,
    1153,   197,   199,   947,   550,   551,   552,   553,   554,   141,
     142,   688,   689,   238,   239,   240,    87,   232,   555,   556,
     522,   523,   236,   243,   244,   245,   321,   246,   524,   525,
     526,   527,   528,   247,   248,   529,   530,   531,   532,   249,
     533,   534,   535,   536,   617,   250,   265,   537,   251,   538,
     539,   252,   253,   266,   912,   540,   541,   542,   267,   254,
     322,   543,   323,   335,   349,   189,   325,   424,  1015,   337,
     435,   437,   438,   436,   440,   439,   189,   441,   189,   447,
     453,   189,   450,   190,   189,   455,   189,   189,   459,   189,
     544,   458,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   469,   928,   486,   487,   524,   525,   526,   527,
     528,   555,   556,   529,   530,   531,   532,   488,   533,   534,
     535,   536,   489,   975,   -62,   537,   490,   538,   539,   492,
     491,   493,   494,   540,    12,   522,   523,   497,   498,   499,
     500,   564,   501,   502,   628,   629,   558,   518,   559,   636,
     565,  1046,   626,   702,   654,   718,   755,   756,   776,   774,
     786,   775,   787,   788,   790,   791,   792,   793,   794,   795,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     255,   824,   433,   836,   838,   840,   870,   873,   189,   555,
     556,   416,   883,   874,   885,   887,   891,   888,   902,   903,
     430,   904,   921,   926,   905,  1097,   256,   257,   906,   907,
     917,   797,   190,   799,   800,   801,   802,   803,   804,   805,
     806,   524,   525,   526,   527,   528,   807,   808,   529,   530,
     531,   532,   809,   533,   534,   535,   536,   927,   937,  1095,
     537,   810,   538,   539,   811,   812,   941,    58,   540,   541,
     542,   813,   814,   815,   543,   940,   942,   945,   946,   974,
     977,   986,   258,  1113,   993,   631,   259,  1000,  1009,  1002,
    1012,  1003,  1005,   255,   632,  1010,  1019,  1020,  1021,  1022,
    1033,  1043,   816,   544,  1034,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,  1066,  1108,  1036,  1039,   256,
     257,  1091,  1122,  1075,   555,   556,  1137,  1151,  1080,  1117,
     495,   496,  1135,  1083,  1086,   633,   260,   189,  1093,  1130,
     261,   507,  1131,   262,  1146,  1149,  1155,  1159,  1147,   511,
     512,   513,  1161,   515,   516,   517,  1180,  1181,   263,  1189,
    1182,  1193,  1194,   690,   825,   112,    70,   264,   152,   190,
     115,   190,  1168,   344,  1171,   258,   113,   973,  1172,   259,
    1048,  1173,    20,  1174,  1175,  1092,   992,  1119,  1120,   763,
     843,  1099,   481,    79,   826,  1006,   620,  1160,   882,   470,
     484,     0,   485,     0,     0,     0,   619,   684,     0,     0,
       0,     0,     0,     0,     0,   627,     0,     0,     0,     0,
       0,     0,     0,     0,  1197,     0,     0,     0,     0,   260,
     522,   523,     0,   261,     0,   857,   262,     0,     0,     0,
       0,     0,     0,     0,     0,   657,   190,     0,   660,     0,
     662,   263,     0,     0,   664,   665,   666,   667,   668,   669,
     264,   675,     0,   675,     0,     0,  1142,     0,     0,     0,
       0,     0,     0,     0,   189,     0,   189,     0,     0,     0,
       0,   712,   713,     0,     0,   714,   715,   716,   717,     0,
     719,     0,   722,   723,   724,   725,   726,   727,   728,   729,
     730,   732,   733,   734,   735,   736,   737,   738,   739,   740,
     741,   742,     0,   750,     0,     0,     0,     0,   526,   527,
     522,   523,   753,     0,     0,     0,   532,     0,   533,   534,
     535,   536,   764,     0,     0,   537,   769,     0,     0,   772,
     773,     0,     0,     0,     0,   778,     0,   782,     0,     0,
       0,   189,     0,     0,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,   818,     0,
       0,     0,     0,     0,   696,   550,   551,   552,   553,   554,
       0,   828,   256,   257,     0,     0,     0,   256,   257,   555,
     556,     0,     0,     0,     0,     0,   524,   525,   526,   527,
     528,   696,   849,   529,   530,   531,   532,     0,   533,   534,
     535,   536,     0,     0,     0,   537,     0,   538,   539,     0,
     511,   517,     0,   540,   541,   542,     0,     0,     0,   517,
       0,     0,   881,   507,     0,     0,   884,     0,   258,     0,
       0,     0,   259,   258,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   544,     0,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
     556,     0,     0,     0,     0,     0,     0,     0,   522,   523,
       0,   916,   260,     0,     0,   920,   261,   260,   989,   262,
       0,   261,   925,   990,   262,     0,     0,     0,     0,   930,
       0,     0,     0,   934,   263,     0,     0,     0,   938,   263,
       0,     0,     0,   264,     0,     0,     0,   944,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   950,   951,   952,   953,
     954,   955,   956,   957,   958,   959,   962,   963,   964,   965,
     966,   967,   968,   971,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   979,   524,   525,   526,   527,   528,   522,
     523,   529,   530,   531,   532,     0,   533,   534,   535,   536,
       0,     0,     0,   537,     0,   538,   539,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   995,   996,     0,   999,     0,   522,   523,
       0,  1004,     0,   675,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1013,  1014,     0,     0,     0,     0,
    1018,   548,   549,   550,   551,   552,   553,   554,     0,     0,
    1027,     0,  1029,     0,  1032,     0,     0,   555,   556,     0,
    1035,     0,     0,     0,  1038,   524,   525,   526,   527,   528,
       0,     0,   529,   530,   531,   532,     0,   533,   534,   535,
     536,     0,   962,   971,   537,     0,   538,   539,     0,     0,
       0,     0,   540,   541,   542,  1054,     0,     0,   543,     0,
       0,     0,     0,     0,   524,   525,   526,   527,   528,     0,
       0,   529,   530,   531,   532,     0,   533,   534,   535,   536,
       0,     0,     0,   537,     0,   538,   539,   544,     0,   545,
     546,   547,   548,   549,   550,   551,   552,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   555,   556,
       0,     0,     0,     0,     0,     0,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,     0,     0,
       0,  1104,     0,  1105,     0,     0,  1107,   555,   556,     0,
       0,  1109,     0,     0,  1111,   619,     0,     0,     0,  1114,
       0,     0,   201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1121,     0,     0,  1123,
    1124,  1125,  1127,     0,     0,  1129,     0,     0,  1132,  1133,
       0,  1134,   202,     0,   203,     0,   204,   205,   206,   207,
     208,  1139,   209,   210,   211,   212,   213,   214,   215,   216,
     217,     0,   218,   219,   220,     0,     0,   221,   222,   223,
     224,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   574,     0,     0,   225,   226,     3,     0,
     575,   576,   577,     0,   578,  1164,   359,   360,   361,   362,
     363,     0,     0,     0,     0,  1176,     0,   579,   364,   580,
     581,     0,  1179,     0,     0,     0,     0,     0,     0,   582,
     365,     0,   583,     0,   584,   366,  1186,  1188,   367,     0,
       8,   368,   585,     0,   586,   369,   227,  1192,   587,   588,
       0,     0,  1195,     0,  1196,   589,   157,   158,   159,     0,
     161,   162,   163,   164,   165,   371,   167,   168,   169,   170,
     171,   172,   173,   174,   175,     0,   177,   178,   179,     0,
       0,   182,   183,   184,   185,     0,     0,   372,   590,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     373,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   591,   592,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   375,   376,   377,   378,   379,     0,   380,     0,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
      51,   391,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   392,   393,   394,
       0,   395,     0,     0,   396,   397,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,   399,   400,   593,   325,
     594,   574,     0,     0,     0,     0,     3,     0,   575,   576,
     577,     0,   578,     0,   359,   360,   361,   362,   363,     0,
       0,     0,     0,     0,     0,   579,   364,   580,   581,     0,
       0,     0,     0,     0,     0,     0,     0,   582,   365,     0,
     583,     0,   584,   366,     0,     0,   367,     0,     8,   368,
     585,     0,   586,   369,     0,     0,   587,   588,     0,     0,
       0,     0,     0,   589,   157,   158,   159,     0,   161,   162,
     163,   164,   165,   371,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,   177,   178,   179,     0,     0,   182,
     183,   184,   185,     0,     0,   372,   590,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   373,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   591,   592,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,   255,     0,     0,     0,     0,
     375,   376,   377,   378,   379,     0,   380,     0,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,    51,   391,
       0,   256,   257,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   392,   393,   394,     0,   395,
       0,     0,   396,   397,     0,     0,     0,   359,   360,   361,
     362,   363,   398,     0,   399,   400,   593,   325,  1112,   364,
       0,     0,     0,     0,     0,   255,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,   366,   258,     0,   367,
       0,   259,   368,     0,   671,     0,   369,     0,     0,     0,
       0,   256,   257,     0,     0,     0,   370,   157,   158,   159,
       0,   161,   162,   163,   164,   165,   371,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,   177,   178,   179,
       0,     0,   182,   183,   184,   185,     0,     0,   372,     0,
       0,   260,     0,     0,     0,   261,     0,   991,   262,     0,
       0,   373,   374,     0,     0,     0,     0,   258,     0,     0,
       0,   259,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   375,   376,   377,   378,   379,     0,   380,
       0,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   672,   391,     0,     0,     0,     0,     0,     0,     0,
       0,   260,     0,     0,     0,   261,     0,     0,   673,   393,
     394,     0,   395,     0,     0,   396,   397,   359,   360,   361,
     362,   363,     0,   263,     0,   674,     0,   399,   400,   364,
     325,     0,   264,     0,     0,   255,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,   366,     0,     0,   367,
       0,     0,   368,     0,     0,     0,   369,     0,     0,     0,
       0,   256,   257,     0,     0,     0,   370,   157,   158,   159,
       0,   161,   162,   163,   164,   165,   371,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,   177,   178,   179,
       0,     0,   182,   183,   184,   185,     0,     0,   372,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   373,   374,     0,     0,     0,     0,   258,     0,     0,
       0,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   375,   376,   377,   378,   379,     0,   380,
       0,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   672,   391,     0,     0,     0,     0,     0,     0,     0,
       0,   260,     0,     0,     0,   261,     0,     0,   673,   393,
     394,     0,   395,     0,     0,   396,   397,   359,   360,   361,
     362,   363,     0,   263,     0,   682,     0,   399,   400,   364,
     325,     0,   264,     0,   255,   425,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,   366,     0,     0,   367,
       0,     0,   368,     0,     0,     0,   369,     0,     0,     0,
     256,   257,     0,     0,     0,     0,   370,   157,   158,   159,
       0,   161,   162,   163,   164,   165,   371,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,   177,   178,   179,
       0,     0,   182,   183,   184,   185,     0,     0,   372,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   373,   374,     0,     0,     0,   258,     0,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   375,   376,   377,   378,   379,     0,   380,
       0,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,    51,   391,     0,     0,     0,     0,     0,     0,     0,
     260,     0,     0,     0,   261,   426,  1089,   262,   392,   393,
     394,     0,   395,     0,     0,   396,   397,     0,     0,     0,
       0,     0,   263,   427,     0,   398,     0,   399,   400,     0,
     325,   264,   428,   359,   360,   361,   362,   363,     0,     0,
       0,     0,     0,     0,  1169,   364,   580,     0,     0,     0,
     255,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,   366,     0,     0,   367,     0,     0,   368,   585,
       0,     0,   369,     0,     0,     0,   256,   257,     0,     0,
       0,     0,   370,   157,   158,   159,     0,   161,   162,   163,
     164,   165,   371,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   177,   178,   179,     0,     0,   182,   183,
     184,   185,     0,     0,   372,  1170,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,   374,     0,
       0,     0,   258,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   375,
     376,   377,   378,   379,     0,   380,     0,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,    51,   391,     0,
     359,   360,   361,   362,   363,     0,   260,     0,     0,     0,
     261,     0,   364,   262,   392,   393,   394,     0,   395,     0,
       0,   396,   397,     0,   365,     0,     0,     0,   263,   366,
       0,   398,   367,   399,   400,   368,   325,   264,     0,   369,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   370,
     157,   158,   159,     0,   161,   162,   163,   164,   165,   371,
     167,   168,   169,   170,   171,   172,   173,   174,   175,     0,
     177,   178,   179,     0,     0,   182,   183,   184,   185,     0,
       0,   372,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   373,   374,     0,     0,     0,     0,
       0,     0,     0,   766,     0,     0,     0,     0,     0,   591,
     592,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,   375,   376,   377,   378,
     379,     0,   380,     0,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,    51,   391,     0,   359,   360,   361,
     362,   363,     0,     0,     0,     0,     0,     0,     0,   364,
       0,   392,   393,   394,     0,   395,     0,     0,   396,   397,
       0,   365,     0,     0,     0,     0,   366,     0,   398,   367,
     399,   400,   368,   325,     0,     0,   369,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   370,   157,   158,   159,
       0,   161,   162,   163,   164,   165,   371,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,   177,   178,   179,
       0,     0,   182,   183,   184,   185,     0,     0,   372,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   373,   374,     0,     0,     0,     0,     0,     0,     0,
     780,     0,     0,     0,     0,     0,   591,   592,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   375,   376,   377,   378,   379,     0,   380,
       0,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,    51,   391,     0,   359,   360,   361,   362,   363,     0,
       0,     0,     0,     0,     0,     0,   364,     0,   392,   393,
     394,   255,   395,     0,     0,   396,   397,     0,   365,     0,
       0,     0,     0,   366,     0,   398,   367,   399,   400,   368,
     325,     0,     0,   369,     0,     0,     0,   256,   257,     0,
       0,     0,     0,   370,   157,   158,   159,     0,   161,   162,
     163,   164,   165,   371,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,   177,   178,   179,     0,     0,   182,
     183,   184,   185,     0,     0,   372,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   373,   374,
       0,     0,     0,   258,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
     375,   376,   377,   378,   379,     0,   380,     0,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,    51,   391,
       0,     0,     0,     0,     0,     0,     0,   260,     0,     0,
       0,   261,     0,  1140,   262,   392,   393,   394,     0,   395,
       0,     0,   396,   397,   359,   360,   361,   362,   363,   263,
     698,     0,   398,   699,   399,   400,   364,   325,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   365,     0,
       0,     0,     0,   366,     0,     0,   367,     0,     0,   368,
       0,     0,     0,   369,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   370,   157,   158,   159,     0,   161,   162,
     163,   164,   165,   371,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,   177,   178,   179,     0,     0,   182,
     183,   184,   185,     0,     0,   372,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   373,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   591,   592,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
     375,   376,   377,   378,   379,     0,   380,     0,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,    51,   391,
       0,   359,   360,   361,   362,   363,     0,     0,     0,     0,
       0,     0,     0,   364,     0,   392,   393,   394,     0,   395,
       0,     0,   396,   397,     0,   365,     0,     0,     0,     0,
     366,     0,   398,   367,   399,   400,   368,   325,     0,     0,
     369,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     370,   157,   158,   159,     0,   161,   162,   163,   164,   165,
     371,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,   177,   178,   179,     0,     0,   182,   183,   184,   185,
       0,     0,   372,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   373,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     960,   961,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,   375,   376,   377,
     378,   379,     0,   380,     0,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,    51,   391,     0,   359,   360,
     361,   362,   363,     0,     0,     0,     0,     0,     0,     0,
     364,     0,   392,   393,   394,     0,   395,     0,     0,   396,
     397,     0,   365,     0,     0,     0,     0,   366,     0,   398,
     367,   399,   400,   368,   325,     0,     0,   369,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   370,   157,   158,
     159,     0,   161,   162,   163,   164,   165,   371,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,   177,   178,
     179,     0,     0,   182,   183,   184,   185,     0,     0,   372,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   373,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   969,   970,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,   375,   376,   377,   378,   379,     0,
     380,     0,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,    51,   391,     0,   359,   360,   361,   362,   363,
       0,     0,     0,     0,     0,     0,     0,   364,     0,   392,
     393,   394,     0,   395,     0,     0,   396,   397,     0,   365,
       0,     0,     0,     0,   366,     0,   398,   367,   399,   400,
     368,   325,     0,     0,   369,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,   157,   158,   159,     0,   161,
     162,   163,   164,   165,   371,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   177,   178,   179,     0,     0,
     182,   183,   184,   185,     0,     0,   372,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   373,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,   375,   376,   377,   378,   379,     0,   380,     0,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,    51,
     391,     0,   359,   360,   361,   362,   363,     0,     0,   731,
       0,     0,     0,     0,   364,     0,   392,   393,   394,     0,
     395,     0,     0,   396,   397,     0,   365,     0,     0,     0,
       0,   366,   429,   398,   367,   399,   400,   368,   325,     0,
       0,   369,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   370,   157,   158,   159,     0,   161,   162,   163,   164,
     165,   371,   167,   168,   169,   170,   171,   172,   173,   174,
     175,     0,   177,   178,   179,     0,     0,   182,   183,   184,
     185,     0,     0,   372,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   373,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,   375,   376,
     377,   378,   379,     0,   380,     0,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,    51,   391,     0,   359,
     360,   361,   362,   363,     0,     0,     0,     0,     0,     0,
       0,   364,     0,   392,   393,   394,     0,   395,     0,     0,
     396,   397,     0,   365,     0,     0,     0,     0,   366,     0,
     398,   367,   399,   400,   368,   325,     0,     0,   369,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   370,   157,
     158,   159,     0,   161,   162,   163,   164,   165,   371,   167,
     168,   169,   170,   171,   172,   173,   174,   175,     0,   177,
     178,   179,     0,     0,   182,   183,   184,   185,     0,     0,
     372,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   373,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,   375,   376,   377,   378,   379,
       0,   380,     0,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,    51,   391,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     392,   393,   394,     0,   395,     0,     0,   396,   397,   359,
     360,   361,   362,   363,     0,     0,     0,   398,   751,   399,
     400,   364,   325,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   365,     0,     0,     0,     0,   366,     0,
       0,   367,     0,     0,   368,     0,     0,     0,   369,     0,
       0,     0,     0,     0,   771,     0,     0,     0,   370,   157,
     158,   159,     0,   161,   162,   163,   164,   165,   371,   167,
     168,   169,   170,   171,   172,   173,   174,   175,     0,   177,
     178,   179,     0,     0,   182,   183,   184,   185,     0,     0,
     372,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   373,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,   375,   376,   377,   378,   379,
       0,   380,     0,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,    51,   391,     0,   359,   360,   361,   362,
     363,     0,     0,     0,     0,     0,     0,     0,   364,     0,
     392,   393,   394,     0,   395,     0,     0,   396,   397,     0,
     365,     0,     0,     0,     0,   366,     0,   398,   367,   399,
     400,   368,   325,     0,     0,   369,     0,     0,   777,     0,
       0,     0,     0,     0,     0,   370,   157,   158,   159,     0,
     161,   162,   163,   164,   165,   371,   167,   168,   169,   170,
     171,   172,   173,   174,   175,     0,   177,   178,   179,     0,
       0,   182,   183,   184,   185,     0,     0,   372,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     373,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,   375,   376,   377,   378,   379,     0,   380,     0,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
      51,   391,     0,   359,   360,   361,   362,   363,     0,     0,
       0,     0,     0,     0,     0,   364,     0,   392,   393,   394,
       0,   395,     0,     0,   396,   397,     0,   365,     0,     0,
       0,     0,   366,     0,   398,   367,   399,   400,   368,   325,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   157,   158,   159,     0,   161,   162,   163,
     164,   165,   371,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   177,   178,   179,     0,     0,   182,   183,
     184,   185,     0,     0,   372,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   375,
     376,   377,   378,   379,     0,   380,     0,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,    51,   391,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   392,   393,   394,     0,   395,     0,
       0,   396,   397,   359,   360,   361,   362,   363,     0,     0,
       0,   398,   846,   399,   400,   364,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,   366,     0,     0,   367,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   157,   158,   159,     0,   161,   162,   163,
     164,   165,   371,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   177,   178,   179,     0,     0,   182,   183,
     184,   185,     0,     0,   372,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   375,
     376,   377,   378,   379,     0,   380,     0,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,    51,   391,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   848,     0,   392,   393,   394,     0,   395,     0,
       0,   396,   397,   359,   360,   361,   362,   363,     0,     0,
       0,   398,     0,   399,   400,   364,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,   366,     0,     0,   367,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   157,   158,   159,     0,   161,   162,   163,
     164,   165,   371,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   177,   178,   179,     0,     0,   182,   183,
     184,   185,     0,     0,   372,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   375,
     376,   377,   378,   379,     0,   380,     0,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,    51,   391,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   392,   393,   394,     0,   395,     0,
       0,   396,   397,   359,   360,   361,   362,   363,     0,     0,
       0,   398,   868,   399,   400,   364,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,   366,     0,     0,   367,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   157,   158,   159,     0,   161,   162,   163,
     164,   165,   371,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   177,   178,   179,     0,     0,   182,   183,
     184,   185,     0,     0,   372,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   375,
     376,   377,   378,   379,     0,   380,     0,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,    51,   391,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   392,   393,   394,     0,   395,     0,
       0,   396,   397,   359,   360,   361,   362,   363,     0,     0,
       0,   398,   878,   399,   400,   364,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,   366,     0,     0,   367,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   157,   158,   159,     0,   161,   162,   163,
     164,   165,   371,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   177,   178,   179,     0,     0,   182,   183,
     184,   185,     0,     0,   372,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   375,
     376,   377,   378,   379,     0,   380,     0,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,    51,   391,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   392,   393,   394,     0,   395,     0,
       0,   396,   397,   359,   360,   361,   362,   363,     0,     0,
       0,   398,  1030,   399,   400,   364,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,   366,     0,     0,   367,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   157,   158,   159,     0,   161,   162,   163,
     164,   165,   371,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   177,   178,   179,     0,     0,   182,   183,
     184,   185,     0,     0,   372,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   375,
     376,   377,   378,   379,     0,   380,     0,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,    51,   391,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   392,   393,   394,     0,   395,     0,
       0,   396,   397,   359,   360,   361,   362,   363,     0,     0,
       0,   398,  1076,   399,   400,   364,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,   366,     0,     0,   367,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   157,   158,   159,     0,   161,   162,   163,
     164,   165,   371,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   177,   178,   179,     0,     0,   182,   183,
     184,   185,     0,     0,   372,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   375,
     376,   377,   378,   379,     0,   380,     0,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,    51,   391,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   392,   393,   394,     0,   395,     0,
       0,   396,   397,   359,   360,   361,   362,   363,     0,     0,
       0,   398,  1126,   399,   400,   364,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,   366,     0,     0,   367,     0,     0,   368,     0,
       0,     0,   369,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   370,   157,   158,   159,     0,   161,   162,   163,
     164,   165,   371,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   177,   178,   179,     0,     0,   182,   183,
     184,   185,     0,     0,   372,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   373,   374,     0,
       0,     0,     0,     0,     0,     0,  1138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,   375,
     376,   377,   378,   379,     0,   380,     0,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,    51,   391,     0,
     359,   360,   361,   362,   363,     0,     0,     0,     0,     0,
       0,     0,   364,     0,   392,   393,   394,     0,   395,     0,
       0,   396,   397,     0,   365,     0,     0,     0,     0,   366,
       0,   398,   367,   399,   400,   368,   325,     0,     0,   369,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   370,
     157,   158,   159,     0,   161,   162,   163,   164,   165,   371,
     167,   168,   169,   170,   171,   172,   173,   174,   175,     0,
     177,   178,   179,     0,     0,   182,   183,   184,   185,     0,
       0,   372,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   373,   374,     0,     0,     0,     0,
       0,     0,     0,  1185,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,   375,   376,   377,   378,
     379,     0,   380,     0,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,    51,   391,     0,   359,   360,   361,
     362,   363,     0,     0,     0,     0,     0,     0,     0,   364,
       0,   392,   393,   394,     0,   395,     0,     0,   396,   397,
       0,   365,     0,     0,     0,     0,   366,     0,   398,   367,
     399,   400,   368,   325,     0,     0,   369,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   370,   157,   158,   159,
       0,   161,   162,   163,   164,   165,   371,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,   177,   178,   179,
       0,     0,   182,   183,   184,   185,     0,     0,   372,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   373,   374,     0,     0,     0,     0,     0,     0,     0,
    1187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,   375,   376,   377,   378,   379,     0,   380,
       0,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,    51,   391,     0,   359,   360,   361,   362,   363,     0,
       0,     0,     0,     0,     0,     0,   364,     0,   392,   393,
     394,     0,   395,     0,     0,   396,   397,     0,   365,     0,
       0,     0,     0,   366,     0,   398,   367,   399,   400,   368,
     325,     0,     0,   369,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   370,   157,   158,   159,     0,   161,   162,
     163,   164,   165,   371,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,   177,   178,   179,     0,     0,   182,
     183,   184,   185,     0,     0,   372,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   373,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     522,   523,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
     375,   376,   377,   378,   379,     0,   380,     0,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,    51,   391,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   392,   393,   394,     0,   395,
       0,     0,   396,   397,     0,     0,     0,     0,     0,     0,
       0,     0,   398,     0,   399,   400,     0,   325,   799,   800,
     801,   802,   803,   804,   805,   806,   524,   525,   526,   527,
     528,   807,   808,   529,   530,   531,   532,   935,   533,   534,
     535,   536,   522,   523,     0,   537,   810,   538,   539,   811,
     812,     0,     0,   540,   541,   542,   813,   814,   815,   543,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   936,   544,     0,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
     556,     0,     0,     0,     0,     0,   980,     0,     0,     0,
     799,   800,   801,   802,   803,   804,   805,   806,   524,   525,
     526,   527,   528,   807,   808,   529,   530,   531,   532,   935,
     533,   534,   535,   536,   522,   523,     0,   537,   810,   538,
     539,   811,   812,     0,     0,   540,   541,   542,   813,   814,
     815,   543,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   936,
     544,     0,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,   556,     0,     0,     0,     0,     0,  1067,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     524,   525,   526,   527,   528,   522,   523,   529,   530,   531,
     532,     0,   533,   534,   535,   536,     0,     0,     0,   537,
       0,   538,   539,     0,     0,     0,     0,   540,   541,   542,
       0,     0,     0,   543,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   544,     0,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   555,   556,     0,     0,     0,     0,     0,
     972,   524,   525,   526,   527,   528,   522,   523,   529,   530,
     531,   532,     0,   533,   534,   535,   536,     0,     0,     0,
     537,     0,   538,   539,     0,     0,     0,     0,   540,   541,
     542,     0,     0,     0,   543,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   544,     0,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   555,   556,     0,     0,     0,     0,
       0,  1157,   524,   525,   526,   527,   528,   522,   523,   529,
     530,   531,   532,     0,   533,   534,   535,   536,     0,     0,
       0,   537,     0,   538,   539,     0,     0,     0,     0,   540,
     541,   542,     0,     0,     0,   543,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   544,     0,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,   556,     0,     0,   557,
       0,     0,     0,   524,   525,   526,   527,   528,   522,   523,
     529,   530,   531,   532,     0,   533,   534,   535,   536,     0,
       0,     0,   537,     0,   538,   539,     0,     0,     0,     0,
     540,   541,   542,     0,     0,     0,   543,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   544,     0,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   555,   556,     0,     0,
     692,     0,     0,     0,   524,   525,   526,   527,   528,   522,
     523,   529,   530,   531,   532,     0,   533,   534,   535,   536,
       0,     0,     0,   537,     0,   538,   539,     0,     0,     0,
       0,   540,   541,   542,     0,     0,     0,   543,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   544,     0,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,   556,     0,
       0,   823,     0,     0,     0,   524,   525,   526,   527,   528,
     522,   523,   529,   530,   531,   532,     0,   533,   534,   535,
     536,     0,     0,     0,   537,     0,   538,   539,     0,     0,
       0,     0,   540,   541,   542,     0,     0,     0,   543,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   544,     0,   545,
     546,   547,   548,   549,   550,   551,   552,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   555,   556,
       0,     0,   851,     0,     0,     0,   524,   525,   526,   527,
     528,   522,   523,   529,   530,   531,   532,     0,   533,   534,
     535,   536,     0,     0,     0,   537,     0,   538,   539,     0,
       0,     0,     0,   540,   541,   542,     0,     0,     0,   543,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   544,     0,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
     556,     0,     0,   854,     0,     0,     0,   524,   525,   526,
     527,   528,   522,   523,   529,   530,   531,   532,     0,   533,
     534,   535,   536,     0,     0,     0,   537,     0,   538,   539,
       0,     0,     0,     0,   540,   541,   542,     0,     0,     0,
     543,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   544,
       0,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     555,   556,     0,     0,   856,     0,     0,     0,   524,   525,
     526,   527,   528,   522,   523,   529,   530,   531,   532,     0,
     533,   534,   535,   536,     0,     0,     0,   537,     0,   538,
     539,     0,     0,     0,     0,   540,   541,   542,     0,     0,
       0,   543,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     544,     0,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,   556,     0,     0,   858,     0,     0,     0,   524,
     525,   526,   527,   528,   522,   523,   529,   530,   531,   532,
       0,   533,   534,   535,   536,     0,     0,     0,   537,     0,
     538,   539,     0,     0,     0,     0,   540,   541,   542,     0,
       0,     0,   543,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   544,     0,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   555,   556,     0,     0,   859,     0,     0,     0,
     524,   525,   526,   527,   528,   522,   523,   529,   530,   531,
     532,     0,   533,   534,   535,   536,     0,     0,     0,   537,
       0,   538,   539,     0,     0,     0,     0,   540,   541,   542,
       0,     0,     0,   543,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   544,     0,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   555,   556,     0,     0,   860,     0,     0,
       0,   524,   525,   526,   527,   528,   522,   523,   529,   530,
     531,   532,     0,   533,   534,   535,   536,     0,     0,     0,
     537,     0,   538,   539,     0,     0,     0,     0,   540,   541,
     542,     0,     0,     0,   543,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   544,     0,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   555,   556,     0,     0,   861,     0,
       0,     0,   524,   525,   526,   527,   528,   522,   523,   529,
     530,   531,   532,     0,   533,   534,   535,   536,     0,     0,
       0,   537,     0,   538,   539,     0,     0,     0,     0,   540,
     541,   542,     0,     0,     0,   543,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   544,     0,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,   556,     0,     0,   862,
       0,     0,     0,   524,   525,   526,   527,   528,   522,   523,
     529,   530,   531,   532,     0,   533,   534,   535,   536,     0,
       0,     0,   537,     0,   538,   539,     0,     0,     0,     0,
     540,   541,   542,     0,     0,     0,   543,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   544,     0,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   555,   556,     0,     0,
     863,     0,     0,     0,   524,   525,   526,   527,   528,   522,
     523,   529,   530,   531,   532,     0,   533,   534,   535,   536,
       0,     0,     0,   537,     0,   538,   539,     0,     0,     0,
       0,   540,   541,   542,     0,     0,     0,   543,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   544,     0,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,   556,     0,
       0,   924,     0,     0,     0,   524,   525,   526,   527,   528,
     522,   523,   529,   530,   531,   532,     0,   533,   534,   535,
     536,     0,     0,     0,   537,     0,   538,   539,     0,     0,
       0,     0,   540,   541,   542,     0,     0,     0,   543,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   544,     0,   545,
     546,   547,   548,   549,   550,   551,   552,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   555,   556,
       0,     0,   987,     0,     0,     0,   524,   525,   526,   527,
     528,   522,   523,   529,   530,   531,   532,     0,   533,   534,
     535,   536,     0,     0,     0,   537,     0,   538,   539,     0,
       0,     0,     0,   540,   541,   542,     0,     0,     0,   543,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   544,     0,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
     556,     0,     0,  1042,     0,     0,     0,   524,   525,   526,
     527,   528,   522,   523,   529,   530,   531,   532,     0,   533,
     534,   535,   536,     0,     0,     0,   537,     0,   538,   539,
       0,     0,     0,     0,   540,   541,   542,     0,     0,     0,
     543,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   544,
       0,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     555,   556,     0,     0,  1087,     0,     0,     0,   524,   525,
     526,   527,   528,   522,   523,   529,   530,   531,   532,     0,
     533,   534,   535,   536,     0,     0,     0,   537,     0,   538,
     539,     0,     0,     0,     0,   540,   541,   542,     0,     0,
       0,   543,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     544,     0,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,   556,     0,     0,  1088,     0,     0,     0,   524,
     525,   526,   527,   528,   522,   523,   529,   530,   531,   532,
       0,   533,   534,   535,   536,     0,     0,     0,   537,     0,
     538,   539,     0,     0,     0,     0,   540,   541,   542,     0,
       0,     0,   543,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   544,     0,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   555,   556,     0,     0,  1098,     0,     0,     0,
     524,   525,   526,   527,   528,   522,   523,   529,   530,   531,
     532,     0,   533,   534,   535,   536,     0,     0,     0,   537,
       0,   538,   539,     0,     0,     0,     0,   540,   541,   542,
       0,     0,     0,   543,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   544,     0,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   555,   556,     0,     0,  1100,     0,     0,
       0,   524,   525,   526,   527,   528,   522,   523,   529,   530,
     531,   532,     0,   533,   534,   535,   536,     0,     0,     0,
     537,     0,   538,   539,     0,     0,     0,     0,   540,   541,
     542,     0,     0,     0,   543,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   544,     0,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   555,   556,     0,     0,  1102,     0,
       0,     0,   524,   525,   526,   527,   528,   522,   523,   529,
     530,   531,   532,     0,   533,   534,   535,   536,     0,     0,
       0,   537,     0,   538,   539,     0,     0,     0,     0,   540,
     541,   542,     0,     0,     0,   543,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   544,     0,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,   556,     0,     0,  1106,
       0,     0,     0,   524,   525,   526,   527,   528,   522,   523,
     529,   530,   531,   532,     0,   533,   534,   535,   536,     0,
       0,     0,   537,     0,   538,   539,     0,     0,     0,     0,
     540,   541,   542,     0,     0,     0,   543,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   544,     0,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   555,   556,     0,     0,
    1143,     0,     0,     0,   524,   525,   526,   527,   528,   522,
     523,   529,   530,   531,   532,     0,   533,   534,   535,   536,
       0,     0,     0,   537,     0,   538,   539,     0,     0,     0,
       0,   540,   541,   542,     0,     0,     0,   543,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   544,     0,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,   556,     0,
       0,  1144,     0,     0,     0,   524,   525,   526,   527,   528,
     522,   523,   529,   530,   531,   532,     0,   533,   534,   535,
     536,     0,     0,     0,   537,     0,   538,   539,     0,     0,
       0,     0,   540,   541,   542,     0,     0,     0,   543,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   544,     0,   545,
     546,   547,   548,   549,   550,   551,   552,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   555,   556,
       0,     0,  1145,     0,     0,     0,   524,   525,   526,   527,
     528,   522,   523,   529,   530,   531,   532,     0,   533,   534,
     535,   536,     0,     0,     0,   537,     0,   538,   539,     0,
       0,     0,     0,   540,   541,   542,     0,     0,     0,   543,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   544,     0,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
     556,     0,     0,  1154,     0,     0,     0,   524,   525,   526,
     527,   528,   522,   523,   529,   530,   531,   532,     0,   533,
     534,   535,   536,     0,     0,     0,   537,     0,   538,   539,
       0,     0,     0,     0,   540,   541,   542,     0,     0,     0,
     543,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   544,
       0,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     555,   556,     0,     0,  1156,     0,     0,     0,   524,   525,
     526,   527,   528,   522,   523,   529,   530,   531,   532,     0,
     533,   534,   535,   536,     0,     0,     0,   537,     0,   538,
     539,     0,     0,     0,     0,   540,   541,   542,     0,     0,
       0,   543,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     544,     0,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,   556,     0,     0,  1191,     0,     0,     0,   524,
     525,   526,   527,   528,   522,   523,   529,   530,   531,   532,
       0,   533,   534,   535,   536,     0,     0,     0,   537,     0,
     538,   539,     0,     0,     0,     0,   540,   541,   542,     0,
       0,     0,   543,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   544,     0,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   555,   556,   560,     0,     0,     0,     0,     0,
     524,   525,   526,   527,   528,   522,   523,   529,   530,   531,
     532,     0,   533,   534,   535,   536,     0,     0,     0,   537,
       0,   538,   539,     0,     0,     0,     0,   540,   541,   542,
       0,     0,     0,   543,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   544,     0,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   555,   556,   908,     0,     0,     0,     0,
       0,   524,   525,   526,   527,   528,   522,   523,   529,   530,
     531,   532,     0,   533,   534,   535,   536,     0,     0,     0,
     537,     0,   538,   539,     0,     0,     0,     0,   540,   541,
     542,     0,     0,     0,   543,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   544,     0,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   555,   556,   922,     0,     0,     0,
       0,     0,   524,   525,   526,   527,   528,   522,   523,   529,
     530,   531,   532,     0,   533,   534,   535,   536,     0,     0,
       0,   537,     0,   538,   539,     0,     0,     0,     0,   540,
     541,   542,     0,     0,     0,   543,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   544,     0,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,   556,  1037,     0,     0,
       0,     0,     0,   524,   525,   526,   527,   528,     0,     0,
     529,   530,   531,   532,     0,   533,   534,   535,   536,   522,
     523,     0,   537,     0,   538,   539,     0,     0,     0,     0,
     540,   541,   542,     0,     0,     0,   543,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   544,     0,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   555,   556,  1040,     0,
       0,     0,     0,     0,     0,     0,     0,   799,   800,   801,
     802,   803,   804,   805,   806,   524,   525,   526,   527,   528,
     807,   808,   529,   530,   531,   532,   935,   533,   534,   535,
     536,  -301,   270,   271,   537,   810,   538,   539,   811,   812,
       0,     0,   540,   541,   542,   813,   814,   815,   543,   272,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   936,   544,     0,   545,
     546,   547,   548,   549,   550,   551,   552,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   555,   556,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   522,   523,
     291,   292,   293,     0,     0,     0,     0,     0,   294,   295,
     296,   297,   298,     0,     0,   299,   300,   301,   302,   303,
     304,   305,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     306,     0,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,     0,     0,   317,   318,     0,     0,   704,     0,
       0,   319,   320,     0,     0,     0,   522,   523,     0,     0,
       0,     0,     0,     0,   524,   525,   526,   527,   528,     0,
       0,   529,   530,   531,   532,     0,   533,   534,   535,   536,
       0,     0,     0,   537,     0,   538,   539,     0,     0,   685,
       0,   540,   541,   542,   157,   158,   159,   543,   161,   162,
     163,   164,   165,   371,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,   177,   178,   179,     0,     0,   182,
     183,   184,   185,     0,     0,     0,   544,     0,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,     0,     0,
       0,     0,   524,   525,   526,   527,   528,   555,   556,   529,
     530,   531,   532,     0,   533,   534,   535,   536,   522,   523,
       0,   537,     0,   538,   539,     0,     0,   869,     0,   540,
     541,   542,     0,     0,     0,   543,     0,   705,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   706,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   544,     0,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,   556,     0,   708,     0,
       0,     0,     0,     0,     0,     0,   522,   523,     0,     0,
       0,     0,     0,     0,   524,   525,   526,   527,   528,     0,
       0,   529,   530,   531,   532,     0,   533,   534,   535,   536,
       0,     0,     0,   537,     0,   538,   539,     0,     0,     0,
       0,   540,   541,   542,   157,   158,   159,   543,   161,   162,
     163,   164,   165,   371,   167,   168,   169,   170,   171,   172,
     173,   174,   175,     0,   177,   178,   179,     0,     0,   182,
     183,   184,   185,     0,     0,     0,   544,   913,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,     0,     0,
       0,     0,   524,   525,   526,   527,   528,   555,   556,   529,
     530,   531,   532,     0,   533,   534,   535,   536,   522,   523,
       0,   537,     0,   538,   539,     0,     0,     0,     0,   540,
     541,   542,     0,     0,     0,   543,     0,   709,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   710,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1011,     0,     0,     0,   544,     0,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,   556,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   522,   523,     0,     0,
       0,     0,     0,     0,   524,   525,   526,   527,   528,     0,
       0,   529,   530,   531,   532,     0,   533,   534,   535,   536,
       0,     0,     0,   537,     0,   538,   539,     0,     0,     0,
       0,   540,   541,   542,     0,     0,     0,   543,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   544,     0,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,     0,     0,
       0,     0,   524,   525,   526,   527,   528,   555,   556,   529,
     530,   531,   532,     0,   533,   534,   535,   536,   522,   523,
       0,   537,     0,   538,   539,     0,     0,     0,     0,   540,
     541,   542,     0,     0,     0,  -631,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   522,   523,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   544,     0,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,   556,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   524,   525,   526,   527,   528,     0,
       0,   529,   530,   531,   532,     0,   533,   534,   535,   536,
       0,     0,     0,   537,     0,   538,   539,     0,     0,     0,
       0,   540,     0,   542,   524,   525,   526,   527,   528,   522,
     523,   529,   530,   531,   532,     0,   533,   534,   535,   536,
       0,     0,     0,   537,     0,   538,   539,     0,     0,     0,
       0,     0,   522,   523,     0,     0,     0,     0,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,   556,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
     547,   548,   549,   550,   551,   552,   553,   554,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,   556,     0,
       0,     0,     0,     0,     0,   524,   525,   526,   527,   528,
       0,     0,   529,   530,   531,   532,     0,   533,   534,   535,
     536,     0,     0,     0,   537,     0,   538,   539,   524,   525,
     526,   527,   528,     0,     0,   529,     0,     0,   532,     0,
     533,   534,   535,   536,     0,     0,     0,   537,     0,   538,
     539,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,     0,     0,
       0,   547,   548,   549,   550,   551,   552,   553,   554,   153,
       0,     0,     0,     0,     0,   154,     0,     0,   555,   556,
       0,     0,     0,     0,     0,   548,   549,   550,   551,   552,
     553,   554,   155,     0,     0,     0,     0,     0,     0,     0,
       0,   555,   556,     0,     0,     0,     0,   156,     0,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   505,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   153,     0,     0,     0,     0,     0,
     154,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   155,     0,   909,
     188,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   156,    51,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   157,   158,   159,     0,   161,
     162,   163,   164,   165,   371,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   177,   178,   179,     0,     0,
     182,   183,   184,   185,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   188,     0,     0,     0,     0,
     153,     0,     0,     0,     0,     0,   154,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   155,     0,     0,     0,     0,   910,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   156,   911,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   153,     0,     0,     0,     0,
       0,   154,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   155,     0,
       0,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   156,    51,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   188,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   477
};

static const yytype_int16 yycheck[] =
{
      12,   140,   119,   229,   265,   449,   566,   352,   472,   354,
     474,   356,   476,   558,   570,   591,   592,   506,     7,   436,
     677,   698,   119,    86,   409,   579,   683,    14,    15,    19,
      21,   122,    51,    19,   456,    51,   590,    19,   117,    48,
      52,    53,   503,   158,   505,    20,    21,   126,   131,   158,
     116,   174,    32,   119,   120,   119,   120,    33,   119,   120,
     133,   134,   135,    44,    55,     0,    18,    19,   158,   169,
     158,     6,   167,   196,   174,    87,    88,    89,    90,   194,
     634,   158,    44,    45,   152,    61,   147,   117,   167,   198,
      20,    21,   192,   172,    29,   671,    31,   158,    33,    14,
      15,   196,   117,   117,    39,   196,   144,   119,   198,   654,
     198,   149,   126,    48,   197,   192,    97,   578,   132,    54,
     193,   382,   383,   189,   190,   189,   190,   158,   189,   190,
     158,   132,   141,   108,   109,    97,   158,   167,    73,   166,
     170,   116,   158,   118,   119,   120,   121,    60,   265,   158,
     125,   166,   167,   167,   169,   193,   132,   172,   166,    94,
      41,   133,   134,   135,   191,   152,   166,   198,   169,   156,
     198,   158,   159,   236,   117,   197,   166,   196,   108,   109,
     166,   166,   158,   126,   166,   193,   116,   178,   118,   119,
     120,   121,   172,   615,   871,   125,   196,    78,   189,   338,
     461,   190,   177,   178,   179,   192,   158,   183,   762,   198,
     190,   868,   766,   117,   189,   190,   192,   166,   117,   199,
     144,   797,   126,   167,   167,   149,   780,   126,   345,   346,
     347,   348,   166,   494,   351,   652,   353,   152,   355,   624,
     357,   156,   169,   158,   159,  1150,    24,   196,   345,   346,
     347,   348,    30,   265,   351,   190,   353,   132,   355,   189,
     190,   196,   196,   167,   158,   382,   383,   147,   167,    47,
     197,   147,   826,  1178,   166,    55,   193,   169,   158,   196,
     172,    61,   158,   158,    62,   179,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,   166,   435,   864,
     166,     4,     5,   192,     7,   133,   166,   135,   197,   166,
     447,   170,   877,   345,   346,   347,   348,   166,   435,   351,
     997,   353,   191,   355,   461,   357,   169,   193,   166,   361,
     447,  1008,    35,   193,   132,   581,   193,   196,   703,   166,
     132,   906,   158,   589,   193,    20,    21,   145,   166,   141,
     382,   383,   928,   132,   146,   193,  1136,   494,   132,   868,
     158,    52,    53,   395,   960,   961,   158,   932,   158,   878,
     149,   150,   151,   969,   970,   193,   152,   658,   659,   158,
     661,   173,   132,  1163,   166,   522,   523,   141,   166,    20,
      21,   141,   158,   166,   173,   141,    87,    88,    89,    90,
     169,   199,   169,   435,   158,   174,   158,   174,   158,   191,
     174,   147,   158,   191,   174,   447,    68,   901,   191,   189,
      72,   453,   158,   192,  1000,   192,   169,   192,   993,   461,
     174,   174,   196,   108,   109,   167,   196,    89,    90,    91,
      92,   116,    55,   158,   119,   120,   121,    11,    61,   192,
     125,    55,   196,   158,   486,    55,   158,    61,    22,    23,
      55,    61,   494,    55,   170,   191,    61,   196,   174,    61,
     196,   194,   195,    11,   197,   106,   107,   108,   109,   196,
     170,   198,    20,    21,   174,   116,   518,   118,   119,   120,
     121,   170,   160,   161,   125,   174,   127,   128,   170,   170,
     637,   170,   174,   174,   170,   174,   170,   196,   174,   198,
     174,   648,   197,   650,   189,   190,   653,   197,   764,   656,
     637,   658,   659,   158,   661,   890,   170,   773,   158,   170,
     174,   648,  1016,   174,   160,   161,   653,    55,   196,   656,
    1120,   158,   197,   789,   175,   176,   177,   178,   179,   160,
     161,   162,   163,   160,   161,   162,   133,    97,   189,   190,
      20,    21,   149,   173,   173,   173,    34,   173,   106,   107,
     108,   109,   110,   192,   173,   113,   114,   115,   116,   173,
     118,   119,   120,   121,  1048,   173,   170,   125,   173,   127,
     128,   173,   173,   196,   731,   133,   134,   135,   158,   192,
      34,   139,   192,   158,   158,   637,   197,    32,   889,   198,
     169,   158,   158,   196,    21,   191,   648,   191,   650,   169,
     169,   653,   197,   760,   656,   197,   658,   659,    41,   661,
     168,   192,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   193,   760,   192,   173,   106,   107,   108,   109,
     110,   189,   190,   113,   114,   115,   116,   192,   118,   119,
     120,   121,   173,   822,     9,   125,   173,   127,   128,   173,
     192,   192,   173,   133,   190,    20,    21,   192,   192,   192,
     192,   158,   192,   192,   158,   158,   192,   195,   191,   174,
     192,   937,   191,   149,   192,   158,   192,    41,   152,   158,
      36,   196,   196,   196,     9,   196,   196,   196,   196,   196,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
      32,    41,   158,   174,   174,   174,   166,   191,   760,   189,
     190,   254,   198,   166,   160,   192,   158,   192,   173,   192,
     263,   173,     1,   158,   192,  1026,    58,    59,   192,   192,
     192,   122,   889,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   158,    13,  1025,
     125,   126,   127,   128,   129,   130,   169,   819,   133,   134,
     135,   136,   137,   138,   139,   167,   152,   195,   197,     7,
     158,   158,   114,  1049,   192,   117,   118,   158,   198,   191,
     160,   191,   191,    32,   126,   191,   158,   158,   158,   158,
     173,    41,   167,   168,   192,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   196,   158,   192,   192,    58,
      59,   193,   158,   192,   189,   190,   174,    32,   191,   196,
     373,   374,   198,   191,   191,   167,   168,   889,   191,   191,
     172,   384,   191,   175,  1110,   196,   174,   191,  1114,   392,
     393,   394,   191,   396,   397,   398,    51,   191,   190,   196,
     198,   191,   191,   510,   196,    66,    37,   199,   116,  1026,
      69,  1028,  1147,   241,  1148,   114,    67,   819,  1148,   118,
     946,  1148,     1,  1148,  1148,  1023,   857,  1062,  1065,   572,
     650,  1028,   358,    45,   630,   876,   450,  1134,   686,   350,
     361,    -1,   361,    -1,    -1,    -1,   449,   506,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   458,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1190,    -1,    -1,    -1,    -1,   168,
      20,    21,    -1,   172,    -1,   174,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   488,  1103,    -1,   491,    -1,
     493,   190,    -1,    -1,   497,   498,   499,   500,   501,   502,
     199,   504,    -1,   506,    -1,    -1,  1103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1026,    -1,  1028,    -1,    -1,    -1,
      -1,   524,   525,    -1,    -1,   528,   529,   530,   531,    -1,
     533,    -1,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,    -1,   556,    -1,    -1,    -1,    -1,   108,   109,
      20,    21,   565,    -1,    -1,    -1,   116,    -1,   118,   119,
     120,   121,   575,    -1,    -1,   125,   579,    -1,    -1,   582,
     583,    -1,    -1,    -1,    -1,   588,    -1,   590,    -1,    -1,
      -1,  1103,    -1,    -1,   597,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   621,    -1,
      -1,    -1,    -1,    -1,  1136,   175,   176,   177,   178,   179,
      -1,   634,    58,    59,    -1,    -1,    -1,    58,    59,   189,
     190,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,  1163,   655,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
     673,   674,    -1,   133,   134,   135,    -1,    -1,    -1,   682,
      -1,    -1,   685,   686,    -1,    -1,   689,    -1,   114,    -1,
      -1,    -1,   118,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,   744,   168,    -1,    -1,   748,   172,   168,   174,   175,
      -1,   172,   755,   174,   175,    -1,    -1,    -1,    -1,   762,
      -1,    -1,    -1,   766,   190,    -1,    -1,    -1,   771,   190,
      -1,    -1,    -1,   199,    -1,    -1,    -1,   780,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   799,   800,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   826,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   866,   867,    -1,   869,    -1,    20,    21,
      -1,   874,    -1,   876,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   887,   888,    -1,    -1,    -1,    -1,
     893,   173,   174,   175,   176,   177,   178,   179,    -1,    -1,
     903,    -1,   905,    -1,   907,    -1,    -1,   189,   190,    -1,
     913,    -1,    -1,    -1,   917,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,   935,   936,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,   948,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,   168,    -1,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,    -1,    -1,
      -1,  1034,    -1,  1036,    -1,    -1,  1039,   189,   190,    -1,
      -1,  1044,    -1,    -1,  1047,  1048,    -1,    -1,    -1,  1052,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1069,    -1,    -1,  1072,
    1073,  1074,  1075,    -1,    -1,  1078,    -1,    -1,  1081,  1082,
      -1,  1084,    64,    -1,    66,    -1,    68,    69,    70,    71,
      72,  1094,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,    -1,   108,   109,     6,    -1,
       8,     9,    10,    -1,    12,  1138,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,  1148,    -1,    25,    26,    27,
      28,    -1,  1155,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    -1,    42,    43,  1169,  1170,    46,    -1,
      48,    49,    50,    -1,    52,    53,   158,  1180,    56,    57,
      -1,    -1,  1185,    -1,  1187,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,   146,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,   176,   177,
      -1,   179,    -1,    -1,   182,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   192,    -1,   194,   195,   196,   197,
     198,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,
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
      -1,    -1,   132,    -1,    -1,    32,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
      -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,   176,   177,    -1,   179,
      -1,    -1,   182,   183,    -1,    -1,    -1,    14,    15,    16,
      17,    18,   192,    -1,   194,   195,   196,   197,   198,    26,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,   114,    -1,    46,
      -1,   118,    49,    -1,    51,    -1,    53,    -1,    -1,    -1,
      -1,    58,    59,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,   168,    -1,    -1,    -1,   172,    -1,   174,   175,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,   118,    -1,   190,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   199,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,    -1,    -1,   172,    -1,    -1,   175,   176,
     177,    -1,   179,    -1,    -1,   182,   183,    14,    15,    16,
      17,    18,    -1,   190,    -1,   192,    -1,   194,   195,    26,
     197,    -1,   199,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    58,    59,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,    -1,    -1,   172,    -1,    -1,   175,   176,
     177,    -1,   179,    -1,    -1,   182,   183,    14,    15,    16,
      17,    18,    -1,   190,    -1,   192,    -1,   194,   195,    26,
     197,    -1,   199,    -1,    32,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     168,    -1,    -1,    -1,   172,   172,   174,   175,   175,   176,
     177,    -1,   179,    -1,    -1,   182,   183,    -1,    -1,    -1,
      -1,    -1,   190,   190,    -1,   192,    -1,   194,   195,    -1,
     197,   199,   199,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    -1,    53,    -1,    -1,    -1,    58,    59,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,
      -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
      14,    15,    16,    17,    18,    -1,   168,    -1,    -1,    -1,
     172,    -1,    26,   175,   175,   176,   177,    -1,   179,    -1,
      -1,   182,   183,    -1,    38,    -1,    -1,    -1,   190,    43,
      -1,   192,    46,   194,   195,    49,   197,   199,    -1,    53,
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
     154,   155,   156,   157,   158,   159,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   175,   176,   177,    -1,   179,    -1,    -1,   182,   183,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   192,    46,
     194,   195,    49,   197,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   175,   176,
     177,    32,   179,    -1,    -1,   182,   183,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   192,    46,   194,   195,    49,
     197,    -1,    -1,    53,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,
      -1,   172,    -1,   174,   175,   175,   176,   177,    -1,   179,
      -1,    -1,   182,   183,    14,    15,    16,    17,    18,   190,
     190,    -1,   192,   193,   194,   195,    26,   197,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   175,   176,   177,    -1,   179,
      -1,    -1,   182,   183,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   192,    46,   194,   195,    49,   197,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,   146,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   175,   176,   177,    -1,   179,    -1,    -1,   182,
     183,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   192,
      46,   194,   195,    49,   197,    -1,    -1,    53,    -1,    -1,
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
     156,   157,   158,   159,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   175,
     176,   177,    -1,   179,    -1,    -1,   182,   183,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   192,    46,   194,   195,
      49,   197,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
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
     159,    -1,    14,    15,    16,    17,    18,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    26,    -1,   175,   176,   177,    -1,
     179,    -1,    -1,   182,   183,    -1,    38,    -1,    -1,    -1,
      -1,    43,   191,   192,    46,   194,   195,    49,   197,    -1,
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
     152,   153,   154,   155,   156,   157,   158,   159,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   175,   176,   177,    -1,   179,    -1,    -1,
     182,   183,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     192,    46,   194,   195,    49,   197,    -1,    -1,    53,    -1,
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
     155,   156,   157,   158,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,   176,   177,    -1,   179,    -1,    -1,   182,   183,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   192,   193,   194,
     195,    26,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,   146,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     175,   176,   177,    -1,   179,    -1,    -1,   182,   183,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   192,    46,   194,
     195,    49,   197,    -1,    -1,    53,    -1,    -1,    56,    -1,
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
     158,   159,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   175,   176,   177,
      -1,   179,    -1,    -1,   182,   183,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   192,    46,   194,   195,    49,   197,
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
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   176,   177,    -1,   179,    -1,
      -1,   182,   183,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   192,   193,   194,   195,    26,   197,    -1,    -1,    -1,
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
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,   175,   176,   177,    -1,   179,    -1,
      -1,   182,   183,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   192,    -1,   194,   195,    26,   197,    -1,    -1,    -1,
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
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   176,   177,    -1,   179,    -1,
      -1,   182,   183,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   192,   193,   194,   195,    26,   197,    -1,    -1,    -1,
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
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   176,   177,    -1,   179,    -1,
      -1,   182,   183,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   192,   193,   194,   195,    26,   197,    -1,    -1,    -1,
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
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   176,   177,    -1,   179,    -1,
      -1,   182,   183,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   192,   193,   194,   195,    26,   197,    -1,    -1,    -1,
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
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   176,   177,    -1,   179,    -1,
      -1,   182,   183,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   192,   193,   194,   195,    26,   197,    -1,    -1,    -1,
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
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,   176,   177,    -1,   179,    -1,
      -1,   182,   183,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   192,   193,   194,   195,    26,   197,    -1,    -1,    -1,
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
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   175,   176,   177,    -1,   179,    -1,
      -1,   182,   183,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   192,    46,   194,   195,    49,   197,    -1,    -1,    53,
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
     154,   155,   156,   157,   158,   159,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   175,   176,   177,    -1,   179,    -1,    -1,   182,   183,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   192,    46,
     194,   195,    49,   197,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   175,   176,
     177,    -1,   179,    -1,    -1,   182,   183,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   192,    46,   194,   195,    49,
     197,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,   176,   177,    -1,   179,
      -1,    -1,   182,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   192,    -1,   194,   195,    -1,   197,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    20,    21,    -1,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,   168,    -1,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,   196,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    20,    21,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
     168,    -1,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,   190,    -1,    -1,    -1,    -1,    -1,   196,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   168,    -1,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,
     196,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   168,    -1,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,   196,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   168,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   168,    -1,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,   190,    -1,    -1,
     193,    -1,    -1,    -1,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,    -1,
      -1,   193,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      20,    21,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,
      -1,    -1,   193,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    20,    21,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
     190,    -1,    -1,   193,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    20,    21,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      -1,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    20,    21,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     168,    -1,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,   190,    -1,    -1,   193,    -1,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,   190,    -1,    -1,   193,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   168,    -1,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,   190,    -1,    -1,   193,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   168,    -1,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   168,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   168,    -1,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,   190,    -1,    -1,
     193,    -1,    -1,    -1,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,    -1,
      -1,   193,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      20,    21,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,
      -1,    -1,   193,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    20,    21,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
     190,    -1,    -1,   193,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    20,    21,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      -1,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    20,    21,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     168,    -1,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,   190,    -1,    -1,   193,    -1,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,   190,    -1,    -1,   193,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   168,    -1,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,   190,    -1,    -1,   193,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   168,    -1,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   168,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    20,    21,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   168,    -1,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,   190,    -1,    -1,
     193,    -1,    -1,    -1,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,    -1,
      -1,   193,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      20,    21,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,
      -1,    -1,   193,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    20,    21,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
     190,    -1,    -1,   193,    -1,    -1,    -1,   106,   107,   108,
     109,   110,    20,    21,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      -1,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,   190,    -1,    -1,   193,    -1,    -1,    -1,   106,   107,
     108,   109,   110,    20,    21,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     168,    -1,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,   190,    -1,    -1,   193,    -1,    -1,    -1,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
     106,   107,   108,   109,   110,    20,    21,   113,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   168,    -1,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    20,    21,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   168,    -1,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,    20,    21,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   168,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    20,
      21,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   168,    -1,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    20,    21,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,   137,   138,   139,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,   168,    -1,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    20,    21,
     118,   119,   120,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     168,    -1,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,    -1,    -1,   182,   183,    -1,    -1,    18,    -1,
      -1,   189,   190,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,   131,
      -1,   133,   134,   135,    64,    65,    66,   139,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    -1,   168,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   189,   190,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    20,    21,
      -1,   125,    -1,   127,   128,    -1,    -1,   131,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   168,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,   190,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    64,    65,    66,   139,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    -1,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   189,   190,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    20,    21,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,    -1,    -1,    -1,   168,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,   190,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,    -1,    -1,
      -1,    -1,   106,   107,   108,   109,   110,   189,   190,   113,
     114,   115,   116,    -1,   118,   119,   120,   121,    20,    21,
      -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   168,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,   190,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   106,   107,   108,   109,   110,    20,
      21,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,
     172,   173,   174,   175,   176,   177,   178,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,   106,   107,
     108,   109,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,   173,   174,   175,   176,   177,
     178,   179,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,   190,    -1,    -1,    -1,    -1,    62,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    18,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,   158,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    64,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,   158,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,   158,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   201,     0,     6,    29,    31,    33,    39,    48,    54,
      73,    94,   190,   196,   204,   212,   213,   217,   245,   249,
     271,   338,   344,   347,   353,   392,   395,    18,    19,   158,
     238,   239,   240,   152,   218,   219,   158,   179,   214,   215,
     158,   197,   341,   158,   194,   203,   396,   393,    33,    61,
     132,   158,   183,   192,   241,   242,   243,   244,   258,     4,
       5,     7,    35,   351,    60,   336,   167,   166,   169,   166,
     214,    21,    55,   178,   189,   216,   342,   341,   343,   336,
     158,   158,   158,   132,   243,   243,   192,   133,   134,   135,
     166,   191,    55,    61,   250,   252,    55,    61,   345,    55,
      61,   352,    55,    61,   337,    14,    15,   152,   156,   158,
     159,   192,   206,   239,   152,   219,   158,   158,   158,   167,
     196,   198,   341,    55,    61,   202,   197,   394,   158,   193,
     240,   243,   243,   243,   243,   253,   158,   346,   354,   197,
     339,   160,   161,   205,    14,    15,   152,   156,   158,   206,
     236,   237,   216,    24,    30,    47,    62,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,   145,   258,
     357,   359,   360,   362,   366,   391,   196,   158,   361,   197,
     193,    34,    64,    66,    68,    69,    70,    71,    72,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    84,    85,
      86,    89,    90,    91,    92,   108,   109,   158,   248,   251,
     169,   197,    97,   349,   350,   334,   149,   313,   160,   161,
     162,   166,   193,   173,   173,   173,   173,   192,   173,   173,
     173,   173,   173,   173,   192,    32,    58,    59,   114,   118,
     168,   172,   175,   190,   199,   170,   196,   158,   326,   327,
      20,    21,    37,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   118,   119,   120,   126,   127,   128,   129,   130,   133,
     134,   135,   136,   137,   138,   139,   168,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   182,   183,   189,
     190,    34,    34,   192,   246,   197,   254,    68,    72,    89,
      90,    91,    92,   358,   340,   158,   355,   198,   335,   240,
     141,   158,   332,   333,   236,   369,   371,   373,   367,   158,
     363,   375,   377,   379,   381,   383,   385,   387,   389,    14,
      15,    16,    17,    18,    26,    38,    43,    46,    49,    53,
      63,    73,    95,   108,   109,   140,   141,   142,   143,   144,
     146,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   159,   175,   176,   177,   179,   182,   183,   192,   194,
     195,   208,   210,   254,   258,   263,   273,   280,   283,   286,
     290,   291,   294,   295,   297,   302,   305,   312,   357,   397,
     401,   405,   407,   409,    32,    32,   172,   190,   199,   191,
     305,   199,   366,   158,   198,   169,   196,   158,   158,   191,
      21,   191,   144,   193,   313,   322,   323,   169,   247,   256,
     197,   158,   198,   169,   348,   197,   313,   191,   192,    41,
     166,   169,   172,   331,   391,   365,   391,   391,   391,   193,
     361,   391,   246,   391,   246,   391,   246,   158,   324,   325,
     391,   327,   173,   260,   359,   397,   192,   173,   192,   173,
     173,   192,   173,   192,   173,   305,   305,   192,   192,   192,
     192,   192,   192,    12,   366,    12,   366,   305,   403,   406,
     207,   305,   305,   305,   258,   305,   305,   305,   195,   158,
     192,   245,    20,    21,   106,   107,   108,   109,   110,   113,
     114,   115,   116,   118,   119,   120,   121,   125,   127,   128,
     133,   134,   135,   139,   168,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   189,   190,   193,   192,   191,
     191,   391,   198,   326,   158,   192,    48,   141,   158,   329,
     356,   193,   196,   391,     1,     8,     9,    10,    12,    25,
      27,    28,    37,    40,    42,    50,    52,    56,    57,    63,
      96,   123,   124,   196,   198,   220,   221,   224,   227,   228,
     230,   231,   232,   233,   234,   255,   257,   259,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   292,   293,   305,
     340,   167,   196,   258,   318,   333,   191,   305,   158,   158,
     366,   117,   126,   167,   330,   174,   174,   196,   174,   174,
     174,   196,   174,   247,   174,   247,   174,   247,   169,   174,
     196,   174,   196,   261,   192,   258,   281,   305,   274,   276,
     305,   278,   305,   366,   305,   305,   305,   305,   305,   305,
     356,    51,   158,   175,   192,   305,   398,   399,   400,   402,
     404,   356,   192,   399,   404,   131,   196,   198,   162,   163,
     205,   211,   193,   141,   146,   173,   258,   296,   190,   193,
     284,   305,   149,   289,    18,   147,   158,   357,    18,   147,
     158,   357,   305,   305,   305,   305,   305,   305,   158,   305,
     147,   158,   305,   305,   305,   305,   305,   305,   305,   305,
     305,    21,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   119,   120,   147,   158,   189,   190,   303,
     305,   193,   284,   305,   329,   192,    41,   117,   166,   167,
     169,   172,   328,   322,   305,   356,   117,   257,   292,   305,
     254,    59,   305,   305,   158,   196,   152,    56,   305,   254,
     117,   257,   305,   195,   290,   290,    36,   196,   196,   305,
       9,   196,   196,   196,   196,   196,   333,   122,   196,    98,
      99,   100,   101,   102,   103,   104,   105,   111,   112,   117,
     126,   129,   130,   136,   137,   138,   167,   198,   305,   190,
     198,   245,   319,   193,    41,   196,   330,   257,   305,   370,
     372,   391,   374,   368,   364,   376,   174,   380,   174,   384,
     174,   391,   388,   324,   390,   391,   193,   284,   173,   305,
     391,   193,   366,   366,   193,   366,   193,   174,   193,   193,
     193,   193,   193,   193,    19,   290,   126,   328,   193,   131,
     166,   196,   400,   191,   166,   191,   196,    19,   193,   400,
     198,   305,   403,   198,   305,   160,   209,   192,   192,   298,
     300,   158,   398,   166,   193,   117,   126,   167,   172,   287,
     288,   246,   173,   192,   173,   192,   192,   192,   191,    18,
     147,   158,   357,   169,   147,   158,   305,   192,   147,   158,
     305,     1,   191,   193,   193,   305,   158,   158,   391,   257,
     305,   254,    19,   257,   305,   117,   167,    13,   305,   254,
     167,   169,   152,   257,   305,   195,   197,   254,   229,   290,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     123,   124,   305,   305,   305,   305,   305,   305,   305,   123,
     124,   305,   196,   244,     7,   313,   317,   158,   257,   305,
     196,   378,   382,   386,   174,   193,   158,   193,   174,   174,
     174,   174,   289,   192,   284,   305,   305,   399,   400,   305,
     158,   398,   191,   191,   305,   191,   402,   284,   399,   198,
     191,   164,   160,   305,   305,   366,   246,   191,   305,   158,
     158,   158,   158,   166,   191,   247,   306,   305,   308,   305,
     193,   284,   305,   173,   192,   305,   192,   191,   305,   192,
     191,   304,   193,    41,   328,   284,   254,   235,   256,    11,
      22,    23,   222,   223,   305,   290,   290,   290,   290,   191,
      55,    61,   316,    44,    97,   314,   196,   196,   262,   174,
     196,   282,   275,   277,   279,   192,   193,   284,   196,   400,
     191,   126,   328,   191,   196,   400,   191,   193,   193,   174,
     247,   193,   287,   191,   131,   254,   285,   366,   193,   391,
     193,   193,   193,   310,   305,   305,   193,   305,   158,   305,
     196,   305,   198,   254,   305,    11,   225,   196,    45,   314,
     316,   305,   158,   305,   305,   305,   193,   305,   193,   305,
     191,   191,   305,   305,   305,   198,   299,   174,   117,   305,
     174,   174,   391,   193,   193,   193,   254,   254,   226,   196,
     320,    32,   315,   329,   193,   174,   193,   196,   408,   191,
     408,   191,   296,   301,   305,   307,   309,   174,   223,    25,
      96,   227,   264,   265,   266,   268,   305,   251,   321,   305,
      51,   191,   198,   296,   311,   117,   305,   117,   305,   196,
     251,   193,   305,   191,   191,   305,   305,   254
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   200,   201,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   202,   202,   202,   203,   203,
     204,   205,   205,   205,   205,   206,   207,   207,   207,   208,
     209,   209,   211,   210,   212,   213,   214,   214,   214,   214,
     215,   215,   216,   216,   217,   218,   218,   219,   219,   220,
     221,   221,   222,   222,   223,   223,   223,   224,   224,   225,
     226,   225,   227,   227,   227,   227,   227,   228,   229,   228,
     230,   231,   232,   233,   235,   234,   236,   236,   236,   236,
     236,   236,   237,   237,   238,   238,   238,   239,   239,   239,
     239,   239,   239,   239,   239,   240,   240,   241,   241,   241,
     242,   242,   243,   243,   243,   243,   243,   243,   244,   244,
     245,   245,   246,   246,   246,   247,   247,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   249,   250,   250,
     250,   251,   253,   252,   254,   254,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   256,   256,   256,   257,
     257,   257,   258,   258,   258,   259,   259,   261,   262,   260,
     260,   263,   263,   263,   263,   264,   265,   266,   266,   266,
     267,   267,   267,   268,   268,   269,   269,   269,   270,   271,
     271,   272,   274,   275,   273,   276,   277,   273,   278,   279,
     273,   281,   282,   280,   283,   283,   283,   284,   284,   285,
     285,   285,   286,   286,   286,   287,   287,   287,   287,   288,
     288,   289,   289,   290,   290,   291,   291,   291,   291,   291,
     291,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     293,   293,   293,   293,   294,   295,   295,   296,   296,   297,
     298,   299,   297,   300,   301,   297,   302,   302,   303,   304,
     302,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     306,   307,   305,   305,   305,   305,   308,   309,   305,   305,
     305,   310,   311,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   313,   313,   314,   314,   314,   315,   315,   316,
     316,   316,   317,   318,   319,   318,   320,   318,   321,   318,
     318,   322,   322,   322,   323,   323,   324,   324,   325,   325,
     326,   327,   327,   328,   328,   329,   329,   329,   329,   329,
     329,   330,   330,   330,   331,   331,   332,   332,   332,   332,
     332,   333,   333,   333,   333,   333,   334,   335,   334,   336,
     336,   337,   337,   337,   338,   339,   338,   340,   340,   340,
     342,   341,   343,   343,   344,   344,   345,   345,   345,   346,
     347,   347,   348,   348,   349,   349,   350,   351,   351,   352,
     352,   352,   354,   355,   353,   356,   356,   356,   356,   356,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   358,   358,   358,   358,   358,
     358,   359,   360,   360,   360,   361,   361,   363,   364,   362,
     365,   365,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   366,   366,   366,   366,   366,   366,   366,   367,
     368,   366,   366,   369,   370,   366,   371,   372,   366,   373,
     374,   366,   366,   375,   376,   366,   377,   378,   366,   366,
     379,   380,   366,   381,   382,   366,   366,   383,   384,   366,
     385,   386,   366,   387,   388,   366,   389,   390,   366,   391,
     391,   391,   393,   394,   392,   396,   395,   397,   397,   397,
     397,   398,   398,   398,   398,   399,   399,   400,   400,   401,
     401,   401,   401,   401,   401,   402,   402,   402,   403,   403,
     404,   404,   405,   405,   406,   406,   407,   408,   408,   409,
     409
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
       1,     4,     1,     2,     3,     3,     3,     3,     1,     3,
       0,     3,     0,     2,     3,     0,     2,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       1,     3,     0,     4,     3,     7,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     2,     2,     2,     0,     2,     2,     3,
       2,     2,     1,     3,     2,     2,     3,     0,     0,     5,
       1,     2,     4,     5,     2,     1,     1,     1,     2,     3,
       2,     2,     3,     2,     3,     2,     2,     3,     4,     1,
       1,     2,     0,     0,     7,     0,     0,     7,     0,     0,
       7,     0,     0,     6,     5,     8,    10,     1,     3,     1,
       2,     3,     1,     1,     2,     2,     2,     2,     2,     1,
       3,     0,     4,     1,     6,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     4,     4,     4,     6,     5,     6,     1,     4,     3,
       0,     0,     8,     0,     0,     9,     3,     4,     0,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     3,     4,     5,     4,     5,     3,     4,     1,     3,
       4,     3,     4,     2,     4,     4,     7,     8,     3,     5,
       0,     0,     8,     3,     3,     3,     0,     0,     8,     3,
       4,     0,     0,     9,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     4,     4,     4,     4,     4,
       4,     1,     6,     7,     6,     6,     7,     7,     6,     7,
       6,     6,     0,     4,     0,     1,     1,     0,     1,     0,
       1,     1,     4,     0,     0,     4,     0,     8,     0,     9,
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
       1,     1,     1,     4,     4,     1,     3,     0,     0,     6,
       1,     3,     1,     1,     1,     1,     4,     3,     4,     2,
       2,     3,     2,     3,     2,     2,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     6,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     1,
       3,     3,     0,     0,     8,     0,     7,     1,     1,     1,
       1,     3,     3,     5,     5,     1,     3,     0,     2,     6,
       5,     7,     8,     6,     8,     1,     3,     3,     3,     1,
       1,     3,     5,     5,     1,     3,     4,     0,     3,    10,
      10
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

  case 100: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
    break;

  case 101: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 102: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 103: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 104: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 105: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 106: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 107: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 108: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 109: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 110: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 111: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 112: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 113: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 114: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 115: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 116: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 117: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 118: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 119: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 120: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 121: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 122: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 123: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 124: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 125: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 126: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 127: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 128: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 129: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 130: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 131: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 132: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 133: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 134: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 135: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 136: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 137: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 138: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 139: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 140: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 141: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 142: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 143: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 144: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 145: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 146: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 147: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 148: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 149: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 150: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 151: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 152: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 153: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 154: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 155: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 156: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 157: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 158: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 159: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 160: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 161: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 162: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 163: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 164: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 165: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 166: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 167: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 168: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 169: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 170: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 171: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 172: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 173: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 174: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 175: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 176: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 177: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 178: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 179: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 180: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 181: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 182: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 183: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 184: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 185: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 186: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 187: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 188: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 189: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 190: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 191: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 192: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 193: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 194: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 195: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 196: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 197: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 198: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 199: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 200: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 201: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 202: /* $@5: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 203: /* function_declaration: optional_public_or_private_function $@5 function_declaration_header expression_block  */
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

  case 204: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 205: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 206: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 207: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 208: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 209: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 210: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 211: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 212: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 213: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 214: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 215: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 216: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 217: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 218: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 219: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 220: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 221: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 222: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 224: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 225: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 226: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 227: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 228: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 229: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 230: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 231: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 232: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 233: /* name_in_namespace: "name" "::" "name"  */
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

  case 234: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 235: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 236: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 237: /* $@6: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 238: /* $@7: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 239: /* new_type_declaration: '<' $@6 type_declaration '>' $@7  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 240: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 241: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 242: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 243: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 244: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 245: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 246: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 247: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 248: /* expression_return_no_pipe: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 249: /* expression_return_no_pipe: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 250: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 251: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 252: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 253: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 254: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 255: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 256: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 257: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 258: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 259: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 260: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 261: /* expression_let: kwd_let let_variable_declaration  */
                                               {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 262: /* $@8: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 263: /* $@9: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 264: /* expr_cast: "cast" '<' $@8 type_declaration_no_options '>' $@9 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 265: /* $@10: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 266: /* $@11: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 267: /* expr_cast: "upcast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 268: /* $@12: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 269: /* $@13: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 270: /* expr_cast: "reinterpret" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 271: /* $@14: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 272: /* $@15: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 273: /* expr_type_decl: "type" '<' $@14 type_declaration '>' $@15  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 274: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 275: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 276: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 277: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 278: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 279: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 280: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 281: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 282: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 283: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 284: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 285: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 286: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 287: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 288: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 289: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 290: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 291: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 292: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 293: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 294: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 295: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 296: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 297: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 298: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 299: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 300: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 301: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 302: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 303: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 304: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 305: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 306: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 307: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 308: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 309: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 310: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 311: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 312: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 313: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 314: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 315: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 316: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 317: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 318: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 319: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 320: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 325: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 326: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 327: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 328: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 329: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 330: /* $@16: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 331: /* $@17: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 332: /* func_addr_expr: '@' '@' '<' $@16 type_declaration_no_options '>' $@17 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 333: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 334: /* $@19: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 335: /* func_addr_expr: '@' '@' '<' $@18 optional_function_argument_list optional_function_type '>' $@19 func_addr_name  */
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

  case 336: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 337: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 338: /* $@20: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 339: /* $@21: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 340: /* expr_field: expr '.' $@20 error $@21  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 341: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 342: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 343: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 344: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 345: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 346: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 347: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 348: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 349: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 350: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 351: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 377: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 380: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 381: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 382: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 383: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 384: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 385: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 386: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 387: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 388: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 389: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 390: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 391: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 392: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 393: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 395: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 396: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 397: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 398: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 400: /* $@22: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 401: /* $@23: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 402: /* expr: expr "is" "type" '<' $@22 type_declaration_no_options '>' $@23  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 403: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 404: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 405: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 406: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 407: /* $@25: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 408: /* expr: expr "as" "type" '<' $@24 type_declaration '>' $@25  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 409: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 410: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 411: /* $@26: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 412: /* $@27: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 413: /* expr: expr '?' "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 414: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 415: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 416: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 417: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 418: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 419: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 420: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 421: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 422: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 423: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) =ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 424: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 425: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 426: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 427: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 428: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 429: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 430: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 431: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 432: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 433: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 434: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 435: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 436: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 437: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 438: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 439: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 440: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 441: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 442: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 443: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 444: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 445: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 446: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 447: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 448: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 449: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 450: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 451: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 452: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 453: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 454: /* $@28: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 455: /* struct_variable_declaration_list: struct_variable_declaration_list $@28 structure_variable_declaration ';'  */
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

  case 456: /* $@29: %empty  */
                                                                                             {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 457: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@29 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
            }
    break;

  case 458: /* $@30: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 459: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@30 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 460: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 461: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 462: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 463: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 464: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 465: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 466: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 467: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 468: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 469: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 470: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 471: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 472: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 473: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 474: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 475: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 476: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 477: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 478: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 479: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 480: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 481: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 482: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 483: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 484: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 485: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 486: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 487: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 488: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 489: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 490: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 491: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 492: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 493: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 494: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 495: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 496: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 497: /* $@31: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 498: /* global_variable_declaration_list: global_variable_declaration_list $@31 optional_field_annotation let_variable_declaration  */
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

  case 499: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 500: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 501: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 502: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 503: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 504: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 505: /* $@32: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 506: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@32 optional_field_annotation let_variable_declaration  */
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

  case 507: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 508: /* enum_list: enum_list "name" ';'  */
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

  case 509: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 510: /* $@33: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 511: /* single_alias: "name" $@33 '=' type_declaration  */
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

  case 516: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 517: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 518: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 519: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 520: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 521: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 522: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 523: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 524: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 525: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 526: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 527: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 528: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 529: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 530: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 531: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 532: /* $@34: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 533: /* $@35: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 534: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@34 structure_name $@35 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
        }
    }
    break;

  case 535: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 536: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 537: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 538: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 539: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 540: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 541: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 542: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 543: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 544: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 545: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 546: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 547: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 548: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 549: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 550: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 551: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 552: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 553: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 554: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 555: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 556: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 557: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 558: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 559: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 560: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 561: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 562: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 563: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 564: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 565: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 566: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 567: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 568: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 569: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 570: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 571: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 572: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 573: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 574: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 575: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 576: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 577: /* $@36: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 578: /* $@37: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 579: /* bitfield_type_declaration: "bitfield" '<' $@36 bitfield_bits '>' $@37  */
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

  case 580: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 581: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 582: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 583: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 584: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 585: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 586: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
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

  case 587: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 588: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 589: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 590: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 591: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 592: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 593: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 594: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 595: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 596: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 597: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 598: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 599: /* $@38: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 600: /* $@39: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 601: /* type_declaration_no_options: "smart_ptr" '<' $@38 type_declaration '>' $@39  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 602: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 603: /* $@40: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 604: /* $@41: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 605: /* type_declaration_no_options: "array" '<' $@40 type_declaration '>' $@41  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 606: /* $@42: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 607: /* $@43: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 608: /* type_declaration_no_options: "table" '<' $@42 table_type_pair '>' $@43  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 609: /* $@44: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 610: /* $@45: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 611: /* type_declaration_no_options: "iterator" '<' $@44 type_declaration '>' $@45  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 612: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 613: /* $@46: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 614: /* $@47: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 615: /* type_declaration_no_options: "block" '<' $@46 type_declaration '>' $@47  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 616: /* $@48: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 617: /* $@49: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 618: /* type_declaration_no_options: "block" '<' $@48 optional_function_argument_list optional_function_type '>' $@49  */
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

  case 619: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 620: /* $@50: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 621: /* $@51: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 622: /* type_declaration_no_options: "function" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 623: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 624: /* $@53: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 625: /* type_declaration_no_options: "function" '<' $@52 optional_function_argument_list optional_function_type '>' $@53  */
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

  case 626: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 627: /* $@54: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 628: /* $@55: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 629: /* type_declaration_no_options: "lambda" '<' $@54 type_declaration '>' $@55  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 630: /* $@56: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 631: /* $@57: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 632: /* type_declaration_no_options: "lambda" '<' $@56 optional_function_argument_list optional_function_type '>' $@57  */
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

  case 633: /* $@58: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 634: /* $@59: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 635: /* type_declaration_no_options: "tuple" '<' $@58 tuple_type_list '>' $@59  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 636: /* $@60: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 637: /* $@61: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 638: /* type_declaration_no_options: "variant" '<' $@60 variant_type_list '>' $@61  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 639: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 640: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 641: /* type_declaration: type_declaration '|' '#'  */
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

  case 642: /* $@62: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 643: /* $@63: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 644: /* variant_alias_declaration: "variant" $@62 "name" $@63 '{' variant_type_list ';' '}'  */
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

  case 645: /* $@64: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 646: /* bitfield_alias_declaration: "bitfield" $@64 "name" '{' bitfield_bits ';' '}'  */
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

  case 647: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 648: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 649: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 650: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 651: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 652: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 653: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 654: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 655: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 656: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 657: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 658: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 659: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block ']' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 660: /* make_struct_decl: "[[" type_declaration_no_options optional_block ']' ']'  */
                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 661: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block ']' ']'  */
                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 662: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 663: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block '}' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 664: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block '}' ']'  */
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

  case 665: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 666: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 667: /* make_tuple: make_tuple ',' expr  */
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

  case 668: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 669: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 670: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 671: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 672: /* make_dim_decl: "[[" type_declaration_no_options make_dim ']' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 673: /* make_dim_decl: "[{" type_declaration_no_options make_dim '}' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 674: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 675: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 676: /* make_table_decl: "{{" make_table '}' '}'  */
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

  case 677: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 678: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 679: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 680: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


