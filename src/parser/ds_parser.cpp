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
    void das_accept_sequence ( yyscan_t yyscanner, const char * seq, size_t seqLen, int lineNo );

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
  YYSYMBOL_DAS_STATIC = 65,                /* "static"  */
  YYSYMBOL_DAS_TBOOL = 66,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 67,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 68,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 69,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 70,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 71,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 72,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 73,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 74,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 75,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 76,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 77,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 78,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 79,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 80,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 81,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 82,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 83,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 84,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 85,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 86,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 87,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 88,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 89,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 90,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 91,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 92,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 93,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 94,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 95,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 96,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 97,                /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 98,              /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 99,             /* "generator"  */
  YYSYMBOL_DAS_YIELD = 100,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 101,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 102,                   /* "+="  */
  YYSYMBOL_SUBEQU = 103,                   /* "-="  */
  YYSYMBOL_DIVEQU = 104,                   /* "/="  */
  YYSYMBOL_MULEQU = 105,                   /* "*="  */
  YYSYMBOL_MODEQU = 106,                   /* "%="  */
  YYSYMBOL_ANDEQU = 107,                   /* "&="  */
  YYSYMBOL_OREQU = 108,                    /* "|="  */
  YYSYMBOL_XOREQU = 109,                   /* "^="  */
  YYSYMBOL_SHL = 110,                      /* "<<"  */
  YYSYMBOL_SHR = 111,                      /* ">>"  */
  YYSYMBOL_ADDADD = 112,                   /* "++"  */
  YYSYMBOL_SUBSUB = 113,                   /* "--"  */
  YYSYMBOL_LEEQU = 114,                    /* "<="  */
  YYSYMBOL_SHLEQU = 115,                   /* "<<="  */
  YYSYMBOL_SHREQU = 116,                   /* ">>="  */
  YYSYMBOL_GREQU = 117,                    /* ">="  */
  YYSYMBOL_EQUEQU = 118,                   /* "=="  */
  YYSYMBOL_NOTEQU = 119,                   /* "!="  */
  YYSYMBOL_RARROW = 120,                   /* "->"  */
  YYSYMBOL_LARROW = 121,                   /* "<-"  */
  YYSYMBOL_QQ = 122,                       /* "??"  */
  YYSYMBOL_QDOT = 123,                     /* "?."  */
  YYSYMBOL_QBRA = 124,                     /* "?["  */
  YYSYMBOL_LPIPE = 125,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 126,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 127,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 128,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 129,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 130,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 131,                 /* ":="  */
  YYSYMBOL_ROTL = 132,                     /* "<<<"  */
  YYSYMBOL_ROTR = 133,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 134,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 135,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 136,                    /* "=>"  */
  YYSYMBOL_COLCOL = 137,                   /* "::"  */
  YYSYMBOL_ANDAND = 138,                   /* "&&"  */
  YYSYMBOL_OROR = 139,                     /* "||"  */
  YYSYMBOL_XORXOR = 140,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 141,                /* "&&="  */
  YYSYMBOL_OROREQU = 142,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 143,                /* "^^="  */
  YYSYMBOL_DOTDOT = 144,                   /* ".."  */
  YYSYMBOL_MTAG_E = 145,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 146,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 147,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 148,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 149,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 150,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 151,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 152,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 153,           /* "..."  */
  YYSYMBOL_BRABRAB = 154,                  /* "[["  */
  YYSYMBOL_BRACBRB = 155,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 156,                  /* "{{"  */
  YYSYMBOL_INTEGER = 157,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 158,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 159,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 160,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 161,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 162,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 163,                   /* "double constant"  */
  YYSYMBOL_NAME = 164,                     /* "name"  */
  YYSYMBOL_KEYWORD = 165,                  /* "keyword"  */
  YYSYMBOL_BEGIN_STRING = 166,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 167,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 168,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 169,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 170,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 171,          /* "}"  */
  YYSYMBOL_END_OF_READ = 172,              /* "end of failed eader macro"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 173,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 174,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 175,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 176,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 177,            /* ",}]"  */
  YYSYMBOL_178_ = 178,                     /* ','  */
  YYSYMBOL_179_ = 179,                     /* '='  */
  YYSYMBOL_180_ = 180,                     /* '?'  */
  YYSYMBOL_181_ = 181,                     /* ':'  */
  YYSYMBOL_182_ = 182,                     /* '|'  */
  YYSYMBOL_183_ = 183,                     /* '^'  */
  YYSYMBOL_184_ = 184,                     /* '&'  */
  YYSYMBOL_185_ = 185,                     /* '<'  */
  YYSYMBOL_186_ = 186,                     /* '>'  */
  YYSYMBOL_187_ = 187,                     /* '-'  */
  YYSYMBOL_188_ = 188,                     /* '+'  */
  YYSYMBOL_189_ = 189,                     /* '*'  */
  YYSYMBOL_190_ = 190,                     /* '/'  */
  YYSYMBOL_191_ = 191,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 192,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 193,               /* UNARY_PLUS  */
  YYSYMBOL_194_ = 194,                     /* '~'  */
  YYSYMBOL_195_ = 195,                     /* '!'  */
  YYSYMBOL_PRE_INC = 196,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 197,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 198,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 199,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 200,                    /* DEREF  */
  YYSYMBOL_201_ = 201,                     /* '.'  */
  YYSYMBOL_202_ = 202,                     /* '['  */
  YYSYMBOL_203_ = 203,                     /* ']'  */
  YYSYMBOL_204_ = 204,                     /* '('  */
  YYSYMBOL_205_ = 205,                     /* ')'  */
  YYSYMBOL_206_ = 206,                     /* '$'  */
  YYSYMBOL_207_ = 207,                     /* '@'  */
  YYSYMBOL_208_ = 208,                     /* ';'  */
  YYSYMBOL_209_ = 209,                     /* '{'  */
  YYSYMBOL_210_ = 210,                     /* '}'  */
  YYSYMBOL_211_ = 211,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 212,                 /* $accept  */
  YYSYMBOL_program = 213,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 214, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 215,              /* module_name  */
  YYSYMBOL_module_declaration = 216,       /* module_declaration  */
  YYSYMBOL_character_sequence = 217,       /* character_sequence  */
  YYSYMBOL_string_constant = 218,          /* string_constant  */
  YYSYMBOL_string_builder_body = 219,      /* string_builder_body  */
  YYSYMBOL_string_builder = 220,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 221, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 222,              /* expr_reader  */
  YYSYMBOL_223_1 = 223,                    /* $@1  */
  YYSYMBOL_options_declaration = 224,      /* options_declaration  */
  YYSYMBOL_require_declaration = 225,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 226,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 227,      /* require_module_name  */
  YYSYMBOL_require_module = 228,           /* require_module  */
  YYSYMBOL_is_public_module = 229,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 230,       /* expect_declaration  */
  YYSYMBOL_expect_list = 231,              /* expect_list  */
  YYSYMBOL_expect_error = 232,             /* expect_error  */
  YYSYMBOL_expression_label = 233,         /* expression_label  */
  YYSYMBOL_expression_goto = 234,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 235,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 236,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 237,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 238, /* expression_else_one_liner  */
  YYSYMBOL_239_2 = 239,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 240,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 241,  /* expression_if_then_else  */
  YYSYMBOL_242_3 = 242,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 243,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 244,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 245,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 246,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 247,    /* expression_with_alias  */
  YYSYMBOL_248_4 = 248,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 249, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 250, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 251, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 252,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 253, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 254, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 255, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 256,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 257,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 258, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 259, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 260,   /* optional_function_type  */
  YYSYMBOL_function_name = 261,            /* function_name  */
  YYSYMBOL_global_function_declaration = 262, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 263, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 264, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 265,     /* function_declaration  */
  YYSYMBOL_266_5 = 266,                    /* $@5  */
  YYSYMBOL_expression_block = 267,         /* expression_block  */
  YYSYMBOL_expression_any = 268,           /* expression_any  */
  YYSYMBOL_expressions = 269,              /* expressions  */
  YYSYMBOL_expr_keyword = 270,             /* expr_keyword  */
  YYSYMBOL_expression_keyword = 271,       /* expression_keyword  */
  YYSYMBOL_272_6 = 272,                    /* $@6  */
  YYSYMBOL_273_7 = 273,                    /* $@7  */
  YYSYMBOL_expr_pipe = 274,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 275,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 276,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 277,     /* new_type_declaration  */
  YYSYMBOL_278_8 = 278,                    /* $@8  */
  YYSYMBOL_279_9 = 279,                    /* $@9  */
  YYSYMBOL_expr_new = 280,                 /* expr_new  */
  YYSYMBOL_expression_break = 281,         /* expression_break  */
  YYSYMBOL_expression_continue = 282,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 283, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 284,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 285, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 286,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 287,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 288,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 289,        /* optional_in_scope  */
  YYSYMBOL_expression_let = 290,           /* expression_let  */
  YYSYMBOL_expr_cast = 291,                /* expr_cast  */
  YYSYMBOL_292_10 = 292,                   /* $@10  */
  YYSYMBOL_293_11 = 293,                   /* $@11  */
  YYSYMBOL_294_12 = 294,                   /* $@12  */
  YYSYMBOL_295_13 = 295,                   /* $@13  */
  YYSYMBOL_296_14 = 296,                   /* $@14  */
  YYSYMBOL_297_15 = 297,                   /* $@15  */
  YYSYMBOL_expr_type_decl = 298,           /* expr_type_decl  */
  YYSYMBOL_299_16 = 299,                   /* $@16  */
  YYSYMBOL_300_17 = 300,                   /* $@17  */
  YYSYMBOL_expr_type_info = 301,           /* expr_type_info  */
  YYSYMBOL_expr_list = 302,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 303,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 304,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 305,            /* capture_entry  */
  YYSYMBOL_capture_list = 306,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 307,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 308,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 309,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 310,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 311,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 312,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 313,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 314,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 315,           /* func_addr_expr  */
  YYSYMBOL_316_18 = 316,                   /* $@18  */
  YYSYMBOL_317_19 = 317,                   /* $@19  */
  YYSYMBOL_318_20 = 318,                   /* $@20  */
  YYSYMBOL_319_21 = 319,                   /* $@21  */
  YYSYMBOL_expr_field = 320,               /* expr_field  */
  YYSYMBOL_321_22 = 321,                   /* $@22  */
  YYSYMBOL_322_23 = 322,                   /* $@23  */
  YYSYMBOL_expr = 323,                     /* expr  */
  YYSYMBOL_324_24 = 324,                   /* $@24  */
  YYSYMBOL_325_25 = 325,                   /* $@25  */
  YYSYMBOL_326_26 = 326,                   /* $@26  */
  YYSYMBOL_327_27 = 327,                   /* $@27  */
  YYSYMBOL_328_28 = 328,                   /* $@28  */
  YYSYMBOL_329_29 = 329,                   /* $@29  */
  YYSYMBOL_expr_mtag = 330,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 331, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 332,        /* optional_override  */
  YYSYMBOL_optional_constant = 333,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 334, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 335, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 336, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 337, /* struct_variable_declaration_list  */
  YYSYMBOL_338_30 = 338,                   /* $@30  */
  YYSYMBOL_339_31 = 339,                   /* $@31  */
  YYSYMBOL_340_32 = 340,                   /* $@32  */
  YYSYMBOL_function_argument_declaration = 341, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 342,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 343,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 344,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 345,             /* variant_type  */
  YYSYMBOL_variant_type_list = 346,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 347,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 348,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 349,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 350,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 351, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 352, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 353, /* global_variable_declaration_list  */
  YYSYMBOL_354_33 = 354,                   /* $@33  */
  YYSYMBOL_optional_shared = 355,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 356, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 357,               /* global_let  */
  YYSYMBOL_358_34 = 358,                   /* $@34  */
  YYSYMBOL_enum_list = 359,                /* enum_list  */
  YYSYMBOL_single_alias = 360,             /* single_alias  */
  YYSYMBOL_361_35 = 361,                   /* $@35  */
  YYSYMBOL_alias_list = 362,               /* alias_list  */
  YYSYMBOL_alias_declaration = 363,        /* alias_declaration  */
  YYSYMBOL_364_36 = 364,                   /* $@36  */
  YYSYMBOL_optional_public_or_private_enum = 365, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 366,                /* enum_name  */
  YYSYMBOL_enum_declaration = 367,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 368, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 369,          /* optional_sealed  */
  YYSYMBOL_structure_name = 370,           /* structure_name  */
  YYSYMBOL_class_or_struct = 371,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 372, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 373,    /* structure_declaration  */
  YYSYMBOL_374_37 = 374,                   /* $@37  */
  YYSYMBOL_375_38 = 375,                   /* $@38  */
  YYSYMBOL_variable_name_with_pos_list = 376, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 377,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 378, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 379, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 380,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 381,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 382, /* bitfield_type_declaration  */
  YYSYMBOL_383_39 = 383,                   /* $@39  */
  YYSYMBOL_384_40 = 384,                   /* $@40  */
  YYSYMBOL_table_type_pair = 385,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 386, /* type_declaration_no_options  */
  YYSYMBOL_387_41 = 387,                   /* $@41  */
  YYSYMBOL_388_42 = 388,                   /* $@42  */
  YYSYMBOL_389_43 = 389,                   /* $@43  */
  YYSYMBOL_390_44 = 390,                   /* $@44  */
  YYSYMBOL_391_45 = 391,                   /* $@45  */
  YYSYMBOL_392_46 = 392,                   /* $@46  */
  YYSYMBOL_393_47 = 393,                   /* $@47  */
  YYSYMBOL_394_48 = 394,                   /* $@48  */
  YYSYMBOL_395_49 = 395,                   /* $@49  */
  YYSYMBOL_396_50 = 396,                   /* $@50  */
  YYSYMBOL_397_51 = 397,                   /* $@51  */
  YYSYMBOL_398_52 = 398,                   /* $@52  */
  YYSYMBOL_399_53 = 399,                   /* $@53  */
  YYSYMBOL_400_54 = 400,                   /* $@54  */
  YYSYMBOL_401_55 = 401,                   /* $@55  */
  YYSYMBOL_402_56 = 402,                   /* $@56  */
  YYSYMBOL_403_57 = 403,                   /* $@57  */
  YYSYMBOL_404_58 = 404,                   /* $@58  */
  YYSYMBOL_405_59 = 405,                   /* $@59  */
  YYSYMBOL_406_60 = 406,                   /* $@60  */
  YYSYMBOL_407_61 = 407,                   /* $@61  */
  YYSYMBOL_408_62 = 408,                   /* $@62  */
  YYSYMBOL_409_63 = 409,                   /* $@63  */
  YYSYMBOL_410_64 = 410,                   /* $@64  */
  YYSYMBOL_type_declaration = 411,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 412, /* variant_alias_declaration  */
  YYSYMBOL_413_65 = 413,                   /* $@65  */
  YYSYMBOL_414_66 = 414,                   /* $@66  */
  YYSYMBOL_bitfield_alias_declaration = 415, /* bitfield_alias_declaration  */
  YYSYMBOL_416_67 = 416,                   /* $@67  */
  YYSYMBOL_make_decl = 417,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 418,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 419,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 420,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 421, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 422, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 423, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 424, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 425, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_make_struct_decl = 426,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 427,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 428,           /* make_map_tuple  */
  YYSYMBOL_make_dim = 429,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 430,            /* make_dim_decl  */
  YYSYMBOL_make_table = 431,               /* make_table  */
  YYSYMBOL_make_table_decl = 432,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 433, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 434       /* array_comprehension  */
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
#define YYLAST   11137

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  212
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  223
/* YYNRULES -- Number of rules.  */
#define YYNRULES  724
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1267

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   439


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
       2,     2,     2,   195,     2,   211,   206,   191,   184,     2,
     204,   205,   189,   188,   178,   187,   201,   190,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   181,   208,
     185,   179,   186,   180,   207,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   202,     2,   203,   183,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   209,   182,   210,   194,     2,     2,     2,
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
     175,   176,   177,   192,   193,   196,   197,   198,   199,   200
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   502,   502,   503,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   518,   522,   523,   524,   528,   529,
     533,   551,   552,   553,   554,   558,   562,   567,   576,   584,
     600,   605,   613,   613,   650,   671,   675,   676,   680,   683,
     687,   693,   702,   705,   711,   712,   716,   720,   721,   725,
     728,   734,   740,   743,   749,   750,   754,   755,   756,   765,
     766,   770,   771,   771,   777,   778,   779,   780,   781,   785,
     791,   791,   797,   803,   811,   821,   830,   830,   837,   838,
     839,   840,   841,   842,   846,   851,   859,   860,   861,   865,
     866,   867,   868,   869,   870,   871,   872,   878,   881,   887,
     888,   889,   893,   901,   914,   917,   925,   936,   947,   958,
     961,   968,   972,   979,   980,   984,   985,   986,   990,   993,
    1000,  1004,  1005,  1006,  1007,  1008,  1009,  1010,  1011,  1012,
    1013,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,
    1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,
    1033,  1034,  1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,
    1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,
    1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,
    1063,  1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,
    1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,
    1083,  1084,  1085,  1086,  1087,  1092,  1114,  1115,  1116,  1120,
    1126,  1126,  1143,  1147,  1158,  1159,  1160,  1161,  1162,  1163,
    1164,  1165,  1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,
    1174,  1175,  1176,  1177,  1178,  1182,  1187,  1193,  1199,  1210,
    1210,  1210,  1221,  1255,  1258,  1261,  1267,  1268,  1279,  1283,
    1286,  1294,  1294,  1294,  1297,  1303,  1306,  1309,  1313,  1319,
    1323,  1327,  1330,  1333,  1341,  1344,  1347,  1355,  1358,  1366,
    1369,  1372,  1380,  1386,  1387,  1391,  1392,  1396,  1402,  1402,
    1402,  1405,  1405,  1405,  1410,  1410,  1410,  1418,  1418,  1418,
    1424,  1434,  1445,  1460,  1463,  1469,  1470,  1477,  1488,  1489,
    1490,  1494,  1495,  1496,  1497,  1501,  1506,  1514,  1515,  1519,
    1524,  1531,  1532,  1533,  1534,  1535,  1536,  1537,  1541,  1542,
    1543,  1544,  1545,  1546,  1547,  1548,  1549,  1550,  1551,  1552,
    1553,  1554,  1555,  1556,  1557,  1558,  1559,  1563,  1564,  1565,
    1566,  1567,  1568,  1572,  1579,  1591,  1596,  1606,  1610,  1617,
    1620,  1620,  1620,  1625,  1625,  1625,  1638,  1642,  1646,  1646,
    1646,  1653,  1654,  1655,  1656,  1657,  1658,  1659,  1660,  1661,
    1662,  1663,  1664,  1665,  1666,  1667,  1668,  1669,  1670,  1671,
    1672,  1673,  1674,  1675,  1676,  1677,  1678,  1679,  1680,  1681,
    1682,  1683,  1684,  1685,  1686,  1687,  1688,  1694,  1695,  1696,
    1697,  1698,  1699,  1700,  1701,  1702,  1703,  1704,  1705,  1706,
    1710,  1714,  1717,  1720,  1721,  1722,  1723,  1726,  1729,  1730,
    1733,  1733,  1733,  1736,  1741,  1745,  1749,  1749,  1749,  1754,
    1757,  1761,  1761,  1761,  1766,  1769,  1770,  1771,  1772,  1773,
    1774,  1775,  1776,  1777,  1778,  1779,  1784,  1788,  1789,  1790,
    1791,  1792,  1793,  1794,  1798,  1802,  1806,  1810,  1814,  1818,
    1822,  1826,  1830,  1837,  1838,  1842,  1843,  1844,  1848,  1849,
    1853,  1854,  1855,  1859,  1860,  1864,  1875,  1878,  1878,  1897,
    1896,  1911,  1910,  1923,  1932,  1938,  1943,  1953,  1954,  1958,
    1961,  1970,  1971,  1975,  1984,  1985,  1990,  1991,  1995,  2001,
    2007,  2010,  2014,  2020,  2029,  2030,  2031,  2035,  2036,  2040,
    2047,  2052,  2061,  2067,  2078,  2081,  2086,  2091,  2099,  2110,
    2113,  2113,  2133,  2134,  2138,  2139,  2140,  2144,  2147,  2147,
    2166,  2169,  2172,  2181,  2194,  2194,  2215,  2216,  2220,  2221,
    2221,  2225,  2226,  2227,  2231,  2241,  2248,  2258,  2259,  2263,
    2264,  2268,  2274,  2275,  2279,  2280,  2281,  2285,  2290,  2285,
    2304,  2311,  2316,  2325,  2331,  2342,  2343,  2344,  2345,  2346,
    2347,  2348,  2349,  2350,  2351,  2352,  2353,  2354,  2355,  2356,
    2357,  2358,  2359,  2360,  2361,  2362,  2363,  2364,  2365,  2366,
    2367,  2368,  2372,  2373,  2374,  2375,  2376,  2377,  2381,  2392,
    2396,  2403,  2415,  2422,  2431,  2431,  2431,  2444,  2448,  2455,
    2456,  2457,  2458,  2459,  2473,  2479,  2483,  2487,  2492,  2497,
    2502,  2507,  2511,  2515,  2520,  2524,  2528,  2533,  2533,  2533,
    2539,  2546,  2546,  2546,  2551,  2551,  2551,  2557,  2557,  2557,
    2562,  2566,  2566,  2566,  2571,  2571,  2571,  2580,  2584,  2584,
    2584,  2589,  2589,  2589,  2598,  2602,  2602,  2602,  2607,  2607,
    2607,  2616,  2616,  2616,  2622,  2622,  2622,  2631,  2634,  2645,
    2661,  2661,  2661,  2685,  2685,  2705,  2706,  2707,  2708,  2712,
    2719,  2726,  2732,  2738,  2745,  2752,  2758,  2768,  2773,  2780,
    2781,  2786,  2787,  2791,  2792,  2796,  2797,  2801,  2802,  2803,
    2807,  2808,  2809,  2814,  2820,  2827,  2835,  2842,  2850,  2862,
    2865,  2871,  2885,  2891,  2897,  2902,  2909,  2914,  2924,  2929,
    2936,  2948,  2949,  2953,  2956
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
  "\"private\"", "\"smart_ptr\"", "\"unsafe\"", "\"inscope\"",
  "\"static\"", "\"bool\"", "\"void\"", "\"string\"", "\"auto\"",
  "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"", "\"uint\"",
  "\"bitfield\"", "\"uint2\"", "\"uint3\"", "\"uint4\"", "\"float\"",
  "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"", "\"urange\"",
  "\"range64\"", "\"urange64\"", "\"block\"", "\"int64\"", "\"uint64\"",
  "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"",
  "\"int16\"", "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"",
  "\"yield\"", "\"sealed\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"",
  "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"",
  "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"",
  "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"",
  "\"$ <|\"", "\"@ <|\"", "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"",
  "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"",
  "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"",
  "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"",
  "\"...\"", "\"[[\"", "\"[{\"", "\"{{\"", "\"integer constant\"",
  "\"long integer constant\"", "\"unsigned integer constant\"",
  "\"unsigned long integer constant\"", "\"unsigned int8 constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"keyword\"", "\"start of the string\"", "STRING_CHARACTER",
  "STRING_CHARACTER_ESC", "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "\";}}\"", "\";}]\"", "\";]]\"",
  "\",]]\"", "\",}]\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'",
  "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS",
  "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC",
  "DEREF", "'.'", "'['", "']'", "'('", "')'", "'$'", "'@'", "';'", "'{'",
  "'}'", "'#'", "$accept", "program", "optional_public_or_private_module",
  "module_name", "module_declaration", "character_sequence",
  "string_constant", "string_builder_body", "string_builder",
  "reader_character_sequence", "expr_reader", "$@1", "options_declaration",
  "require_declaration", "keyword_or_name", "require_module_name",
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
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@30", "$@31", "$@32",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@33", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@34", "enum_list",
  "single_alias", "$@35", "alias_list", "alias_declaration", "$@36",
  "optional_public_or_private_enum", "enum_name", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@37", "$@38", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@39", "$@40", "table_type_pair",
  "type_declaration_no_options", "$@41", "$@42", "$@43", "$@44", "$@45",
  "$@46", "$@47", "$@48", "$@49", "$@50", "$@51", "$@52", "$@53", "$@54",
  "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62", "$@63",
  "$@64", "type_declaration", "variant_alias_declaration", "$@65", "$@66",
  "bitfield_alias_declaration", "$@67", "make_decl", "make_struct_fields",
  "make_struct_dim", "optional_block",
  "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "make_struct_decl", "make_tuple",
  "make_map_tuple", "make_dim", "make_dim_decl", "make_table",
  "make_table_decl", "array_comprehension_where", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1139)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-659)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1139,    38, -1139, -1139,    40,    37,   165,   -30, -1139,   -42,
   -1139, -1139,    60, -1139, -1139, -1139, -1139, -1139,   333, -1139,
     168, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
      93, -1139,   155,   162,   225, -1139, -1139, -1139,   165, -1139,
      15, -1139,   274,   274, -1139, -1139,   168,   276,   328, -1139,
   -1139,    60,   346,   364,    60,    60,   362, -1139,   520,   -10,
   -1139, -1139, -1139,   286,   438,   442, -1139,   457,    10,    40,
     411,    37,   318,   412, -1139,   302,   302, -1139, -1139,   366,
    -102,   381,   481,   388, -1139, -1139, -1139,   430, -1139,   -49,
      40,    60,    60,    60,    60, -1139, -1139, -1139, -1139, -1139,
   -1139, -1139,   450, -1139, -1139, -1139, -1139, -1139,   407, -1139,
   -1139, -1139, -1139, -1139,   360,   292, -1139, -1139, -1139, -1139,
     564, -1139, -1139,   466, -1139, -1139,   445, -1139, -1139, -1139,
   -1139,   497,   467, -1139, -1139,   -16, -1139,    31,   528,   520,
   10973, -1139,     3,   573, -1139,   521, -1139, -1139,   501, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139,   130, -1139, 10785, -1139,
   -1139,   469,   515, -1139,  9545, -1139, -1139, -1139, -1139, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
     647,   648, -1139,   479,   475,   394, -1139, -1139,   522, -1139,
     477,    40,    65, -1139, -1139, -1139,   292, -1139,   500,   503,
     504,   505, -1139, -1139, -1139,   489, -1139, -1139, -1139, -1139,
   -1139,   509, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
   -1139, -1139, -1139,   510, -1139, -1139, -1139,   511,   512, -1139,
   -1139, -1139, -1139,   513,   514,   498, -1139, -1139, -1139, -1139,
   -1139,   337,   518,   -70,   523, -1139,   495,   541,   542, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,   543,
     507, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
   -1139, -1139, -1139,   687, -1139, -1139, -1139, -1139, -1139, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139,   545,   508, -1139, -1139,
     -81,   531, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
     516,  -109,   532,   525, -1139,   521,    44,   517,   677,    84,
   -1139, -1139, -1139, -1139, -1139, -1139,   555, -1139,   239,   261,
     305, -1139, -1139,  6644, -1139, -1139, -1139,    -9, -1139, -1139,
   -1139,    -3,  3726, -1139,   207, -1139, -1139, 10785,   -47, -1139,
   -1139, -1139, -1139,   556,   591, -1139,   519, -1139,    -7, -1139,
     380, 10785, -1139,  1623, -1139,  -105, -1139, -1139,   217, -1139,
   -1139,    65,   527,  6644,   560,   567, 10785, -1139,   -68, 10785,
   10785, 10785, 10785,   533,   497, 10785,   479, 10785,   479, 10785,
     479, 10884,   515, -1139, -1139,   315,   537,   547, -1139,   539,
     559,   569,   552,   572,   554, -1139,   574,  6644,  6644,   557,
     558,   561,   565,   566,   568, -1139, 10427, 10526,  6644, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139,   578, -1139,  6644,  6644,
    6644,   217,  6644,  6644,  6644, -1139,   570, -1139, -1139, -1139,
   -1139,   -34, -1139, -1139, -1139, -1139,   562, -1139, -1139, -1139,
   -1139, -1139, -1139,  1344, -1139,   575, -1139, -1139, -1139, -1139,
   -1139, -1139, -1139, -1139, -1139,   546, -1139, -1139,  9011, -1139,
     337,   518, -1139, -1139, -1139, -1139,  6644,   273,   583,   727,
   -1139,   -74, -1139,   -71,   518, -1139,  6644, -1139, -1139,   273,
    2564, -1139,   475,  3885,  6644,   607, -1139,   584,   616,  4044,
     201,  2723,   448,   448,   448,  4203, -1139,   742,   589,   590,
    6644,   790, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
   -1139,   592,   594,   598,   599, -1139,   600, -1139, -1139,   741,
   -1139,   -77, -1139,  1196,    53,  6644, -1139, -1139,    11, -1139,
   -1139,  7037, -1139,   771,    -2, -1139, -1139, -1139,  3067,   271,
     627,  -110,   359,   389, -1139,    58,   401,   531,   404,   531,
     414,   531,    14, -1139,   264,   518,   296, -1139,   610, -1139,
   -1139,   217, -1139,  6644, -1139, -1139,  6644, -1139,  6644, 10785,
     410,   410,  6644,  6644,  6644,  6644,  6644,  6644,   273,  2018,
     273,  2200,  9580, -1139,   -76, -1139,   465,   410,   410,   -36,
   -1139,   410,   410,  7143,   185, -1139,  3544,   662, 10560, 10655,
    6644,  6644, -1139, -1139,  6644,  6644,  6644,  6644,   653,  6644,
     368,  6644,  6644,  6644,  6644,  6644,  6644,  6644,  6644,  6644,
    4385,  6644,  6644,  6644,  6644,  6644,  6644,  6644,  6644,  6644,
    6644,   196,  6644, -1139,  4544, -1139, -1139,  7178, -1139,  6644,
     654, -1139,   655, -1139, 10785, -1139,  3067, -1139,   671,     8,
    3067, -1139,   696,  9437,   810,  6644,  9891,   671,   645, -1139,
     644,   670,  9891, -1139,  3067, -1139,  9437,   621, -1139, -1139,
   -1139,   671,   620, -1139, -1139,   671, -1139, -1139, -1139, -1139,
   -1139, -1139, -1139,    65,   448, -1139,  6644,  6644,  6644,  6644,
    6644,  6644,  6644,  6644,  6644,  6644,  3226,  6644,  6644,  6644,
    6644,  6644,  6644,  3385, -1139,  6896,    60, -1139,   826,   521,
   -1139,   672, -1139,  3067, -1139,  6752, -1139, -1139, 10785, -1139,
   -1139, -1139,   673, -1139,   649, -1139,   658, -1139,   663, 10785,
   -1139, 10884, -1139,   515, 10785,  4726,  4908, 10785,  7284, 10785,
   10785,  7319, 10785,  7425,   294,  7460,  7566,  7601,  7707,  7742,
    7848,    13,   448,   630,   -19,  2382,  5090,  9675,   661,   -20,
     224,   674,   159,    29,  5272,   -20,   153,  6644, -1139,  6644,
     638, -1139, 10785, -1139,  6644,   496,   683, -1139,   636,   659,
     325, -1139, -1139,   374, -1139,   187,  9891,   -25,   479,   684,
     666, -1139, -1139,   690,   678, -1139, -1139,   895,   895,  5464,
    5464,   754,   754,   679,    25,   681, -1139,  9106,   -59,   -59,
     895,   895, 10133,  1044, 10047,  9926, 10686,  9770, 10161, 10247,
   10275,  5464,  5464,   526,   526,    25,    25,    25,   385,  6644,
     686, -1139,   391,  6644,   880,  9201, -1139,   208, -1139,  7883,
   -1139,   850,   -21, -1139,  9437, -1139,  6644, -1139,  9437,  6644,
    6644,   475,  9891, -1139, -1139, -1139, -1139, -1139,  9437, -1139,
   -1139, -1139,   472,  6644, -1139, -1139,  9891,  9891,  9891,  9891,
    9891,  9891,  9891,  9891,  9891,  9891,   448,   448,   448,  9891,
    9891,  9891,  9891,  9891,  9891,  9891,   448,   448,   448,  9891,
   -1139,    45,   506,   828,   688, -1139, -1139,  6859, -1139, -1139,
   -1139,   518, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
   -1139,   518, -1139, -1139, -1139,   427, -1139,   212,   733,  7989,
     435, -1139,   549,   708, -1139,  1234, -1139,   662, -1139, -1139,
   -1139, -1139, -1139,   694,  6644, -1139,  6644,  6644,  6644,   -11,
    6644,   392,   374,   224, -1139, -1139,   697, -1139,  6644, -1139,
     698,  6644, -1139,  6644,   374,   181, -1139,   699, -1139,  9891,
   -1139, -1139,  1309,  9805, -1139,   737,  6644,  6644, 10785,   479,
      -6,   147,  5454, -1139,   744,   747,   749,   753, -1139,   229,
     531, -1139,  6644, -1139,  6644,  5621,  6644, -1139,   720,   716,
   -1139, -1139,  6644,   717, -1139,  9236,  6644,   718, -1139,  9331,
   -1139, -1139,  6644, -1139, -1139,   760,  6644,    41, -1139,  6644,
    1833,   475, -1139, -1139,  6644, -1139,   440, -1139, -1139, -1139,
   -1139, -1139, -1139,   719, -1139, -1139,   361, -1139,    22, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139,   299, -1139, -1139, -1139,
   -1139, -1139,   721,  5803,   138,  8024,  9891,  9891,   -20,   224,
    9891,   722,   136,   661, -1139, -1139,  9891, -1139,   674,   143,
     -20, -1139, -1139,   726, -1139, -1139, -1139, -1139, -1139,  8130,
    8165,  1376,   531,   725,   374,  9891, -1139, -1139, -1139, -1139,
     -25,   728,   -82, 10785,  8271, 10785,  8306, -1139,   216,  8412,
   -1139,  6644, 10012,  6644, -1139,  8447,  6644, -1139, -1139, -1139,
    9891,   475,  9891, -1139, -1139,   671, -1139,   729, -1139,   901,
      22, -1139, -1139,   506, -1139,  6644,   770, -1139,  6644,  6644,
    6644,  5985, -1139,   223,  6644,   193,   224, -1139,  6644,  6644,
    6644,  6644,   181, -1139,  6644, -1139, -1139, -1139,   750, -1139,
     245, -1139, -1139,  6167, -1139, -1139,  1381, -1139,   458, -1139,
   -1139, -1139, 10785,  8553,  8588, -1139,  8694, -1139,   472,  2908,
   -1139, -1139, -1139,   901,   273,  8729,   752,  5464,  5464,  5464,
   -1139,  8835, -1139,  7002,  6644,  6644, -1139,  8870,  9891,  9891,
    7002, -1139,  5464,   238, -1139,   730,  6644,  9926, -1139, -1139,
     461, -1139, -1139, -1139, -1139,  6326,  6485, -1139, -1139, -1139,
   -1139, -1139,  9891, 10973, -1139, -1139, -1139,  6644, -1139,   899,
     743,  9891,  9891,   197,   748, -1139,   238, -1139, 10012, -1139,
   -1139, -1139,  6644,  9891,  6644,  9891,   745, 10973,  8976,  6644,
     751,  6644,  6644,   756, -1139, -1139,  9891,  9891, -1139,   475,
   -1139,  9891, -1139,  9891,  9891, -1139, -1139
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   113,     1,   273,     0,     0,     0,   539,   274,     0,
     673,   670,     0,    14,     3,    10,     9,     8,     0,     7,
     522,     6,    11,     5,     4,    12,    13,    87,    88,    86,
      95,    97,    34,    49,    46,    47,    36,    37,     0,    38,
      44,    35,     0,     0,    19,    18,   522,     0,     0,   100,
     101,     0,     0,   246,     0,     0,   102,   104,   111,     0,
      99,   553,   552,   206,   541,   554,   523,   524,     0,     0,
       0,     0,    39,     0,    45,     0,     0,    42,   534,     0,
       0,     0,    15,     0,   671,   110,   248,     0,   105,     0,
       0,     0,     0,     0,     0,   114,   208,   207,   210,   205,
     543,   542,     0,   556,   555,   557,   526,   525,   528,    93,
      94,    91,    92,    90,     0,     0,    89,    98,    50,    48,
      44,    41,    40,     0,   536,   538,     0,   540,    16,    17,
      20,     0,     0,   247,   109,     0,   106,   107,   108,   112,
       0,   544,     0,   549,   519,   463,    21,    22,     0,    82,
      83,    80,    81,    79,    78,    84,     0,    43,     0,   537,
     602,     0,     0,   103,     0,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,     0,   120,   115,     0,     0,   530,   550,     0,   558,
     520,     0,     0,    23,    24,    25,     0,    96,     0,     0,
       0,     0,   565,   585,   566,   599,   567,   571,   572,   573,
     574,   591,   578,   579,   580,   581,   582,   583,   584,   586,
     587,   588,   589,   640,   570,   577,   590,   647,   654,   568,
     575,   569,   576,     0,     0,     0,   598,   609,   612,   610,
     611,   667,   535,     0,     0,   494,     0,   174,   175,   172,
     123,   124,   126,   125,   127,   128,   129,   130,   156,   157,
     154,   155,   147,   158,   159,   148,   145,   146,   173,   167,
       0,   171,   160,   161,   162,   163,   134,   135,   136,   131,
     132,   133,   144,     0,   150,   151,   149,   142,   143,   138,
     137,   139,   140,   141,   122,   121,   166,     0,   152,   153,
     463,   118,   235,   211,   592,   595,   593,   596,   594,   597,
       0,     0,   547,     0,   527,   463,     0,     0,   509,   507,
     529,    85,   631,   634,   637,   627,     0,   604,   641,   648,
     655,   661,   664,     0,   617,   622,   616,     0,   630,   626,
     619,     0,     0,   621,     0,   603,   674,     0,     0,   176,
     177,   170,   165,   178,   168,   164,     0,   116,     0,   487,
       0,     0,   209,     0,   530,     0,   531,   545,     0,   551,
     476,     0,     0,     0,     0,     0,     0,   508,     0,     0,
       0,     0,     0,     0,     0,     0,   115,     0,   115,     0,
     115,     0,     0,   367,   368,     0,     0,     0,   361,     0,
       0,     0,     0,     0,     0,   591,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   452,     0,     0,     0,   311,
     313,   312,   314,   315,   316,   317,     0,    26,     0,     0,
       0,     0,     0,     0,     0,   298,   299,   365,   364,   309,
     446,   362,   438,   437,   436,   435,   113,   441,   363,   440,
     439,   408,   369,     0,   370,     0,   366,   675,   676,   677,
     678,   624,   625,   618,   620,     0,   623,   614,     0,   669,
     668,   493,   672,   495,   179,   169,     0,     0,     0,   560,
     484,   498,   117,   463,   119,   237,     0,    59,    60,     0,
     261,   259,     0,     0,     0,     0,   260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   214,   212,     0,     0,
       0,     0,   230,   225,   222,   221,   223,   224,   236,   216,
     215,     0,    67,    68,    65,   228,    66,   229,   231,   276,
     220,     0,   217,   318,     0,     0,   532,   548,   477,   521,
     464,     0,   511,   512,     0,   505,   506,   504,     0,     0,
       0,   607,     0,     0,   600,     0,     0,   118,     0,   118,
       0,   118,   246,   491,     0,   489,     0,   251,   255,   254,
     258,     0,   287,     0,   278,   281,     0,   284,     0,     0,
     397,   398,     0,     0,     0,     0,     0,     0,     0,   689,
       0,     0,   713,   718,     0,   239,     0,   374,   373,   413,
      32,   372,   371,     0,   300,   444,     0,   307,     0,     0,
       0,     0,   399,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   358,     0,   601,     0,   615,   613,     0,   485,     0,
       0,   497,     0,   496,     0,   499,     0,   488,     0,     0,
       0,   265,     0,   262,     0,     0,   249,     0,     0,   234,
       0,     0,    53,    73,     0,   270,   267,   299,   245,   243,
     244,     0,     0,   232,   233,     0,    70,   219,   226,   227,
     264,   269,   275,     0,     0,   218,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   546,     0,     0,   559,     0,   463,
     510,     0,   514,     0,   518,   318,   632,   635,     0,   638,
     628,   605,     0,   642,     0,   649,     0,   656,     0,     0,
     662,     0,   665,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,     0,   709,   687,   689,
       0,   714,     0,     0,     0,   689,     0,     0,   692,     0,
       0,   720,     0,    29,     0,    27,     0,   401,     0,     0,
     350,   347,   349,     0,   409,     0,   293,     0,   115,     0,
       0,   424,   423,     0,     0,   425,   429,   375,   376,   388,
     389,   386,   387,     0,   418,     0,   406,     0,   442,   443,
     377,   378,   393,   394,   395,   396,     0,     0,   391,   392,
     390,   384,   385,   380,   379,   381,   382,   383,     0,     0,
       0,   356,     0,     0,     0,     0,   411,     0,   486,     0,
     562,   563,   500,   503,   502,    74,     0,   266,   263,     0,
       0,     0,   250,    75,    76,    51,    52,   271,   268,   300,
     238,   235,    56,     0,   277,   242,   328,   329,   331,   330,
     332,   322,   323,   324,   333,   334,     0,     0,     0,   320,
     321,   335,   336,   325,   326,   327,     0,     0,     0,   319,
     533,     0,   470,   473,     0,   513,   516,   318,   517,   633,
     636,   608,   639,   629,   606,   643,   645,   650,   652,   657,
     659,   490,   663,   492,   666,     0,   256,     0,     0,     0,
       0,   414,     0,     0,   415,     0,   445,   307,   447,   448,
     449,   450,   451,     0,     0,   690,     0,     0,     0,   689,
       0,     0,     0,     0,   698,   699,     0,   704,     0,   696,
       0,     0,   716,     0,     0,     0,   694,     0,   717,   712,
     719,   691,     0,     0,    30,    33,     0,     0,     0,   115,
       0,     0,     0,   410,     0,     0,     0,     0,   305,     0,
     118,   420,     0,   426,     0,     0,     0,   404,     0,     0,
     430,   434,     0,     0,   407,     0,     0,     0,   357,     0,
     359,   402,     0,   412,   561,     0,     0,     0,   272,     0,
       0,     0,    54,    55,     0,    69,    61,   342,   340,   341,
     339,   337,   338,   114,   471,   472,   473,   474,   465,   478,
     515,   646,   653,   660,   252,   257,     0,   290,   288,   279,
     282,   285,     0,     0,     0,     0,   680,   679,   689,     0,
     710,     0,     0,   688,   703,   697,   711,   695,   715,     0,
     689,   701,   702,     0,   707,   693,   240,    28,    31,     0,
       0,     0,   118,     0,     0,   294,   303,   304,   302,   301,
       0,     0,     0,     0,     0,     0,     0,   345,     0,     0,
     431,     0,   419,     0,   405,     0,     0,   403,   360,   564,
     501,     0,    77,   213,    57,     0,    62,     0,   483,   468,
     465,   466,   467,   470,   253,     0,     0,   289,     0,     0,
       0,     0,   453,     0,     0,     0,     0,   705,     0,     0,
       0,     0,     0,   700,     0,   348,   462,   351,     0,   343,
       0,   306,   308,     0,   295,   310,     0,   461,     0,   459,
     346,   456,     0,     0,     0,   455,     0,    72,    56,     0,
      71,   469,   479,   468,     0,     0,     0,   280,   283,   286,
     416,     0,   454,   721,     0,     0,   706,     0,   682,   681,
     721,   708,   241,     0,   354,     0,     0,   296,   421,   427,
       0,   460,   458,   457,    58,   261,     0,    63,    67,    68,
      65,    66,    64,     0,   481,   475,   291,     0,   417,     0,
       0,   684,   683,     0,     0,   352,     0,   344,   297,   422,
     428,   432,     0,   262,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,   355,   433,   263,   268,   480,     0,
     292,   722,   723,   686,   685,   724,   482
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1139, -1139, -1139, -1139, -1139,   345,   884, -1139, -1139, -1139,
   -1139, -1139, -1139, -1139,   597,   919, -1139,   840, -1139, -1139,
     891, -1139, -1139, -1139,  -215, -1139, -1139, -1139,  -214, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139, -1139,   758, -1139, -1139,
     898,   -57, -1139, -1139,   422,   242,  -396,  -384,  -561, -1139,
   -1139, -1139, -1138, -1139, -1139,  -193, -1139,    88, -1139, -1139,
   -1139, -1139,  -474,   -12, -1139, -1139, -1139, -1139, -1139,  -209,
    -208,  -207, -1139,  -206, -1139, -1139,   973, -1139, -1139, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
    -641, -1139, -1139,  -125, -1139,    30,  -462, -1139,  -368, -1139,
   -1139, -1139, -1108, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
   -1139,   403, -1139, -1139, -1139, -1139, -1139, -1139, -1139,  -138,
    -152,  -204,  -153,   -65, -1139, -1139, -1139, -1139, -1139,   490,
   -1139,   231, -1139,  -349,   582,  -480,  -483,   431, -1139, -1139,
    -364, -1139, -1139,   941, -1139, -1139, -1139,   614,   140, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
   -1139, -1139, -1139, -1139,  -465,  -156, -1139,   585, -1139,   609,
   -1139, -1139, -1139, -1139,  -351, -1139, -1139, -1139, -1139, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,
   -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139, -1139,  -139,
   -1139, -1139, -1139, -1139, -1139,   587,  -782,  -586,  -757, -1139,
   -1139, -1139,  -943,  -159, -1139,    23,   220,   400, -1139, -1139,
   -1139,  -195, -1139
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   130,    46,    14,   148,   154,   606,   447,   985,
     448,   796,    15,    16,    39,    40,    41,    77,    17,    34,
      35,   518,   519,  1034,  1035,   520,  1127,  1179,   521,   522,
     883,   523,   524,   525,   526,   527,  1029,   155,   156,    30,
      31,    32,    56,    57,    58,    59,    18,   311,   372,   193,
      19,    98,   194,    99,   140,   449,   528,   373,   529,   450,
     792,  1154,   530,   451,   531,   578,   754,  1134,   452,   532,
     533,   534,   535,   536,   537,   538,   539,   703,   540,   453,
     759,  1138,   760,  1139,   762,  1140,   454,   757,  1137,   455,
     805,  1165,   456,   998,   999,   808,   457,   458,   672,   542,
     459,   460,   802,   461,   988,  1203,   989,  1236,   462,   854,
    1118,   806,  1103,  1239,  1105,  1240,  1172,  1255,   464,   368,
    1133,  1182,  1046,  1048,   914,   548,   729,  1223,  1247,   369,
     370,   573,   574,   255,   256,   958,   490,   558,   388,   329,
     330,   200,   325,    67,   108,    21,   145,   321,    79,   123,
      80,    22,    43,   102,   142,    23,   379,   198,   199,    65,
     105,    24,   143,   323,   491,   465,   320,   248,   249,   161,
     250,   394,   924,   560,   251,   392,   923,   389,   919,   390,
     920,   391,   922,   395,   925,   396,  1051,   397,   927,   398,
    1052,   399,   929,   400,  1053,   401,   932,   402,   934,   575,
      25,    48,   132,    26,    47,   466,   778,   779,   780,   791,
     978,   972,   967,  1084,   467,   781,   603,   782,   468,   604,
     469,  1230,   470
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,   313,   247,   480,   658,   541,   744,   202,   746,   483,
     748,   666,   567,   857,   569,   785,   571,   549,  -113,   252,
    1074,   991,   963,   471,   109,   110,   671,   866,   975,   473,
     344,   772,   954,   135,   669,   554,    73,   685,     2,    60,
     772,   487,    60,    60,     3,   618,   619,   661,   973,   704,
     688,   689,   690,   555,  1163,   375,   345,   346,    27,    28,
     617,   628,    78,   556,   630,   631,  1131,     4,   366,     5,
      74,     6,   354,   201,   545,   599,   601,     7,   366,    60,
      60,    60,    60,   201,   734,  1246,     8,   630,   631,    91,
      92,    93,     9,    49,   355,  1235,   994,   788,   738,   376,
     661,   377,   661,   546,   662,   663,   995,   664,   125,  1259,
     665,   557,   957,    10,   937,   661,   347,   254,    87,   555,
     348,    50,    44,  1132,   367,   957,  1147,   312,  1254,   556,
     615,   705,   789,   771,   790,   783,    11,   622,   623,   488,
     356,   773,   651,   652,   326,   628,   246,   629,   630,   631,
     632,    87,   728,   990,   996,   633,   134,   489,   663,   997,
     663,   354,    69,   482,    45,   651,   652,   111,    94,    91,
     616,    93,   112,   663,   113,   472,   114,   557,   349,    42,
    1073,   474,   350,    81,   195,   351,   662,   381,   962,   163,
      51,   662,   863,    95,    33,   749,   867,    52,   247,   475,
     352,   247,  1069,  1196,    29,    75,   732,   662,   476,   353,
     877,   327,   196,   726,   115,   247,    76,   375,   481,  1022,
     126,   727,    69,    94,    53,  1027,   651,   652,    66,   328,
     247,   208,   494,   247,   247,   247,   247,   209,   764,   247,
      12,   247,   885,   247,   741,   247,    13,   382,  1043,  1121,
     559,   561,   562,   563,   210,    54,   566,   661,   568,   916,
     570,   376,   385,   724,    55,   386,   742,  1149,   387,   211,
     247,   247,    68,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   149,   150,   206,   674,
     955,  1146,  1160,  1064,   661,   663,  1022,   683,   661,   848,
     849,  1022,    52,  1152,  1194,   961,   344,   976,  1251,    36,
      37,   798,  1079,    69,   969,   207,   799,    61,    62,   884,
      63,    96,   246,    70,    52,   246,  1144,    97,   850,    53,
    1093,  1151,   345,   346,    52,  1081,    38,   245,  1082,   246,
     851,   971,   970,   977,  1108,   992,   547,   971,    64,   344,
     800,    53,   663,  1068,   246,    52,   663,   246,   246,   246,
     246,    53,  1026,   246,   798,   246,  1022,   246,  1080,   246,
    1022,  1083,   993,   246,  1022,   345,   346,   852,   853,   964,
     965,  1022,    53,    71,   483,   588,  1129,  1100,   942,   943,
     312,   945,   347,  1023,   246,   246,   348,  1055,   479,   488,
    -644,  1170,  1143,   961,  1000,  -644,  1047,   966,  1192,   610,
     618,   619,  1101,   247,  1037,  1038,  1039,   489,    78,  1102,
      83,   982,  -651,  -644,  1040,  1041,  1042,  -651,  1205,   151,
     750,  1126,    52,   354,   152,   347,   153,   736,   114,   348,
     618,   619,   812,   816,   314,  -651,    36,    37,   315,   426,
     427,   428,   751,    85,   349,   865,    88,    89,   350,    53,
     947,   351,   752,  1031,   873,  1135,  -658,   316,   317,   318,
     319,  -658,    84,   100,  1032,  1033,   352,   103,   880,   101,
     577,    87,   882,   104,   753,   353,  -353,  1136,   247,  -658,
      86,  -353,   106,   136,   137,   138,   139,   349,   107,    76,
     825,   350,   622,   623,   351,   862,   773,   146,   147,  -353,
     628,  1158,   826,   630,   631,   632,   128,  1013,   990,   352,
     633,   354,   129,  1017,  1071,   739,   618,   619,   353,  1014,
     620,   621,   622,   623,   624,  1018,  1072,   625,   626,   627,
     628,  1044,   629,   630,   631,   632,    90,  1045,   118,   756,
     633,   354,   634,   635,   124,   740,   120,   246,   636,   637,
     638,   344,   247,   354,   639,   492,   354,   743,   493,   127,
     745,   913,  1150,   247,   133,   247,   354,   131,   247,   921,
     747,   247,   801,   247,   247,  1092,   247,   345,   346,   354,
     931,   651,   652,  1054,   141,   935,   144,   354,   940,    74,
     640,  1058,   641,   642,   643,   644,   645,   646,   647,   648,
     649,   650,   146,   147,   793,   794,   247,  1091,   622,   623,
     354,   651,   652,   354,  1209,   158,   628,  1241,   629,   630,
     631,   632,   246,   159,   445,   687,   633,   312,    91,    92,
      93,   160,   541,   203,   204,  1195,    91,   347,   203,   204,
     205,   348,   121,   122,   197,   201,   162,   253,  1028,   254,
    1011,   308,   309,   310,   312,   332,   322,   324,   333,   334,
     335,   618,   619,   336,   337,   338,   339,   340,   341,   342,
     354,  1225,   343,   358,   357,   359,   360,   361,   363,   364,
     362,   365,   371,   378,    60,   648,   649,   650,   384,   393,
     484,   383,   485,   486,   552,   374,   246,   651,   652,   349,
     550,   553,   582,   350,   380,  1059,   351,   246,   564,   246,
     344,   581,   246,   583,   584,   246,   463,   246,   246,   655,
     246,   352,  1166,  1252,   585,   478,   586,   587,   588,   589,
     353,   592,   593,   605,    12,   594,   345,   346,   660,   595,
     596,   678,   597,   680,   618,   619,   543,   614,   692,   654,
     246,   620,   621,   622,   623,   624,   551,   659,   625,   626,
     627,   628,   679,   629,   630,   631,   632,   693,   694,   696,
     697,   633,   698,   634,   635,   702,   699,   700,   701,   636,
     637,   638,   731,   737,   755,   639,   807,   823,   860,   861,
     590,   591,   704,   871,   874,   875,   347,   876,   879,   881,
     348,   602,   247,   912,   956,   926,   915,   355,  1124,   961,
     986,   607,   608,   609,   928,   611,   612,   613,   981,   930,
     984,   640,   968,   641,   642,   643,   644,   645,   646,   647,
     648,   649,   650,   987,   620,   621,   622,   623,   624,  1001,
    1002,   625,   651,   652,   628,  1003,   629,   630,   631,   632,
     312,  1020,  1004,  1005,   633,  1006,   634,   635,   349,   657,
    1016,  1025,   350,  1047,  1060,   351,  1049,  1056,  1063,   668,
    1075,  1077,  1085,   673,  1088,  1110,   676,   677,  1096,  1164,
     352,  1097,   682,  1098,   686,   618,   619,  1099,   691,   353,
    1111,  1113,  1116,   695,  1119,  1141,  1148,  1128,  1177,  1153,
    1159,  1162,  1178,  1181,  1186,  1237,  1204,  1180,  1227,   644,
     645,   646,   647,   648,   649,   650,  1250,   247,   725,   247,
    1249,   795,   116,  1258,  1262,   651,   652,    72,  1253,  1265,
     157,   735,   119,  1214,   331,  1217,  1168,   117,   911,  1030,
    1218,  1219,  1220,  1221,    20,  1161,   246,  1062,  1183,  1224,
    1184,  1130,   933,   667,   576,   733,   758,    82,   544,   761,
     579,   763,   580,  1201,  1078,   765,   766,   767,   768,   769,
     770,   786,   777,   565,   777,  1234,     0,   622,   623,   980,
       0,     0,     0,     0,     0,   628,   247,   629,   630,   631,
     632,     0,     0,   817,   818,   633,     0,   819,   820,   821,
     822,     0,   824,  1210,   827,   828,   829,   830,   831,   832,
     833,   834,   835,   837,   838,   839,   840,   841,   842,   843,
     844,   845,   846,   847,     0,   855,     0,     0,     0,     0,
       0,     0,   859,     0,   618,   619,  1266,     0,     0,   864,
       0,     0,     0,   868,     0,     0,     0,     0,   872,     0,
       0,     0,   646,   647,   648,   649,   650,   878,     0,     0,
       0,   246,     0,   246,     0,     0,   651,   652,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   886,
     887,   888,   889,   890,   891,   892,   893,   894,   895,   899,
     900,   901,   902,   903,   904,   905,   909,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   917,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   620,   621,   622,   623,   624,   939,
     246,   625,   626,   627,   628,     0,   629,   630,   631,   632,
       0,     0,     0,     0,   633,     0,   634,   635,   607,   613,
       0,     0,   636,     0,   638,     0,     0,   613,     0,     0,
     979,   801,   602,     0,     0,     0,     0,   983,     0,     0,
       0,     0,     0,     0,     0,   -64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   618,   619,     0,     0,
       0,     0,     0,     0,   801,     0,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   651,   652,     0,     0,     0,
       0,     0,  1015,     0,     0,     0,  1019,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
       0,     0,   899,   909,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1036,     0,     0,     0,
       0,     0,   345,   346,     0,     0,     0,     0,   706,   707,
     708,   709,   710,   711,   712,   713,   620,   621,   622,   623,
     624,   714,   715,   625,   626,   627,   628,   716,   629,   630,
     631,   632,     0,     0,     0,     0,   633,   717,   634,   635,
     718,   719,     0,     0,   636,   637,   638,   720,   721,   722,
     639,   344,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,     0,     0,     0,   348,     0,     0,  1065,
    1066,  1067,     0,  1070,   618,   619,     0,   345,   346,     0,
       0,  1076,     0,     0,   777,   723,   640,     0,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,     0,  1089,
    1090,     0,     0,     0,     0,  1095,     0,   651,   652,     0,
       0,     0,     0,     0,     0,  1104,     0,  1106,   344,  1109,
       0,     0,     0,   344,   349,  1112,     0,     0,   350,  1115,
    1061,   351,     0,     0,     0,  1095,     0,   347,     0,  1120,
       0,   348,  1122,   543,   345,   346,   352,  1125,     0,   345,
     346,     0,     0,     0,     0,   353,     0,     0,     0,     0,
       0,     0,     0,     0,   620,   621,   622,   623,   624,     0,
       0,   625,   626,   627,   628,     0,   629,   630,   631,   632,
       0,     0,     0,     0,   633,     0,   634,   635,     0,     0,
       0,     0,   636,   637,   638,     0,     0,     0,   639,   349,
       0,     0,     0,   350,   347,  1086,   351,     0,   348,   347,
       0,     0,     0,   348,     0,     0,     0,     0,     0,     0,
       0,   352,     0,     0,  1173,     0,  1174,     0,     0,  1176,
     353,     0,     0,     0,   640,     0,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,     0,     0,  1185,     0,
       0,  1187,  1188,  1189,  1191,   651,   652,  1193,     0,   653,
       0,  1197,  1198,  1199,  1200,     0,   349,  1202,     0,     0,
     350,   349,  1157,   351,     0,   350,  1207,  1208,   351,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,     0,
       0,     0,  1222,   352,     0,     0,     0,   353,     0,     0,
       0,     0,   353,     0,     0,     0,     0,  1231,  1232,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1238,
       0,     0,     0,     0,     0,     0,     0,     0,  1243,  1245,
       0,     0,     0,     0,   495,     0,     0,     0,     0,     3,
    1248,   496,   497,   498,     0,   499,     0,   403,   404,   405,
     406,   407,     0,     0,     0,  1256,     0,  1257,   500,   408,
     501,   502,  1261,     0,  1263,  1264,     0,     0,     0,     0,
     503,   409,     0,   504,     0,   505,   410,     0,     0,   411,
       0,     8,   412,   506,     0,   507,   413,     0,     0,   508,
     509,     0,     0,     0,     0,     0,   510,     0,     0,   212,
     213,   214,     0,   216,   217,   218,   219,   220,   415,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
       0,   234,   235,   236,     0,     0,   239,   240,   241,   242,
       0,     0,   416,   511,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   417,   418,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     512,   513,   514,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   419,   420,
     421,   422,   423,     0,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,    53,   515,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     438,   439,   440,     0,   441,     0,     0,   442,   443,     0,
       0,     0,     0,     0,     0,     0,     0,   444,     0,   445,
     446,   516,   312,   517,   495,     0,     0,     0,     0,     3,
       0,   496,   497,   498,     0,   499,     0,   403,   404,   405,
     406,   407,     0,     0,     0,     0,     0,     0,   500,   408,
     501,   502,     0,     0,     0,     0,     0,     0,     0,     0,
     503,   409,     0,   504,     0,   505,   410,     0,     0,   411,
       0,     8,   412,   506,     0,   507,   413,     0,     0,   508,
     509,     0,     0,     0,     0,     0,   510,     0,     0,   212,
     213,   214,     0,   216,   217,   218,   219,   220,   415,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
       0,   234,   235,   236,     0,     0,   239,   240,   241,   242,
       0,     0,   416,   511,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   417,   418,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     512,   513,   514,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   419,   420,
     421,   422,   423,     0,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,    53,   515,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     438,   439,   440,     0,   441,     0,     0,   442,   443,     0,
       0,     0,   403,   404,   405,   406,   407,   444,     0,   445,
     446,   516,   312,  1123,   408,     0,     0,     0,     0,     0,
     344,     0,     0,     0,     0,     0,   409,     0,     0,     0,
       0,   410,     0,     0,   411,     0,     0,   412,     0,   772,
       0,   413,     0,     0,     0,     0,   345,   346,     0,     0,
       0,   414,     0,     0,   212,   213,   214,     0,   216,   217,
     218,   219,   220,   415,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,     0,   234,   235,   236,     0,
       0,   239,   240,   241,   242,     0,     0,   416,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     417,   418,     0,     0,     0,     0,   347,     0,     0,     0,
     348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   419,   420,   421,   422,   423,     0,   424,
     773,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   774,   436,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,     0,
       0,     0,   350,     0,     0,   775,   439,   440,     0,   441,
       0,     0,   442,   443,   403,   404,   405,   406,   407,     0,
     352,     0,   776,     0,   445,   446,   408,   312,     0,   353,
       0,     0,   344,     0,     0,     0,     0,     0,   409,     0,
       0,     0,     0,   410,     0,     0,   411,     0,     0,   412,
       0,     0,     0,   413,     0,     0,     0,     0,   345,   346,
       0,     0,     0,   414,     0,     0,   212,   213,   214,     0,
     216,   217,   218,   219,   220,   415,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,     0,   234,   235,
     236,     0,     0,   239,   240,   241,   242,     0,     0,   416,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   417,   418,     0,     0,     0,     0,   347,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   419,   420,   421,   422,   423,
       0,   424,   773,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   774,   436,   437,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     349,     0,     0,     0,   350,     0,     0,   775,   439,   440,
       0,   441,     0,     0,   442,   443,   403,   404,   405,   406,
     407,     0,   352,     0,   784,     0,   445,   446,   408,   312,
       0,   353,     0,     0,   473,     0,     0,     0,     0,     0,
     409,     0,     0,     0,     0,   410,     0,     0,   411,     0,
       0,   412,     0,     0,     0,   413,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   414,     0,     0,   212,   213,
     214,     0,   216,   217,   218,   219,   220,   415,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     234,   235,   236,     0,     0,   239,   240,   241,   242,     0,
       0,   416,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   417,   418,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   419,   420,   421,
     422,   423,     0,   424,     0,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,    53,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,     0,     0,   438,
     439,   440,     0,   441,     0,     0,   442,   443,   403,   404,
     405,   406,   407,     0,   475,     0,   444,     0,   445,   446,
     408,   312,     0,   476,     0,     0,     0,     0,     0,     0,
       0,     0,   409,     0,     0,     0,     0,   410,     0,     0,
     411,     0,     0,   412,     0,     0,     0,   413,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   414,     0,     0,
     212,   213,   214,     0,   216,   217,   218,   219,   220,   415,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   234,   235,   236,     0,     0,   239,   240,   241,
     242,     0,     0,   416,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   417,   418,     0,     0,
       0,     0,     0,     0,     0,   670,     0,     0,     0,     0,
       0,   512,   513,   514,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   419,
     420,   421,   422,   423,     0,   424,     0,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,    53,   436,
     437,     0,     0,     0,     0,     0,     0,   403,   404,   405,
     406,   407,     0,     0,     0,     0,     0,     0,     0,   408,
       0,   438,   439,   440,     0,   441,     0,     0,   442,   443,
       0,   409,     0,     0,     0,     0,   410,     0,   444,   411,
     445,   446,   412,   312,     0,     0,   413,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   414,     0,     0,   212,
     213,   214,     0,   216,   217,   218,   219,   220,   415,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
       0,   234,   235,   236,     0,     0,   239,   240,   241,   242,
       0,     0,   416,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   417,   418,     0,     0,     0,
       0,     0,     0,     0,   684,     0,     0,     0,     0,     0,
     512,   513,   514,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   419,   420,
     421,   422,   423,     0,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,    53,   436,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     438,   439,   440,     0,   441,     0,     0,   442,   443,     0,
       0,     0,   403,   404,   405,   406,   407,   444,     0,   445,
     446,     0,   312,  1215,   408,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   409,     0,     0,     0,
       0,   410,     0,     0,   411,     0,     0,   412,   506,     0,
       0,   413,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   414,     0,     0,   212,   213,   214,     0,   216,   217,
     218,   219,   220,   415,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,     0,   234,   235,   236,     0,
       0,   239,   240,   241,   242,     0,     0,   416,  1216,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     417,   418,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   419,   420,   421,   422,   423,     0,   424,
       0,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,   435,    53,   436,   437,     0,     0,     0,     0,     0,
       0,   403,   404,   405,   406,   407,     0,     0,     0,     0,
       0,     0,     0,   408,     0,   438,   439,   440,     0,   441,
       0,     0,   442,   443,     0,   409,     0,     0,     0,     0,
     410,     0,   444,   411,   445,   446,   412,   312,     0,     0,
     413,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     414,     0,     0,   212,   213,   214,     0,   216,   217,   218,
     219,   220,   415,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,     0,   234,   235,   236,     0,     0,
     239,   240,   241,   242,     0,     0,   416,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   417,
     418,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   512,   513,   514,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   419,   420,   421,   422,   423,     0,   424,     0,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,    53,   436,   437,     0,     0,     0,     0,     0,     0,
     403,   404,   405,   406,   407,     0,     0,     0,     0,     0,
       0,     0,   408,     0,   438,   439,   440,     0,   441,     0,
       0,   442,   443,     0,   409,     0,     0,     0,     0,   410,
       0,   444,   411,   445,   446,   412,   312,     0,     0,   413,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   414,
       0,     0,   212,   213,   214,     0,   216,   217,   218,   219,
     220,   415,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   234,   235,   236,     0,     0,   239,
     240,   241,   242,     0,     0,   416,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   417,   418,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   896,   897,   898,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   419,   420,   421,   422,   423,     0,   424,     0,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
      53,   436,   437,     0,     0,     0,     0,     0,     0,   403,
     404,   405,   406,   407,     0,     0,     0,     0,     0,     0,
       0,   408,     0,   438,   439,   440,     0,   441,     0,     0,
     442,   443,     0,   409,     0,     0,     0,     0,   410,     0,
     444,   411,   445,   446,   412,   312,     0,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   414,     0,
       0,   212,   213,   214,     0,   216,   217,   218,   219,   220,
     415,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,     0,   234,   235,   236,     0,     0,   239,   240,
     241,   242,     0,     0,   416,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   417,   418,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   906,   907,   908,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     419,   420,   421,   422,   423,     0,   424,     0,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,    53,
     436,   437,     0,     0,     0,     0,     0,     0,   403,   404,
     405,   406,   407,     0,     0,     0,     0,     0,     0,     0,
     408,     0,   438,   439,   440,     0,   441,     0,     0,   442,
     443,     0,   409,     0,     0,     0,     0,   410,     0,   444,
     411,   445,   446,   412,   312,     0,     0,   413,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   414,     0,     0,
     212,   213,   214,     0,   216,   217,   218,   219,   220,   415,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   234,   235,   236,     0,     0,   239,   240,   241,
     242,     0,     0,   416,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   417,   418,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   419,
     420,   421,   422,   423,     0,   424,     0,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,    53,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,   440,     0,   441,     0,     0,   442,   443,
     403,   404,   405,   406,   407,     0,   803,     0,   444,   804,
     445,   446,   408,   312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   409,     0,     0,     0,     0,   410,
       0,     0,   411,     0,     0,   412,     0,     0,     0,   413,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   414,
       0,     0,   212,   213,   214,     0,   216,   217,   218,   219,
     220,   415,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   234,   235,   236,     0,     0,   239,
     240,   241,   242,     0,     0,   416,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   417,   418,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   419,   420,   421,   422,   423,     0,   424,     0,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
      53,   436,   437,     0,     0,     0,     0,     0,     0,   403,
     404,   405,   406,   407,     0,     0,     0,     0,     0,     0,
       0,   408,     0,   438,   439,   440,     0,   441,     0,     0,
     442,   443,     0,   409,     0,     0,     0,     0,   410,   477,
     444,   411,   445,   446,   412,   312,     0,     0,   413,     0,
       0,     0,     0,     0,   675,     0,     0,     0,   414,     0,
       0,   212,   213,   214,     0,   216,   217,   218,   219,   220,
     415,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,     0,   234,   235,   236,     0,     0,   239,   240,
     241,   242,     0,     0,   416,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   417,   418,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     419,   420,   421,   422,   423,     0,   424,     0,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,    53,
     436,   437,     0,     0,     0,     0,     0,     0,   403,   404,
     405,   406,   407,     0,     0,     0,     0,     0,     0,     0,
     408,     0,   438,   439,   440,     0,   441,     0,     0,   442,
     443,     0,   409,     0,     0,     0,     0,   410,     0,   444,
     411,   445,   446,   412,   312,     0,     0,   413,     0,     0,
     681,     0,     0,     0,     0,     0,     0,   414,     0,     0,
     212,   213,   214,     0,   216,   217,   218,   219,   220,   415,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   234,   235,   236,     0,     0,   239,   240,   241,
     242,     0,     0,   416,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   417,   418,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   419,
     420,   421,   422,   423,     0,   424,     0,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,    53,   436,
     437,     0,     0,     0,     0,     0,     0,   403,   404,   405,
     406,   407,     0,     0,     0,     0,     0,     0,     0,   408,
       0,   438,   439,   440,     0,   441,     0,     0,   442,   443,
       0,   409,     0,     0,     0,     0,   410,     0,   444,   411,
     445,   446,   412,   312,     0,     0,   413,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   414,     0,     0,   212,
     213,   214,     0,   216,   217,   218,   219,   220,   415,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
       0,   234,   235,   236,     0,     0,   239,   240,   241,   242,
       0,     0,   416,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   417,   418,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   419,   420,
     421,   422,   423,     0,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,    53,   436,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   605,     0,
     438,   439,   440,     0,   441,     0,     0,   442,   443,   403,
     404,   405,   406,   407,     0,     0,   836,   444,     0,   445,
     446,   408,   312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   409,     0,     0,     0,     0,   410,     0,
       0,   411,     0,     0,   412,     0,     0,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   414,     0,
       0,   212,   213,   214,     0,   216,   217,   218,   219,   220,
     415,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,     0,   234,   235,   236,     0,     0,   239,   240,
     241,   242,     0,     0,   416,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   417,   418,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     419,   420,   421,   422,   423,     0,   424,     0,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,    53,
     436,   437,     0,     0,     0,     0,     0,     0,   403,   404,
     405,   406,   407,     0,     0,     0,     0,     0,     0,     0,
     408,     0,   438,   439,   440,     0,   441,     0,     0,   442,
     443,     0,   409,     0,     0,     0,     0,   410,     0,   444,
     411,   445,   446,   412,   312,     0,     0,   413,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   414,     0,     0,
     212,   213,   214,     0,   216,   217,   218,   219,   220,   415,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   234,   235,   236,     0,     0,   239,   240,   241,
     242,     0,     0,   416,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   417,   418,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   419,
     420,   421,   422,   423,     0,   424,     0,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,    53,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,   440,     0,   441,     0,     0,   442,   443,
     403,   404,   405,   406,   407,     0,     0,     0,   444,   856,
     445,   446,   408,   312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   409,     0,     0,     0,     0,   410,
       0,     0,   411,     0,     0,   412,     0,     0,     0,   413,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   414,
       0,     0,   212,   213,   214,     0,   216,   217,   218,   219,
     220,   415,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   234,   235,   236,     0,     0,   239,
     240,   241,   242,     0,     0,   416,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   417,   418,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   419,   420,   421,   422,   423,     0,   424,     0,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
      53,   436,   437,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   438,   439,   440,     0,   441,     0,     0,
     442,   443,   403,   404,   405,   406,   407,     0,     0,     0,
     444,   936,   445,   446,   408,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   409,     0,     0,     0,
       0,   410,     0,     0,   411,     0,     0,   412,     0,     0,
       0,   413,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   414,     0,     0,   212,   213,   214,     0,   216,   217,
     218,   219,   220,   415,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,     0,   234,   235,   236,     0,
       0,   239,   240,   241,   242,     0,     0,   416,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     417,   418,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   419,   420,   421,   422,   423,     0,   424,
       0,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,   435,    53,   436,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   938,     0,   438,   439,   440,     0,   441,
       0,     0,   442,   443,   403,   404,   405,   406,   407,     0,
       0,     0,   444,     0,   445,   446,   408,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   409,     0,
       0,     0,     0,   410,     0,     0,   411,     0,     0,   412,
       0,     0,     0,   413,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   414,     0,     0,   212,   213,   214,     0,
     216,   217,   218,   219,   220,   415,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,     0,   234,   235,
     236,     0,     0,   239,   240,   241,   242,     0,     0,   416,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   417,   418,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   419,   420,   421,   422,   423,
       0,   424,     0,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,    53,   436,   437,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   438,   439,   440,
       0,   441,     0,     0,   442,   443,   403,   404,   405,   406,
     407,     0,     0,     0,   444,   959,   445,   446,   408,   312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     409,     0,     0,     0,     0,   410,     0,     0,   411,     0,
       0,   412,     0,     0,     0,   413,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   414,     0,     0,   212,   213,
     214,     0,   216,   217,   218,   219,   220,   415,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     234,   235,   236,     0,     0,   239,   240,   241,   242,     0,
       0,   416,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   417,   418,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,   419,   420,   421,
     422,   423,     0,   424,     0,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,    53,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   438,
     439,   440,     0,   441,     0,     0,   442,   443,   403,   404,
     405,   406,   407,     0,     0,     0,   444,   974,   445,   446,
     408,   312,     0,     0,   618,   619,     0,     0,     0,     0,
       0,     0,   409,     0,     0,     0,     0,   410,     0,     0,
     411,     0,     0,   412,     0,     0,     0,   413,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   414,     0,     0,
     212,   213,   214,     0,   216,   217,   218,   219,   220,   415,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   234,   235,   236,     0,     0,   239,   240,   241,
     242,     0,     0,   416,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   417,   418,     0,     0,
       0,     0,     0,     0,   620,   621,   622,   623,     0,     0,
       0,     0,     0,     0,   628,     0,   629,   630,   631,   632,
       0,    52,     0,     0,   633,     0,   634,   635,     0,   419,
     420,   421,   422,   423,     0,   424,     0,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,    53,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,   404,   405,   406,   407,
       0,   438,   439,   440,     0,   441,     0,   408,   442,   443,
       0,   646,   647,   648,   649,   650,  1094,     0,   444,   409,
     445,   446,     0,   312,   410,   651,   652,   411,     0,     0,
     412,     0,     0,     0,   413,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   414,     0,     0,   212,   213,   214,
       0,   216,   217,   218,   219,   220,   415,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,     0,   234,
     235,   236,     0,     0,   239,   240,   241,   242,     0,     0,
     416,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   417,   418,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   419,   420,   421,   422,
     423,     0,   424,     0,   425,   426,   427,   428,   429,   430,
     431,   432,   433,   434,   435,    53,   436,   437,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   438,   439,
     440,     0,   441,     0,     0,   442,   443,   403,   404,   405,
     406,   407,     0,     0,     0,   444,  1107,   445,   446,   408,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   409,     0,     0,     0,     0,   410,     0,     0,   411,
       0,     0,   412,     0,     0,     0,   413,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   414,     0,     0,   212,
     213,   214,     0,   216,   217,   218,   219,   220,   415,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
       0,   234,   235,   236,     0,     0,   239,   240,   241,   242,
       0,     0,   416,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   417,   418,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   419,   420,
     421,   422,   423,     0,   424,     0,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,    53,   436,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     438,   439,   440,     0,   441,     0,     0,   442,   443,   403,
     404,   405,   406,   407,     0,     0,     0,   444,  1142,   445,
     446,   408,   312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   409,     0,     0,     0,     0,   410,     0,
       0,   411,     0,     0,   412,     0,     0,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   414,     0,
       0,   212,   213,   214,     0,   216,   217,   218,   219,   220,
     415,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,     0,   234,   235,   236,     0,     0,   239,   240,
     241,   242,     0,     0,   416,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   417,   418,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     419,   420,   421,   422,   423,     0,   424,     0,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,    53,
     436,   437,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   438,   439,   440,     0,   441,     0,     0,   442,
     443,   403,   404,   405,   406,   407,     0,     0,     0,   444,
    1190,   445,   446,   408,   312,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   409,     0,     0,     0,     0,
     410,     0,     0,   411,     0,     0,   412,     0,     0,     0,
     413,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     414,     0,     0,   212,   213,   214,     0,   216,   217,   218,
     219,   220,   415,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,     0,   234,   235,   236,     0,     0,
     239,   240,   241,   242,     0,     0,   416,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   417,
     418,     0,     0,     0,     0,     0,     0,     0,  1206,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   419,   420,   421,   422,   423,     0,   424,     0,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,    53,   436,   437,     0,     0,     0,     0,     0,     0,
     403,   404,   405,   406,   407,     0,     0,     0,     0,     0,
       0,     0,   408,     0,   438,   439,   440,     0,   441,     0,
       0,   442,   443,     0,   409,     0,     0,     0,     0,   410,
       0,   444,   411,   445,   446,   412,   312,     0,     0,   413,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   414,
       0,     0,   212,   213,   214,     0,   216,   217,   218,   219,
     220,   415,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   234,   235,   236,     0,     0,   239,
     240,   241,   242,     0,     0,   416,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   417,   418,
       0,     0,     0,     0,     0,     0,     0,  1242,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   419,   420,   421,   422,   423,     0,   424,     0,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
      53,   436,   437,     0,     0,     0,     0,     0,     0,   403,
     404,   405,   406,   407,     0,     0,     0,     0,     0,     0,
       0,   408,     0,   438,   439,   440,     0,   441,     0,     0,
     442,   443,     0,   409,     0,     0,     0,     0,   410,     0,
     444,   411,   445,   446,   412,   312,     0,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   414,     0,
       0,   212,   213,   214,     0,   216,   217,   218,   219,   220,
     415,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,     0,   234,   235,   236,     0,     0,   239,   240,
     241,   242,     0,     0,   416,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   417,   418,     0,
       0,     0,     0,     0,     0,     0,  1244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     419,   420,   421,   422,   423,     0,   424,     0,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,    53,
     436,   437,     0,     0,     0,     0,     0,     0,   403,   404,
     405,   406,   407,     0,     0,     0,     0,     0,     0,     0,
     408,     0,   438,   439,   440,     0,   441,     0,     0,   442,
     443,     0,   409,     0,     0,     0,     0,   410,     0,   444,
     411,   445,   446,   412,   312,     0,     0,   413,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   414,     0,     0,
     212,   213,   214,     0,   216,   217,   218,   219,   220,   415,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   234,   235,   236,     0,     0,   239,   240,   241,
     242,     0,     0,   416,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   417,   418,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   618,   619,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   419,
     420,   421,   422,   423,     0,   424,     0,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,    53,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,   440,     0,   441,     0,     0,   442,   443,
       0,     0,     0,     0,     0,     0,     0,     0,   444,     0,
     445,   446,     0,   312,   706,   707,   708,   709,   710,   711,
     712,   713,   620,   621,   622,   623,   624,   714,   715,   625,
     626,   627,   628,   869,   629,   630,   631,   632,     0,   618,
     619,     0,   633,   717,   634,   635,   718,   719,     0,     0,
     636,   637,   638,   720,   721,   722,   639,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   618,   619,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   870,   640,     0,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   651,   652,     0,     0,     0,     0,     0,
     918,   706,   707,   708,   709,   710,   711,   712,   713,   620,
     621,   622,   623,   624,   714,   715,   625,   626,   627,   628,
     869,   629,   630,   631,   632,     0,     0,     0,     0,   633,
     717,   634,   635,   718,   719,     0,     0,   636,   637,   638,
     720,   721,   722,   639,     0,     0,   620,   621,   622,   623,
     624,     0,     0,   625,   626,   627,   628,     0,   629,   630,
     631,   632,   618,   619,     0,     0,   633,     0,   634,   635,
       0,     0,     0,     0,   636,   637,   638,     0,   870,   640,
     639,   641,   642,   643,   644,   645,   646,   647,   648,   649,
     650,     0,     0,     0,     0,     0,     0,   618,   619,     0,
     651,   652,     0,     0,     0,     0,     0,  1050,     0,     0,
       0,     0,     0,     0,     0,     0,   640,     0,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   651,   652,     0,
       0,     0,     0,     0,   910,     0,     0,     0,     0,     0,
       0,     0,   620,   621,   622,   623,   624,     0,     0,   625,
     626,   627,   628,     0,   629,   630,   631,   632,     0,     0,
       0,     0,   633,     0,   634,   635,     0,     0,     0,     0,
     636,   637,   638,     0,     0,     0,   639,   620,   621,   622,
     623,   624,     0,     0,   625,   626,   627,   628,     0,   629,
     630,   631,   632,   618,   619,     0,     0,   633,     0,   634,
     635,     0,     0,     0,     0,   636,   637,   638,     0,     0,
       0,   639,   640,     0,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,     0,     0,     0,     0,   618,   619,
       0,     0,     0,   651,   652,     0,     0,     0,     0,     0,
    1229,     0,     0,     0,     0,     0,     0,   640,     0,   641,
     642,   643,   644,   645,   646,   647,   648,   649,   650,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   651,   652,
       0,     0,   730,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   620,   621,   622,   623,   624,     0,     0,
     625,   626,   627,   628,     0,   629,   630,   631,   632,     0,
       0,     0,     0,   633,     0,   634,   635,     0,     0,     0,
       0,   636,   637,   638,     0,     0,     0,   639,   620,   621,
     622,   623,   624,     0,     0,   625,   626,   627,   628,     0,
     629,   630,   631,   632,   618,   619,     0,     0,   633,     0,
     634,   635,     0,     0,     0,     0,   636,   637,   638,     0,
       0,     0,   639,   640,     0,   641,   642,   643,   644,   645,
     646,   647,   648,   649,   650,     0,     0,     0,     0,   618,
     619,     0,     0,     0,   651,   652,     0,     0,   797,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   640,     0,
     641,   642,   643,   644,   645,   646,   647,   648,   649,   650,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   651,
     652,     0,     0,   858,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   620,   621,   622,   623,   624,     0,
       0,   625,   626,   627,   628,     0,   629,   630,   631,   632,
       0,     0,     0,     0,   633,     0,   634,   635,     0,     0,
       0,     0,   636,   637,   638,     0,     0,     0,   639,   620,
     621,   622,   623,   624,     0,     0,   625,   626,   627,   628,
       0,   629,   630,   631,   632,   618,   619,     0,     0,   633,
       0,   634,   635,     0,     0,     0,     0,   636,   637,   638,
       0,     0,     0,   639,   640,     0,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,     0,     0,     0,     0,
     618,   619,     0,     0,     0,   651,   652,     0,     0,   941,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   640,
       0,   641,   642,   643,   644,   645,   646,   647,   648,   649,
     650,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     651,   652,     0,     0,   944,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   620,   621,   622,   623,   624,
       0,     0,   625,   626,   627,   628,     0,   629,   630,   631,
     632,     0,     0,     0,     0,   633,     0,   634,   635,     0,
       0,     0,     0,   636,   637,   638,     0,     0,     0,   639,
     620,   621,   622,   623,   624,     0,     0,   625,   626,   627,
     628,     0,   629,   630,   631,   632,   618,   619,     0,     0,
     633,     0,   634,   635,     0,     0,     0,     0,   636,   637,
     638,     0,     0,     0,   639,   640,     0,   641,   642,   643,
     644,   645,   646,   647,   648,   649,   650,     0,     0,     0,
       0,   618,   619,     0,     0,     0,   651,   652,     0,     0,
     946,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     640,     0,   641,   642,   643,   644,   645,   646,   647,   648,
     649,   650,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   651,   652,     0,     0,   948,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   620,   621,   622,   623,
     624,     0,     0,   625,   626,   627,   628,     0,   629,   630,
     631,   632,     0,     0,     0,     0,   633,     0,   634,   635,
       0,     0,     0,     0,   636,   637,   638,     0,     0,     0,
     639,   620,   621,   622,   623,   624,     0,     0,   625,   626,
     627,   628,     0,   629,   630,   631,   632,   618,   619,     0,
       0,   633,     0,   634,   635,     0,     0,     0,     0,   636,
     637,   638,     0,     0,     0,   639,   640,     0,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,     0,     0,
       0,     0,   618,   619,     0,     0,     0,   651,   652,     0,
       0,   949,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   640,     0,   641,   642,   643,   644,   645,   646,   647,
     648,   649,   650,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   651,   652,     0,     0,   950,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   620,   621,   622,
     623,   624,     0,     0,   625,   626,   627,   628,     0,   629,
     630,   631,   632,     0,     0,     0,     0,   633,     0,   634,
     635,     0,     0,     0,     0,   636,   637,   638,     0,     0,
       0,   639,   620,   621,   622,   623,   624,     0,     0,   625,
     626,   627,   628,     0,   629,   630,   631,   632,   618,   619,
       0,     0,   633,     0,   634,   635,     0,     0,     0,     0,
     636,   637,   638,     0,     0,     0,   639,   640,     0,   641,
     642,   643,   644,   645,   646,   647,   648,   649,   650,     0,
       0,     0,     0,   618,   619,     0,     0,     0,   651,   652,
       0,     0,   951,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   640,     0,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   651,   652,     0,     0,   952,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   620,   621,
     622,   623,   624,     0,     0,   625,   626,   627,   628,     0,
     629,   630,   631,   632,     0,     0,     0,     0,   633,     0,
     634,   635,     0,     0,     0,     0,   636,   637,   638,     0,
       0,     0,   639,   620,   621,   622,   623,   624,     0,     0,
     625,   626,   627,   628,     0,   629,   630,   631,   632,   618,
     619,     0,     0,   633,     0,   634,   635,     0,     0,     0,
       0,   636,   637,   638,     0,     0,     0,   639,   640,     0,
     641,   642,   643,   644,   645,   646,   647,   648,   649,   650,
       0,     0,     0,     0,   618,   619,     0,     0,     0,   651,
     652,     0,     0,   953,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   640,     0,   641,   642,   643,   644,   645,
     646,   647,   648,   649,   650,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   651,   652,     0,     0,  1024,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   620,
     621,   622,   623,   624,     0,     0,   625,   626,   627,   628,
       0,   629,   630,   631,   632,     0,     0,     0,     0,   633,
       0,   634,   635,     0,     0,     0,     0,   636,   637,   638,
       0,     0,     0,   639,   620,   621,   622,   623,   624,     0,
       0,   625,   626,   627,   628,     0,   629,   630,   631,   632,
     618,   619,     0,     0,   633,     0,   634,   635,     0,     0,
       0,     0,   636,   637,   638,     0,     0,     0,   639,   640,
       0,   641,   642,   643,   644,   645,   646,   647,   648,   649,
     650,     0,     0,     0,     0,   618,   619,     0,     0,     0,
     651,   652,     0,     0,  1057,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   640,     0,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   651,   652,     0,     0,  1145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     620,   621,   622,   623,   624,     0,     0,   625,   626,   627,
     628,     0,   629,   630,   631,   632,     0,     0,     0,     0,
     633,     0,   634,   635,     0,     0,     0,     0,   636,   637,
     638,     0,     0,     0,   639,   620,   621,   622,   623,   624,
       0,     0,   625,   626,   627,   628,     0,   629,   630,   631,
     632,   618,   619,     0,     0,   633,     0,   634,   635,     0,
       0,     0,     0,   636,   637,   638,     0,     0,     0,   639,
     640,     0,   641,   642,   643,   644,   645,   646,   647,   648,
     649,   650,     0,     0,     0,     0,   618,   619,     0,     0,
       0,   651,   652,     0,     0,  1155,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   640,     0,   641,   642,   643,
     644,   645,   646,   647,   648,   649,   650,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   651,   652,     0,     0,
    1156,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   620,   621,   622,   623,   624,     0,     0,   625,   626,
     627,   628,     0,   629,   630,   631,   632,     0,     0,     0,
       0,   633,     0,   634,   635,     0,     0,     0,     0,   636,
     637,   638,     0,     0,     0,   639,   620,   621,   622,   623,
     624,     0,     0,   625,   626,   627,   628,     0,   629,   630,
     631,   632,   618,   619,     0,     0,   633,     0,   634,   635,
       0,     0,     0,     0,   636,   637,   638,     0,     0,     0,
     639,   640,     0,   641,   642,   643,   644,   645,   646,   647,
     648,   649,   650,     0,     0,     0,     0,   618,   619,     0,
       0,     0,   651,   652,     0,     0,  1167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   640,     0,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   651,   652,     0,
       0,  1169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   620,   621,   622,   623,   624,     0,     0,   625,
     626,   627,   628,     0,   629,   630,   631,   632,     0,     0,
       0,     0,   633,     0,   634,   635,     0,     0,     0,     0,
     636,   637,   638,     0,     0,     0,   639,   620,   621,   622,
     623,   624,     0,     0,   625,   626,   627,   628,     0,   629,
     630,   631,   632,   618,   619,     0,     0,   633,     0,   634,
     635,     0,     0,     0,     0,   636,   637,   638,     0,     0,
       0,   639,   640,     0,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,     0,     0,     0,     0,   618,   619,
       0,     0,     0,   651,   652,     0,     0,  1171,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   640,     0,   641,
     642,   643,   644,   645,   646,   647,   648,   649,   650,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   651,   652,
       0,     0,  1175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   620,   621,   622,   623,   624,     0,     0,
     625,   626,   627,   628,     0,   629,   630,   631,   632,     0,
       0,     0,     0,   633,     0,   634,   635,     0,     0,     0,
       0,   636,   637,   638,     0,     0,     0,   639,   620,   621,
     622,   623,   624,     0,     0,   625,   626,   627,   628,     0,
     629,   630,   631,   632,   618,   619,     0,     0,   633,     0,
     634,   635,     0,     0,     0,     0,   636,   637,   638,     0,
       0,     0,   639,   640,     0,   641,   642,   643,   644,   645,
     646,   647,   648,   649,   650,     0,     0,     0,     0,   618,
     619,     0,     0,     0,   651,   652,     0,     0,  1211,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   640,     0,
     641,   642,   643,   644,   645,   646,   647,   648,   649,   650,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   651,
     652,     0,     0,  1212,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   620,   621,   622,   623,   624,     0,
       0,   625,   626,   627,   628,     0,   629,   630,   631,   632,
       0,     0,     0,     0,   633,     0,   634,   635,     0,     0,
       0,     0,   636,   637,   638,     0,     0,     0,   639,   620,
     621,   622,   623,   624,     0,     0,   625,   626,   627,   628,
       0,   629,   630,   631,   632,   618,   619,     0,     0,   633,
       0,   634,   635,     0,     0,     0,     0,   636,   637,   638,
       0,     0,     0,   639,   640,     0,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,     0,     0,     0,     0,
     618,   619,     0,     0,     0,   651,   652,     0,     0,  1213,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   640,
       0,   641,   642,   643,   644,   645,   646,   647,   648,   649,
     650,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     651,   652,     0,     0,  1226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   620,   621,   622,   623,   624,
       0,     0,   625,   626,   627,   628,     0,   629,   630,   631,
     632,     0,     0,     0,     0,   633,     0,   634,   635,     0,
       0,     0,     0,   636,   637,   638,     0,     0,     0,   639,
     620,   621,   622,   623,   624,     0,     0,   625,   626,   627,
     628,     0,   629,   630,   631,   632,   618,   619,     0,     0,
     633,     0,   634,   635,     0,     0,     0,     0,   636,   637,
     638,     0,     0,     0,   639,   640,     0,   641,   642,   643,
     644,   645,   646,   647,   648,   649,   650,     0,     0,     0,
       0,   618,   619,     0,     0,     0,   651,   652,     0,     0,
    1228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     640,     0,   641,   642,   643,   644,   645,   646,   647,   648,
     649,   650,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   651,   652,     0,     0,  1233,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   620,   621,   622,   623,
     624,     0,     0,   625,   626,   627,   628,     0,   629,   630,
     631,   632,     0,     0,     0,     0,   633,     0,   634,   635,
       0,     0,     0,     0,   636,   637,   638,     0,     0,     0,
     639,   620,   621,   622,   623,   624,   618,   619,   625,   626,
     627,   628,     0,   629,   630,   631,   632,     0,     0,     0,
       0,   633,     0,   634,   635,     0,     0,     0,     0,   636,
     637,   638,     0,     0,     0,   639,   640,     0,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   651,   652,     0,
       0,  1260,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   640,     0,   641,   642,   643,   644,   645,   646,   647,
     648,   649,   650,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   651,   652,   656,     0,   620,   621,   622,   623,
     624,   618,   619,   625,   626,   627,   628,     0,   629,   630,
     631,   632,     0,     0,     0,     0,   633,     0,   634,   635,
       0,     0,     0,     0,   636,   637,   638,     0,     0,     0,
     639,     0,     0,     0,     0,     0,   618,   619,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   640,     0,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   651,   652,  1007,
       0,   620,   621,   622,   623,   624,     0,     0,   625,   626,
     627,   628,     0,   629,   630,   631,   632,     0,     0,     0,
       0,   633,     0,   634,   635,     0,     0,     0,     0,   636,
     637,   638,     0,     0,     0,   639,   620,   621,   622,   623,
     624,   618,   619,   625,   626,   627,   628,     0,   629,   630,
     631,   632,     0,     0,     0,     0,   633,     0,   634,   635,
       0,     0,     0,     0,   636,   637,   638,     0,     0,     0,
     639,   640,     0,   641,   642,   643,   644,   645,   646,   647,
     648,   649,   650,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   651,   652,  1021,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   640,     0,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   651,   652,  1114,
       0,   620,   621,   622,   623,   624,     0,     0,   625,   626,
     627,   628,     0,   629,   630,   631,   632,   618,   619,     0,
       0,   633,     0,   634,   635,     0,     0,     0,     0,   636,
     637,   638,     0,     0,     0,   639,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   640,     0,   641,   642,   643,   644,   645,   646,   647,
     648,   649,   650,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   651,   652,  1117,     0,     0,     0,     0,   706,
     707,   708,   709,   710,   711,   712,   713,   620,   621,   622,
     623,   624,   714,   715,   625,   626,   627,   628,   869,   629,
     630,   631,   632,  -318,     0,   257,   258,   633,   717,   634,
     635,   718,   719,     0,     0,   636,   637,   638,   720,   721,
     722,   639,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     618,   619,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   870,   640,     0,   641,
     642,   643,   644,   645,   646,   647,   648,   649,   650,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   651,   652,
       0,     0,     0,     0,     0,     0,     0,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,     0,     0,   278,   279,   280,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
     285,     0,     0,   286,   287,   288,   289,   290,   291,   292,
     620,   621,   622,   623,   624,   618,   619,   625,   626,   627,
     628,     0,   629,   630,   631,   632,     0,     0,     0,     0,
     633,     0,   634,   635,     0,     0,   787,     0,   636,   637,
     638,     0,     0,     0,   639,   293,     0,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,     0,     0,   304,
     305,     0,     0,     0,     0,     0,   306,   307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     640,     0,   641,   642,   643,   644,   645,   646,   647,   648,
     649,   650,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   651,   652,     0,     0,   620,   621,   622,   623,   624,
     618,   619,   625,   626,   627,   628,     0,   629,   630,   631,
     632,     0,     0,     0,     0,   633,     0,   634,   635,     0,
       0,   960,     0,   636,   637,   638,     0,     0,     0,   639,
       0,     0,     0,     0,     0,   618,   619,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   640,     0,   641,   642,   643,
     644,   645,   646,   647,   648,   649,   650,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   651,   652,     0,     0,
     620,   621,   622,   623,   624,     0,     0,   625,   626,   627,
     628,     0,   629,   630,   631,   632,     0,     0,     0,     0,
     633,     0,   634,   635,     0,     0,     0,     0,   636,   637,
     638,   618,   619,     0,   639,   620,   621,   622,   623,   624,
       0,     0,   625,   626,   627,   628,     0,   629,   630,   631,
     632,     0,     0,     0,     0,   633,     0,   634,   635,     0,
       0,     0,     0,   636,   637,   638,   618,   619,     0,   639,
     640,  1012,   641,   642,   643,   644,   645,   646,   647,   648,
     649,   650,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   651,   652,     0,     0,     0,  1087,     0,     0,     0,
       0,     0,     0,     0,     0,   640,     0,   641,   642,   643,
     644,   645,   646,   647,   648,   649,   650,     0,     0,     0,
       0,   620,   621,   622,   623,   624,   651,   652,   625,   626,
     627,   628,     0,   629,   630,   631,   632,     0,     0,     0,
       0,   633,     0,   634,   635,     0,     0,     0,     0,   636,
     637,   638,   618,   619,     0,   639,   620,   621,   622,   623,
     624,     0,     0,   625,   626,   627,   628,     0,   629,   630,
     631,   632,     0,     0,     0,     0,   633,     0,   634,   635,
       0,     0,     0,     0,   636,   637,   638,   618,   619,     0,
    -659,   640,     0,   641,   642,   643,   644,   645,   646,   647,
     648,   649,   650,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   651,   652,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   640,     0,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,     0,     0,
       0,     0,   620,   621,   622,   623,   624,   651,   652,   625,
     626,   627,   628,     0,   629,   630,   631,   632,     0,     0,
       0,     0,   633,     0,   634,   635,     0,     0,     0,     0,
     636,   637,   638,   618,   619,     0,     0,   620,   621,   622,
     623,   624,     0,     0,   625,   626,   627,   628,     0,   629,
     630,   631,   632,     0,     0,     0,     0,   633,     0,   634,
     635,   618,   619,     0,     0,   636,     0,     0,     0,     0,
       0,     0,   640,     0,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   651,   652,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   641,
     642,   643,   644,   645,   646,   647,   648,   649,   650,     0,
       0,     0,     0,   620,   621,   622,   623,   624,   651,   652,
     625,   626,   627,   628,     0,   629,   630,   631,   632,     0,
       0,     0,     0,   633,     0,   634,   635,   618,   619,     0,
       0,   620,   621,   622,   623,   624,     0,     0,   625,   626,
     627,   628,     0,   629,   630,   631,   632,     0,     0,     0,
       0,   633,     0,   634,   635,   618,   619,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   641,   642,   643,   644,   645,
     646,   647,   648,   649,   650,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   651,   652,     0,     0,     0,     0,
       0,     0,     0,     0,   642,   643,   644,   645,   646,   647,
     648,   649,   650,     0,     0,     0,     0,   620,   621,   622,
     623,   624,   651,   652,   625,   626,   627,   628,     0,   629,
     630,   631,   632,     0,     0,     0,     0,   633,     0,   634,
     635,     0,     0,     0,     0,   620,   621,   622,   623,   624,
       0,     0,   625,   626,   627,   628,     0,   629,   630,   631,
     632,     0,     0,     0,     0,   633,     0,   634,   635,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   643,   644,   645,   646,   647,   648,   649,   650,   598,
       0,     0,     0,     0,     0,     0,     0,     0,   651,   652,
       0,   208,     0,     0,     0,     0,     0,   209,     0,     0,
     644,   645,   646,   647,   648,   649,   650,     0,     0,     0,
       0,     0,     0,     0,   210,     0,   651,   652,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   211,
       0,     0,     0,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   600,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     208,     0,     0,     0,     0,     0,   209,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,   210,     0,     0,     0,   245,   809,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   211,     0,
       0,    53,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   212,   213,   214,     0,
     216,   217,   218,   219,   220,   415,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,     0,   234,   235,
     236,     0,     0,   239,   240,   241,   242,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   813,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1008,     0,     0,     0,     0,     0,
       0,     0,   810,     0,     0,     0,     0,     0,     0,     0,
       0,   212,   213,   214,   811,   216,   217,   218,   219,   220,
     415,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,     0,   234,   235,   236,     0,     0,   239,   240,
     241,   242,   212,   213,   214,     0,   216,   217,   218,   219,
     220,   415,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   234,   235,   236,     0,     0,   239,
     240,   241,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   814,     0,   208,
       0,     0,     0,     0,     0,   209,     0,     0,     0,   815,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   210,     0,     0,     0,     0,     0,  1009,     0,
       0,     0,     0,     0,     0,     0,     0,   211,     0,     0,
    1010,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   208,     0,
       0,     0,     0,     0,   209,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,   210,     0,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   211,     0,     0,    53,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   164,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,   165,
       0,   166,     0,   167,   168,   169,   170,   171,   572,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
       0,   183,   184,   185,     0,     0,   186,   187,   188,   189,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   190,   191,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   192
};

