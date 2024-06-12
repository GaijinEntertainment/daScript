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
    void das_accept_sequence ( yyscan_t yyscanner, const char * seq, size_t seqLen, int lineNo, FileInfo * info );

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
  YYSYMBOL_DAS_FIXED_ARRAY = 66,           /* "fixed_array"  */
  YYSYMBOL_DAS_TBOOL = 67,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 68,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 69,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 70,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 71,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 72,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 73,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 74,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 75,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 76,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 77,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 78,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 79,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 80,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 81,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 82,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 83,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 84,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 85,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 86,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 87,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 88,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 89,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 90,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 91,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 92,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 93,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 94,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 95,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 96,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 97,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 98,                /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 99,              /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 100,            /* "generator"  */
  YYSYMBOL_DAS_YIELD = 101,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 102,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 103,                   /* "+="  */
  YYSYMBOL_SUBEQU = 104,                   /* "-="  */
  YYSYMBOL_DIVEQU = 105,                   /* "/="  */
  YYSYMBOL_MULEQU = 106,                   /* "*="  */
  YYSYMBOL_MODEQU = 107,                   /* "%="  */
  YYSYMBOL_ANDEQU = 108,                   /* "&="  */
  YYSYMBOL_OREQU = 109,                    /* "|="  */
  YYSYMBOL_XOREQU = 110,                   /* "^="  */
  YYSYMBOL_SHL = 111,                      /* "<<"  */
  YYSYMBOL_SHR = 112,                      /* ">>"  */
  YYSYMBOL_ADDADD = 113,                   /* "++"  */
  YYSYMBOL_SUBSUB = 114,                   /* "--"  */
  YYSYMBOL_LEEQU = 115,                    /* "<="  */
  YYSYMBOL_SHLEQU = 116,                   /* "<<="  */
  YYSYMBOL_SHREQU = 117,                   /* ">>="  */
  YYSYMBOL_GREQU = 118,                    /* ">="  */
  YYSYMBOL_EQUEQU = 119,                   /* "=="  */
  YYSYMBOL_NOTEQU = 120,                   /* "!="  */
  YYSYMBOL_RARROW = 121,                   /* "->"  */
  YYSYMBOL_LARROW = 122,                   /* "<-"  */
  YYSYMBOL_QQ = 123,                       /* "??"  */
  YYSYMBOL_QDOT = 124,                     /* "?."  */
  YYSYMBOL_QBRA = 125,                     /* "?["  */
  YYSYMBOL_LPIPE = 126,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 127,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 128,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 129,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 130,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 131,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 132,                 /* ":="  */
  YYSYMBOL_ROTL = 133,                     /* "<<<"  */
  YYSYMBOL_ROTR = 134,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 135,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 136,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 137,                    /* "=>"  */
  YYSYMBOL_COLCOL = 138,                   /* "::"  */
  YYSYMBOL_ANDAND = 139,                   /* "&&"  */
  YYSYMBOL_OROR = 140,                     /* "||"  */
  YYSYMBOL_XORXOR = 141,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 142,                /* "&&="  */
  YYSYMBOL_OROREQU = 143,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 144,                /* "^^="  */
  YYSYMBOL_DOTDOT = 145,                   /* ".."  */
  YYSYMBOL_MTAG_E = 146,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 147,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 148,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 149,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 150,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 151,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 152,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 153,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 154,           /* "..."  */
  YYSYMBOL_BRABRAB = 155,                  /* "[["  */
  YYSYMBOL_BRACBRB = 156,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 157,                  /* "{{"  */
  YYSYMBOL_INTEGER = 158,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 159,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 160,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 161,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 162,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 163,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 164,                   /* "double constant"  */
  YYSYMBOL_NAME = 165,                     /* "name"  */
  YYSYMBOL_KEYWORD = 166,                  /* "keyword"  */
  YYSYMBOL_BEGIN_STRING = 167,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 168,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 169,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 170,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 171,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 172,          /* "}"  */
  YYSYMBOL_END_OF_READ = 173,              /* "end of failed eader macro"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 174,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 175,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 176,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 177,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 178,            /* ",}]"  */
  YYSYMBOL_179_ = 179,                     /* ','  */
  YYSYMBOL_180_ = 180,                     /* '='  */
  YYSYMBOL_181_ = 181,                     /* '?'  */
  YYSYMBOL_182_ = 182,                     /* ':'  */
  YYSYMBOL_183_ = 183,                     /* '|'  */
  YYSYMBOL_184_ = 184,                     /* '^'  */
  YYSYMBOL_185_ = 185,                     /* '&'  */
  YYSYMBOL_186_ = 186,                     /* '<'  */
  YYSYMBOL_187_ = 187,                     /* '>'  */
  YYSYMBOL_188_ = 188,                     /* '-'  */
  YYSYMBOL_189_ = 189,                     /* '+'  */
  YYSYMBOL_190_ = 190,                     /* '*'  */
  YYSYMBOL_191_ = 191,                     /* '/'  */
  YYSYMBOL_192_ = 192,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 193,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 194,               /* UNARY_PLUS  */
  YYSYMBOL_195_ = 195,                     /* '~'  */
  YYSYMBOL_196_ = 196,                     /* '!'  */
  YYSYMBOL_PRE_INC = 197,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 198,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 199,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 200,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 201,                    /* DEREF  */
  YYSYMBOL_202_ = 202,                     /* '.'  */
  YYSYMBOL_203_ = 203,                     /* '['  */
  YYSYMBOL_204_ = 204,                     /* ']'  */
  YYSYMBOL_205_ = 205,                     /* '('  */
  YYSYMBOL_206_ = 206,                     /* ')'  */
  YYSYMBOL_207_ = 207,                     /* '$'  */
  YYSYMBOL_208_ = 208,                     /* '@'  */
  YYSYMBOL_209_ = 209,                     /* ';'  */
  YYSYMBOL_210_ = 210,                     /* '{'  */
  YYSYMBOL_211_ = 211,                     /* '}'  */
  YYSYMBOL_212_ = 212,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 213,                 /* $accept  */
  YYSYMBOL_program = 214,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 215,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 216, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 217,              /* module_name  */
  YYSYMBOL_module_declaration = 218,       /* module_declaration  */
  YYSYMBOL_character_sequence = 219,       /* character_sequence  */
  YYSYMBOL_string_constant = 220,          /* string_constant  */
  YYSYMBOL_string_builder_body = 221,      /* string_builder_body  */
  YYSYMBOL_string_builder = 222,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 223, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 224,              /* expr_reader  */
  YYSYMBOL_225_1 = 225,                    /* $@1  */
  YYSYMBOL_options_declaration = 226,      /* options_declaration  */
  YYSYMBOL_require_declaration = 227,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 228,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 229,      /* require_module_name  */
  YYSYMBOL_require_module = 230,           /* require_module  */
  YYSYMBOL_is_public_module = 231,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 232,       /* expect_declaration  */
  YYSYMBOL_expect_list = 233,              /* expect_list  */
  YYSYMBOL_expect_error = 234,             /* expect_error  */
  YYSYMBOL_expression_label = 235,         /* expression_label  */
  YYSYMBOL_expression_goto = 236,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 237,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 238,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 239,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 240, /* expression_else_one_liner  */
  YYSYMBOL_241_2 = 241,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 242,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 243,  /* expression_if_then_else  */
  YYSYMBOL_244_3 = 244,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 245,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 246,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 247,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 248,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 249,    /* expression_with_alias  */
  YYSYMBOL_250_4 = 250,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 251, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 252, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 253, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 254,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 255, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 256, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 257, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 258,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 259,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 260, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 261, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 262,   /* optional_function_type  */
  YYSYMBOL_function_name = 263,            /* function_name  */
  YYSYMBOL_global_function_declaration = 264, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 265, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 266, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 267,     /* function_declaration  */
  YYSYMBOL_268_5 = 268,                    /* $@5  */
  YYSYMBOL_expression_block = 269,         /* expression_block  */
  YYSYMBOL_expression_any = 270,           /* expression_any  */
  YYSYMBOL_expressions = 271,              /* expressions  */
  YYSYMBOL_expr_keyword = 272,             /* expr_keyword  */
  YYSYMBOL_expression_keyword = 273,       /* expression_keyword  */
  YYSYMBOL_274_6 = 274,                    /* $@6  */
  YYSYMBOL_275_7 = 275,                    /* $@7  */
  YYSYMBOL_expr_pipe = 276,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 277,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 278,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 279,     /* new_type_declaration  */
  YYSYMBOL_280_8 = 280,                    /* $@8  */
  YYSYMBOL_281_9 = 281,                    /* $@9  */
  YYSYMBOL_expr_new = 282,                 /* expr_new  */
  YYSYMBOL_expression_break = 283,         /* expression_break  */
  YYSYMBOL_expression_continue = 284,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 285, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 286,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 287, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 288,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 289,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 290,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 291,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 292,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 293, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 294,           /* expression_let  */
  YYSYMBOL_expr_cast = 295,                /* expr_cast  */
  YYSYMBOL_296_10 = 296,                   /* $@10  */
  YYSYMBOL_297_11 = 297,                   /* $@11  */
  YYSYMBOL_298_12 = 298,                   /* $@12  */
  YYSYMBOL_299_13 = 299,                   /* $@13  */
  YYSYMBOL_300_14 = 300,                   /* $@14  */
  YYSYMBOL_301_15 = 301,                   /* $@15  */
  YYSYMBOL_expr_type_decl = 302,           /* expr_type_decl  */
  YYSYMBOL_303_16 = 303,                   /* $@16  */
  YYSYMBOL_304_17 = 304,                   /* $@17  */
  YYSYMBOL_expr_type_info = 305,           /* expr_type_info  */
  YYSYMBOL_expr_list = 306,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 307,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 308,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 309,            /* capture_entry  */
  YYSYMBOL_capture_list = 310,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 311,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 312,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 313,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 314,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 315,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 316,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 317,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 318,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 319,           /* func_addr_expr  */
  YYSYMBOL_320_18 = 320,                   /* $@18  */
  YYSYMBOL_321_19 = 321,                   /* $@19  */
  YYSYMBOL_322_20 = 322,                   /* $@20  */
  YYSYMBOL_323_21 = 323,                   /* $@21  */
  YYSYMBOL_expr_field = 324,               /* expr_field  */
  YYSYMBOL_325_22 = 325,                   /* $@22  */
  YYSYMBOL_326_23 = 326,                   /* $@23  */
  YYSYMBOL_expr = 327,                     /* expr  */
  YYSYMBOL_328_24 = 328,                   /* $@24  */
  YYSYMBOL_329_25 = 329,                   /* $@25  */
  YYSYMBOL_330_26 = 330,                   /* $@26  */
  YYSYMBOL_331_27 = 331,                   /* $@27  */
  YYSYMBOL_332_28 = 332,                   /* $@28  */
  YYSYMBOL_333_29 = 333,                   /* $@29  */
  YYSYMBOL_expr_mtag = 334,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 335, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 336,        /* optional_override  */
  YYSYMBOL_optional_constant = 337,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 338, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 339, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 340, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 341, /* struct_variable_declaration_list  */
  YYSYMBOL_342_30 = 342,                   /* $@30  */
  YYSYMBOL_343_31 = 343,                   /* $@31  */
  YYSYMBOL_344_32 = 344,                   /* $@32  */
  YYSYMBOL_function_argument_declaration = 345, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 346,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 347,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 348,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 349,             /* variant_type  */
  YYSYMBOL_variant_type_list = 350,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 351,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 352,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 353,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 354,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 355,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 356, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 357, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 358, /* global_variable_declaration_list  */
  YYSYMBOL_359_33 = 359,                   /* $@33  */
  YYSYMBOL_optional_shared = 360,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 361, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 362,               /* global_let  */
  YYSYMBOL_363_34 = 363,                   /* $@34  */
  YYSYMBOL_enum_list = 364,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 365, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 366,             /* single_alias  */
  YYSYMBOL_367_35 = 367,                   /* $@35  */
  YYSYMBOL_alias_list = 368,               /* alias_list  */
  YYSYMBOL_alias_declaration = 369,        /* alias_declaration  */
  YYSYMBOL_370_36 = 370,                   /* $@36  */
  YYSYMBOL_optional_public_or_private_enum = 371, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 372,                /* enum_name  */
  YYSYMBOL_enum_declaration = 373,         /* enum_declaration  */
  YYSYMBOL_374_37 = 374,                   /* $@37  */
  YYSYMBOL_375_38 = 375,                   /* $@38  */
  YYSYMBOL_376_39 = 376,                   /* $@39  */
  YYSYMBOL_377_40 = 377,                   /* $@40  */
  YYSYMBOL_optional_structure_parent = 378, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 379,          /* optional_sealed  */
  YYSYMBOL_structure_name = 380,           /* structure_name  */
  YYSYMBOL_class_or_struct = 381,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 382, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 383,    /* structure_declaration  */
  YYSYMBOL_384_41 = 384,                   /* $@41  */
  YYSYMBOL_385_42 = 385,                   /* $@42  */
  YYSYMBOL_variable_name_with_pos_list = 386, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 387,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 388, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 389, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 390,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 391,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 392,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 393, /* bitfield_type_declaration  */
  YYSYMBOL_394_43 = 394,                   /* $@43  */
  YYSYMBOL_395_44 = 395,                   /* $@44  */
  YYSYMBOL_table_type_pair = 396,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 397, /* type_declaration_no_options  */
  YYSYMBOL_398_45 = 398,                   /* $@45  */
  YYSYMBOL_399_46 = 399,                   /* $@46  */
  YYSYMBOL_400_47 = 400,                   /* $@47  */
  YYSYMBOL_401_48 = 401,                   /* $@48  */
  YYSYMBOL_402_49 = 402,                   /* $@49  */
  YYSYMBOL_403_50 = 403,                   /* $@50  */
  YYSYMBOL_404_51 = 404,                   /* $@51  */
  YYSYMBOL_405_52 = 405,                   /* $@52  */
  YYSYMBOL_406_53 = 406,                   /* $@53  */
  YYSYMBOL_407_54 = 407,                   /* $@54  */
  YYSYMBOL_408_55 = 408,                   /* $@55  */
  YYSYMBOL_409_56 = 409,                   /* $@56  */
  YYSYMBOL_410_57 = 410,                   /* $@57  */
  YYSYMBOL_411_58 = 411,                   /* $@58  */
  YYSYMBOL_412_59 = 412,                   /* $@59  */
  YYSYMBOL_413_60 = 413,                   /* $@60  */
  YYSYMBOL_414_61 = 414,                   /* $@61  */
  YYSYMBOL_415_62 = 415,                   /* $@62  */
  YYSYMBOL_416_63 = 416,                   /* $@63  */
  YYSYMBOL_417_64 = 417,                   /* $@64  */
  YYSYMBOL_418_65 = 418,                   /* $@65  */
  YYSYMBOL_419_66 = 419,                   /* $@66  */
  YYSYMBOL_420_67 = 420,                   /* $@67  */
  YYSYMBOL_421_68 = 421,                   /* $@68  */
  YYSYMBOL_type_declaration = 422,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 423, /* variant_alias_declaration  */
  YYSYMBOL_424_69 = 424,                   /* $@69  */
  YYSYMBOL_425_70 = 425,                   /* $@70  */
  YYSYMBOL_426_71 = 426,                   /* $@71  */
  YYSYMBOL_427_72 = 427,                   /* $@72  */
  YYSYMBOL_bitfield_alias_declaration = 428, /* bitfield_alias_declaration  */
  YYSYMBOL_429_73 = 429,                   /* $@73  */
  YYSYMBOL_430_74 = 430,                   /* $@74  */
  YYSYMBOL_431_75 = 431,                   /* $@75  */
  YYSYMBOL_432_76 = 432,                   /* $@76  */
  YYSYMBOL_make_decl = 433,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 434,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 435,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 436,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 437, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 438, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 439, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 440, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 441, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_make_struct_decl = 442,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 443,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 444,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 445,          /* make_tuple_call  */
  YYSYMBOL_make_dim = 446,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 447,            /* make_dim_decl  */
  YYSYMBOL_448_77 = 448,                   /* $@77  */
  YYSYMBOL_449_78 = 449,                   /* $@78  */
  YYSYMBOL_450_79 = 450,                   /* $@79  */
  YYSYMBOL_451_80 = 451,                   /* $@80  */
  YYSYMBOL_make_table = 452,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 453,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 454,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 455, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 456       /* array_comprehension  */
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
#define YYLAST   12086

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  213
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  244
/* YYNRULES -- Number of rules.  */
#define YYNRULES  765
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1353

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   440


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
       2,     2,     2,   196,     2,   212,   207,   192,   185,     2,
     205,   206,   190,   189,   179,   188,   202,   191,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   182,   209,
     186,   180,   187,   181,   208,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   203,     2,   204,   184,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   210,   183,   211,   195,     2,     2,     2,
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
     175,   176,   177,   178,   193,   194,   197,   198,   199,   200,
     201
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   510,   510,   511,   516,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   531,   537,   538,   539,
     543,   544,   548,   566,   567,   568,   569,   573,   577,   582,
     591,   599,   615,   620,   628,   628,   667,   694,   698,   699,
     703,   706,   710,   716,   725,   728,   734,   735,   739,   743,
     744,   748,   751,   757,   763,   766,   772,   773,   777,   778,
     779,   788,   789,   793,   794,   794,   800,   801,   802,   803,
     804,   808,   814,   814,   820,   826,   834,   844,   853,   853,
     860,   861,   862,   863,   864,   865,   869,   874,   882,   883,
     884,   888,   889,   890,   891,   892,   893,   894,   895,   901,
     904,   910,   911,   912,   916,   924,   937,   940,   948,   959,
     970,   981,   984,   991,   995,  1002,  1003,  1007,  1008,  1009,
    1013,  1016,  1023,  1027,  1028,  1029,  1030,  1031,  1032,  1033,
    1034,  1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,
    1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,
    1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,
    1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,
    1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,
    1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,
    1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,
    1104,  1105,  1106,  1107,  1108,  1109,  1110,  1115,  1137,  1138,
    1139,  1143,  1149,  1149,  1166,  1170,  1181,  1182,  1183,  1184,
    1185,  1186,  1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,
    1195,  1196,  1197,  1198,  1199,  1200,  1201,  1205,  1210,  1216,
    1222,  1233,  1233,  1233,  1244,  1278,  1281,  1284,  1290,  1291,
    1302,  1306,  1309,  1317,  1317,  1317,  1320,  1326,  1329,  1332,
    1336,  1342,  1346,  1350,  1353,  1356,  1364,  1367,  1370,  1378,
    1381,  1389,  1392,  1395,  1403,  1409,  1410,  1414,  1415,  1419,
    1424,  1432,  1438,  1450,  1453,  1459,  1459,  1459,  1462,  1462,
    1462,  1467,  1467,  1467,  1475,  1475,  1475,  1481,  1491,  1502,
    1517,  1520,  1526,  1527,  1534,  1545,  1546,  1547,  1551,  1552,
    1553,  1554,  1558,  1563,  1571,  1572,  1576,  1581,  1588,  1589,
    1590,  1591,  1592,  1593,  1594,  1598,  1599,  1600,  1601,  1602,
    1603,  1604,  1605,  1606,  1607,  1608,  1609,  1610,  1611,  1612,
    1613,  1614,  1615,  1616,  1620,  1621,  1622,  1623,  1624,  1625,
    1629,  1636,  1648,  1653,  1663,  1667,  1674,  1677,  1677,  1677,
    1682,  1682,  1682,  1695,  1699,  1703,  1703,  1703,  1710,  1711,
    1712,  1713,  1714,  1715,  1716,  1717,  1718,  1719,  1720,  1721,
    1722,  1723,  1724,  1725,  1726,  1727,  1728,  1729,  1730,  1731,
    1732,  1733,  1734,  1735,  1736,  1737,  1738,  1739,  1740,  1741,
    1742,  1743,  1744,  1745,  1751,  1752,  1753,  1754,  1755,  1756,
    1757,  1758,  1759,  1760,  1761,  1762,  1763,  1767,  1771,  1774,
    1777,  1778,  1779,  1780,  1783,  1786,  1787,  1790,  1790,  1790,
    1793,  1798,  1802,  1806,  1806,  1806,  1811,  1814,  1818,  1818,
    1818,  1823,  1826,  1827,  1828,  1829,  1830,  1831,  1832,  1833,
    1834,  1836,  1840,  1841,  1846,  1850,  1851,  1852,  1853,  1854,
    1855,  1856,  1860,  1864,  1868,  1872,  1876,  1880,  1884,  1888,
    1892,  1899,  1900,  1904,  1905,  1906,  1910,  1911,  1915,  1916,
    1917,  1921,  1922,  1926,  1937,  1940,  1940,  1959,  1958,  1973,
    1972,  1985,  1994,  2000,  2005,  2015,  2016,  2020,  2023,  2032,
    2033,  2037,  2046,  2047,  2051,  2054,  2057,  2071,  2072,  2076,
    2082,  2088,  2091,  2095,  2101,  2110,  2111,  2112,  2116,  2117,
    2121,  2128,  2133,  2142,  2148,  2159,  2162,  2167,  2172,  2180,
    2191,  2194,  2194,  2214,  2215,  2219,  2220,  2221,  2225,  2228,
    2228,  2247,  2250,  2253,  2268,  2287,  2288,  2289,  2294,  2294,
    2316,  2317,  2321,  2322,  2322,  2326,  2327,  2328,  2332,  2342,
    2347,  2342,  2359,  2364,  2359,  2379,  2380,  2384,  2385,  2389,
    2395,  2396,  2400,  2401,  2402,  2406,  2411,  2406,  2425,  2432,
    2437,  2446,  2452,  2463,  2464,  2465,  2466,  2467,  2468,  2469,
    2470,  2471,  2472,  2473,  2474,  2475,  2476,  2477,  2478,  2479,
    2480,  2481,  2482,  2483,  2484,  2485,  2486,  2487,  2488,  2489,
    2493,  2494,  2495,  2496,  2497,  2498,  2502,  2513,  2517,  2524,
    2536,  2543,  2552,  2557,  2560,  2573,  2573,  2573,  2586,  2590,
    2597,  2598,  2599,  2600,  2601,  2615,  2621,  2625,  2629,  2634,
    2639,  2644,  2649,  2653,  2657,  2662,  2666,  2670,  2675,  2675,
    2675,  2681,  2688,  2688,  2688,  2693,  2693,  2693,  2699,  2699,
    2699,  2704,  2708,  2708,  2708,  2713,  2713,  2713,  2722,  2726,
    2726,  2726,  2731,  2731,  2731,  2740,  2744,  2744,  2744,  2749,
    2749,  2749,  2758,  2758,  2758,  2764,  2764,  2764,  2773,  2776,
    2787,  2803,  2803,  2808,  2814,  2803,  2839,  2839,  2844,  2849,
    2839,  2879,  2880,  2881,  2882,  2883,  2887,  2894,  2901,  2907,
    2913,  2920,  2927,  2933,  2943,  2948,  2955,  2956,  2961,  2962,
    2966,  2967,  2971,  2972,  2976,  2977,  2978,  2982,  2983,  2984,
    2989,  2995,  3002,  3010,  3017,  3025,  3037,  3040,  3046,  3060,
    3066,  3072,  3081,  3086,  3093,  3098,  3105,  3113,  3113,  3113,
    3121,  3128,  3128,  3128,  3138,  3143,  3150,  3153,  3159,  3168,
    3176,  3184,  3197,  3198,  3202,  3205
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
  "\"static\"", "\"fixed_array\"", "\"bool\"", "\"void\"", "\"string\"",
  "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"", "\"uint\"",
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
  "kwd_let", "optional_in_scope", "tuple_expansion",
  "tuple_expansion_variable_declaration", "expression_let", "expr_cast",
  "$@10", "$@11", "$@12", "$@13", "$@14", "$@15", "expr_type_decl", "$@16",
  "$@17", "expr_type_info", "expr_list", "block_or_simple_block",
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
  "tuple_type_list", "variant_type", "variant_type_list",
  "variant_alias_type_list", "copy_or_move", "variable_declaration",
  "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@33", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@34", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@35", "alias_list",
  "alias_declaration", "$@36", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "$@37", "$@38", "$@39", "$@40",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@41", "$@42", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_alias_bits", "bitfield_type_declaration", "$@43", "$@44",
  "table_type_pair", "type_declaration_no_options", "$@45", "$@46", "$@47",
  "$@48", "$@49", "$@50", "$@51", "$@52", "$@53", "$@54", "$@55", "$@56",
  "$@57", "$@58", "$@59", "$@60", "$@61", "$@62", "$@63", "$@64", "$@65",
  "$@66", "$@67", "$@68", "type_declaration", "variant_alias_declaration",
  "$@69", "$@70", "$@71", "$@72", "bitfield_alias_declaration", "$@73",
  "$@74", "$@75", "$@76", "make_decl", "make_struct_fields",
  "make_struct_dim", "optional_block",
  "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "make_struct_decl", "make_tuple",
  "make_map_tuple", "make_tuple_call", "make_dim", "make_dim_decl", "$@77",
  "$@78", "$@79", "$@80", "make_table", "expr_map_tuple_list",
  "make_table_decl", "array_comprehension_where", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1129)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-680)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1129,    66, -1129, -1129,    57,    -3,   229,  -130, -1129,    58,
     414,   414,    30,    35, -1129, -1129, -1129,    51, -1129, -1129,
   -1129,   345, -1129,   249, -1129, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129,   123, -1129,   180,   226,   247, -1129, -1129,
   -1129,   229, -1129,    14, -1129,   414,   414, -1129, -1129,   249,
   -1129, -1129, -1129, -1129,   299,   344, -1129, -1129, -1129,    35,
      35,    35,   307, -1129,   495,   -29, -1129, -1129, -1129, -1129,
     450,   460,   490, -1129,   508,    25,    57,   419,    -3,   318,
     433, -1129,   364,   364, -1129,   439,   411,    10,   451,   513,
     449,   467, -1129,   478,   480, -1129, -1129,   -78,    57,    35,
      35,    35,    35, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
     503, -1129, -1129, -1129, -1129, -1129,   464, -1129, -1129, -1129,
   -1129, -1129,   458,    92, -1129, -1129, -1129, -1129,   621, -1129,
   -1129, -1129, -1129, -1129,   475, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129,   517, -1129,    18, -1129,   534,   547,   495,
   11921, -1129,   -11,   585, -1129,   533, -1129, -1129,   497, -1129,
   -1129, -1129, -1129, -1129, -1129, -1129,    83, -1129,   510, -1129,
     481,   482, -1129, -1129, 10482, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
     659,   660, -1129,   491,   487,   401, -1129, -1129,   535, -1129,
     488,    57,    15, -1129, -1129, -1129,    92, -1129, 11733, -1129,
   -1129,   536,   537, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129,   539,   499, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129,   684, -1129, -1129,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
     541,   504, -1129, -1129,   -71,   525, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129, -1129,   500, -1129,   527,   501, -1129,   533,
      64,   507,   672,   382, -1129, -1129,   528,   529,   530,   531,
   -1129, -1129, -1129,   515, -1129, -1129, -1129, -1129, -1129,   545,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
   -1129,   548, -1129, -1129, -1129,   549,   550, -1129, -1129, -1129,
   -1129,   552,   553,   542, -1129, -1129, -1129, -1129, -1129,  7943,
     559, -1129, -1129, -1129, -1129, -1129, -1129,   568,   612, -1129,
     544, -1129,    -6, -1129,   386, 11733, -1129,  1736, -1129,   -92,
      30, -1129, -1129,    15,   551,  7218,   581,   587, 11733, -1129,
     -24, -1129, -1129, -1129, -1129,   589, -1129,   286,   317,   319,
   -1129, -1129,  7218, -1129, -1129, -1129,     2, -1129, -1129, -1129,
      32,  4061, -1129,  7758,   -88,   -80, -1129, -1129,  7218,   170,
     555,   715, -1129,   -55, -1129,   423,   559, -1129,  7218, -1129,
   -1129,   170, -1129, -1129,   362,   556,   571,    43,  2742, -1129,
   -1129,   487,   228,  4246,   558,  7218,   593,   586,   588,   562,
   -1129,   564,   590,   613,  4431,   381,   323, -1129,   570,   592,
    2927,  7218,  7218,   437,   437,   437,   574,   575,   576,   577,
     578,   579, -1129, 11376, 11475,  7218, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129,  4616, -1129,  7218,  7218,  7218,  7218,  7218,
    7218, -1129,   582, -1129,   755, -1129, -1129,   584,   591,  7218,
     788, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129,   -33,   595, -1129,   596,   597,   598, -1129,   599,
   -1129, -1129,   734, -1129, -1129, -1129, -1129,   606, -1129, -1129,
     -83, -1129, -1129, -1129, -1129, -1129,  1174, -1129,   594, -1129,
   -1129, -1129, -1129, -1129, -1129, -1129,  -106, -1129,   601, -1129,
      23, -1129, -1129,   605,   615,  7750, -1129,   761,   391, -1129,
   -1129, -1129,  3309, 11733, 11733, 11733, 11733,   609,   652, 11733,
     491, 11733,   491, 11733,   491, 11832,   653,  7865, -1129, -1129,
   -1129, -1129,   616, -1129, -1129,  9943, -1129,  7943,   610, -1129,
     611,   639, -1129,   618,   614,  7900, -1129,  7218,   664, -1129,
     666, -1129, 11733, -1129,  3309, -1129,  1369,    17, -1129,   619,
   -1129, -1129,    30, -1129, -1129,  7218,  3309, -1129,   654,   707,
   10373,   822, 11733,  7218,  7218, 10840,  7218,  1369,   656, -1129,
   -1129,  7218, -1129, -1129,   655,   680, 10840,  7218, -1129, -1129,
    7218,  7218, 11733,  3309, -1129, 10373,   285,   285,   632, -1129,
   -1129, -1129,  7218,  7218,  7218,  7218,  7218,  7218,   170,  2145,
     170,  2344, 10517, -1129,    62, -1129,  1369,   470,   285,   285,
     -65,   285,   285,  7996,   -34,   638, -1129, -1129,  1369, -1129,
   -1129,  3864, -1129, -1129, -1129, -1129, -1129, -1129,   361,   694,
     437, -1129, 11508, 11603,  7218,  7218,  7218,  7218,  7218,  7218,
    7218,  7218,  7218,  7218, -1129, -1129,  7218,  7218,  7218,  7218,
    7218,  7218,   685,  3494,  7218,    11,  7218,  7218,  7218,  7218,
    7218,  7218,  7218,  7218,  7218,  7218,  7218,  7218,  7218,  7218,
    7218,  3679,  4801,  7218,  7218,  7218,  7218,  7218,  7218,  7218,
    7218,  7218,  7218,   -42,  7218,  4986,   -92,  7218, -1129, -1129,
      35, -1129,   845,   533, -1129,   692, -1129,  3309, -1129,  7326,
      80,   671,    63,   291,   355, -1129, -1129,   106,   369,   525,
     410,   525,   418,   525,   -36, -1129,   261,   559, -1129,   280,
   -1129, -1129, -1129, -1129, -1129, 11733, -1129, -1129, -1129,  8092,
   -1129,   821,   -44, -1129, 10373, -1129,  7218, 11733,  5171,  5356,
   11733, 11733,   148, 10840, -1129,   654,  7218,  7218,  7218,   487,
     421, -1129,   154, 10840,  8127, -1129, -1129, 11733, 11733,  8223,
   11733, -1129, -1129,  8319, 11733,   160,   176,   434, -1129, 10373,
   -1129,  8354,  8450,  8546,  8581,  8677,  8773,    19,   437,   661,
      47,  2543,  5541, 10613,   686,   -18,   143,   688,    98,    39,
    5726,   -18,    79,  7218, -1129,  7218,   658, -1129, 11733, -1129,
   -1129,  7218,   486, -1129,   673,   674,   320, -1129, -1129, -1129,
     524,  7218,   111, -1129,   182,   711, -1129, -1129,   -31,   491,
   -1129,   691,   675, -1129, -1129,   695,   677, -1129, -1129, 10840,
   10840, 10840, 10840, 10840, 10840, 10840, 10840,    89,    89,   730,
   10840, 10840,   730,  1136,  1136,   678,   437,   437,   437, 10840,
     173,   696, -1129, 10039,   -69,   -69,   594, 10840,    89,    89,
   10840, 10840, 11195, 11067, 11109, 10840, 10840, 10840, 10936,   437,
     437,   437, 10840, 11634, 10709, 11224,  1406,   774,   730,   730,
     187,   187,   173,   173,   173,   188,  7218,   697, -1129,   192,
    7218,   889, 10135, -1129,   190,   693,  1525,   167,   520,   826,
     687, -1129, -1129,  7433, -1129, -1129, -1129, 11733, -1129, -1129,
   -1129,   738, -1129,   719, -1129,   723, -1129,   724, 11733, -1129,
   11832, -1129,   653,   559, -1129,   748,  7218,  -128,   424, -1129,
     218,   749,  8808,   429,  1018, -1129, 10840, -1129,   710, 11733,
    7218, -1129, -1129,  7218,  1213,  2066, -1129,  2484, -1129,  2924,
   -1129, -1129,   694, -1129, -1129, -1129, -1129, -1129,   712,  7218,
   -1129,  7218,  7218,  7218,    -8,  7218,   264,   111,   143, -1129,
   -1129,   720, -1129,  7218, -1129,   721,  7218, -1129,  7218,   111,
      74, -1129,   722, -1129, 10840, -1129, -1129,  3861, 10744,  7218,
    7218, 11733,   491,  1947,   487, -1129, -1129,  7218, -1129,   538,
     -10,   179,  5911, -1129, -1129,   243,   751,   758,   763,   764,
   -1129,   255,   525, -1129,  7218, -1129,  7218,  6108, -1129, -1129,
   -1129,  7218, -1129, -1129, -1129, -1129,   744,   726, -1129, -1129,
    7218,   729, -1129, 10170,  7218,   731, -1129, 10266, -1129, -1129,
   -1129, -1129, -1129,   728, -1129, -1129,   390, -1129,    13, -1129,
   -1129, -1129, -1129,   559, -1129, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129, -1129,   559, -1129, -1129, -1129, -1129, -1129,
   10840,   487, -1129, -1129,   294, -1129, -1129, -1129,  7218,  5908,
   -1129, 10840, -1129, -1129, -1129, -1129,   733,  6293,  -109,  8904,
   10840, 10840,   -18,   143, 10840,   735,    45,   686, -1129, -1129,
   10840, -1129,   688,   -93,   -18, -1129, -1129,   737, -1129, -1129,
   -1129, -1129,  9000,  9035,  7215,   525, -1129, -1129,  1369, -1129,
     736,   743,   111,   777,   739, -1129, -1129, -1129, -1129,   -31,
     740,   -91, 11733,  9131, 11733,  9227, -1129,   225,  9262, -1129,
    7218, 10971,  7218, -1129,  9358,  7218, -1129, -1129, -1129,   907,
      13, -1129, -1129,   520, -1129, -1129, -1129, -1129, -1129,  7218,
     785, -1129,   746,   234,   747,  7218,  7218,  7218,   750,  6478,
   -1129,   239,  7218,   115,   143, -1129,  7218,  7218,  7218,  7218,
      74, -1129,  7218, -1129, -1129, -1129,   766,   524,  3124, -1129,
   -1129,   267, -1129,   446, -1129, -1129,  6663, -1129, -1129,  7292,
   -1129,   430, -1129, -1129, -1129, 11733,  9454,  9489, -1129,  9585,
   -1129, -1129,   907,   170,  9681,   767,  7218, -1129,  7218,   730,
     730,   730,  7218, -1129,  9716, -1129,  7470,  7218,  7218, -1129,
    9812, 10840, 10840,  7470, -1129,   730,   265, -1129, -1129,  6848,
    7033, -1129, -1129, -1129, -1129, -1129, 10840,   752, 11733,   -24,
    7218, 10936, -1129, -1129,   442, -1129, -1129, -1129, 11921, -1129,
   -1129, -1129,  7218,   246,   248,   259, -1129,   917,   768, 10840,
   10840,   134,   759, -1129,   265,  7218,  7218, 10840, -1129,  1016,
    7218, 10971, -1129, -1129, -1129,   762, 11921,  9908, -1129, -1129,
   -1129,  7218,   769,  7218,  7218,   770, -1129, 10840,  7218,  7577,
   -1129, -1129,   487, -1129, 10840, -1129, 10840, 10840, -1129,  7612,
   -1129, -1129, -1129
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   115,     1,   275,     0,     0,     0,   553,   276,     0,
     545,   545,     0,     0,    15,    14,     3,     0,    10,     9,
       8,     0,     7,   533,     6,    11,     5,     4,    12,    13,
      89,    90,    88,    97,    99,    36,    51,    48,    49,    38,
      39,     0,    40,    46,    37,   545,   545,    21,    20,   533,
     547,   546,   696,   691,     0,   248,    34,   102,   103,     0,
       0,     0,   104,   106,   113,     0,   101,    16,   571,   570,
     208,   555,   572,   534,   535,     0,     0,     0,     0,    41,
       0,    47,     0,     0,    44,     0,     0,   545,     0,    17,
       0,     0,   250,     0,     0,   112,   107,     0,     0,     0,
       0,     0,     0,   116,   210,   209,   212,   207,   557,   556,
       0,   574,   573,   575,   537,   536,   539,    95,    96,    93,
      94,    92,     0,     0,    91,   100,    52,    50,    46,    43,
      42,   548,   550,   552,     0,   554,    18,    19,    22,   697,
     692,   249,    32,    35,   111,     0,   108,   109,   110,   114,
       0,   558,     0,   567,   530,   471,    23,    24,     0,    84,
      85,    82,    83,    81,    80,    86,     0,    45,     0,   551,
       0,     0,    33,   105,     0,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
       0,     0,   122,   117,     0,     0,   559,   568,     0,   576,
     531,     0,     0,    25,    26,    27,     0,    98,     0,   698,
     693,   176,   177,   174,   125,   126,   128,   127,   129,   130,
     131,   132,   158,   159,   156,   157,   149,   160,   161,   150,
     147,   148,   175,   169,     0,   173,   162,   163,   164,   165,
     136,   137,   138,   133,   134,   135,   146,     0,   152,   153,
     151,   144,   145,   140,   139,   141,   142,   143,   124,   123,
     168,     0,   154,   155,   471,   120,   237,   213,   610,   613,
     611,   614,   612,   615,     0,   541,   565,     0,   538,   471,
       0,     0,   520,   518,   540,    87,     0,     0,     0,     0,
     583,   603,   584,   617,   585,   589,   590,   591,   592,   609,
     596,   597,   598,   599,   600,   601,   602,   604,   605,   606,
     607,   661,   588,   595,   608,   668,   675,   586,   593,   587,
     594,     0,     0,     0,   616,   630,   633,   631,   632,   688,
     549,   622,   504,   178,   179,   172,   167,   180,   170,   166,
       0,   118,     0,   495,     0,     0,   211,     0,   562,   560,
       0,   569,   484,     0,     0,     0,     0,     0,     0,   519,
       0,   652,   655,   658,   648,     0,   625,   662,   669,   676,
     682,   685,     0,   638,   643,   637,     0,   651,   647,   640,
       0,     0,   642,     0,   699,   694,   181,   171,     0,     0,
       0,   578,   492,   509,   119,   471,   121,   239,     0,    61,
      62,     0,   374,   375,     0,     0,     0,     0,   263,   368,
     261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,   609,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   460,     0,     0,     0,   318,   320,   319,   321,
     322,   323,   324,     0,    28,     0,     0,     0,     0,     0,
       0,   305,   306,   216,   214,   372,   371,     0,     0,     0,
       0,   232,   227,   224,   223,   225,   226,   316,   238,   218,
     454,   217,   369,     0,   445,    69,    70,    67,   230,    68,
     231,   233,   278,   222,   444,   443,   442,   115,   448,   370,
       0,   219,   447,   446,   415,   376,   325,   377,     0,   373,
     701,   705,   702,   703,   704,   541,     0,   542,     0,   566,
     485,   532,   472,     0,     0,     0,   522,   523,     0,   516,
     517,   515,     0,     0,     0,     0,     0,     0,     0,     0,
     117,     0,   117,     0,   117,     0,     0,     0,   645,   646,
     639,   641,     0,   644,   635,     0,   690,   689,     0,   623,
       0,     0,   505,     0,     0,     0,   493,     0,     0,   508,
       0,   507,     0,   510,     0,   496,     0,     0,   253,   257,
     256,   260,     0,   294,   747,     0,     0,   267,   264,     0,
     300,     0,     0,     0,     0,   251,     0,     0,     0,   285,
     288,     0,   236,   291,     0,     0,    55,     0,    75,   751,
       0,     0,     0,     0,   272,   269,   404,   405,   306,   247,
     245,   246,     0,     0,     0,     0,     0,     0,     0,   716,
       0,     0,   740,   754,     0,   241,     0,     0,   381,   380,
     420,   379,   378,     0,   307,     0,   234,   235,     0,    72,
     452,     0,   221,   228,   229,   266,   271,   277,     0,   314,
       0,   220,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   406,   407,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   365,     0,     0,   563,     0,   543,   561,
       0,   577,     0,   471,   521,     0,   525,     0,   529,   325,
       0,     0,   628,     0,     0,   618,   620,     0,     0,   120,
       0,   120,     0,   120,   248,   499,     0,   497,   502,     0,
     619,   636,   634,   624,   700,     0,   506,   695,   494,     0,
     580,   581,   511,   514,   513,    76,     0,     0,     0,     0,
       0,     0,     0,   300,   268,   265,     0,     0,     0,     0,
       0,   756,     0,   252,     0,    77,    78,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,     0,   273,   270,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     248,     0,     0,   736,   714,   716,     0,   742,     0,     0,
       0,   716,     0,     0,   719,     0,     0,   758,     0,   240,
      31,     0,    29,   408,     0,     0,   357,   354,   356,   237,
      58,     0,     0,   416,     0,     0,   284,   283,     0,   117,
     244,     0,     0,   431,   430,     0,     0,   432,   436,   335,
     336,   338,   337,   339,   329,   330,   331,   382,   383,   395,
     340,   341,   396,   393,   394,     0,     0,     0,     0,   327,
     425,     0,   413,     0,   449,   450,   451,   328,   384,   385,
     342,   343,   400,   401,   402,   332,   333,   334,   403,     0,
       0,     0,   326,     0,     0,   398,   399,   397,   391,   392,
     387,   386,   388,   389,   390,     0,     0,     0,   363,     0,
       0,     0,     0,   418,     0,     0,     0,     0,   478,   481,
       0,   524,   527,   325,   528,   653,   656,     0,   659,   649,
     626,     0,   663,     0,   670,     0,   677,     0,     0,   683,
       0,   686,     0,   501,   579,     0,     0,     0,     0,   258,
       0,     0,     0,     0,     0,   746,   301,   274,     0,     0,
       0,   759,   421,     0,     0,     0,   422,     0,   453,     0,
     750,   741,   314,   455,   456,   457,   458,   459,     0,     0,
     717,     0,     0,     0,   716,     0,     0,     0,     0,   725,
     726,     0,   731,     0,   723,     0,     0,   744,     0,     0,
       0,   721,     0,   745,   739,   755,   718,     0,     0,     0,
       0,     0,   117,     0,     0,    56,    57,     0,    71,    63,
       0,     0,     0,   417,   279,     0,     0,     0,     0,     0,
     312,     0,   120,   427,     0,   433,     0,     0,   349,   347,
     348,     0,   411,   346,   344,   345,     0,     0,   437,   441,
       0,     0,   414,     0,     0,     0,   364,     0,   366,   409,
     419,   564,   544,   116,   479,   480,   481,   482,   473,   486,
     526,   654,   657,   629,   660,   650,   627,   621,   664,   666,
     671,   673,   678,   680,   498,   684,   500,   687,   503,   582,
     512,     0,   254,   259,     0,   297,   295,   748,     0,     0,
     757,    79,   286,   289,   292,   752,     0,     0,     0,     0,
     707,   706,   716,     0,   737,     0,     0,   715,   730,   724,
     738,   722,   743,     0,   716,   728,   729,     0,   734,   720,
     242,    30,     0,     0,     0,   120,   215,    59,     0,    64,
       0,     0,     0,     0,     0,   310,   311,   309,   308,     0,
       0,     0,     0,     0,     0,     0,   352,     0,     0,   438,
       0,   426,     0,   412,     0,     0,   410,   367,   491,   476,
     473,   474,   475,   478,   667,   674,   681,    74,   255,     0,
       0,   296,     0,     0,     0,     0,     0,     0,     0,     0,
     461,     0,     0,     0,     0,   732,     0,     0,     0,     0,
       0,   727,     0,   355,   470,   358,     0,    58,     0,    73,
     350,     0,   280,   518,   313,   315,     0,   302,   317,     0,
     469,     0,   467,   353,   464,     0,     0,     0,   463,     0,
     477,   487,   476,     0,     0,     0,     0,   760,     0,   287,
     290,   293,     0,   423,     0,   462,   762,     0,     0,   733,
       0,   709,   708,   762,   735,   243,     0,   361,    60,   263,
       0,    65,    69,    70,    67,    68,    66,     0,     0,     0,
       0,   303,   428,   434,     0,   468,   466,   465,     0,   489,
     483,   298,     0,     0,     0,     0,   424,     0,     0,   711,
     710,     0,     0,   359,     0,     0,     0,   269,   351,     0,
       0,   304,   429,   435,   439,     0,     0,     0,   749,   761,
     753,     0,     0,     0,     0,     0,   362,   270,     0,     0,
     440,   488,     0,   299,   763,   764,   713,   712,   765,     0,
     282,   490,   281
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1129, -1129, -1129, -1129, -1129, -1129,   328,   903, -1129, -1129,
   -1129,   978, -1129, -1129, -1129,   472,   939, -1129,   853, -1129,
   -1129,   905, -1129, -1129, -1129,  -243, -1129, -1129, -1129,  -242,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,   771, -1129,
   -1129,   909,   -48, -1129, -1129,   223,   258,  -156,  -538,  -736,
   -1129, -1129, -1129, -1056, -1129, -1129,  -202, -1129,   141, -1129,
   -1129, -1129, -1129,  -395,   -12, -1129, -1129, -1129, -1129, -1129,
    -237,  -236,  -234, -1129,  -233, -1129, -1129,   995, -1129, -1129,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129,  -567, -1129, -1129,  -172, -1129,     6,  -425,
   -1129,  -351, -1129, -1129, -1129, -1128, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129, -1129,   405, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129,  -145,  -191,  -252,  -190,   -84, -1129, -1129, -1129,
   -1129, -1129,   600, -1129,    44, -1129,  -547, -1129, -1129,  -399,
    -394,  -530,  -227, -1129,  -342, -1129, -1129,   958, -1129, -1129,
   -1129,   485,   670,    97, -1129, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129,  -387,  -215, -1129,   603, -1129, -1129, -1129,
   -1129, -1129, -1129, -1129,  -361, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,  -181,
   -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129, -1129,
     604,  -825,  -619,  -800, -1129, -1129, -1129,  -982,  -208, -1129,
      -2,  -444, -1129,   372, -1129, -1129, -1129, -1129, -1129, -1129,
   -1077, -1129,  -258, -1129
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    15,   138,    49,    16,   158,   164,   647,   475,
     143,   476,    94,    18,    19,    42,    43,    44,    84,    20,
      37,    38,   477,   478,  1037,  1038,   479,  1160,  1228,   480,
     481,   851,   482,   483,   484,   485,   486,   983,   165,   166,
      33,    34,    35,    62,    63,    64,    65,    21,   275,   356,
     203,    22,   106,   204,   107,   150,   487,   488,   357,   489,
     490,   838,  1222,   491,   492,   493,   589,   777,  1198,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   668,  1045,
     856,   503,   504,   797,  1205,   798,  1206,   800,  1207,   505,
     780,  1201,   506,   598,  1238,   507,  1050,  1051,   859,   508,
     509,   599,   511,   512,   513,   848,   514,  1031,  1276,  1032,
    1314,   515,   931,  1187,   783,  1172,  1322,  1174,  1323,  1245,
    1340,   517,   352,  1193,  1251,  1086,  1088,   940,   530,   733,
    1298,  1326,   353,   354,   755,   756,   573,   759,   395,  1003,
     402,   542,   370,   293,   294,   210,   289,    74,   116,    24,
     155,   359,    85,    86,   168,    87,    25,    46,   110,   152,
      26,   285,   528,   525,   935,   361,   208,   209,    72,   113,
      27,   153,   287,   403,   518,   284,   336,   337,   747,   394,
     338,   548,  1096,   741,   339,   546,  1095,   543,  1091,   544,
    1092,   545,  1094,   549,  1098,   550,  1194,   551,  1100,   552,
    1195,   553,  1102,   554,  1196,   555,  1105,   556,  1107,   757,
      28,    91,   171,   342,   574,    29,    90,   170,   341,   570,
     519,   824,   825,   826,   837,  1023,  1017,  1012,  1148,   520,
     827,   791,   521,   828,   522,   781,  1202,   804,  1208,   644,
     792,   523,  1308,   524
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      56,    66,   277,   335,   584,   576,   510,   538,   737,   758,
     212,   643,   749,   953,   751,   955,   753,   957,   629,   630,
     631,   531,   831,   597,   587,  1008,  1138,  1041,   782,   785,
    -115,  1020,   567,   818,   558,    80,   776,   340,   999,   117,
     118,  1203,   399,   818,   670,   624,  1236,    66,    66,    66,
     145,   786,   692,   805,   806,   695,   696,  1191,  1018,   695,
     696,    99,   100,   101,   560,    50,     2,   579,    57,    81,
     786,    51,     3,   526,   727,    30,    31,   568,   579,   350,
      45,  1111,   925,   926,   211,   571,   786,    66,    66,    66,
      66,  1046,   639,   641,   854,     4,    58,     5,   539,     6,
    1212,  1047,    93,   728,    54,     7,   159,   160,   540,   672,
     673,   927,   579,   844,     8,  1192,  1219,   527,   845,   276,
       9,   569,  1002,   928,   580,   581,   671,   582,   144,   572,
     583,    55,   660,   723,   724,   351,   581,   723,   724,   393,
     335,   400,    10,    88,   363,   819,   958,   738,  1313,  1048,
     102,  1215,   846,   335,  1049,    36,   541,  1040,   934,   401,
     929,   930,   291,   290,   891,    11,    59,   579,    54,   579,
     581,   205,   661,    54,   406,   103,   892,  1217,   335,  1002,
     292,  1304,  1137,   119,   134,    93,  1336,   559,   120,   773,
     121,  1007,   122,   672,   673,    55,   580,    76,   580,   206,
      55,   784,   684,   685,  1133,    82,   334,   672,   673,   967,
     692,   970,   694,   695,   696,   697,    83,   561,   580,   601,
     698,   133,    32,    47,   173,   581,   730,   581,   808,   594,
     123,    60,  1269,   618,   731,   562,   834,   579,   335,   335,
      61,   790,  1325,    76,   563,   860,   393,  1267,   595,  1145,
     161,   817,  1146,   829,  1021,   162,   579,   163,    12,   122,
      67,   807,   216,   393,   819,    48,  1333,   945,   364,    13,
    1342,   835,   947,   836,  1014,    14,  1040,   718,   719,   720,
     721,   722,    95,    96,    97,  1147,   684,   685,  1016,   217,
    1022,   723,   724,   950,   692,   581,   694,   695,   696,   697,
     684,   685,  1015,    75,   698,   672,   673,  1016,   692,    73,
     694,   695,   696,   697,   581,   951,  1171,   400,   698,  1009,
    1010,  1052,   146,   147,   148,   149,   857,   786,   335,   335,
     335,   335,  1214,   980,   335,   401,   335,  1231,   335,   786,
     335,  1071,   942,   334,  1220,  1075,   102,  1011,   529,    68,
      69,   669,    70,  1072,   975,   786,   334,  1076,  1006,    76,
     981,  1042,   740,   742,   743,   744,   990,   335,   748,   786,
     750,  1083,   752,   966,   732,   723,   724,   720,   721,   722,
      71,   334,   991,  1161,   775,  1132,   417,   335,  1043,   723,
     724,  1025,   422,  1000,    39,    40,  1080,   786,   684,   685,
    1144,   772,   334,    54,   786,   795,   692,   335,    77,   695,
     696,   697,   844,   980,   602,  1108,   698,  1135,   786,  1226,
     974,    41,  1163,   383,  1113,   786,    78,   980,   436,  1136,
      55,  1243,  1128,   603,  1169,  1189,   984,   985,   786,   987,
    1257,   334,   334,   989,   839,  1265,  1006,  1164,   959,   384,
     385,  1143,  1328,   383,  1329,  1087,   850,   864,   868,  1170,
     438,  1058,  1059,  1060,    92,  1330,   383,   961,  -665,    50,
     960,  1287,   278,  -665,   393,    51,   279,  1027,   948,   384,
     385,  1199,    93,   896,  1063,  1064,  1065,   723,   724,   962,
    1177,  -665,   384,   385,  1155,   280,   281,   282,   283,  -672,
      54,  -679,  -360,  1200,  -672,   104,  -679,  -360,   291,   619,
     386,   105,    98,   539,   387,   108,   855,   453,   454,   455,
      83,   109,  -672,   540,  -679,  -360,   292,    55,   620,    39,
      40,   334,   334,   334,   334,  1034,  1120,   334,   393,   334,
     386,   334,   949,   334,   387,   111,  1035,  1036,   588,  1159,
     335,   112,   393,   386,   129,   130,   952,   387,   672,   673,
    1211,   367,   335,   114,   368,   335,   335,   369,   136,   115,
     334,   541,   388,   350,   137,  1084,   389,   126,   211,   390,
     779,  1085,   335,   335,   963,   335,   617,   977,   939,   335,
     334,   276,   404,   393,   391,   405,   968,   954,   128,   973,
     736,   393,   388,   392,   131,   956,   389,   393,   978,   390,
     334,  1112,   393,   393,   139,   388,  1116,  1293,  1119,   389,
     132,   992,   390,   335,   391,   393,   156,   157,  1288,  1324,
     979,   369,   140,   392,    99,   100,   101,   391,   156,   157,
     840,   841,   847,   141,   471,   628,   392,   276,   142,   682,
     683,   684,   685,   686,   213,   214,   689,   690,   691,   692,
     135,   694,   695,   696,   697,   213,   214,   215,   151,   698,
    1154,   700,   701,    99,   154,   101,    81,   704,   705,   706,
      52,    53,   510,   710,   169,   172,    99,   207,   211,  1303,
     218,   219,   220,   272,   273,  1305,   274,   276,  1069,   288,
     286,   343,   344,   346,   345,   347,   348,   355,   349,   360,
     358,   362,   365,   366,   371,   372,   373,   374,    66,   712,
     375,   713,   714,   715,   716,   717,   718,   719,   720,   721,
     722,   376,   335,   396,   377,   378,   379,  1218,   380,   381,
     723,   724,   393,   335,   397,   335,   536,   382,   785,   398,
     672,   673,   537,   334,   547,   532,   578,   593,   608,  1320,
     577,   592,   516,   606,   335,   334,  1093,   611,   334,   334,
     535,   614,   609,   612,   610,   621,   613,  1104,   622,   632,
     633,   634,   635,   636,   637,   334,   334,   557,   334,  1338,
     654,   655,   334,   656,   672,   673,   565,   659,   667,   725,
     657,   645,   735,   575,   662,   663,   664,   665,   666,    13,
     617,  1239,   729,   586,  1268,   745,   335,   746,   571,   763,
     761,   765,   764,   600,   778,   767,   334,   766,   605,   770,
     607,   771,  1157,   786,   670,   789,   796,   801,   802,   616,
     810,   682,   683,   684,   685,   625,   626,   627,   849,   858,
     885,   692,   938,   694,   695,   696,   697,   941,   946,  1300,
     642,   698,   965,   700,   701,  1006,  1001,  1013,   646,  1026,
     648,   649,   650,   651,   652,   653,  1044,  1053,  1029,  1030,
    1054,  1055,  1056,  1057,   658,   682,   683,   684,   685,   686,
    1078,  1087,   689,   690,   691,   692,  1089,   694,   695,   696,
     697,  1061,  1074,  1097,  1081,   698,  1099,   700,   701,  1197,
    1101,  1103,  1334,  1109,  1114,  1118,  1165,  1127,   718,   719,
     720,   721,   722,  1166,  1139,  1141,  1149,  1319,  1167,  1168,
    1179,  1180,   723,   724,  1182,   334,  1185,  1188,  1209,  1250,
    1216,  1221,  1232,  1233,  1235,  1229,   334,   739,   334,  1230,
    1255,  1256,  1258,  1277,  1302,  1262,  1227,   335,  1318,   335,
     716,   717,   718,   719,   720,   721,   722,   334,  1331,  1237,
    1335,  1341,  1332,  1345,  1348,   842,   723,   724,   124,    17,
      79,   167,   769,   127,  1278,   125,  1281,   295,   937,   774,
    1033,  1282,  1283,  1241,  1284,  1285,    23,  1234,  1126,  1252,
    1299,   600,  1190,  1253,  1106,   585,  1289,    89,   642,   793,
     726,   794,  1274,   832,  1142,  1312,   799,   590,   591,   334,
       0,     0,   803,     0,     0,     0,     0,     0,   809,     0,
     335,     0,     0,     0,     0,     0,     0,   811,   812,   813,
     814,   815,   816,     0,   823,     0,   823,     0,   383,     0,
     383,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1294,     0,     0,     0,     0,     0,
       0,     0,     0,   335,   384,   385,   384,   385,     0,   869,
     870,   871,   872,   873,   874,   875,   876,   877,   878,     0,
       0,   879,   880,   881,   882,   883,   884,     0,   889,   890,
       0,   893,   894,   895,   897,   898,   899,   900,   901,   902,
     903,   904,   905,   906,   907,   908,   912,   914,   915,   916,
     917,   918,   919,   920,   921,   922,   923,   924,     0,   932,
       0,     0,   936,     0,     0,   386,     0,   386,   539,   387,
    1351,   387,   943,     0,     0,     0,     0,     0,   540,     0,
       0,     0,     0,     0,     0,     0,   672,   673,     0,     0,
     334,     0,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -66,   972,     0,     0,     0,     0,     0,
       0,   976,   889,   912,   672,   673,   541,   388,     0,   388,
       0,   389,     0,   389,   390,  1117,   390,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   391,
       0,   391,     0,     0,     0,     0,   648,   653,   392,     0,
     392,     0,     0,   334,     0,   653,     0,     0,  1024,     0,
     642,     0,     0,     0,     0,   383,  1028,   682,   683,   684,
     685,   686,     0,     0,   689,     0,  1039,   692,     0,   694,
     695,   696,   697,     0,   847,     0,     0,   698,     0,   700,
     701,   384,   385,     0,     0,     0,   334,   674,   675,   676,
     677,   678,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,   691,   692,   693,   694,   695,   696,
     697,     0,   847,     0,     0,   698,   699,   700,   701,   702,
     703,     0,     0,   704,   705,   706,   707,   708,   709,   710,
       0,     0,   716,   717,   718,   719,   720,   721,   722,     0,
       0,  1073,   386,     0,     0,  1077,   387,     0,   723,   724,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   711,   712,     0,   713,   714,   715,
     716,   717,   718,   719,   720,   721,   722,     0,     0,     0,
       0,  1110,     0,     0,     0,     0,   723,   724,     0,     0,
       0,     0,     0,     0,     0,   642,     0,     0,  1121,   672,
     673,     0,     0,     0,   388,     0,     0,     0,   389,     0,
    1122,   390,     0,     0,     0,     0,  1129,  1130,  1131,     0,
    1134,     0,     0,     0,     0,     0,   391,     0,  1140,     0,
       0,   823,     0,     0,     0,   392,   672,   673,     0,     0,
       0,     0,     0,     0,  1152,  1153,     0,     0,   516,     0,
       0,     0,  1158,     0,     0,     0,     0,   976,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1173,
       0,  1175,     0,     0,     0,     0,  1178,     0,     0,     0,
       0,     0,     0,     0,     0,  1181,     0,     0,     0,  1184,
     682,   683,   684,   685,   686,     0,     0,   689,   690,   691,
     692,     0,   694,   695,   696,   697,     0,     0,     0,     0,
     698,     0,   700,   701,     0,     0,     0,     0,   704,   705,
     706,     0,     0,     0,   710,     0,     0,   682,   683,   684,
     685,   686,     0,   642,   689,   690,   691,   692,     0,   694,
     695,   696,   697,     0,     0,     0,     0,   698,     0,   700,
     701,     0,     0,     0,     0,   672,   673,     0,     0,     0,
     712,     0,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   723,   724,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,  1246,     0,  1247,     0,     0,
    1249,   715,   716,   717,   718,   719,   720,   721,   722,     0,
       0,     0,     0,     0,  1254,     0,     0,     0,   723,   724,
    1259,  1260,  1261,     0,  1264,     0,     0,  1266,     0,     0,
       0,  1270,  1271,  1272,  1273,     0,     0,  1275,     0,     0,
       0,     0,     0,  1286,     0,     0,   682,   683,   684,   685,
     686,  1291,     0,   689,   690,   691,   692,     0,   694,   695,
     696,   697,     0,     0,     0,     0,   698,     0,   700,   701,
       0,     0,     0,   642,   704,   705,   706,     0,     0,     0,
     710,     0,  1309,  1310,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   712,  1327,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,     0,     0,
       0,  1337,     0,     0,     0,  1339,     0,   723,   724,     0,
       0,     0,     0,     0,  1082,     0,  1344,   407,  1346,  1347,
       0,     0,     3,  1349,   408,   409,   410,     0,   411,     0,
     412,   413,   414,   415,   416,     0,     0,     0,     0,     0,
     417,   418,   419,   420,   421,     0,   422,     0,     0,     0,
       0,     0,     0,   423,   424,     0,   425,     0,   426,   427,
       0,     0,   428,     0,     8,   429,   430,     0,   431,   432,
       0,     0,   433,   434,     0,     0,     0,     0,     0,   435,
       0,     0,   436,   300,   301,   302,     0,   304,   305,   306,
     307,   308,   437,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   322,   323,   324,     0,     0,
     327,   328,   329,   330,   438,     0,   439,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   443,   444,   445,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,   446,   447,   448,   449,   450,     0,   451,     0,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,    55,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   465,   466,   467,     0,    12,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,   470,     0,   471,   472,   473,   276,   474,   407,     0,
       0,     0,     0,     3,     0,   408,   409,   410,     0,   411,
       0,   412,   413,   414,   415,   416,     0,     0,     0,     0,
       0,   417,   418,   419,   420,   421,     0,   422,     0,     0,
       0,     0,     0,     0,   423,   424,     0,   425,     0,   426,
     427,     0,     0,   428,     0,     8,   429,   430,     0,   431,
     432,     0,     0,   433,   434,     0,     0,     0,     0,     0,
     435,     0,     0,   436,   300,   301,   302,     0,   304,   305,
     306,   307,   308,   437,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,     0,   322,   323,   324,     0,
       0,   327,   328,   329,   330,   438,     0,   439,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,   442,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   443,   444,   445,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,   446,   447,   448,   449,   450,   383,   451,
       0,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,    55,   463,   464,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   384,   385,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   465,   466,   467,     0,    12,
       0,     0,   468,   469,     0,     0,     0,     0,     0,     0,
       0,     0,   470,     0,   471,   472,   473,   276,  1156,   412,
     413,   414,   415,   416,     0,     0,     0,     0,     0,   417,
       0,   419,     0,     0,     0,   422,     0,   383,     0,     0,
       0,     0,     0,   424,     0,   386,     0,     0,   427,   387,
       0,   428,     0,     0,   429,     0,   818,     0,   432,     0,
       0,     0,     0,   384,   385,     0,     0,     0,   533,     0,
       0,   436,   300,   301,   302,     0,   304,   305,   306,   307,
     308,   437,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,     0,   322,   323,   324,     0,     0,   327,
     328,   329,   330,   438,     0,   439,     0,   388,     0,     0,
       0,   389,     0,  1123,   390,     0,     0,     0,   441,   442,
       0,     0,     0,     0,   386,     0,     0,     0,   387,   391,
       0,     0,     0,     0,     0,     0,     0,     0,   392,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,   446,   447,   448,   449,   450,     0,   451,   819,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     820,   534,   464,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   388,     0,     0,     0,
     389,     0,     0,   821,   466,   467,     0,    12,     0,     0,
     468,   469,     0,     0,     0,     0,     0,     0,   391,     0,
     822,     0,   471,   472,     0,   276,     0,   392,   412,   413,
     414,   415,   416,     0,     0,     0,     0,     0,   417,     0,
     419,     0,     0,     0,   422,     0,   383,     0,     0,     0,
       0,     0,   424,     0,     0,     0,     0,   427,     0,     0,
     428,     0,     0,   429,     0,     0,     0,   432,     0,     0,
       0,     0,   384,   385,     0,     0,     0,   533,     0,     0,
     436,   300,   301,   302,     0,   304,   305,   306,   307,   308,
     437,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,     0,   322,   323,   324,     0,     0,   327,   328,
     329,   330,   438,     0,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,     0,
       0,     0,     0,   386,     0,     0,     0,   387,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
     446,   447,   448,   449,   450,     0,   451,   819,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   820,
     534,   464,     0,     0,     0,     0,   383,     0,     0,     0,
       0,     0,     0,     0,     0,   388,     0,     0,     0,   389,
       0,     0,   821,   466,   467,     0,    12,     0,     0,   468,
     469,     0,   384,   385,     0,     0,     0,   391,     0,   830,
       0,   471,   472,     0,   276,     0,   392,   412,   413,   414,
     415,   416,     0,     0,     0,     0,     0,   417,     0,   419,
       0,     0,     0,   422,     0,   560,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,   427,     0,     0,   428,
       0,     0,   429,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,   386,     0,     0,   533,   387,     0,   436,
     300,   301,   302,     0,   304,   305,   306,   307,   308,   437,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,     0,   322,   323,   324,     0,     0,   327,   328,   329,
     330,   438,     0,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   441,   442,     0,     0,
       0,     0,     0,     0,     0,   388,     0,     0,     0,   389,
       0,  1124,   390,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,   391,     0,   446,
     447,   448,   449,   450,     0,   451,   392,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,    55,   534,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   561,     0,
       0,   465,   466,   467,     0,    12,     0,     0,   468,   469,
       0,     0,     0,     0,     0,     0,   562,     0,   470,     0,
     471,   472,     0,   276,     0,   563,   412,   413,   414,   415,
     416,     0,     0,     0,     0,     0,   417,     0,   419,     0,
       0,     0,   422,     0,     0,     0,     0,     0,     0,     0,
     424,     0,     0,     0,     0,   427,     0,     0,   428,     0,
       0,   429,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   533,     0,     0,   436,   300,
     301,   302,     0,   304,   305,   306,   307,   308,   437,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
       0,   322,   323,   324,     0,     0,   327,   328,   329,   330,
     438,     0,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   441,   442,     0,     0,     0,
       0,     0,     0,     0,   596,     0,     0,     0,     0,     0,
     443,   444,   445,     0,     0,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,   446,   447,
     448,   449,   450,     0,   451,     0,   452,   453,   454,   455,
     456,   457,   458,   459,   460,   461,   462,    55,   534,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     465,   466,   467,     0,    12,     0,     0,   468,   469,     0,
       0,   412,   413,   414,   415,   416,     0,   470,     0,   471,
     472,   417,   276,   419,     0,     0,   383,   422,     0,     0,
       0,     0,     0,     0,     0,   424,     0,     0,     0,     0,
     427,     0,     0,   428,     0,     0,   429,     0,     0,     0,
     432,     0,   384,   385,     0,     0,     0,     0,     0,     0,
     533,     0,     0,   436,   300,   301,   302,     0,   304,   305,
     306,   307,   308,   437,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,     0,   322,   323,   324,     0,
       0,   327,   328,   329,   330,   438,     0,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,   442,     0,   386,     0,     0,     0,   387,     0,   623,
       0,     0,     0,     0,     0,   443,   444,   445,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,   446,   447,   448,   449,   450,     0,   451,
       0,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,    55,   534,   464,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   388,     0,     0,     0,   389,
       0,  1125,   390,     0,     0,   465,   466,   467,     0,    12,
       0,     0,   468,   469,     0,     0,     0,   391,     0,     0,
       0,     0,   470,     0,   471,   472,   392,   276,   412,   413,
     414,   415,   416,     0,     0,     0,     0,     0,   417,  1279,
     419,   420,     0,     0,   422,     0,     0,     0,     0,     0,
       0,     0,   424,     0,     0,     0,     0,   427,     0,     0,
     428,     0,     0,   429,   430,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   533,     0,     0,
     436,   300,   301,   302,     0,   304,   305,   306,   307,   308,
     437,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,     0,   322,   323,   324,     0,     0,   327,   328,
     329,   330,   438,     0,   439,  1280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
     446,   447,   448,   449,   450,     0,   451,     0,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,    55,
     534,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   465,   466,   467,     0,    12,     0,     0,   468,
     469,     0,     0,   412,   413,   414,   415,   416,     0,   470,
       0,   471,   472,   417,   276,   419,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,   424,     0,     0,
       0,     0,   427,     0,     0,   428,     0,     0,   429,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   533,     0,     0,   436,   300,   301,   302,     0,
     304,   305,   306,   307,   308,   437,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,     0,   322,   323,
     324,     0,     0,   327,   328,   329,   330,   438,     0,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   441,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   443,   444,   445,
       0,     0,     0,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,   446,   447,   448,   449,   450,
       0,   451,     0,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,    55,   534,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   465,   466,   467,
       0,    12,     0,     0,   468,   469,     0,     0,   412,   413,
     414,   415,   416,     0,   470,     0,   471,   472,   417,   276,
     419,     0,     0,     0,   422,     0,     0,     0,     0,     0,
       0,     0,   424,     0,     0,     0,     0,   427,     0,     0,
     428,     0,     0,   429,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   533,     0,     0,
     436,   300,   301,   302,     0,   304,   305,   306,   307,   308,
     437,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,     0,   322,   323,   324,     0,     0,   327,   328,
     329,   330,   438,     0,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   886,   887,   888,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
     446,   447,   448,   449,   450,     0,   451,     0,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,    55,
     534,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   465,   466,   467,     0,    12,     0,     0,   468,
     469,     0,     0,   412,   413,   414,   415,   416,     0,   470,
       0,   471,   472,   417,   276,   419,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,   424,     0,     0,
       0,     0,   427,     0,     0,   428,     0,     0,   429,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   533,     0,     0,   436,   300,   301,   302,     0,
     304,   305,   306,   307,   308,   437,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,     0,   322,   323,
     324,     0,     0,   327,   328,   329,   330,   438,     0,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   441,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   909,   910,   911,
       0,     0,     0,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,   446,   447,   448,   449,   450,
       0,   451,     0,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,    55,   534,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   465,   466,   467,
       0,    12,     0,     0,   468,   469,     0,     0,   412,   413,
     414,   415,   416,     0,   470,     0,   471,   472,   417,   276,
     419,     0,     0,   383,   422,     0,     0,     0,     0,     0,
       0,     0,   424,     0,     0,     0,     0,   427,     0,     0,
     428,     0,     0,   429,     0,     0,     0,   432,     0,   384,
     385,     0,     0,     0,     0,     0,     0,   533,     0,     0,
     436,   300,   301,   302,     0,   304,   305,   306,   307,   308,
     437,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,     0,   322,   323,   324,     0,     0,   327,   328,
     329,   330,   438,     0,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   442,     0,
     386,     0,     0,     0,   387,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
     446,   447,   448,   449,   450,     0,   451,     0,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,    55,
     534,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   388,     0,     0,     0,   389,     0,  1150,   390,
       0,     0,   465,   466,   467,     0,    12,     0,     0,   468,
     469,     0,     0,     0,   391,     0,     0,   852,     0,   470,
     853,   471,   472,   392,   276,   412,   413,   414,   415,   416,
       0,     0,     0,     0,     0,   417,     0,   419,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,   424,
       0,     0,     0,     0,   427,     0,     0,   428,     0,     0,
     429,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   533,     0,     0,   436,   300,   301,
     302,     0,   304,   305,   306,   307,   308,   437,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,     0,     0,   327,   328,   329,   330,   438,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,   446,   447,   448,
     449,   450,     0,   451,     0,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,    55,   534,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     466,   467,     0,    12,     0,     0,   468,   469,     0,     0,
     412,   413,   414,   415,   416,   564,   470,     0,   471,   472,
     417,   276,   419,     0,     0,     0,   422,     0,     0,     0,
       0,     0,     0,     0,   424,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   432,
       0,     0,     0,     0,     0,   604,     0,     0,     0,   533,
       0,     0,   436,   300,   301,   302,     0,   304,   305,   306,
     307,   308,   437,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   322,   323,   324,     0,     0,
     327,   328,   329,   330,   438,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,   446,   447,   448,   449,   450,     0,   451,     0,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,    55,   534,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   465,   466,   467,     0,    12,     0,
       0,   468,   469,     0,     0,   412,   413,   414,   415,   416,
       0,   470,     0,   471,   472,   417,   276,   419,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,   424,
       0,     0,     0,     0,   427,     0,     0,   428,     0,     0,
     429,     0,     0,     0,   432,     0,     0,   615,     0,     0,
       0,     0,     0,     0,   533,     0,     0,   436,   300,   301,
     302,     0,   304,   305,   306,   307,   308,   437,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,     0,     0,   327,   328,   329,   330,   438,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,   446,   447,   448,
     449,   450,     0,   451,     0,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,    55,   534,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     466,   467,     0,    12,     0,     0,   468,   469,     0,     0,
     412,   413,   414,   415,   416,     0,   470,     0,   471,   472,
     417,   276,   419,     0,     0,     0,   422,     0,     0,     0,
       0,     0,     0,     0,   424,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   533,
       0,     0,   436,   300,   301,   302,     0,   304,   305,   306,
     307,   308,   437,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   322,   323,   324,     0,     0,
     327,   328,   329,   330,   438,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,   446,   447,   448,   449,   450,     0,   451,     0,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,    55,   534,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   645,     0,   465,   466,   467,     0,    12,     0,
       0,   468,   469,     0,     0,   412,   413,   414,   415,   416,
       0,   470,   913,   471,   472,   417,   276,   419,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,   424,
       0,     0,     0,     0,   427,     0,     0,   428,     0,     0,
     429,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   533,     0,     0,   436,   300,   301,
     302,     0,   304,   305,   306,   307,   308,   437,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,     0,     0,   327,   328,   329,   330,   438,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,   446,   447,   448,
     449,   450,     0,   451,     0,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,    55,   534,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     466,   467,     0,    12,     0,     0,   468,   469,     0,     0,
     412,   413,   414,   415,   416,     0,   470,     0,   471,   472,
     417,   276,   419,     0,     0,     0,   422,     0,     0,     0,
       0,     0,     0,     0,   424,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   533,
       0,     0,   436,   300,   301,   302,     0,   304,   305,   306,
     307,   308,   437,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   322,   323,   324,     0,     0,
     327,   328,   329,   330,   438,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,   446,   447,   448,   449,   450,     0,   451,     0,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,    55,   534,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   465,   466,   467,     0,    12,     0,
       0,   468,   469,     0,     0,   412,   413,   414,   415,   416,
       0,   470,   933,   471,   472,   417,   276,   419,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,   424,
       0,     0,     0,     0,   427,     0,     0,   428,     0,     0,
     429,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   533,     0,     0,   436,   300,   301,
     302,     0,   304,   305,   306,   307,   308,   437,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,     0,     0,   327,   328,   329,   330,   438,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,   446,   447,   448,
     449,   450,     0,   451,     0,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,    55,   534,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     466,   467,     0,    12,     0,     0,   468,   469,     0,     0,
     412,   413,   414,   415,   416,     0,   470,   969,   471,   472,
     417,   276,   419,     0,     0,     0,   422,     0,     0,     0,
       0,     0,     0,     0,   424,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   533,
       0,     0,   436,   300,   301,   302,     0,   304,   305,   306,
     307,   308,   437,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   322,   323,   324,     0,     0,
     327,   328,   329,   330,   438,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,   446,   447,   448,   449,   450,     0,   451,     0,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,    55,   534,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   971,     0,   465,   466,   467,     0,    12,     0,
       0,   468,   469,     0,     0,   412,   413,   414,   415,   416,
       0,   470,     0,   471,   472,   417,   276,   419,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,   424,
       0,     0,     0,     0,   427,     0,     0,   428,     0,     0,
     429,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   533,     0,     0,   436,   300,   301,
     302,     0,   304,   305,   306,   307,   308,   437,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,     0,     0,   327,   328,   329,   330,   438,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,   446,   447,   448,
     449,   450,     0,   451,     0,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,    55,   534,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
     466,   467,     0,    12,     0,     0,   468,   469,     0,     0,
     412,   413,   414,   415,   416,     0,   470,  1004,   471,   472,
     417,   276,   419,     0,     0,     0,   422,     0,     0,     0,
       0,     0,     0,     0,   424,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   429,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   533,
       0,     0,   436,   300,   301,   302,     0,   304,   305,   306,
     307,   308,   437,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,     0,   322,   323,   324,     0,     0,
     327,   328,   329,   330,   438,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,   446,   447,   448,   449,   450,     0,   451,     0,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,    55,   534,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   465,   466,   467,     0,    12,     0,
       0,   468,   469,     0,     0,   412,   413,   414,   415,   416,
       0,   470,  1019,   471,   472,   417,   276,   419,     0,     0,
     383,   422,     0,     0,     0,     0,     0,     0,     0,   424,
       0,     0,     0,     0,   427,     0,     0,   428,     0,     0,
     429,     0,     0,     0,   432,     0,   384,   385,     0,     0,
       0,     0,     0,     0,   533,     0,     0,   436,   300,   301,
     302,     0,   304,   305,   306,   307,   308,   437,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,     0,
     322,   323,   324,     0,     0,   327,   328,   329,   330,   438,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,   386,     0,     0,
       0,   387,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,   446,   447,   448,
     449,   450,     0,   451,     0,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,    55,   534,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   388,
       0,     0,     0,   389,     0,  1204,   390,     0,     0,   465,
     466,   467,     0,    12,     0,     0,   468,   469,     0,     0,
       0,   391,     0,     0,  1162,     0,   470,     0,   471,   472,
     392,   276,   412,   413,   414,   415,   416,     0,     0,     0,
       0,     0,   417,     0,   419,     0,     0,     0,   422,     0,
       0,     0,     0,     0,     0,     0,   424,     0,     0,     0,
       0,   427,     0,     0,   428,     0,     0,   429,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   533,     0,     0,   436,   300,   301,   302,     0,   304,
     305,   306,   307,   308,   437,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,     0,   322,   323,   324,
       0,     0,   327,   328,   329,   330,   438,     0,   439,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,   446,   447,   448,   449,   450,     0,
     451,     0,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,    55,   534,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   466,   467,     0,
      12,     0,     0,   468,   469,     0,     0,   412,   413,   414,
     415,   416,     0,   470,  1176,   471,   472,   417,   276,   419,
       0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,   427,     0,     0,   428,
       0,     0,   429,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   533,     0,     0,   436,
     300,   301,   302,     0,   304,   305,   306,   307,   308,   437,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,     0,   322,   323,   324,     0,     0,   327,   328,   329,
     330,   438,     0,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   441,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,   446,
     447,   448,   449,   450,     0,   451,     0,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,    55,   534,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   465,   466,   467,     0,    12,     0,     0,   468,   469,
       0,     0,   412,   413,   414,   415,   416,     0,   470,  1210,
     471,   472,   417,   276,   419,     0,     0,     0,   422,     0,
       0,     0,     0,     0,     0,     0,   424,     0,     0,     0,
       0,   427,     0,     0,   428,     0,     0,   429,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   533,     0,     0,   436,   300,   301,   302,     0,   304,
     305,   306,   307,   308,   437,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,     0,   322,   323,   324,
       0,     0,   327,   328,   329,   330,   438,     0,   439,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,   446,   447,   448,   449,   450,     0,
     451,     0,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,    55,   534,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   466,   467,     0,
      12,     0,     0,   468,   469,     0,     0,   412,   413,   414,
     415,   416,     0,   470,  1263,   471,   472,   417,   276,   419,
       0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,   427,     0,     0,   428,
       0,     0,   429,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   533,     0,     0,   436,
     300,   301,   302,     0,   304,   305,   306,   307,   308,   437,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,     0,   322,   323,   324,     0,     0,   327,   328,   329,
     330,   438,     0,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   441,   442,     0,     0,
       0,     0,     0,     0,     0,  1290,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,   446,
     447,   448,   449,   450,     0,   451,     0,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,    55,   534,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   465,   466,   467,     0,    12,     0,     0,   468,   469,
       0,     0,   412,   413,   414,   415,   416,     0,   470,     0,
     471,   472,   417,   276,   419,     0,     0,     0,   422,     0,
       0,     0,     0,     0,     0,     0,   424,     0,     0,     0,
       0,   427,     0,     0,   428,     0,     0,   429,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   533,     0,     0,   436,   300,   301,   302,     0,   304,
     305,   306,   307,   308,   437,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,     0,   322,   323,   324,
       0,     0,   327,   328,   329,   330,   438,     0,   439,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,     0,     0,     0,     0,     0,     0,     0,
    1315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,   446,   447,   448,   449,   450,     0,
     451,     0,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,    55,   534,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   466,   467,     0,
      12,     0,     0,   468,   469,     0,     0,   412,   413,   414,
     415,   416,     0,   470,     0,   471,   472,   417,   276,   419,
       0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,   427,     0,     0,   428,
       0,     0,   429,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   533,     0,     0,   436,
     300,   301,   302,     0,   304,   305,   306,   307,   308,   437,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,     0,   322,   323,   324,     0,     0,   327,   328,   329,
     330,   438,     0,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   441,   442,     0,     0,
       0,     0,     0,     0,     0,  1316,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,   446,
     447,   448,   449,   450,     0,   451,     0,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,    55,   534,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   465,   466,   467,     0,    12,     0,     0,   468,   469,
       0,     0,   412,   413,   414,   415,   416,     0,   470,     0,
     471,   472,   417,   276,   419,     0,     0,   383,   422,     0,
       0,     0,     0,     0,     0,     0,   424,     0,     0,     0,
       0,   427,     0,     0,   428,     0,     0,   429,     0,     0,
       0,   432,     0,   384,   385,     0,     0,     0,     0,     0,
       0,   533,     0,     0,   436,   300,   301,   302,     0,   304,
     305,   306,   307,   308,   437,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,     0,   322,   323,   324,
       0,     0,   327,   328,   329,   330,   438,     0,   439,     0,
       0,     0,     0,     0,   383,     0,     0,     0,     0,     0,
       0,   441,   442,     0,   386,     0,     0,     0,   387,     0,
       0,     0,     0,     0,     0,     0,   672,   673,     0,     0,
     384,   385,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,   446,   447,   448,   449,   450,     0,
     451,     0,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,    55,   534,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   388,     0,     0,     0,
     389,     0,  1225,   390,     0,     0,   465,   466,   467,     0,
      12,   386,     0,   468,   469,   387,     0,     0,   391,     0,
       0,     0,     0,   470,     0,   471,   472,   392,   276,   674,
     675,   676,   677,   678,   679,   680,   681,   682,   683,   684,
     685,   686,   687,   688,   689,   690,   691,   692,   787,   694,
     695,   696,   697,   672,   673,     0,     0,   698,   699,   700,
     701,   702,   703,     0,     0,   704,   705,   706,   707,   708,
     709,   710,     0,   388,     0,     0,     0,   389,     0,  1292,
     390,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     672,   673,     0,     0,     0,   391,     0,     0,     0,     0,
       0,     0,     0,     0,   392,     0,   788,   712,     0,   713,
     714,   715,   716,   717,   718,   719,   720,   721,   722,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   723,   724,
       0,     0,     0,     0,     0,   944,   674,   675,   676,   677,
     678,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,   691,   692,   787,   694,   695,   696,   697,
       0,     0,     0,     0,   698,   699,   700,   701,   702,   703,
       0,     0,   704,   705,   706,   707,   708,   709,   710,     0,
       0,   682,   683,   684,   685,   686,     0,     0,   689,   690,
     691,   692,     0,   694,   695,   696,   697,   672,   673,     0,
       0,   698,     0,   700,   701,     0,     0,     0,     0,   704,
     705,   706,     0,   788,   712,   710,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,     0,     0,     0,     0,
       0,     0,   672,   673,     0,   723,   724,     0,     0,     0,
       0,     0,  1090,     0,     0,     0,     0,     0,     0,     0,
       0,   712,     0,   713,   714,   715,   716,   717,   718,   719,
     720,   721,   722,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   723,   724,     0,     0,     0,     0,     0,  1307,
       0,     0,     0,     0,     0,     0,     0,     0,   682,   683,
     684,   685,   686,     0,     0,   689,   690,   691,   692,     0,
     694,   695,   696,   697,     0,     0,     0,     0,   698,     0,
     700,   701,     0,     0,     0,     0,   704,   705,   706,     0,
       0,     0,   710,   682,   683,   684,   685,   686,     0,     0,
     689,   690,   691,   692,     0,   694,   695,   696,   697,     0,
       0,     0,     0,   698,     0,   700,   701,     0,     0,     0,
       0,   704,   705,   706,     0,     0,     0,   710,   712,     0,
     713,   714,   715,   716,   717,   718,   719,   720,   721,   722,
     672,   673,     0,     0,     0,     0,     0,     0,     0,   723,
     724,     0,   296,     0,     0,     0,  1350,     0,   297,     0,
       0,     0,     0,   712,     0,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,   298,     0,     0,     0,     0,
       0,     0,     0,     0,   723,   724,     0,     0,     0,     0,
     299,  1352,     0,     0,     0,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,     0,     0,
       0,   682,   683,   684,   685,   686,     0,     0,   689,   690,
     691,   692,     0,   694,   695,   696,   697,     0,     0,     0,
       0,   698,     0,   700,   701,   672,   673,     0,     0,   704,
     705,   706,     0,     0,     0,   710,    54,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   333,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     672,   673,     0,    55,     0,     0,     0,     0,     0,     0,
       0,   712,     0,   713,   714,   715,   716,   717,   718,   719,
     720,   721,   722,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   723,   724,     0,     0,   734,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     566,     0,     0,     0,     0,   383,   682,   683,   684,   685,
     686,     0,     0,   689,   690,   691,   692,     0,   694,   695,
     696,   697,     0,     0,     0,     0,   698,     0,   700,   701,
       0,   384,   385,     0,   704,   705,   706,     0,     0,     0,
     710,   682,   683,   684,   685,   686,   672,   673,   689,   690,
     691,   692,     0,   694,   695,   696,   697,     0,     0,     0,
       0,   698,     0,   700,   701,     0,     0,     0,     0,   704,
     705,   706,     0,     0,     0,   710,   712,     0,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,     0,     0,
       0,     0,   386,     0,     0,     0,   387,   723,   724,     0,
       0,   760,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   712,     0,   713,   714,   715,   716,   717,   718,   719,
     720,   721,   722,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   723,   724,     0,     0,   768,   682,   683,   684,
     685,   686,   672,   673,   689,   690,   691,   692,     0,   694,
     695,   696,   697,     0,   388,     0,     0,   698,   389,   700,
     701,   390,     0,     0,     0,   704,   705,   706,     0,     0,
       0,   710,     0,     0,     0,     0,   391,   672,   673,     0,
       0,     0,     0,     0,     0,   392,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   712,     0,   713,
     714,   715,   716,   717,   718,   719,   720,   721,   722,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   723,   724,
       0,     0,   843,   682,   683,   684,   685,   686,     0,     0,
     689,   690,   691,   692,     0,   694,   695,   696,   697,     0,
       0,     0,     0,   698,     0,   700,   701,     0,     0,     0,
       0,   704,   705,   706,     0,     0,     0,   710,   682,   683,
     684,   685,   686,   672,   673,   689,   690,   691,   692,     0,
     694,   695,   696,   697,     0,     0,     0,     0,   698,     0,
     700,   701,     0,     0,     0,     0,   704,   705,   706,     0,
       0,     0,   710,   712,     0,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   723,   724,     0,     0,   964,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   712,     0,
     713,   714,   715,   716,   717,   718,   719,   720,   721,   722,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   723,
     724,     0,     0,   982,   682,   683,   684,   685,   686,   672,
     673,   689,   690,   691,   692,     0,   694,   695,   696,   697,
       0,     0,     0,     0,   698,     0,   700,   701,     0,     0,
       0,     0,   704,   705,   706,     0,     0,     0,   710,     0,
       0,     0,     0,     0,   672,   673,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   712,     0,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   723,   724,     0,     0,   986,
     682,   683,   684,   685,   686,     0,     0,   689,   690,   691,
     692,     0,   694,   695,   696,   697,     0,     0,     0,     0,
     698,     0,   700,   701,     0,     0,     0,     0,   704,   705,
     706,     0,     0,     0,   710,   682,   683,   684,   685,   686,
     672,   673,   689,   690,   691,   692,     0,   694,   695,   696,
     697,     0,     0,     0,     0,   698,     0,   700,   701,     0,
       0,     0,     0,   704,   705,   706,     0,     0,     0,   710,
     712,     0,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   723,   724,     0,     0,   988,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   712,     0,   713,   714,   715,
     716,   717,   718,   719,   720,   721,   722,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   723,   724,     0,     0,
     993,   682,   683,   684,   685,   686,   672,   673,   689,   690,
     691,   692,     0,   694,   695,   696,   697,     0,     0,     0,
       0,   698,     0,   700,   701,     0,     0,     0,     0,   704,
     705,   706,     0,     0,     0,   710,     0,     0,     0,     0,
       0,   672,   673,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   712,     0,   713,   714,   715,   716,   717,   718,   719,
     720,   721,   722,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   723,   724,     0,     0,   994,   682,   683,   684,
     685,   686,     0,     0,   689,   690,   691,   692,     0,   694,
     695,   696,   697,     0,     0,     0,     0,   698,     0,   700,
     701,     0,     0,     0,     0,   704,   705,   706,     0,     0,
       0,   710,   682,   683,   684,   685,   686,   672,   673,   689,
     690,   691,   692,     0,   694,   695,   696,   697,     0,     0,
       0,     0,   698,     0,   700,   701,     0,     0,     0,     0,
     704,   705,   706,     0,     0,     0,   710,   712,     0,   713,
     714,   715,   716,   717,   718,   719,   720,   721,   722,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   723,   724,
       0,     0,   995,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   712,     0,   713,   714,   715,   716,   717,   718,
     719,   720,   721,   722,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   723,   724,     0,     0,   996,   682,   683,
     684,   685,   686,   672,   673,   689,   690,   691,   692,     0,
     694,   695,   696,   697,     0,     0,     0,     0,   698,     0,
     700,   701,     0,     0,     0,     0,   704,   705,   706,     0,
       0,     0,   710,     0,     0,     0,     0,     0,   672,   673,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   712,     0,
     713,   714,   715,   716,   717,   718,   719,   720,   721,   722,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   723,
     724,     0,     0,   997,   682,   683,   684,   685,   686,     0,
       0,   689,   690,   691,   692,     0,   694,   695,   696,   697,
       0,     0,     0,     0,   698,     0,   700,   701,     0,     0,
       0,     0,   704,   705,   706,     0,     0,     0,   710,   682,
     683,   684,   685,   686,   672,   673,   689,   690,   691,   692,
       0,   694,   695,   696,   697,     0,     0,     0,     0,   698,
       0,   700,   701,     0,     0,     0,     0,   704,   705,   706,
       0,     0,     0,   710,   712,     0,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   723,   724,     0,     0,   998,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   712,
       0,   713,   714,   715,   716,   717,   718,   719,   720,   721,
     722,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     723,   724,     0,     0,  1115,   682,   683,   684,   685,   686,
     672,   673,   689,   690,   691,   692,     0,   694,   695,   696,
     697,     0,     0,     0,     0,   698,     0,   700,   701,     0,
       0,     0,     0,   704,   705,   706,     0,     0,     0,   710,
       0,     0,     0,     0,     0,   672,   673,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   712,     0,   713,   714,   715,
     716,   717,   718,   719,   720,   721,   722,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   723,   724,     0,     0,
    1213,   682,   683,   684,   685,   686,     0,     0,   689,   690,
     691,   692,     0,   694,   695,   696,   697,     0,     0,     0,
       0,   698,     0,   700,   701,     0,     0,     0,     0,   704,
     705,   706,     0,     0,     0,   710,   682,   683,   684,   685,
     686,   672,   673,   689,   690,   691,   692,     0,   694,   695,
     696,   697,     0,     0,     0,     0,   698,     0,   700,   701,
       0,     0,     0,     0,   704,   705,   706,     0,     0,     0,
     710,   712,     0,   713,   714,   715,   716,   717,   718,   719,
     720,   721,   722,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   723,   724,     0,     0,  1223,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   712,     0,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   723,   724,     0,
       0,  1224,   682,   683,   684,   685,   686,   672,   673,   689,
     690,   691,   692,     0,   694,   695,   696,   697,     0,     0,
       0,     0,   698,     0,   700,   701,     0,     0,     0,     0,
     704,   705,   706,     0,     0,     0,   710,     0,     0,     0,
       0,     0,   672,   673,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   712,     0,   713,   714,   715,   716,   717,   718,
     719,   720,   721,   722,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   723,   724,     0,     0,  1240,   682,   683,
     684,   685,   686,     0,     0,   689,   690,   691,   692,     0,
     694,   695,   696,   697,     0,     0,     0,     0,   698,     0,
     700,   701,     0,     0,     0,     0,   704,   705,   706,     0,
       0,     0,   710,   682,   683,   684,   685,   686,   672,   673,
     689,   690,   691,   692,     0,   694,   695,   696,   697,     0,
       0,     0,     0,   698,     0,   700,   701,     0,     0,     0,
       0,   704,   705,   706,     0,     0,     0,   710,   712,     0,
     713,   714,   715,   716,   717,   718,   719,   720,   721,   722,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   723,
     724,     0,     0,  1242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   712,     0,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   723,   724,     0,     0,  1244,   682,
     683,   684,   685,   686,   672,   673,   689,   690,   691,   692,
       0,   694,   695,   696,   697,     0,     0,     0,     0,   698,
       0,   700,   701,     0,     0,     0,     0,   704,   705,   706,
       0,     0,     0,   710,     0,     0,     0,     0,     0,   672,
     673,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   712,
       0,   713,   714,   715,   716,   717,   718,   719,   720,   721,
     722,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     723,   724,     0,     0,  1248,   682,   683,   684,   685,   686,
       0,     0,   689,   690,   691,   692,     0,   694,   695,   696,
     697,     0,     0,     0,     0,   698,     0,   700,   701,     0,
       0,     0,     0,   704,   705,   706,     0,     0,     0,   710,
     682,   683,   684,   685,   686,   672,   673,   689,   690,   691,
     692,     0,   694,   695,   696,   697,     0,     0,     0,     0,
     698,     0,   700,   701,     0,     0,     0,     0,   704,   705,
     706,     0,     0,     0,   710,   712,     0,   713,   714,   715,
     716,   717,   718,   719,   720,   721,   722,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   723,   724,     0,     0,
    1295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     712,     0,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   723,   724,     0,     0,  1296,   682,   683,   684,   685,
     686,   672,   673,   689,   690,   691,   692,     0,   694,   695,
     696,   697,     0,     0,     0,     0,   698,     0,   700,   701,
       0,     0,     0,     0,   704,   705,   706,     0,     0,     0,
     710,     0,     0,     0,     0,     0,   672,   673,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   712,     0,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   723,   724,     0,
       0,  1297,   682,   683,   684,   685,   686,     0,     0,   689,
     690,   691,   692,     0,   694,   695,   696,   697,     0,     0,
       0,     0,   698,     0,   700,   701,     0,     0,     0,     0,
     704,   705,   706,     0,     0,     0,   710,   682,   683,   684,
     685,   686,   672,   673,   689,   690,   691,   692,     0,   694,
     695,   696,   697,     0,     0,     0,     0,   698,     0,   700,
     701,     0,     0,     0,     0,   704,   705,   706,     0,     0,
       0,   710,   712,     0,   713,   714,   715,   716,   717,   718,
     719,   720,   721,   722,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   723,   724,     0,     0,  1301,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   712,     0,   713,
     714,   715,   716,   717,   718,   719,   720,   721,   722,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   723,   724,
       0,     0,  1306,   682,   683,   684,   685,   686,   672,   673,
     689,   690,   691,   692,     0,   694,   695,   696,   697,     0,
       0,     0,     0,   698,     0,   700,   701,     0,     0,     0,
       0,   704,   705,   706,     0,     0,     0,   710,     0,     0,
       0,     0,     0,   672,   673,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   712,     0,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   723,   724,     0,     0,  1311,   682,
     683,   684,   685,   686,     0,     0,   689,   690,   691,   692,
       0,   694,   695,   696,   697,     0,     0,     0,     0,   698,
       0,   700,   701,     0,     0,     0,     0,   704,   705,   706,
       0,     0,     0,   710,   682,   683,   684,   685,   686,   672,
     673,   689,   690,   691,   692,     0,   694,   695,   696,   697,
       0,     0,     0,     0,   698,     0,   700,   701,     0,     0,
       0,     0,   704,   705,   706,     0,     0,     0,   710,   712,
       0,   713,   714,   715,   716,   717,   718,   719,   720,   721,
     722,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     723,   724,     0,     0,  1343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   712,     0,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   723,   724,   762,     0,     0,
     682,   683,   684,   685,   686,   672,   673,   689,   690,   691,
     692,     0,   694,   695,   696,   697,     0,     0,     0,     0,
     698,     0,   700,   701,     0,     0,     0,     0,   704,   705,
     706,     0,     0,     0,   710,     0,     0,     0,     0,     0,
     672,   673,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     712,     0,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   723,   724,  1062,     0,     0,   682,   683,   684,   685,
     686,     0,     0,   689,   690,   691,   692,     0,   694,   695,
     696,   697,     0,     0,     0,     0,   698,     0,   700,   701,
       0,     0,     0,     0,   704,   705,   706,     0,     0,     0,
     710,   682,   683,   684,   685,   686,   672,   673,   689,   690,
     691,   692,     0,   694,   695,   696,   697,     0,     0,     0,
       0,   698,     0,   700,   701,     0,     0,     0,     0,   704,
     705,   706,     0,     0,     0,   710,   712,     0,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   723,   724,  1079,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   712,     0,   713,   714,   715,   716,   717,   718,   719,
     720,   721,   722,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   723,   724,  1183,     0,     0,   682,   683,   684,
     685,   686,     0,     0,   689,   690,   691,   692,     0,   694,
     695,   696,   697,   672,   673,     0,     0,   698,     0,   700,
     701,     0,     0,     0,     0,   704,   705,   706,     0,     0,
       0,   710,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   712,     0,   713,
     714,   715,   716,   717,   718,   719,   720,   721,   722,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   723,   724,
    1186,     0,     0,     0,     0,     0,   674,   675,   676,   677,
     678,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,   691,   692,   787,   694,   695,   696,   697,
    -325,     0,   221,   222,   698,   699,   700,   701,   702,   703,
       0,     0,   704,   705,   706,   707,   708,   709,   710,   223,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   672,   673,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   788,   712,     0,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   723,   724,     0,     0,     0,
       0,     0,     0,     0,     0,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,     0,     0,   242,   243,   244,     0,     0,
       0,     0,     0,     0,   245,   246,   247,   248,   249,     0,
       0,   250,   251,   252,   253,   254,   255,   256,   682,   683,
     684,   685,   686,   672,   673,   689,   690,   691,   692,     0,
     694,   695,   696,   697,     0,     0,     0,     0,   698,     0,
     700,   701,     0,     0,   833,     0,   704,   705,   706,     0,
       0,     0,   710,   257,     0,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,     0,     0,   268,   269,     0,
       0,     0,     0,     0,   270,   271,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   712,     0,
     713,   714,   715,   716,   717,   718,   719,   720,   721,   722,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   723,
     724,     0,     0,     0,   682,   683,   684,   685,   686,   672,
     673,   689,   690,   691,   692,     0,   694,   695,   696,   697,
       0,     0,     0,     0,   698,     0,   700,   701,     0,     0,
    1005,     0,   704,   705,   706,     0,     0,     0,   710,     0,
       0,     0,     0,     0,   672,   673,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   712,     0,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   723,   724,     0,     0,     0,
     682,   683,   684,   685,   686,     0,     0,   689,   690,   691,
     692,     0,   694,   695,   696,   697,     0,     0,     0,     0,
     698,     0,   700,   701,     0,     0,     0,     0,   704,   705,
     706,     0,     0,     0,   710,   682,   683,   684,   685,   686,
     672,   673,   689,   690,   691,   692,     0,   694,   695,   696,
     697,     0,     0,     0,     0,   698,     0,   700,   701,     0,
       0,     0,     0,   704,   705,   706,     0,     0,     0,   710,
     712,  1070,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   723,   724,     0,     0,     0,  1151,     0,     0,     0,
       0,     0,     0,     0,     0,   712,     0,   713,   714,   715,
     716,   717,   718,   719,   720,   721,   722,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   723,   724,     0,     0,
       0,   682,   683,   684,   685,   686,   672,   673,   689,   690,
     691,   692,     0,   694,   695,   696,   697,     0,     0,     0,
       0,   698,     0,   700,   701,     0,     0,     0,     0,   704,
     705,   706,     0,     0,     0,   710,     0,     0,     0,     0,
       0,   672,   673,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   712,     0,   713,   714,   715,   716,   717,   718,   719,
     720,   721,   722,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   723,   724,     0,     0,     0,   682,   683,   684,
     685,   686,     0,     0,   689,   690,   691,   692,     0,   694,
     695,   696,   697,     0,     0,     0,     0,   698,     0,   700,
     701,     0,     0,     0,     0,   704,   705,   706,     0,     0,
       0,  -680,   682,   683,   684,   685,   686,   672,   673,   689,
     690,   691,   692,     0,   694,   695,   696,   697,     0,     0,
       0,     0,   698,     0,   700,   701,     0,     0,     0,     0,
     704,   705,   706,     0,     0,     0,     0,   712,     0,   713,
     714,   715,   716,   717,   718,   719,   720,   721,   722,   672,
     673,     0,     0,     0,     0,     0,     0,     0,   723,   724,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   712,     0,   713,   714,   715,   716,   717,   718,
     719,   720,   721,   722,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   723,   724,     0,     0,     0,   682,   683,
     684,   685,   686,     0,     0,   689,   690,   691,   692,     0,
     694,   695,   696,   697,     0,     0,     0,     0,   698,     0,
     700,   701,     0,     0,     0,     0,   704,     0,   706,     0,
       0,     0,     0,     0,     0,   672,   673,     0,     0,     0,
     682,   683,   684,   685,   686,     0,     0,   689,   690,   691,
     692,     0,   694,   695,   696,   697,     0,     0,     0,     0,
     698,     0,   700,   701,   672,   673,     0,     0,   704,     0,
     713,   714,   715,   716,   717,   718,   719,   720,   721,   722,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   723,
     724,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,     0,     0,     0,     0,   682,   683,   684,   685,
     686,   723,   724,   689,   690,   691,   692,     0,   694,   695,
     696,   697,     0,     0,     0,     0,   698,     0,   700,   701,
       0,     0,     0,     0,     0,   682,   683,   684,   685,   686,
       0,     0,   689,   690,   691,   692,     0,   694,   695,   696,
     697,     0,     0,     0,     0,   698,     0,   700,   701,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,   638,     0,
       0,     0,     0,     0,     0,     0,     0,   723,   724,     0,
     296,     0,     0,     0,     0,     0,   297,     0,   714,   715,
     716,   717,   718,   719,   720,   721,   722,     0,     0,     0,
       0,     0,     0,   298,     0,     0,   723,   724,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   299,     0,
       0,     0,     0,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   640,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   296,
       0,     0,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,     0,   861,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   299,     0,     0,
       0,    55,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   300,   301,   302,     0,   304,
     305,   306,   307,   308,   437,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,     0,   322,   323,   324,
       0,     0,   327,   328,   329,   330,     0,     0,     0,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,   865,     0,     0,     0,     0,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1066,     0,     0,     0,     0,     0,     0,     0,
       0,   862,     0,     0,     0,     0,     0,     0,     0,     0,
     300,   301,   302,   863,   304,   305,   306,   307,   308,   437,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,     0,   322,   323,   324,     0,     0,   327,   328,   329,
     330,   300,   301,   302,     0,   304,   305,   306,   307,   308,
     437,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,     0,   322,   323,   324,     0,     0,   327,   328,
     329,   330,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   866,   296,     0,     0,
       0,     0,     0,   297,     0,     0,     0,     0,   867,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,     0,     0,     0,     0,  1067,     0,     0,
       0,     0,     0,     0,     0,   299,     0,     0,     0,  1068,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,     0,     0,     0,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,   298,
       0,     0,     0,     0,   333,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   299,     0,     0,     0,    55,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   174,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   333,     0,     0,     0,     0,   175,     0,
     176,     0,   177,   178,   179,   180,   181,   754,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
     193,   194,   195,     0,     0,   196,   197,   198,   199,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   200,   201,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   202
};

