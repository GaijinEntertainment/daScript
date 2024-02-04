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
  YYSYMBOL_top_level_reader_macro = 214,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 215, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 216,              /* module_name  */
  YYSYMBOL_module_declaration = 217,       /* module_declaration  */
  YYSYMBOL_character_sequence = 218,       /* character_sequence  */
  YYSYMBOL_string_constant = 219,          /* string_constant  */
  YYSYMBOL_string_builder_body = 220,      /* string_builder_body  */
  YYSYMBOL_string_builder = 221,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 222, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 223,              /* expr_reader  */
  YYSYMBOL_224_1 = 224,                    /* $@1  */
  YYSYMBOL_options_declaration = 225,      /* options_declaration  */
  YYSYMBOL_require_declaration = 226,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 227,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 228,      /* require_module_name  */
  YYSYMBOL_require_module = 229,           /* require_module  */
  YYSYMBOL_is_public_module = 230,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 231,       /* expect_declaration  */
  YYSYMBOL_expect_list = 232,              /* expect_list  */
  YYSYMBOL_expect_error = 233,             /* expect_error  */
  YYSYMBOL_expression_label = 234,         /* expression_label  */
  YYSYMBOL_expression_goto = 235,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 236,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 237,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 238,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 239, /* expression_else_one_liner  */
  YYSYMBOL_240_2 = 240,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 241,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 242,  /* expression_if_then_else  */
  YYSYMBOL_243_3 = 243,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 244,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 245,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 246,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 247,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 248,    /* expression_with_alias  */
  YYSYMBOL_249_4 = 249,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 250, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 251, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 252, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 253,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 254, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 255, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 256, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 257,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 258,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 259, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 260, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 261,   /* optional_function_type  */
  YYSYMBOL_function_name = 262,            /* function_name  */
  YYSYMBOL_global_function_declaration = 263, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 264, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 265, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 266,     /* function_declaration  */
  YYSYMBOL_267_5 = 267,                    /* $@5  */
  YYSYMBOL_expression_block = 268,         /* expression_block  */
  YYSYMBOL_expression_any = 269,           /* expression_any  */
  YYSYMBOL_expressions = 270,              /* expressions  */
  YYSYMBOL_expr_keyword = 271,             /* expr_keyword  */
  YYSYMBOL_expression_keyword = 272,       /* expression_keyword  */
  YYSYMBOL_273_6 = 273,                    /* $@6  */
  YYSYMBOL_274_7 = 274,                    /* $@7  */
  YYSYMBOL_expr_pipe = 275,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 276,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 277,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 278,     /* new_type_declaration  */
  YYSYMBOL_279_8 = 279,                    /* $@8  */
  YYSYMBOL_280_9 = 280,                    /* $@9  */
  YYSYMBOL_expr_new = 281,                 /* expr_new  */
  YYSYMBOL_expression_break = 282,         /* expression_break  */
  YYSYMBOL_expression_continue = 283,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 284, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 285,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 286, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 287,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 288,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 289,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 290,        /* optional_in_scope  */
  YYSYMBOL_expression_let = 291,           /* expression_let  */
  YYSYMBOL_expr_cast = 292,                /* expr_cast  */
  YYSYMBOL_293_10 = 293,                   /* $@10  */
  YYSYMBOL_294_11 = 294,                   /* $@11  */
  YYSYMBOL_295_12 = 295,                   /* $@12  */
  YYSYMBOL_296_13 = 296,                   /* $@13  */
  YYSYMBOL_297_14 = 297,                   /* $@14  */
  YYSYMBOL_298_15 = 298,                   /* $@15  */
  YYSYMBOL_expr_type_decl = 299,           /* expr_type_decl  */
  YYSYMBOL_300_16 = 300,                   /* $@16  */
  YYSYMBOL_301_17 = 301,                   /* $@17  */
  YYSYMBOL_expr_type_info = 302,           /* expr_type_info  */
  YYSYMBOL_expr_list = 303,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 304,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 305,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 306,            /* capture_entry  */
  YYSYMBOL_capture_list = 307,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 308,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 309,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 310,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 311,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 312,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 313,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 314,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 315,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 316,           /* func_addr_expr  */
  YYSYMBOL_317_18 = 317,                   /* $@18  */
  YYSYMBOL_318_19 = 318,                   /* $@19  */
  YYSYMBOL_319_20 = 319,                   /* $@20  */
  YYSYMBOL_320_21 = 320,                   /* $@21  */
  YYSYMBOL_expr_field = 321,               /* expr_field  */
  YYSYMBOL_322_22 = 322,                   /* $@22  */
  YYSYMBOL_323_23 = 323,                   /* $@23  */
  YYSYMBOL_expr = 324,                     /* expr  */
  YYSYMBOL_325_24 = 325,                   /* $@24  */
  YYSYMBOL_326_25 = 326,                   /* $@25  */
  YYSYMBOL_327_26 = 327,                   /* $@26  */
  YYSYMBOL_328_27 = 328,                   /* $@27  */
  YYSYMBOL_329_28 = 329,                   /* $@28  */
  YYSYMBOL_330_29 = 330,                   /* $@29  */
  YYSYMBOL_expr_mtag = 331,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 332, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 333,        /* optional_override  */
  YYSYMBOL_optional_constant = 334,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 335, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 336, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 337, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 338, /* struct_variable_declaration_list  */
  YYSYMBOL_339_30 = 339,                   /* $@30  */
  YYSYMBOL_340_31 = 340,                   /* $@31  */
  YYSYMBOL_341_32 = 341,                   /* $@32  */
  YYSYMBOL_function_argument_declaration = 342, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 343,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 344,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 345,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 346,             /* variant_type  */
  YYSYMBOL_variant_type_list = 347,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 348,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 349,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 350,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 351,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 352, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 353, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 354, /* global_variable_declaration_list  */
  YYSYMBOL_355_33 = 355,                   /* $@33  */
  YYSYMBOL_optional_shared = 356,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 357, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 358,               /* global_let  */
  YYSYMBOL_359_34 = 359,                   /* $@34  */
  YYSYMBOL_enum_list = 360,                /* enum_list  */
  YYSYMBOL_single_alias = 361,             /* single_alias  */
  YYSYMBOL_362_35 = 362,                   /* $@35  */
  YYSYMBOL_alias_list = 363,               /* alias_list  */
  YYSYMBOL_alias_declaration = 364,        /* alias_declaration  */
  YYSYMBOL_365_36 = 365,                   /* $@36  */
  YYSYMBOL_optional_public_or_private_enum = 366, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 367,                /* enum_name  */
  YYSYMBOL_enum_declaration = 368,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 369, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 370,          /* optional_sealed  */
  YYSYMBOL_structure_name = 371,           /* structure_name  */
  YYSYMBOL_class_or_struct = 372,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 373, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 374,    /* structure_declaration  */
  YYSYMBOL_375_37 = 375,                   /* $@37  */
  YYSYMBOL_376_38 = 376,                   /* $@38  */
  YYSYMBOL_variable_name_with_pos_list = 377, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 378,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 379, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 380, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 381,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 382,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 383, /* bitfield_type_declaration  */
  YYSYMBOL_384_39 = 384,                   /* $@39  */
  YYSYMBOL_385_40 = 385,                   /* $@40  */
  YYSYMBOL_table_type_pair = 386,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 387, /* type_declaration_no_options  */
  YYSYMBOL_388_41 = 388,                   /* $@41  */
  YYSYMBOL_389_42 = 389,                   /* $@42  */
  YYSYMBOL_390_43 = 390,                   /* $@43  */
  YYSYMBOL_391_44 = 391,                   /* $@44  */
  YYSYMBOL_392_45 = 392,                   /* $@45  */
  YYSYMBOL_393_46 = 393,                   /* $@46  */
  YYSYMBOL_394_47 = 394,                   /* $@47  */
  YYSYMBOL_395_48 = 395,                   /* $@48  */
  YYSYMBOL_396_49 = 396,                   /* $@49  */
  YYSYMBOL_397_50 = 397,                   /* $@50  */
  YYSYMBOL_398_51 = 398,                   /* $@51  */
  YYSYMBOL_399_52 = 399,                   /* $@52  */
  YYSYMBOL_400_53 = 400,                   /* $@53  */
  YYSYMBOL_401_54 = 401,                   /* $@54  */
  YYSYMBOL_402_55 = 402,                   /* $@55  */
  YYSYMBOL_403_56 = 403,                   /* $@56  */
  YYSYMBOL_404_57 = 404,                   /* $@57  */
  YYSYMBOL_405_58 = 405,                   /* $@58  */
  YYSYMBOL_406_59 = 406,                   /* $@59  */
  YYSYMBOL_407_60 = 407,                   /* $@60  */
  YYSYMBOL_408_61 = 408,                   /* $@61  */
  YYSYMBOL_409_62 = 409,                   /* $@62  */
  YYSYMBOL_410_63 = 410,                   /* $@63  */
  YYSYMBOL_411_64 = 411,                   /* $@64  */
  YYSYMBOL_type_declaration = 412,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 413, /* variant_alias_declaration  */
  YYSYMBOL_414_65 = 414,                   /* $@65  */
  YYSYMBOL_415_66 = 415,                   /* $@66  */
  YYSYMBOL_bitfield_alias_declaration = 416, /* bitfield_alias_declaration  */
  YYSYMBOL_417_67 = 417,                   /* $@67  */
  YYSYMBOL_make_decl = 418,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 419,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 420,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 421,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 422, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 423, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 424, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 425, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 426, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_make_struct_decl = 427,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 428,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 429,           /* make_map_tuple  */
  YYSYMBOL_make_dim = 430,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 431,            /* make_dim_decl  */
  YYSYMBOL_make_table = 432,               /* make_table  */
  YYSYMBOL_make_table_decl = 433,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 434, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 435       /* array_comprehension  */
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
#define YYLAST   11068

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  212
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  224
/* YYNRULES -- Number of rules.  */
#define YYNRULES  726
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1270

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
     514,   515,   516,   517,   518,   519,   523,   529,   530,   531,
     535,   536,   540,   558,   559,   560,   561,   565,   569,   574,
     583,   591,   607,   612,   620,   620,   658,   679,   683,   684,
     688,   691,   695,   701,   710,   713,   719,   720,   724,   728,
     729,   733,   736,   742,   748,   751,   757,   758,   762,   763,
     764,   773,   774,   778,   779,   779,   785,   786,   787,   788,
     789,   793,   799,   799,   805,   811,   819,   829,   838,   838,
     845,   846,   847,   848,   849,   850,   854,   859,   867,   868,
     869,   873,   874,   875,   876,   877,   878,   879,   880,   886,
     889,   895,   896,   897,   901,   909,   922,   925,   933,   944,
     955,   966,   969,   976,   980,   987,   988,   992,   993,   994,
     998,  1001,  1008,  1012,  1013,  1014,  1015,  1016,  1017,  1018,
    1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,
    1029,  1030,  1031,  1032,  1033,  1034,  1035,  1036,  1037,  1038,
    1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,
    1049,  1050,  1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,
    1059,  1060,  1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,
    1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,
    1079,  1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,
    1089,  1090,  1091,  1092,  1093,  1094,  1095,  1100,  1122,  1123,
    1124,  1128,  1134,  1134,  1151,  1155,  1166,  1167,  1168,  1169,
    1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,
    1180,  1181,  1182,  1183,  1184,  1185,  1186,  1190,  1195,  1201,
    1207,  1218,  1218,  1218,  1229,  1263,  1266,  1269,  1275,  1276,
    1287,  1291,  1294,  1302,  1302,  1302,  1305,  1311,  1314,  1317,
    1321,  1327,  1331,  1335,  1338,  1341,  1349,  1352,  1355,  1363,
    1366,  1374,  1377,  1380,  1388,  1394,  1395,  1399,  1400,  1404,
    1410,  1410,  1410,  1413,  1413,  1413,  1418,  1418,  1418,  1426,
    1426,  1426,  1432,  1442,  1453,  1468,  1471,  1477,  1478,  1485,
    1496,  1497,  1498,  1502,  1503,  1504,  1505,  1509,  1514,  1522,
    1523,  1527,  1532,  1539,  1540,  1541,  1542,  1543,  1544,  1545,
    1549,  1550,  1551,  1552,  1553,  1554,  1555,  1556,  1557,  1558,
    1559,  1560,  1561,  1562,  1563,  1564,  1565,  1566,  1567,  1571,
    1572,  1573,  1574,  1575,  1576,  1580,  1587,  1599,  1604,  1614,
    1618,  1625,  1628,  1628,  1628,  1633,  1633,  1633,  1646,  1650,
    1654,  1654,  1654,  1661,  1662,  1663,  1664,  1665,  1666,  1667,
    1668,  1669,  1670,  1671,  1672,  1673,  1674,  1675,  1676,  1677,
    1678,  1679,  1680,  1681,  1682,  1683,  1684,  1685,  1686,  1687,
    1688,  1689,  1690,  1691,  1692,  1693,  1694,  1695,  1696,  1702,
    1703,  1704,  1705,  1706,  1707,  1708,  1709,  1710,  1711,  1712,
    1713,  1714,  1718,  1722,  1725,  1728,  1729,  1730,  1731,  1734,
    1737,  1738,  1741,  1741,  1741,  1744,  1749,  1753,  1757,  1757,
    1757,  1762,  1765,  1769,  1769,  1769,  1774,  1777,  1778,  1779,
    1780,  1781,  1782,  1783,  1784,  1785,  1786,  1787,  1792,  1796,
    1797,  1798,  1799,  1800,  1801,  1802,  1806,  1810,  1814,  1818,
    1822,  1826,  1830,  1834,  1838,  1845,  1846,  1850,  1851,  1852,
    1856,  1857,  1861,  1862,  1863,  1867,  1868,  1872,  1883,  1886,
    1886,  1905,  1904,  1919,  1918,  1931,  1940,  1946,  1951,  1961,
    1962,  1966,  1969,  1978,  1979,  1983,  1992,  1993,  1998,  1999,
    2003,  2009,  2015,  2018,  2022,  2028,  2037,  2038,  2039,  2043,
    2044,  2048,  2055,  2060,  2069,  2075,  2086,  2089,  2094,  2099,
    2107,  2118,  2121,  2121,  2141,  2142,  2146,  2147,  2148,  2152,
    2155,  2155,  2174,  2177,  2180,  2189,  2202,  2202,  2223,  2224,
    2228,  2229,  2229,  2233,  2234,  2235,  2239,  2249,  2256,  2266,
    2267,  2271,  2272,  2276,  2282,  2283,  2287,  2288,  2289,  2293,
    2298,  2293,  2312,  2319,  2324,  2333,  2339,  2350,  2351,  2352,
    2353,  2354,  2355,  2356,  2357,  2358,  2359,  2360,  2361,  2362,
    2363,  2364,  2365,  2366,  2367,  2368,  2369,  2370,  2371,  2372,
    2373,  2374,  2375,  2376,  2380,  2381,  2382,  2383,  2384,  2385,
    2389,  2400,  2404,  2411,  2423,  2430,  2439,  2439,  2439,  2452,
    2456,  2463,  2464,  2465,  2466,  2467,  2481,  2487,  2491,  2495,
    2500,  2505,  2510,  2515,  2519,  2523,  2528,  2532,  2536,  2541,
    2541,  2541,  2547,  2554,  2554,  2554,  2559,  2559,  2559,  2565,
    2565,  2565,  2570,  2574,  2574,  2574,  2579,  2579,  2579,  2588,
    2592,  2592,  2592,  2597,  2597,  2597,  2606,  2610,  2610,  2610,
    2615,  2615,  2615,  2624,  2624,  2624,  2630,  2630,  2630,  2639,
    2642,  2653,  2669,  2669,  2669,  2693,  2693,  2713,  2714,  2715,
    2716,  2720,  2727,  2734,  2740,  2746,  2753,  2760,  2766,  2776,
    2781,  2788,  2789,  2794,  2795,  2799,  2800,  2804,  2805,  2809,
    2810,  2811,  2815,  2816,  2817,  2822,  2828,  2835,  2843,  2850,
    2858,  2870,  2873,  2879,  2893,  2899,  2905,  2910,  2917,  2922,
    2932,  2937,  2944,  2956,  2957,  2961,  2964
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
  "'}'", "'#'", "$accept", "program", "top_level_reader_macro",
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