static const yytype_int16 yycheck[] =
{
      12,   194,   158,   354,   487,   373,   567,   145,   569,   358,
     571,   491,   396,   654,   398,   601,   400,   381,     7,   158,
     963,   803,   779,    32,    14,    15,   500,    19,   785,    32,
      32,    51,    19,    90,   499,   386,    21,   511,     0,    51,
      51,    48,    54,    55,     6,    20,    21,   121,    19,   126,
     512,   513,   514,   121,   136,   164,    58,    59,    18,    19,
     456,   120,   164,   131,   123,   124,    44,    29,   149,    31,
      55,    33,   182,   154,   179,   426,   427,    39,   149,    91,
      92,    93,    94,   154,   558,  1223,    48,   123,   124,   138,
     139,   140,    54,    33,   164,  1203,   121,   173,   208,   208,
     121,   210,   121,   208,   178,   179,   131,   181,   210,  1247,
     184,   179,   131,    75,   755,   121,   118,   164,   137,   121,
     122,    61,   164,   101,   205,   131,  1069,   209,  1236,   131,
     164,   208,   208,   598,   210,   600,    98,   112,   113,   146,
     210,   152,   201,   202,   201,   120,   158,   122,   123,   124,
     125,   137,   548,   164,   179,   130,   205,   164,   179,   184,
     179,   182,   178,   210,   206,   201,   202,   157,   178,   138,
     204,   140,   162,   179,   164,   184,   166,   179,   180,   209,
     962,   184,   184,    43,   181,   187,   178,   325,   208,   205,
     130,   178,   666,   203,   157,   181,   670,   137,   354,   202,
     202,   357,   959,  1146,   164,   190,   208,   178,   211,   211,
     684,   146,   209,   202,   204,   371,   201,   164,   357,   178,
      80,   210,   178,   178,   164,   866,   201,   202,    60,   164,
     386,    24,   371,   389,   390,   391,   392,    30,   589,   395,
     202,   397,   704,   399,   186,   401,   208,   203,   203,   208,
     389,   390,   391,   392,    47,   195,   395,   121,   397,   733,
     399,   208,   178,   210,   204,   181,   208,   131,   184,    62,
     426,   427,   179,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    14,    15,   178,   502,
     772,  1068,  1094,   954,   121,   179,   178,   510,   121,   123,
     124,   178,   137,  1080,   131,   178,    32,   174,   131,   164,
     165,   146,   973,   178,   175,   205,   151,     4,     5,   703,
       7,    55,   354,   181,   137,   357,   208,    61,   152,   164,
     203,   208,    58,    59,   137,   174,   191,   150,   177,   371,
     164,   208,   203,   210,  1005,   178,   378,   208,    35,    32,
     185,   164,   179,   959,   386,   137,   179,   389,   390,   391,
     392,   164,   862,   395,   146,   397,   178,   399,   974,   401,
     178,   210,   205,   405,   178,    58,    59,   201,   202,   175,
     176,   178,   164,   178,   753,   204,    45,   178,   759,   760,
     209,   762,   118,   205,   426,   427,   122,   205,   211,   146,
     181,   205,  1063,   178,   808,   186,    65,   203,   205,   441,
      20,    21,   203,   589,   896,   897,   898,   164,   164,  1000,
     164,   792,   181,   204,   906,   907,   908,   186,   203,   157,
     186,    11,   137,   182,   162,   118,   164,   186,   166,   122,
      20,    21,   618,   619,    70,   204,   164,   165,    74,   154,
     155,   156,   208,    51,   180,   668,    54,    55,   184,   164,
     186,   187,   186,    11,   677,   186,   181,    93,    94,    95,
      96,   186,   164,    55,    22,    23,   202,    55,   691,    61,
     185,   137,   695,    61,   208,   211,   181,   208,   664,   204,
     164,   186,    55,    91,    92,    93,    94,   180,    61,   201,
     152,   184,   112,   113,   187,   664,   152,   167,   168,   204,
     120,  1092,   164,   123,   124,   125,    55,   152,   164,   202,
     130,   182,    61,   152,   152,   186,    20,    21,   211,   164,
     110,   111,   112,   113,   114,   164,   164,   117,   118,   119,
     120,    55,   122,   123,   124,   125,   204,    61,   157,   581,
     130,   182,   132,   133,   208,   186,   164,   589,   138,   139,
     140,    32,   738,   182,   144,   205,   182,   186,   208,   208,
     186,   729,  1072,   749,   164,   751,   182,   209,   754,   738,
     186,   757,   614,   759,   760,   989,   762,    58,    59,   182,
     749,   201,   202,   186,   164,   754,   209,   182,   757,    55,
     180,   186,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   167,   168,   169,   170,   792,   988,   112,   113,
     182,   201,   202,   182,   186,   179,   120,   186,   122,   123,
     124,   125,   664,   208,   206,   207,   130,   209,   138,   139,
     140,   164,  1030,   167,   168,  1145,   138,   118,   167,   168,
     169,   122,    75,    76,   101,   154,   209,   208,   871,   164,
     836,    34,    34,   204,   209,   185,   164,   210,   185,   185,
     185,    20,    21,   204,   185,   185,   185,   185,   185,   185,
     182,  1184,   204,   208,   181,   164,   164,   164,    21,   164,
     203,   203,   181,   181,   726,   189,   190,   191,    41,   164,
     164,   204,   131,   204,   164,   209,   738,   201,   202,   180,
     203,   164,   185,   184,   209,   186,   187,   749,   205,   751,
      32,   204,   754,   204,   185,   757,   343,   759,   760,   203,
     762,   202,  1103,  1233,   185,   352,   204,   185,   204,   185,
     211,   204,   204,   185,   202,   204,    58,    59,    41,   204,
     204,   164,   204,   157,    20,    21,   373,   207,    36,   204,
     792,   110,   111,   112,   113,   114,   383,   204,   117,   118,
     119,   120,   208,   122,   123,   124,   125,   208,   208,     9,
     208,   130,   208,   132,   133,    64,   208,   208,   208,   138,
     139,   140,    41,   186,   204,   144,   154,   164,   164,   164,
     417,   418,   126,    13,   179,   181,   118,   157,   207,   209,
     122,   428,   988,     7,   204,   186,   164,   164,  1031,   178,
     204,   438,   439,   440,   186,   442,   443,   444,   210,   186,
     167,   180,   178,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   204,   110,   111,   112,   113,   114,   185,
     204,   117,   201,   202,   120,   185,   122,   123,   124,   125,
     209,     1,   204,   204,   130,   204,   132,   133,   180,   486,
     204,    41,   184,    65,   186,   187,   208,   164,   204,   496,
     203,   203,   203,   500,   167,   185,   503,   504,   164,  1102,
     202,   164,   509,   164,   511,    20,    21,   164,   515,   211,
     204,   204,   204,   520,   164,   204,   204,   208,  1121,   203,
     205,   203,  1125,    32,   164,   205,   186,   208,   186,   185,
     186,   187,   188,   189,   190,   191,   203,  1103,   545,  1105,
      51,   606,    68,   208,   203,   201,   202,    38,   210,   203,
     120,   558,    71,  1178,   206,  1179,  1105,    69,   726,   881,
    1179,  1179,  1179,  1179,     1,  1100,   988,   947,  1130,  1183,
    1133,  1046,   751,   493,   402,   554,   583,    46,   374,   586,
     405,   588,   405,  1152,   971,   592,   593,   594,   595,   596,
     597,   601,   599,   394,   601,  1200,    -1,   112,   113,   789,
      -1,    -1,    -1,    -1,    -1,   120,  1172,   122,   123,   124,
     125,    -1,    -1,   620,   621,   130,    -1,   624,   625,   626,
     627,    -1,   629,  1172,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,    -1,   652,    -1,    -1,    -1,    -1,
      -1,    -1,   659,    -1,    20,    21,  1259,    -1,    -1,   666,
      -1,    -1,    -1,   670,    -1,    -1,    -1,    -1,   675,    -1,
      -1,    -1,   187,   188,   189,   190,   191,   684,    -1,    -1,
      -1,  1103,    -1,  1105,    -1,    -1,   201,   202,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   706,
     707,   708,   709,   710,   711,   712,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,   723,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   733,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,   756,
    1172,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,   132,   133,   775,   776,
      -1,    -1,   138,    -1,   140,    -1,    -1,   784,    -1,    -1,
     787,  1203,   789,    -1,    -1,    -1,    -1,   794,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,  1236,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,
      -1,    -1,   849,    -1,    -1,    -1,   853,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   869,   870,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   883,    -1,    -1,    -1,
      -1,    -1,    58,    59,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,   135,    -1,    -1,   138,   139,   140,   141,   142,   143,
     144,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,   122,    -1,    -1,   956,
     957,   958,    -1,   960,    20,    21,    -1,    58,    59,    -1,
      -1,   968,    -1,    -1,   971,   179,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,   986,
     987,    -1,    -1,    -1,    -1,   992,    -1,   201,   202,    -1,
      -1,    -1,    -1,    -1,    -1,  1002,    -1,  1004,    32,  1006,
      -1,    -1,    -1,    32,   180,  1012,    -1,    -1,   184,  1016,
     186,   187,    -1,    -1,    -1,  1022,    -1,   118,    -1,  1026,
      -1,   122,  1029,  1030,    58,    59,   202,  1034,    -1,    58,
      59,    -1,    -1,    -1,    -1,   211,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,    -1,
      -1,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,
      -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,   180,
      -1,    -1,    -1,   184,   118,   186,   187,    -1,   122,   118,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,    -1,    -1,  1111,    -1,  1113,    -1,    -1,  1116,
     211,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,  1135,    -1,
      -1,  1138,  1139,  1140,  1141,   201,   202,  1144,    -1,   205,
      -1,  1148,  1149,  1150,  1151,    -1,   180,  1154,    -1,    -1,
     184,   180,   186,   187,    -1,   184,  1163,   186,   187,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,    -1,
      -1,    -1,  1179,   202,    -1,    -1,    -1,   211,    -1,    -1,
      -1,    -1,   211,    -1,    -1,    -1,    -1,  1194,  1195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1215,  1216,
      -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,    -1,     6,
    1227,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,  1242,    -1,  1244,    25,    26,
      27,    28,  1249,    -1,  1251,  1252,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,    -1,   206,
     207,   208,   209,   210,     1,    -1,    -1,    -1,    -1,     6,
      -1,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,
      -1,    -1,    14,    15,    16,    17,    18,   204,    -1,   206,
     207,   208,   209,   210,    26,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    51,
      -1,    53,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,
      -1,    63,    -1,    -1,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    90,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,   184,    -1,    -1,   187,   188,   189,    -1,   191,
      -1,    -1,   194,   195,    14,    15,    16,    17,    18,    -1,
     202,    -1,   204,    -1,   206,   207,    26,   209,    -1,   211,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    58,    59,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,   184,    -1,    -1,   187,   188,   189,
      -1,   191,    -1,    -1,   194,   195,    14,    15,    16,    17,
      18,    -1,   202,    -1,   204,    -1,   206,   207,    26,   209,
      -1,   211,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,
     148,   149,    -1,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    14,    15,
      16,    17,    18,    -1,   202,    -1,   204,    -1,   206,   207,
      26,   209,    -1,   211,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,    -1,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,    46,
     206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    -1,
      -1,    -1,    14,    15,    16,    17,    18,   204,    -1,   206,
     207,    -1,   209,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    90,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,   148,   149,    -1,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   187,   188,   189,    -1,   191,
      -1,    -1,   194,   195,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   204,    46,   206,   207,    49,   209,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,   147,   148,   149,    -1,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   204,    46,   206,   207,    49,   209,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,   148,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     204,    46,   206,   207,    49,   209,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,   128,   129,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,   148,   149,    -1,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,
      46,   206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,    -1,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      14,    15,    16,    17,    18,    -1,   202,    -1,   204,   205,
     206,   207,    26,   209,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,   148,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    38,    -1,    -1,    -1,    -1,    43,   203,
     204,    46,   206,   207,    49,   209,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,   148,   149,    -1,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,
      46,   206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,    -1,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,    46,
     206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    14,
      15,    16,    17,    18,    -1,    -1,    21,   204,    -1,   206,
     207,    26,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,   148,   149,    -1,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,
      46,   206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,    -1,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   204,   205,
     206,   207,    26,   209,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,   148,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     204,   205,   206,   207,    26,   209,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    90,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,   148,   149,    -1,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,    -1,   187,   188,   189,    -1,   191,
      -1,    -1,   194,   195,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   204,    -1,   206,   207,    26,   209,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,   148,   149,
      -1,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,
      -1,   191,    -1,    -1,   194,   195,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   204,   205,   206,   207,    26,   209,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,
     148,   149,    -1,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
     188,   189,    -1,   191,    -1,    -1,   194,   195,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   204,   205,   206,   207,
      26,   209,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,   123,   124,   125,
      -1,   137,    -1,    -1,   130,    -1,   132,   133,    -1,   145,
     146,   147,   148,   149,    -1,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,   187,   188,   189,    -1,   191,    -1,    26,   194,   195,
      -1,   187,   188,   189,   190,   191,   202,    -1,   204,    38,
     206,   207,    -1,   209,    43,   201,   202,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,   148,
     149,    -1,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
     189,    -1,   191,    -1,    -1,   194,   195,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   204,   205,   206,   207,    26,
     209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   204,   205,   206,
     207,    26,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,   148,   149,    -1,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    14,    15,    16,    17,    18,    -1,    -1,    -1,   204,
     205,   206,   207,    26,   209,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,   147,   148,   149,    -1,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   204,    46,   206,   207,    49,   209,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,   148,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
     204,    46,   206,   207,    49,   209,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,   146,   147,   148,   149,    -1,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   204,
      46,   206,   207,    49,   209,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
     146,   147,   148,   149,    -1,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   204,    -1,
     206,   207,    -1,   209,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    20,
      21,    -1,   130,   131,   132,   133,   134,   135,    -1,    -1,
     138,   139,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   179,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,
     208,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,   135,    -1,    -1,   138,   139,   140,
     141,   142,   143,   144,    -1,    -1,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    20,    21,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,   179,   180,
     144,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
     201,   202,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    -1,    -1,    -1,   144,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    20,    21,    -1,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,
      -1,   144,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,   138,   139,   140,    -1,    -1,    -1,   144,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,   125,    20,    21,    -1,    -1,   130,    -1,
     132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,
      -1,    -1,   144,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,    -1,
      -1,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,
      -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,   110,
     111,   112,   113,   114,    -1,    -1,   117,   118,   119,   120,
      -1,   122,   123,   124,   125,    20,    21,    -1,    -1,   130,
      -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,
      -1,    -1,    -1,   144,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,
     110,   111,   112,   113,   114,    -1,    -1,   117,   118,   119,
     120,    -1,   122,   123,   124,   125,    20,    21,    -1,    -1,
     130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,
     140,    -1,    -1,    -1,   144,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   201,   202,    -1,    -1,
     205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    20,    21,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    -1,    -1,    -1,   144,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,   201,   202,    -1,
      -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,
      -1,   144,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    20,    21,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    -1,    -1,    -1,   144,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   201,   202,
      -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,   205,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,
     132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,
      -1,    -1,   144,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,   125,    20,
      21,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,   138,   139,   140,    -1,    -1,    -1,   144,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,   201,
     202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,   114,    -1,    -1,   117,   118,   119,   120,
      -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,
      -1,    -1,    -1,   144,   110,   111,   112,   113,   114,    -1,
      -1,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
      20,    21,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,
      -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,    -1,    -1,   117,   118,   119,
     120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,
     140,    -1,    -1,    -1,   144,   110,   111,   112,   113,   114,
      -1,    -1,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,    20,    21,    -1,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,
     205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    -1,    -1,    -1,   144,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    20,    21,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    -1,    -1,    -1,   144,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    20,    21,    -1,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,
      -1,   144,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,   201,   202,    -1,    -1,   205,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,
      -1,   138,   139,   140,    -1,    -1,    -1,   144,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,   125,    20,    21,    -1,    -1,   130,    -1,
     132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,    -1,
      -1,    -1,   144,   180,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,    -1,
      -1,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,    -1,
      -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,   110,
     111,   112,   113,   114,    -1,    -1,   117,   118,   119,   120,
      -1,   122,   123,   124,   125,    20,    21,    -1,    -1,   130,
      -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,
      -1,    -1,    -1,   144,   180,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,   201,   202,    -1,    -1,   205,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,
     110,   111,   112,   113,   114,    -1,    -1,   117,   118,   119,
     120,    -1,   122,   123,   124,   125,    20,    21,    -1,    -1,
     130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,
     140,    -1,    -1,    -1,   144,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,   201,   202,    -1,    -1,
     205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,   110,   111,   112,   113,   114,    20,    21,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    -1,    -1,    -1,   144,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,   203,    -1,   110,   111,   112,   113,
     114,    20,    21,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,   203,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    -1,    -1,    -1,   144,   110,   111,   112,   113,
     114,    20,    21,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,   203,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,   203,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    20,    21,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,   203,    -1,    -1,    -1,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,    20,    21,   130,   131,   132,
     133,   134,   135,    -1,    -1,   138,   139,   140,   141,   142,
     143,   144,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,   180,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
     135,    -1,    -1,   138,   139,   140,   141,   142,   143,   144,
     110,   111,   112,   113,   114,    20,    21,   117,   118,   119,
     120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,   132,   133,    -1,    -1,   136,    -1,   138,   139,
     140,    -1,    -1,    -1,   144,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,   194,
     195,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,   110,   111,   112,   113,   114,
      20,    21,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,
      -1,   136,    -1,   138,   139,   140,    -1,    -1,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,
     110,   111,   112,   113,   114,    -1,    -1,   117,   118,   119,
     120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,    -1,
     130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,
     140,    20,    21,    -1,   144,   110,   111,   112,   113,   114,
      -1,    -1,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,   138,   139,   140,    20,    21,    -1,   144,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,   114,   201,   202,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    20,    21,    -1,   144,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    20,    21,    -1,
     144,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   201,   202,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    20,    21,    -1,    -1,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,    20,    21,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,   114,   201,   202,
     117,   118,   119,   120,    -1,   122,   123,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,   132,   133,    20,    21,    -1,
      -1,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   201,   202,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,   185,   186,   187,   188,   189,   190,   191,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,   201,   202,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    -1,    -1,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    -1,   150,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,   164,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,   164,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
     164,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,   164,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    66,
      -1,    68,    -1,    70,    71,    72,    73,    74,   164,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   213,     0,     6,    29,    31,    33,    39,    48,    54,
      75,    98,   202,   208,   216,   224,   225,   230,   258,   262,
     288,   357,   363,   367,   373,   412,   415,    18,    19,   164,
     251,   252,   253,   157,   231,   232,   164,   165,   191,   226,
     227,   228,   209,   364,   164,   206,   215,   416,   413,    33,
      61,   130,   137,   164,   195,   204,   254,   255,   256,   257,
     275,     4,     5,     7,    35,   371,    60,   355,   179,   178,
     181,   178,   227,    21,    55,   190,   201,   229,   164,   360,
     362,   360,   355,   164,   164,   256,   164,   137,   256,   256,
     204,   138,   139,   140,   178,   203,    55,    61,   263,   265,
      55,    61,   365,    55,    61,   372,    55,    61,   356,    14,
      15,   157,   162,   164,   166,   204,   218,   252,   157,   232,
     164,   226,   226,   361,   208,   210,   360,   208,    55,    61,
     214,   209,   414,   164,   205,   253,   256,   256,   256,   256,
     266,   164,   366,   374,   209,   358,   167,   168,   217,    14,
      15,   157,   162,   164,   218,   249,   250,   229,   179,   208,
     164,   381,   209,   205,    34,    66,    68,    70,    71,    72,
      73,    74,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    88,    89,    90,    93,    94,    95,    96,
     112,   113,   164,   261,   264,   181,   209,   101,   369,   370,
     353,   154,   331,   167,   168,   169,   178,   205,    24,    30,
      47,    62,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,   150,   275,   377,   379,   380,
     382,   386,   411,   208,   164,   345,   346,    20,    21,    37,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   122,   123,
     124,   131,   132,   133,   134,   135,   138,   139,   140,   141,
     142,   143,   144,   180,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   194,   195,   201,   202,    34,    34,
     204,   259,   209,   267,    70,    74,    93,    94,    95,    96,
     378,   359,   164,   375,   210,   354,   253,   146,   164,   351,
     352,   249,   185,   185,   185,   185,   204,   185,   185,   185,
     185,   185,   185,   204,    32,    58,    59,   118,   122,   180,
     184,   187,   202,   211,   182,   164,   210,   181,   208,   164,
     164,   164,   203,    21,   164,   203,   149,   205,   331,   341,
     342,   181,   260,   269,   209,   164,   208,   210,   181,   368,
     209,   331,   203,   204,    41,   178,   181,   184,   350,   389,
     391,   393,   387,   164,   383,   395,   397,   399,   401,   403,
     405,   407,   409,    14,    15,    16,    17,    18,    26,    38,
      43,    46,    49,    53,    63,    75,    99,   112,   113,   145,
     146,   147,   148,   149,   151,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   165,   166,   187,   188,
     189,   191,   194,   195,   204,   206,   207,   220,   222,   267,
     271,   275,   280,   291,   298,   301,   304,   308,   309,   312,
     313,   315,   320,   323,   330,   377,   417,   426,   430,   432,
     434,    32,   184,    32,   184,   202,   211,   203,   323,   211,
     386,   411,   210,   345,   164,   131,   204,    48,   146,   164,
     348,   376,   205,   208,   411,     1,     8,     9,    10,    12,
      25,    27,    28,    37,    40,    42,    50,    52,    56,    57,
      63,   100,   127,   128,   129,   165,   208,   210,   233,   234,
     237,   240,   241,   243,   244,   245,   246,   247,   268,   270,
     274,   276,   281,   282,   283,   284,   285,   286,   287,   288,
     290,   310,   311,   323,   359,   179,   208,   275,   337,   352,
     203,   323,   164,   164,   386,   121,   131,   179,   349,   411,
     385,   411,   411,   411,   205,   381,   411,   259,   411,   259,
     411,   259,   164,   343,   344,   411,   346,   185,   277,   379,
     417,   204,   185,   204,   185,   185,   204,   185,   204,   185,
     323,   323,   204,   204,   204,   204,   204,   204,    12,   386,
      12,   386,   323,   428,   431,   185,   219,   323,   323,   323,
     275,   323,   323,   323,   207,   164,   204,   258,    20,    21,
     110,   111,   112,   113,   114,   117,   118,   119,   120,   122,
     123,   124,   125,   130,   132,   133,   138,   139,   140,   144,
     180,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   201,   202,   205,   204,   203,   203,   323,   348,   204,
      41,   121,   178,   179,   181,   184,   347,   341,   323,   376,
     121,   274,   310,   323,   267,    59,   323,   323,   164,   208,
     157,    56,   323,   267,   121,   274,   323,   207,   308,   308,
     308,   323,    36,   208,   208,   323,     9,   208,   208,   208,
     208,   208,    64,   289,   126,   208,   102,   103,   104,   105,
     106,   107,   108,   109,   115,   116,   121,   131,   134,   135,
     141,   142,   143,   179,   210,   323,   202,   210,   258,   338,
     205,    41,   208,   349,   274,   323,   186,   186,   208,   186,
     186,   186,   208,   186,   260,   186,   260,   186,   260,   181,
     186,   208,   186,   208,   278,   204,   275,   299,   323,   292,
     294,   323,   296,   323,   386,   323,   323,   323,   323,   323,
     323,   376,    51,   152,   164,   187,   204,   323,   418,   419,
     420,   427,   429,   376,   204,   419,   429,   136,   173,   208,
     210,   421,   272,   169,   170,   217,   223,   205,   146,   151,
     185,   275,   314,   202,   205,   302,   323,   154,   307,    18,
     152,   164,   377,    18,   152,   164,   377,   323,   323,   323,
     323,   323,   323,   164,   323,   152,   164,   323,   323,   323,
     323,   323,   323,   323,   323,   323,    21,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   123,   124,
     152,   164,   201,   202,   321,   323,   205,   302,   205,   323,
     164,   164,   411,   274,   323,   267,    19,   274,   323,   121,
     179,    13,   323,   267,   179,   181,   157,   274,   323,   207,
     267,   209,   267,   242,   352,   308,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   127,   128,   129,   323,
     323,   323,   323,   323,   323,   323,   127,   128,   129,   323,
     208,   257,     7,   331,   336,   164,   274,   323,   208,   390,
     392,   411,   394,   388,   384,   396,   186,   400,   186,   404,
     186,   411,   408,   343,   410,   411,   205,   302,   185,   323,
     411,   205,   386,   386,   205,   386,   205,   186,   205,   205,
     205,   205,   205,   205,    19,   308,   204,   131,   347,   205,
     136,   178,   208,   420,   175,   176,   203,   424,   178,   175,
     203,   208,   423,    19,   205,   420,   174,   210,   422,   323,
     428,   210,   386,   323,   167,   221,   204,   204,   316,   318,
     164,   418,   178,   205,   121,   131,   179,   184,   305,   306,
     259,   185,   204,   185,   204,   204,   204,   203,    18,   152,
     164,   377,   181,   152,   164,   323,   204,   152,   164,   323,
       1,   203,   178,   205,   205,    41,   347,   302,   267,   248,
     269,    11,    22,    23,   235,   236,   323,   308,   308,   308,
     308,   308,   308,   203,    55,    61,   334,    65,   335,   208,
     208,   398,   402,   406,   186,   205,   164,   205,   186,   186,
     186,   186,   307,   204,   302,   323,   323,   323,   419,   420,
     323,   152,   164,   418,   424,   203,   323,   203,   427,   302,
     419,   174,   177,   210,   425,   203,   186,   171,   167,   323,
     323,   386,   259,   203,   202,   323,   164,   164,   164,   164,
     178,   203,   260,   324,   323,   326,   323,   205,   302,   323,
     185,   204,   323,   204,   203,   323,   204,   203,   322,   164,
     323,   208,   323,   210,   267,   323,    11,   238,   208,    45,
     335,    44,   101,   332,   279,   186,   208,   300,   293,   295,
     297,   204,   205,   302,   208,   205,   420,   424,   204,   131,
     347,   208,   420,   203,   273,   205,   205,   186,   260,   205,
     418,   305,   203,   136,   267,   303,   386,   205,   411,   205,
     205,   205,   328,   323,   323,   205,   323,   267,   267,   239,
     208,    32,   333,   332,   334,   323,   164,   323,   323,   323,
     205,   323,   205,   323,   131,   347,   424,   323,   323,   323,
     323,   425,   323,   317,   186,   203,   121,   323,   186,   186,
     411,   205,   205,   205,   236,    25,   100,   240,   281,   282,
     283,   285,   323,   339,   333,   348,   205,   186,   205,   208,
     433,   323,   323,   205,   433,   314,   319,   205,   323,   325,
     327,   186,   121,   323,   121,   323,   264,   340,   323,    51,
     203,   131,   347,   210,   314,   329,   323,   323,   208,   264,
     205,   323,   203,   323,   323,   203,   267
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   212,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   214,   214,   214,   215,   215,
     216,   217,   217,   217,   217,   218,   219,   219,   219,   220,
     221,   221,   223,   222,   224,   225,   226,   226,   227,   227,
     227,   227,   228,   228,   229,   229,   230,   231,   231,   232,
     232,   233,   234,   234,   235,   235,   236,   236,   236,   237,
     237,   238,   239,   238,   240,   240,   240,   240,   240,   241,
     242,   241,   243,   244,   245,   246,   248,   247,   249,   249,
     249,   249,   249,   249,   250,   250,   251,   251,   251,   252,
     252,   252,   252,   252,   252,   252,   252,   253,   253,   254,
     254,   254,   255,   255,   256,   256,   256,   256,   256,   256,
     256,   257,   257,   258,   258,   259,   259,   259,   260,   260,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   262,   263,   263,   263,   264,
     266,   265,   267,   267,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   269,   269,   269,   270,   272,
     273,   271,   274,   274,   274,   274,   275,   275,   275,   276,
     276,   278,   279,   277,   277,   280,   280,   280,   280,   281,
     282,   283,   283,   283,   284,   284,   284,   285,   285,   286,
     286,   286,   287,   288,   288,   289,   289,   290,   292,   293,
     291,   294,   295,   291,   296,   297,   291,   299,   300,   298,
     301,   301,   301,   302,   302,   303,   303,   303,   304,   304,
     304,   305,   305,   305,   305,   306,   306,   307,   307,   308,
     308,   309,   309,   309,   309,   309,   309,   309,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   311,   311,   311,
     311,   311,   311,   312,   312,   313,   313,   314,   314,   315,
     316,   317,   315,   318,   319,   315,   320,   320,   321,   322,
     320,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     324,   325,   323,   323,   323,   323,   326,   327,   323,   323,
     323,   328,   329,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   331,   331,   332,   332,   332,   333,   333,
     334,   334,   334,   335,   335,   336,   337,   338,   337,   339,
     337,   340,   337,   337,   341,   341,   341,   342,   342,   343,
     343,   344,   344,   345,   346,   346,   347,   347,   348,   348,
     348,   348,   348,   348,   349,   349,   349,   350,   350,   351,
     351,   351,   351,   351,   352,   352,   352,   352,   352,   353,
     354,   353,   355,   355,   356,   356,   356,   357,   358,   357,
     359,   359,   359,   359,   361,   360,   362,   362,   363,   364,
     363,   365,   365,   365,   366,   367,   367,   368,   368,   369,
     369,   370,   371,   371,   372,   372,   372,   374,   375,   373,
     376,   376,   376,   376,   376,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   378,   378,   378,   378,   378,   378,   379,   380,
     380,   380,   381,   381,   383,   384,   382,   385,   385,   386,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   386,   386,   386,   386,   387,   388,   386,
     386,   389,   390,   386,   391,   392,   386,   393,   394,   386,
     386,   395,   396,   386,   397,   398,   386,   386,   399,   400,
     386,   401,   402,   386,   386,   403,   404,   386,   405,   406,
     386,   407,   408,   386,   409,   410,   386,   411,   411,   411,
     413,   414,   412,   416,   415,   417,   417,   417,   417,   418,
     418,   418,   418,   418,   418,   418,   418,   419,   419,   420,
     420,   421,   421,   422,   422,   423,   423,   424,   424,   424,
     425,   425,   425,   426,   426,   426,   426,   426,   426,   427,
     427,   427,   428,   428,   429,   429,   430,   430,   431,   431,
     432,   433,   433,   434,   434
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
       2,     2,     2,     2,     3,     3,     2,     2,     3,     4,
       3,     2,     2,     2,     2,     2,     3,     3,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     1,     1,     3,
       0,     4,     3,     7,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     2,     2,     2,     0,     2,     2,     3,     0,
       0,     7,     3,     2,     2,     2,     1,     3,     2,     2,
       3,     0,     0,     5,     1,     2,     4,     5,     2,     1,
       1,     1,     2,     3,     2,     2,     3,     2,     3,     2,
       2,     3,     4,     1,     1,     1,     0,     3,     0,     0,
       7,     0,     0,     7,     0,     0,     7,     0,     0,     6,
       5,     8,    10,     1,     3,     1,     2,     3,     1,     1,
       2,     2,     2,     2,     2,     1,     3,     0,     4,     1,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     4,
       4,     4,     4,     6,     8,     5,     6,     1,     4,     3,
       0,     0,     8,     0,     0,     9,     3,     4,     0,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     3,     4,     5,     4,     5,     3,     4,     1,     3,
       4,     3,     4,     2,     4,     4,     7,     8,     3,     5,
       0,     0,     8,     3,     3,     3,     0,     0,     8,     3,
       4,     0,     0,     9,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     4,     1,     4,     4,     4,
       4,     4,     1,     6,     7,     6,     6,     7,     7,     6,
       7,     6,     6,     0,     4,     0,     1,     1,     0,     1,
       0,     1,     1,     0,     1,     5,     0,     0,     4,     0,
       9,     0,    10,     5,     2,     3,     4,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     1,     1,     2,
       3,     5,     3,     3,     1,     1,     1,     0,     1,     1,
       4,     3,     3,     5,     4,     6,     5,     5,     4,     0,
       0,     4,     0,     1,     0,     1,     1,     6,     0,     6,
       0,     2,     3,     5,     0,     4,     2,     3,     4,     0,
       4,     0,     1,     1,     1,     7,     9,     0,     2,     0,
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
       3,     5,     5,     6,     6,     8,     8,     1,     3,     0,
       2,     2,     1,     2,     1,     2,     1,     2,     1,     1,
       2,     1,     1,     5,     4,     6,     7,     5,     7,     1,
       3,     3,     3,     1,     1,     3,     4,     4,     1,     3,
       3,     0,     3,    10,    10
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
  case 3: /* program: program module_declaration  */
                                   {
            if ( yyextra->das_has_type_declarations ) {
                das_yyerror(scanner,"module name has to be first declaration",tokAt(scanner,(yylsp[0])), CompilationError::syntax_error);
            }
        }
    break;

  case 4: /* program: program structure_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 5: /* program: program enum_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 6: /* program: program global_let  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 7: /* program: program global_function_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 11: /* program: program alias_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 12: /* program: program variant_alias_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 13: /* program: program bitfield_alias_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

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
        if ( auto seqt = yyextra->g_ReaderMacro->suffix(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, tokAt(scanner,(yylsp[0]))) ) {
            das_accept_sequence(scanner,seqt,strlen(seqt),(yylsp[0]).first_line+1); // todo: where from?
            yylloc.first_column = (yylsp[0]).first_column;
            yylloc.first_line = (yylsp[0]).first_line;
            yylloc.last_column = (yylsp[0]).last_column;
            yylloc.last_line = (yylsp[0]).last_line;
        }
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
        if ( !(yyvsp[-2].fa)->annotation || !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation || !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
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

  case 169: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 170: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 171: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 172: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 173: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 174: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 175: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 176: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 177: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 178: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 179: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 180: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 181: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 182: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 183: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 184: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 185: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 186: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 187: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 188: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 189: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 190: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 191: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 192: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 193: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 194: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 195: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 196: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 197: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 198: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 199: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 200: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 201: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 202: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 203: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 204: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 205: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 206: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 207: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 208: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 209: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 210: /* $@5: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 211: /* function_declaration: optional_public_or_private_function $@5 function_declaration_header expression_block  */
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

  case 212: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 213: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 214: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 215: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 216: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 217: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 218: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 219: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 220: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 221: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 222: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 226: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 227: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 228: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 233: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 234: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 235: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 236: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 237: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 238: /* expr_keyword: "keyword" expr expression_block  */
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

  case 239: /* $@6: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 240: /* $@7: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 241: /* expression_keyword: "keyword" '<' $@6 type_declaration_no_options '>' $@7 expr  */
                                                                                                                                               {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),*(yyvsp[-6].s));
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCall->arguments.push_back(ExpressionPtr(td));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 242: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 243: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 244: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 245: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 246: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 247: /* name_in_namespace: "name" "::" "name"  */
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

  case 248: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 249: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 250: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 251: /* $@8: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 252: /* $@9: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 253: /* new_type_declaration: '<' $@8 type_declaration '>' $@9  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 254: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 255: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 256: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 257: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 258: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 259: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 260: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 261: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 262: /* expression_return_no_pipe: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 263: /* expression_return_no_pipe: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 264: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 265: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 266: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 267: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 268: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 269: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 270: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 271: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 272: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 273: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 274: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 275: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 276: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 277: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 278: /* $@10: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 279: /* $@11: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 280: /* expr_cast: "cast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 281: /* $@12: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 282: /* $@13: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 283: /* expr_cast: "upcast" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 284: /* $@14: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 285: /* $@15: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 286: /* expr_cast: "reinterpret" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 287: /* $@16: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 288: /* $@17: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 289: /* expr_type_decl: "type" '<' $@16 type_declaration '>' $@17  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 290: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 291: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 292: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 293: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 294: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 295: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 296: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 297: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 298: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 299: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 300: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 301: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 302: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 303: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 304: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 305: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 306: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 307: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 308: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 309: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 310: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 311: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 312: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 313: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 314: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 315: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 316: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 317: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 318: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 319: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 320: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 325: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 326: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 335: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr_assign_pipe: expr '=' "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr_assign_pipe: expr "<-" "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 344: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 345: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 346: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 347: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 348: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 349: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 350: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 351: /* $@19: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 352: /* func_addr_expr: '@' '@' '<' $@18 type_declaration_no_options '>' $@19 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 353: /* $@20: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 354: /* $@21: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 355: /* func_addr_expr: '@' '@' '<' $@20 optional_function_argument_list optional_function_type '>' $@21 func_addr_name  */
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

  case 356: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 357: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 358: /* $@22: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 359: /* $@23: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 360: /* expr_field: expr '.' $@22 error $@23  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 361: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 362: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 363: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 364: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 366: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 367: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 368: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 369: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 370: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 371: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 397: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 400: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 401: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 402: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 403: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 404: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 405: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 406: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 407: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 408: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 409: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 410: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 411: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 412: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 413: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 414: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 415: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 416: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 417: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 418: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 419: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 420: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 421: /* $@25: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 422: /* expr: expr "is" "type" '<' $@24 type_declaration_no_options '>' $@25  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 423: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 424: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 425: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 426: /* $@26: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 427: /* $@27: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 428: /* expr: expr "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 429: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 430: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 431: /* $@28: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 432: /* $@29: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 433: /* expr: expr '?' "as" "type" '<' $@28 type_declaration '>' $@29  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 434: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 435: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 436: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 437: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 438: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 439: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 440: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 442: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 443: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 444: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 445: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 446: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 447: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 448: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 449: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 450: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 451: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 452: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 453: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 454: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 455: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 456: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 457: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 458: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 459: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 460: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 461: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 462: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 463: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 464: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 465: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 466: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 467: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 468: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 469: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 470: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 471: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 472: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 473: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 474: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 475: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 476: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 477: /* $@30: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 478: /* struct_variable_declaration_list: struct_variable_declaration_list $@30 structure_variable_declaration ';'  */
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

  case 479: /* $@31: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 480: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@31 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 481: /* $@32: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 482: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@32 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 483: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 484: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 485: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 486: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 487: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 488: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 489: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 490: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 491: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 492: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 493: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 494: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 495: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 496: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 497: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 498: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 499: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 500: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 501: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 502: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 503: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 504: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 505: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 506: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 507: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 508: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 509: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 510: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 511: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 512: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 513: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 514: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 515: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 516: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 517: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 518: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 519: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 520: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 521: /* global_variable_declaration_list: global_variable_declaration_list $@33 optional_field_annotation let_variable_declaration  */
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

  case 522: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 523: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 524: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 525: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 526: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 527: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 528: /* $@34: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 529: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@34 optional_field_annotation let_variable_declaration  */
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

  case 530: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 531: /* enum_list: enum_list ';'  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 532: /* enum_list: enum_list "name" ';'  */
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

  case 533: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 534: /* $@35: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 535: /* single_alias: "name" $@35 '=' type_declaration  */
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

  case 539: /* $@36: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 541: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 542: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 543: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 544: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 545: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 546: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 547: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 548: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 549: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 550: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 551: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 552: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 553: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 554: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 555: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 556: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 557: /* $@37: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 558: /* $@38: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 559: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@37 structure_name $@38 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        if ( (yyvsp[-4].pStructure) ) {
            ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
            if ( !yyextra->g_CommentReaders.empty() ) {
                auto tak = tokAt(scanner,(yylsp[-7]));
                for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
            }
        } else {
            deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
        }
    }
    break;

  case 560: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 561: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 562: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 563: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 564: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 565: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 566: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 567: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 568: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 569: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 570: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 571: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 572: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 573: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 574: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 575: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 576: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 577: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 578: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 579: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 580: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 581: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 582: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 583: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 584: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 585: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 586: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 587: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 588: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 589: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 590: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 591: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 592: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 593: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 594: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 595: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 596: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 597: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 598: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 599: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 600: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 601: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 602: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 603: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 604: /* $@39: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 605: /* $@40: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 606: /* bitfield_type_declaration: "bitfield" '<' $@39 bitfield_bits '>' $@40  */
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

  case 607: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 608: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 609: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 610: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 611: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 612: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 613: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
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

  case 614: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 615: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 616: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 617: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 618: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 619: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 620: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 621: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 622: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 623: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 624: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 625: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 626: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 627: /* $@41: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 628: /* $@42: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 629: /* type_declaration_no_options: "smart_ptr" '<' $@41 type_declaration '>' $@42  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 630: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 631: /* $@43: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 632: /* $@44: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 633: /* type_declaration_no_options: "array" '<' $@43 type_declaration '>' $@44  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 634: /* $@45: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 635: /* $@46: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 636: /* type_declaration_no_options: "table" '<' $@45 table_type_pair '>' $@46  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 637: /* $@47: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 638: /* $@48: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 639: /* type_declaration_no_options: "iterator" '<' $@47 type_declaration '>' $@48  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 640: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 641: /* $@49: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 642: /* $@50: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 643: /* type_declaration_no_options: "block" '<' $@49 type_declaration '>' $@50  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 644: /* $@51: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 645: /* $@52: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 646: /* type_declaration_no_options: "block" '<' $@51 optional_function_argument_list optional_function_type '>' $@52  */
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

  case 647: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 648: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 649: /* $@54: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 650: /* type_declaration_no_options: "function" '<' $@53 type_declaration '>' $@54  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 651: /* $@55: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 652: /* $@56: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 653: /* type_declaration_no_options: "function" '<' $@55 optional_function_argument_list optional_function_type '>' $@56  */
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

  case 654: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 655: /* $@57: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 656: /* $@58: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 657: /* type_declaration_no_options: "lambda" '<' $@57 type_declaration '>' $@58  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 658: /* $@59: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 659: /* $@60: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 660: /* type_declaration_no_options: "lambda" '<' $@59 optional_function_argument_list optional_function_type '>' $@60  */
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

  case 661: /* $@61: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 662: /* $@62: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 663: /* type_declaration_no_options: "tuple" '<' $@61 tuple_type_list '>' $@62  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 664: /* $@63: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 665: /* $@64: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 666: /* type_declaration_no_options: "variant" '<' $@63 variant_type_list '>' $@64  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 667: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 668: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 669: /* type_declaration: type_declaration '|' '#'  */
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

  case 670: /* $@65: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 671: /* $@66: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 672: /* variant_alias_declaration: "variant" $@65 "name" $@66 '{' variant_type_list ';' '}'  */
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

  case 673: /* $@67: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 674: /* bitfield_alias_declaration: "bitfield" $@67 "name" '{' bitfield_bits ';' '}'  */
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

  case 675: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 676: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 677: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 678: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 679: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 680: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 681: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 682: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 683: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 684: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 685: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 686: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 687: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 688: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 689: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 690: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 703: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 704: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 705: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 706: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 707: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 708: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 709: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 710: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 711: /* make_tuple: make_tuple ',' expr  */
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

  case 712: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 713: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 714: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 715: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 716: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
       ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
       (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
       (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 717: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
       ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
       (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 718: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 719: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 720: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
                                                                          {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = mkt;
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-2]));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = ttm;
    }
    break;

  case 721: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 722: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 723: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 724: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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
    if ( !yyextra->das_suppress_errors ) {
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


