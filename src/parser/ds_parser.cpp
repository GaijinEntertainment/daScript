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
  YYSYMBOL_DAS_TBLOCK = 84,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 85,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 86,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 87,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 88,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 89,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 90,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 91,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 92,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 93,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 94,                /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 95,              /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 96,             /* "generator"  */
  YYSYMBOL_DAS_YIELD = 97,                 /* "yield"  */
  YYSYMBOL_DAS_SEALED = 98,                /* "sealed"  */
  YYSYMBOL_ADDEQU = 99,                    /* "+="  */
  YYSYMBOL_SUBEQU = 100,                   /* "-="  */
  YYSYMBOL_DIVEQU = 101,                   /* "/="  */
  YYSYMBOL_MULEQU = 102,                   /* "*="  */
  YYSYMBOL_MODEQU = 103,                   /* "%="  */
  YYSYMBOL_ANDEQU = 104,                   /* "&="  */
  YYSYMBOL_OREQU = 105,                    /* "|="  */
  YYSYMBOL_XOREQU = 106,                   /* "^="  */
  YYSYMBOL_SHL = 107,                      /* "<<"  */
  YYSYMBOL_SHR = 108,                      /* ">>"  */
  YYSYMBOL_ADDADD = 109,                   /* "++"  */
  YYSYMBOL_SUBSUB = 110,                   /* "--"  */
  YYSYMBOL_LEEQU = 111,                    /* "<="  */
  YYSYMBOL_SHLEQU = 112,                   /* "<<="  */
  YYSYMBOL_SHREQU = 113,                   /* ">>="  */
  YYSYMBOL_GREQU = 114,                    /* ">="  */
  YYSYMBOL_EQUEQU = 115,                   /* "=="  */
  YYSYMBOL_NOTEQU = 116,                   /* "!="  */
  YYSYMBOL_RARROW = 117,                   /* "->"  */
  YYSYMBOL_LARROW = 118,                   /* "<-"  */
  YYSYMBOL_QQ = 119,                       /* "??"  */
  YYSYMBOL_QDOT = 120,                     /* "?."  */
  YYSYMBOL_QBRA = 121,                     /* "?["  */
  YYSYMBOL_LPIPE = 122,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 123,                   /* " <|"  */
  YYSYMBOL_LAPIPE = 124,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 125,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 126,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 127,                 /* ":="  */
  YYSYMBOL_ROTL = 128,                     /* "<<<"  */
  YYSYMBOL_ROTR = 129,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 130,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 131,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 132,                    /* "=>"  */
  YYSYMBOL_COLCOL = 133,                   /* "::"  */
  YYSYMBOL_ANDAND = 134,                   /* "&&"  */
  YYSYMBOL_OROR = 135,                     /* "||"  */
  YYSYMBOL_XORXOR = 136,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 137,                /* "&&="  */
  YYSYMBOL_OROREQU = 138,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 139,                /* "^^="  */
  YYSYMBOL_DOTDOT = 140,                   /* ".."  */
  YYSYMBOL_MTAG_E = 141,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 142,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 143,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 144,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 145,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 146,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 147,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 148,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 149,           /* "..."  */
  YYSYMBOL_BRABRAB = 150,                  /* "[["  */
  YYSYMBOL_BRACBRB = 151,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 152,                  /* "{{"  */
  YYSYMBOL_INTEGER = 153,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 154,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 155,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 156,    /* "unsigned long integer constant"  */
  YYSYMBOL_FLOAT = 157,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 158,                   /* "double constant"  */
  YYSYMBOL_NAME = 159,                     /* "name"  */
  YYSYMBOL_KEYWORD = 160,                  /* "keyword"  */
  YYSYMBOL_BEGIN_STRING = 161,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 162,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 163,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 164,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 165,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 166,          /* "}"  */
  YYSYMBOL_END_OF_READ = 167,              /* "end of failed eader macro"  */
  YYSYMBOL_168_ = 168,                     /* ','  */
  YYSYMBOL_169_ = 169,                     /* '='  */
  YYSYMBOL_170_ = 170,                     /* '?'  */
  YYSYMBOL_171_ = 171,                     /* ':'  */
  YYSYMBOL_172_ = 172,                     /* '|'  */
  YYSYMBOL_173_ = 173,                     /* '^'  */
  YYSYMBOL_174_ = 174,                     /* '&'  */
  YYSYMBOL_175_ = 175,                     /* '<'  */
  YYSYMBOL_176_ = 176,                     /* '>'  */
  YYSYMBOL_177_ = 177,                     /* '-'  */
  YYSYMBOL_178_ = 178,                     /* '+'  */
  YYSYMBOL_179_ = 179,                     /* '*'  */
  YYSYMBOL_180_ = 180,                     /* '/'  */
  YYSYMBOL_181_ = 181,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 182,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 183,               /* UNARY_PLUS  */
  YYSYMBOL_184_ = 184,                     /* '~'  */
  YYSYMBOL_185_ = 185,                     /* '!'  */
  YYSYMBOL_PRE_INC = 186,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 187,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 188,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 189,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 190,                    /* DEREF  */
  YYSYMBOL_191_ = 191,                     /* '.'  */
  YYSYMBOL_192_ = 192,                     /* '['  */
  YYSYMBOL_193_ = 193,                     /* ']'  */
  YYSYMBOL_194_ = 194,                     /* '('  */
  YYSYMBOL_195_ = 195,                     /* ')'  */
  YYSYMBOL_196_ = 196,                     /* '$'  */
  YYSYMBOL_197_ = 197,                     /* '@'  */
  YYSYMBOL_198_ = 198,                     /* ';'  */
  YYSYMBOL_199_ = 199,                     /* '{'  */
  YYSYMBOL_200_ = 200,                     /* '}'  */
  YYSYMBOL_201_ = 201,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 202,                 /* $accept  */
  YYSYMBOL_program = 203,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 204, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 205,              /* module_name  */
  YYSYMBOL_module_declaration = 206,       /* module_declaration  */
  YYSYMBOL_character_sequence = 207,       /* character_sequence  */
  YYSYMBOL_string_constant = 208,          /* string_constant  */
  YYSYMBOL_string_builder_body = 209,      /* string_builder_body  */
  YYSYMBOL_string_builder = 210,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 211, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 212,              /* expr_reader  */
  YYSYMBOL_213_1 = 213,                    /* $@1  */
  YYSYMBOL_options_declaration = 214,      /* options_declaration  */
  YYSYMBOL_require_declaration = 215,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 216,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 217,      /* require_module_name  */
  YYSYMBOL_require_module = 218,           /* require_module  */
  YYSYMBOL_is_public_module = 219,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 220,       /* expect_declaration  */
  YYSYMBOL_expect_list = 221,              /* expect_list  */
  YYSYMBOL_expect_error = 222,             /* expect_error  */
  YYSYMBOL_expression_label = 223,         /* expression_label  */
  YYSYMBOL_expression_goto = 224,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 225,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 226,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 227,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 228, /* expression_else_one_liner  */
  YYSYMBOL_229_2 = 229,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 230,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 231,  /* expression_if_then_else  */
  YYSYMBOL_232_3 = 232,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 233,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 234,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 235,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 236,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 237,    /* expression_with_alias  */
  YYSYMBOL_238_4 = 238,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 239, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 240, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 241, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 242,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 243, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 244, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 245, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 246,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 247,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 248, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 249, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 250,   /* optional_function_type  */
  YYSYMBOL_function_name = 251,            /* function_name  */
  YYSYMBOL_global_function_declaration = 252, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 253, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 254, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 255,     /* function_declaration  */
  YYSYMBOL_256_5 = 256,                    /* $@5  */
  YYSYMBOL_expression_block = 257,         /* expression_block  */
  YYSYMBOL_expression_any = 258,           /* expression_any  */
  YYSYMBOL_expressions = 259,              /* expressions  */
  YYSYMBOL_expr_keyword = 260,             /* expr_keyword  */
  YYSYMBOL_expression_keyword = 261,       /* expression_keyword  */
  YYSYMBOL_262_6 = 262,                    /* $@6  */
  YYSYMBOL_263_7 = 263,                    /* $@7  */
  YYSYMBOL_expr_pipe = 264,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 265,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 266,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 267,     /* new_type_declaration  */
  YYSYMBOL_268_8 = 268,                    /* $@8  */
  YYSYMBOL_269_9 = 269,                    /* $@9  */
  YYSYMBOL_expr_new = 270,                 /* expr_new  */
  YYSYMBOL_expression_break = 271,         /* expression_break  */
  YYSYMBOL_expression_continue = 272,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 273, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 274,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 275, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 276,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 277,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 278,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 279,        /* optional_in_scope  */
  YYSYMBOL_expression_let = 280,           /* expression_let  */
  YYSYMBOL_expr_cast = 281,                /* expr_cast  */
  YYSYMBOL_282_10 = 282,                   /* $@10  */
  YYSYMBOL_283_11 = 283,                   /* $@11  */
  YYSYMBOL_284_12 = 284,                   /* $@12  */
  YYSYMBOL_285_13 = 285,                   /* $@13  */
  YYSYMBOL_286_14 = 286,                   /* $@14  */
  YYSYMBOL_287_15 = 287,                   /* $@15  */
  YYSYMBOL_expr_type_decl = 288,           /* expr_type_decl  */
  YYSYMBOL_289_16 = 289,                   /* $@16  */
  YYSYMBOL_290_17 = 290,                   /* $@17  */
  YYSYMBOL_expr_type_info = 291,           /* expr_type_info  */
  YYSYMBOL_expr_list = 292,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 293,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 294,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 295,            /* capture_entry  */
  YYSYMBOL_capture_list = 296,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 297,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 298,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 299,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 300,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 301,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 302,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 303,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 304,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 305,           /* func_addr_expr  */
  YYSYMBOL_306_18 = 306,                   /* $@18  */
  YYSYMBOL_307_19 = 307,                   /* $@19  */
  YYSYMBOL_308_20 = 308,                   /* $@20  */
  YYSYMBOL_309_21 = 309,                   /* $@21  */
  YYSYMBOL_expr_field = 310,               /* expr_field  */
  YYSYMBOL_311_22 = 311,                   /* $@22  */
  YYSYMBOL_312_23 = 312,                   /* $@23  */
  YYSYMBOL_expr = 313,                     /* expr  */
  YYSYMBOL_314_24 = 314,                   /* $@24  */
  YYSYMBOL_315_25 = 315,                   /* $@25  */
  YYSYMBOL_316_26 = 316,                   /* $@26  */
  YYSYMBOL_317_27 = 317,                   /* $@27  */
  YYSYMBOL_318_28 = 318,                   /* $@28  */
  YYSYMBOL_319_29 = 319,                   /* $@29  */
  YYSYMBOL_expr_mtag = 320,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 321, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 322,        /* optional_override  */
  YYSYMBOL_optional_constant = 323,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 324, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 325, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 326, /* struct_variable_declaration_list  */
  YYSYMBOL_327_30 = 327,                   /* $@30  */
  YYSYMBOL_328_31 = 328,                   /* $@31  */
  YYSYMBOL_329_32 = 329,                   /* $@32  */
  YYSYMBOL_function_argument_declaration = 330, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 331,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 332,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 333,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 334,             /* variant_type  */
  YYSYMBOL_variant_type_list = 335,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 336,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 337,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 338,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 339,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 340, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 341, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 342, /* global_variable_declaration_list  */
  YYSYMBOL_343_33 = 343,                   /* $@33  */
  YYSYMBOL_optional_shared = 344,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 345, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 346,               /* global_let  */
  YYSYMBOL_347_34 = 347,                   /* $@34  */
  YYSYMBOL_enum_list = 348,                /* enum_list  */
  YYSYMBOL_single_alias = 349,             /* single_alias  */
  YYSYMBOL_350_35 = 350,                   /* $@35  */
  YYSYMBOL_alias_list = 351,               /* alias_list  */
  YYSYMBOL_alias_declaration = 352,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 353, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 354,                /* enum_name  */
  YYSYMBOL_enum_declaration = 355,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 356, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 357,          /* optional_sealed  */
  YYSYMBOL_structure_name = 358,           /* structure_name  */
  YYSYMBOL_class_or_struct = 359,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 360, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 361,    /* structure_declaration  */
  YYSYMBOL_362_36 = 362,                   /* $@36  */
  YYSYMBOL_363_37 = 363,                   /* $@37  */
  YYSYMBOL_variable_name_with_pos_list = 364, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 365,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 366, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 367, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 368,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 369,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 370, /* bitfield_type_declaration  */
  YYSYMBOL_371_38 = 371,                   /* $@38  */
  YYSYMBOL_372_39 = 372,                   /* $@39  */
  YYSYMBOL_table_type_pair = 373,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 374, /* type_declaration_no_options  */
  YYSYMBOL_375_40 = 375,                   /* $@40  */
  YYSYMBOL_376_41 = 376,                   /* $@41  */
  YYSYMBOL_377_42 = 377,                   /* $@42  */
  YYSYMBOL_378_43 = 378,                   /* $@43  */
  YYSYMBOL_379_44 = 379,                   /* $@44  */
  YYSYMBOL_380_45 = 380,                   /* $@45  */
  YYSYMBOL_381_46 = 381,                   /* $@46  */
  YYSYMBOL_382_47 = 382,                   /* $@47  */
  YYSYMBOL_383_48 = 383,                   /* $@48  */
  YYSYMBOL_384_49 = 384,                   /* $@49  */
  YYSYMBOL_385_50 = 385,                   /* $@50  */
  YYSYMBOL_386_51 = 386,                   /* $@51  */
  YYSYMBOL_387_52 = 387,                   /* $@52  */
  YYSYMBOL_388_53 = 388,                   /* $@53  */
  YYSYMBOL_389_54 = 389,                   /* $@54  */
  YYSYMBOL_390_55 = 390,                   /* $@55  */
  YYSYMBOL_391_56 = 391,                   /* $@56  */
  YYSYMBOL_392_57 = 392,                   /* $@57  */
  YYSYMBOL_393_58 = 393,                   /* $@58  */
  YYSYMBOL_394_59 = 394,                   /* $@59  */
  YYSYMBOL_395_60 = 395,                   /* $@60  */
  YYSYMBOL_396_61 = 396,                   /* $@61  */
  YYSYMBOL_397_62 = 397,                   /* $@62  */
  YYSYMBOL_398_63 = 398,                   /* $@63  */
  YYSYMBOL_type_declaration = 399,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 400, /* variant_alias_declaration  */
  YYSYMBOL_401_64 = 401,                   /* $@64  */
  YYSYMBOL_402_65 = 402,                   /* $@65  */
  YYSYMBOL_bitfield_alias_declaration = 403, /* bitfield_alias_declaration  */
  YYSYMBOL_404_66 = 404,                   /* $@66  */
  YYSYMBOL_make_decl = 405,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 406,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 407,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 408,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 409,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 410,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 411,           /* make_map_tuple  */
  YYSYMBOL_make_dim = 412,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 413,            /* make_dim_decl  */
  YYSYMBOL_make_table = 414,               /* make_table  */
  YYSYMBOL_make_table_decl = 415,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 416, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 417       /* array_comprehension  */
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
#define YYLAST   11221

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  202
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  216
/* YYNRULES -- Number of rules.  */
#define YYNRULES  691
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1215

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   429


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
       2,     2,     2,   185,     2,   201,   196,   181,   174,     2,
     194,   195,   179,   178,   168,   177,   191,   180,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   171,   198,
     175,   169,   176,   170,   197,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   192,     2,   193,   173,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   199,   172,   200,   184,     2,     2,     2,
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
     165,   166,   167,   182,   183,   186,   187,   188,   189,   190
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   487,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   503,   504,   505,   509,   510,
     514,   532,   533,   534,   535,   539,   543,   548,   557,   565,
     581,   586,   594,   594,   624,   645,   649,   650,   654,   657,
     661,   667,   676,   679,   685,   686,   690,   694,   695,   699,
     702,   708,   714,   717,   723,   724,   728,   729,   730,   739,
     740,   744,   745,   745,   751,   752,   753,   754,   755,   759,
     765,   765,   771,   777,   785,   795,   804,   804,   811,   812,
     813,   814,   815,   816,   820,   825,   833,   834,   835,   839,
     840,   841,   842,   843,   844,   845,   846,   852,   855,   861,
     862,   863,   867,   875,   888,   891,   899,   910,   921,   932,
     938,   942,   949,   950,   954,   955,   956,   960,   963,   970,
     974,   975,   976,   977,   978,   979,   980,   981,   982,   983,
     984,   985,   986,   987,   988,   989,   990,   991,   992,   993,
     994,   995,   996,   997,   998,   999,  1000,  1001,  1002,  1003,
    1004,  1005,  1006,  1007,  1008,  1009,  1010,  1011,  1012,  1013,
    1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,
    1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,
    1034,  1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,
    1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,  1057,
    1079,  1080,  1081,  1085,  1091,  1091,  1108,  1112,  1123,  1124,
    1125,  1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,
    1135,  1136,  1137,  1138,  1139,  1140,  1141,  1142,  1143,  1147,
    1152,  1158,  1164,  1175,  1175,  1175,  1186,  1220,  1223,  1229,
    1230,  1241,  1245,  1248,  1256,  1256,  1256,  1259,  1265,  1268,
    1271,  1275,  1281,  1285,  1289,  1292,  1295,  1303,  1306,  1309,
    1317,  1320,  1328,  1331,  1334,  1342,  1348,  1349,  1353,  1354,
    1358,  1364,  1364,  1364,  1367,  1367,  1367,  1372,  1372,  1372,
    1380,  1380,  1380,  1386,  1396,  1407,  1422,  1425,  1431,  1432,
    1439,  1450,  1451,  1452,  1456,  1457,  1458,  1459,  1463,  1468,
    1476,  1477,  1481,  1486,  1493,  1494,  1495,  1496,  1497,  1498,
    1502,  1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,
    1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,  1520,  1524,
    1525,  1526,  1527,  1531,  1542,  1547,  1557,  1561,  1568,  1571,
    1571,  1571,  1576,  1576,  1576,  1589,  1593,  1597,  1597,  1597,
    1604,  1605,  1606,  1607,  1608,  1609,  1610,  1611,  1612,  1613,
    1614,  1615,  1616,  1617,  1618,  1619,  1620,  1621,  1622,  1623,
    1624,  1625,  1626,  1627,  1628,  1629,  1630,  1631,  1632,  1633,
    1634,  1635,  1636,  1637,  1638,  1639,  1645,  1646,  1647,  1648,
    1649,  1650,  1651,  1652,  1653,  1654,  1655,  1656,  1657,  1661,
    1665,  1668,  1671,  1672,  1673,  1674,  1677,  1680,  1681,  1684,
    1684,  1684,  1687,  1692,  1696,  1700,  1700,  1700,  1705,  1708,
    1712,  1712,  1712,  1717,  1720,  1721,  1722,  1723,  1724,  1725,
    1726,  1727,  1728,  1729,  1730,  1735,  1739,  1740,  1741,  1742,
    1743,  1744,  1745,  1749,  1753,  1757,  1761,  1765,  1769,  1773,
    1777,  1781,  1788,  1789,  1793,  1794,  1795,  1799,  1800,  1804,
    1805,  1806,  1810,  1820,  1823,  1823,  1842,  1841,  1856,  1855,
    1868,  1877,  1883,  1888,  1898,  1899,  1903,  1906,  1915,  1916,
    1920,  1929,  1930,  1935,  1936,  1940,  1946,  1952,  1955,  1959,
    1965,  1974,  1975,  1976,  1980,  1981,  1985,  1992,  1997,  2006,
    2012,  2023,  2026,  2031,  2036,  2044,  2055,  2058,  2058,  2078,
    2079,  2083,  2084,  2085,  2089,  2092,  2092,  2111,  2114,  2123,
    2136,  2136,  2157,  2158,  2162,  2163,  2167,  2168,  2169,  2173,
    2183,  2190,  2200,  2201,  2205,  2206,  2210,  2216,  2217,  2221,
    2222,  2223,  2227,  2232,  2227,  2242,  2249,  2254,  2263,  2269,
    2280,  2281,  2282,  2283,  2284,  2285,  2286,  2287,  2288,  2289,
    2290,  2291,  2292,  2293,  2294,  2295,  2296,  2297,  2298,  2299,
    2300,  2301,  2302,  2303,  2304,  2308,  2309,  2310,  2311,  2312,
    2313,  2317,  2328,  2332,  2339,  2351,  2358,  2367,  2367,  2367,
    2380,  2384,  2391,  2392,  2393,  2394,  2395,  2409,  2415,  2419,
    2423,  2428,  2433,  2438,  2443,  2447,  2451,  2456,  2460,  2464,
    2469,  2469,  2469,  2475,  2482,  2482,  2482,  2487,  2487,  2487,
    2493,  2493,  2493,  2498,  2502,  2502,  2502,  2507,  2507,  2507,
    2516,  2520,  2520,  2520,  2525,  2525,  2525,  2534,  2538,  2538,
    2538,  2543,  2543,  2543,  2552,  2552,  2552,  2558,  2558,  2558,
    2567,  2570,  2581,  2597,  2597,  2597,  2621,  2621,  2641,  2642,
    2643,  2644,  2648,  2655,  2662,  2668,  2677,  2682,  2689,  2690,
    2694,  2700,  2707,  2715,  2722,  2730,  2742,  2745,  2751,  2765,
    2771,  2777,  2782,  2789,  2794,  2804,  2809,  2816,  2828,  2829,
    2833,  2836
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
  "\"range\"", "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"",
  "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"",
  "\"int16\"", "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"",
  "\"yield\"", "\"sealed\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"",
  "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"",
  "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"",
  "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"",
  "\"@ <|\"", "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"",
  "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"",
  "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"",
  "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"",
  "\"[{\"", "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
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
     415,   416,   417,   418,   419,   420,   421,   422,    44,    61,
      63,    58,   124,    94,    38,    60,    62,    45,    43,    42,
      47,    37,   423,   424,   126,    33,   425,   426,   427,   428,
     429,    46,    91,    93,    40,    41,    36,    64,    59,   123,
     125,    35
};
#endif