#define YYPACT_NINF (-1066)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-661)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1066,    21, -1066, -1066,    45,   -64,   138,    22, -1066,    -1,
   -1066, -1066,   107,   127, -1066, -1066, -1066,    32, -1066, -1066,
   -1066,   332, -1066,   161, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066, -1066, -1066,   168, -1066,   193,   172,   206, -1066, -1066,
   -1066,   138, -1066,     7, -1066,   255,   255, -1066, -1066,   161,
     265,   305,   315,   306, -1066, -1066, -1066,   127,   127,   127,
     281, -1066,   402,   178, -1066, -1066, -1066, -1066,    42,    43,
     117, -1066,   272,    20,    45,   348,   -64,   310,   343, -1066,
     226,   226, -1066, -1066,   353,  -103,   363,   277,   359, -1066,
   -1066,   370,   376, -1066, -1066,   -73,    45,   127,   127,   127,
     127, -1066, -1066, -1066, -1066, -1066, -1066, -1066,   391, -1066,
   -1066, -1066, -1066, -1066,   365, -1066, -1066, -1066, -1066, -1066,
     326,    99, -1066, -1066, -1066, -1066,   489, -1066, -1066,   377,
   -1066, -1066,   367, -1066, -1066, -1066, -1066,   412,   374, -1066,
   -1066,   419, -1066,   165, -1066,   302,   449,   402, 10904, -1066,
     135,   487, -1066,   437, -1066, -1066,   380, -1066, -1066, -1066,
   -1066, -1066, -1066, -1066,   174, -1066, 10716, -1066, -1066,   384,
     430, -1066, -1066,  9525, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,   562,
     568, -1066,   401,   398,   382, -1066, -1066,   444, -1066,   404,
      45,   -54, -1066, -1066, -1066,    99, -1066,   426,   431,   432,
     434, -1066, -1066, -1066,   417, -1066, -1066, -1066, -1066, -1066,
     447, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066, -1066,   448, -1066, -1066, -1066,   450,   454, -1066, -1066,
   -1066, -1066,   455,   458,   442, -1066, -1066, -1066, -1066, -1066,
     870,   462,   -42,   470, -1066,   446,   488,   492, -1066, -1066,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,   493,   459,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066, -1066,   637, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066, -1066, -1066, -1066, -1066,   497,   460, -1066, -1066,   -75,
     484, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,   457,
     -82,   486,   461, -1066,   437,   200,   464,   623,   249, -1066,
   -1066, -1066, -1066, -1066, -1066,   505, -1066,   278,   285,   286,
   -1066, -1066,  6727, -1066, -1066, -1066,     9, -1066, -1066, -1066,
       8,  3809, -1066,  6980, -1066, -1066, 10716,   -39, -1066, -1066,
   -1066, -1066,   507,   541, -1066,   469, -1066,    -9, -1066,   -35,
   10716, -1066,  1706, -1066,    51, -1066, -1066,   107, -1066, -1066,
     -54,   471,  6727,   511,   513, 10716, -1066,   -41, 10716, 10716,
   10716, 10716,   475,   412, 10716,   401, 10716,   401, 10716,   401,
   10815,   430, -1066, -1066,   164,   474,   498, -1066,   480,   500,
     501,   491,   502,   496, -1066,   503,  6727,  6727,   504,   509,
     516,   519,   524,   530, -1066, 10358, 10457,  6727, -1066, -1066,
   -1066, -1066, -1066, -1066, -1066,   512, -1066,  6727,  6727,  6727,
    6727,  6727,  6727, -1066,   494, -1066, -1066, -1066, -1066,    -5,
   -1066, -1066, -1066, -1066,   527, -1066, -1066, -1066, -1066, -1066,
   -1066,  6969, -1066,   531, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066, -1066, -1066,   534, -1066, -1066,  9051, -1066,   870,   462,
   -1066, -1066, -1066, -1066,  6727,    16,   535,   649, -1066,   -63,
   -1066,    25,   462, -1066,  6727, -1066, -1066,    16,  2647, -1066,
     398,  3968,  6727,   532, -1066,   499,   581,  4127,   300,  2806,
     360,   360,   360,  4286, -1066,   704,   537,   539,  6727,   733,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,   542,
     543,   545,   548, -1066,   553, -1066, -1066,   679, -1066,   -77,
   -1066,  1279,   -81,  6727, -1066, -1066,    23, -1066, -1066,  7075,
   -1066,   721,   982, -1066, -1066, -1066,  3150,   -80,   577,   142,
     -17,    94, -1066,   246,    97,   484,   324,   484,   337,   484,
     -69, -1066,   284,   462,   294, -1066,   560, -1066, -1066,   107,
   -1066,  6727, -1066, -1066,  6727, -1066,  6727, 10716,   943,   943,
    6727,  6727,  6727,  6727,  6727,  6727,    16,  2101,    16,  2283,
    9560, -1066,    38, -1066,   369,   943,   943,   -51,   943,   943,
    7112,   280, -1066,  3627,   611, 10491, 10586,  6727,  6727, -1066,
   -1066,  6727,  6727,  6727,  6727,   602,  6727,    12,  6727,  6727,
    6727,  6727,  6727,  6727,  6727,  6727,  6727,  4468,  6727,  6727,
    6727,  6727,  6727,  6727,  6727,  6727,  6727,  6727,   -53,  6727,
   -1066,  4627, -1066, -1066,  7218, -1066,  6727,   603, -1066,   604,
   -1066, 10716, -1066,  3150, -1066,   592,    13,  3150, -1066,   643,
    9417,   760,  6727,  9871,   592,   605, -1066,   607,   628,  9871,
   -1066,  3150, -1066,  9417,   579, -1066, -1066, -1066,   592,   580,
   -1066, -1066,   592, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
     -54,   360, -1066,  6727,  6727,  6727,  6727,  6727,  6727,  6727,
    6727,  6727,  6727,  3309,  6727,  6727,  6727,  6727,  6727,  6727,
    3468, -1066,   698,   127, -1066,   783,   437, -1066,   627, -1066,
    3150, -1066,  1421, -1066, -1066, 10716, -1066, -1066, -1066,   631,
   -1066,   606, -1066,   610, -1066,   612, 10716, -1066, 10815, -1066,
     430, 10716,  4809,  4991, 10716,  7253, 10716, 10716,  7359, 10716,
    7394,  1160,  7500,  7535,  7641,  7676,  7782,  7817,    24,   360,
     593,   -37,  2465,  5173,  9655,   621,   -18,   166,   622,    67,
      29,  5355,   -18,    41,  6727, -1066,  6727,   594, -1066, 10716,
   -1066,  6727,   350, -1066,   598,   599,   287, -1066, -1066,   126,
   -1066,   194,  9871,   -40,   401,   620,   609, -1066, -1066,   629,
     615, -1066, -1066,  2397,  2397,   325,   325, 10221, 10221,   635,
     525,   636, -1066,  9086,   -67,   -67,  2397,  2397, 10021,   175,
    9992,  9906, 10617,  9750, 10107,  1457, 10135,   325,   325,   569,
     569,   525,   525,   525,   334,  6727,   639, -1066,   335,  6727,
     805,  9181, -1066,   196, -1066,  7923, -1066,   766,   -62, -1066,
    9417, -1066,  6727, -1066,  9417,  6727,  6727,   398,  9871, -1066,
   -1066, -1066, -1066, -1066,  9417, -1066, -1066, -1066,   473,  6727,
   -1066, -1066,  9871,  9871,  9871,  9871,  9871,  9871,  9871,  9871,
    9871,  9871,   360,   360,   360,  9871,  9871,  9871,  9871,  9871,
    9871,  9871,   360,   360,   360,  9871, -1066,   207,   279,   759,
     617, -1066, -1066,  6835, -1066, -1066, -1066,   462, -1066, -1066,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066,   462, -1066, -1066,
   -1066,   338, -1066,   202,   662,  7958,   346, -1066,  1629,  1765,
   -1066,  2213, -1066,   611, -1066, -1066, -1066, -1066, -1066,   641,
    6727, -1066,  6727,  6727,  6727,    -7,  6727,   336,   126,   166,
   -1066, -1066,   624, -1066,  6727, -1066,   630,  6727, -1066,  6727,
     126,   100, -1066,   638, -1066,  9871, -1066, -1066,  2577,  9785,
    6727,  6727, 10716,   401,   -26,   222,  5537, -1066,   668,   682,
     683,   684, -1066,   243,   484, -1066,  6727, -1066,  6727,  5704,
    6727, -1066,   664,   646, -1066, -1066,  6727,   647, -1066,  9276,
    6727,   648, -1066,  9311, -1066, -1066,  6727, -1066, -1066,   689,
    6727,   104, -1066,  6727,  1916,   398, -1066, -1066,  6727, -1066,
     440, -1066, -1066, -1066, -1066, -1066, -1066,   650, -1066, -1066,
      44, -1066,    57, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
     295, -1066, -1066, -1066, -1066, -1066,   651,  5886,   106,  8064,
    9871,  9871,   -18,   166,  9871,   655,     2,   621, -1066, -1066,
    9871, -1066,   622,   118,   -18, -1066, -1066,   657, -1066, -1066,
   -1066, -1066,  8099,  8205,  3626,   484,   656,   126,  9871, -1066,
   -1066, -1066, -1066,   -40,   659,   -85, 10716,  8240, 10716,  8346,
   -1066,   215,  8381, -1066,  6727,   836,  6727, -1066,  8487,  6727,
   -1066, -1066, -1066,  9871,   398,  9871, -1066, -1066,   592, -1066,
     658, -1066,   822,    57, -1066, -1066,   279, -1066,  6727,   699,
   -1066,  6727,  6727,  6727,  6068, -1066,   217,  6727,    15,   166,
   -1066,  6727,  6727,  6727,  6727,   100, -1066,  6727, -1066, -1066,
   -1066,   678, -1066,   250, -1066, -1066,  6250, -1066, -1066,  3739,
   -1066,   347, -1066, -1066, -1066, 10716,  8522,  8628, -1066,  8663,
   -1066,   473,  2991, -1066, -1066, -1066,   822,    16,  8769,   681,
     325,   325,   325, -1066,  8804, -1066,  1131,  6727,  6727, -1066,
    8910,  9871,  9871,  1131, -1066,   325,   184, -1066,   663,  6727,
    9906, -1066, -1066,   349, -1066, -1066, -1066, -1066,  6409,  6568,
   -1066, -1066, -1066, -1066, -1066,  9871, 10904, -1066, -1066, -1066,
    6727, -1066,   818,   667,  9871,  9871,    35,   661, -1066,   184,
   -1066,   836, -1066, -1066, -1066,  6727,  9871,  6727,  9871,   665,
   10904,  8945,  6727,   669,  6727,  6727,   671, -1066, -1066,  9871,
    9871, -1066,   398, -1066,  9871, -1066,  9871,  9871, -1066, -1066
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   115,     1,   275,     0,     0,     0,   541,   276,     0,
     675,   672,     0,     0,    15,    14,     3,     0,    10,     9,
       8,     0,     7,   524,     6,    11,     5,     4,    12,    13,
      89,    90,    88,    97,    99,    36,    51,    48,    49,    38,
      39,     0,    40,    46,    37,     0,     0,    21,    20,   524,
       0,     0,     0,   248,    34,   102,   103,     0,     0,     0,
     104,   106,   113,     0,   101,    16,   555,   554,   208,   543,
     556,   525,   526,     0,     0,     0,     0,    41,     0,    47,
       0,     0,    44,   536,     0,     0,     0,    17,     0,   673,
     250,     0,     0,   112,   107,     0,     0,     0,     0,     0,
       0,   116,   210,   209,   212,   207,   545,   544,     0,   558,
     557,   559,   528,   527,   530,    95,    96,    93,    94,    92,
       0,     0,    91,   100,    52,    50,    46,    43,    42,     0,
     538,   540,     0,   542,    18,    19,    22,     0,     0,   249,
      32,    35,   111,     0,   108,   109,   110,   114,     0,   546,
       0,   551,   521,   465,    23,    24,     0,    84,    85,    82,
      83,    81,    80,    86,     0,    45,     0,   539,   604,     0,
       0,    33,   105,     0,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,     0,
       0,   122,   117,     0,     0,   532,   552,     0,   560,   522,
       0,     0,    25,    26,    27,     0,    98,     0,     0,     0,
       0,   567,   587,   568,   601,   569,   573,   574,   575,   576,
     593,   580,   581,   582,   583,   584,   585,   586,   588,   589,
     590,   591,   642,   572,   579,   592,   649,   656,   570,   577,
     571,   578,     0,     0,     0,   600,   611,   614,   612,   613,
     669,   537,     0,     0,   496,     0,   176,   177,   174,   125,
     126,   128,   127,   129,   130,   131,   132,   158,   159,   156,
     157,   149,   160,   161,   150,   147,   148,   175,   169,     0,
     173,   162,   163,   164,   165,   136,   137,   138,   133,   134,
     135,   146,     0,   152,   153,   151,   144,   145,   140,   139,
     141,   142,   143,   124,   123,   168,     0,   154,   155,   465,
     120,   237,   213,   594,   597,   595,   598,   596,   599,     0,
       0,   549,     0,   529,   465,     0,     0,   511,   509,   531,
      87,   633,   636,   639,   629,     0,   606,   643,   650,   657,
     663,   666,     0,   619,   624,   618,     0,   632,   628,   621,
       0,     0,   623,     0,   605,   676,     0,     0,   178,   179,
     172,   167,   180,   170,   166,     0,   118,     0,   489,     0,
       0,   211,     0,   532,     0,   533,   547,     0,   553,   478,
       0,     0,     0,     0,     0,     0,   510,     0,     0,     0,
       0,     0,     0,     0,     0,   117,     0,   117,     0,   117,
       0,     0,   369,   370,     0,     0,     0,   363,     0,     0,
       0,     0,     0,     0,   593,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   454,     0,     0,     0,   313,   315,
     314,   316,   317,   318,   319,     0,    28,     0,     0,     0,
       0,     0,     0,   300,   301,   367,   366,   311,   448,   364,
     440,   439,   438,   437,   115,   443,   365,   442,   441,   410,
     371,     0,   372,     0,   368,   677,   678,   679,   680,   626,
     627,   620,   622,     0,   625,   616,     0,   671,   670,   495,
     674,   497,   181,   171,     0,     0,     0,   562,   486,   500,
     119,   465,   121,   239,     0,    61,    62,     0,   263,   261,
       0,     0,     0,     0,   262,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   216,   214,     0,     0,     0,     0,
     232,   227,   224,   223,   225,   226,   238,   218,   217,     0,
      69,    70,    67,   230,    68,   231,   233,   278,   222,     0,
     219,   320,     0,     0,   534,   550,   479,   523,   466,     0,
     513,   514,     0,   507,   508,   506,     0,     0,     0,   609,
       0,     0,   602,     0,     0,   120,     0,   120,     0,   120,
     248,   493,     0,   491,     0,   253,   257,   256,   260,     0,
     289,     0,   280,   283,     0,   286,     0,     0,   399,   400,
       0,     0,     0,     0,     0,     0,     0,   691,     0,     0,
     715,   720,     0,   241,     0,   376,   375,   415,   374,   373,
       0,   302,   446,     0,   309,     0,     0,     0,     0,   401,
     402,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   360,     0,
     603,     0,   617,   615,     0,   487,     0,     0,   499,     0,
     498,     0,   501,     0,   490,     0,     0,     0,   267,     0,
     264,     0,     0,   251,     0,     0,   236,     0,     0,    55,
      75,     0,   272,   269,   301,   247,   245,   246,     0,     0,
     234,   235,     0,    72,   221,   228,   229,   266,   271,   277,
       0,     0,   220,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   548,     0,     0,   561,     0,   465,   512,     0,   516,
       0,   520,   320,   634,   637,     0,   640,   630,   607,     0,
     644,     0,   651,     0,   658,     0,     0,   664,     0,   667,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   248,     0,     0,   711,   689,   691,     0,   716,     0,
       0,     0,   691,     0,     0,   694,     0,     0,   722,     0,
      31,     0,    29,   403,     0,     0,   352,   349,   351,     0,
     411,     0,   295,     0,   117,     0,     0,   426,   425,     0,
       0,   427,   431,   377,   378,   390,   391,   388,   389,     0,
     420,     0,   408,     0,   444,   445,   379,   380,   395,   396,
     397,   398,     0,     0,   393,   394,   392,   386,   387,   382,
     381,   383,   384,   385,     0,     0,     0,   358,     0,     0,
       0,     0,   413,     0,   488,     0,   564,   565,   502,   505,
     504,    76,     0,   268,   265,     0,     0,     0,   252,    77,
      78,    53,    54,   273,   270,   302,   240,   237,    58,     0,
     279,   244,   330,   331,   333,   332,   334,   324,   325,   326,
     335,   336,     0,     0,     0,   322,   323,   337,   338,   327,
     328,   329,     0,     0,     0,   321,   535,     0,   472,   475,
       0,   515,   518,   320,   519,   635,   638,   610,   641,   631,
     608,   645,   647,   652,   654,   659,   661,   492,   665,   494,
     668,     0,   258,     0,     0,     0,     0,   416,     0,     0,
     417,     0,   447,   309,   449,   450,   451,   452,   453,     0,
       0,   692,     0,     0,     0,   691,     0,     0,     0,     0,
     700,   701,     0,   706,     0,   698,     0,     0,   718,     0,
       0,     0,   696,     0,   719,   714,   721,   693,     0,     0,
       0,     0,     0,   117,     0,     0,     0,   412,     0,     0,
       0,     0,   307,     0,   120,   422,     0,   428,     0,     0,
       0,   406,     0,     0,   432,   436,     0,     0,   409,     0,
       0,     0,   359,     0,   361,   404,     0,   414,   563,     0,
       0,     0,   274,     0,     0,     0,    56,    57,     0,    71,
      63,   344,   342,   343,   341,   339,   340,   116,   473,   474,
     475,   476,   467,   480,   517,   648,   655,   662,   254,   259,
       0,   292,   290,   281,   284,   287,     0,     0,     0,     0,
     682,   681,   691,     0,   712,     0,     0,   690,   705,   699,
     713,   697,   717,     0,   691,   703,   704,     0,   709,   695,
     242,    30,     0,     0,     0,   120,     0,     0,   296,   305,
     306,   304,   303,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,   433,     0,   421,     0,   407,     0,     0,
     405,   362,   566,   503,     0,    79,   215,    59,     0,    64,
       0,   485,   470,   467,   468,   469,   472,   255,     0,     0,
     291,     0,     0,     0,     0,   455,     0,     0,     0,     0,
     707,     0,     0,     0,     0,     0,   702,     0,   350,   464,
     353,     0,   345,     0,   308,   310,     0,   297,   312,     0,
     463,     0,   461,   348,   458,     0,     0,     0,   457,     0,
      74,    58,     0,    73,   471,   481,   470,     0,     0,     0,
     282,   285,   288,   418,     0,   456,   723,     0,     0,   708,
       0,   684,   683,   723,   710,   243,     0,   356,     0,     0,
     298,   423,   429,     0,   462,   460,   459,    60,   263,     0,
      65,    69,    70,    67,    68,    66,     0,   483,   477,   293,
       0,   419,     0,     0,   686,   685,     0,     0,   354,     0,
     346,   299,   424,   430,   434,     0,   264,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,   357,   435,   265,
     270,   482,     0,   294,   724,   725,   688,   687,   726,   484
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1066, -1066, -1066, -1066, -1066, -1066,   262,   804, -1066, -1066,
   -1066,   878, -1066, -1066, -1066,   441,   849, -1066,   765, -1066,
   -1066,   816, -1066, -1066, -1066,  -288, -1066, -1066, -1066,  -287,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,   686, -1066,
   -1066,   820,   -58, -1066, -1066,   169,   163,  -409,  -396,  -565,
   -1066, -1066, -1066, -1065, -1066, -1066,  -201, -1066,    10, -1066,
   -1066, -1066, -1066,  -490,   -12, -1066, -1066, -1066, -1066, -1066,
    -284,  -279,  -277, -1066,  -275, -1066, -1066,   907, -1066, -1066,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066,  -654, -1066, -1066,  -192, -1066,   -38,  -498, -1066,  -374,
   -1066, -1066, -1066, -1017, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066, -1066,   483, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
    -136,  -221,  -273,  -222,  -134, -1066, -1066, -1066, -1066, -1066,
     416, -1066,   160, -1066,  -348,   508,  -495,  -489,   362, -1066,
   -1066,  -375, -1066, -1066,   872, -1066, -1066, -1066,   554,   121,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066, -1066, -1066, -1066, -1066,  -465,  -163, -1066,   522, -1066,
     523, -1066, -1066, -1066, -1066,  -358, -1066, -1066, -1066, -1066,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
   -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066, -1066,
    -146, -1066, -1066, -1066, -1066, -1066,   526,  -793,  -583,  -761,
   -1066, -1066, -1066,  -933,  -233, -1066,   -52,   143,   329, -1066,
   -1066, -1066,  -262, -1066
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    15,   136,    49,    16,   156,   162,   614,   455,
     141,   456,    92,    18,    19,    42,    43,    44,    82,    20,
      37,    38,   526,   527,  1038,  1039,   528,  1130,  1182,   529,
     530,   889,   531,   532,   533,   534,   535,  1033,   163,   164,
      33,    34,    35,    60,    61,    62,    63,    21,   320,   381,
     202,    22,   104,   203,   105,   148,   457,   536,   382,   537,
     458,   799,  1157,   538,   459,   539,   586,   761,  1137,   460,
     540,   541,   542,   543,   544,   545,   546,   547,   710,   548,
     461,   766,  1141,   767,  1142,   769,  1143,   462,   764,  1140,
     463,   811,  1168,   464,  1002,  1003,   814,   465,   466,   679,
     550,   467,   468,   808,   469,   992,  1206,   993,  1239,   470,
     860,  1121,   812,  1106,  1242,  1108,  1243,  1175,  1258,   472,
     377,  1136,  1185,  1050,  1052,   920,   556,   736,  1226,  1250,
     378,   379,   581,   582,   264,   265,   964,   498,   566,   397,
     338,   339,   209,   334,    72,   114,    24,   153,   330,    84,
     129,    85,    25,    46,   108,   150,    26,   388,   207,   208,
      70,   111,    27,   151,   332,   499,   473,   329,   257,   258,
     169,   259,   403,   930,   568,   260,   401,   929,   398,   925,
     399,   926,   400,   928,   404,   931,   405,  1055,   406,   933,
     407,  1056,   408,   935,   409,  1057,   410,   938,   411,   940,
     583,    28,    51,   138,    29,    50,   474,   785,   786,   787,
     798,   984,   978,   973,  1088,   475,   788,   611,   789,   476,
     612,   477,  1233,   478
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      54,    64,   322,   256,   673,   488,   665,   863,   549,   575,
     751,   577,   753,   579,   755,   557,   995,   211,   678,   491,
     261,     2,   695,   696,   697,   969,   792,     3,    78,   692,
    -115,   981,   872,   779,   115,   116,  1078,   562,   143,   495,
     481,   479,   676,   960,   779,    64,    64,    64,   979,   711,
       4,  1166,     5,   635,     6,   624,   637,   638,   668,   668,
       7,    83,    79,    30,    31,    97,    98,    99,    91,     8,
     854,   855,   637,   638,   375,     9,   741,   607,   609,   210,
     563,   998,   384,   384,   668,    64,    64,    64,    64,  1132,
     564,   999,   336,    36,   963,   668,    10,   102,   106,   856,
      91,  1134,   363,   103,   107,   963,   743,   131,   943,  1051,
     337,   857,   756,   157,   158,   669,   670,   670,   671,    11,
     363,   672,   364,   668,   321,   263,   385,   385,   386,   731,
     376,   712,   142,  1152,   658,   659,   668,   496,   565,  1000,
    1150,   778,   670,   790,  1001,   780,  1197,   735,   858,   859,
     658,   659,   335,   670,   255,   497,   668,   994,  1135,   622,
      55,  1249,   496,    47,   831,   363,  1254,    86,   365,   746,
     500,   490,   109,   501,   375,  1077,   832,   117,   110,   210,
     497,   670,   118,   869,   119,  1262,   120,   873,    56,  1238,
     968,   669,   482,   480,   670,   625,   626,    80,   390,   623,
     256,   883,   669,   256,  1073,    48,   132,   669,    81,    32,
     483,   795,    12,   891,   670,   982,  1199,   256,  1031,   484,
     489,    71,  1257,    13,   121,   733,    93,    94,    95,    14,
     553,    45,   256,   734,   502,   256,   256,   256,   256,   771,
      65,   256,   975,   256,    52,   256,   796,   256,   797,   977,
     922,   983,   567,   569,   570,   571,   159,    57,   574,   554,
     576,   160,   578,   161,    52,   120,   144,   145,   146,   147,
     976,    53,   256,   256,  1085,   977,   363,  1086,   780,   363,
     747,   961,  1026,   750,  1026,   627,   628,   629,   630,   631,
     994,    53,   632,   633,   634,   635,  1026,   636,   637,   638,
     639,    52,    39,    40,  1163,   640,  1068,   641,   642,   681,
    1087,  1149,  1124,   643,  1147,   645,   204,   690,   435,   436,
     437,    52,    58,  1155,   363,  1083,  1154,   112,    53,    41,
     804,    59,   134,   113,  1048,   890,    66,    67,   135,    68,
    1049,   970,   971,    74,   205,   625,   626,    73,    53,   585,
     745,   255,   215,    75,   255,  1111,   100,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,    69,   255,   972,
     172,    74,   996,  1030,  1026,   555,   658,   659,    74,   216,
    1026,   101,  1072,   255,    76,   100,   255,   255,   255,   255,
      39,    40,   255,  1026,   255,  1026,   255,  1084,   255,   997,
     967,  1027,   255,   391,  1041,  1042,  1043,  1059,   948,   949,
    1047,   951,   491,  1146,  1044,  1045,  1046,    52,  1004,    83,
    1173,  1103,  1195,   255,   255,  1096,   804,   394,   967,    88,
     395,   805,   748,   396,   256,   627,   628,   629,   630,  1105,
      97,   988,    99,    91,    53,   635,  1104,   636,   637,   638,
     639,  1129,   323,  1208,   749,   640,   324,   641,   642,  -646,
     625,   626,   818,   822,  -646,   806,  -653,  -660,  -355,    89,
     757,  -653,  -660,  -355,   871,   325,   326,   327,   328,    90,
     759,  1138,  -646,   879,  1035,    96,  1017,  1021,  1075,  -653,
    -660,  -355,   758,   154,   155,  1036,  1037,   886,  1018,  1022,
    1076,   888,   760,  1139,   596,   124,   363,   126,   256,   321,
     752,    81,   653,   654,   655,   656,   657,   212,   213,   363,
     363,   127,   128,   754,  1058,   868,   658,   659,   363,   363,
    1161,   363,  1062,  1212,   139,  1244,   154,   155,   800,   801,
      97,    98,    99,   140,    79,   625,   626,   212,   213,   214,
     627,   628,   629,   630,   631,   149,   166,   632,   633,   634,
     635,   130,   636,   637,   638,   639,   453,   694,   137,   321,
     640,   133,   641,   642,   152,   167,   168,   763,   643,   644,
     645,  1153,   256,   170,   646,   255,   171,    97,   206,   625,
     626,   210,   262,   256,   263,   256,   317,  1095,   256,   927,
     919,   256,   318,   256,   256,   319,   256,   321,   331,   807,
     937,   341,   625,   626,   333,   941,   342,   343,   946,   344,
     647,   345,   648,   649,   650,   651,   652,   653,   654,   655,
     656,   657,   346,   347,  1094,   348,   256,   629,   630,   349,
     350,   658,   659,   351,   363,   635,   352,   636,   637,   638,
     639,   366,   368,  1198,   367,   640,   369,   370,   372,   255,
     549,   373,   371,   374,   393,   380,   383,   387,   392,   402,
     389,   492,   493,   494,   558,   560,  1032,   561,   589,  1015,
     572,   629,   630,   590,   591,   592,   593,   595,   597,   635,
     667,   636,   637,   638,   639,   594,   685,   613,  1228,   640,
     596,   621,   627,   628,   629,   630,   631,   686,   600,   632,
     633,   634,   635,   601,   636,   637,   638,   639,   625,   626,
     602,    64,   640,   603,   641,   642,   658,   659,   604,    13,
     643,   644,   645,   255,   605,   661,   646,   662,   687,   666,
     699,  1255,   703,   709,   255,   700,   255,   701,  1169,   255,
     704,   705,   255,   706,   255,   255,   707,   255,   655,   656,
     657,   708,   738,   744,   762,   813,   829,   866,   867,   711,
     658,   659,   647,   877,   648,   649,   650,   651,   652,   653,
     654,   655,   656,   657,   880,   882,   885,   255,   881,   887,
     918,   921,   932,   658,   659,   364,   934,   962,   936,   967,
     974,   321,   990,   991,   987,  1005,  1024,  1029,   627,   628,
     629,   630,   631,  1006,  1007,   632,   633,   634,   635,  1008,
     636,   637,   638,   639,  1051,  1053,  1060,  1079,   640,   256,
     641,   642,  1099,  1081,  1127,   471,   643,   644,   645,  1009,
    1010,  1089,   646,  1020,   486,  1067,  1100,  1101,  1102,  1113,
    1114,  1116,  1119,  1122,  1184,  1144,   625,   626,  1131,  1151,
    1156,  1162,  1165,  1189,  1207,   551,  1183,  1230,  1240,  1252,
    1253,  1256,  1265,  1261,  1268,   559,   802,   122,   647,    17,
     648,   649,   650,   651,   652,   653,   654,   655,   656,   657,
      77,   165,   125,  1217,   123,  1220,   917,  1034,  1221,   658,
     659,   340,   353,  1222,  1167,  1223,   916,  1224,    23,   598,
     599,  1164,  1186,  1227,  1187,  1066,  1133,   674,   939,   584,
     610,    87,  1204,  1180,   740,  1082,   573,  1181,   354,   355,
     615,   616,   617,   618,   619,   620,   587,   552,   793,   986,
     588,  1237,     0,   256,     0,   256,   627,   628,   629,   630,
     631,     0,     0,   632,   633,   634,   635,     0,   636,   637,
     638,   639,  1171,   625,   626,     0,   640,     0,   641,   642,
       0,     0,     0,     0,   643,   644,   645,   664,     0,     0,
     255,     0,     0,     0,     0,     0,     0,   675,   356,     0,
       0,   680,   357,     0,   683,   684,     0,     0,     0,     0,
     689,     0,   693,     0,     0,     0,   698,     0,     0,     0,
       0,   702,   256,     0,   353,     0,   647,     0,   648,   649,
     650,   651,   652,   653,   654,   655,   656,   657,     0,  1213,
       0,     0,     0,     0,     0,     0,   732,   658,   659,     0,
     354,   355,     0,     0,     0,     0,     0,     0,     0,   742,
     358,     0,     0,     0,   359,   629,   630,   360,     0,     0,
       0,  1269,     0,   635,     0,     0,   637,   638,   639,     0,
       0,     0,   361,   640,   765,     0,     0,   768,     0,   770,
       0,   362,     0,   772,   773,   774,   775,   776,   777,     0,
     784,     0,   784,     0,   255,     0,   255,     0,     0,     0,
     356,     0,     0,   563,   357,     0,     0,     0,     0,     0,
     823,   824,     0,   564,   825,   826,   827,   828,     0,   830,
       0,   833,   834,   835,   836,   837,   838,   839,   840,   841,
     843,   844,   845,   846,   847,   848,   849,   850,   851,   852,
     853,     0,   861,     0,   658,   659,     0,     0,     0,   865,
       0,   625,   626,     0,     0,     0,   870,     0,     0,     0,
     874,   565,   358,   255,     0,   878,   359,     0,     0,   360,
       0,     0,     0,     0,   884,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   361,     0,     0,     0,     0,     0,
     739,     0,   353,   362,   807,     0,   892,   893,   894,   895,
     896,   897,   898,   899,   900,   901,   905,   906,   907,   908,
     909,   910,   911,   915,     0,     0,     0,     0,   354,   355,
       0,     0,     0,   923,     0,     0,     0,   807,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   627,   628,   629,   630,   631,   945,     0,   632,   633,
     634,   635,     0,   636,   637,   638,   639,     0,     0,     0,
       0,   640,     0,   641,   642,   615,   620,     0,     0,   643,
     644,   645,     0,     0,   620,   646,     0,   985,   356,   610,
       0,     0,   357,     0,   989,     0,     0,     0,   -66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   625,
     626,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   647,     0,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   658,   659,     0,     0,     0,     0,  1019,  1232,
     358,     0,  1023,     0,   359,     0,   953,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   905,   915,
       0,     0,   361,     0,     0,     0,     0,     0,     0,     0,
       0,   362,  1040,     0,     0,     0,     0,     0,     0,     0,
       0,   713,   714,   715,   716,   717,   718,   719,   720,   627,
     628,   629,   630,   631,   721,   722,   632,   633,   634,   635,
     723,   636,   637,   638,   639,     0,     0,     0,     0,   640,
     724,   641,   642,   725,   726,     0,     0,   643,   644,   645,
     727,   728,   729,   646,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   625,   626,     0,     0,  1069,  1070,  1071,     0,  1074,
       0,     0,     0,     0,     0,     0,     0,  1080,   730,   647,
     784,   648,   649,   650,   651,   652,   653,   654,   655,   656,
     657,     0,     0,  1092,  1093,     0,     0,   625,   626,  1098,
     658,   659,     0,     0,     0,     0,     0,     0,     0,  1107,
       0,  1109,     0,  1112,     0,     0,     0,     0,     0,  1115,
       0,     0,     0,  1118,     0,     0,     0,     0,     0,  1098,
       0,     0,     0,  1123,     0,     0,  1125,   551,     0,     0,
       0,  1128,     0,   713,   714,   715,   716,   717,   718,   719,
     720,   627,   628,   629,   630,   631,   721,   722,   632,   633,
     634,   635,   875,   636,   637,   638,   639,     0,     0,     0,
       0,   640,   724,   641,   642,   725,   726,     0,     0,   643,
     644,   645,   727,   728,   729,   646,     0,   627,   628,   629,
     630,   631,     0,     0,   632,   633,   634,   635,     0,   636,
     637,   638,   639,     0,     0,     0,     0,   640,     0,   641,
     642,     0,     0,     0,     0,     0,     0,  1176,     0,  1177,
     876,   647,  1179,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,     0,     0,     0,     0,     0,     0,     0,
       0,  1188,   658,   659,  1190,  1191,  1192,  1194,     0,   924,
    1196,     0,     0,     0,  1200,  1201,  1202,  1203,     0,     0,
    1205,   650,   651,   652,   653,   654,   655,   656,   657,  1210,
       0,     0,     0,     0,     0,     0,     0,     0,   658,   659,
       0,   353,     0,     0,     0,  1225,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1234,  1235,     0,     0,     0,     0,     0,   354,   355,     0,
       0,     0,  1241,     0,     0,     0,     0,     0,     0,     0,
       0,  1246,  1248,     0,     0,     0,     0,   503,     0,     0,
       0,     0,     3,  1251,   504,   505,   506,     0,   507,     0,
     412,   413,   414,   415,   416,     0,     0,     0,  1259,     0,
    1260,   508,   417,   509,   510,  1264,     0,  1266,  1267,     0,
       0,     0,     0,   511,   418,     0,   512,   356,   513,   419,
       0,   357,   420,     0,     8,   421,   514,     0,   515,   422,
       0,     0,   516,   517,     0,     0,     0,     0,     0,   518,
       0,     0,   221,   222,   223,     0,   225,   226,   227,   228,
     229,   424,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,     0,   243,   244,   245,   353,     0,   248,
     249,   250,   251,     0,     0,   425,   519,     0,     0,   358,
       0,     0,     0,   359,     0,  1063,   360,     0,   426,   427,
       0,     0,     0,   354,   355,     0,     0,     0,     0,     0,
       0,   361,     0,   520,   521,   522,     0,     0,     0,     0,
     362,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   428,   429,   430,   431,   432,     0,   433,     0,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
      53,   523,   446,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   356,     0,     0,     0,   357,     0,     0,
       0,     0,     0,   447,   448,   449,     0,    12,     0,     0,
     450,   451,     0,     0,     0,     0,     0,     0,     0,     0,
     452,     0,   453,   454,   524,   321,   525,   503,     0,     0,
       0,     0,     3,     0,   504,   505,   506,     0,   507,     0,
     412,   413,   414,   415,   416,     0,     0,     0,     0,     0,
       0,   508,   417,   509,   510,   358,     0,     0,     0,   359,
       0,  1064,   360,   511,   418,     0,   512,     0,   513,   419,
       0,     0,   420,     0,     8,   421,   514,   361,   515,   422,
       0,     0,   516,   517,     0,     0,   362,     0,     0,   518,
       0,     0,   221,   222,   223,     0,   225,   226,   227,   228,
     229,   424,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,     0,   243,   244,   245,     0,     0,   248,
     249,   250,   251,     0,     0,   425,   519,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   426,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   520,   521,   522,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   428,   429,   430,   431,   432,     0,   433,     0,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
      53,   523,   446,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   447,   448,   449,     0,    12,     0,     0,
     450,   451,     0,     0,     0,   412,   413,   414,   415,   416,
     452,     0,   453,   454,   524,   321,  1126,   417,     0,     0,
       0,     0,     0,   353,     0,     0,     0,     0,     0,   418,
       0,     0,     0,     0,   419,     0,     0,   420,     0,     0,
     421,     0,   779,     0,   422,     0,     0,     0,     0,   354,
     355,     0,     0,     0,   423,     0,     0,   221,   222,   223,
       0,   225,   226,   227,   228,   229,   424,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,     0,   243,
     244,   245,     0,     0,   248,   249,   250,   251,     0,     0,
     425,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   426,   427,     0,     0,     0,     0,   356,
       0,     0,     0,   357,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,   353,   428,   429,   430,   431,
     432,     0,   433,   780,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   781,   445,   446,     0,     0,
       0,   354,   355,     0,     0,     0,     0,     0,     0,     0,
       0,   358,     0,     0,     0,   359,     0,     0,   782,   448,
     449,     0,    12,     0,     0,   450,   451,   412,   413,   414,
     415,   416,     0,   361,     0,   783,     0,   453,   454,   417,
     321,     0,   362,     0,     0,   353,     0,     0,     0,     0,
       0,   418,     0,     0,     0,     0,   419,     0,     0,   420,
       0,   356,   421,     0,     0,   357,   422,     0,     0,     0,
       0,   354,   355,     0,     0,     0,   423,     0,     0,   221,
     222,   223,     0,   225,   226,   227,   228,   229,   424,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
       0,   243,   244,   245,     0,     0,   248,   249,   250,   251,
       0,     0,   425,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   358,     0,   426,   427,   359,     0,  1065,
     360,   356,     0,     0,     0,   357,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   361,     0,   625,   626,     0,
      52,     0,     0,     0,   362,     0,     0,     0,   428,   429,
     430,   431,   432,     0,   433,   780,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   781,   445,   446,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   358,     0,     0,     0,   359,     0,     0,
     782,   448,   449,     0,    12,     0,     0,   450,   451,   412,
     413,   414,   415,   416,     0,   361,     0,   791,     0,   453,
     454,   417,   321,     0,   362,     0,     0,   481,     0,     0,
       0,     0,     0,   418,     0,     0,     0,     0,   419,   629,
     630,   420,     0,     0,   421,     0,     0,   635,   422,   636,
     637,   638,   639,     0,     0,     0,     0,   640,   423,     0,
       0,   221,   222,   223,     0,   225,   226,   227,   228,   229,
     424,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,     0,   243,   244,   245,     0,     0,   248,   249,
     250,   251,     0,     0,   425,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   426,   427,     0,
       0,     0,     0,     0,   653,   654,   655,   656,   657,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   658,   659,
       0,     0,    52,     0,     0,     0,     0,     0,     0,   353,
     428,   429,   430,   431,   432,     0,   433,     0,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   443,   444,    53,
     445,   446,     0,     0,     0,   354,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   482,
       0,     0,   447,   448,   449,     0,    12,     0,     0,   450,
     451,   412,   413,   414,   415,   416,     0,   483,     0,   452,
       0,   453,   454,   417,   321,     0,   484,     0,     0,     0,
       0,     0,     0,     0,     0,   418,     0,     0,     0,     0,
     419,     0,     0,   420,     0,   356,   421,     0,     0,   357,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     423,     0,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   424,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,     0,     0,   425,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   358,     0,   426,
     427,   359,     0,  1090,   360,     0,     0,     0,   677,     0,
       0,     0,     0,     0,   520,   521,   522,     0,     0,   361,
       0,     0,     0,     0,    52,     0,     0,     0,   362,     0,
       0,     0,   428,   429,   430,   431,   432,     0,   433,     0,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,    53,   445,   446,     0,     0,     0,     0,     0,     0,
     412,   413,   414,   415,   416,     0,     0,     0,     0,     0,
       0,     0,   417,     0,   447,   448,   449,     0,    12,     0,
       0,   450,   451,     0,   418,     0,     0,     0,     0,   419,
       0,   452,   420,   453,   454,   421,   321,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423,
       0,     0,   221,   222,   223,     0,   225,   226,   227,   228,
     229,   424,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,     0,   243,   244,   245,     0,     0,   248,
     249,   250,   251,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   426,   427,
       0,     0,     0,     0,     0,     0,     0,   691,     0,     0,
       0,     0,     0,   520,   521,   522,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   428,   429,   430,   431,   432,     0,   433,     0,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
      53,   445,   446,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   447,   448,   449,     0,    12,     0,     0,
     450,   451,     0,     0,     0,   412,   413,   414,   415,   416,
     452,     0,   453,   454,     0,   321,  1218,   417,   509,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   418,
       0,     0,     0,     0,   419,     0,     0,   420,     0,     0,
     421,   514,     0,     0,   422,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   423,     0,     0,   221,   222,   223,
       0,   225,   226,   227,   228,   229,   424,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,     0,   243,
     244,   245,     0,     0,   248,   249,   250,   251,     0,     0,
     425,  1219,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   426,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   428,   429,   430,   431,
     432,     0,   433,     0,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,    53,   445,   446,     0,     0,
       0,     0,     0,     0,   412,   413,   414,   415,   416,     0,
       0,     0,     0,     0,     0,     0,   417,     0,   447,   448,
     449,     0,    12,     0,     0,   450,   451,     0,   418,     0,
       0,     0,     0,   419,     0,   452,   420,   453,   454,   421,
     321,     0,     0,   422,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   423,     0,     0,   221,   222,   223,     0,
     225,   226,   227,   228,   229,   424,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,     0,   243,   244,
     245,     0,     0,   248,   249,   250,   251,     0,     0,   425,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   426,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   520,   521,   522,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   428,   429,   430,   431,   432,
       0,   433,     0,   434,   435,   436,   437,   438,   439,   440,
     441,   442,   443,   444,    53,   445,   446,     0,     0,     0,
       0,     0,     0,   412,   413,   414,   415,   416,     0,     0,
       0,     0,     0,     0,     0,   417,     0,   447,   448,   449,
       0,    12,     0,     0,   450,   451,     0,   418,     0,     0,
       0,     0,   419,     0,   452,   420,   453,   454,   421,   321,
       0,     0,   422,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   423,     0,     0,   221,   222,   223,     0,   225,
     226,   227,   228,   229,   424,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,     0,   243,   244,   245,
       0,     0,   248,   249,   250,   251,     0,     0,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   426,   427,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   902,   903,   904,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   428,   429,   430,   431,   432,     0,
     433,     0,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,    53,   445,   446,     0,     0,     0,     0,
       0,     0,   412,   413,   414,   415,   416,     0,     0,     0,
       0,     0,     0,     0,   417,     0,   447,   448,   449,     0,
      12,     0,     0,   450,   451,     0,   418,     0,     0,     0,
       0,   419,     0,   452,   420,   453,   454,   421,   321,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,   221,   222,   223,     0,   225,   226,
     227,   228,   229,   424,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,     0,   243,   244,   245,     0,
       0,   248,   249,   250,   251,     0,     0,   425,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     426,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   912,   913,   914,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   428,   429,   430,   431,   432,     0,   433,
       0,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     443,   444,    53,   445,   446,     0,     0,     0,     0,     0,
       0,   412,   413,   414,   415,   416,     0,     0,     0,     0,
       0,     0,     0,   417,     0,   447,   448,   449,   353,    12,
       0,     0,   450,   451,     0,   418,     0,     0,     0,     0,
     419,     0,   452,   420,   453,   454,   421,   321,     0,     0,
     422,     0,     0,     0,   354,   355,     0,     0,     0,     0,
     423,     0,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   424,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,     0,     0,   425,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   426,
     427,     0,     0,     0,   356,     0,     0,     0,   357,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,   353,   428,   429,   430,   431,   432,     0,   433,     0,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,    53,   445,   446,     0,     0,     0,   354,   355,     0,
       0,     0,     0,     0,     0,     0,   358,     0,     0,     0,
     359,     0,  1160,   360,   447,   448,   449,     0,    12,     0,
       0,   450,   451,   412,   413,   414,   415,   416,   361,   809,
       0,   452,   810,   453,   454,   417,   321,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   418,     0,     0,
       0,     0,   419,     0,     0,   420,     0,   356,   421,     0,
       0,   357,   422,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   423,     0,     0,   221,   222,   223,     0,   225,
     226,   227,   228,   229,   424,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,     0,   243,   244,   245,
       0,     0,   248,   249,   250,   251,     0,     0,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   358,
       0,   426,   427,   359,     0,  1211,   360,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   361,     0,     0,     0,     0,    52,     0,     0,     0,
     362,     0,     0,     0,   428,   429,   430,   431,   432,     0,
     433,     0,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,    53,   445,   446,     0,     0,     0,     0,
       0,     0,   412,   413,   414,   415,   416,     0,     0,     0,
       0,     0,     0,     0,   417,     0,   447,   448,   449,     0,
      12,     0,     0,   450,   451,     0,   418,     0,     0,     0,
       0,   419,   485,   452,   420,   453,   454,   421,   321,     0,
       0,   422,     0,     0,     0,     0,     0,   682,     0,     0,
       0,   423,     0,     0,   221,   222,   223,     0,   225,   226,
     227,   228,   229,   424,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,     0,   243,   244,   245,     0,
       0,   248,   249,   250,   251,     0,     0,   425,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     426,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   428,   429,   430,   431,   432,     0,   433,
       0,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     443,   444,    53,   445,   446,     0,     0,     0,     0,     0,
       0,   412,   413,   414,   415,   416,     0,     0,     0,     0,
       0,     0,     0,   417,     0,   447,   448,   449,     0,    12,
       0,     0,   450,   451,     0,   418,     0,     0,     0,     0,
     419,     0,   452,   420,   453,   454,   421,   321,     0,     0,
     422,     0,     0,   688,     0,     0,     0,     0,     0,     0,
     423,     0,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   424,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,     0,     0,   425,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   426,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   428,   429,   430,   431,   432,     0,   433,     0,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,    53,   445,   446,     0,     0,     0,     0,     0,     0,
     412,   413,   414,   415,   416,     0,     0,     0,     0,     0,
       0,     0,   417,     0,   447,   448,   449,     0,    12,     0,
       0,   450,   451,     0,   418,     0,     0,     0,     0,   419,
       0,   452,   420,   453,   454,   421,   321,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423,
       0,     0,   221,   222,   223,     0,   225,   226,   227,   228,
     229,   424,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,     0,   243,   244,   245,     0,     0,   248,
     249,   250,   251,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   426,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   428,   429,   430,   431,   432,     0,   433,     0,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
      53,   445,   446,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   613,     0,   447,   448,   449,     0,    12,     0,     0,
     450,   451,   412,   413,   414,   415,   416,     0,     0,   842,
     452,     0,   453,   454,   417,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   418,     0,     0,     0,
       0,   419,     0,     0,   420,     0,     0,   421,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,   221,   222,   223,     0,   225,   226,
     227,   228,   229,   424,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,     0,   243,   244,   245,     0,
       0,   248,   249,   250,   251,     0,     0,   425,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     426,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   428,   429,   430,   431,   432,     0,   433,
       0,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     443,   444,    53,   445,   446,     0,     0,     0,     0,     0,
       0,   412,   413,   414,   415,   416,     0,     0,     0,     0,
       0,     0,     0,   417,     0,   447,   448,   449,     0,    12,
       0,     0,   450,   451,     0,   418,     0,     0,     0,     0,
     419,     0,   452,   420,   453,   454,   421,   321,     0,     0,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     423,     0,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   424,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,     0,     0,   425,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   426,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   428,   429,   430,   431,   432,     0,   433,     0,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,    53,   445,   446,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   447,   448,   449,     0,    12,     0,
       0,   450,   451,   412,   413,   414,   415,   416,     0,     0,
       0,   452,   862,   453,   454,   417,   321,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   418,     0,     0,
       0,     0,   419,     0,     0,   420,     0,     0,   421,     0,
       0,     0,   422,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   423,     0,     0,   221,   222,   223,     0,   225,
     226,   227,   228,   229,   424,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,     0,   243,   244,   245,
       0,     0,   248,   249,   250,   251,     0,     0,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   426,   427,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   428,   429,   430,   431,   432,     0,
     433,     0,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,    53,   445,   446,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   448,   449,     0,
      12,     0,     0,   450,   451,   412,   413,   414,   415,   416,
       0,     0,     0,   452,   942,   453,   454,   417,   321,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   418,
       0,     0,     0,     0,   419,     0,     0,   420,     0,     0,
     421,     0,     0,     0,   422,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   423,     0,     0,   221,   222,   223,
       0,   225,   226,   227,   228,   229,   424,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,     0,   243,
     244,   245,     0,     0,   248,   249,   250,   251,     0,     0,
     425,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   426,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,   428,   429,   430,   431,
     432,     0,   433,     0,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,    53,   445,   446,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   944,     0,   447,   448,
     449,     0,    12,     0,     0,   450,   451,   412,   413,   414,
     415,   416,     0,     0,     0,   452,     0,   453,   454,   417,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   418,     0,     0,     0,     0,   419,     0,     0,   420,
       0,     0,   421,     0,     0,     0,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   423,     0,     0,   221,
     222,   223,     0,   225,   226,   227,   228,   229,   424,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
       0,   243,   244,   245,     0,     0,   248,   249,   250,   251,
       0,     0,   425,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   426,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,   428,   429,
     430,   431,   432,     0,   433,     0,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,    53,   445,   446,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     447,   448,   449,     0,    12,     0,     0,   450,   451,   412,
     413,   414,   415,   416,     0,     0,     0,   452,   965,   453,
     454,   417,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   418,     0,     0,     0,     0,   419,     0,
       0,   420,     0,     0,   421,     0,     0,     0,   422,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   423,     0,
       0,   221,   222,   223,     0,   225,   226,   227,   228,   229,
     424,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,     0,   243,   244,   245,     0,     0,   248,   249,
     250,   251,     0,     0,   425,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   426,   427,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
     428,   429,   430,   431,   432,     0,   433,     0,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   443,   444,    53,
     445,   446,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   447,   448,   449,     0,    12,     0,     0,   450,
     451,   412,   413,   414,   415,   416,     0,     0,     0,   452,
     980,   453,   454,   417,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   418,     0,     0,     0,     0,
     419,     0,     0,   420,     0,     0,   421,     0,     0,     0,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     423,     0,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   424,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,     0,     0,   425,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   426,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   428,   429,   430,   431,   432,     0,   433,     0,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,    53,   445,   446,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   412,   413,
     414,   415,   416,     0,   447,   448,   449,     0,    12,     0,
     417,   450,   451,     0,     0,     0,     0,     0,     0,  1097,
       0,   452,   418,   453,   454,     0,   321,   419,     0,     0,
     420,     0,     0,   421,     0,     0,     0,   422,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   423,     0,     0,
     221,   222,   223,     0,   225,   226,   227,   228,   229,   424,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,     0,   243,   244,   245,     0,     0,   248,   249,   250,
     251,     0,     0,   425,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   426,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,   428,
     429,   430,   431,   432,     0,   433,     0,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,    53,   445,
     446,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   447,   448,   449,     0,    12,     0,     0,   450,   451,
     412,   413,   414,   415,   416,     0,     0,     0,   452,  1110,
     453,   454,   417,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   418,     0,     0,     0,     0,   419,
       0,     0,   420,     0,     0,   421,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423,
       0,     0,   221,   222,   223,     0,   225,   226,   227,   228,
     229,   424,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,     0,   243,   244,   245,     0,     0,   248,
     249,   250,   251,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   426,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,   428,   429,   430,   431,   432,     0,   433,     0,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
      53,   445,   446,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   447,   448,   449,     0,    12,     0,     0,
     450,   451,   412,   413,   414,   415,   416,     0,     0,     0,
     452,  1145,   453,   454,   417,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   418,     0,     0,     0,
       0,   419,     0,     0,   420,     0,     0,   421,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,   221,   222,   223,     0,   225,   226,
     227,   228,   229,   424,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,     0,   243,   244,   245,     0,
       0,   248,   249,   250,   251,     0,     0,   425,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     426,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   428,   429,   430,   431,   432,     0,   433,
       0,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     443,   444,    53,   445,   446,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   447,   448,   449,     0,    12,
       0,     0,   450,   451,   412,   413,   414,   415,   416,     0,
       0,     0,   452,  1193,   453,   454,   417,   321,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   418,     0,
       0,     0,     0,   419,     0,     0,   420,     0,     0,   421,
       0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   423,     0,     0,   221,   222,   223,     0,
     225,   226,   227,   228,   229,   424,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,     0,   243,   244,
     245,     0,     0,   248,   249,   250,   251,     0,     0,   425,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   426,   427,     0,     0,     0,     0,     0,     0,
       0,  1209,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,   428,   429,   430,   431,   432,
       0,   433,     0,   434,   435,   436,   437,   438,   439,   440,
     441,   442,   443,   444,    53,   445,   446,     0,     0,     0,
       0,     0,     0,   412,   413,   414,   415,   416,     0,     0,
       0,     0,     0,     0,     0,   417,     0,   447,   448,   449,
       0,    12,     0,     0,   450,   451,     0,   418,     0,     0,
       0,     0,   419,     0,   452,   420,   453,   454,   421,   321,
       0,     0,   422,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   423,     0,     0,   221,   222,   223,     0,   225,
     226,   227,   228,   229,   424,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,     0,   243,   244,   245,
       0,     0,   248,   249,   250,   251,     0,     0,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   426,   427,     0,     0,     0,     0,     0,     0,     0,
    1245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,   428,   429,   430,   431,   432,     0,
     433,     0,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,    53,   445,   446,     0,     0,     0,     0,
       0,     0,   412,   413,   414,   415,   416,     0,     0,     0,
       0,     0,     0,     0,   417,     0,   447,   448,   449,     0,
      12,     0,     0,   450,   451,     0,   418,     0,     0,     0,
       0,   419,     0,   452,   420,   453,   454,   421,   321,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,   221,   222,   223,     0,   225,   226,
     227,   228,   229,   424,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,     0,   243,   244,   245,     0,
       0,   248,   249,   250,   251,     0,     0,   425,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     426,   427,     0,     0,     0,     0,     0,     0,     0,  1247,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,   428,   429,   430,   431,   432,     0,   433,
       0,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     443,   444,    53,   445,   446,     0,     0,     0,     0,     0,
       0,   412,   413,   414,   415,   416,     0,     0,     0,     0,
       0,     0,     0,   417,     0,   447,   448,   449,     0,    12,
       0,     0,   450,   451,     0,   418,     0,     0,     0,     0,
     419,     0,   452,   420,   453,   454,   421,   321,     0,     0,
     422,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     423,     0,     0,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   424,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,     0,     0,   425,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   426,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   625,   626,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,   428,   429,   430,   431,   432,     0,   433,     0,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,    53,   445,   446,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   447,   448,   449,     0,    12,     0,
       0,   450,   451,     0,     0,     0,     0,     0,     0,     0,
       0,   452,     0,   453,   454,     0,   321,   713,   714,   715,
     716,   717,   718,   719,   720,   627,   628,   629,   630,   631,
     721,   722,   632,   633,   634,   635,   875,   636,   637,   638,
     639,     0,     0,     0,     0,   640,   724,   641,   642,   725,
     726,     0,     0,   643,   644,   645,   727,   728,   729,   646,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   625,
     626,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   217,     0,     0,     0,     0,     0,
     218,     0,     0,     0,   876,   647,     0,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   219,     0,     0,
       0,     0,     0,     0,     0,     0,   658,   659,     0,     0,
       0,     0,   220,  1054,     0,     0,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   627,
     628,   629,   630,   631,     0,     0,   632,   633,   634,   635,
       0,   636,   637,   638,   639,   625,   626,     0,     0,   640,
       0,   641,   642,     0,     0,     0,     0,   643,   644,   645,
       0,     0,     0,   646,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     254,     0,   625,   626,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,   647,
       0,   648,   649,   650,   651,   652,   653,   654,   655,   656,
     657,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     658,   659,     0,     0,   660,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   627,   628,   629,   630,   631,
       0,   487,   632,   633,   634,   635,     0,   636,   637,   638,
     639,     0,     0,     0,     0,   640,     0,   641,   642,     0,
       0,     0,     0,   643,   644,   645,     0,     0,     0,   646,
       0,     0,   627,   628,   629,   630,   631,     0,     0,   632,
     633,   634,   635,     0,   636,   637,   638,   639,   625,   626,
       0,     0,   640,     0,   641,   642,     0,     0,     0,     0,
     643,   644,   645,     0,     0,   647,   646,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,     0,     0,     0,
       0,     0,     0,   625,   626,     0,   658,   659,     0,     0,
     737,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   647,     0,   648,   649,   650,   651,   652,   653,
     654,   655,   656,   657,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   658,   659,     0,     0,   803,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   627,   628,
     629,   630,   631,     0,     0,   632,   633,   634,   635,     0,
     636,   637,   638,   639,     0,     0,     0,     0,   640,     0,
     641,   642,     0,     0,     0,     0,   643,   644,   645,     0,
       0,     0,   646,   627,   628,   629,   630,   631,     0,     0,
     632,   633,   634,   635,     0,   636,   637,   638,   639,   625,
     626,     0,     0,   640,     0,   641,   642,     0,     0,     0,
       0,   643,   644,   645,     0,     0,     0,   646,   647,     0,
     648,   649,   650,   651,   652,   653,   654,   655,   656,   657,
       0,     0,     0,     0,   625,   626,     0,     0,     0,   658,
     659,     0,     0,   864,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   647,     0,   648,   649,   650,   651,   652,
     653,   654,   655,   656,   657,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   658,   659,     0,     0,   947,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   627,
     628,   629,   630,   631,     0,     0,   632,   633,   634,   635,
       0,   636,   637,   638,   639,     0,     0,     0,     0,   640,
       0,   641,   642,     0,     0,     0,     0,   643,   644,   645,
       0,     0,     0,   646,   627,   628,   629,   630,   631,     0,
       0,   632,   633,   634,   635,     0,   636,   637,   638,   639,
     625,   626,     0,     0,   640,     0,   641,   642,     0,     0,
       0,     0,   643,   644,   645,     0,     0,     0,   646,   647,
       0,   648,   649,   650,   651,   652,   653,   654,   655,   656,
     657,     0,     0,     0,     0,   625,   626,     0,     0,     0,
     658,   659,     0,     0,   950,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   647,     0,   648,   649,   650,   651,
     652,   653,   654,   655,   656,   657,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   658,   659,     0,     0,   952,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     627,   628,   629,   630,   631,     0,     0,   632,   633,   634,
     635,     0,   636,   637,   638,   639,     0,     0,     0,     0,
     640,     0,   641,   642,     0,     0,     0,     0,   643,   644,
     645,     0,     0,     0,   646,   627,   628,   629,   630,   631,
       0,     0,   632,   633,   634,   635,     0,   636,   637,   638,
     639,   625,   626,     0,     0,   640,     0,   641,   642,     0,
       0,     0,     0,   643,   644,   645,     0,     0,     0,   646,
     647,     0,   648,   649,   650,   651,   652,   653,   654,   655,
     656,   657,     0,     0,     0,     0,   625,   626,     0,     0,
       0,   658,   659,     0,     0,   954,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   647,     0,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   658,   659,     0,     0,
     955,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   627,   628,   629,   630,   631,     0,     0,   632,   633,
     634,   635,     0,   636,   637,   638,   639,     0,     0,     0,
       0,   640,     0,   641,   642,     0,     0,     0,     0,   643,
     644,   645,     0,     0,     0,   646,   627,   628,   629,   630,
     631,     0,     0,   632,   633,   634,   635,     0,   636,   637,
     638,   639,   625,   626,     0,     0,   640,     0,   641,   642,
       0,     0,     0,     0,   643,   644,   645,     0,     0,     0,
     646,   647,     0,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,     0,     0,     0,     0,   625,   626,     0,
       0,     0,   658,   659,     0,     0,   956,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   647,     0,   648,   649,
     650,   651,   652,   653,   654,   655,   656,   657,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   658,   659,     0,
       0,   957,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   627,   628,   629,   630,   631,     0,     0,   632,
     633,   634,   635,     0,   636,   637,   638,   639,     0,     0,
       0,     0,   640,     0,   641,   642,     0,     0,     0,     0,
     643,   644,   645,     0,     0,     0,   646,   627,   628,   629,
     630,   631,     0,     0,   632,   633,   634,   635,     0,   636,
     637,   638,   639,   625,   626,     0,     0,   640,     0,   641,
     642,     0,     0,     0,     0,   643,   644,   645,     0,     0,
       0,   646,   647,     0,   648,   649,   650,   651,   652,   653,
     654,   655,   656,   657,     0,     0,     0,     0,   625,   626,
       0,     0,     0,   658,   659,     0,     0,   958,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   647,     0,   648,
     649,   650,   651,   652,   653,   654,   655,   656,   657,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   658,   659,
       0,     0,   959,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   627,   628,   629,   630,   631,     0,     0,
     632,   633,   634,   635,     0,   636,   637,   638,   639,     0,
       0,     0,     0,   640,     0,   641,   642,     0,     0,     0,
       0,   643,   644,   645,     0,     0,     0,   646,   627,   628,
     629,   630,   631,     0,     0,   632,   633,   634,   635,     0,
     636,   637,   638,   639,   625,   626,     0,     0,   640,     0,
     641,   642,     0,     0,     0,     0,   643,   644,   645,     0,
       0,     0,   646,   647,     0,   648,   649,   650,   651,   652,
     653,   654,   655,   656,   657,     0,     0,     0,     0,   625,
     626,     0,     0,     0,   658,   659,     0,     0,  1028,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   647,     0,
     648,   649,   650,   651,   652,   653,   654,   655,   656,   657,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   658,
     659,     0,     0,  1061,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   627,   628,   629,   630,   631,     0,
       0,   632,   633,   634,   635,     0,   636,   637,   638,   639,
       0,     0,     0,     0,   640,     0,   641,   642,     0,     0,
       0,     0,   643,   644,   645,     0,     0,     0,   646,   627,
     628,   629,   630,   631,     0,     0,   632,   633,   634,   635,
       0,   636,   637,   638,   639,   625,   626,     0,     0,   640,
       0,   641,   642,     0,     0,     0,     0,   643,   644,   645,
       0,     0,     0,   646,   647,     0,   648,   649,   650,   651,
     652,   653,   654,   655,   656,   657,     0,     0,     0,     0,
     625,   626,     0,     0,     0,   658,   659,     0,     0,  1148,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   647,
       0,   648,   649,   650,   651,   652,   653,   654,   655,   656,
     657,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     658,   659,     0,     0,  1158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   627,   628,   629,   630,   631,
       0,     0,   632,   633,   634,   635,     0,   636,   637,   638,
     639,     0,     0,     0,     0,   640,     0,   641,   642,     0,
       0,     0,     0,   643,   644,   645,     0,     0,     0,   646,
     627,   628,   629,   630,   631,     0,     0,   632,   633,   634,
     635,     0,   636,   637,   638,   639,   625,   626,     0,     0,
     640,     0,   641,   642,     0,     0,     0,     0,   643,   644,
     645,     0,     0,     0,   646,   647,     0,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,     0,     0,     0,
       0,   625,   626,     0,     0,     0,   658,   659,     0,     0,
    1159,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     647,     0,   648,   649,   650,   651,   652,   653,   654,   655,
     656,   657,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   658,   659,     0,     0,  1170,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   627,   628,   629,   630,
     631,     0,     0,   632,   633,   634,   635,     0,   636,   637,
     638,   639,     0,     0,     0,     0,   640,     0,   641,   642,
       0,     0,     0,     0,   643,   644,   645,     0,     0,     0,
     646,   627,   628,   629,   630,   631,     0,     0,   632,   633,
     634,   635,     0,   636,   637,   638,   639,   625,   626,     0,
       0,   640,     0,   641,   642,     0,     0,     0,     0,   643,
     644,   645,     0,     0,     0,   646,   647,     0,   648,   649,
     650,   651,   652,   653,   654,   655,   656,   657,     0,     0,
       0,     0,   625,   626,     0,     0,     0,   658,   659,     0,
       0,  1172,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   647,     0,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   658,   659,     0,     0,  1174,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   627,   628,   629,
     630,   631,     0,     0,   632,   633,   634,   635,     0,   636,
     637,   638,   639,     0,     0,     0,     0,   640,     0,   641,
     642,     0,     0,     0,     0,   643,   644,   645,     0,     0,
       0,   646,   627,   628,   629,   630,   631,     0,     0,   632,
     633,   634,   635,     0,   636,   637,   638,   639,   625,   626,
       0,     0,   640,     0,   641,   642,     0,     0,     0,     0,
     643,   644,   645,     0,     0,     0,   646,   647,     0,   648,
     649,   650,   651,   652,   653,   654,   655,   656,   657,     0,
       0,     0,     0,   625,   626,     0,     0,     0,   658,   659,
       0,     0,  1178,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   647,     0,   648,   649,   650,   651,   652,   653,
     654,   655,   656,   657,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   658,   659,     0,     0,  1214,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   627,   628,
     629,   630,   631,     0,     0,   632,   633,   634,   635,     0,
     636,   637,   638,   639,     0,     0,     0,     0,   640,     0,
     641,   642,     0,     0,     0,     0,   643,   644,   645,     0,
       0,     0,   646,   627,   628,   629,   630,   631,     0,     0,
     632,   633,   634,   635,     0,   636,   637,   638,   639,   625,
     626,     0,     0,   640,     0,   641,   642,     0,     0,     0,
       0,   643,   644,   645,     0,     0,     0,   646,   647,     0,
     648,   649,   650,   651,   652,   653,   654,   655,   656,   657,
       0,     0,     0,     0,   625,   626,     0,     0,     0,   658,
     659,     0,     0,  1215,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   647,     0,   648,   649,   650,   651,   652,
     653,   654,   655,   656,   657,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   658,   659,     0,     0,  1216,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   627,
     628,   629,   630,   631,     0,     0,   632,   633,   634,   635,
       0,   636,   637,   638,   639,     0,     0,     0,     0,   640,
       0,   641,   642,     0,     0,     0,     0,   643,   644,   645,
       0,     0,     0,   646,   627,   628,   629,   630,   631,     0,
       0,   632,   633,   634,   635,     0,   636,   637,   638,   639,
     625,   626,     0,     0,   640,     0,   641,   642,     0,     0,
       0,     0,   643,   644,   645,     0,     0,     0,   646,   647,
       0,   648,   649,   650,   651,   652,   653,   654,   655,   656,
     657,     0,     0,     0,     0,   625,   626,     0,     0,     0,
     658,   659,     0,     0,  1229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   647,     0,   648,   649,   650,   651,
     652,   653,   654,   655,   656,   657,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   658,   659,     0,     0,  1231,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     627,   628,   629,   630,   631,     0,     0,   632,   633,   634,
     635,     0,   636,   637,   638,   639,     0,     0,     0,     0,
     640,     0,   641,   642,     0,     0,     0,     0,   643,   644,
     645,     0,     0,     0,   646,   627,   628,   629,   630,   631,
       0,     0,   632,   633,   634,   635,     0,   636,   637,   638,
     639,   625,   626,     0,     0,   640,     0,   641,   642,     0,
       0,     0,     0,   643,   644,   645,     0,     0,     0,   646,
     647,     0,   648,   649,   650,   651,   652,   653,   654,   655,
     656,   657,     0,     0,     0,     0,   625,   626,     0,     0,
       0,   658,   659,     0,     0,  1236,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   647,     0,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   658,   659,     0,     0,
    1263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   627,   628,   629,   630,   631,     0,     0,   632,   633,
     634,   635,     0,   636,   637,   638,   639,     0,     0,     0,
       0,   640,     0,   641,   642,     0,     0,     0,     0,   643,
     644,   645,     0,     0,     0,   646,   627,   628,   629,   630,
     631,   625,   626,   632,   633,   634,   635,     0,   636,   637,
     638,   639,     0,     0,     0,     0,   640,     0,   641,   642,
       0,     0,     0,     0,   643,   644,   645,     0,     0,     0,
     646,   647,     0,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   658,   659,   663,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   647,     0,   648,   649,
     650,   651,   652,   653,   654,   655,   656,   657,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   658,   659,  1011,
       0,   627,   628,   629,   630,   631,   625,   626,   632,   633,
     634,   635,     0,   636,   637,   638,   639,     0,     0,     0,
       0,   640,     0,   641,   642,     0,     0,     0,     0,   643,
     644,   645,     0,     0,     0,   646,     0,     0,     0,     0,
       0,   625,   626,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   647,     0,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   658,   659,  1025,     0,   627,   628,   629,   630,
     631,     0,     0,   632,   633,   634,   635,     0,   636,   637,
     638,   639,     0,     0,     0,     0,   640,     0,   641,   642,
       0,     0,     0,     0,   643,   644,   645,     0,     0,     0,
     646,   627,   628,   629,   630,   631,     0,     0,   632,   633,
     634,   635,     0,   636,   637,   638,   639,   625,   626,     0,
       0,   640,     0,   641,   642,     0,     0,     0,     0,   643,
     644,   645,     0,     0,     0,   646,   647,     0,   648,   649,
     650,   651,   652,   653,   654,   655,   656,   657,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   658,   659,  1117,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   647,     0,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   658,   659,  1120,     0,     0,     0,     0,   713,
     714,   715,   716,   717,   718,   719,   720,   627,   628,   629,
     630,   631,   721,   722,   632,   633,   634,   635,   875,   636,
     637,   638,   639,  -320,     0,   266,   267,   640,   724,   641,
     642,   725,   726,     0,     0,   643,   644,   645,   727,   728,
     729,   646,   268,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     625,   626,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   876,   647,     0,   648,
     649,   650,   651,   652,   653,   654,   655,   656,   657,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   658,   659,
       0,     0,     0,     0,     0,     0,     0,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,     0,     0,   287,   288,   289,
       0,     0,     0,     0,     0,     0,   290,   291,   292,   293,
     294,     0,     0,   295,   296,   297,   298,   299,   300,   301,
     627,   628,   629,   630,   631,   625,   626,   632,   633,   634,
     635,     0,   636,   637,   638,   639,     0,     0,     0,     0,
     640,     0,   641,   642,     0,     0,   794,     0,   643,   644,
     645,     0,     0,     0,   646,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,     0,     0,   313,
     314,     0,     0,     0,     0,     0,   315,   316,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     647,     0,   648,   649,   650,   651,   652,   653,   654,   655,
     656,   657,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   658,   659,     0,     0,   627,   628,   629,   630,   631,
     625,   626,   632,   633,   634,   635,     0,   636,   637,   638,
     639,     0,     0,     0,     0,   640,     0,   641,   642,     0,
       0,   966,     0,   643,   644,   645,     0,     0,     0,   646,
       0,     0,     0,     0,     0,   625,   626,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   647,     0,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   658,   659,     0,     0,
     627,   628,   629,   630,   631,     0,     0,   632,   633,   634,
     635,     0,   636,   637,   638,   639,     0,     0,     0,     0,
     640,     0,   641,   642,     0,     0,     0,     0,   643,   644,
     645,   625,   626,     0,   646,   627,   628,   629,   630,   631,
       0,     0,   632,   633,   634,   635,     0,   636,   637,   638,
     639,     0,     0,     0,     0,   640,     0,   641,   642,     0,
       0,     0,     0,   643,   644,   645,   625,   626,     0,   646,
     647,  1016,   648,   649,   650,   651,   652,   653,   654,   655,
     656,   657,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   658,   659,     0,     0,     0,  1091,     0,     0,     0,
       0,     0,     0,     0,     0,   647,     0,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,     0,     0,     0,
       0,   627,   628,   629,   630,   631,   658,   659,   632,   633,
     634,   635,     0,   636,   637,   638,   639,     0,     0,     0,
       0,   640,     0,   641,   642,     0,     0,     0,     0,   643,
     644,   645,   625,   626,     0,   646,   627,   628,   629,   630,
     631,     0,     0,   632,   633,   634,   635,     0,   636,   637,
     638,   639,     0,     0,     0,     0,   640,     0,   641,   642,
       0,   625,   626,     0,   643,   644,   645,     0,     0,     0,
    -661,   647,     0,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   658,   659,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   647,     0,   648,   649,
     650,   651,   652,   653,   654,   655,   656,   657,     0,     0,
       0,     0,   627,   628,   629,   630,   631,   658,   659,   632,
     633,   634,   635,     0,   636,   637,   638,   639,     0,     0,
       0,     0,   640,     0,   641,   642,     0,   625,   626,     0,
     643,   627,   628,   629,   630,   631,     0,     0,   632,   633,
     634,   635,     0,   636,   637,   638,   639,     0,     0,     0,
       0,   640,     0,   641,   642,   625,   626,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   648,   649,   650,   651,   652,   653,
     654,   655,   656,   657,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   658,   659,     0,     0,     0,     0,     0,
       0,     0,     0,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,     0,     0,     0,     0,   627,   628,   629,
     630,   631,   658,   659,   632,   633,   634,   635,     0,   636,
     637,   638,   639,     0,     0,     0,     0,   640,     0,   641,
     642,   625,   626,     0,     0,   627,   628,   629,   630,   631,
       0,     0,   632,   633,   634,   635,     0,   636,   637,   638,
     639,     0,     0,     0,     0,   640,     0,   641,   642,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     649,   650,   651,   652,   653,   654,   655,   656,   657,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   658,   659,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     651,   652,   653,   654,   655,   656,   657,     0,     0,     0,
       0,   627,   628,   629,   630,   631,   658,   659,   632,     0,
       0,   635,     0,   636,   637,   638,   639,     0,     0,     0,
       0,   640,     0,   641,   642,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     606,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,     0,     0,     0,     0,     0,   218,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   219,   651,   652,   653,   654,
     655,   656,   657,     0,     0,     0,     0,     0,     0,     0,
     220,     0,   658,   659,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   608,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   217,     0,     0,     0,     0,     0,   218,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,   219,     0,     0,     0,   254,   815,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   220,
       0,     0,    53,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   221,   222,   223,
       0,   225,   226,   227,   228,   229,   424,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,     0,   243,
     244,   245,     0,     0,   248,   249,   250,   251,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   819,     0,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1012,     0,     0,     0,     0,
       0,     0,     0,   816,     0,     0,     0,     0,     0,     0,
       0,     0,   221,   222,   223,   817,   225,   226,   227,   228,
     229,   424,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,     0,   243,   244,   245,     0,     0,   248,
     249,   250,   251,   221,   222,   223,     0,   225,   226,   227,
     228,   229,   424,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,     0,   243,   244,   245,     0,     0,
     248,   249,   250,   251,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   820,     0,
     217,     0,     0,     0,     0,     0,   218,     0,     0,     0,
     821,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   219,     0,     0,     0,     0,     0,  1013,
       0,     0,     0,     0,     0,     0,     0,     0,   220,     0,
       0,  1014,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   217,
       0,     0,     0,     0,     0,   218,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,   219,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   220,     0,     0,
      53,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   173,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   254,     0,     0,     0,     0,
     174,     0,   175,     0,   176,   177,   178,   179,   180,   580,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,     0,   192,   193,   194,     0,     0,   195,   196,   197,
     198,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   199,   200,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   201
};