static const yytype_int16 yycheck[] =
{
      12,    13,   204,   218,   403,   399,   357,   368,   538,   556,
     155,   455,   550,   749,   552,   751,   554,   753,   443,   444,
     445,   363,   641,   418,   411,   825,  1008,   852,   595,   596,
       7,   831,   393,    51,    32,    21,    19,   218,    19,    14,
      15,  1118,    48,    51,   127,   440,   137,    59,    60,    61,
      98,   179,   121,   620,   621,   124,   125,    44,    19,   124,
     125,   139,   140,   141,    32,    55,     0,   122,    33,    55,
     179,    61,     6,   165,   180,    18,    19,   165,   122,   150,
     210,   209,   124,   125,   155,   165,   179,    99,   100,   101,
     102,   122,   453,   454,   661,    29,    61,    31,   122,    33,
     209,   132,   138,   209,   138,    39,    14,    15,   132,    20,
      21,   153,   122,   147,    48,   102,   209,   209,   152,   210,
      54,   209,   132,   165,   179,   180,   209,   182,   206,   209,
     185,   165,   165,   202,   203,   206,   180,   202,   203,   183,
     355,   147,    76,    46,   289,   153,   182,   542,  1276,   180,
     179,  1133,   186,   368,   185,   158,   180,   165,   725,   165,
     202,   203,   147,   211,   153,    99,   131,   122,   138,   122,
     180,   182,   205,   138,   355,   204,   165,   132,   393,   132,
     165,  1258,  1007,   158,    87,   138,  1314,   185,   163,   584,
     165,   209,   167,    20,    21,   165,   179,   179,   179,   210,
     165,   596,   113,   114,  1004,   191,   218,    20,    21,   776,
     121,   778,   123,   124,   125,   126,   202,   185,   179,   421,
     131,   211,   165,   165,   206,   180,   203,   180,   623,   186,
     205,   196,  1214,   435,   211,   203,   174,   122,   453,   454,
     205,   602,  1298,   179,   212,   670,   183,   132,   205,   175,
     158,   638,   178,   640,   175,   163,   122,   165,   192,   167,
     209,   622,   179,   183,   153,   207,   132,   187,   204,   203,
    1326,   209,   209,   211,   176,   209,   165,   188,   189,   190,
     191,   192,    59,    60,    61,   211,   113,   114,   209,   206,
     211,   202,   203,   187,   121,   180,   123,   124,   125,   126,
     113,   114,   204,   180,   131,    20,    21,   209,   121,    60,
     123,   124,   125,   126,   180,   209,  1052,   147,   131,   176,
     177,   859,    99,   100,   101,   102,   668,   179,   543,   544,
     545,   546,  1132,   179,   549,   165,   551,  1162,   553,   179,
     555,   153,   737,   355,  1144,   153,   179,   204,   360,     4,
       5,   507,     7,   165,   206,   179,   368,   165,   179,   179,
     206,   179,   543,   544,   545,   546,   206,   582,   549,   179,
     551,   204,   553,   772,   530,   202,   203,   190,   191,   192,
      35,   393,   206,   204,   586,  1004,    24,   602,   206,   202,
     203,   835,    30,   818,   165,   166,   206,   179,   113,   114,
    1019,   582,   414,   138,   179,   607,   121,   622,   182,   124,
     125,   126,   147,   179,   186,   962,   131,   153,   179,  1155,
     781,   192,   179,    32,   206,   179,   179,   179,    66,   165,
     165,   206,   999,   205,   179,    45,   797,   798,   179,   800,
     206,   453,   454,   804,   646,   206,   179,   204,   187,    58,
      59,  1018,   206,    32,   206,    65,   658,   672,   673,   204,
      98,   886,   887,   888,   165,   206,    32,   187,   182,    55,
     209,   204,    71,   187,   183,    61,    75,   838,   187,    58,
      59,   187,   138,   698,   909,   910,   911,   202,   203,   209,
    1057,   205,    58,    59,  1032,    94,    95,    96,    97,   182,
     138,   182,   182,   209,   187,    55,   187,   187,   147,   186,
     119,    61,   205,   122,   123,    55,   155,   155,   156,   157,
     202,    61,   205,   132,   205,   205,   165,   165,   205,   165,
     166,   543,   544,   545,   546,    11,   980,   549,   183,   551,
     119,   553,   187,   555,   123,    55,    22,    23,   186,    11,
     765,    61,   183,   119,    82,    83,   187,   123,    20,    21,
    1127,   179,   777,    55,   182,   780,   781,   185,    55,    61,
     582,   180,   181,   150,    61,    55,   185,   158,   155,   188,
     592,    61,   797,   798,   765,   800,   205,   789,   733,   804,
     602,   210,   206,   183,   203,   209,   777,   187,   165,   780,
     209,   183,   181,   212,   165,   187,   185,   183,   187,   188,
     622,   187,   183,   183,   165,   181,   187,   187,   979,   185,
     209,   187,   188,   838,   203,   183,   168,   169,   182,   187,
     209,   185,   165,   212,   139,   140,   141,   203,   168,   169,
     170,   171,   654,   165,   207,   208,   212,   210,   168,   111,
     112,   113,   114,   115,   168,   169,   118,   119,   120,   121,
     209,   123,   124,   125,   126,   168,   169,   170,   165,   131,
    1031,   133,   134,   139,   210,   141,    55,   139,   140,   141,
      10,    11,  1033,   145,   209,   168,   139,   102,   155,  1256,
     180,   210,   210,    34,    34,  1262,   205,   210,   913,   211,
     165,   165,   165,   204,   165,    21,   165,   182,   204,   182,
     210,   210,   205,    41,   186,   186,   186,   186,   730,   181,
     205,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   186,   947,   165,   186,   186,   186,  1136,   186,   186,
     202,   203,   183,   958,   132,   960,   165,   205,  1315,   205,
      20,    21,   165,   765,   165,   204,    41,   186,   165,  1289,
     205,   205,   357,   205,   979,   777,   947,   205,   780,   781,
     365,   158,   186,   209,   186,   205,   186,   958,   186,   205,
     205,   205,   205,   205,   205,   797,   798,   382,   800,  1319,
     208,    36,   804,   209,    20,    21,   391,     9,    64,   205,
     209,   186,    41,   398,   209,   209,   209,   209,   209,   203,
     205,  1172,   211,   408,  1213,   206,  1031,   165,   165,   209,
     204,   182,   211,   418,   205,   211,   838,   209,   423,   165,
     425,   165,  1034,   179,   127,    13,   180,   182,   158,   434,
     208,   111,   112,   113,   114,   440,   441,   442,   210,   155,
     165,   121,     7,   123,   124,   125,   126,   165,   187,  1253,
     455,   131,    41,   133,   134,   179,   205,   179,   463,   211,
     465,   466,   467,   468,   469,   470,   165,   186,   205,   205,
     205,   186,   205,   205,   479,   111,   112,   113,   114,   115,
       1,    65,   118,   119,   120,   121,   209,   123,   124,   125,
     126,   205,   205,   165,   211,   131,   187,   133,   134,  1111,
     187,   187,  1311,   165,   165,   205,   165,   205,   188,   189,
     190,   191,   192,   165,   204,   204,   204,  1288,   165,   165,
     186,   205,   202,   203,   205,   947,   205,   209,   205,    32,
     205,   204,   165,   204,   204,   209,   958,   542,   960,   206,
     165,   205,   205,   187,   187,   205,  1158,  1172,   206,  1174,
     186,   187,   188,   189,   190,   191,   192,   979,    51,  1171,
     211,   209,   204,   204,   204,   647,   202,   203,    75,     1,
      41,   128,   577,    78,  1227,    76,  1228,   216,   730,   584,
     849,  1228,  1228,  1174,  1228,  1228,     1,  1169,   992,  1190,
    1252,   596,  1086,  1193,   960,   405,  1233,    49,   603,   604,
     525,   606,  1220,   641,  1016,  1273,   611,   414,   414,  1031,
      -1,    -1,   617,    -1,    -1,    -1,    -1,    -1,   623,    -1,
    1245,    -1,    -1,    -1,    -1,    -1,    -1,   632,   633,   634,
     635,   636,   637,    -1,   639,    -1,   641,    -1,    32,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1245,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1288,    58,    59,    58,    59,    -1,   674,
     675,   676,   677,   678,   679,   680,   681,   682,   683,    -1,
      -1,   686,   687,   688,   689,   690,   691,    -1,   693,   694,
      -1,   696,   697,   698,   699,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,   710,   711,   712,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,    -1,   724,
      -1,    -1,   727,    -1,    -1,   119,    -1,   119,   122,   123,
    1342,   123,   737,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
    1172,    -1,  1174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,   779,    -1,    -1,    -1,    -1,    -1,
      -1,   786,   787,   788,    20,    21,   180,   181,    -1,   181,
      -1,   185,    -1,   185,   188,   187,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,
      -1,   203,    -1,    -1,    -1,    -1,   821,   822,   212,    -1,
     212,    -1,    -1,  1245,    -1,   830,    -1,    -1,   833,    -1,
     835,    -1,    -1,    -1,    -1,    32,   841,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   851,   121,    -1,   123,
     124,   125,   126,    -1,  1276,    -1,    -1,   131,    -1,   133,
     134,    58,    59,    -1,    -1,    -1,  1288,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,    -1,  1314,    -1,    -1,   131,   132,   133,   134,   135,
     136,    -1,    -1,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,   186,   187,   188,   189,   190,   191,   192,    -1,
      -1,   926,   119,    -1,    -1,   930,   123,    -1,   202,   203,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,   181,    -1,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,    -1,    -1,    -1,
      -1,   966,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   980,    -1,    -1,   983,    20,
      21,    -1,    -1,    -1,   181,    -1,    -1,    -1,   185,    -1,
     187,   188,    -1,    -1,    -1,    -1,  1001,  1002,  1003,    -1,
    1005,    -1,    -1,    -1,    -1,    -1,   203,    -1,  1013,    -1,
      -1,  1016,    -1,    -1,    -1,   212,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,  1029,  1030,    -1,    -1,  1033,    -1,
      -1,    -1,  1037,    -1,    -1,    -1,    -1,  1042,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1054,
      -1,  1056,    -1,    -1,    -1,    -1,  1061,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1070,    -1,    -1,    -1,  1074,
     111,   112,   113,   114,   115,    -1,    -1,   118,   119,   120,
     121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,    -1,   111,   112,   113,
     114,   115,    -1,  1118,   118,   119,   120,   121,    -1,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     181,    -1,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,
      -1,    -1,    -1,    -1,    -1,  1180,    -1,  1182,    -1,    -1,
    1185,   185,   186,   187,   188,   189,   190,   191,   192,    -1,
      -1,    -1,    -1,    -1,  1199,    -1,    -1,    -1,   202,   203,
    1205,  1206,  1207,    -1,  1209,    -1,    -1,  1212,    -1,    -1,
      -1,  1216,  1217,  1218,  1219,    -1,    -1,  1222,    -1,    -1,
      -1,    -1,    -1,  1228,    -1,    -1,   111,   112,   113,   114,
     115,  1236,    -1,   118,   119,   120,   121,    -1,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,
      -1,    -1,    -1,  1258,   139,   140,   141,    -1,    -1,    -1,
     145,    -1,  1267,  1268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1280,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1290,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,  1302,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,    -1,    -1,
      -1,  1316,    -1,    -1,    -1,  1320,    -1,   202,   203,    -1,
      -1,    -1,    -1,    -1,   209,    -1,  1331,     1,  1333,  1334,
      -1,    -1,     6,  1338,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    25,    26,    27,    28,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,
      -1,    -1,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    -1,
      94,    95,    96,    97,    98,    -1,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,   129,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,   190,    -1,   192,    -1,
      -1,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,    -1,   207,   208,   209,   210,   211,     1,    -1,
      -1,    -1,    -1,     6,    -1,     8,     9,    10,    -1,    12,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    25,    26,    27,    28,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      -1,    94,    95,    96,    97,    98,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,   129,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    32,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,   190,    -1,   192,
      -1,    -1,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   205,    -1,   207,   208,   209,   210,   211,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,   119,    -1,    -1,    43,   123,
      -1,    46,    -1,    -1,    49,    -1,    51,    -1,    53,    -1,
      -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    -1,    94,
      95,    96,    97,    98,    -1,   100,    -1,   181,    -1,    -1,
      -1,   185,    -1,   187,   188,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,   203,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
     185,    -1,    -1,   188,   189,   190,    -1,   192,    -1,    -1,
     195,   196,    -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,
     205,    -1,   207,   208,    -1,   210,    -1,   212,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    58,    59,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    -1,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,   185,
      -1,    -1,   188,   189,   190,    -1,   192,    -1,    -1,   195,
     196,    -1,    58,    59,    -1,    -1,    -1,   203,    -1,   205,
      -1,   207,   208,    -1,   210,    -1,   212,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,    -1,    63,   123,    -1,    66,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    -1,    94,    95,    96,
      97,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,   185,
      -1,   187,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,   203,    -1,   146,
     147,   148,   149,   150,    -1,   152,   212,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
      -1,   188,   189,   190,    -1,   192,    -1,    -1,   195,   196,
      -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,   205,    -1,
     207,   208,    -1,   210,    -1,   212,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    91,    -1,    -1,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,   189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,
      -1,    14,    15,    16,    17,    18,    -1,   205,    -1,   207,
     208,    24,   210,    26,    -1,    -1,    32,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      -1,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,    -1,   119,    -1,    -1,    -1,   123,    -1,   122,
      -1,    -1,    -1,    -1,    -1,   128,   129,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,   185,
      -1,   187,   188,    -1,    -1,   188,   189,   190,    -1,   192,
      -1,    -1,   195,   196,    -1,    -1,    -1,   203,    -1,    -1,
      -1,    -1,   205,    -1,   207,   208,   212,   210,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    25,
      26,    27,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    -1,    94,    95,
      96,    97,    98,    -1,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,   190,    -1,   192,    -1,    -1,   195,
     196,    -1,    -1,    14,    15,    16,    17,    18,    -1,   205,
      -1,   207,   208,    24,   210,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    -1,    94,    95,    96,    97,    98,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,
      -1,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,   190,
      -1,   192,    -1,    -1,   195,   196,    -1,    -1,    14,    15,
      16,    17,    18,    -1,   205,    -1,   207,   208,    24,   210,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    -1,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,   190,    -1,   192,    -1,    -1,   195,
     196,    -1,    -1,    14,    15,    16,    17,    18,    -1,   205,
      -1,   207,   208,    24,   210,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    -1,    94,    95,    96,    97,    98,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,
      -1,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,   190,
      -1,   192,    -1,    -1,   195,   196,    -1,    -1,    14,    15,
      16,    17,    18,    -1,   205,    -1,   207,   208,    24,   210,
      26,    -1,    -1,    32,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    -1,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
     119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,   147,   148,   149,   150,    -1,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,   185,    -1,   187,   188,
      -1,    -1,   188,   189,   190,    -1,   192,    -1,    -1,   195,
     196,    -1,    -1,    -1,   203,    -1,    -1,   203,    -1,   205,
     206,   207,   208,   212,   210,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    -1,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,
      14,    15,    16,    17,    18,   204,   205,    -1,   207,   208,
      24,   210,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    -1,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,   190,    -1,   192,    -1,
      -1,   195,   196,    -1,    -1,    14,    15,    16,    17,    18,
      -1,   205,    -1,   207,   208,    24,   210,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    -1,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,
      14,    15,    16,    17,    18,    -1,   205,    -1,   207,   208,
      24,   210,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    -1,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,    -1,   188,   189,   190,    -1,   192,    -1,
      -1,   195,   196,    -1,    -1,    14,    15,    16,    17,    18,
      -1,   205,    21,   207,   208,    24,   210,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    -1,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,
      14,    15,    16,    17,    18,    -1,   205,    -1,   207,   208,
      24,   210,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    -1,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,   190,    -1,   192,    -1,
      -1,   195,   196,    -1,    -1,    14,    15,    16,    17,    18,
      -1,   205,   206,   207,   208,    24,   210,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    -1,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,
      14,    15,    16,    17,    18,    -1,   205,   206,   207,   208,
      24,   210,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    -1,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,    -1,   188,   189,   190,    -1,   192,    -1,
      -1,   195,   196,    -1,    -1,    14,    15,    16,    17,    18,
      -1,   205,    -1,   207,   208,    24,   210,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    -1,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,
      14,    15,    16,    17,    18,    -1,   205,   206,   207,   208,
      24,   210,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    -1,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,   190,    -1,   192,    -1,
      -1,   195,   196,    -1,    -1,    14,    15,    16,    17,    18,
      -1,   205,   206,   207,   208,    24,   210,    26,    -1,    -1,
      32,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    58,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    -1,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,   119,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,   185,    -1,   187,   188,    -1,    -1,   188,
     189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,
      -1,   203,    -1,    -1,   203,    -1,   205,    -1,   207,   208,
     212,   210,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    66,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    -1,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,   190,    -1,
     192,    -1,    -1,   195,   196,    -1,    -1,    14,    15,    16,
      17,    18,    -1,   205,   206,   207,   208,    24,   210,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    -1,    94,    95,    96,
      97,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,   190,    -1,   192,    -1,    -1,   195,   196,
      -1,    -1,    14,    15,    16,    17,    18,    -1,   205,   206,
     207,   208,    24,   210,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    66,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    -1,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,   190,    -1,
     192,    -1,    -1,   195,   196,    -1,    -1,    14,    15,    16,
      17,    18,    -1,   205,   206,   207,   208,    24,   210,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    -1,    94,    95,    96,
      97,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,   190,    -1,   192,    -1,    -1,   195,   196,
      -1,    -1,    14,    15,    16,    17,    18,    -1,   205,    -1,
     207,   208,    24,   210,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    66,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    -1,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,   189,   190,    -1,
     192,    -1,    -1,   195,   196,    -1,    -1,    14,    15,    16,
      17,    18,    -1,   205,    -1,   207,   208,    24,   210,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    -1,    94,    95,    96,
      97,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,
     147,   148,   149,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,   189,   190,    -1,   192,    -1,    -1,   195,   196,
      -1,    -1,    14,    15,    16,    17,    18,    -1,   205,    -1,
     207,   208,    24,   210,    26,    -1,    -1,    32,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    66,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    -1,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,   119,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
     185,    -1,   187,   188,    -1,    -1,   188,   189,   190,    -1,
     192,   119,    -1,   195,   196,   123,    -1,    -1,   203,    -1,
      -1,    -1,    -1,   205,    -1,   207,   208,   212,   210,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,    20,    21,    -1,    -1,   131,   132,   133,
     134,   135,   136,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,    -1,   181,    -1,    -1,    -1,   185,    -1,   187,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   212,    -1,   180,   181,    -1,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,    -1,    -1,    -1,   209,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,   136,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,    -1,
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,    20,    21,    -1,
      -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,   180,   181,   145,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,   202,   203,    -1,    -1,    -1,
      -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,    -1,    -1,    -1,   209,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,   181,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    24,    -1,    -1,    -1,   209,    -1,    30,    -1,
      -1,    -1,    -1,   181,    -1,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,
      62,   209,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,    -1,
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,   131,    -1,   133,   134,    20,    21,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     212,    -1,    -1,    -1,    -1,    32,   111,   112,   113,   114,
     115,    -1,    -1,   118,   119,   120,   121,    -1,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,
      -1,    58,    59,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,   111,   112,   113,   114,   115,    20,    21,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,   181,    -1,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    -1,   123,   202,   203,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,   206,   111,   112,   113,
     114,   115,    20,    21,   118,   119,   120,   121,    -1,   123,
     124,   125,   126,    -1,   181,    -1,    -1,   131,   185,   133,
     134,   188,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,   203,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,   206,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,   111,   112,
     113,   114,   115,    20,    21,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,   181,    -1,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,   206,   111,   112,   113,   114,   115,    20,
      21,   118,   119,   120,   121,    -1,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,
     111,   112,   113,   114,   115,    -1,    -1,   118,   119,   120,
     121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,   111,   112,   113,   114,   115,
      20,    21,   118,   119,   120,   121,    -1,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
     181,    -1,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
     206,   111,   112,   113,   114,   115,    20,    21,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,   206,   111,   112,   113,
     114,   115,    -1,    -1,   118,   119,   120,   121,    -1,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,   111,   112,   113,   114,   115,    20,    21,   118,
     119,   120,   121,    -1,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,   181,    -1,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,   206,   111,   112,
     113,   114,   115,    20,    21,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,   206,   111,   112,   113,   114,   115,    -1,
      -1,   118,   119,   120,   121,    -1,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,   111,
     112,   113,   114,   115,    20,    21,   118,   119,   120,   121,
      -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,   181,    -1,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,    -1,    -1,   206,   111,   112,   113,   114,   115,
      20,    21,   118,   119,   120,   121,    -1,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
     206,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,   111,   112,   113,   114,
     115,    20,    21,   118,   119,   120,   121,    -1,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,   181,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,   206,   111,   112,   113,   114,   115,    20,    21,   118,
     119,   120,   121,    -1,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,   206,   111,   112,
     113,   114,   115,    -1,    -1,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,   111,   112,   113,   114,   115,    20,    21,
     118,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,   181,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,   111,
     112,   113,   114,   115,    20,    21,   118,   119,   120,   121,
      -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,    -1,    -1,   206,   111,   112,   113,   114,   115,
      -1,    -1,   118,   119,   120,   121,    -1,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
     111,   112,   113,   114,   115,    20,    21,   118,   119,   120,
     121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,   181,    -1,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,   206,   111,   112,   113,   114,
     115,    20,    21,   118,   119,   120,   121,    -1,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,   206,   111,   112,   113,   114,   115,    -1,    -1,   118,
     119,   120,   121,    -1,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,   145,   111,   112,   113,
     114,   115,    20,    21,   118,   119,   120,   121,    -1,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,   181,    -1,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,   206,   111,   112,   113,   114,   115,    20,    21,
     118,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,   111,
     112,   113,   114,   115,    -1,    -1,   118,   119,   120,   121,
      -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,
      -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,   111,   112,   113,   114,   115,    20,
      21,   118,   119,   120,   121,    -1,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,   181,
      -1,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,    -1,
     111,   112,   113,   114,   115,    20,    21,   118,   119,   120,
     121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,   204,    -1,    -1,   111,   112,   113,   114,
     115,    -1,    -1,   118,   119,   120,   121,    -1,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,   111,   112,   113,   114,   115,    20,    21,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,   181,    -1,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,   204,    -1,    -1,   111,   112,   113,
     114,   115,    -1,    -1,   118,   119,   120,   121,    -1,   123,
     124,   125,   126,    20,    21,    -1,    -1,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    -1,    20,    21,   131,   132,   133,   134,   135,   136,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,   181,    -1,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   111,   112,
     113,   114,   115,    20,    21,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,
     133,   134,    -1,    -1,   137,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,   181,    -1,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,    -1,    -1,   195,   196,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,    -1,   111,   112,   113,   114,   115,    20,
      21,   118,   119,   120,   121,    -1,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,
     137,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,   119,   120,
     121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
     131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,   111,   112,   113,   114,   115,
      20,    21,   118,   119,   120,   121,    -1,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,    -1,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
      -1,   111,   112,   113,   114,   115,    20,    21,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,    -1,   111,   112,   113,
     114,   115,    -1,    -1,   118,   119,   120,   121,    -1,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,   111,   112,   113,   114,   115,    20,    21,   118,
     119,   120,   121,    -1,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,    -1,   181,    -1,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,   119,   120,
     121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
     131,    -1,   133,   134,    20,    21,    -1,    -1,   139,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
     115,   202,   203,   118,   119,   120,   121,    -1,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,   115,
      -1,    -1,   118,   119,   120,   121,    -1,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,   184,   185,
     186,   187,   188,   189,   190,   191,   192,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,   202,   203,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    18,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,   165,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,   165,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    -1,    94,    95,    96,
      97,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    -1,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,   165,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,   165,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    67,    -1,
      69,    -1,    71,    72,    73,    74,    75,   165,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    -1,    94,    95,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   214,     0,     6,    29,    31,    33,    39,    48,    54,
      76,    99,   192,   203,   209,   215,   218,   224,   226,   227,
     232,   260,   264,   290,   362,   369,   373,   383,   423,   428,
      18,    19,   165,   253,   254,   255,   158,   233,   234,   165,
     166,   192,   228,   229,   230,   210,   370,   165,   207,   217,
      55,    61,   365,   365,   138,   165,   277,    33,    61,   131,
     196,   205,   256,   257,   258,   259,   277,   209,     4,     5,
       7,    35,   381,    60,   360,   180,   179,   182,   179,   229,
      21,    55,   191,   202,   231,   365,   366,   368,   366,   360,
     429,   424,   165,   138,   225,   258,   258,   258,   205,   139,
     140,   141,   179,   204,    55,    61,   265,   267,    55,    61,
     371,    55,    61,   382,    55,    61,   361,    14,    15,   158,
     163,   165,   167,   205,   220,   254,   158,   234,   165,   228,
     228,   165,   209,   211,   366,   209,    55,    61,   216,   165,
     165,   165,   168,   223,   206,   255,   258,   258,   258,   258,
     268,   165,   372,   384,   210,   363,   168,   169,   219,    14,
      15,   158,   163,   165,   220,   251,   252,   231,   367,   209,
     430,   425,   168,   206,    34,    67,    69,    71,    72,    73,
      74,    75,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    89,    90,    91,    94,    95,    96,    97,
     113,   114,   165,   263,   266,   182,   210,   102,   379,   380,
     358,   155,   335,   168,   169,   170,   179,   206,   180,   210,
     210,    20,    21,    37,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   123,   124,   125,   132,   133,   134,   135,   136,
     139,   140,   141,   142,   143,   144,   145,   181,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   195,   196,
     202,   203,    34,    34,   205,   261,   210,   269,    71,    75,
      94,    95,    96,    97,   388,   374,   165,   385,   211,   359,
     255,   147,   165,   356,   357,   251,    24,    30,    47,    62,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   151,   277,   387,   389,   390,   393,   397,
     422,   431,   426,   165,   165,   165,   204,    21,   165,   204,
     150,   206,   335,   345,   346,   182,   262,   271,   210,   364,
     182,   378,   210,   335,   204,   205,    41,   179,   182,   185,
     355,   186,   186,   186,   186,   205,   186,   186,   186,   186,
     186,   186,   205,    32,    58,    59,   119,   123,   181,   185,
     188,   203,   212,   183,   392,   351,   165,   132,   205,    48,
     147,   165,   353,   386,   206,   209,   422,     1,     8,     9,
      10,    12,    14,    15,    16,    17,    18,    24,    25,    26,
      27,    28,    30,    37,    38,    40,    42,    43,    46,    49,
      50,    52,    53,    56,    57,    63,    66,    76,    98,   100,
     101,   113,   114,   128,   129,   130,   146,   147,   148,   149,
     150,   152,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   166,   167,   188,   189,   190,   195,   196,
     205,   207,   208,   209,   211,   222,   224,   235,   236,   239,
     242,   243,   245,   246,   247,   248,   249,   269,   270,   272,
     273,   276,   277,   278,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   294,   295,   302,   305,   308,   312,   313,
     314,   315,   316,   317,   319,   324,   327,   334,   387,   433,
     442,   445,   447,   454,   456,   376,   165,   209,   375,   277,
     341,   357,   204,    63,   166,   327,   165,   165,   397,   122,
     132,   180,   354,   400,   402,   404,   398,   165,   394,   406,
     408,   410,   412,   414,   416,   418,   420,   327,    32,   185,
      32,   185,   203,   212,   204,   327,   212,   397,   165,   209,
     432,   165,   209,   349,   427,   327,   353,   205,    41,   122,
     179,   180,   182,   185,   352,   345,   327,   386,   186,   279,
     389,   433,   205,   186,   186,   205,   122,   276,   306,   314,
     327,   269,   186,   205,    59,   327,   205,   327,   165,   186,
     186,   205,   209,   186,   158,    56,   327,   205,   269,   186,
     205,   205,   186,   122,   276,   327,   327,   327,   208,   312,
     312,   312,   205,   205,   205,   205,   205,   205,    12,   397,
      12,   397,   327,   444,   452,   186,   327,   221,   327,   327,
     327,   327,   327,   327,   208,    36,   209,   209,   327,     9,
     165,   205,   209,   209,   209,   209,   209,    64,   291,   260,
     127,   209,    20,    21,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   131,   132,
     133,   134,   135,   136,   139,   140,   141,   142,   143,   144,
     145,   180,   181,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   202,   203,   205,   364,   180,   209,   211,
     203,   211,   260,   342,   206,    41,   209,   354,   276,   327,
     422,   396,   422,   422,   422,   206,   165,   391,   422,   261,
     422,   261,   422,   261,   165,   347,   348,   422,   349,   350,
     206,   204,   204,   209,   211,   182,   209,   211,   206,   327,
     165,   165,   422,   276,   327,   269,    19,   280,   205,   277,
     303,   448,   306,   327,   276,   306,   179,   122,   180,    13,
     397,   444,   453,   327,   327,   269,   180,   296,   298,   327,
     300,   182,   158,   327,   450,   306,   306,   397,   276,   327,
     208,   327,   327,   327,   327,   327,   327,   386,    51,   153,
     165,   188,   205,   327,   434,   435,   436,   443,   446,   386,
     205,   435,   446,   137,   174,   209,   211,   437,   274,   269,
     170,   171,   219,   206,   147,   152,   186,   277,   318,   210,
     269,   244,   203,   206,   306,   155,   293,   357,   155,   311,
     312,    18,   153,   165,   387,    18,   153,   165,   387,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   165,   128,   129,   130,   327,
     327,   153,   165,   327,   327,   327,   387,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   128,
     129,   130,   327,    21,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   124,   125,   153,   165,   202,
     203,   325,   327,   206,   306,   377,   327,   259,     7,   335,
     340,   165,   276,   327,   209,   187,   187,   209,   187,   187,
     187,   209,   187,   262,   187,   262,   187,   262,   182,   187,
     209,   187,   209,   422,   206,    41,   352,   306,   422,   206,
     306,   186,   327,   422,   397,   206,   327,   269,   187,   209,
     179,   206,   206,   250,   397,   397,   206,   397,   206,   397,
     206,   206,   187,   206,   206,   206,   206,   206,   206,    19,
     312,   205,   132,   352,   206,   137,   179,   209,   436,   176,
     177,   204,   440,   179,   176,   204,   209,   439,    19,   206,
     436,   175,   211,   438,   327,   444,   211,   397,   327,   205,
     205,   320,   322,   271,    11,    22,    23,   237,   238,   327,
     165,   434,   179,   206,   165,   292,   122,   132,   180,   185,
     309,   310,   261,   186,   205,   186,   205,   205,   312,   312,
     312,   205,   204,   312,   312,   312,    18,   153,   165,   387,
     182,   153,   165,   327,   205,   153,   165,   327,     1,   204,
     206,   211,   209,   204,    55,    61,   338,    65,   339,   209,
     209,   401,   403,   422,   405,   399,   395,   165,   407,   187,
     411,   187,   415,   187,   422,   419,   347,   421,   349,   165,
     327,   209,   187,   206,   165,   206,   187,   187,   205,   397,
     444,   327,   187,   187,   187,   187,   311,   205,   306,   327,
     327,   327,   435,   436,   327,   153,   165,   434,   440,   204,
     327,   204,   443,   306,   435,   175,   178,   211,   441,   204,
     187,   172,   327,   327,   397,   261,   211,   269,   327,    11,
     240,   204,   203,   179,   204,   165,   165,   165,   165,   179,
     204,   262,   328,   327,   330,   327,   206,   306,   327,   186,
     205,   327,   205,   204,   327,   205,   204,   326,   209,    45,
     339,    44,   102,   336,   409,   413,   417,   269,   281,   187,
     209,   304,   449,   453,   187,   297,   299,   301,   451,   205,
     206,   306,   209,   206,   436,   440,   205,   132,   352,   209,
     436,   204,   275,   206,   206,   187,   262,   269,   241,   209,
     206,   434,   165,   204,   309,   204,   137,   269,   307,   397,
     206,   422,   206,   206,   206,   332,   327,   327,   206,   327,
      32,   337,   336,   338,   327,   165,   205,   206,   205,   327,
     327,   327,   205,   206,   327,   206,   327,   132,   352,   440,
     327,   327,   327,   327,   441,   327,   321,   187,   238,    25,
     101,   242,   283,   284,   285,   287,   327,   204,   182,   355,
     122,   327,   187,   187,   422,   206,   206,   206,   343,   337,
     353,   206,   187,   306,   453,   306,   206,   209,   455,   327,
     327,   206,   455,   318,   323,   122,   122,   327,   206,   397,
     354,   327,   329,   331,   187,   266,   344,   327,   206,   206,
     206,    51,   204,   132,   352,   211,   318,   327,   354,   327,
     333,   209,   266,   206,   327,   204,   327,   327,   204,   327,
     209,   269,   209
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   213,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   215,   216,   216,   216,
     217,   217,   218,   219,   219,   219,   219,   220,   221,   221,
     221,   222,   223,   223,   225,   224,   226,   227,   228,   228,
     229,   229,   229,   229,   230,   230,   231,   231,   232,   233,
     233,   234,   234,   235,   236,   236,   237,   237,   238,   238,
     238,   239,   239,   240,   241,   240,   242,   242,   242,   242,
     242,   243,   244,   243,   245,   246,   247,   248,   250,   249,
     251,   251,   251,   251,   251,   251,   252,   252,   253,   253,
     253,   254,   254,   254,   254,   254,   254,   254,   254,   255,
     255,   256,   256,   256,   257,   257,   258,   258,   258,   258,
     258,   258,   258,   259,   259,   260,   260,   261,   261,   261,
     262,   262,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   264,   265,   265,
     265,   266,   268,   267,   269,   269,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   271,   271,   271,
     272,   274,   275,   273,   276,   276,   276,   276,   277,   277,
     277,   278,   278,   280,   281,   279,   279,   282,   282,   282,
     282,   283,   284,   285,   285,   285,   286,   286,   286,   287,
     287,   288,   288,   288,   289,   290,   290,   291,   291,   292,
     292,   293,   293,   294,   294,   296,   297,   295,   298,   299,
     295,   300,   301,   295,   303,   304,   302,   305,   305,   305,
     306,   306,   307,   307,   307,   308,   308,   308,   309,   309,
     309,   309,   310,   310,   311,   311,   312,   312,   313,   313,
     313,   313,   313,   313,   313,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   315,   315,   315,   315,   315,   315,
     316,   316,   317,   317,   318,   318,   319,   320,   321,   319,
     322,   323,   319,   324,   324,   325,   326,   324,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   328,   329,   327,
     327,   327,   327,   330,   331,   327,   327,   327,   332,   333,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   335,   335,   336,   336,   336,   337,   337,   338,   338,
     338,   339,   339,   340,   341,   342,   341,   343,   341,   344,
     341,   341,   345,   345,   345,   346,   346,   347,   347,   348,
     348,   349,   350,   350,   351,   351,   351,   352,   352,   353,
     353,   353,   353,   353,   353,   354,   354,   354,   355,   355,
     356,   356,   356,   356,   356,   357,   357,   357,   357,   357,
     358,   359,   358,   360,   360,   361,   361,   361,   362,   363,
     362,   364,   364,   364,   364,   365,   365,   365,   367,   366,
     368,   368,   369,   370,   369,   371,   371,   371,   372,   374,
     375,   373,   376,   377,   373,   378,   378,   379,   379,   380,
     381,   381,   382,   382,   382,   384,   385,   383,   386,   386,
     386,   386,   386,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     388,   388,   388,   388,   388,   388,   389,   390,   390,   390,
     391,   391,   392,   392,   392,   394,   395,   393,   396,   396,
     397,   397,   397,   397,   397,   397,   397,   397,   397,   397,
     397,   397,   397,   397,   397,   397,   397,   397,   398,   399,
     397,   397,   400,   401,   397,   402,   403,   397,   404,   405,
     397,   397,   406,   407,   397,   408,   409,   397,   397,   410,
     411,   397,   412,   413,   397,   397,   414,   415,   397,   416,
     417,   397,   418,   419,   397,   420,   421,   397,   422,   422,
     422,   424,   425,   426,   427,   423,   429,   430,   431,   432,
     428,   433,   433,   433,   433,   433,   434,   434,   434,   434,
     434,   434,   434,   434,   435,   435,   436,   436,   437,   437,
     438,   438,   439,   439,   440,   440,   440,   441,   441,   441,
     442,   442,   442,   442,   442,   442,   443,   443,   443,   444,
     444,   445,   446,   446,   447,   447,   447,   448,   449,   447,
     447,   450,   451,   447,   452,   452,   453,   453,   454,   454,
     454,   454,   455,   455,   456,   456
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
       3,     2,     2,     3,     4,     1,     1,     1,     0,     1,
       3,     9,     8,     3,     3,     0,     0,     7,     0,     0,
       7,     0,     0,     7,     0,     0,     6,     5,     8,    10,
       1,     3,     1,     2,     3,     1,     1,     2,     2,     2,
       2,     2,     1,     3,     0,     4,     1,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     4,     4,     4,     4,     4,     4,
       6,     8,     5,     6,     1,     4,     3,     0,     0,     8,
       0,     0,     9,     3,     4,     0,     0,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     3,     4,
       5,     4,     5,     3,     4,     1,     3,     4,     3,     4,
       2,     4,     4,     7,     8,     3,     5,     0,     0,     8,
       3,     3,     3,     0,     0,     8,     3,     4,     0,     0,
       9,     4,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     2,     4,     1,     4,     4,     4,     4,     4,
       1,     6,     7,     6,     6,     7,     7,     6,     7,     6,
       6,     0,     4,     0,     1,     1,     0,     1,     0,     1,
       1,     0,     1,     5,     0,     0,     4,     0,     9,     0,
      10,     5,     2,     3,     4,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     0,     2,     3,     1,     1,     1,
       2,     3,     5,     3,     3,     1,     1,     1,     0,     1,
       1,     4,     3,     3,     5,     4,     6,     5,     5,     4,
       0,     0,     4,     0,     1,     0,     1,     1,     6,     0,
       6,     0,     2,     3,     5,     0,     1,     1,     0,     5,
       2,     3,     4,     0,     4,     0,     1,     1,     1,     0,
       0,     9,     0,     0,    11,     0,     2,     0,     1,     3,
       1,     1,     0,     1,     1,     0,     0,     9,     1,     4,
       3,     3,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     3,     0,     2,     3,     0,     0,     6,     1,     3,
       1,     1,     1,     1,     4,     3,     4,     2,     2,     3,
       2,     3,     2,     2,     3,     3,     3,     2,     0,     0,
       6,     2,     0,     0,     6,     0,     0,     6,     0,     0,
       6,     1,     0,     0,     6,     0,     0,     7,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     0,     0,     6,     0,     0,     6,     1,     3,
       3,     0,     0,     0,     0,    10,     0,     0,     0,     0,
      10,     1,     1,     1,     1,     1,     3,     3,     5,     5,
       6,     6,     8,     8,     1,     3,     0,     2,     2,     1,
       2,     1,     2,     1,     2,     1,     1,     2,     1,     1,
       5,     4,     6,     7,     5,     7,     1,     3,     3,     3,
       1,     4,     1,     3,     4,     4,     4,     0,     0,     9,
       4,     0,     0,     9,     1,     3,     1,     3,     3,     4,
       7,     9,     0,     3,    10,    10
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

    case YYSYMBOL_tuple_expansion: /* tuple_expansion  */
            { delete ((*yyvaluep).pNameList); }
        break;

    case YYSYMBOL_tuple_expansion_variable_declaration: /* tuple_expansion_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
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

    case YYSYMBOL_variant_alias_type_list: /* variant_alias_type_list  */
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

    case YYSYMBOL_bitfield_alias_bits: /* bitfield_alias_bits  */
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

    case YYSYMBOL_make_tuple_call: /* make_tuple_call  */
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

    case YYSYMBOL_expr_map_tuple_list: /* expr_map_tuple_list  */
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
        FileInfo * info = nullptr;
        if ( auto seqt = yyextra->g_ReaderMacro->suffix(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, thisLine, info, tokAt(scanner,(yylsp[0]))) ) {
            das_accept_sequence(scanner,seqt,strlen(seqt),thisLine,info);
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
            for ( auto & opt : *(yyvsp[0].aaList) ) {
                if ( yyextra->g_Access->isOptionAllowed(opt.name, yyextra->g_Program->thisModuleName) ) {
                    yyextra->g_Program->options.push_back(opt);
                } else {
                    das_yyerror(scanner,"option " + opt.name + " is not allowed here",
                        tokAt(scanner,(yylsp[0])), CompilationError::invalid_option);
                }
            }
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

  case 264: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 265: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
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

  case 279: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 280: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 281: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 282: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-6]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 283: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 284: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 285: /* $@10: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 286: /* $@11: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 287: /* expr_cast: "cast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 288: /* $@12: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 289: /* $@13: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 290: /* expr_cast: "upcast" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 291: /* $@14: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 292: /* $@15: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 293: /* expr_cast: "reinterpret" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 294: /* $@16: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 295: /* $@17: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 296: /* expr_type_decl: "type" '<' $@16 type_declaration '>' $@17  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 297: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 298: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 299: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 300: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 301: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 302: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 303: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 304: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 305: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 306: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 307: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 308: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 309: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 310: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 311: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 312: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 313: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 314: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 315: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 316: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 317: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 318: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 319: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 320: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 321: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 322: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 323: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 324: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 325: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 326: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 335: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 344: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 345: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr_assign_pipe: expr '=' "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr_assign_pipe: expr "<-" "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 351: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 352: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 353: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 354: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 355: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 356: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 357: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 358: /* $@19: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 359: /* func_addr_expr: '@' '@' '<' $@18 type_declaration_no_options '>' $@19 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 360: /* $@20: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 361: /* $@21: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 362: /* func_addr_expr: '@' '@' '<' $@20 optional_function_argument_list optional_function_type '>' $@21 func_addr_name  */
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

  case 363: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 364: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 365: /* $@22: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 366: /* $@23: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 367: /* expr_field: expr '.' $@22 error $@23  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 368: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 369: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 370: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 371: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 372: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 373: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 374: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 375: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 376: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 377: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 378: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 402: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 404: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 405: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 406: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 407: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 408: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 409: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 410: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 411: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 412: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 413: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 414: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 415: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 416: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 417: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 418: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 419: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 420: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 421: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 422: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 423: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 424: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 425: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 426: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 427: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 428: /* $@25: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 429: /* expr: expr "is" "type" '<' $@24 type_declaration_no_options '>' $@25  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 430: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 431: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 432: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 433: /* $@26: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 434: /* $@27: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 435: /* expr: expr "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 436: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 437: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 438: /* $@28: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 439: /* $@29: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 440: /* expr: expr '?' "as" "type" '<' $@28 type_declaration '>' $@29  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 441: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 442: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 443: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 444: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 445: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 446: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 447: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 448: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 449: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 450: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 451: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 452: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 453: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 454: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 455: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 456: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 457: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 458: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 459: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 460: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 461: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 462: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 463: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 464: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 465: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 466: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 467: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 468: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 469: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 470: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 471: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 472: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 473: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 474: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 475: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 476: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 477: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 478: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 479: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 480: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 481: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 482: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 483: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 484: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 485: /* $@30: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 486: /* struct_variable_declaration_list: struct_variable_declaration_list $@30 structure_variable_declaration ';'  */
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

  case 487: /* $@31: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 488: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@31 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 489: /* $@32: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 490: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@32 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 491: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 492: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 493: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 494: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 495: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 496: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 497: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 498: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 499: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 500: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 501: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 502: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 503: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 504: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 505: /* variant_alias_type_list: variant_alias_type_list ';'  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 506: /* variant_alias_type_list: variant_alias_type_list variant_type ';'  */
                                                             {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[-1].pVarDecl)->pNameList) ) {
                    crd->afterVariantEntry(nl.name.c_str(), nl.at);
                }
            }
        }
    }
    break;

  case 507: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 508: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 509: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 510: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 511: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 512: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 513: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 514: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 515: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 516: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 517: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 518: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 519: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 520: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 521: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 522: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 523: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 524: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 525: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 526: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 527: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 528: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 529: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 530: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 531: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 532: /* global_variable_declaration_list: global_variable_declaration_list $@33 optional_field_annotation let_variable_declaration  */
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

  case 533: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 534: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 535: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 536: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 537: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 538: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 539: /* $@34: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 540: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@34 optional_field_annotation let_variable_declaration  */
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

  case 541: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 542: /* enum_list: enum_list ';'  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 543: /* enum_list: enum_list "name" ';'  */
                                     {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt(scanner,(yylsp[-1]))) ) {
            das_yyerror(scanner,"enumeration already declared " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[-1].s)->c_str(), tokName);
            }
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
    break;

  case 544: /* enum_list: enum_list "name" '=' expr ';'  */
                                                     {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt(scanner,(yylsp[-3]))) ) {
            das_yyerror(scanner,"enumeration value already declared " + *(yyvsp[-3].s), tokAt(scanner,(yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[-3].s)->c_str(), tokName);
            }
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
    break;

  case 545: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 546: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 547: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 548: /* $@35: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 549: /* single_alias: optional_public_or_private_alias "name" $@35 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
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

  case 553: /* $@36: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 555: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 556: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 557: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 558: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 559: /* $@37: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 560: /* $@38: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 561: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' $@37 enum_list $@38 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 562: /* $@39: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 563: /* $@40: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 564: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' $@39 enum_list $@40 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 565: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 566: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 567: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 568: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 569: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 570: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 571: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 572: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 573: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 574: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 575: /* $@41: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 576: /* $@42: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 577: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@41 structure_name $@42 '{' struct_variable_declaration_list '}'  */
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

  case 578: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 579: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 580: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 581: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 582: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 583: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 584: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 585: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 586: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 587: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 588: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 589: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 590: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 591: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 592: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 593: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 594: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 595: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 596: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 597: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 598: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 599: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 600: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 601: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 602: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 603: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 604: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 605: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 606: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 607: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 608: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 609: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 610: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 611: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 612: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 613: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 614: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 615: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 616: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 617: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 618: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 619: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 620: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 621: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 622: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 623: /* bitfield_alias_bits: bitfield_alias_bits ';'  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 624: /* bitfield_alias_bits: bitfield_alias_bits "name" ';'  */
                                                 {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[-1].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-1].s)->c_str(),atvname);
        }
        delete (yyvsp[-1].s);
    }
    break;

  case 625: /* $@43: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 626: /* $@44: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 627: /* bitfield_type_declaration: "bitfield" '<' $@43 bitfield_bits '>' $@44  */
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

  case 628: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 629: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 630: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 631: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 632: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 633: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 634: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
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

  case 635: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 636: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 637: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 638: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 639: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 640: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 641: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 642: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 643: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 644: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 645: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 646: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 647: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 648: /* $@45: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 649: /* $@46: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 650: /* type_declaration_no_options: "smart_ptr" '<' $@45 type_declaration '>' $@46  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 651: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 652: /* $@47: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 653: /* $@48: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 654: /* type_declaration_no_options: "array" '<' $@47 type_declaration '>' $@48  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 655: /* $@49: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 656: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 657: /* type_declaration_no_options: "table" '<' $@49 table_type_pair '>' $@50  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 658: /* $@51: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 659: /* $@52: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 660: /* type_declaration_no_options: "iterator" '<' $@51 type_declaration '>' $@52  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 661: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 662: /* $@53: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 663: /* $@54: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 664: /* type_declaration_no_options: "block" '<' $@53 type_declaration '>' $@54  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 665: /* $@55: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 666: /* $@56: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 667: /* type_declaration_no_options: "block" '<' $@55 optional_function_argument_list optional_function_type '>' $@56  */
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

  case 668: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 669: /* $@57: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 670: /* $@58: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 671: /* type_declaration_no_options: "function" '<' $@57 type_declaration '>' $@58  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 672: /* $@59: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 673: /* $@60: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 674: /* type_declaration_no_options: "function" '<' $@59 optional_function_argument_list optional_function_type '>' $@60  */
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

  case 675: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 676: /* $@61: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 677: /* $@62: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 678: /* type_declaration_no_options: "lambda" '<' $@61 type_declaration '>' $@62  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 679: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 680: /* $@64: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 681: /* type_declaration_no_options: "lambda" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 682: /* $@65: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 683: /* $@66: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 684: /* type_declaration_no_options: "tuple" '<' $@65 tuple_type_list '>' $@66  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 685: /* $@67: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 686: /* $@68: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 687: /* type_declaration_no_options: "variant" '<' $@67 variant_type_list '>' $@68  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 688: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 689: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 690: /* type_declaration: type_declaration '|' '#'  */
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

  case 691: /* $@69: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 692: /* $@70: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 693: /* $@71: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 694: /* $@72: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 695: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@69 "name" $@70 '{' $@71 variant_alias_type_list $@72 '}'  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-6].s);
        vtype->at = tokAt(scanner,(yylsp[-6]));
        vtype->isPrivateAlias = !(yyvsp[-8].b);
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-6].s),tokAt(scanner,(yylsp[-6])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-6].s)->c_str(),atvname);
        }
        delete (yyvsp[-6].s);
    }
    break;

  case 696: /* $@73: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 697: /* $@74: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 698: /* $@75: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 699: /* $@76: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 700: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@73 "name" $@74 '{' $@75 bitfield_alias_bits $@76 '}'  */
          {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-6].s);
        btype->at = tokAt(scanner,(yylsp[-6]));
        btype->argNames = *(yyvsp[-2].pNameList);
        btype->isPrivateAlias = !(yyvsp[-8].b);
        if ( btype->argNames.size()>32 ) {
            das_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-6])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-6].s),tokAt(scanner,(yylsp[-6])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-6].s)->c_str(),atvname);
        }
        delete (yyvsp[-6].s);
        delete (yyvsp[-2].pNameList);
    }
    break;

  case 701: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 702: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 703: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 704: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 705: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 706: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 707: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 708: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 709: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 710: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 711: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 712: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 713: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 714: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 715: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 716: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 717: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 730: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 731: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 732: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 733: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 734: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 735: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 736: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 737: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 738: /* make_tuple: make_tuple ',' expr  */
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

  case 739: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 740: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 741: /* make_tuple_call: "tuple" '(' expr_list ')'  */
                                                     {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-3])));
        mkt->values = sequenceToList((yyvsp[-1].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 742: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 743: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 744: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 745: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 746: /* make_dim_decl: "array" '(' expr_list ')'  */
                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 747: /* $@77: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 748: /* $@78: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 749: /* make_dim_decl: "array" '<' $@77 type_declaration_no_options '>' $@78 '(' expr_list ')'  */
                                                                                                                                                               {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 750: /* make_dim_decl: "fixed_array" '(' expr_list ')'  */
                                                          {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 751: /* $@79: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 752: /* $@80: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 753: /* make_dim_decl: "fixed_array" '<' $@79 type_declaration_no_options '>' $@80 '(' expr_list ')'  */
                                                                                                                                                                     {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-8])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 754: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 755: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 756: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 757: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 758: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 759: /* make_table_decl: "table" '(' expr_map_tuple_list ')'  */
                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 760: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list ')'  */
                                                                                                        {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-6])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 761: /* make_table_decl: "table" '<' type_declaration_no_options ';' type_declaration_no_options '>' '(' expr_map_tuple_list ')'  */
                                                                                                                                                 {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::tTuple);
        mka->makeType->argTypes.push_back((yyvsp[-6].pTypeDecl));
        mka->makeType->argTypes.push_back((yyvsp[-4].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 762: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 763: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 764: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 765: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