#define YYPACT_NINF (-865)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-642)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -865,    19,  -865,  -865,    20,   -67,   237,   -56,  -865,   -31,
    -865,  -865,   115,  -865,  -865,  -865,  -865,  -865,   277,  -865,
       9,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
     -65,  -865,   133,   179,   189,  -865,  -865,  -865,   237,  -865,
      30,  -865,  -865,   234,  -865,  -865,  -865,     9,   249,   293,
    -865,  -865,   325,   373,   115,   115,   351,  -865,   300,   -21,
    -865,  -865,  -865,    98,   309,   421,  -865,   442,     7,    20,
     362,   -67,   332,   388,  -865,    91,    91,  -865,   381,   354,
    -110,   446,   355,  -865,  -865,   397,  -865,   -73,    20,   115,
     115,   115,   115,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
     399,  -865,  -865,  -865,  -865,  -865,   370,  -865,  -865,  -865,
    -865,  -865,   259,   126,  -865,  -865,  -865,  -865,   512,  -865,
    -865, 10882,  -865,  -865,   378,  -865,  -865,  -865,   423,   386,
    -865,  -865,    97,  -865,    33,   487,   300, 11062,  -865,   131,
     532,  -865,   481,  -865,  -865,   401,  -865,  -865,  -865,  -865,
    -865,  -865,  -865,   138,  -865,   457,   459,   464,   465,  -865,
    -865,  -865,   444,  -865,  -865,  -865,  -865,  -865,   466,  -865,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,   467,  -865,
    -865,  -865,   468,   470,  -865,  -865,  -865,  -865,   472,   473,
     462,  -865,  -865,  -865,  -865,  -865,   653,   478,  -865,  -865,
     455,   514,  -865,  9570,  -865,  -865,  -865,  -865,  -865,  -865,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,   637,   641,  -865,
     480,   477,   372,  -865,  -865,   518,  -865,   479,    20,   148,
    -865,  -865,  -865,   126,  -865,  -865,  -865,  -865,  -865,   519,
    -865,   238,   262,   296,  -865,  -865,  6215,  -865,  -865,  -865,
       0,  -865,  -865,  -865,    24,  3624,  -865,  1146,  -106,   511,
    -865,   485,   529,   530,  -865,  -865,  -865,  -865,  -865,  -865,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
    -865,  -865,  -865,  -865,  -865,   497,  -865,  -865,  -865,  -865,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,   670,  -865,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
    -865,  -865,   500,  -865,  -865,   -79,   523,  -865,  -865,  -865,
    -865,  -865,  -865,  -865,  -865,   498,   -95,   525,   501,  -865,
     481,    50,   507,   661,   334,  -865,  -865, 10882, 10882, 10882,
   10882,   508,   423, 10882,   480, 10882,   480, 10882,   480, 10977,
     514,  -865,  -865,   209,   510,   531,  -865,   513,   533,   534,
     516,   538,   520,  -865,   540,  6215,  6215,   522,   524,   527,
     528,   535,   545,  -865, 10692, 10787,  6215,  -865,  -865,  -865,
    -865,  -865,  -865,   542,  -865,  6215,  6215,  6215,   232,  6215,
    6215,  6215,  -865,   543,  -865,  -865,  -865,  -865,  -105,  -865,
    -865,  -865,  -865,   536,  -865,  -865,  -865,  -865,  -865,  -865,
    6511,  -865,   547,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
    -865,  -865,   526,  -865,  -865,  8995,  -865,   653,  -865,  -865,
   10882,   -94,  -865,  -865,  -865,   546,  -865,   552,  -865,    86,
    -865,   298, 10882,  -865,  1651,  -865,   122,  -865,   232,  -865,
    -865,   148,   541,  6215,   589,   590, 10882,  -865,    -6,   -84,
     577,   -98,   279,   344,  -865,    51,   349,   523,   350,   523,
     352,   523,    44,  -865,   229,   478,   255,  -865,   560,  -865,
    -865,   232,  -865,  6215,  -865,  -865,  6215,  -865,  6215, 10882,
     303,   303,  6215,  6215,  6215,  6215,  6215,  6215,   260,  2026,
     260,  2198,  9667,  -865,   110,  -865,   410,   303,   303,   -37,
    -865,   303,   303,  6603,   324,  -865,  3005,   610,  6495,  6679,
    6215,  6215,  -865,  -865,  6215,  6215,  6215,  6215,   604,  6215,
     329,  6215,  6215,  6215,  6215,  6215,  6215,  6215,  6215,  6215,
    3773,  6215,  6215,  6215,  6215,  6215,  6215,  6215,  6215,  6215,
    6215,   278,  6215,  -865,  3922,  -865,  -865,   478,  -865,  -865,
    -865,  6215,   260,   570,   725,  -865,   -72,  -865,   368,   478,
    -865,  6215,  -865,  -865,   260,  2707,  -865,   477,  4094,  6215,
     611,  -865,   571,   618,  4243,   281,  2856,   384,   384,  4392,
    -865,   737,   578,   579,  6215,   769,  -865,  -865,  -865,  -865,
    -865,  -865,  -865,  -865,  -865,   581,   582,   583,   584,  -865,
     586,  -865,  -865,   721,  -865,   -90,  -865,   489,   -39,  6215,
    -865,  -865,    13,  -865,  -865,  6695,  -865,   746,   640,  -865,
    -865,  -865,  3177,  -865,  -865, 10882,  -865,  -865,  -865,   642,
    -865,   624,  -865,   627,  -865,   630, 10882,  -865, 10977,  -865,
     514, 10882,  4564,  4736, 10882,  6787, 10882, 10882,  6879, 10882,
    6971,   385,  7063,  7155,  7247,  7339,  7431,  7523,    36,   384,
     142,  2370,  4908,  9746,   644,   -25,   623,   651,   321,    38,
    5080,   -25,   339,  6215,  6215,   621, 10882,  -865,  6215,   379,
     660,  -865,   634,   639,   297,  -865,  -865,   665,  -865,   145,
   10031,   -59,   480,   656,   643,  -865,  -865,   659,   648,  -865,
    -865,   137,   137,  1262,  1262, 10557, 10557,   649,    61,   650,
    -865,  9087,   102,   102,   137,   137, 10375, 10243, 10335, 10110,
    6863,  9849, 10454, 10478,   967,  1262,  1262,   268,   268,    61,
      61,    61,   330,  6215,   652,  -865,   333,  6215,   835,  9179,
    -865,   157,  7615,  -865,  6215,   688,  -865,   694,  -865, 10882,
    -865,  3177,  -865,   616,    41,  3177,  -865,   736,  9466,   851,
    6215, 10031,   616,   696,  -865,   695,   715, 10031,  -865,  3177,
    -865,  9466,   674,  -865,  -865,   616,   673,  -865,  -865,   616,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,   148,   384,  -865,
    6215,  6215,  6215,  6215,  6215,  6215,  6215,  6215,  6215,  6215,
    3326,  6215,  6215,  6215,  6215,  6215,  6215,  3475,  -865,   741,
     115,  -865,   866,   481,  -865,   719,  -865,  3177,  -865,  1409,
    -865,  -865,   478,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
    -865,  -865,   478,  -865,  -865,  -865,   357,  -865,   159,   720,
    7707,   358,  -865,   628,   767,  -865,   781,  -865,   610,  -865,
    -865,  -865,  -865,  -865,   680,  6215,  -865,  6215,  6215,   -20,
    6215,   724,   665,   687,   697,  6215,   698,  6215,  6215,   665,
     692,   700, 10031,  -865,  -865,  1077,  9928,  -865,   726,  6215,
    6215, 10882,   480,    57,   178,  6215,  -865,   728,   735,   739,
     740,  -865,   181,   523,  -865,  6215,  -865,  6215,  5252,  6215,
    -865,   727,   709,  -865,  -865,  6215,   713,  -865,  9271,  6215,
     718,  -865,  9363,  -865,  -865,  -865,  -865,  7799,  -865,   883,
     -71,  -865,  9466,  -865,  6215,  -865,  9466,  6215,  6215,   477,
   10031,  -865,  -865,  -865,  -865,  -865,  9466,  -865,  -865,  -865,
     463,  6215,  -865,  -865, 10031, 10031, 10031, 10031, 10031, 10031,
   10031, 10031, 10031, 10031,   384,   384, 10031, 10031, 10031, 10031,
   10031, 10031, 10031,   384,   384, 10031,  -865,   211,   456,    -3,
     729,  -865,  -865,  6316,  -865,  -865,  -865,  -865,  -865,  -865,
     263,  -865,  -865,  -865,  -865,  -865,   732,  5424,   -81, 10031,
   10031,   -25,   738, 10031,   150,   644,   742,  -865, 10031,  -865,
     651,    46,   -25,   743,  -865,  -865,  -865,  -865,  7891,  7983,
    1243,   523,   733, 10031,  -865,  -865,  -865,  -865,   -59,   745,
     -92, 10882,  8075, 10882,  8167,  -865,   185,  8259,  -865,  6215,
   10213,  6215,  -865,  8351,  6215,  -865,  -865,  -865,   770,  6215,
      66,  -865,  6215,  1851,   477,  -865,  -865,  6215,  -865,    16,
    -865,  -865,  -865,  -865,   744,  -865,  -865,    31,  -865,  -865,
     456,  -865,  -865,  -865,  6215,   771,  -865,  6215,  6215,  6215,
    5596,  -865,   188,  6215,   747,   752,  6215,  6215,  -865,  6215,
     734,  -865,  6215,  -865,  -865,  -865,   772,  -865,  -865,  -865,
    5768,  -865,  -865,  1285,  -865,   359,  -865,  -865,  -865, 10882,
    8443,  8535,  -865,  8627,  -865, 10031,   477, 10031,  -865,  -865,
     616,  -865,   748,  -865,  -865,   915,   260,  8719,   773,  1262,
    1262,  1262,  -865,  8811,  -865,  6419,   757,  -865, 10031, 10031,
    6419,   759,  1262,   162,  -865,  6215, 10110,  -865,  -865,   360,
    -865,  -865,  -865,  -865,   463,  2558,  -865, 11062,  -865,  -865,
    -865,  -865,  6215,  -865,   902,   761,  -865,   756,  -865,  -865,
     162, 10213,  -865,  -865,  -865,  -865,  5917,  6066,  -865,  -865,
    -865,  -865,  -865, 10031,   762, 11062,  8903,  6215,   768,   774,
    -865,  -865,  6215, 10031,  6215, 10031,  -865,   477,  -865, 10031,
    -865,  -865, 10031, 10031,  -865
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   112,     1,   266,     0,     0,     0,     0,   267,     0,
     656,   653,     0,    14,     3,    10,     9,     8,     0,     7,
     509,     6,    11,     5,     4,    12,    13,    87,    88,    86,
      95,    97,    34,    49,    46,    47,    36,    37,     0,    38,
      44,    35,   520,     0,   525,    19,    18,   509,     0,     0,
     100,   101,     0,   239,     0,     0,   102,   104,   110,     0,
      99,   538,   537,   200,   526,   539,   510,   511,     0,     0,
       0,     0,    39,     0,    45,     0,     0,    42,     0,     0,
       0,    15,     0,   654,   241,     0,   105,     0,     0,     0,
       0,     0,     0,   113,   202,   201,   204,   199,   528,   527,
       0,   541,   540,   542,   513,   512,   515,    93,    94,    91,
      92,    90,     0,     0,    89,    98,    50,    48,    44,    41,
      40,     0,   522,   524,     0,    16,    17,    20,     0,     0,
     240,   109,     0,   106,   107,   108,   111,     0,   529,     0,
     534,   506,   452,    21,    22,     0,    82,    83,    80,    81,
      79,    78,    84,     0,    43,     0,     0,     0,     0,   550,
     570,   551,   582,   552,   556,   557,   558,   559,   574,   563,
     564,   565,   566,   567,   568,   569,   571,   572,   623,   555,
     562,   573,   630,   637,   553,   560,   554,   561,     0,     0,
       0,   581,   592,   595,   593,   594,   650,   521,   523,   585,
       0,     0,   103,     0,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,     0,   119,
     114,     0,     0,   517,   535,     0,   543,   507,     0,     0,
      23,    24,    25,     0,    96,   614,   617,   620,   610,     0,
     587,   624,   631,   638,   644,   647,     0,   600,   605,   599,
       0,   613,   609,   602,     0,     0,   604,     0,     0,     0,
     481,     0,   170,   171,   168,   122,   123,   125,   124,   126,
     127,   128,   129,   155,   156,   153,   154,   146,   157,   158,
     147,   144,   145,   169,   166,     0,   167,   159,   160,   161,
     162,   133,   134,   135,   130,   131,   132,   143,     0,   149,
     150,   148,   141,   142,   137,   136,   138,   139,   140,   121,
     120,   165,     0,   151,   152,   452,   117,   229,   205,   575,
     578,   576,   579,   577,   580,     0,     0,   532,     0,   514,
     452,     0,     0,   496,   494,   516,    85,     0,     0,     0,
       0,     0,     0,     0,   114,     0,   114,     0,   114,     0,
       0,   356,   357,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,   574,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,   304,   306,   305,
     307,   308,   309,     0,    26,     0,     0,     0,     0,     0,
       0,     0,   291,   292,   354,   353,   302,   435,   351,   427,
     426,   425,   424,   112,   430,   352,   429,   428,   397,   358,
       0,   359,     0,   355,   658,   659,   660,   661,   607,   608,
     601,   603,     0,   606,   597,     0,   652,   651,   586,   657,
       0,     0,   172,   173,   164,   174,   163,     0,   115,     0,
     474,     0,     0,   203,     0,   517,     0,   530,     0,   536,
     463,     0,     0,     0,     0,     0,     0,   495,     0,     0,
       0,   590,     0,     0,   583,     0,     0,   117,     0,   117,
       0,   117,   239,   478,     0,   476,     0,   244,   248,   247,
     251,     0,   280,     0,   271,   274,     0,   277,     0,     0,
     386,   387,     0,     0,     0,     0,     0,     0,     0,   668,
       0,     0,   680,   685,     0,   233,     0,   363,   362,   402,
      32,   361,   360,     0,   293,   433,     0,   300,     0,     0,
       0,     0,   388,   389,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,   584,     0,   598,   596,   480,   655,   482,
     175,     0,     0,     0,   545,   471,   485,   116,   452,   118,
     231,     0,    59,    60,     0,   254,   252,     0,     0,     0,
       0,   253,     0,     0,     0,     0,     0,     0,     0,     0,
     208,   206,     0,     0,     0,     0,   224,   219,   216,   215,
     217,   218,   230,   210,   209,     0,    67,    68,    65,   222,
      66,   223,   225,   269,   214,     0,   211,   310,     0,     0,
     518,   533,   464,   508,   453,     0,   498,   499,     0,   492,
     493,   491,     0,   615,   618,     0,   621,   611,   588,     0,
     625,     0,   632,     0,   639,     0,     0,   645,     0,   648,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,     0,   676,   666,   668,     0,   681,     0,     0,
       0,   668,     0,     0,     0,     0,     0,    29,     0,    27,
       0,   390,     0,     0,   339,   336,   338,     0,   398,     0,
     286,     0,   114,     0,     0,   413,   412,     0,     0,   414,
     418,   364,   365,   377,   378,   375,   376,     0,   407,     0,
     395,     0,   431,   432,   366,   367,   382,   383,   384,   385,
       0,     0,   380,   381,   379,   373,   374,   369,   368,   370,
     371,   372,     0,     0,     0,   345,     0,     0,     0,     0,
     400,     0,     0,   472,     0,     0,   484,     0,   483,     0,
     486,     0,   475,     0,     0,     0,   258,     0,   255,     0,
       0,   242,     0,     0,   228,     0,     0,    53,    73,     0,
     263,   260,   292,   237,   238,     0,     0,   226,   227,     0,
      70,   213,   220,   221,   257,   262,   268,     0,     0,   212,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   531,     0,
       0,   544,     0,   452,   497,     0,   501,     0,   505,   310,
     616,   619,   591,   622,   612,   589,   626,   628,   633,   635,
     640,   642,   477,   646,   479,   649,     0,   249,     0,     0,
       0,     0,   403,     0,     0,   404,     0,   434,   300,   436,
     437,   438,   439,   440,     0,     0,   669,     0,     0,   668,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   679,   686,   687,     0,     0,    30,    33,     0,
       0,     0,   114,     0,     0,     0,   399,     0,     0,     0,
       0,   298,     0,   117,   409,     0,   415,     0,     0,     0,
     393,     0,     0,   419,   423,     0,     0,   396,     0,     0,
       0,   346,     0,   348,   391,   401,   473,     0,   547,   548,
     487,   490,   489,    74,     0,   259,   256,     0,     0,     0,
     243,    75,    76,    51,    52,   264,   261,   293,   232,   229,
      56,     0,   270,   236,   320,   321,   323,   322,   324,   314,
     315,   316,   325,   326,     0,     0,   312,   313,   327,   328,
     317,   318,   319,     0,     0,   311,   519,     0,   459,   454,
       0,   500,   503,   310,   504,   629,   636,   643,   245,   250,
       0,   283,   281,   272,   275,   278,     0,     0,     0,   663,
     662,   668,     0,   677,     0,   667,     0,   671,   678,   683,
     682,     0,   668,     0,   684,   234,    28,    31,     0,     0,
       0,   117,     0,   287,   296,   297,   295,   294,     0,     0,
       0,     0,     0,     0,     0,   334,     0,     0,   420,     0,
     408,     0,   394,     0,     0,   392,   349,   546,     0,     0,
       0,   265,     0,     0,     0,    54,    55,     0,    69,    61,
     331,   332,   329,   330,   113,   460,   461,   454,   455,   456,
     459,   465,   502,   246,     0,     0,   282,     0,     0,     0,
       0,   442,     0,     0,     0,     0,     0,     0,   670,     0,
       0,   674,     0,   337,   451,   340,     0,   333,   299,   301,
       0,   288,   303,     0,   450,     0,   448,   335,   445,     0,
       0,     0,   444,     0,   549,   488,     0,    77,   207,    57,
       0,    62,     0,   470,   466,   457,     0,     0,     0,   273,
     276,   279,   405,     0,   443,   688,     0,   672,   665,   664,
     688,     0,   235,     0,   343,     0,   289,   410,   416,     0,
     449,   447,   446,    72,    56,     0,    71,     0,   458,   468,
     462,   284,     0,   406,     0,     0,   673,     0,   675,   341,
       0,   290,   411,   417,   421,    58,   254,     0,    63,    67,
      68,    65,    66,    64,     0,     0,     0,     0,     0,     0,
     344,   422,     0,   255,     0,   260,   467,     0,   285,   689,
     690,   691,   256,   261,   469
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -865,  -865,  -865,  -865,  -865,   448,   894,  -865,  -865,  -865,
    -865,  -865,  -865,  -865,   537,   927,  -865,   852,  -865,  -865,
     898,  -865,  -865,  -865,  -193,  -865,  -865,  -865,  -191,  -865,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,   749,  -865,  -865,
     906,   -53,  -865,  -865,   207,   146,  -396,  -345,  -471,  -865,
    -865,  -865,  -864,  -865,  -865,  -229,  -865,    18,  -865,  -865,
    -865,  -865,  -551,   -12,  -865,  -865,  -865,  -865,  -865,  -186,
    -185,  -184,  -865,  -182,  -865,  -865,   983,  -865,  -865,  -865,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
    -549,  -865,  -865,   -52,  -865,   117,  -568,  -865,  -449,  -865,
    -865,  -865,  -848,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
    -865,   509,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -138,
     -88,  -865,   -89,  -865,  -865,  -865,  -865,  -865,   412,  -865,
     335,  -865,  -418,   635,  -564,  -565,   356,  -865,  -865,  -445,
    -865,  -865,   949,  -865,  -865,  -865,   544,   119,  -865,  -865,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
    -865,  -865,  -438,  -118,  -865,   645,  -865,   646,  -865,  -865,
    -865,  -865,  -266,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,  -865,
    -865,  -865,  -865,  -865,  -865,  -865,  -865,   -77,  -865,  -865,
    -865,  -865,  -865,   647,  -679,  -497,  -667,  -865,   113,   310,
     486,  -865,  -865,  -865,  -147,  -865
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   127,    47,    14,   145,   151,   516,   404,   898,
     405,   700,    15,    16,    39,    40,    41,    77,    17,    34,
      35,   602,   603,  1067,  1068,   604,  1132,  1165,   605,   606,
     961,   607,   608,   609,   610,   611,  1062,   152,   153,    30,
      31,    32,    56,    57,    58,    59,    18,   326,   453,   230,
      19,    96,   231,    97,   137,   406,   612,   454,   613,   407,
     696,  1102,   614,   408,   615,   488,   661,  1083,   409,   616,
     617,   618,   619,   620,   621,   622,   623,   807,   624,   410,
     666,  1087,   667,  1088,   669,  1089,   411,   664,  1086,   412,
     709,  1112,   413,   911,   912,   712,   414,   415,   777,   626,
     416,   417,   706,   418,   901,  1153,   902,  1180,   419,   758,
    1056,   710,  1041,  1182,  1043,  1183,  1119,  1201,   421,   449,
    1080,  1169,  1077,   990,   632,   833,  1167,  1195,   450,   451,
     483,   484,   270,   271,   878,   575,   642,   468,   344,   345,
     237,   340,    67,   106,    21,   142,   336,    44,    78,    80,
      22,   100,   139,    23,   459,   235,   236,    65,   103,    24,
     140,   338,   576,   422,   335,   193,   194,   200,   195,   352,
     845,   470,   196,   350,   844,   347,   840,   348,   841,   349,
     843,   353,   846,   354,   995,   355,   848,   356,   996,   357,
     850,   358,   997,   359,   853,   360,   855,   485,    25,    49,
     129,    26,    48,   423,   684,   685,   686,   424,   687,   513,
     688,   425,   514,   426,  1175,   427
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,   437,   328,   192,   239,   625,   651,   763,   653,   477,
     655,   479,   771,   481,   691,   761,   633,   527,   883,     2,
    -112,   107,   108,   569,   890,     3,   679,  1131,   904,   793,
     794,   679,   428,   808,   776,   132,   528,   529,    27,    28,
    1110,  1078,    60,    60,   197,   790,   766,   766,     4,    42,
       5,    73,     6,   438,   525,   875,   430,   888,     7,   907,
     944,    89,    90,    91,   456,   269,   447,     8,   908,    66,
     678,   238,   689,     9,   267,  1078,  1134,    60,    60,    60,
      60,   528,   529,   540,   541,    74,    33,   905,   267,   526,
     123,   838,   643,    10,   439,  1079,   767,   768,   768,   769,
     645,   267,   770,    42,    68,   457,   568,   327,   809,   191,
     909,   876,   639,   858,    11,   910,   448,  1093,   509,   511,
     456,   640,   131,   530,   531,   532,   533,   534,    45,  1079,
     535,   536,   537,   538,   572,   539,   540,   541,   542,   903,
     146,   147,   543,    43,   544,   545,   774,    92,    50,   192,
     546,   547,   548,    94,   561,   562,   549,   528,   529,    95,
     109,   828,    79,   641,   110,    46,   111,    89,   112,    91,
     532,   533,    93,   882,   429,   766,    51,    85,   538,    29,
     539,   540,   541,   542,   877,   341,   550,   543,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   431,   124,
     638,   113,   461,  1015,   767,   830,   767,   561,   562,   767,
      75,    12,  1012,   831,   905,   656,   432,    13,    69,   538,
     941,    76,   540,   541,   945,   433,   768,   648,   573,   192,
     192,   192,   192,   671,   905,   192,   832,   192,   955,   192,
     963,   192,   569,   462,  1099,   574,   532,   533,    52,   649,
      36,    37,   561,   562,   538,   191,   539,   540,   541,   542,
     766,    86,    87,   543,  1126,    69,   192,   192,   766,   877,
     469,   471,   472,   473,    53,    85,   476,  1096,   478,   148,
     480,    61,    62,   149,    63,   150,   992,   112,   528,   529,
     342,   629,   202,   561,   562,    52,   133,   134,   135,   136,
      54,    69,   232,  1194,   702,  1179,   243,   343,   694,    55,
     695,   768,    64,   905,   556,   557,   558,   559,   560,   768,
     630,    53,   192,   528,   529,   905,  1008,   905,   561,   562,
     233,  1207,  1200,   244,   192,   191,   191,   191,   191,  1021,
     906,   191,    52,   191,  1094,   191,   881,   191,   192,  1038,
      70,   191,   935,   905,   999,  1100,   905,    71,   779,   384,
     385,   386,   962,   567,    98,    52,   788,   913,    53,  1046,
      99,  1032,   191,   191,  1039,   579,  1059,   532,   533,    92,
    1117,   192,  1011,  1144,   487,   538,   520,   539,   540,   541,
     542,    53,  1022,    42,   543,  1060,    36,    37,   752,   753,
     863,   864,   573,   866,  1074,   657,  1070,  1071,    82,  -627,
     716,   720,   532,   533,  -627,  1072,  1073,   257,    38,   574,
     538,   143,   144,   540,   541,   542,   754,   658,   191,   543,
     895,   659,  -627,  -634,    89,    90,    91,   755,  -634,  1084,
     191,   329,  1040,   258,   259,   330,   631,   558,   559,   560,
    1097,   267,    83,   660,   191,   646,  -634,    52,  1092,   561,
     562,  1085,   331,   332,   333,   334,   702,  -641,  -342,   756,
     757,   703,  -641,  -342,  1064,   498,   101,   729,   926,   663,
     327,   930,   102,    53,    84,  1065,  1066,   191,   730,   927,
    -641,  -342,   931,   577,   561,   562,   578,   104,   -64,   704,
     260,   125,   465,   105,   261,   466,    85,   126,   467,   528,
     529,  1075,   705,   447,   886,   116,   267,  1076,   238,   887,
     647,   267,   267,    76,   267,   650,   652,   192,   654,   267,
     267,   267,   267,   998,  1002,  1158,  1184,   887,   192,   891,
     192,   240,   241,   192,   943,    88,   192,   118,   192,   192,
     121,   192,   122,   951,   128,   262,   130,  1031,   138,   263,
    1106,   868,   264,   240,   241,   242,   958,    74,   842,   141,
     960,  1170,   143,   144,   697,   698,   198,   265,   192,   852,
     402,   792,   199,   327,   856,   201,   266,   861,   810,   811,
     812,   813,   814,   815,   816,   817,   530,   531,   532,   533,
     534,   818,   819,   535,   536,   537,   538,   820,   539,   540,
     541,   542,   119,   120,   625,   543,   821,   544,   545,   822,
     823,    89,   924,   546,   547,   548,   824,   825,   826,   549,
     234,   238,   245,   191,   246,  1030,   528,   529,   249,   247,
     248,   250,   251,   252,   191,   253,   191,   254,   255,   191,
     267,   192,   191,   268,   191,   191,   256,   191,   827,   550,
     257,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   323,   257,   269,   325,   324,   327,   337,   351,   339,
     561,   562,   440,   441,   191,   257,   258,   259,   442,   443,
     444,   445,   940,   446,   452,   989,   458,   455,   258,   259,
     460,   463,   464,   474,   491,   570,   492,   493,   494,   495,
     496,   258,   259,   497,   498,   499,   502,   515,   503,   565,
    1061,   504,   505,   530,   531,   532,   533,   534,    12,   506,
     535,   536,   537,   538,   634,   539,   540,   541,   542,   507,
     524,   564,   543,   260,   544,   545,   571,   261,   636,   637,
     546,   547,   548,   644,   662,   260,   549,   191,   639,   261,
     711,   528,   529,   727,   764,   420,   765,   640,   260,   784,
     783,   785,   261,   796,   435,  1113,   797,   798,   800,   801,
     802,   803,   804,   192,   805,   806,   550,   835,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   262,   257,
     847,   438,   263,   849,  1003,   264,   851,   561,   562,   641,
     262,  1111,   881,   257,   263,   327,   884,   264,    60,   885,
     265,   894,   897,   262,   903,   258,   259,   263,   899,   266,
     264,   914,   265,   900,   916,  1129,   933,   915,   836,   258,
     259,   266,   917,   918,   919,   265,   929,   938,   530,   531,
     532,   533,   534,   939,   266,   535,   536,   537,   538,   808,
     539,   540,   541,   542,   949,   952,   953,   543,   954,   544,
     545,   957,   959,   988,  1007,   546,   547,   548,   991,  1000,
    1016,   549,   260,  1014,   500,   501,   261,  1034,  1027,   191,
    1017,  1019,  1023,  1024,  1035,   512,   260,  1163,  1036,  1037,
     261,  1164,  1048,  1049,   517,   518,   519,  1051,   521,   522,
     523,   550,  1054,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   192,  1058,   192,  1090,  1081,  1107,  1124,
    1138,  1095,   561,   562,  1151,  1098,  1101,   262,  1109,   986,
    1146,   263,  1133,  1004,   264,  1147,  1166,  1168,  1154,  1172,
    1176,   262,  1178,  1197,  1198,   263,  1199,  1005,   264,   265,
    1206,  1210,   114,   627,   699,    72,  1115,  1211,   266,   117,
     154,  1185,   635,   265,  1188,   115,   987,  1063,  1214,  1189,
    1190,  1191,   266,  1192,    20,  1006,  1108,   528,   529,  1135,
     772,  1136,   346,   854,   837,   486,    81,   692,   475,   628,
    1020,   192,   665,  1177,   893,   668,     0,   670,   489,     0,
     490,   672,   673,   674,   675,   676,   677,     0,   683,     0,
     683,     0,     0,     0,     0,     0,     0,     0,     0,   191,
       0,   191,     0,     0,     0,     0,     0,     0,     0,   721,
     722,     0,  1159,   723,   724,   725,   726,     0,   728,     0,
     731,   732,   733,   734,   735,   736,   737,   738,   739,   741,
     742,   743,   744,   745,   746,   747,   748,   749,   750,   751,
       0,   759,     0,     0,   530,   531,   532,   533,   534,     0,
     762,   535,   536,   537,   538,     0,   539,   540,   541,   542,
     773,     0,     0,   543,   778,   544,   545,   781,   782,     0,
       0,     0,     0,   787,     0,   791,     0,   191,   795,   257,
       0,     0,     0,   799,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   258,   259,     0,   829,     0,
       0,   705,   554,   555,   556,   557,   558,   559,   560,     0,
       0,   839,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,     0,     0,     0,     0,     0,     0,   705,     0,
     155,     0,   860,     0,     0,     0,   156,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     517,   523,   260,   157,     0,     0,   261,     0,     0,   523,
       0,     0,   892,   512,     0,     0,     0,   896,   158,     0,
       0,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,     0,     0,     0,     0,     0,   262,     0,     0,
       0,   263,     0,  1025,   264,     0,     0,     0,     0,     0,
       0,     0,   928,     0,     0,     0,   932,     0,     0,   265,
       0,     0,     0,   937,     0,   257,     0,     0,   266,    52,
     942,     0,   528,   529,   946,     0,     0,     0,     0,   950,
       0,     0,   190,     0,     0,     0,     0,     0,   956,     0,
       0,   258,   259,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   257,     0,   964,
     965,   966,   967,   968,   969,   970,   971,   972,   973,   976,
     977,   978,   979,   980,   981,   982,   985,     0,     0,     0,
       0,     0,     0,   258,   259,     0,   993,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
       0,     0,   261,     0,     0,     0,     0,     0,     0,   530,
     531,   532,   533,     0,     0,     0,     0,     0,     0,   538,
       0,   539,   540,   541,   542,     0,  1009,  1010,   543,  1013,
     544,   545,     0,     0,  1018,     0,   683,     0,     0,     0,
     260,     0,     0,     0,   261,     0,     0,     0,  1028,  1029,
       0,     0,     0,   262,  1033,     0,     0,   263,     0,  1105,
     264,     0,     0,     0,  1042,     0,  1044,     0,  1047,   528,
     529,     0,     0,     0,  1050,   265,     0,     0,  1053,   556,
     557,   558,   559,   560,   266,     0,     0,     0,     0,     0,
       0,     0,     0,   561,   562,   262,   976,   985,     0,   263,
       0,  1157,   264,     0,     0,     0,     0,     0,     0,     0,
    1069,     0,     0,     0,     0,     0,     0,   265,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   810,   811,
     812,   813,   814,   815,   816,   817,   530,   531,   532,   533,
     534,   818,   819,   535,   536,   537,   538,   947,   539,   540,
     541,   542,     0,     0,     0,   543,   821,   544,   545,   822,
     823,     0,     0,   546,   547,   548,   824,   825,   826,   549,
       0,     0,     0,     0,     0,     0,     0,     0,  1120,     0,
    1121,     0,     0,  1123,     0,     0,     0,     0,  1125,     0,
       0,  1127,   627,     0,     0,     0,  1130,     0,   948,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,  1137,     0,     0,  1139,  1140,  1141,  1143,
     561,   562,  1145,     0,     0,  1148,  1149,   994,  1150,     0,
       0,  1152,     0,     0,     0,     0,     0,     0,     0,  1156,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   580,     0,     0,     0,     0,     3,     0,   581,
     582,   583,     0,   584,  1181,   361,   362,   363,   364,   365,
       0,     0,     0,     0,  1193,     0,   585,   366,   586,   587,
       0,  1196,     0,     0,     0,     0,     0,     0,   588,   367,
       0,   589,     0,   590,   368,  1203,  1205,   369,     0,     8,
     370,   591,     0,   592,   371,     0,  1209,   593,   594,     0,
       0,  1212,     0,  1213,   595,     0,   159,   160,   161,     0,
     163,   164,   165,   166,   167,   373,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,   179,   180,   181,     0,
       0,   184,   185,   186,   187,     0,     0,   374,   596,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   597,   598,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   379,   380,   381,     0,   382,     0,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
      53,   599,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,   396,
     397,     0,   398,     0,     0,   399,   400,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,   402,   403,   600,
     327,   601,   580,     0,     0,     0,     0,     3,     0,   581,
     582,   583,     0,   584,     0,   361,   362,   363,   364,   365,
       0,     0,     0,     0,     0,     0,   585,   366,   586,   587,
       0,     0,     0,     0,     0,     0,     0,     0,   588,   367,
       0,   589,     0,   590,   368,     0,     0,   369,     0,     8,
     370,   591,     0,   592,   371,     0,     0,   593,   594,     0,
       0,     0,     0,     0,   595,     0,   159,   160,   161,     0,
     163,   164,   165,   166,   167,   373,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,   179,   180,   181,     0,
       0,   184,   185,   186,   187,     0,     0,   374,   596,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   597,   598,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   379,   380,   381,     0,   382,     0,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
      53,   599,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,   396,
     397,     0,   398,     0,     0,   399,   400,     0,     0,     0,
     361,   362,   363,   364,   365,   401,     0,   402,   403,   600,
     327,  1128,   366,     0,     0,     0,     0,     0,   257,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,   368,
       0,     0,   369,     0,     0,   370,     0,   679,     0,   371,
       0,     0,     0,     0,   258,   259,     0,     0,     0,   372,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,   260,     0,     0,     0,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,   680,   393,   394,     0,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
     263,     0,     0,   681,   396,   397,     0,   398,     0,     0,
     399,   400,   361,   362,   363,   364,   365,     0,   265,     0,
     682,     0,   402,   403,   366,   327,     0,   266,     0,     0,
     257,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,   368,     0,     0,   369,     0,     0,   370,     0,     0,
       0,   371,     0,     0,     0,     0,   258,   259,     0,     0,
       0,   372,     0,   159,   160,   161,     0,   163,   164,   165,
     166,   167,   373,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,   179,   180,   181,     0,     0,   184,   185,
     186,   187,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   376,     0,
       0,     0,     0,   260,     0,     0,     0,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,   380,   381,     0,   382,     0,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,   680,   393,   394,
       0,     0,     0,     0,     0,     0,     0,     0,   262,     0,
       0,     0,   263,     0,     0,   681,   396,   397,     0,   398,
       0,     0,   399,   400,   361,   362,   363,   364,   365,     0,
     265,     0,   690,     0,   402,   403,   366,   327,     0,   266,
       0,     0,   430,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,   368,     0,     0,   369,     0,     0,   370,
       0,     0,     0,   371,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   372,     0,   159,   160,   161,     0,   163,
     164,   165,   166,   167,   373,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,   179,   180,   181,     0,     0,
     184,   185,   186,   187,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
     376,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   377,   378,   379,   380,   381,     0,   382,     0,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,    53,
     393,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,     0,     0,   395,   396,   397,
       0,   398,     0,     0,   399,   400,     0,     0,     0,     0,
       0,     0,   432,     0,   401,     0,   402,   403,     0,   327,
       0,   433,   361,   362,   363,   364,   365,     0,     0,     0,
       0,     0,     0,  1186,   366,   586,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,   368,     0,     0,   369,     0,     0,   370,   591,     0,
       0,   371,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   372,     0,   159,   160,   161,     0,   163,   164,   165,
     166,   167,   373,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,   179,   180,   181,     0,     0,   184,   185,
     186,   187,     0,     0,   374,  1187,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,   380,   381,     0,   382,     0,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,    53,   393,   394,
       0,   361,   362,   363,   364,   365,     0,     0,     0,     0,
       0,     0,     0,   366,     0,   395,   396,   397,     0,   398,
       0,     0,   399,   400,     0,   367,     0,     0,     0,     0,
     368,     0,   401,   369,   402,   403,   370,   327,     0,     0,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,     0,   159,   160,   161,     0,   163,   164,   165,   166,
     167,   373,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,   179,   180,   181,     0,     0,   184,   185,   186,
     187,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,   376,     0,     0,
       0,     0,     0,     0,     0,   775,     0,     0,     0,     0,
       0,   597,   598,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   377,   378,
     379,   380,   381,     0,   382,     0,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,    53,   393,   394,     0,
     361,   362,   363,   364,   365,     0,     0,     0,     0,     0,
       0,     0,   366,     0,   395,   396,   397,     0,   398,     0,
       0,   399,   400,     0,   367,     0,     0,     0,     0,   368,
       0,   401,   369,   402,   403,   370,   327,     0,     0,   371,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   372,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,   789,     0,     0,     0,     0,     0,
     597,   598,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,   393,   394,     0,   361,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,   366,     0,   395,   396,   397,     0,   398,     0,     0,
     399,   400,     0,   367,     0,     0,     0,     0,   368,     0,
     401,   369,   402,   403,   370,   327,     0,     0,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   380,
     381,     0,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,    53,   393,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,   396,   397,     0,   398,     0,     0,   399,
     400,   361,   362,   363,   364,   365,     0,   707,     0,   401,
     708,   402,   403,   366,   327,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,     0,     0,     0,     0,
     368,     0,     0,   369,     0,     0,   370,     0,     0,     0,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,     0,   159,   160,   161,     0,   163,   164,   165,   166,
     167,   373,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,   179,   180,   181,     0,     0,   184,   185,   186,
     187,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,   598,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   377,   378,
     379,   380,   381,     0,   382,     0,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,    53,   393,   394,     0,
     361,   362,   363,   364,   365,     0,     0,     0,     0,     0,
       0,     0,   366,     0,   395,   396,   397,     0,   398,     0,
       0,   399,   400,     0,   367,     0,     0,     0,     0,   368,
       0,   401,   369,   402,   403,   370,   327,     0,     0,   371,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   372,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     974,   975,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,   393,   394,     0,   361,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,   366,     0,   395,   396,   397,     0,   398,     0,     0,
     399,   400,     0,   367,     0,     0,     0,     0,   368,     0,
     401,   369,   402,   403,   370,   327,     0,     0,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   983,
     984,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   380,
     381,     0,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,    53,   393,   394,     0,   361,   362,
     363,   364,   365,     0,     0,     0,     0,     0,     0,     0,
     366,     0,   395,   396,   397,     0,   398,     0,     0,   399,
     400,     0,   367,     0,     0,     0,     0,   368,     0,   401,
     369,   402,   403,   370,   327,     0,     0,   371,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   372,     0,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    53,   393,   394,     0,   361,   362,   363,
     364,   365,     0,     0,   740,     0,     0,     0,     0,   366,
       0,   395,   396,   397,     0,   398,     0,     0,   399,   400,
       0,   367,     0,     0,     0,     0,   368,   434,   401,   369,
     402,   403,   370,   327,     0,     0,   371,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   372,     0,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   373,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,   179,   180,
     181,     0,     0,   184,   185,   186,   187,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,   376,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   377,   378,   379,   380,   381,     0,
     382,     0,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,    53,   393,   394,     0,   361,   362,   363,   364,
     365,     0,     0,     0,     0,     0,     0,     0,   366,     0,
     395,   396,   397,     0,   398,     0,     0,   399,   400,     0,
     367,     0,     0,     0,     0,   368,     0,   401,   369,   402,
     403,   370,   327,     0,     0,   371,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   159,   160,   161,
       0,   163,   164,   165,   166,   167,   373,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,   179,   180,   181,
       0,     0,   184,   185,   186,   187,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   377,   378,   379,   380,   381,     0,   382,
       0,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,    53,   393,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
     396,   397,     0,   398,     0,     0,   399,   400,   361,   362,
     363,   364,   365,     0,     0,     0,   401,   760,   402,   403,
     366,   327,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,   368,     0,     0,
     369,     0,     0,   370,     0,     0,     0,   371,     0,     0,
       0,     0,     0,   780,     0,     0,     0,   372,     0,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    53,   393,   394,     0,   361,   362,   363,
     364,   365,     0,     0,     0,     0,     0,     0,     0,   366,
       0,   395,   396,   397,     0,   398,     0,     0,   399,   400,
       0,   367,     0,     0,     0,     0,   368,     0,   401,   369,
     402,   403,   370,   327,     0,     0,   371,     0,     0,   786,
       0,     0,     0,     0,     0,     0,   372,     0,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   373,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,   179,   180,
     181,     0,     0,   184,   185,   186,   187,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,   376,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   377,   378,   379,   380,   381,     0,
     382,     0,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,    53,   393,   394,     0,   361,   362,   363,   364,
     365,     0,     0,     0,     0,     0,     0,     0,   366,     0,
     395,   396,   397,     0,   398,     0,     0,   399,   400,     0,
     367,     0,     0,     0,     0,   368,     0,   401,   369,   402,
     403,   370,   327,     0,     0,   371,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   159,   160,   161,
       0,   163,   164,   165,   166,   167,   373,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,   179,   180,   181,
       0,     0,   184,   185,   186,   187,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   377,   378,   379,   380,   381,     0,   382,
       0,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,    53,   393,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   515,     0,   395,
     396,   397,     0,   398,     0,     0,   399,   400,   361,   362,
     363,   364,   365,     0,     0,     0,   401,     0,   402,   403,
     366,   327,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,   368,     0,     0,
     369,     0,     0,   370,     0,     0,     0,   371,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   372,     0,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    53,   393,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,   396,   397,     0,   398,     0,     0,   399,   400,
     361,   362,   363,   364,   365,     0,     0,     0,   401,   857,
     402,   403,   366,   327,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,   368,
       0,     0,   369,     0,     0,   370,     0,     0,     0,   371,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   372,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,   393,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   859,     0,   395,   396,   397,     0,   398,     0,     0,
     399,   400,   361,   362,   363,   364,   365,     0,     0,     0,
     401,     0,   402,   403,   366,   327,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,   368,     0,     0,   369,     0,     0,   370,     0,     0,
       0,   371,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   372,     0,   159,   160,   161,     0,   163,   164,   165,
     166,   167,   373,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,   179,   180,   181,     0,     0,   184,   185,
     186,   187,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,   380,   381,     0,   382,     0,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,    53,   393,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,   396,   397,     0,   398,
       0,     0,   399,   400,   361,   362,   363,   364,   365,     0,
       0,     0,   401,   879,   402,   403,   366,   327,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,   368,     0,     0,   369,     0,     0,   370,
       0,     0,     0,   371,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   372,     0,   159,   160,   161,     0,   163,
     164,   165,   166,   167,   373,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,   179,   180,   181,     0,     0,
     184,   185,   186,   187,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
     376,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   377,   378,   379,   380,   381,     0,   382,     0,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,    53,
     393,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,   396,   397,
       0,   398,     0,     0,   399,   400,   361,   362,   363,   364,
     365,     0,     0,     0,   401,   889,   402,   403,   366,   327,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,     0,     0,     0,     0,   368,     0,     0,   369,     0,
       0,   370,     0,     0,     0,   371,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   159,   160,   161,
       0,   163,   164,   165,   166,   167,   373,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,   179,   180,   181,
       0,     0,   184,   185,   186,   187,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   377,   378,   379,   380,   381,     0,   382,
       0,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,    53,   393,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
     396,   397,     0,   398,     0,     0,   399,   400,   361,   362,
     363,   364,   365,     0,     0,     0,   401,  1045,   402,   403,
     366,   327,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,     0,     0,     0,     0,   368,     0,     0,
     369,     0,     0,   370,     0,     0,     0,   371,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   372,     0,   159,
     160,   161,     0,   163,   164,   165,   166,   167,   373,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   179,
     180,   181,     0,     0,   184,   185,   186,   187,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   377,   378,   379,   380,   381,
       0,   382,     0,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,    53,   393,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,   396,   397,     0,   398,     0,     0,   399,   400,
     361,   362,   363,   364,   365,     0,     0,     0,   401,  1091,
     402,   403,   366,   327,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,   368,
       0,     0,   369,     0,     0,   370,     0,     0,     0,   371,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   372,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,   393,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,   396,   397,     0,   398,     0,     0,
     399,   400,   361,   362,   363,   364,   365,     0,     0,     0,
     401,  1142,   402,   403,   366,   327,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   367,     0,     0,     0,
       0,   368,     0,     0,   369,     0,     0,   370,     0,     0,
       0,   371,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   372,     0,   159,   160,   161,     0,   163,   164,   165,
     166,   167,   373,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,   179,   180,   181,     0,     0,   184,   185,
     186,   187,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   376,     0,
       0,     0,     0,     0,     0,     0,  1155,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   379,   380,   381,     0,   382,     0,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,    53,   393,   394,
       0,   361,   362,   363,   364,   365,     0,     0,     0,     0,
       0,     0,     0,   366,     0,   395,   396,   397,     0,   398,
       0,     0,   399,   400,     0,   367,     0,     0,     0,     0,
     368,     0,   401,   369,   402,   403,   370,   327,     0,     0,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,     0,   159,   160,   161,     0,   163,   164,   165,   166,
     167,   373,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,   179,   180,   181,     0,     0,   184,   185,   186,
     187,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,   376,     0,     0,
       0,     0,     0,     0,     0,  1202,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   377,   378,
     379,   380,   381,     0,   382,     0,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,    53,   393,   394,     0,
     361,   362,   363,   364,   365,     0,     0,     0,     0,     0,
       0,     0,   366,     0,   395,   396,   397,     0,   398,     0,
       0,   399,   400,     0,   367,     0,     0,     0,     0,   368,
       0,   401,   369,   402,   403,   370,   327,     0,     0,   371,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   372,
       0,   159,   160,   161,     0,   163,   164,   165,   166,   167,
     373,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,   179,   180,   181,     0,     0,   184,   185,   186,   187,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   376,     0,     0,     0,
       0,     0,     0,     0,  1204,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   377,   378,   379,
     380,   381,     0,   382,     0,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,    53,   393,   394,     0,   361,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,   366,     0,   395,   396,   397,     0,   398,     0,     0,
     399,   400,     0,   367,     0,     0,     0,     0,   368,     0,
     401,   369,   402,   403,   370,   327,     0,     0,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,     0,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   528,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   380,
     381,     0,   382,     0,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,    53,   393,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,   396,   397,     0,   398,     0,     0,   399,
     400,     0,     0,     0,     0,     0,     0,     0,     0,   401,
       0,   402,   403,     0,   327,   810,   811,   812,   813,   814,
     815,   816,   817,   530,   531,   532,   533,   534,   818,   819,
     535,   536,   537,   538,   947,   539,   540,   541,   542,   528,
     529,     0,   543,   821,   544,   545,   822,   823,     0,     0,
     546,   547,   548,   824,   825,   826,   549,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   948,   550,     0,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   561,   562,     0,
       0,     0,     0,   713,  1082,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
     159,   160,   161,     0,   163,   164,   165,   166,   167,   373,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     179,   180,   181,     0,     0,   184,   185,   186,   187,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,     0,     0,     0,  1174,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,   714,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,   715,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,   717,     0,     0,
       0,     0,   561,   562,     0,     0,   563,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,   159,   160,   161,     0,   163,   164,
     165,   166,   167,   373,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,   179,   180,   181,     0,     0,   184,
     185,   186,   187,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,   701,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,   718,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,   719,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,   921,     0,     0,     0,     0,   561,   562,     0,     0,
     834,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,   159,   160,
     161,     0,   163,   164,   165,   166,   167,   373,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,   179,   180,
     181,     0,     0,   184,   185,   186,   187,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,   862,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,   922,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,   923,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,   865,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,     0,     0,   867,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,   869,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,     0,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,     0,     0,
     870,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,   871,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,   872,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,     0,     0,   873,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,   874,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,     0,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,     0,     0,
     936,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,  1001,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,  1057,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,     0,     0,  1103,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,  1104,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,     0,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,     0,     0,
    1114,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,  1116,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,  1118,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,     0,     0,  1122,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,  1160,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,     0,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,     0,     0,
    1161,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,  1162,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,     0,     0,  1171,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,     0,     0,  1173,     0,     0,     0,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,  1208,     0,
       0,     0,   530,   531,   532,   533,   534,   528,   529,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,     0,     0,   546,
     547,   548,     0,     0,     0,   549,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   550,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,   566,     0,
       0,     0,     0,     0,   530,   531,   532,   533,   534,   528,
     529,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
     920,     0,     0,     0,     0,     0,   530,   531,   532,   533,
     534,   528,   529,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
       0,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,   562,   934,     0,     0,     0,     0,     0,   530,   531,
     532,   533,   534,   528,   529,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   562,  1052,     0,     0,     0,     0,     0,
     530,   531,   532,   533,   534,     0,     0,   535,   536,   537,
     538,     0,   539,   540,   541,   542,   528,   529,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
       0,     0,     0,   549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,  1055,     0,     0,     0,
       0,     0,     0,     0,     0,   810,   811,   812,   813,   814,
     815,   816,   817,   530,   531,   532,   533,   534,   818,   819,
     535,   536,   537,   538,   947,   539,   540,   541,   542,  -310,
     272,   273,   543,   821,   544,   545,   822,   823,     0,     0,
     546,   547,   548,   824,   825,   826,   549,   274,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   948,   550,     0,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   561,   562,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   528,   529,   293,
     294,   295,     0,     0,     0,     0,     0,   296,   297,   298,
     299,   300,     0,     0,   301,   302,   303,   304,   305,   306,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,     0,     0,   319,   320,     0,     0,     0,     0,
       0,   321,   322,     0,     0,     0,   528,   529,     0,     0,
       0,     0,     0,     0,   530,   531,   532,   533,   534,     0,
       0,   535,   536,   537,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,   693,
       0,   546,   547,   548,     0,     0,     0,   549,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   550,     0,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,     0,     0,   530,   531,   532,   533,   534,   561,   562,
     535,   536,   537,   538,     0,   539,   540,   541,   542,   528,
     529,     0,   543,     0,   544,   545,     0,     0,   880,     0,
     546,   547,   548,     0,     0,     0,   549,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   550,     0,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   561,   562,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,   529,
       0,     0,     0,     0,     0,     0,   530,   531,   532,   533,
     534,     0,     0,   535,   536,   537,   538,     0,   539,   540,
     541,   542,     0,     0,     0,   543,     0,   544,   545,     0,
       0,     0,     0,   546,   547,   548,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   550,
     925,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,     0,     0,   530,   531,   532,   533,   534,
     561,   562,   535,   536,   537,   538,     0,   539,   540,   541,
     542,   528,   529,     0,   543,     0,   544,   545,     0,     0,
       0,     0,   546,   547,   548,     0,     0,     0,   549,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1026,     0,     0,     0,   550,     0,
     551,   552,   553,   554,   555,   556,   557,   558,   559,   560,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   561,
     562,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     528,   529,     0,     0,     0,     0,     0,     0,   530,   531,
     532,   533,   534,     0,     0,   535,   536,   537,   538,     0,
     539,   540,   541,   542,     0,     0,     0,   543,     0,   544,
     545,     0,     0,     0,     0,   546,   547,   548,     0,     0,
       0,   549,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   550,     0,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,     0,     0,   530,   531,   532,
     533,   534,   561,   562,   535,   536,   537,   538,     0,   539,
     540,   541,   542,   528,   529,     0,   543,     0,   544,   545,
       0,     0,     0,     0,   546,   547,   548,     0,     0,     0,
    -642,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   528,   529,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     550,     0,   551,   552,   553,   554,   555,   556,   557,   558,
     559,   560,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   561,   562,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,   531,   532,   533,   534,     0,     0,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,   547,   548,
     530,   531,   532,   533,   534,   528,   529,   535,   536,   537,
     538,     0,   539,   540,   541,   542,     0,     0,     0,   543,
       0,   544,   545,     0,     0,     0,     0,   546,     0,   548,
       0,     0,     0,   550,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,   528,   529,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   561,   562,     0,     0,     0,     0,
       0,     0,   530,   531,   532,   533,   534,     0,     0,   535,
     536,   537,   538,     0,   539,   540,   541,   542,     0,     0,
       0,   543,     0,   544,   545,     0,     0,     0,     0,   546,
       0,     0,     0,     0,   528,   529,     0,     0,     0,     0,
       0,     0,   530,   531,   532,   533,   534,     0,     0,   535,
     536,   537,   538,     0,   539,   540,   541,   542,   528,   529,
       0,   543,     0,   544,   545,     0,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   561,   562,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,     0,
       0,   530,   531,   532,   533,   534,   561,   562,   535,   536,
     537,   538,     0,   539,   540,   541,   542,   528,   529,     0,
     543,     0,   544,   545,     0,   530,   531,   532,   533,   534,
       0,     0,   535,   536,   537,   538,     0,   539,   540,   541,
     542,     0,     0,     0,   543,     0,   544,   545,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   552,   553,   554,
     555,   556,   557,   558,   559,   560,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   561,   562,     0,     0,     0,
       0,     0,   553,   554,   555,   556,   557,   558,   559,   560,
       0,     0,     0,     0,   530,   531,   532,   533,   534,   561,
     562,   535,     0,     0,   538,     0,   539,   540,   541,   542,
       0,     0,     0,   543,     0,   544,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   508,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   155,     0,     0,     0,
       0,     0,   156,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   554,   555,   556,   557,   558,   559,   560,   157,
       0,     0,     0,     0,     0,     0,     0,     0,   561,   562,
       0,     0,     0,     0,   158,     0,     0,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   510,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   155,     0,     0,     0,     0,     0,   156,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,   157,     0,     0,     0,   190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   158,
       0,    53,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   155,     0,     0,     0,
       0,     0,   156,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,   157,
       0,     0,     0,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   158,     0,    53,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   155,     0,     0,     0,     0,     0,   156,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,   157,     0,     0,     0,   190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   158,
       0,    53,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   203,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,     0,     0,     0,   204,     0,   205,
       0,   206,   207,   208,   209,   210,   482,   211,   212,   213,
     214,   215,   216,   217,   218,   219,     0,   220,   221,   222,
       0,     0,   223,   224,   225,   226,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   227,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229
};