static const yytype_int16 yycheck[] =
{
      12,    13,   203,   166,   499,   363,   495,   661,   382,   405,
     575,   407,   577,   409,   579,   390,   809,   153,   508,   367,
     166,     0,   520,   521,   522,   786,   609,     6,    21,   519,
       7,   792,    19,    51,    14,    15,   969,   395,    96,    48,
      32,    32,   507,    19,    51,    57,    58,    59,    19,   126,
      29,   136,    31,   120,    33,   464,   123,   124,   121,   121,
      39,   164,    55,    18,    19,   138,   139,   140,   137,    48,
     123,   124,   123,   124,   149,    54,   566,   435,   436,   154,
     121,   121,   164,   164,   121,    97,    98,    99,   100,    45,
     131,   131,   146,   157,   131,   121,    75,    55,    55,   152,
     137,    44,   182,    61,    61,   131,   186,   210,   762,    65,
     164,   164,   181,    14,    15,   178,   179,   179,   181,    98,
     182,   184,   164,   121,   209,   164,   208,   208,   210,   210,
     205,   208,   205,   131,   201,   202,   121,   146,   179,   179,
    1073,   606,   179,   608,   184,   152,   131,   556,   201,   202,
     201,   202,   210,   179,   166,   164,   121,   164,   101,   164,
      33,  1226,   146,   164,   152,   182,   131,    46,   210,   186,
     205,   210,    55,   208,   149,   968,   164,   157,    61,   154,
     164,   179,   162,   673,   164,  1250,   166,   677,    61,  1206,
     208,   178,   184,   184,   179,    20,    21,   190,   334,   204,
     363,   691,   178,   366,   965,   206,    85,   178,   201,   164,
     202,   173,   191,   711,   179,   174,  1149,   380,   872,   211,
     366,    60,  1239,   202,   204,   202,    57,    58,    59,   208,
     179,   209,   395,   210,   380,   398,   399,   400,   401,   597,
     208,   404,   175,   406,   137,   408,   208,   410,   210,   208,
     740,   210,   398,   399,   400,   401,   157,   130,   404,   208,
     406,   162,   408,   164,   137,   166,    97,    98,    99,   100,
     203,   164,   435,   436,   174,   208,   182,   177,   152,   182,
     186,   779,   178,   186,   178,   110,   111,   112,   113,   114,
     164,   164,   117,   118,   119,   120,   178,   122,   123,   124,
     125,   137,   164,   165,  1097,   130,   960,   132,   133,   510,
     210,  1072,   208,   138,   208,   140,   181,   518,   154,   155,
     156,   137,   195,  1084,   182,   979,   208,    55,   164,   191,
     146,   204,    55,    61,    55,   710,     4,     5,    61,     7,
      61,   175,   176,   178,   209,    20,    21,   179,   164,   185,
     208,   363,   178,   181,   366,  1009,   178,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    35,   380,   203,
     205,   178,   178,   868,   178,   387,   201,   202,   178,   205,
     178,   203,   965,   395,   178,   178,   398,   399,   400,   401,
     164,   165,   404,   178,   406,   178,   408,   980,   410,   205,
     178,   205,   414,   203,   902,   903,   904,   205,   766,   767,
     203,   769,   760,  1067,   912,   913,   914,   137,   814,   164,
     205,   178,   205,   435,   436,   203,   146,   178,   178,   164,
     181,   151,   186,   184,   597,   110,   111,   112,   113,  1004,
     138,   799,   140,   137,   164,   120,   203,   122,   123,   124,
     125,    11,    70,   203,   208,   130,    74,   132,   133,   181,
      20,    21,   625,   626,   186,   185,   181,   181,   181,   164,
     186,   186,   186,   186,   675,    93,    94,    95,    96,   164,
     186,   186,   204,   684,    11,   204,   152,   152,   152,   204,
     204,   204,   208,   167,   168,    22,    23,   698,   164,   164,
     164,   702,   208,   208,   204,   157,   182,   164,   671,   209,
     186,   201,   187,   188,   189,   190,   191,   167,   168,   182,
     182,    80,    81,   186,   186,   671,   201,   202,   182,   182,
    1095,   182,   186,   186,   164,   186,   167,   168,   169,   170,
     138,   139,   140,   167,    55,    20,    21,   167,   168,   169,
     110,   111,   112,   113,   114,   164,   179,   117,   118,   119,
     120,   208,   122,   123,   124,   125,   206,   207,   209,   209,
     130,   208,   132,   133,   209,   208,   164,   589,   138,   139,
     140,  1076,   745,   209,   144,   597,   167,   138,   101,    20,
      21,   154,   208,   756,   164,   758,    34,   993,   761,   745,
     736,   764,    34,   766,   767,   204,   769,   209,   164,   621,
     756,   185,    20,    21,   210,   761,   185,   185,   764,   185,
     180,   204,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   185,   185,   992,   185,   799,   112,   113,   185,
     185,   201,   202,   185,   182,   120,   204,   122,   123,   124,
     125,   181,   164,  1148,   208,   130,   164,   164,    21,   671,
    1034,   164,   203,   203,    41,   181,   209,   181,   204,   164,
     209,   164,   131,   204,   203,   164,   877,   164,   204,   842,
     205,   112,   113,   185,   204,   185,   185,   185,   185,   120,
      41,   122,   123,   124,   125,   204,   164,   185,  1187,   130,
     204,   207,   110,   111,   112,   113,   114,   208,   204,   117,
     118,   119,   120,   204,   122,   123,   124,   125,    20,    21,
     204,   733,   130,   204,   132,   133,   201,   202,   204,   202,
     138,   139,   140,   745,   204,   204,   144,   203,   157,   204,
      36,  1236,     9,    64,   756,   208,   758,   208,  1106,   761,
     208,   208,   764,   208,   766,   767,   208,   769,   189,   190,
     191,   208,    41,   186,   204,   154,   164,   164,   164,   126,
     201,   202,   180,    13,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   179,   157,   207,   799,   181,   209,
       7,   164,   186,   201,   202,   164,   186,   204,   186,   178,
     178,   209,   204,   204,   210,   185,     1,    41,   110,   111,
     112,   113,   114,   204,   185,   117,   118,   119,   120,   204,
     122,   123,   124,   125,    65,   208,   164,   203,   130,   992,
     132,   133,   164,   203,  1035,   352,   138,   139,   140,   204,
     204,   203,   144,   204,   361,   204,   164,   164,   164,   185,
     204,   204,   204,   164,    32,   204,    20,    21,   208,   204,
     203,   205,   203,   164,   186,   382,   208,   186,   205,    51,
     203,   210,   203,   208,   203,   392,   614,    73,   180,     1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      41,   126,    76,  1181,    74,  1182,   733,   887,  1182,   201,
     202,   215,    32,  1182,  1105,  1182,   208,  1182,     1,   426,
     427,  1103,  1133,  1186,  1136,   953,  1050,   501,   758,   411,
     437,    49,  1155,  1124,   562,   977,   403,  1128,    58,    59,
     447,   448,   449,   450,   451,   452,   414,   383,   609,   796,
     414,  1203,    -1,  1106,    -1,  1108,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,  1108,    20,    21,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,   494,    -1,    -1,
     992,    -1,    -1,    -1,    -1,    -1,    -1,   504,   118,    -1,
      -1,   508,   122,    -1,   511,   512,    -1,    -1,    -1,    -1,
     517,    -1,   519,    -1,    -1,    -1,   523,    -1,    -1,    -1,
      -1,   528,  1175,    -1,    32,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,  1175,
      -1,    -1,    -1,    -1,    -1,    -1,   553,   201,   202,    -1,
      58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   566,
     180,    -1,    -1,    -1,   184,   112,   113,   187,    -1,    -1,
      -1,  1262,    -1,   120,    -1,    -1,   123,   124,   125,    -1,
      -1,    -1,   202,   130,   591,    -1,    -1,   594,    -1,   596,
      -1,   211,    -1,   600,   601,   602,   603,   604,   605,    -1,
     607,    -1,   609,    -1,  1106,    -1,  1108,    -1,    -1,    -1,
     118,    -1,    -1,   121,   122,    -1,    -1,    -1,    -1,    -1,
     627,   628,    -1,   131,   631,   632,   633,   634,    -1,   636,
      -1,   638,   639,   640,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,   651,   652,   653,   654,   655,   656,
     657,    -1,   659,    -1,   201,   202,    -1,    -1,    -1,   666,
      -1,    20,    21,    -1,    -1,    -1,   673,    -1,    -1,    -1,
     677,   179,   180,  1175,    -1,   682,   184,    -1,    -1,   187,
      -1,    -1,    -1,    -1,   691,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,    -1,    -1,    -1,    -1,    -1,
     208,    -1,    32,   211,  1206,    -1,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,   723,   724,   725,   726,
     727,   728,   729,   730,    -1,    -1,    -1,    -1,    58,    59,
      -1,    -1,    -1,   740,    -1,    -1,    -1,  1239,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,   114,   763,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,   782,   783,    -1,    -1,   138,
     139,   140,    -1,    -1,   791,   144,    -1,   794,   118,   796,
      -1,    -1,   122,    -1,   801,    -1,    -1,    -1,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,    -1,    -1,    -1,    -1,   855,   208,
     180,    -1,   859,    -1,   184,    -1,   186,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   875,   876,
      -1,    -1,   202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   889,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    -1,    -1,    -1,   130,
     131,   132,   133,   134,   135,    -1,    -1,   138,   139,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,   962,   963,   964,    -1,   966,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   974,   179,   180,
     977,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,   990,   991,    -1,    -1,    20,    21,   996,
     201,   202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1006,
      -1,  1008,    -1,  1010,    -1,    -1,    -1,    -1,    -1,  1016,
      -1,    -1,    -1,  1020,    -1,    -1,    -1,    -1,    -1,  1026,
      -1,    -1,    -1,  1030,    -1,    -1,  1033,  1034,    -1,    -1,
      -1,  1038,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,   135,    -1,    -1,   138,
     139,   140,   141,   142,   143,   144,    -1,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,  1114,    -1,  1116,
     179,   180,  1119,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1138,   201,   202,  1141,  1142,  1143,  1144,    -1,   208,
    1147,    -1,    -1,    -1,  1151,  1152,  1153,  1154,    -1,    -1,
    1157,   184,   185,   186,   187,   188,   189,   190,   191,  1166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    32,    -1,    -1,    -1,  1182,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1197,  1198,    -1,    -1,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,  1209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1218,  1219,    -1,    -1,    -1,    -1,     1,    -1,    -1,
      -1,    -1,     6,  1230,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,  1245,    -1,
    1247,    25,    26,    27,    28,  1252,    -1,  1254,  1255,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,   118,    42,    43,
      -1,   122,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    32,    -1,    93,
      94,    95,    96,    -1,    -1,    99,   100,    -1,    -1,   180,
      -1,    -1,    -1,   184,    -1,   186,   187,    -1,   112,   113,
      -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,
      -1,   202,    -1,   127,   128,   129,    -1,    -1,    -1,    -1,
     211,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,   148,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     204,    -1,   206,   207,   208,   209,   210,     1,    -1,    -1,
      -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,   180,    -1,    -1,    -1,   184,
      -1,   186,   187,    37,    38,    -1,    40,    -1,    42,    43,
      -1,    -1,    46,    -1,    48,    49,    50,   202,    52,    53,
      -1,    -1,    56,    57,    -1,    -1,   211,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,   148,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    -1,    -1,    14,    15,    16,    17,    18,
     204,    -1,   206,   207,   208,   209,   210,    26,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    51,    -1,    53,    -1,    -1,    -1,    -1,    58,
      59,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,   184,    -1,    -1,   187,   188,
     189,    -1,   191,    -1,    -1,   194,   195,    14,    15,    16,
      17,    18,    -1,   202,    -1,   204,    -1,   206,   207,    26,
     209,    -1,   211,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,   118,    49,    -1,    -1,   122,    53,    -1,    -1,    -1,
      -1,    58,    59,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   112,   113,   184,    -1,   186,
     187,   118,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,    -1,    20,    21,    -1,
     137,    -1,    -1,    -1,   211,    -1,    -1,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,   184,    -1,    -1,
     187,   188,   189,    -1,   191,    -1,    -1,   194,   195,    14,
      15,    16,    17,    18,    -1,   202,    -1,   204,    -1,   206,
     207,    26,   209,    -1,   211,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,   112,
     113,    46,    -1,    -1,    49,    -1,    -1,   120,    53,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    63,    -1,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,
      -1,    -1,    -1,    -1,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    32,
     145,   146,   147,   148,   149,    -1,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,   194,
     195,    14,    15,    16,    17,    18,    -1,   202,    -1,   204,
      -1,   206,   207,    26,   209,    -1,   211,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,   118,    49,    -1,    -1,   122,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   112,
     113,   184,    -1,   186,   187,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,    -1,    -1,   202,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   211,    -1,
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
      -1,    -1,    -1,   127,   128,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,   148,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    -1,    -1,    -1,    14,    15,    16,    17,    18,
     204,    -1,   206,   207,    -1,   209,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   146,   147,   148,
     149,    -1,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   187,   188,
     189,    -1,   191,    -1,    -1,   194,   195,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   204,    46,   206,   207,    49,
     209,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,   148,   149,
      -1,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   187,   188,   189,
      -1,   191,    -1,    -1,   194,   195,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   204,    46,   206,   207,    49,   209,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,   146,   147,   148,   149,    -1,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,   187,   188,   189,    -1,
     191,    -1,    -1,   194,   195,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,   204,    46,   206,   207,    49,   209,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    90,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,   146,   147,   148,   149,    -1,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   187,   188,   189,    32,   191,
      -1,    -1,   194,   195,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   204,    46,   206,   207,    49,   209,    -1,    -1,
      53,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    32,   145,   146,   147,   148,   149,    -1,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
     184,    -1,   186,   187,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    14,    15,    16,    17,    18,   202,   202,
      -1,   204,   205,   206,   207,    26,   209,   211,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,   118,    49,    -1,
      -1,   122,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,   112,   113,   184,    -1,   186,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
     211,    -1,    -1,    -1,   145,   146,   147,   148,   149,    -1,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,   187,   188,   189,    -1,
     191,    -1,    -1,   194,   195,    -1,    38,    -1,    -1,    -1,
      -1,    43,   203,   204,    46,   206,   207,    49,   209,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,
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
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   187,   188,   189,    -1,   191,
      -1,    -1,   194,   195,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   204,    46,   206,   207,    49,   209,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,   146,   147,   148,   149,    -1,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,    -1,   187,   188,   189,    -1,   191,    -1,    -1,
     194,   195,    14,    15,    16,    17,    18,    -1,    -1,    21,
     204,    -1,   206,   207,    26,   209,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,   147,   148,   149,    -1,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    14,    15,    16,    17,    18,    -1,    -1,
      -1,   204,   205,   206,   207,    26,   209,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,   146,   147,   148,   149,    -1,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,    -1,
     191,    -1,    -1,   194,   195,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   204,   205,   206,   207,    26,   209,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,   187,   188,
     189,    -1,   191,    -1,    -1,   194,   195,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   204,    -1,   206,   207,    26,
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
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,   147,   148,   149,    -1,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,   187,   188,   189,    -1,   191,    -1,
      26,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,   202,
      -1,   204,    38,   206,   207,    -1,   209,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,
      -1,    -1,   194,   195,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   204,   205,   206,   207,    26,   209,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,   146,   147,   148,   149,
      -1,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   187,   188,   189,
      -1,   191,    -1,    -1,   194,   195,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   204,    46,   206,   207,    49,   209,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    90,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   145,   146,   147,   148,   149,    -1,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,   187,   188,   189,    -1,
     191,    -1,    -1,   194,   195,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,   204,    46,   206,   207,    49,   209,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    90,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
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
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,   146,   147,   148,   149,    -1,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,   188,   189,    -1,   191,    -1,
      -1,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   204,    -1,   206,   207,    -1,   209,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,   133,   134,
     135,    -1,    -1,   138,   139,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,   179,   180,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,
      -1,    -1,    62,   208,    -1,    -1,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,   110,
     111,   112,   113,   114,    -1,    -1,   117,   118,   119,   120,
      -1,   122,   123,   124,   125,    20,    21,    -1,    -1,   130,
      -1,   132,   133,    -1,    -1,    -1,    -1,   138,   139,   140,
      -1,    -1,    -1,   144,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,   180,
      -1,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     201,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
      -1,   211,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,   144,
      -1,    -1,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    20,    21,
      -1,    -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,
     138,   139,   140,    -1,    -1,   180,   144,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,   201,   202,    -1,    -1,
     205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     114,    20,    21,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,   203,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,   203,
      -1,   110,   111,   112,   113,   114,    20,    21,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,   203,    -1,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    20,    21,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,   138,
     139,   140,    -1,    -1,    -1,   144,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,   203,
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
      -1,    20,    21,    -1,   138,   139,   140,    -1,    -1,    -1,
     144,   180,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   201,   202,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,    -1,    -1,
      -1,    -1,   130,    -1,   132,   133,    -1,    20,    21,    -1,
     138,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   201,   202,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,   132,
     133,    20,    21,    -1,    -1,   110,   111,   112,   113,   114,
      -1,    -1,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,   114,   201,   202,   117,    -1,
      -1,   120,    -1,   122,   123,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    47,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,   201,   202,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,   150,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    -1,   164,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,   164,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,   164,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
     164,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      66,    -1,    68,    -1,    70,    71,    72,    73,    74,   164,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   213,     0,     6,    29,    31,    33,    39,    48,    54,
      75,    98,   191,   202,   208,   214,   217,   223,   225,   226,
     231,   259,   263,   289,   358,   364,   368,   374,   413,   416,
      18,    19,   164,   252,   253,   254,   157,   232,   233,   164,
     165,   191,   227,   228,   229,   209,   365,   164,   206,   216,
     417,   414,   137,   164,   276,    33,    61,   130,   195,   204,
     255,   256,   257,   258,   276,   208,     4,     5,     7,    35,
     372,    60,   356,   179,   178,   181,   178,   228,    21,    55,
     190,   201,   230,   164,   361,   363,   361,   356,   164,   164,
     164,   137,   224,   257,   257,   257,   204,   138,   139,   140,
     178,   203,    55,    61,   264,   266,    55,    61,   366,    55,
      61,   373,    55,    61,   357,    14,    15,   157,   162,   164,
     166,   204,   219,   253,   157,   233,   164,   227,   227,   362,
     208,   210,   361,   208,    55,    61,   215,   209,   415,   164,
     167,   222,   205,   254,   257,   257,   257,   257,   267,   164,
     367,   375,   209,   359,   167,   168,   218,    14,    15,   157,
     162,   164,   219,   250,   251,   230,   179,   208,   164,   382,
     209,   167,   205,    34,    66,    68,    70,    71,    72,    73,
      74,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    88,    89,    90,    93,    94,    95,    96,   112,
     113,   164,   262,   265,   181,   209,   101,   370,   371,   354,
     154,   332,   167,   168,   169,   178,   205,    24,    30,    47,
      62,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,   150,   276,   378,   380,   381,   383,
     387,   412,   208,   164,   346,   347,    20,    21,    37,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   122,   123,   124,
     131,   132,   133,   134,   135,   138,   139,   140,   141,   142,
     143,   144,   180,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   194,   195,   201,   202,    34,    34,   204,
     260,   209,   268,    70,    74,    93,    94,    95,    96,   379,
     360,   164,   376,   210,   355,   254,   146,   164,   352,   353,
     250,   185,   185,   185,   185,   204,   185,   185,   185,   185,
     185,   185,   204,    32,    58,    59,   118,   122,   180,   184,
     187,   202,   211,   182,   164,   210,   181,   208,   164,   164,
     164,   203,    21,   164,   203,   149,   205,   332,   342,   343,
     181,   261,   270,   209,   164,   208,   210,   181,   369,   209,
     332,   203,   204,    41,   178,   181,   184,   351,   390,   392,
     394,   388,   164,   384,   396,   398,   400,   402,   404,   406,
     408,   410,    14,    15,    16,    17,    18,    26,    38,    43,
      46,    49,    53,    63,    75,    99,   112,   113,   145,   146,
     147,   148,   149,   151,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   165,   166,   187,   188,   189,
     194,   195,   204,   206,   207,   221,   223,   268,   272,   276,
     281,   292,   299,   302,   305,   309,   310,   313,   314,   316,
     321,   324,   331,   378,   418,   427,   431,   433,   435,    32,
     184,    32,   184,   202,   211,   203,   324,   211,   387,   412,
     210,   346,   164,   131,   204,    48,   146,   164,   349,   377,
     205,   208,   412,     1,     8,     9,    10,    12,    25,    27,
      28,    37,    40,    42,    50,    52,    56,    57,    63,   100,
     127,   128,   129,   165,   208,   210,   234,   235,   238,   241,
     242,   244,   245,   246,   247,   248,   269,   271,   275,   277,
     282,   283,   284,   285,   286,   287,   288,   289,   291,   311,
     312,   324,   360,   179,   208,   276,   338,   353,   203,   324,
     164,   164,   387,   121,   131,   179,   350,   412,   386,   412,
     412,   412,   205,   382,   412,   260,   412,   260,   412,   260,
     164,   344,   345,   412,   347,   185,   278,   380,   418,   204,
     185,   204,   185,   185,   204,   185,   204,   185,   324,   324,
     204,   204,   204,   204,   204,   204,    12,   387,    12,   387,
     324,   429,   432,   185,   220,   324,   324,   324,   324,   324,
     324,   207,   164,   204,   259,    20,    21,   110,   111,   112,
     113,   114,   117,   118,   119,   120,   122,   123,   124,   125,
     130,   132,   133,   138,   139,   140,   144,   180,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   201,   202,
     205,   204,   203,   203,   324,   349,   204,    41,   121,   178,
     179,   181,   184,   348,   342,   324,   377,   121,   275,   311,
     324,   268,    59,   324,   324,   164,   208,   157,    56,   324,
     268,   121,   275,   324,   207,   309,   309,   309,   324,    36,
     208,   208,   324,     9,   208,   208,   208,   208,   208,    64,
     290,   126,   208,   102,   103,   104,   105,   106,   107,   108,
     109,   115,   116,   121,   131,   134,   135,   141,   142,   143,
     179,   210,   324,   202,   210,   259,   339,   205,    41,   208,
     350,   275,   324,   186,   186,   208,   186,   186,   186,   208,
     186,   261,   186,   261,   186,   261,   181,   186,   208,   186,
     208,   279,   204,   276,   300,   324,   293,   295,   324,   297,
     324,   387,   324,   324,   324,   324,   324,   324,   377,    51,
     152,   164,   187,   204,   324,   419,   420,   421,   428,   430,
     377,   204,   420,   430,   136,   173,   208,   210,   422,   273,
     169,   170,   218,   205,   146,   151,   185,   276,   315,   202,
     205,   303,   324,   154,   308,    18,   152,   164,   378,    18,
     152,   164,   378,   324,   324,   324,   324,   324,   324,   164,
     324,   152,   164,   324,   324,   324,   324,   324,   324,   324,
     324,   324,    21,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   123,   124,   152,   164,   201,   202,
     322,   324,   205,   303,   205,   324,   164,   164,   412,   275,
     324,   268,    19,   275,   324,   121,   179,    13,   324,   268,
     179,   181,   157,   275,   324,   207,   268,   209,   268,   243,
     353,   309,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   127,   128,   129,   324,   324,   324,   324,   324,
     324,   324,   127,   128,   129,   324,   208,   258,     7,   332,
     337,   164,   275,   324,   208,   391,   393,   412,   395,   389,
     385,   397,   186,   401,   186,   405,   186,   412,   409,   344,
     411,   412,   205,   303,   185,   324,   412,   205,   387,   387,
     205,   387,   205,   186,   205,   205,   205,   205,   205,   205,
      19,   309,   204,   131,   348,   205,   136,   178,   208,   421,
     175,   176,   203,   425,   178,   175,   203,   208,   424,    19,
     205,   421,   174,   210,   423,   324,   429,   210,   387,   324,
     204,   204,   317,   319,   164,   419,   178,   205,   121,   131,
     179,   184,   306,   307,   260,   185,   204,   185,   204,   204,
     204,   203,    18,   152,   164,   378,   181,   152,   164,   324,
     204,   152,   164,   324,     1,   203,   178,   205,   205,    41,
     348,   303,   268,   249,   270,    11,    22,    23,   236,   237,
     324,   309,   309,   309,   309,   309,   309,   203,    55,    61,
     335,    65,   336,   208,   208,   399,   403,   407,   186,   205,
     164,   205,   186,   186,   186,   186,   308,   204,   303,   324,
     324,   324,   420,   421,   324,   152,   164,   419,   425,   203,
     324,   203,   428,   303,   420,   174,   177,   210,   426,   203,
     186,   171,   324,   324,   387,   260,   203,   202,   324,   164,
     164,   164,   164,   178,   203,   261,   325,   324,   327,   324,
     205,   303,   324,   185,   204,   324,   204,   203,   324,   204,
     203,   323,   164,   324,   208,   324,   210,   268,   324,    11,
     239,   208,    45,   336,    44,   101,   333,   280,   186,   208,
     301,   294,   296,   298,   204,   205,   303,   208,   205,   421,
     425,   204,   131,   348,   208,   421,   203,   274,   205,   205,
     186,   261,   205,   419,   306,   203,   136,   268,   304,   387,
     205,   412,   205,   205,   205,   329,   324,   324,   205,   324,
     268,   268,   240,   208,    32,   334,   333,   335,   324,   164,
     324,   324,   324,   205,   324,   205,   324,   131,   348,   425,
     324,   324,   324,   324,   426,   324,   318,   186,   203,   121,
     324,   186,   186,   412,   205,   205,   205,   237,    25,   100,
     241,   282,   283,   284,   286,   324,   340,   334,   349,   205,
     186,   205,   208,   434,   324,   324,   205,   434,   315,   320,
     205,   324,   326,   328,   186,   121,   324,   121,   324,   265,
     341,   324,    51,   203,   131,   348,   210,   315,   330,   324,
     324,   208,   265,   205,   324,   203,   324,   324,   203,   268
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   212,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   214,   215,   215,   215,
     216,   216,   217,   218,   218,   218,   218,   219,   220,   220,
     220,   221,   222,   222,   224,   223,   225,   226,   227,   227,
     228,   228,   228,   228,   229,   229,   230,   230,   231,   232,
     232,   233,   233,   234,   235,   235,   236,   236,   237,   237,
     237,   238,   238,   239,   240,   239,   241,   241,   241,   241,
     241,   242,   243,   242,   244,   245,   246,   247,   249,   248,
     250,   250,   250,   250,   250,   250,   251,   251,   252,   252,
     252,   253,   253,   253,   253,   253,   253,   253,   253,   254,
     254,   255,   255,   255,   256,   256,   257,   257,   257,   257,
     257,   257,   257,   258,   258,   259,   259,   260,   260,   260,
     261,   261,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   263,   264,   264,
     264,   265,   267,   266,   268,   268,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   270,   270,   270,
     271,   273,   274,   272,   275,   275,   275,   275,   276,   276,
     276,   277,   277,   279,   280,   278,   278,   281,   281,   281,
     281,   282,   283,   284,   284,   284,   285,   285,   285,   286,
     286,   287,   287,   287,   288,   289,   289,   290,   290,   291,
     293,   294,   292,   295,   296,   292,   297,   298,   292,   300,
     301,   299,   302,   302,   302,   303,   303,   304,   304,   304,
     305,   305,   305,   306,   306,   306,   306,   307,   307,   308,
     308,   309,   309,   310,   310,   310,   310,   310,   310,   310,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   312,
     312,   312,   312,   312,   312,   313,   313,   314,   314,   315,
     315,   316,   317,   318,   316,   319,   320,   316,   321,   321,
     322,   323,   321,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   325,   326,   324,   324,   324,   324,   327,   328,
     324,   324,   324,   329,   330,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   332,   332,   333,   333,   333,
     334,   334,   335,   335,   335,   336,   336,   337,   338,   339,
     338,   340,   338,   341,   338,   338,   342,   342,   342,   343,
     343,   344,   344,   345,   345,   346,   347,   347,   348,   348,
     349,   349,   349,   349,   349,   349,   350,   350,   350,   351,
     351,   352,   352,   352,   352,   352,   353,   353,   353,   353,
     353,   354,   355,   354,   356,   356,   357,   357,   357,   358,
     359,   358,   360,   360,   360,   360,   362,   361,   363,   363,
     364,   365,   364,   366,   366,   366,   367,   368,   368,   369,
     369,   370,   370,   371,   372,   372,   373,   373,   373,   375,
     376,   374,   377,   377,   377,   377,   377,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   378,   379,   379,   379,   379,   379,   379,
     380,   381,   381,   381,   382,   382,   384,   385,   383,   386,
     386,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   388,
     389,   387,   387,   390,   391,   387,   392,   393,   387,   394,
     395,   387,   387,   396,   397,   387,   398,   399,   387,   387,
     400,   401,   387,   402,   403,   387,   387,   404,   405,   387,
     406,   407,   387,   408,   409,   387,   410,   411,   387,   412,
     412,   412,   414,   415,   413,   417,   416,   418,   418,   418,
     418,   419,   419,   419,   419,   419,   419,   419,   419,   420,
     420,   421,   421,   422,   422,   423,   423,   424,   424,   425,
     425,   425,   426,   426,   426,   427,   427,   427,   427,   427,
     427,   428,   428,   428,   429,   429,   430,   430,   431,   431,
     432,   432,   433,   434,   434,   435,   435
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     0,     1,     1,
       1,     1,     4,     1,     1,     2,     2,     3,     0,     2,
       4,     3,     1,     2,     0,     4,     2,     2,     1,     1,
       1,     2,     3,     3,     2,     4,     0,     1,     2,     1,
       3,     1,     3,     3,     3,     2,     1,     1,     0,     2,
       4,     1,     1,     0,     0,     3,     1,     1,     1,     1,
       1,     4,     0,     6,     6,     2,     3,     3,     0,     5,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     1,     5,     1,
       3,     1,     1,     1,     1,     4,     1,     2,     3,     3,
       3,     3,     2,     1,     3,     0,     3,     0,     2,     3,
       0,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     3,     2,     2,
       3,     4,     3,     2,     2,     2,     2,     2,     3,     3,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       1,     3,     0,     4,     3,     7,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     2,     2,     2,     0,     2,     2,
       3,     0,     0,     7,     3,     2,     2,     2,     1,     3,
       2,     2,     3,     0,     0,     5,     1,     2,     4,     5,
       2,     1,     1,     1,     2,     3,     2,     2,     3,     2,
       3,     2,     2,     3,     4,     1,     1,     1,     0,     3,
       0,     0,     7,     0,     0,     7,     0,     0,     7,     0,
       0,     6,     5,     8,    10,     1,     3,     1,     2,     3,
       1,     1,     2,     2,     2,     2,     2,     1,     3,     0,
       4,     1,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       4,     4,     4,     4,     4,     6,     8,     5,     6,     1,
       4,     3,     0,     0,     8,     0,     0,     9,     3,     4,
       0,     0,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     3,     4,     5,     4,     5,     3,     4,
       1,     3,     4,     3,     4,     2,     4,     4,     7,     8,
       3,     5,     0,     0,     8,     3,     3,     3,     0,     0,
       8,     3,     4,     0,     0,     9,     4,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     2,     4,     1,     4,
       4,     4,     4,     4,     1,     6,     7,     6,     6,     7,
       7,     6,     7,     6,     6,     0,     4,     0,     1,     1,
       0,     1,     0,     1,     1,     0,     1,     5,     0,     0,
       4,     0,     9,     0,    10,     5,     2,     3,     4,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     1,
       1,     2,     3,     5,     3,     3,     1,     1,     1,     0,
       1,     1,     4,     3,     3,     5,     4,     6,     5,     5,
       4,     0,     0,     4,     0,     1,     0,     1,     1,     6,
       0,     6,     0,     2,     3,     5,     0,     4,     2,     3,
       4,     0,     4,     0,     1,     1,     1,     7,     9,     0,
       2,     0,     1,     3,     1,     1,     0,     1,     1,     0,
       0,     9,     1,     4,     3,     3,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     1,     3,     0,     0,     6,     1,
       3,     1,     1,     1,     1,     4,     3,     4,     2,     2,
       3,     2,     3,     2,     2,     3,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     6,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     1,
       3,     3,     0,     0,     8,     0,     7,     1,     1,     1,
       1,     3,     3,     5,     5,     6,     6,     8,     8,     1,
       3,     0,     2,     2,     1,     2,     1,     2,     1,     2,
       1,     1,     2,     1,     1,     5,     4,     6,     7,     5,
       7,     1,     3,     3,     3,     1,     1,     3,     4,     4,
       1,     3,     3,     0,     3,    10,    10
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

  case 16: /* top_level_reader_macro: expr_reader ';'  */
                             {
        delete (yyvsp[-1].pExpression);    // we do nothing, we don't even attemp to 'visit'
    }
    break;

  case 17: /* optional_public_or_private_module: %empty  */
                        { (yyval.b) = yyextra->g_Program->policies.default_module_public; }
    break;

  case 18: /* optional_public_or_private_module: "public"  */
                        { (yyval.b) = true; }
    break;

  case 19: /* optional_public_or_private_module: "private"  */
                        { (yyval.b) = false; }
    break;

  case 20: /* module_name: '$'  */
                    { (yyval.s) = new string("$"); }
    break;

  case 21: /* module_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 22: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module  */
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

  case 23: /* character_sequence: STRING_CHARACTER  */
                                                            { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 24: /* character_sequence: STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 25: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 26: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 27: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 28: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 29: /* string_builder_body: string_builder_body character_sequence  */
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

  case 30: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 31: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 32: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 33: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 34: /* $@1: %empty  */
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

  case 35: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
                                     {
        yyextra->g_ReaderExpr->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = yyextra->g_ReaderExpr;
        int thisLine = 0;
        if ( auto seqt = yyextra->g_ReaderMacro->suffix(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, thisLine, tokAt(scanner,(yylsp[0]))) ) {
            das_accept_sequence(scanner,seqt,strlen(seqt),thisLine);
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

  case 36: /* options_declaration: "options" annotation_argument_list  */
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

  case 38: /* keyword_or_name: "name"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 39: /* keyword_or_name: "keyword"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 40: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 41: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 42: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 43: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 44: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 45: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 46: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 47: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 51: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 52: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 53: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 54: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 55: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 56: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 57: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 58: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 59: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 60: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 61: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 62: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 63: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 64: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 65: /* expression_else_one_liner: "else" $@2 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 66: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 67: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 69: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 70: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 71: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 72: /* $@3: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 73: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner ';'  */
                                                                                                                                                   {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 74: /* expression_for_loop: "for" variable_name_with_pos_list "in" expr_list ';' expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 75: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 76: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 77: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 78: /* $@4: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 79: /* expression_with_alias: "assume" "name" '=' $@4 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 80: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 81: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 82: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 83: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 84: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 85: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 86: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 87: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 88: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 89: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 90: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 91: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 92: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 93: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 94: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 95: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 96: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 97: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 98: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 99: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 100: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 101: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 102: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 103: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 104: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
    break;

  case 105: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 106: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 107: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 108: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 109: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 110: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 111: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 112: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 113: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 114: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 115: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 116: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 117: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 118: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 119: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 120: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 121: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 122: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 123: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 124: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 125: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 126: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 127: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 128: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 129: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 130: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 131: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 132: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 133: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 134: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 135: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 136: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 137: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 138: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 139: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 140: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 141: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 142: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 143: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 144: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 145: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 146: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 147: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 148: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 149: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 150: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 151: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 152: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 153: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 154: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 155: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 156: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 157: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 158: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 159: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 160: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 161: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 162: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 163: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 164: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 165: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 166: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 167: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 168: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 169: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 170: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 171: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 172: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 173: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 174: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 175: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 176: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 177: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 178: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 179: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 180: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 181: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 182: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 183: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 184: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 185: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 186: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 187: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 188: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 189: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 190: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 191: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 192: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 193: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 194: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 195: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 196: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 197: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 198: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 199: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 200: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 201: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 202: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 203: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 204: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 205: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 206: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 207: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 208: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 209: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 210: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 211: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 212: /* $@5: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 213: /* function_declaration: optional_public_or_private_function $@5 function_declaration_header expression_block  */
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

  case 214: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 215: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 216: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 217: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 218: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 219: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 220: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 221: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 222: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 226: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 227: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 229: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 230: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 235: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 236: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 237: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 238: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 239: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 240: /* expr_keyword: "keyword" expr expression_block  */
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

  case 241: /* $@6: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 242: /* $@7: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 243: /* expression_keyword: "keyword" '<' $@6 type_declaration_no_options '>' $@7 expr  */
                                                                                                                                               {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),*(yyvsp[-6].s));
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCall->arguments.push_back(ExpressionPtr(td));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 244: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 245: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 246: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 247: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 248: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 249: /* name_in_namespace: "name" "::" "name"  */
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

  case 250: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 251: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 252: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 253: /* $@8: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 254: /* $@9: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 255: /* new_type_declaration: '<' $@8 type_declaration '>' $@9  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 256: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 257: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 258: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 259: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 260: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 261: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 262: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 263: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 264: /* expression_return_no_pipe: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 265: /* expression_return_no_pipe: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 266: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 267: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 268: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 269: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 270: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 271: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 272: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 273: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 274: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 275: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 276: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 277: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 278: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 279: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 280: /* $@10: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 281: /* $@11: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 282: /* expr_cast: "cast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 283: /* $@12: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 284: /* $@13: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 285: /* expr_cast: "upcast" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 286: /* $@14: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 287: /* $@15: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 288: /* expr_cast: "reinterpret" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 289: /* $@16: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 290: /* $@17: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 291: /* expr_type_decl: "type" '<' $@16 type_declaration '>' $@17  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 292: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 293: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 294: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 295: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 296: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 297: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 298: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 299: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 300: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 301: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 302: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 303: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 304: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 305: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 306: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 307: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 308: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 309: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 310: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 311: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 312: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 313: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 314: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 315: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 316: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 317: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 318: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 319: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 320: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 321: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 325: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 326: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 335: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr_assign_pipe: expr '=' "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 344: /* expr_assign_pipe: expr "<-" "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 345: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 346: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 347: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 348: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 349: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 350: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 351: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 352: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 353: /* $@19: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 354: /* func_addr_expr: '@' '@' '<' $@18 type_declaration_no_options '>' $@19 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 355: /* $@20: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 356: /* $@21: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 357: /* func_addr_expr: '@' '@' '<' $@20 optional_function_argument_list optional_function_type '>' $@21 func_addr_name  */
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

  case 358: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 359: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 360: /* $@22: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 361: /* $@23: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 362: /* expr_field: expr '.' $@22 error $@23  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 363: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 364: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 365: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 366: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 367: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 368: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 369: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 370: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 371: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 372: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 373: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 399: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 402: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 403: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 404: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 405: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 406: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 407: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 408: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 409: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 410: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 411: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 412: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 413: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 414: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 415: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 416: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 417: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 418: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 419: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 420: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 421: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 422: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 423: /* $@25: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 424: /* expr: expr "is" "type" '<' $@24 type_declaration_no_options '>' $@25  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 425: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 426: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 427: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 428: /* $@26: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 429: /* $@27: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 430: /* expr: expr "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 431: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 432: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 433: /* $@28: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 434: /* $@29: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 435: /* expr: expr '?' "as" "type" '<' $@28 type_declaration '>' $@29  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 436: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 437: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 438: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 439: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 440: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 442: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 443: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 444: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 445: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 446: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 447: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 448: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 449: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 450: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 451: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 452: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 453: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 454: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 455: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 456: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 457: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 458: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 459: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 460: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 461: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 462: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 463: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 464: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 465: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 466: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 467: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 468: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 469: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 470: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 471: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 472: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 473: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 474: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 475: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 476: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 477: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 478: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 479: /* $@30: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 480: /* struct_variable_declaration_list: struct_variable_declaration_list $@30 structure_variable_declaration ';'  */
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

  case 481: /* $@31: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 482: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@31 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 483: /* $@32: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 484: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@32 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 485: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 486: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 487: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 488: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 489: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 490: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 491: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 492: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 493: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 494: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 495: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 496: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 497: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 498: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 499: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 500: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 501: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 502: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 503: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 504: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 505: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 506: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 507: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 508: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 509: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 510: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 511: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 512: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 513: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 514: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 515: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 516: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 517: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 518: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 519: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 520: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 521: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 522: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 523: /* global_variable_declaration_list: global_variable_declaration_list $@33 optional_field_annotation let_variable_declaration  */
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

  case 524: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 525: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 526: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 527: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 528: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 529: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 530: /* $@34: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 531: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@34 optional_field_annotation let_variable_declaration  */
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

  case 532: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 533: /* enum_list: enum_list ';'  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 534: /* enum_list: enum_list "name" ';'  */
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

  case 535: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 536: /* $@35: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 537: /* single_alias: "name" $@35 '=' type_declaration  */
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

  case 541: /* $@36: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 543: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 544: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 545: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 546: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 547: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 548: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 549: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 550: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 551: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 552: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 553: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 554: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 555: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 556: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 557: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 558: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 559: /* $@37: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 560: /* $@38: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 561: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@37 structure_name $@38 '{' struct_variable_declaration_list '}'  */
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

  case 562: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 563: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 564: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 565: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 566: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 567: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 568: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 569: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 570: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 571: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 572: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 573: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 574: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 575: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 576: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 577: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 578: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 579: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 580: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 581: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 582: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 583: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 584: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 585: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 586: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 587: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 588: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 589: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 590: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 591: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 592: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 593: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 594: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 595: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 596: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 597: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 598: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 599: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 600: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 601: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 602: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 603: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 604: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 605: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 606: /* $@39: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 607: /* $@40: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 608: /* bitfield_type_declaration: "bitfield" '<' $@39 bitfield_bits '>' $@40  */
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

  case 609: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 610: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 611: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 612: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 613: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 614: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 615: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
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

  case 616: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 617: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 618: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 619: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 620: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 621: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 622: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 623: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 624: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 625: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 626: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 627: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 628: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 629: /* $@41: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 630: /* $@42: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 631: /* type_declaration_no_options: "smart_ptr" '<' $@41 type_declaration '>' $@42  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 632: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 633: /* $@43: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 634: /* $@44: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 635: /* type_declaration_no_options: "array" '<' $@43 type_declaration '>' $@44  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 636: /* $@45: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 637: /* $@46: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 638: /* type_declaration_no_options: "table" '<' $@45 table_type_pair '>' $@46  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 639: /* $@47: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 640: /* $@48: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 641: /* type_declaration_no_options: "iterator" '<' $@47 type_declaration '>' $@48  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 642: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 643: /* $@49: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 644: /* $@50: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 645: /* type_declaration_no_options: "block" '<' $@49 type_declaration '>' $@50  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 646: /* $@51: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 647: /* $@52: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 648: /* type_declaration_no_options: "block" '<' $@51 optional_function_argument_list optional_function_type '>' $@52  */
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

  case 649: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 650: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 651: /* $@54: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 652: /* type_declaration_no_options: "function" '<' $@53 type_declaration '>' $@54  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 653: /* $@55: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 654: /* $@56: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 655: /* type_declaration_no_options: "function" '<' $@55 optional_function_argument_list optional_function_type '>' $@56  */
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

  case 656: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 657: /* $@57: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 658: /* $@58: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 659: /* type_declaration_no_options: "lambda" '<' $@57 type_declaration '>' $@58  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 660: /* $@59: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 661: /* $@60: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 662: /* type_declaration_no_options: "lambda" '<' $@59 optional_function_argument_list optional_function_type '>' $@60  */
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

  case 663: /* $@61: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 664: /* $@62: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 665: /* type_declaration_no_options: "tuple" '<' $@61 tuple_type_list '>' $@62  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 666: /* $@63: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 667: /* $@64: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 668: /* type_declaration_no_options: "variant" '<' $@63 variant_type_list '>' $@64  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 669: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 670: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 671: /* type_declaration: type_declaration '|' '#'  */
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

  case 672: /* $@65: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 673: /* $@66: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 674: /* variant_alias_declaration: "variant" $@65 "name" $@66 '{' variant_type_list ';' '}'  */
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

  case 675: /* $@67: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 676: /* bitfield_alias_declaration: "bitfield" $@67 "name" '{' bitfield_bits ';' '}'  */
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

  case 677: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 678: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 679: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 680: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 681: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 682: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 683: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 684: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 685: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 686: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 687: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 688: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 689: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 690: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 691: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 692: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 705: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 706: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 707: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 708: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 709: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 710: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 711: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 712: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 713: /* make_tuple: make_tuple ',' expr  */
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

  case 714: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 715: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 716: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 717: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 718: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
       ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
       (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
       (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 719: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
       ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
       (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 720: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 721: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 722: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 723: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 724: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 725: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 726: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