static const yytype_int16 yycheck[] =
{
      12,   267,   231,   121,   142,   454,   477,   572,   479,   354,
     481,   356,   576,   358,   511,   564,   461,   413,   685,     0,
       7,    14,    15,   441,   691,     6,    51,    11,   707,   597,
     598,    51,    32,   123,   585,    88,    20,    21,    18,    19,
     132,    44,    54,    55,   121,   596,   118,   118,    29,   159,
      31,    21,    33,   159,   159,    19,    32,    19,    39,   118,
      19,   134,   135,   136,   159,   159,   145,    48,   127,    60,
     508,   150,   510,    54,   172,    44,    45,    89,    90,    91,
      92,    20,    21,   120,   121,    55,   153,   168,   172,   194,
     200,   642,   176,    74,   200,    98,   168,   169,   169,   171,
     198,   172,   174,   159,   169,   200,   200,   199,   198,   121,
     169,   679,   118,   662,    95,   174,   195,   198,   384,   385,
     159,   127,   195,   107,   108,   109,   110,   111,   159,    98,
     114,   115,   116,   117,    48,   119,   120,   121,   122,   159,
      14,    15,   126,   199,   128,   129,   584,   168,    33,   267,
     134,   135,   136,    55,   191,   192,   140,    20,    21,    61,
     153,   200,    43,   169,   157,   196,   159,   134,   161,   136,
     109,   110,   193,   198,   174,   118,    61,   133,   117,   159,
     119,   120,   121,   122,   127,   238,   170,   126,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   174,    80,
     466,   194,   340,   882,   168,   192,   168,   191,   192,   168,
     180,   192,   879,   200,   168,   171,   192,   198,   168,   117,
     771,   191,   120,   121,   775,   201,   169,   176,   142,   347,
     348,   349,   350,   499,   168,   353,   632,   355,   789,   357,
     808,   359,   660,   193,   198,   159,   109,   110,   133,   198,
     159,   160,   191,   192,   117,   267,   119,   120,   121,   122,
     118,    54,    55,   126,   198,   168,   384,   385,   118,   127,
     347,   348,   349,   350,   159,   133,   353,   127,   355,   153,
     357,     4,     5,   157,     7,   159,   837,   161,    20,    21,
     142,   169,   195,   191,   192,   133,    89,    90,    91,    92,
     185,   168,   171,  1167,   142,  1153,   168,   159,   198,   194,
     200,   169,    35,   168,   177,   178,   179,   180,   181,   169,
     198,   159,   440,    20,    21,   168,   875,   168,   191,   192,
     199,  1195,  1180,   195,   452,   347,   348,   349,   350,   888,
     195,   353,   133,   355,  1011,   357,   168,   359,   466,   168,
     171,   363,   195,   168,   195,  1022,   168,   168,   587,   150,
     151,   152,   807,   440,    55,   133,   595,   712,   159,   918,
      61,   193,   384,   385,   193,   452,   940,   109,   110,   168,
     195,   499,   879,   195,   175,   117,   398,   119,   120,   121,
     122,   159,   889,   159,   126,   944,   159,   160,   120,   121,
     666,   667,   142,   669,   193,   176,   974,   975,   159,   171,
     528,   529,   109,   110,   176,   983,   984,    32,   181,   159,
     117,   162,   163,   120,   121,   122,   148,   198,   440,   126,
     696,   176,   194,   171,   134,   135,   136,   159,   176,   176,
     452,    69,   913,    58,    59,    73,   458,   179,   180,   181,
    1014,   172,   159,   198,   466,   176,   194,   133,  1007,   191,
     192,   198,    90,    91,    92,    93,   142,   171,   171,   191,
     192,   147,   176,   176,    11,   194,    55,   148,   148,   491,
     199,   148,    61,   159,   159,    22,    23,   499,   159,   159,
     194,   194,   159,   195,   191,   192,   198,    55,     9,   175,
     115,    55,   168,    61,   119,   171,   133,    61,   174,    20,
      21,    55,   524,   145,   193,   153,   172,    61,   150,   198,
     176,   172,   172,   191,   172,   176,   176,   645,   176,   172,
     172,   172,   172,   176,   176,   176,   176,   198,   656,   200,
     658,   162,   163,   661,   773,   194,   664,   159,   666,   667,
     169,   669,   198,   782,   199,   170,   159,   902,   159,   174,
    1031,   176,   177,   162,   163,   164,   795,    55,   645,   199,
     799,  1136,   162,   163,   164,   165,   198,   192,   696,   656,
     196,   197,   159,   199,   661,   199,   201,   664,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    75,    76,  1063,   126,   127,   128,   129,   130,
     131,   134,   740,   134,   135,   136,   137,   138,   139,   140,
      98,   150,   175,   645,   175,   901,    20,    21,   194,   175,
     175,   175,   175,   175,   656,   175,   658,   175,   175,   661,
     172,   769,   664,   198,   666,   667,   194,   669,   169,   170,
      32,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    34,    32,   159,   194,    34,   199,   159,   159,   200,
     191,   192,   171,   198,   696,    32,    58,    59,   159,   159,
     193,    21,   769,   193,   171,   833,   171,   199,    58,    59,
     199,   194,    41,   195,   194,   159,   175,   194,   175,   175,
     194,    58,    59,   175,   194,   175,   194,   175,   194,   193,
     949,   194,   194,   107,   108,   109,   110,   111,   192,   194,
     114,   115,   116,   117,   193,   119,   120,   121,   122,   194,
     197,   194,   126,   115,   128,   129,   194,   119,   159,   159,
     134,   135,   136,   176,   194,   115,   140,   769,   118,   119,
     150,    20,    21,   159,   194,   256,    41,   127,   115,   198,
     159,   153,   119,    36,   265,  1041,   198,   198,     9,   198,
     198,   198,   198,   901,   198,    64,   170,    41,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   170,    32,
     176,   159,   174,   176,   176,   177,   176,   191,   192,   169,
     170,  1040,   168,    32,   174,   199,   193,   177,   830,   168,
     192,   200,   162,   170,   159,    58,    59,   174,   194,   201,
     177,   175,   192,   194,   175,  1064,     1,   194,   198,    58,
      59,   201,   194,   194,   194,   192,   194,   159,   107,   108,
     109,   110,   111,   159,   201,   114,   115,   116,   117,   123,
     119,   120,   121,   122,    13,   169,   171,   126,   153,   128,
     129,   197,   199,     7,   194,   134,   135,   136,   159,   159,
     193,   140,   115,   159,   375,   376,   119,   159,   162,   901,
     193,   193,   200,   193,   159,   386,   115,  1126,   159,   159,
     119,  1130,   175,   194,   395,   396,   397,   194,   399,   400,
     401,   170,   194,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,  1041,    41,  1043,   194,   198,   195,   159,
     159,   193,   191,   192,   200,   193,   193,   170,   193,   198,
     193,   174,   198,   176,   177,   193,   198,    32,   176,   176,
     193,   170,   193,    51,   193,   174,   200,   176,   177,   192,
     198,   193,    68,   454,   516,    38,  1043,   193,   201,    71,
     118,  1164,   463,   192,  1165,    69,   830,   959,  1207,  1165,
    1165,  1165,   201,  1165,     1,   868,  1038,    20,    21,  1077,
     578,  1080,   243,   658,   638,   360,    47,   511,   352,   455,
     887,  1119,   493,  1150,   694,   496,    -1,   498,   363,    -1,
     363,   502,   503,   504,   505,   506,   507,    -1,   509,    -1,
     511,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1041,
      -1,  1043,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   530,
     531,    -1,  1119,   534,   535,   536,   537,    -1,   539,    -1,
     541,   542,   543,   544,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,   559,   560,
      -1,   562,    -1,    -1,   107,   108,   109,   110,   111,    -1,
     571,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
     581,    -1,    -1,   126,   585,   128,   129,   588,   589,    -1,
      -1,    -1,    -1,   594,    -1,   596,    -1,  1119,   599,    32,
      -1,    -1,    -1,   604,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    -1,   629,    -1,
      -1,  1153,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,   642,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1180,    -1,
      24,    -1,   663,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     681,   682,   115,    47,    -1,    -1,   119,    -1,    -1,   690,
      -1,    -1,   693,   694,    -1,    -1,    -1,   698,    62,    -1,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,   174,    -1,   176,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   753,    -1,    -1,    -1,   757,    -1,    -1,   192,
      -1,    -1,    -1,   764,    -1,    32,    -1,    -1,   201,   133,
     771,    -1,    20,    21,   775,    -1,    -1,    -1,    -1,   780,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,   789,    -1,
      -1,    58,    59,    -1,    -1,   159,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   810,
     811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    -1,   837,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,   107,
     108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,   119,   120,   121,   122,    -1,   877,   878,   126,   880,
     128,   129,    -1,    -1,   885,    -1,   887,    -1,    -1,    -1,
     115,    -1,    -1,    -1,   119,    -1,    -1,    -1,   899,   900,
      -1,    -1,    -1,   170,   905,    -1,    -1,   174,    -1,   176,
     177,    -1,    -1,    -1,   915,    -1,   917,    -1,   919,    20,
      21,    -1,    -1,    -1,   925,   192,    -1,    -1,   929,   177,
     178,   179,   180,   181,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   170,   947,   948,    -1,   174,
      -1,   176,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     961,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,    -1,    -1,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,   138,   139,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1049,    -1,
    1051,    -1,    -1,  1054,    -1,    -1,    -1,    -1,  1059,    -1,
      -1,  1062,  1063,    -1,    -1,    -1,  1067,    -1,   169,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,  1084,    -1,    -1,  1087,  1088,  1089,  1090,
     191,   192,  1093,    -1,    -1,  1096,  1097,   198,  1099,    -1,
      -1,  1102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,
       9,    10,    -1,    12,  1155,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,  1165,    -1,    25,    26,    27,    28,
      -1,  1172,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    -1,    42,    43,  1186,  1187,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,  1197,    56,    57,    -1,
      -1,  1202,    -1,  1204,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    85,    86,    87,    -1,
      -1,    90,    91,    92,    93,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,    -1,   181,    -1,    -1,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,    -1,   196,   197,   198,
     199,   200,     1,    -1,    -1,    -1,    -1,     6,    -1,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    -1,    42,    43,    -1,    -1,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    85,    86,    87,    -1,
      -1,    90,    91,    92,    93,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,   147,    -1,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,    -1,   181,    -1,    -1,   184,   185,    -1,    -1,    -1,
      14,    15,    16,    17,    18,   194,    -1,   196,   197,   198,
     199,   200,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    51,    -1,    53,
      -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
     174,    -1,    -1,   177,   178,   179,    -1,   181,    -1,    -1,
     184,   185,    14,    15,    16,    17,    18,    -1,   192,    -1,
     194,    -1,   196,   197,    26,   199,    -1,   201,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    86,    87,    -1,    -1,    90,    91,
      92,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,
      -1,    -1,   174,    -1,    -1,   177,   178,   179,    -1,   181,
      -1,    -1,   184,   185,    14,    15,    16,    17,    18,    -1,
     192,    -1,   194,    -1,   196,   197,    26,   199,    -1,   201,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    85,    86,    87,    -1,    -1,
      90,    91,    92,    93,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,    -1,   177,   178,   179,
      -1,   181,    -1,    -1,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,   192,    -1,   194,    -1,   196,   197,    -1,   199,
      -1,   201,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    86,    87,    -1,    -1,    90,    91,
      92,    93,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   177,   178,   179,    -1,   181,
      -1,    -1,   184,   185,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   194,    46,   196,   197,    49,   199,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    85,    86,    87,    -1,    -1,    90,    91,    92,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,
      -1,   184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   194,    46,   196,   197,    49,   199,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
     124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,    -1,
     184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     194,    46,   196,   197,    49,   199,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,
     185,    14,    15,    16,    17,    18,    -1,   192,    -1,   194,
     195,   196,   197,    26,   199,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    85,    86,    87,    -1,    -1,    90,    91,    92,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,
      -1,   184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   194,    46,   196,   197,    49,   199,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,    -1,
     184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     194,    46,   196,   197,    49,   199,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,
     185,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   194,
      46,   196,   197,    49,   199,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      86,    87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,
      -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,   185,
      -1,    38,    -1,    -1,    -1,    -1,    43,   193,   194,    46,
     196,   197,    49,   199,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    86,
      87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     177,   178,   179,    -1,   181,    -1,    -1,   184,   185,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   194,    46,   196,
     197,    49,   199,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    86,    87,
      -1,    -1,    90,    91,    92,    93,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
     178,   179,    -1,   181,    -1,    -1,   184,   185,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   194,   195,   196,   197,
      26,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    59,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      86,    87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,   185,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   194,    46,
     196,   197,    49,   199,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    86,
      87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
     147,    -1,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     177,   178,   179,    -1,   181,    -1,    -1,   184,   185,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   194,    46,   196,
     197,    49,   199,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    86,    87,
      -1,    -1,    90,    91,    92,    93,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,
     178,   179,    -1,   181,    -1,    -1,   184,   185,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   194,    -1,   196,   197,
      26,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      86,    87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,   185,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   194,   195,
     196,   197,    26,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,   178,   179,    -1,   181,    -1,    -1,
     184,   185,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     194,    -1,   196,   197,    26,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    86,    87,    -1,    -1,    90,    91,
      92,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,   179,    -1,   181,
      -1,    -1,   184,   185,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   194,   195,   196,   197,    26,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    85,    86,    87,    -1,    -1,
      90,    91,    92,    93,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,   147,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,
      -1,   181,    -1,    -1,   184,   185,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   194,   195,   196,   197,    26,   199,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    86,    87,
      -1,    -1,    90,    91,    92,    93,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,   147,
      -1,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
     178,   179,    -1,   181,    -1,    -1,   184,   185,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   194,   195,   196,   197,
      26,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    85,
      86,    87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,   147,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,   185,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   194,   195,
     196,   197,    26,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,   179,    -1,   181,    -1,    -1,
     184,   185,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     194,   195,   196,   197,    26,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    85,    86,    87,    -1,    -1,    90,    91,
      92,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,   147,    -1,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   177,   178,   179,    -1,   181,
      -1,    -1,   184,   185,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   194,    46,   196,   197,    49,   199,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    85,    86,    87,    -1,    -1,    90,    91,    92,
      93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,   147,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,
      -1,   184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   194,    46,   196,   197,    49,   199,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    85,    86,    87,    -1,    -1,    90,    91,    92,    93,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   177,   178,   179,    -1,   181,    -1,    -1,
     184,   185,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     194,    46,   196,   197,    49,   199,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,    -1,
      -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,   147,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,    -1,   181,    -1,    -1,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
      -1,   196,   197,    -1,   199,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    20,
      21,    -1,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,   138,   139,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,   170,    -1,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,
      -1,    -1,    -1,    18,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      85,    86,    87,    -1,    -1,    90,    91,    92,    93,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,    -1,    -1,    -1,   198,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,   148,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    85,    86,    87,    -1,    -1,    90,
      91,    92,    93,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,   148,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,   159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
     195,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    -1,    85,    86,
      87,    -1,    -1,    90,    91,    92,    93,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,   195,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,   148,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,   195,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
     195,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,   195,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,   195,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
     195,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,   195,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,   195,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
     195,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,   195,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,   195,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
     195,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,   195,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,    -1,    -1,   195,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,    -1,    -1,   195,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,   195,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    20,    21,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,    20,
      21,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
     193,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    20,    21,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,   193,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    20,    21,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,   193,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    -1,    -1,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    20,    21,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,   193,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      20,    21,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,   138,   139,   140,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,   170,    -1,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    20,    21,   119,
     120,   121,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,   138,   139,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,    -1,    -1,   184,   185,    -1,    -1,    -1,    -1,
      -1,   191,   192,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,    -1,
      -1,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,   132,
      -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
      -1,    -1,    -1,   107,   108,   109,   110,   111,   191,   192,
     114,   115,   116,   117,    -1,   119,   120,   121,   122,    20,
      21,    -1,   126,    -1,   128,   129,    -1,    -1,   132,    -1,
     134,   135,   136,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
     111,    -1,    -1,   114,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,
      -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,    -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,
     191,   192,   114,   115,   116,   117,    -1,   119,   120,   121,
     122,    20,    21,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,   170,    -1,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,
     192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,    -1,    -1,   114,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,   126,    -1,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,   191,   192,   114,   115,   116,   117,    -1,   119,
     120,   121,   122,    20,    21,    -1,   126,    -1,   128,   129,
      -1,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,   111,    -1,    -1,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,   135,   136,
     107,   108,   109,   110,   111,    20,    21,   114,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    -1,    -1,   126,
      -1,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    -1,    -1,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,   126,    -1,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,   111,    -1,    -1,   114,
     115,   116,   117,    -1,   119,   120,   121,   122,    20,    21,
      -1,   126,    -1,   128,   129,    -1,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    -1,    -1,    -1,
      -1,   107,   108,   109,   110,   111,   191,   192,   114,   115,
     116,   117,    -1,   119,   120,   121,   122,    20,    21,    -1,
     126,    -1,   128,   129,    -1,   107,   108,   109,   110,   111,
      -1,    -1,   114,   115,   116,   117,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,   174,   175,
     176,   177,   178,   179,   180,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
      -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,   191,
     192,   114,    -1,    -1,   117,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,   176,   177,   178,   179,   180,   181,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,   159,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,   159,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,   159,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    65,    -1,    67,
      -1,    69,    70,    71,    72,    73,   159,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    85,    86,    87,
      -1,    -1,    90,    91,    92,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   203,     0,     6,    29,    31,    33,    39,    48,    54,
      74,    95,   192,   198,   206,   214,   215,   220,   248,   252,
     278,   346,   352,   355,   361,   400,   403,    18,    19,   159,
     241,   242,   243,   153,   221,   222,   159,   160,   181,   216,
     217,   218,   159,   199,   349,   159,   196,   205,   404,   401,
      33,    61,   133,   159,   185,   194,   244,   245,   246,   247,
     265,     4,     5,     7,    35,   359,    60,   344,   169,   168,
     171,   168,   217,    21,    55,   180,   191,   219,   350,   349,
     351,   344,   159,   159,   159,   133,   246,   246,   194,   134,
     135,   136,   168,   193,    55,    61,   253,   255,    55,    61,
     353,    55,    61,   360,    55,    61,   345,    14,    15,   153,
     157,   159,   161,   194,   208,   242,   153,   222,   159,   216,
     216,   169,   198,   200,   349,    55,    61,   204,   199,   402,
     159,   195,   243,   246,   246,   246,   246,   256,   159,   354,
     362,   199,   347,   162,   163,   207,    14,    15,   153,   157,
     159,   208,   239,   240,   219,    24,    30,    47,    62,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
     146,   265,   365,   367,   368,   370,   374,   399,   198,   159,
     369,   199,   195,    34,    65,    67,    69,    70,    71,    72,
      73,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      85,    86,    87,    90,    91,    92,    93,   109,   110,   159,
     251,   254,   171,   199,    98,   357,   358,   342,   150,   321,
     162,   163,   164,   168,   195,   175,   175,   175,   175,   194,
     175,   175,   175,   175,   175,   175,   194,    32,    58,    59,
     115,   119,   170,   174,   177,   192,   201,   172,   198,   159,
     334,   335,    20,    21,    37,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   119,   120,   121,   127,   128,   129,   130,
     131,   134,   135,   136,   137,   138,   139,   140,   170,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   184,
     185,   191,   192,    34,    34,   194,   249,   199,   257,    69,
      73,    90,    91,    92,    93,   366,   348,   159,   363,   200,
     343,   243,   142,   159,   340,   341,   239,   377,   379,   381,
     375,   159,   371,   383,   385,   387,   389,   391,   393,   395,
     397,    14,    15,    16,    17,    18,    26,    38,    43,    46,
      49,    53,    63,    74,    96,   109,   110,   141,   142,   143,
     144,   145,   147,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   160,   161,   177,   178,   179,   181,   184,
     185,   194,   196,   197,   210,   212,   257,   261,   265,   270,
     281,   288,   291,   294,   298,   299,   302,   303,   305,   310,
     313,   320,   365,   405,   409,   413,   415,   417,    32,   174,
      32,   174,   192,   201,   193,   313,   201,   374,   159,   200,
     171,   198,   159,   159,   193,    21,   193,   145,   195,   321,
     330,   331,   171,   250,   259,   199,   159,   200,   171,   356,
     199,   321,   193,   194,    41,   168,   171,   174,   339,   399,
     373,   399,   399,   399,   195,   369,   399,   249,   399,   249,
     399,   249,   159,   332,   333,   399,   335,   175,   267,   367,
     405,   194,   175,   194,   175,   175,   194,   175,   194,   175,
     313,   313,   194,   194,   194,   194,   194,   194,    12,   374,
      12,   374,   313,   411,   414,   175,   209,   313,   313,   313,
     265,   313,   313,   313,   197,   159,   194,   248,    20,    21,
     107,   108,   109,   110,   111,   114,   115,   116,   117,   119,
     120,   121,   122,   126,   128,   129,   134,   135,   136,   140,
     170,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   191,   192,   195,   194,   193,   193,   399,   200,   334,
     159,   194,    48,   142,   159,   337,   364,   195,   198,   399,
       1,     8,     9,    10,    12,    25,    27,    28,    37,    40,
      42,    50,    52,    56,    57,    63,    97,   124,   125,   160,
     198,   200,   223,   224,   227,   230,   231,   233,   234,   235,
     236,   237,   258,   260,   264,   266,   271,   272,   273,   274,
     275,   276,   277,   278,   280,   300,   301,   313,   348,   169,
     198,   265,   326,   341,   193,   313,   159,   159,   374,   118,
     127,   169,   338,   176,   176,   198,   176,   176,   176,   198,
     176,   250,   176,   250,   176,   250,   171,   176,   198,   176,
     198,   268,   194,   265,   289,   313,   282,   284,   313,   286,
     313,   374,   313,   313,   313,   313,   313,   313,   364,    51,
     159,   177,   194,   313,   406,   407,   408,   410,   412,   364,
     194,   407,   412,   132,   198,   200,   262,   164,   165,   207,
     213,   195,   142,   147,   175,   265,   304,   192,   195,   292,
     313,   150,   297,    18,   148,   159,   365,    18,   148,   159,
     365,   313,   313,   313,   313,   313,   313,   159,   313,   148,
     159,   313,   313,   313,   313,   313,   313,   313,   313,   313,
      21,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   120,   121,   148,   159,   191,   192,   311,   313,
     195,   292,   313,   337,   194,    41,   118,   168,   169,   171,
     174,   336,   330,   313,   364,   118,   264,   300,   313,   257,
      59,   313,   313,   159,   198,   153,    56,   313,   257,   118,
     264,   313,   197,   298,   298,   313,    36,   198,   198,   313,
       9,   198,   198,   198,   198,   198,    64,   279,   123,   198,
      99,   100,   101,   102,   103,   104,   105,   106,   112,   113,
     118,   127,   130,   131,   137,   138,   139,   169,   200,   313,
     192,   200,   248,   327,   195,    41,   198,   338,   264,   313,
     378,   380,   399,   382,   376,   372,   384,   176,   388,   176,
     392,   176,   399,   396,   332,   398,   399,   195,   292,   175,
     313,   399,   195,   374,   374,   195,   374,   195,   176,   195,
     195,   195,   195,   195,   195,    19,   298,   127,   336,   195,
     132,   168,   198,   408,   193,   168,   193,   198,    19,   195,
     408,   200,   313,   411,   200,   374,   313,   162,   211,   194,
     194,   306,   308,   159,   406,   168,   195,   118,   127,   169,
     174,   295,   296,   249,   175,   194,   175,   194,   194,   194,
     193,    18,   148,   159,   365,   171,   148,   159,   313,   194,
     148,   159,   313,     1,   193,   195,   195,   313,   159,   159,
     399,   264,   313,   257,    19,   264,   313,   118,   169,    13,
     313,   257,   169,   171,   153,   264,   313,   197,   257,   199,
     257,   232,   341,   298,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   124,   125,   313,   313,   313,   313,
     313,   313,   313,   124,   125,   313,   198,   247,     7,   321,
     325,   159,   264,   313,   198,   386,   390,   394,   176,   195,
     159,   195,   176,   176,   176,   176,   297,   194,   292,   313,
     313,   407,   408,   313,   159,   406,   193,   193,   313,   193,
     410,   292,   407,   200,   193,   176,   166,   162,   313,   313,
     374,   249,   193,   313,   159,   159,   159,   159,   168,   193,
     250,   314,   313,   316,   313,   195,   292,   313,   175,   194,
     313,   194,   193,   313,   194,   193,   312,   195,    41,   336,
     292,   257,   238,   259,    11,    22,    23,   225,   226,   313,
     298,   298,   298,   298,   193,    55,    61,   324,    44,    98,
     322,   198,   198,   269,   176,   198,   290,   283,   285,   287,
     194,   195,   292,   198,   408,   193,   127,   336,   193,   198,
     408,   193,   263,   195,   195,   176,   250,   195,   295,   193,
     132,   257,   293,   374,   195,   399,   195,   195,   195,   318,
     313,   313,   195,   313,   159,   313,   198,   313,   200,   257,
     313,    11,   228,   198,    45,   322,   324,   313,   159,   313,
     313,   313,   195,   313,   195,   313,   193,   193,   313,   313,
     313,   200,   313,   307,   176,   118,   313,   176,   176,   399,
     195,   195,   195,   257,   257,   229,   198,   328,    32,   323,
     337,   195,   176,   195,   198,   416,   193,   416,   193,   304,
     309,   313,   315,   317,   176,   226,    25,    97,   230,   271,
     272,   273,   275,   313,   254,   329,   313,    51,   193,   200,
     304,   319,   118,   313,   118,   313,   198,   254,   195,   313,
     193,   193,   313,   313,   257
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   202,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   204,   204,   204,   205,   205,
     206,   207,   207,   207,   207,   208,   209,   209,   209,   210,
     211,   211,   213,   212,   214,   215,   216,   216,   217,   217,
     217,   217,   218,   218,   219,   219,   220,   221,   221,   222,
     222,   223,   224,   224,   225,   225,   226,   226,   226,   227,
     227,   228,   229,   228,   230,   230,   230,   230,   230,   231,
     232,   231,   233,   234,   235,   236,   238,   237,   239,   239,
     239,   239,   239,   239,   240,   240,   241,   241,   241,   242,
     242,   242,   242,   242,   242,   242,   242,   243,   243,   244,
     244,   244,   245,   245,   246,   246,   246,   246,   246,   246,
     247,   247,   248,   248,   249,   249,   249,   250,   250,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   252,
     253,   253,   253,   254,   256,   255,   257,   257,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   259,
     259,   259,   260,   262,   263,   261,   264,   264,   264,   265,
     265,   265,   266,   266,   268,   269,   267,   267,   270,   270,
     270,   270,   271,   272,   273,   273,   273,   274,   274,   274,
     275,   275,   276,   276,   276,   277,   278,   278,   279,   279,
     280,   282,   283,   281,   284,   285,   281,   286,   287,   281,
     289,   290,   288,   291,   291,   291,   292,   292,   293,   293,
     293,   294,   294,   294,   295,   295,   295,   295,   296,   296,
     297,   297,   298,   298,   299,   299,   299,   299,   299,   299,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   301,
     301,   301,   301,   302,   303,   303,   304,   304,   305,   306,
     307,   305,   308,   309,   305,   310,   310,   311,   312,   310,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   314,
     315,   313,   313,   313,   313,   316,   317,   313,   313,   313,
     318,   319,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   321,   321,   322,   322,   322,   323,   323,   324,
     324,   324,   325,   326,   327,   326,   328,   326,   329,   326,
     326,   330,   330,   330,   331,   331,   332,   332,   333,   333,
     334,   335,   335,   336,   336,   337,   337,   337,   337,   337,
     337,   338,   338,   338,   339,   339,   340,   340,   340,   340,
     340,   341,   341,   341,   341,   341,   342,   343,   342,   344,
     344,   345,   345,   345,   346,   347,   346,   348,   348,   348,
     350,   349,   351,   351,   352,   352,   353,   353,   353,   354,
     355,   355,   356,   356,   357,   357,   358,   359,   359,   360,
     360,   360,   362,   363,   361,   364,   364,   364,   364,   364,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   365,   365,   365,   365,   366,   366,   366,   366,   366,
     366,   367,   368,   368,   368,   369,   369,   371,   372,   370,
     373,   373,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     375,   376,   374,   374,   377,   378,   374,   379,   380,   374,
     381,   382,   374,   374,   383,   384,   374,   385,   386,   374,
     374,   387,   388,   374,   389,   390,   374,   374,   391,   392,
     374,   393,   394,   374,   395,   396,   374,   397,   398,   374,
     399,   399,   399,   401,   402,   400,   404,   403,   405,   405,
     405,   405,   406,   406,   406,   406,   407,   407,   408,   408,
     409,   409,   409,   409,   409,   409,   410,   410,   410,   411,
     411,   412,   412,   413,   413,   414,   414,   415,   416,   416,
     417,   417
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
       2,     2,     3,     0,     0,     7,     3,     2,     2,     1,
       3,     2,     2,     3,     0,     0,     5,     1,     2,     4,
       5,     2,     1,     1,     1,     2,     3,     2,     2,     3,
       2,     3,     2,     2,     3,     4,     1,     1,     1,     0,
       3,     0,     0,     7,     0,     0,     7,     0,     0,     7,
       0,     0,     6,     5,     8,    10,     1,     3,     1,     2,
       3,     1,     1,     2,     2,     2,     2,     2,     1,     3,
       0,     4,     1,     6,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       4,     4,     4,     6,     5,     6,     1,     4,     3,     0,
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
       2,     3,     2,     3,     2,     2,     3,     3,     3,     2,
       0,     0,     6,     2,     0,     0,     6,     0,     0,     6,
       0,     0,     6,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     0,     0,     6,     0,     0,     6,
       1,     3,     3,     0,     0,     8,     0,     7,     1,     1,
       1,     1,     3,     3,     5,     5,     1,     3,     0,     2,
       6,     5,     7,     8,     6,     8,     1,     3,     3,     3,
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

  case 233: /* $@6: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 234: /* $@7: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 235: /* expression_keyword: "keyword" '<' $@6 type_declaration_no_options '>' $@7 expr  */
                                                                                                                                               {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),*(yyvsp[-6].s));
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCall->arguments.push_back(ExpressionPtr(td));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 236: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 237: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 238: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 239: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 240: /* name_in_namespace: "name" "::" "name"  */
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

  case 241: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 242: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 243: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 244: /* $@8: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 245: /* $@9: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 246: /* new_type_declaration: '<' $@8 type_declaration '>' $@9  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 247: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 248: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 249: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 250: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 251: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 252: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 253: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 254: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 255: /* expression_return_no_pipe: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 256: /* expression_return_no_pipe: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 257: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 258: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 259: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 260: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 261: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 262: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 263: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 264: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 265: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 266: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 267: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 268: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 269: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 270: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 271: /* $@10: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 272: /* $@11: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 273: /* expr_cast: "cast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 274: /* $@12: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 275: /* $@13: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 276: /* expr_cast: "upcast" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 277: /* $@14: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 278: /* $@15: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 279: /* expr_cast: "reinterpret" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 280: /* $@16: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 281: /* $@17: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 282: /* expr_type_decl: "type" '<' $@16 type_declaration '>' $@17  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 283: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 284: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 285: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 286: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 287: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 288: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 289: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 290: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 291: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 292: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 293: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 294: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 295: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 296: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 297: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 298: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 299: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 300: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 301: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 302: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 303: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 304: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 305: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 306: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 307: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 308: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 309: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 310: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 311: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 312: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 313: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 314: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 315: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 316: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 317: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 318: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 319: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 320: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 325: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 326: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 334: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 335: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 336: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 337: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 338: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 339: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 340: /* $@19: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 341: /* func_addr_expr: '@' '@' '<' $@18 type_declaration_no_options '>' $@19 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 342: /* $@20: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 343: /* $@21: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 344: /* func_addr_expr: '@' '@' '<' $@20 optional_function_argument_list optional_function_type '>' $@21 func_addr_name  */
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

  case 345: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 346: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 347: /* $@22: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 348: /* $@23: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 349: /* expr_field: expr '.' $@22 error $@23  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 350: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 351: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 352: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 353: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 354: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 355: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 356: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 357: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 358: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 359: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 360: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 386: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 389: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 390: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 391: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 392: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 393: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 394: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 395: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 396: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 397: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 398: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 399: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 400: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 401: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 402: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 404: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 405: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 406: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 407: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 408: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 409: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 410: /* $@25: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 411: /* expr: expr "is" "type" '<' $@24 type_declaration_no_options '>' $@25  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 412: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 413: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 414: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 415: /* $@26: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 416: /* $@27: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 417: /* expr: expr "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 418: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 419: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 420: /* $@28: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 421: /* $@29: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 422: /* expr: expr '?' "as" "type" '<' $@28 type_declaration '>' $@29  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 423: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 424: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 425: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 426: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 427: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 428: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 429: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 430: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 431: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 432: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 433: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 434: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 435: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 436: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 437: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 438: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 439: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 440: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 441: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 442: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 443: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 444: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 445: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 446: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 447: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 448: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 449: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 450: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 451: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 452: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 453: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 454: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 455: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 456: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 457: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 458: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 459: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 460: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 461: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 462: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 463: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 464: /* $@30: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 465: /* struct_variable_declaration_list: struct_variable_declaration_list $@30 structure_variable_declaration ';'  */
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

  case 466: /* $@31: %empty  */
                                                                                             {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 467: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@31 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-7].pVarDeclList),(yyvsp[-6].faList),(yyvsp[-4].b),(yyvsp[-1].pFuncDecl));
            }
    break;

  case 468: /* $@32: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 469: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@32 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 470: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 471: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 472: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 473: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 474: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 475: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 476: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 477: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 478: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 479: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 480: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 481: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 482: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 483: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 484: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 485: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 486: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 487: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 488: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 489: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 490: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 491: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 492: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 493: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 494: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 495: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 496: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 497: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 498: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 499: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 500: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 501: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 502: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 503: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 504: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 505: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 506: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 507: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 508: /* global_variable_declaration_list: global_variable_declaration_list $@33 optional_field_annotation let_variable_declaration  */
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

  case 509: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 510: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 511: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 512: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 513: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 514: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 515: /* $@34: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 516: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@34 optional_field_annotation let_variable_declaration  */
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

  case 517: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 518: /* enum_list: enum_list "name" ';'  */
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

  case 519: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 520: /* $@35: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 521: /* single_alias: "name" $@35 '=' type_declaration  */
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

  case 526: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 527: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 528: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 529: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 530: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 531: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 532: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 533: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 534: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 535: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 536: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 537: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 538: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 539: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 540: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 541: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 542: /* $@36: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 543: /* $@37: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 544: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@36 structure_name $@37 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
        }
    }
    break;

  case 545: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 546: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 547: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 548: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 549: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 550: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 551: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 552: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 553: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 554: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 555: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 556: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 557: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 558: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 559: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 560: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 561: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 562: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 563: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 564: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 565: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 566: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 567: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 568: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 569: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 570: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 571: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 572: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 573: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 574: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 575: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 576: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 577: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 578: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 579: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 580: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 581: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 582: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 583: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 584: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 585: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 586: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 587: /* $@38: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 588: /* $@39: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 589: /* bitfield_type_declaration: "bitfield" '<' $@38 bitfield_bits '>' $@39  */
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

  case 590: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 591: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 592: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 593: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 594: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 595: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 596: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
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

  case 597: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 598: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 599: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 600: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 601: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 602: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 603: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 604: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 605: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 606: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 607: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 608: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 609: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 610: /* $@40: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 611: /* $@41: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 612: /* type_declaration_no_options: "smart_ptr" '<' $@40 type_declaration '>' $@41  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 613: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 614: /* $@42: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 615: /* $@43: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 616: /* type_declaration_no_options: "array" '<' $@42 type_declaration '>' $@43  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 617: /* $@44: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 618: /* $@45: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 619: /* type_declaration_no_options: "table" '<' $@44 table_type_pair '>' $@45  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 620: /* $@46: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 621: /* $@47: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 622: /* type_declaration_no_options: "iterator" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 623: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 624: /* $@48: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 625: /* $@49: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 626: /* type_declaration_no_options: "block" '<' $@48 type_declaration '>' $@49  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 627: /* $@50: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 628: /* $@51: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 629: /* type_declaration_no_options: "block" '<' $@50 optional_function_argument_list optional_function_type '>' $@51  */
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

  case 630: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 631: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 632: /* $@53: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 633: /* type_declaration_no_options: "function" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 634: /* $@54: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 635: /* $@55: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 636: /* type_declaration_no_options: "function" '<' $@54 optional_function_argument_list optional_function_type '>' $@55  */
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

  case 637: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 638: /* $@56: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 639: /* $@57: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 640: /* type_declaration_no_options: "lambda" '<' $@56 type_declaration '>' $@57  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 641: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 642: /* $@59: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 643: /* type_declaration_no_options: "lambda" '<' $@58 optional_function_argument_list optional_function_type '>' $@59  */
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

  case 644: /* $@60: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 645: /* $@61: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 646: /* type_declaration_no_options: "tuple" '<' $@60 tuple_type_list '>' $@61  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 647: /* $@62: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 648: /* $@63: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 649: /* type_declaration_no_options: "variant" '<' $@62 variant_type_list '>' $@63  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 650: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 651: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 652: /* type_declaration: type_declaration '|' '#'  */
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

  case 653: /* $@64: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 654: /* $@65: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 655: /* variant_alias_declaration: "variant" $@64 "name" $@65 '{' variant_type_list ';' '}'  */
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

  case 656: /* $@66: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 657: /* bitfield_alias_declaration: "bitfield" $@66 "name" '{' bitfield_bits ';' '}'  */
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

  case 658: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 659: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 660: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 661: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 662: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 663: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 664: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 665: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 666: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 667: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 668: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 669: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 670: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block ']' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 671: /* make_struct_decl: "[[" type_declaration_no_options optional_block ']' ']'  */
                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 672: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block ']' ']'  */
                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 673: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 674: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block '}' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 675: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block '}' ']'  */
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

  case 676: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 677: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 678: /* make_tuple: make_tuple ',' expr  */
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

  case 679: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 680: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 681: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 682: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 683: /* make_dim_decl: "[[" type_declaration_no_options make_dim ']' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 684: /* make_dim_decl: "[{" type_declaration_no_options make_dim '}' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 685: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 686: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 687: /* make_table_decl: "{{" make_table '}' '}'  */
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

  case 688: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 689: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 690: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 691: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


