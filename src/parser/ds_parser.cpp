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
  YYSYMBOL_tuple_alias_type_list = 349,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 350,             /* variant_type  */
  YYSYMBOL_variant_type_list = 351,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 352,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 353,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 354,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 355,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 356,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 357, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 358, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 359, /* global_variable_declaration_list  */
  YYSYMBOL_360_33 = 360,                   /* $@33  */
  YYSYMBOL_optional_shared = 361,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 362, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 363,               /* global_let  */
  YYSYMBOL_364_34 = 364,                   /* $@34  */
  YYSYMBOL_enum_list = 365,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 366, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 367,             /* single_alias  */
  YYSYMBOL_368_35 = 368,                   /* $@35  */
  YYSYMBOL_alias_list = 369,               /* alias_list  */
  YYSYMBOL_alias_declaration = 370,        /* alias_declaration  */
  YYSYMBOL_371_36 = 371,                   /* $@36  */
  YYSYMBOL_optional_public_or_private_enum = 372, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 373,                /* enum_name  */
  YYSYMBOL_enum_declaration = 374,         /* enum_declaration  */
  YYSYMBOL_375_37 = 375,                   /* $@37  */
  YYSYMBOL_376_38 = 376,                   /* $@38  */
  YYSYMBOL_377_39 = 377,                   /* $@39  */
  YYSYMBOL_378_40 = 378,                   /* $@40  */
  YYSYMBOL_optional_structure_parent = 379, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 380,          /* optional_sealed  */
  YYSYMBOL_structure_name = 381,           /* structure_name  */
  YYSYMBOL_class_or_struct = 382,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 383, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 384,    /* structure_declaration  */
  YYSYMBOL_385_41 = 385,                   /* $@41  */
  YYSYMBOL_386_42 = 386,                   /* $@42  */
  YYSYMBOL_variable_name_with_pos_list = 387, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 388,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 389, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 390, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 391,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 392,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 393,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 394, /* bitfield_type_declaration  */
  YYSYMBOL_395_43 = 395,                   /* $@43  */
  YYSYMBOL_396_44 = 396,                   /* $@44  */
  YYSYMBOL_table_type_pair = 397,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 398, /* type_declaration_no_options  */
  YYSYMBOL_399_45 = 399,                   /* $@45  */
  YYSYMBOL_400_46 = 400,                   /* $@46  */
  YYSYMBOL_401_47 = 401,                   /* $@47  */
  YYSYMBOL_402_48 = 402,                   /* $@48  */
  YYSYMBOL_403_49 = 403,                   /* $@49  */
  YYSYMBOL_404_50 = 404,                   /* $@50  */
  YYSYMBOL_405_51 = 405,                   /* $@51  */
  YYSYMBOL_406_52 = 406,                   /* $@52  */
  YYSYMBOL_407_53 = 407,                   /* $@53  */
  YYSYMBOL_408_54 = 408,                   /* $@54  */
  YYSYMBOL_409_55 = 409,                   /* $@55  */
  YYSYMBOL_410_56 = 410,                   /* $@56  */
  YYSYMBOL_411_57 = 411,                   /* $@57  */
  YYSYMBOL_412_58 = 412,                   /* $@58  */
  YYSYMBOL_413_59 = 413,                   /* $@59  */
  YYSYMBOL_414_60 = 414,                   /* $@60  */
  YYSYMBOL_415_61 = 415,                   /* $@61  */
  YYSYMBOL_416_62 = 416,                   /* $@62  */
  YYSYMBOL_417_63 = 417,                   /* $@63  */
  YYSYMBOL_418_64 = 418,                   /* $@64  */
  YYSYMBOL_419_65 = 419,                   /* $@65  */
  YYSYMBOL_420_66 = 420,                   /* $@66  */
  YYSYMBOL_421_67 = 421,                   /* $@67  */
  YYSYMBOL_422_68 = 422,                   /* $@68  */
  YYSYMBOL_type_declaration = 423,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 424,  /* tuple_alias_declaration  */
  YYSYMBOL_425_69 = 425,                   /* $@69  */
  YYSYMBOL_426_70 = 426,                   /* $@70  */
  YYSYMBOL_427_71 = 427,                   /* $@71  */
  YYSYMBOL_428_72 = 428,                   /* $@72  */
  YYSYMBOL_variant_alias_declaration = 429, /* variant_alias_declaration  */
  YYSYMBOL_430_73 = 430,                   /* $@73  */
  YYSYMBOL_431_74 = 431,                   /* $@74  */
  YYSYMBOL_432_75 = 432,                   /* $@75  */
  YYSYMBOL_433_76 = 433,                   /* $@76  */
  YYSYMBOL_bitfield_alias_declaration = 434, /* bitfield_alias_declaration  */
  YYSYMBOL_435_77 = 435,                   /* $@77  */
  YYSYMBOL_436_78 = 436,                   /* $@78  */
  YYSYMBOL_437_79 = 437,                   /* $@79  */
  YYSYMBOL_438_80 = 438,                   /* $@80  */
  YYSYMBOL_make_decl = 439,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 440,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 441,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 442,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 443, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 444, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 445, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 446, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 447, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_make_struct_decl = 448,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 449,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 450,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 451,          /* make_tuple_call  */
  YYSYMBOL_make_dim = 452,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 453,            /* make_dim_decl  */
  YYSYMBOL_454_81 = 454,                   /* $@81  */
  YYSYMBOL_455_82 = 455,                   /* $@82  */
  YYSYMBOL_456_83 = 456,                   /* $@83  */
  YYSYMBOL_457_84 = 457,                   /* $@84  */
  YYSYMBOL_make_table = 458,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 459,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 460,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 461, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 462       /* array_comprehension  */
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
#define YYLAST   12156

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  213
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  250
/* YYNRULES -- Number of rules.  */
#define YYNRULES  774
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1367

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
       0,   511,   511,   512,   517,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   533,   539,   540,
     541,   545,   546,   550,   568,   569,   570,   571,   575,   579,
     584,   593,   601,   617,   622,   630,   630,   669,   696,   700,
     701,   705,   708,   712,   718,   727,   730,   736,   737,   741,
     745,   746,   750,   753,   759,   765,   768,   774,   775,   779,
     780,   781,   790,   791,   795,   796,   796,   802,   803,   804,
     805,   806,   810,   816,   816,   822,   828,   836,   846,   855,
     855,   862,   863,   864,   865,   866,   867,   871,   876,   884,
     885,   886,   890,   891,   892,   893,   894,   895,   896,   897,
     903,   906,   912,   913,   914,   918,   926,   939,   942,   950,
     961,   972,   983,   986,   993,   997,  1004,  1005,  1009,  1010,
    1011,  1015,  1018,  1025,  1029,  1030,  1031,  1032,  1033,  1034,
    1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,  1044,
    1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,  1054,
    1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,
    1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,
    1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,
    1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,
    1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,  1117,  1139,
    1140,  1141,  1145,  1151,  1151,  1168,  1172,  1183,  1184,  1185,
    1186,  1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,  1195,
    1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,  1207,  1212,
    1218,  1224,  1235,  1235,  1235,  1246,  1280,  1283,  1286,  1292,
    1293,  1304,  1308,  1311,  1319,  1319,  1319,  1322,  1328,  1331,
    1334,  1338,  1344,  1348,  1352,  1355,  1358,  1366,  1369,  1372,
    1380,  1383,  1391,  1394,  1397,  1405,  1411,  1412,  1416,  1417,
    1421,  1426,  1434,  1440,  1452,  1455,  1461,  1461,  1461,  1464,
    1464,  1464,  1469,  1469,  1469,  1477,  1477,  1477,  1483,  1493,
    1504,  1519,  1522,  1528,  1529,  1536,  1547,  1548,  1549,  1553,
    1554,  1555,  1556,  1560,  1565,  1573,  1574,  1578,  1583,  1590,
    1591,  1592,  1593,  1594,  1595,  1596,  1600,  1601,  1602,  1603,
    1604,  1605,  1606,  1607,  1608,  1609,  1610,  1611,  1612,  1613,
    1614,  1615,  1616,  1617,  1618,  1622,  1623,  1624,  1625,  1626,
    1627,  1631,  1638,  1650,  1655,  1665,  1669,  1676,  1679,  1679,
    1679,  1684,  1684,  1684,  1697,  1701,  1705,  1705,  1705,  1712,
    1713,  1714,  1715,  1716,  1717,  1718,  1719,  1720,  1721,  1722,
    1723,  1724,  1725,  1726,  1727,  1728,  1729,  1730,  1731,  1732,
    1733,  1734,  1735,  1736,  1737,  1738,  1739,  1740,  1741,  1742,
    1743,  1744,  1745,  1746,  1747,  1753,  1754,  1755,  1756,  1757,
    1758,  1759,  1760,  1761,  1762,  1763,  1764,  1765,  1769,  1773,
    1776,  1779,  1780,  1781,  1782,  1785,  1788,  1789,  1792,  1792,
    1792,  1795,  1800,  1804,  1808,  1808,  1808,  1813,  1816,  1820,
    1820,  1820,  1825,  1828,  1829,  1830,  1831,  1832,  1833,  1834,
    1835,  1836,  1838,  1842,  1843,  1848,  1852,  1853,  1854,  1855,
    1856,  1857,  1858,  1862,  1866,  1870,  1874,  1878,  1882,  1886,
    1890,  1894,  1901,  1902,  1906,  1907,  1908,  1912,  1913,  1917,
    1918,  1919,  1923,  1924,  1928,  1939,  1942,  1942,  1961,  1960,
    1975,  1974,  1987,  1996,  2002,  2007,  2017,  2018,  2022,  2025,
    2034,  2035,  2039,  2042,  2045,  2061,  2070,  2071,  2075,  2078,
    2081,  2095,  2096,  2100,  2106,  2112,  2115,  2119,  2125,  2134,
    2135,  2136,  2140,  2141,  2145,  2152,  2157,  2166,  2172,  2183,
    2186,  2191,  2196,  2204,  2215,  2218,  2218,  2238,  2239,  2243,
    2244,  2245,  2249,  2252,  2252,  2271,  2274,  2277,  2292,  2311,
    2312,  2313,  2318,  2318,  2340,  2341,  2345,  2346,  2346,  2350,
    2351,  2352,  2356,  2366,  2371,  2366,  2383,  2388,  2383,  2403,
    2404,  2408,  2409,  2413,  2419,  2420,  2424,  2425,  2426,  2430,
    2435,  2430,  2449,  2456,  2461,  2470,  2476,  2487,  2488,  2489,
    2490,  2491,  2492,  2493,  2494,  2495,  2496,  2497,  2498,  2499,
    2500,  2501,  2502,  2503,  2504,  2505,  2506,  2507,  2508,  2509,
    2510,  2511,  2512,  2513,  2517,  2518,  2519,  2520,  2521,  2522,
    2526,  2537,  2541,  2548,  2560,  2567,  2576,  2581,  2584,  2597,
    2597,  2597,  2610,  2614,  2621,  2622,  2623,  2624,  2625,  2639,
    2645,  2649,  2653,  2658,  2663,  2668,  2673,  2677,  2681,  2686,
    2690,  2694,  2699,  2699,  2699,  2705,  2712,  2712,  2712,  2717,
    2717,  2717,  2723,  2723,  2723,  2728,  2732,  2732,  2732,  2737,
    2737,  2737,  2746,  2750,  2750,  2750,  2755,  2755,  2755,  2764,
    2768,  2768,  2768,  2773,  2773,  2773,  2782,  2782,  2782,  2788,
    2788,  2788,  2797,  2800,  2811,  2827,  2827,  2832,  2837,  2827,
    2862,  2862,  2867,  2873,  2862,  2898,  2898,  2903,  2908,  2898,
    2938,  2939,  2940,  2941,  2942,  2946,  2953,  2960,  2966,  2972,
    2979,  2986,  2992,  3002,  3007,  3014,  3015,  3020,  3021,  3025,
    3026,  3030,  3031,  3035,  3036,  3037,  3041,  3042,  3043,  3048,
    3054,  3061,  3069,  3076,  3084,  3096,  3099,  3105,  3119,  3125,
    3131,  3140,  3145,  3152,  3157,  3164,  3172,  3172,  3172,  3180,
    3187,  3187,  3187,  3197,  3202,  3209,  3212,  3218,  3227,  3235,
    3243,  3256,  3257,  3261,  3264
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
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
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
  "$@66", "$@67", "$@68", "type_declaration", "tuple_alias_declaration",
  "$@69", "$@70", "$@71", "$@72", "variant_alias_declaration", "$@73",
  "$@74", "$@75", "$@76", "bitfield_alias_declaration", "$@77", "$@78",
  "$@79", "$@80", "make_decl", "make_struct_fields", "make_struct_dim",
  "optional_block", "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "make_struct_decl", "make_tuple",
  "make_map_tuple", "make_tuple_call", "make_dim", "make_dim_decl", "$@81",
  "$@82", "$@83", "$@84", "make_table", "expr_map_tuple_list",
  "make_table_decl", "array_comprehension_where", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1203)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-684)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1203,    69, -1203, -1203,    41,   -10,    84,   158, -1203,   -87,
     472,   472,   472,    82,    22, -1203, -1203, -1203,   -51, -1203,
   -1203, -1203,   367, -1203,   129, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203,   203, -1203,    85,   286,   215,
   -1203, -1203, -1203,    84, -1203,    13, -1203,   472,   472, -1203,
   -1203,   129, -1203, -1203, -1203, -1203, -1203,   317,   352, -1203,
   -1203, -1203,    22,    22,    22,   321, -1203,   552,    54, -1203,
   -1203, -1203, -1203,   476,   502,   505, -1203,   509,    27,    41,
     386,   -10,   326,   415, -1203,   -59,   -59, -1203,   421,   379,
     -23,   409,   512,   430,   460,   468, -1203,   478,   438, -1203,
   -1203,   -55,    41,    22,    22,    22,    22, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203,   500, -1203, -1203, -1203, -1203, -1203,
     427, -1203, -1203, -1203, -1203, -1203,   337,    49, -1203, -1203,
   -1203, -1203,   586, -1203, -1203, -1203, -1203, -1203,   467, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,   511, -1203,
     140, -1203,   197,   531,   552, 11991, -1203,   -86,   575, -1203,
     525, -1203, -1203,   526, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203,   151, -1203,   517, -1203,   479,   493,   498, -1203, -1203,
   10565, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203,   672,   675, -1203,   506,
     503,   392, -1203, -1203,   545, -1203,   501,    41,   -50, -1203,
   -1203, -1203,    49, -1203, 11803, -1203, -1203, -1203,   550,   561,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
     562,   524, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203,   708, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203,   565,   528, -1203,
   -1203,   -84,   551, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203,   527, -1203,   556,   530, -1203,   525,    66,   537,   703,
     376, -1203, -1203,   559,   560,   563,   566, -1203, -1203, -1203,
     542, -1203, -1203, -1203, -1203, -1203,   568, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,   569, -1203,
   -1203, -1203,   572,   573, -1203, -1203, -1203, -1203,   574,   577,
     543, -1203, -1203, -1203, -1203, -1203,  7776,   567, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203,   596,   619, -1203,   570, -1203,
       9, -1203,    56, 11803, -1203,  1750, -1203,  -100,    82, -1203,
   -1203,   -50,   576,  7232,   603,   606, 11803, -1203,   -45, -1203,
   -1203, -1203, -1203,   607, -1203,   294,   320,   327, -1203, -1203,
    7232, -1203, -1203, -1203,    15, -1203, -1203, -1203,    14,  4075,
   -1203,   213,   -91, 10018,   -83, -1203, -1203,  7232,   307,   571,
     733, -1203,   -52, -1203,    75,   567, -1203,  7232, -1203, -1203,
     307, -1203, -1203,   318,   579,   591,    48,  2756, -1203, -1203,
     503,   234,  4260,   580,  7232,   616,   600,   601,   583, -1203,
     581,   605,   631,  4445,   293,   236, -1203,   588,   608,  2941,
    7232,  7232,   461,   461,   461,   598,   609,   611,   612,   614,
     618, -1203, 11477, 11576,  7232, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203,  4630, -1203,  7232,  7232,  7232,  7232,  7232,  7232,
   -1203,   593, -1203,   760, -1203, -1203,   602,   604,  7232,   812,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203,   -53,   615, -1203,   620,   622,   624, -1203,   625, -1203,
   -1203,   763, -1203, -1203, -1203, -1203,   632, -1203, -1203,   -73,
   -1203, -1203, -1203, -1203, -1203,  7340, -1203,   623, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203,   -30, -1203,   627, -1203,    20,
   -1203, -1203,   637,   650,  7733, -1203,   802,  1249, -1203, -1203,
   -1203,  3323, 11803, 11803, 11803, 11803,   638,   680, 11803,   506,
   11803,   506, 11803,   506, 11902,   681,  7829, -1203, -1203, -1203,
   -1203,   643, -1203, -1203, 10007, -1203,  7776,   642, -1203,   641,
     -57, -1203,   644,   567,   647,   677, -1203,   655,   654,  7925,
   -1203,  7232,   701, -1203,   705, -1203, 11803, -1203,  3323, -1203,
    1172,    18, -1203,   663, -1203, -1203,    82, -1203, -1203,  7232,
    3323, -1203,   692,   751, 10456,   872, 11803,  7232,  7232, 10923,
    7232,  1172,   709, -1203, -1203,  7232, -1203, -1203,   714,   732,
   10923,  7232, -1203, -1203,  7232,  7232, 11803,  3323, -1203, 10456,
     736,   736,   683, -1203, -1203, -1203,  7232,  7232,  7232,  7232,
    7232,  7232,   307,  2159,   307,  2358, 10600, -1203,   -25, -1203,
    1172,   492,   736,   736,     7,   736,   736,  7960,   235,   687,
   -1203, -1203,  1172, -1203, -1203,  3878, -1203, -1203, -1203, -1203,
   -1203, -1203,   383,   744,   461, -1203, 10333, 11609,  7232,  7232,
    7232,  7232,  7232,  7232,  7232,  7232,  7232,  7232, -1203, -1203,
    7232,  7232,  7232,  7232,  7232,  7232,   735,  3508,  7232,   290,
    7232,  7232,  7232,  7232,  7232,  7232,  7232,  7232,  7232,  7232,
    7232,  7232,  7232,  7232,  7232,  3693,  4815,  7232,  7232,  7232,
    7232,  7232,  7232,  7232,  7232,  7232,  7232,   114,  7232,  5000,
    -100,  7232, -1203, -1203,    22, -1203,   894,   525, -1203,   737,
   -1203,  3323, -1203,  7447,   270,   716,   135,   382,   385, -1203,
   -1203,    37,   398,   551,   417,   551,   418,   551, -1203,   237,
   -1203,   250, -1203, -1203, -1203, -1203, -1203, 11803, -1203, -1203,
   11803, -1203, -1203, -1203,  8056, -1203,   863,   -46, -1203, 10456,
   -1203,  7232, 11803,  5185,  5370, 11803, 11803,   170, 10923, -1203,
     692,  7232,  7232,  7232,   503,   707, -1203,   173, 10923,  8152,
   -1203, -1203, 11803, 11803,  8187, 11803, -1203, -1203,  8283, 11803,
     187,   191,  2080, -1203, 10456, -1203,  8379,  8414,  8510,  8606,
    8641,  8737,    24,   461,   700,   -19,  2557,  5555, 10696,   727,
      -7,   -88,   730,   149,    26,  5740,    -7,    60,  7232, -1203,
    7232,   702, -1203, 11803, -1203, -1203,  7232,   444, -1203,   706,
     710,   347, -1203, -1203, -1203,   304,  7232,   331, -1203,   198,
     747, -1203, -1203,   -21,   506, -1203,   728,   715, -1203, -1203,
     738,   717, -1203, -1203, 10923, 10923, 10923, 10923, 10923, 10923,
   10923, 10923,   684,   684,  1761, 10923, 10923,  1761, 11233, 11233,
     718,   461,   461,   461, 10923,    52,   720, -1203, 10122,   -63,
     -63,   623, 10923,   684,   684, 10923, 10923,  1207, 11054, 11150,
   10923, 10923, 10923, 11019,   461,   461,   461, 10923, 11704, 10792,
   11192,  1420,   496,  1761,  1761,   387,   387,    52,    52,    52,
     370,  7232,   721, -1203,   371,  7232,   920, 10157, -1203,   206,
     719,  1384,   207,   521,   862,   722, -1203, -1203,  7554, -1203,
   -1203, -1203, 11803, -1203, -1203, -1203,   764, -1203,   741, -1203,
     745, -1203,   746, -1203, 11902, -1203,   681,   567,   567, -1203,
     770,  7232,   -38,   441, -1203,   212,   771,  8833,   448,  2938,
   -1203, 10923, -1203,   739, 11803,  7232, -1203, -1203,  7232,  3875,
    5922, -1203,  7229, -1203,  7306, -1203, -1203,   744, -1203, -1203,
   -1203, -1203, -1203,   740,  7232, -1203,  7232,  7232,  7232,    16,
    7232,   397,   331,   -88, -1203, -1203,   742, -1203,  7232, -1203,
     743,  7232, -1203,  7232,   331,   145, -1203,   748, -1203, 10923,
   -1203, -1203, 10426, 10827,  7232,  7232, 11803,   506,  1961,   503,
   -1203, -1203,  7232, -1203,   887,   -18,   210,  5925, -1203, -1203,
     238,   772,   775,   776,   777, -1203,   244,   551, -1203,  7232,
   -1203,  7232,  6122, -1203, -1203, -1203,  7232, -1203, -1203, -1203,
   -1203,   757,   749, -1203, -1203,  7232,   750, -1203, 10253,  7232,
     752, -1203, 10349, -1203, -1203, -1203, -1203, -1203,   753, -1203,
   -1203,   450, -1203,    55, -1203, -1203, -1203, -1203,   567, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203, 10923,   503, -1203, -1203,   253, -1203,
   -1203, -1203,  7232, 11269, -1203, 10923, -1203, -1203, -1203, -1203,
     754,  6307,    42,  8868, 10923, 10923,    -7,   -88, 10923,   755,
      97,   727, -1203, -1203, 10923, -1203,   730,    65,    -7, -1203,
   -1203,   759, -1203, -1203, -1203, -1203,  8964,  9060, 11311,   551,
   -1203, -1203,  1172, -1203,   761,   762,   331,   783,   765, -1203,
   -1203, -1203, -1203,   -21,   767,   -81, 11803,  9095, 11803,  9191,
   -1203,   220,  9287, -1203,  7232,   533,  7232, -1203,  9322,  7232,
   -1203, -1203, -1203,   918,    55, -1203, -1203,   521, -1203, -1203,
   -1203, -1203, -1203,  7232,   786, -1203,   768,   226,   769,  7232,
    7232,  7232,   773,  6492, -1203,   227,  7232,   134,   -88, -1203,
    7232,  7232,  7232,  7232,   145, -1203,  7232, -1203, -1203, -1203,
     766,   304,  3138, -1203, -1203,   256, -1203,   457, -1203, -1203,
    6677, -1203, -1203, 11328, -1203,   451, -1203, -1203, -1203, 11803,
    9418,  9514, -1203,  9549, -1203, -1203,   918,   307,  9645,   779,
    7232, -1203,  7232,  1761,  1761,  1761,  7232, -1203,  9741, -1203,
    1539,  7232,  7232, -1203,  9776, 10923, 10923,  1539, -1203,  1761,
     175, -1203, -1203,  6862,  7047, -1203, -1203, -1203, -1203, -1203,
   10923,   774, 11803,   -45,  7232, 11019, -1203, -1203,   453, -1203,
   -1203, -1203, 11991, -1203, -1203, -1203,  7232,   230,   252,   255,
   -1203,   907,   778, 10923, 10923,   141,   780, -1203,   175,  7232,
    7232, 10923, -1203,  2556,  7232,   533, -1203, -1203, -1203,   781,
   11991,  9872, -1203, -1203, -1203,  7232,   782,  7232,  7232,   784,
   -1203, 10923,  7232,  7591, -1203, -1203,   503, -1203, 10923, -1203,
   10923, 10923, -1203,  7698, -1203, -1203, -1203
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   116,     1,   276,     0,     0,     0,   557,   277,     0,
     549,   549,   549,     0,     0,    16,    15,     3,     0,    10,
       9,     8,     0,     7,   537,     6,    11,     5,     4,    13,
      12,    14,    90,    91,    89,    98,   100,    37,    52,    49,
      50,    39,    40,     0,    41,    47,    38,   549,   549,    22,
      21,   537,   551,   550,   705,   695,   700,     0,   249,    35,
     103,   104,     0,     0,     0,   105,   107,   114,     0,   102,
      17,   575,   574,   209,   559,   576,   538,   539,     0,     0,
       0,     0,    42,     0,    48,     0,     0,    45,     0,     0,
     549,     0,    18,     0,     0,     0,   251,     0,     0,   113,
     108,     0,     0,     0,     0,     0,     0,   117,   211,   210,
     213,   208,   561,   560,     0,   578,   577,   579,   541,   540,
     543,    96,    97,    94,    95,    93,     0,     0,    92,   101,
      53,    51,    47,    44,    43,   552,   554,   556,     0,   558,
      19,    20,    23,   706,   696,   701,   250,    33,    36,   112,
       0,   109,   110,   111,   115,     0,   562,     0,   571,   534,
     472,    24,    25,     0,    85,    86,    83,    84,    82,    81,
      87,     0,    46,     0,   555,     0,     0,     0,    34,   106,
       0,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,     0,     0,   123,   118,
       0,     0,   563,   572,     0,   580,   535,     0,     0,    26,
      27,    28,     0,    99,     0,   707,   697,   702,   177,   178,
     175,   126,   127,   129,   128,   130,   131,   132,   133,   159,
     160,   157,   158,   150,   161,   162,   151,   148,   149,   176,
     170,     0,   174,   163,   164,   165,   166,   137,   138,   139,
     134,   135,   136,   147,     0,   153,   154,   152,   145,   146,
     141,   140,   142,   143,   144,   125,   124,   169,     0,   155,
     156,   472,   121,   238,   214,   614,   617,   615,   618,   616,
     619,     0,   545,   569,     0,   542,   472,     0,     0,   524,
     522,   544,    88,     0,     0,     0,     0,   587,   607,   588,
     621,   589,   593,   594,   595,   596,   613,   600,   601,   602,
     603,   604,   605,   606,   608,   609,   610,   611,   665,   592,
     599,   612,   672,   679,   590,   597,   591,   598,     0,     0,
       0,   620,   634,   637,   635,   636,   692,   553,   626,   502,
     508,   179,   180,   173,   168,   181,   171,   167,     0,   119,
       0,   496,     0,     0,   212,     0,   566,   564,     0,   573,
     485,     0,     0,     0,     0,     0,     0,   523,     0,   656,
     659,   662,   652,     0,   629,   666,   673,   680,   686,   689,
       0,   642,   647,   641,     0,   655,   651,   644,     0,     0,
     646,     0,   708,   698,   703,   182,   172,     0,     0,     0,
     582,   493,   513,   120,   472,   122,   240,     0,    62,    63,
       0,   375,   376,     0,     0,     0,     0,   264,   369,   262,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,   613,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   461,     0,     0,     0,   319,   321,   320,   322,   323,
     324,   325,     0,    29,     0,     0,     0,     0,     0,     0,
     306,   307,   217,   215,   373,   372,     0,     0,     0,     0,
     233,   228,   225,   224,   226,   227,   317,   239,   219,   455,
     218,   370,     0,   446,    70,    71,    68,   231,    69,   232,
     234,   279,   223,   445,   444,   443,   116,   449,   371,     0,
     220,   448,   447,   416,   377,   326,   378,     0,   374,   710,
     714,   711,   712,   713,   545,     0,   546,     0,   570,   486,
     536,   473,     0,     0,     0,   526,   527,     0,   520,   521,
     519,     0,     0,     0,     0,     0,     0,     0,     0,   118,
       0,   118,     0,   118,     0,     0,     0,   649,   650,   643,
     645,     0,   648,   639,     0,   694,   693,     0,   627,     0,
     249,   503,     0,   498,     0,     0,   509,     0,     0,     0,
     494,     0,     0,   512,     0,   511,     0,   514,     0,   497,
       0,     0,   254,   258,   257,   261,     0,   295,   756,     0,
       0,   268,   265,     0,   301,     0,     0,     0,     0,   252,
       0,     0,     0,   286,   289,     0,   237,   292,     0,     0,
      56,     0,    76,   760,     0,     0,     0,     0,   273,   270,
     405,   406,   307,   248,   246,   247,     0,     0,     0,     0,
       0,     0,     0,   725,     0,     0,   749,   763,     0,   242,
       0,     0,   382,   381,   421,   380,   379,     0,   308,     0,
     235,   236,     0,    73,   453,     0,   222,   229,   230,   267,
     272,   278,     0,   315,     0,   221,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   407,   408,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,     0,
     567,     0,   547,   565,     0,   581,     0,   472,   525,     0,
     529,     0,   533,   326,     0,     0,   632,     0,     0,   622,
     624,     0,     0,   121,     0,   121,     0,   121,   500,     0,
     506,     0,   623,   640,   638,   628,   709,     0,   504,   699,
       0,   510,   704,   495,     0,   584,   585,   515,   518,   517,
      77,     0,     0,     0,     0,     0,     0,     0,   301,   269,
     266,     0,     0,     0,     0,     0,   765,     0,   253,     0,
      78,    79,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   274,   271,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   249,     0,     0,   745,   723,
     725,     0,   751,     0,     0,     0,   725,     0,     0,   728,
       0,     0,   767,     0,   241,    32,     0,    30,   409,     0,
       0,   358,   355,   357,   238,    59,     0,     0,   417,     0,
       0,   285,   284,     0,   118,   245,     0,     0,   432,   431,
       0,     0,   433,   437,   336,   337,   339,   338,   340,   330,
     331,   332,   383,   384,   396,   341,   342,   397,   394,   395,
       0,     0,     0,     0,   328,   426,     0,   414,     0,   450,
     451,   452,   329,   385,   386,   343,   344,   401,   402,   403,
     333,   334,   335,   404,     0,     0,     0,   327,     0,     0,
     399,   400,   398,   392,   393,   388,   387,   389,   390,   391,
       0,     0,     0,   364,     0,     0,     0,     0,   419,     0,
       0,     0,     0,   479,   482,     0,   528,   531,   326,   532,
     657,   660,     0,   663,   653,   630,     0,   667,     0,   674,
       0,   681,     0,   687,     0,   690,     0,   499,   505,   583,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
     755,   302,   275,     0,     0,     0,   768,   422,     0,     0,
       0,   423,     0,   454,     0,   759,   750,   315,   456,   457,
     458,   459,   460,     0,     0,   726,     0,     0,     0,   725,
       0,     0,     0,     0,   734,   735,     0,   740,     0,   732,
       0,     0,   753,     0,     0,     0,   730,     0,   754,   748,
     764,   727,     0,     0,     0,     0,     0,   118,     0,     0,
      57,    58,     0,    72,    64,     0,     0,     0,   418,   280,
       0,     0,     0,     0,     0,   313,     0,   121,   428,     0,
     434,     0,     0,   350,   348,   349,     0,   412,   347,   345,
     346,     0,     0,   438,   442,     0,     0,   415,     0,     0,
       0,   365,     0,   367,   410,   420,   568,   548,   117,   480,
     481,   482,   483,   474,   487,   530,   658,   661,   633,   664,
     654,   631,   625,   668,   670,   675,   677,   682,   684,   688,
     501,   691,   507,   586,   516,     0,   255,   260,     0,   298,
     296,   757,     0,     0,   766,    80,   287,   290,   293,   761,
       0,     0,     0,     0,   716,   715,   725,     0,   746,     0,
       0,   724,   739,   733,   747,   731,   752,     0,   725,   737,
     738,     0,   743,   729,   243,    31,     0,     0,     0,   121,
     216,    60,     0,    65,     0,     0,     0,     0,     0,   311,
     312,   310,   309,     0,     0,     0,     0,     0,     0,     0,
     353,     0,     0,   439,     0,   427,     0,   413,     0,     0,
     411,   368,   492,   477,   474,   475,   476,   479,   671,   678,
     685,    75,   256,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,   462,     0,     0,     0,     0,   741,
       0,     0,     0,     0,     0,   736,     0,   356,   471,   359,
       0,    59,     0,    74,   351,     0,   281,   522,   314,   316,
       0,   303,   318,     0,   470,     0,   468,   354,   465,     0,
       0,     0,   464,     0,   478,   488,   477,     0,     0,     0,
       0,   769,     0,   288,   291,   294,     0,   424,     0,   463,
     771,     0,     0,   742,     0,   718,   717,   771,   744,   244,
       0,   362,    61,   264,     0,    66,    70,    71,    68,    69,
      67,     0,     0,     0,     0,   304,   429,   435,     0,   469,
     467,   466,     0,   490,   484,   299,     0,     0,     0,     0,
     425,     0,     0,   720,   719,     0,     0,   360,     0,     0,
       0,   270,   352,     0,     0,   305,   430,   436,   440,     0,
       0,     0,   758,   770,   762,     0,     0,     0,     0,     0,
     363,   271,     0,     0,   441,   489,     0,   300,   772,   773,
     722,   721,   774,     0,   283,   491,   282
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1203, -1203, -1203, -1203, -1203, -1203,   311,   897, -1203, -1203,
   -1203,   975, -1203, -1203, -1203,   564,   936, -1203,   851, -1203,
   -1203,   903, -1203, -1203, -1203,  -256, -1203, -1203, -1203,  -255,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,   787, -1203,
   -1203,   910,   -54, -1203, -1203,   416,   248,  -344,  -550,  -739,
   -1203, -1203, -1203, -1202, -1203, -1203,  -208, -1203,   130, -1203,
   -1203, -1203, -1203,  -397,   -13, -1203, -1203, -1203, -1203, -1203,
    -249,  -245,  -238, -1203,  -228, -1203, -1203,  1015, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203,  -595, -1203, -1203,  -166, -1203,    12,  -432,
   -1203,  -358, -1203, -1203, -1203, -1185, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203,   405, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203,  -150,  -180,  -237,  -176,   -67, -1203, -1203, -1203,
   -1203, -1203,   621, -1203,  -545, -1203, -1203,  -549, -1203, -1203,
    -407,  -404,  -541,  -210, -1203,  -353, -1203, -1203,   988, -1203,
   -1203, -1203,   507,   690,    31, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203,  -395,  -221, -1203,   617, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203,  -368, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
    -193, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203, -1203,
   -1203, -1203, -1203, -1203, -1203, -1203,   626,  -831,  -638,  -811,
   -1203, -1203, -1203, -1000,  -191, -1203,    17,  -452, -1203,   389,
   -1203, -1203, -1203, -1203, -1203, -1203, -1079, -1203,  -242, -1203
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   142,    51,    17,   163,   169,   661,   484,
     148,   485,    98,    19,    20,    44,    45,    46,    87,    21,
      39,    40,   486,   487,  1052,  1053,   488,  1174,  1242,   489,
     490,   866,   491,   492,   493,   494,   495,   998,   170,   171,
      35,    36,    37,    65,    66,    67,    68,    22,   282,   364,
     209,    23,   110,   210,   111,   155,   496,   497,   365,   498,
     499,   853,  1236,   500,   501,   502,   603,   792,  1212,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   682,  1060,
     871,   512,   513,   812,  1219,   813,  1220,   815,  1221,   514,
     795,  1215,   515,   612,  1252,   516,  1065,  1066,   874,   517,
     518,   613,   520,   521,   522,   863,   523,  1046,  1290,  1047,
    1328,   524,   946,  1201,   798,  1186,  1336,  1188,  1337,  1259,
    1354,   526,   360,  1207,  1265,  1101,  1103,   955,   539,   747,
    1312,  1340,   361,   362,   582,   769,   403,   587,   771,   404,
    1018,   411,   551,   378,   300,   301,   216,   296,    77,   120,
      25,   160,   367,    88,    89,   173,    90,    26,    48,   114,
     157,    27,   292,   537,   534,   950,   369,   214,   215,    75,
     117,    28,   158,   294,   412,   527,   291,   343,   344,   761,
     402,   345,   557,  1111,   755,   346,   555,  1110,   552,  1106,
     553,  1107,   554,  1109,   558,  1113,   559,  1208,   560,  1115,
     561,  1209,   562,  1117,   563,  1210,   564,  1119,   565,  1121,
     583,    29,    94,   176,   349,   584,    30,    95,   177,   350,
     588,    31,    93,   175,   348,   579,   528,   839,   840,   841,
     852,  1038,  1032,  1027,  1162,   529,   842,   806,   530,   843,
     531,   796,  1216,   819,  1222,   658,   807,   532,  1322,   533
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      59,    69,   284,   342,   590,   598,   751,   519,   547,   763,
     218,   765,   657,   767,   797,   800,   770,   846,   540,   768,
     643,   644,   645,  1152,   968,   601,   970,  -116,   972,  1023,
     611,   347,    52,   576,    83,  1035,  1056,   791,    53,   820,
     821,   121,   122,  1014,   833,  1033,   569,   567,   150,    69,
      69,    69,   638,  1217,   684,    60,  1250,   408,   706,    32,
      33,   709,   710,   164,   165,   535,   358,   833,    84,     2,
     593,   217,   686,   687,   577,     3,   593,   548,    49,    91,
     869,    97,   585,    61,   103,   104,   105,   549,  1024,  1025,
      69,    69,    69,    69,   653,   655,   211,   298,     4,  1205,
       5,  1061,     6,   593,   593,  1327,    41,    42,     7,   536,
    1339,  1062,   674,  1017,  1017,   299,  1026,     8,   578,    97,
      50,   138,   359,     9,   212,   777,   586,   594,   595,   283,
     596,   709,   710,   597,   595,   550,   685,   401,  1356,   737,
     738,   801,   342,  1350,   949,    10,   371,  1229,    38,   849,
     741,   149,   675,    62,   752,   342,   409,  1206,    70,  1063,
      57,   595,   595,   297,  1064,   698,   699,    11,    12,   834,
     415,  1125,   683,   706,   410,   708,   709,   710,   711,   742,
     342,  1055,   342,   712,   850,   123,   851,    58,   137,    76,
     124,  1151,   125,  1318,   126,   746,   982,   594,   985,   570,
     568,   788,  1022,   594,    85,   594,    34,   166,  1147,   737,
     738,   341,   167,   799,   168,    86,   126,   571,    63,   593,
      57,   801,   615,   744,   965,   358,   572,    64,  1283,  1231,
     217,   745,   127,   106,   608,  1036,   632,   303,   940,   941,
     823,   342,   342,   304,   801,    79,   966,    58,   805,    41,
      42,  1226,   875,   609,   737,   738,   593,   832,   107,   844,
     305,    13,   413,   593,    79,   414,  1281,   942,   822,  1031,
     372,  1037,    14,  1347,  1233,   306,    43,   595,    15,   943,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,    57,   595,  1049,   944,   945,   401,    79,
    1159,   595,   859,  1160,  1067,  1029,  1050,  1051,  1185,   872,
     222,   342,   342,   342,   342,  1228,   103,   342,   105,   342,
      58,   342,   426,   342,   962,  1245,   179,  1234,   431,   801,
     341,    57,   995,  1030,   957,   538,  1161,   223,  1031,   754,
     756,   757,   758,   341,   340,   762,   801,   764,    47,   766,
     801,    71,    72,    57,    73,   342,   990,  1057,    58,   996,
     981,  1146,   859,    78,   445,   801,   106,   860,   341,  1021,
     341,   801,   790,  1005,    81,   342,  1158,  1006,  1040,   801,
      58,  1015,    74,   787,  1058,   995,   801,   686,   687,   801,
     341,  1098,  1095,   810,  1175,   342,   447,  1177,  1127,  1142,
     616,   861,   633,  1183,   973,   575,  1257,  1122,   989,  1120,
    1240,   995,  1271,  1279,   801,  1021,  1342,   975,  1157,   617,
    1213,   634,  1178,   906,   999,  1000,   974,  1002,  1184,   341,
     341,  1004,   854,   401,   409,   907,    57,   960,  1343,   976,
    1301,  1344,  1214,   285,   865,   879,   883,   286,    80,  1073,
    1074,  1075,   410,   462,   463,   464,  -669,  1191,    99,   100,
     101,  -669,    96,    58,   834,  1042,   287,   288,   289,   290,
      97,   911,  1078,  1079,  1080,  1203,  1055,  1169,   631,  -669,
     698,   699,  -676,   283,   602,   161,   162,  -676,   706,  -683,
     708,   709,   710,   711,  -683,  1102,   686,   687,   712,   151,
     152,   153,   154,  1086,  1090,  -676,   102,    52,    86,  -361,
     298,   108,  -683,    53,  -361,  1087,  1091,   109,   870,   341,
     341,   341,   341,  1134,   130,   341,  1225,   341,   299,   341,
    1149,   341,  -361,   686,   687,   375,   342,   112,   376,   342,
     115,   377,  1150,   113,   118,   401,   116,   140,   401,   963,
     119,   342,   964,   141,   342,   342,  1099,   734,   735,   736,
     132,   401,  1100,   341,   977,   967,   135,   978,   136,   737,
     738,   342,   342,   794,   342,   143,   992,   954,   342,   983,
     401,   401,   988,   341,   969,   971,   147,   696,   697,   698,
     699,   700,   219,   220,   703,   704,   705,   706,   139,   708,
     709,   710,   711,   341,   401,   144,  1133,   712,  1126,   714,
     715,   401,   342,   145,   401,  1130,   401,   159,  1307,  1302,
    1338,    84,   377,   146,   696,   697,   698,   699,   700,   133,
     134,   703,   704,   705,   706,   862,   708,   709,   710,   711,
     161,   162,   855,   856,   712,   156,   714,   715,   480,   642,
     103,   283,   718,   719,   720,  1317,   174,   213,  1168,   178,
     217,  1319,   730,   731,   732,   733,   734,   735,   736,   225,
     519,   103,   104,   105,   219,   220,   221,   224,   737,   738,
      54,    55,    56,   226,   686,   687,   279,  1084,   227,   280,
     293,   281,   295,   283,   726,   351,   727,   728,   729,   730,
     731,   732,   733,   734,   735,   736,   352,   353,   354,   355,
     356,    69,   357,   363,   800,   737,   738,   366,   368,   391,
     370,   342,   373,  1232,   374,   379,   380,   383,   390,   381,
     401,   406,   382,   342,   384,   385,   686,   687,   386,   387,
     388,   405,  1334,   389,   341,   392,   393,   341,   545,  1108,
     525,   546,   556,   342,   592,   407,   591,   607,   544,   341,
     541,   622,   341,   341,   606,   620,   623,   624,   625,   628,
     626,   627,  1352,   635,   636,   566,   669,   698,   699,   341,
     341,   668,   341,   646,   574,   706,   341,   708,   709,   710,
     711,   670,   589,   671,   647,   712,   648,   649,  1253,   650,
    1282,   673,   600,   651,   676,   342,   394,   681,   739,   677,
     395,   678,   614,   679,   680,    14,   659,   619,   743,   621,
     341,  1171,   631,   749,   759,   760,   585,   773,   630,   698,
     699,   775,   776,   778,   639,   640,   641,   706,   779,   780,
     709,   710,   711,  1314,   781,   782,   785,   712,   793,   656,
     786,   801,   732,   733,   734,   735,   736,   660,   684,   662,
     663,   664,   665,   666,   667,   804,   737,   738,   396,   811,
     817,   825,   397,   672,   993,   398,   816,   864,  1173,   873,
     900,   953,   956,   961,   980,  1016,  1021,   686,   687,  1028,
     399,  1044,  1059,  1041,  1068,  1045,   994,  1211,  1348,   400,
    1069,  1093,  1071,  1072,  1070,  1076,  1089,  1102,  1114,  1112,
    1096,  1104,  1116,  1118,  1333,  1123,  1128,  1179,   737,   738,
    1180,  1181,  1182,  1193,  1132,  1141,  1153,  1155,  1246,   341,
    1264,  1269,  1163,  1291,  1194,  1196,   753,  1199,  1345,  1223,
    1230,   341,  1202,  1235,  1241,   342,  1316,   342,  1244,  1247,
    1243,  1249,   857,  1270,  1272,   128,    18,  1251,  1276,    82,
    1332,   341,  1346,   172,   131,  1292,  1359,  1295,  1362,   129,
    1355,  1349,   952,  1296,  1048,  1255,   784,  1297,   696,   697,
     698,   699,   700,   789,  1298,   703,   704,   705,   706,   302,
     708,   709,   710,   711,  1299,   614,    24,  1248,   712,  1140,
     714,   715,   656,   808,  1266,   809,   718,   719,   720,  1313,
     814,  1267,   724,   341,  1204,   599,   818,  1303,   342,    92,
     604,   740,   824,  1288,   847,  1326,     0,     0,  1156,   605,
       0,   826,   827,   828,   829,   830,   831,     0,   838,     0,
     838,     0,     0,     0,     0,     0,  1308,     0,   726,     0,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
       0,   342,     0,     0,     0,     0,     0,     0,     0,   737,
     738,     0,     0,   884,   885,   886,   887,   888,   889,   890,
     891,   892,   893,     0,     0,   894,   895,   896,   897,   898,
     899,     0,   904,   905,     0,   908,   909,   910,   912,   913,
     914,   915,   916,   917,   918,   919,   920,   921,   922,   923,
     927,   929,   930,   931,   932,   933,   934,   935,   936,   937,
     938,   939,     0,   947,     0,     0,   951,     0,  1365,     0,
       0,     0,     0,     0,     0,     0,   958,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,     0,   341,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   686,   687,     0,     0,     0,     0,     0,   987,
       0,     0,     0,     0,     0,     0,   991,   904,   927,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   686,   687,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   662,   667,     0,     0,     0,   341,     0,     0,     0,
     667,     0,     0,  1039,     0,   656,     0,     0,     0,     0,
       0,  1043,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1054,     0,     0,     0,     0,     0,   862,     0,     0,
       0,   391,     0,   696,   697,   698,   699,   700,     0,   341,
     703,   704,   705,   706,     0,   708,   709,   710,   711,     0,
       0,     0,     0,   712,     0,   714,   715,   392,   393,     0,
       0,   718,   719,   720,     0,   862,     0,   724,   696,   697,
     698,   699,   700,     0,     0,   703,   704,   705,   706,     0,
     708,   709,   710,   711,     0,     0,     0,     0,   712,     0,
     714,   715,     0,     0,     0,     0,  1088,     0,     0,     0,
    1092,     0,     0,   726,     0,   727,   728,   729,   730,   731,
     732,   733,   734,   735,   736,     0,     0,     0,   394,     0,
       0,   548,   395,     0,   737,   738,     0,     0,     0,     0,
       0,   549,   283,     0,     0,     0,  1124,     0,     0,     0,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
     656,     0,     0,  1135,   686,   687,     0,     0,     0,   737,
     738,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1143,  1144,  1145,     0,  1148,     0,     0,     0,   550,
     396,     0,     0,  1154,   397,     0,   838,   398,     0,     0,
     686,   687,     0,     0,     0,     0,     0,     0,     0,  1166,
    1167,     0,   399,   525,     0,     0,     0,  1172,   750,     0,
       0,   400,   991,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1187,     0,  1189,     0,     0,     0,
       0,  1192,     0,     0,     0,     0,     0,     0,     0,     0,
    1195,     0,     0,     0,  1198,   696,   697,   698,   699,   700,
       0,     0,   703,   704,   705,   706,     0,   708,   709,   710,
     711,     0,     0,     0,     0,   712,     0,   714,   715,     0,
       0,     0,     0,   718,   719,   720,     0,     0,     0,   724,
       0,   696,   697,   698,   699,   700,     0,   656,   703,   704,
     705,   706,     0,   708,   709,   710,   711,     0,     0,     0,
       0,   712,     0,   714,   715,     0,     0,     0,     0,   686,
     687,     0,     0,     0,     0,   726,     0,   727,   728,   729,
     730,   731,   732,   733,   734,   735,   736,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   737,   738,     0,     0,
       0,     0,     0,  1097,     0,     0,     0,     0,     0,  1260,
       0,  1261,     0,     0,  1263,   729,   730,   731,   732,   733,
     734,   735,   736,     0,     0,     0,     0,     0,  1268,     0,
       0,     0,   737,   738,  1273,  1274,  1275,     0,  1278,     0,
       0,  1280,     0,     0,     0,  1284,  1285,  1286,  1287,     0,
       0,  1289,     0,     0,     0,     0,     0,  1300,     0,     0,
     696,   697,   698,   699,   700,  1305,     0,   703,   704,   705,
     706,     0,   708,   709,   710,   711,     0,     0,     0,     0,
     712,     0,   714,   715,     0,     0,     0,   656,   718,   719,
     720,     0,     0,     0,   724,     0,  1323,  1324,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1335,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     726,  1341,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,     0,     0,     0,  1351,     0,     0,     0,  1353,
       0,   737,   738,     0,     0,     0,     0,     0,  1321,     0,
    1358,   416,  1360,  1361,     0,     0,     3,  1363,   417,   418,
     419,     0,   420,     0,   421,   422,   423,   424,   425,     0,
       0,     0,     0,     0,   426,   427,   428,   429,   430,     0,
     431,   686,   687,     0,     0,     0,     0,   432,   433,     0,
     434,     0,   435,   436,     0,     0,   437,     0,     8,   438,
     439,     0,   440,   441,     0,     0,   442,   443,     0,     0,
       0,     0,     0,   444,     0,     0,   445,   307,   308,   309,
       0,   311,   312,   313,   314,   315,   446,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,     0,   329,
     330,   331,     0,     0,   334,   335,   336,   337,   447,     0,
     448,   449,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   450,   451,     0,     0,     0,     0,     0,
       0,     0,   696,   697,   698,   699,     0,     0,   452,   453,
     454,     0,   706,     0,   708,   709,   710,   711,    57,     0,
       0,     0,   712,     0,   714,   715,   455,   456,   457,   458,
     459,     0,   460,     0,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,    58,   472,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,     0,    13,     0,     0,   477,   478,     0,     0,   732,
     733,   734,   735,   736,     0,   479,     0,   480,   481,   482,
     283,   483,   416,   737,   738,     0,     0,     3,     0,   417,
     418,   419,     0,   420,     0,   421,   422,   423,   424,   425,
       0,     0,     0,     0,     0,   426,   427,   428,   429,   430,
       0,   431,     0,     0,     0,     0,     0,     0,   432,   433,
       0,   434,     0,   435,   436,     0,     0,   437,     0,     8,
     438,   439,     0,   440,   441,     0,     0,   442,   443,     0,
       0,     0,     0,     0,   444,     0,     0,   445,   307,   308,
     309,     0,   311,   312,   313,   314,   315,   446,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,     0,
     329,   330,   331,     0,     0,   334,   335,   336,   337,   447,
       0,   448,   449,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   450,   451,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   452,
     453,   454,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   455,   456,   457,
     458,   459,   391,   460,     0,   461,   462,   463,   464,   465,
     466,   467,   468,   469,   470,   471,    58,   472,   473,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   392,   393,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   474,
     475,   476,     0,    13,     0,     0,   477,   478,     0,     0,
       0,     0,     0,     0,     0,     0,   479,     0,   480,   481,
     482,   283,  1170,   421,   422,   423,   424,   425,     0,     0,
       0,     0,     0,   426,     0,   428,     0,     0,     0,   431,
       0,   391,     0,     0,     0,     0,     0,   433,     0,   394,
       0,     0,   436,   395,     0,   437,     0,     0,   438,     0,
     833,     0,   441,     0,     0,     0,     0,   392,   393,     0,
       0,     0,   542,     0,     0,   445,   307,   308,   309,     0,
     311,   312,   313,   314,   315,   446,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,     0,   329,   330,
     331,     0,     0,   334,   335,   336,   337,   447,     0,   448,
       0,   396,     0,     0,     0,   397,     0,  1007,   398,     0,
       0,     0,   450,   451,     0,     0,     0,     0,   394,     0,
       0,     0,   395,   399,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,   455,   456,   457,   458,   459,
       0,   460,   834,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   835,   543,   473,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,   397,     0,     0,   836,   475,   476,
       0,    13,     0,     0,   477,   478,     0,     0,     0,     0,
       0,     0,   399,     0,   837,     0,   480,   481,     0,   283,
       0,   400,   421,   422,   423,   424,   425,     0,     0,     0,
       0,     0,   426,     0,   428,     0,     0,     0,   431,     0,
     391,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,   436,     0,     0,   437,     0,     0,   438,     0,     0,
       0,   441,     0,     0,     0,     0,   392,   393,     0,     0,
       0,   542,     0,     0,   445,   307,   308,   309,     0,   311,
     312,   313,   314,   315,   446,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,     0,   329,   330,   331,
       0,     0,   334,   335,   336,   337,   447,     0,   448,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   450,   451,     0,     0,     0,     0,   394,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   455,   456,   457,   458,   459,     0,
     460,   834,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   835,   543,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,   397,     0,     0,   836,   475,   476,     0,
      13,     0,     0,   477,   478,     0,     0,     0,     0,     0,
       0,   399,     0,   845,     0,   480,   481,     0,   283,     0,
     400,   421,   422,   423,   424,   425,     0,     0,     0,     0,
       0,   426,     0,   428,     0,     0,     0,   431,   391,   569,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
     436,     0,     0,   437,     0,     0,   438,     0,     0,     0,
     441,     0,     0,     0,   392,   393,     0,     0,     0,     0,
     542,     0,     0,   445,   307,   308,   309,     0,   311,   312,
     313,   314,   315,   446,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,     0,   329,   330,   331,     0,
       0,   334,   335,   336,   337,   447,     0,   448,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     450,   451,     0,     0,     0,   394,     0,     0,   548,   395,
       0,     0,     0,     0,     0,     0,     0,     0,   549,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   455,   456,   457,   458,   459,     0,   460,
       0,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,    58,   543,   473,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   550,   396,     0,     0,
       0,   397,   570,     0,   398,   474,   475,   476,     0,    13,
       0,     0,   477,   478,     0,     0,     0,     0,     0,   399,
     571,     0,   479,     0,   480,   481,     0,   283,   400,   572,
     421,   422,   423,   424,   425,     0,     0,     0,     0,     0,
     426,     0,   428,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,     0,   436,
       0,     0,   437,     0,     0,   438,     0,     0,     0,   441,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
       0,     0,   445,   307,   308,   309,     0,   311,   312,   313,
     314,   315,   446,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,     0,   329,   330,   331,     0,     0,
     334,   335,   336,   337,   447,     0,   448,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   450,
     451,     0,     0,     0,     0,     0,     0,     0,   610,     0,
       0,     0,     0,     0,   452,   453,   454,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,   455,   456,   457,   458,   459,     0,   460,     0,
     461,   462,   463,   464,   465,   466,   467,   468,   469,   470,
     471,    58,   543,   473,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,     0,    13,     0,
       0,   477,   478,     0,     0,   421,   422,   423,   424,   425,
       0,   479,     0,   480,   481,   426,   283,   428,     0,     0,
     391,   431,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,   436,     0,     0,   437,     0,     0,
     438,     0,     0,     0,   441,     0,   392,   393,     0,     0,
       0,     0,     0,     0,   542,     0,     0,   445,   307,   308,
     309,     0,   311,   312,   313,   314,   315,   446,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,     0,
     329,   330,   331,     0,     0,   334,   335,   336,   337,   447,
       0,   448,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   450,   451,     0,   394,     0,     0,
       0,   395,     0,   637,     0,     0,     0,     0,     0,   452,
     453,   454,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,   455,   456,   457,
     458,   459,     0,   460,     0,   461,   462,   463,   464,   465,
     466,   467,   468,   469,   470,   471,    58,   543,   473,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,   397,     0,  1131,   398,     0,     0,   474,
     475,   476,     0,    13,     0,     0,   477,   478,     0,     0,
       0,   399,     0,     0,     0,     0,   479,     0,   480,   481,
     400,   283,   421,   422,   423,   424,   425,     0,     0,     0,
       0,     0,   426,  1293,   428,   429,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,   436,     0,     0,   437,     0,     0,   438,   439,     0,
       0,   441,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,     0,     0,   445,   307,   308,   309,     0,   311,
     312,   313,   314,   315,   446,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,     0,   329,   330,   331,
       0,     0,   334,   335,   336,   337,   447,     0,   448,  1294,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   450,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   455,   456,   457,   458,   459,     0,
     460,     0,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,    58,   543,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,     0,
      13,     0,     0,   477,   478,     0,     0,   421,   422,   423,
     424,   425,     0,   479,     0,   480,   481,   426,   283,   428,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,   436,     0,     0,   437,
       0,     0,   438,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   542,     0,     0,   445,
     307,   308,   309,     0,   311,   312,   313,   314,   315,   446,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,     0,   329,   330,   331,     0,     0,   334,   335,   336,
     337,   447,     0,   448,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   450,   451,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   452,   453,   454,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   455,
     456,   457,   458,   459,     0,   460,     0,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   471,    58,   543,
     473,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   474,   475,   476,     0,    13,     0,     0,   477,   478,
       0,     0,   421,   422,   423,   424,   425,     0,   479,     0,
     480,   481,   426,   283,   428,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,   436,     0,     0,   437,     0,     0,   438,     0,     0,
       0,   441,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,     0,     0,   445,   307,   308,   309,     0,   311,
     312,   313,   314,   315,   446,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,     0,   329,   330,   331,
       0,     0,   334,   335,   336,   337,   447,     0,   448,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   450,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   901,   902,   903,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   455,   456,   457,   458,   459,     0,
     460,     0,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,    58,   543,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,     0,
      13,     0,     0,   477,   478,     0,     0,   421,   422,   423,
     424,   425,     0,   479,     0,   480,   481,   426,   283,   428,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,   436,     0,     0,   437,
       0,     0,   438,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   542,     0,     0,   445,
     307,   308,   309,     0,   311,   312,   313,   314,   315,   446,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,     0,   329,   330,   331,     0,     0,   334,   335,   336,
     337,   447,     0,   448,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   450,   451,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   924,   925,   926,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   455,
     456,   457,   458,   459,     0,   460,     0,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   471,    58,   543,
     473,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   474,   475,   476,     0,    13,     0,     0,   477,   478,
       0,     0,   421,   422,   423,   424,   425,     0,   479,     0,
     480,   481,   426,   283,   428,     0,     0,   391,   431,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,   436,     0,     0,   437,     0,     0,   438,     0,     0,
       0,   441,     0,   392,   393,     0,     0,     0,     0,     0,
       0,   542,     0,     0,   445,   307,   308,   309,     0,   311,
     312,   313,   314,   315,   446,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,     0,   329,   330,   331,
       0,     0,   334,   335,   336,   337,   447,     0,   448,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   450,   451,     0,   394,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,   455,   456,   457,   458,   459,     0,
     460,     0,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,    58,   543,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
     397,     0,  1136,   398,     0,     0,   474,   475,   476,     0,
      13,     0,     0,   477,   478,     0,     0,     0,   399,     0,
       0,   867,     0,   479,   868,   480,   481,   400,   283,   421,
     422,   423,   424,   425,     0,     0,     0,     0,     0,   426,
       0,   428,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,   436,     0,
       0,   437,     0,     0,   438,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,     0,
       0,   445,   307,   308,   309,     0,   311,   312,   313,   314,
     315,   446,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,     0,   329,   330,   331,     0,     0,   334,
     335,   336,   337,   447,     0,   448,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   450,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   455,   456,   457,   458,   459,     0,   460,     0,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
      58,   543,   473,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,   476,     0,    13,     0,     0,
     477,   478,     0,     0,   421,   422,   423,   424,   425,   573,
     479,     0,   480,   481,   426,   283,   428,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,   436,     0,     0,   437,     0,     0,   438,
       0,     0,     0,   441,     0,     0,     0,     0,     0,   618,
       0,     0,     0,   542,     0,     0,   445,   307,   308,   309,
       0,   311,   312,   313,   314,   315,   446,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,     0,   329,
     330,   331,     0,     0,   334,   335,   336,   337,   447,     0,
     448,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   450,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   455,   456,   457,   458,
     459,     0,   460,     0,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,    58,   543,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,     0,    13,     0,     0,   477,   478,     0,     0,   421,
     422,   423,   424,   425,     0,   479,     0,   480,   481,   426,
     283,   428,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,   436,     0,
       0,   437,     0,     0,   438,     0,     0,     0,   441,     0,
       0,   629,     0,     0,     0,     0,     0,     0,   542,     0,
       0,   445,   307,   308,   309,     0,   311,   312,   313,   314,
     315,   446,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,     0,   329,   330,   331,     0,     0,   334,
     335,   336,   337,   447,     0,   448,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   450,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   455,   456,   457,   458,   459,     0,   460,     0,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
      58,   543,   473,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,   476,     0,    13,     0,     0,
     477,   478,     0,     0,   421,   422,   423,   424,   425,     0,
     479,     0,   480,   481,   426,   283,   428,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,   436,     0,     0,   437,     0,     0,   438,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,     0,     0,   445,   307,   308,   309,
       0,   311,   312,   313,   314,   315,   446,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,     0,   329,
     330,   331,     0,     0,   334,   335,   336,   337,   447,     0,
     448,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   450,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   455,   456,   457,   458,
     459,     0,   460,     0,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,    58,   543,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   659,     0,   474,   475,
     476,     0,    13,     0,     0,   477,   478,     0,     0,   421,
     422,   423,   424,   425,     0,   479,   928,   480,   481,   426,
     283,   428,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,   436,     0,
       0,   437,     0,     0,   438,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,     0,
       0,   445,   307,   308,   309,     0,   311,   312,   313,   314,
     315,   446,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,     0,   329,   330,   331,     0,     0,   334,
     335,   336,   337,   447,     0,   448,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   450,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   455,   456,   457,   458,   459,     0,   460,     0,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
      58,   543,   473,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,   476,     0,    13,     0,     0,
     477,   478,     0,     0,   421,   422,   423,   424,   425,     0,
     479,     0,   480,   481,   426,   283,   428,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,   436,     0,     0,   437,     0,     0,   438,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,     0,     0,   445,   307,   308,   309,
       0,   311,   312,   313,   314,   315,   446,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,     0,   329,
     330,   331,     0,     0,   334,   335,   336,   337,   447,     0,
     448,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   450,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   455,   456,   457,   458,
     459,     0,   460,     0,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,    58,   543,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,     0,    13,     0,     0,   477,   478,     0,     0,   421,
     422,   423,   424,   425,     0,   479,   948,   480,   481,   426,
     283,   428,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,   436,     0,
       0,   437,     0,     0,   438,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,     0,
       0,   445,   307,   308,   309,     0,   311,   312,   313,   314,
     315,   446,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,     0,   329,   330,   331,     0,     0,   334,
     335,   336,   337,   447,     0,   448,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   450,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   455,   456,   457,   458,   459,     0,   460,     0,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
      58,   543,   473,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,   476,     0,    13,     0,     0,
     477,   478,     0,     0,   421,   422,   423,   424,   425,     0,
     479,   984,   480,   481,   426,   283,   428,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,   436,     0,     0,   437,     0,     0,   438,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,     0,     0,   445,   307,   308,   309,
       0,   311,   312,   313,   314,   315,   446,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,     0,   329,
     330,   331,     0,     0,   334,   335,   336,   337,   447,     0,
     448,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   450,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   455,   456,   457,   458,
     459,     0,   460,     0,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,    58,   543,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   986,     0,   474,   475,
     476,     0,    13,     0,     0,   477,   478,     0,     0,   421,
     422,   423,   424,   425,     0,   479,     0,   480,   481,   426,
     283,   428,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,   436,     0,
       0,   437,     0,     0,   438,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,     0,
       0,   445,   307,   308,   309,     0,   311,   312,   313,   314,
     315,   446,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,     0,   329,   330,   331,     0,     0,   334,
     335,   336,   337,   447,     0,   448,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   450,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   455,   456,   457,   458,   459,     0,   460,     0,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
      58,   543,   473,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,   476,     0,    13,     0,     0,
     477,   478,     0,     0,   421,   422,   423,   424,   425,     0,
     479,  1019,   480,   481,   426,   283,   428,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,   436,     0,     0,   437,     0,     0,   438,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,     0,     0,   445,   307,   308,   309,
       0,   311,   312,   313,   314,   315,   446,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,     0,   329,
     330,   331,     0,     0,   334,   335,   336,   337,   447,     0,
     448,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   450,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,   455,   456,   457,   458,
     459,     0,   460,     0,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,    58,   543,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,     0,    13,     0,     0,   477,   478,     0,     0,   421,
     422,   423,   424,   425,     0,   479,  1034,   480,   481,   426,
     283,   428,     0,     0,   391,   431,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,   436,     0,
       0,   437,     0,     0,   438,     0,     0,     0,   441,     0,
     392,   393,     0,     0,     0,     0,     0,     0,   542,     0,
       0,   445,   307,   308,   309,     0,   311,   312,   313,   314,
     315,   446,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,     0,   329,   330,   331,     0,     0,   334,
     335,   336,   337,   447,     0,   448,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   450,   451,
       0,   394,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,   455,   456,   457,   458,   459,     0,   460,     0,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
      58,   543,   473,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,   397,     0,  1137,
     398,     0,     0,   474,   475,   476,     0,    13,     0,     0,
     477,   478,     0,     0,     0,   399,     0,     0,  1176,     0,
     479,     0,   480,   481,   400,   283,   421,   422,   423,   424,
     425,     0,     0,     0,     0,     0,   426,     0,   428,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,   436,     0,     0,   437,     0,
       0,   438,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,     0,     0,   445,   307,
     308,   309,     0,   311,   312,   313,   314,   315,   446,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
       0,   329,   330,   331,     0,     0,   334,   335,   336,   337,
     447,     0,   448,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   450,   451,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   455,   456,
     457,   458,   459,     0,   460,     0,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,    58,   543,   473,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,     0,    13,     0,     0,   477,   478,     0,
       0,   421,   422,   423,   424,   425,     0,   479,  1190,   480,
     481,   426,   283,   428,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
     436,     0,     0,   437,     0,     0,   438,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,     0,     0,   445,   307,   308,   309,     0,   311,   312,
     313,   314,   315,   446,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,     0,   329,   330,   331,     0,
       0,   334,   335,   336,   337,   447,     0,   448,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     450,   451,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   455,   456,   457,   458,   459,     0,   460,
       0,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,    58,   543,   473,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,   476,     0,    13,
       0,     0,   477,   478,     0,     0,   421,   422,   423,   424,
     425,     0,   479,  1224,   480,   481,   426,   283,   428,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,   436,     0,     0,   437,     0,
       0,   438,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,     0,     0,   445,   307,
     308,   309,     0,   311,   312,   313,   314,   315,   446,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
       0,   329,   330,   331,     0,     0,   334,   335,   336,   337,
     447,     0,   448,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   450,   451,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   455,   456,
     457,   458,   459,     0,   460,     0,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,    58,   543,   473,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,     0,    13,     0,     0,   477,   478,     0,
       0,   421,   422,   423,   424,   425,     0,   479,  1277,   480,
     481,   426,   283,   428,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
     436,     0,     0,   437,     0,     0,   438,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,     0,     0,   445,   307,   308,   309,     0,   311,   312,
     313,   314,   315,   446,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,     0,   329,   330,   331,     0,
       0,   334,   335,   336,   337,   447,     0,   448,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     450,   451,     0,     0,     0,     0,     0,     0,     0,  1304,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   455,   456,   457,   458,   459,     0,   460,
       0,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,    58,   543,   473,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,   476,     0,    13,
       0,     0,   477,   478,     0,     0,   421,   422,   423,   424,
     425,     0,   479,     0,   480,   481,   426,   283,   428,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,   436,     0,     0,   437,     0,
       0,   438,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,     0,     0,   445,   307,
     308,   309,     0,   311,   312,   313,   314,   315,   446,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
       0,   329,   330,   331,     0,     0,   334,   335,   336,   337,
     447,     0,   448,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   450,   451,     0,     0,     0,
       0,     0,     0,     0,  1329,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   455,   456,
     457,   458,   459,     0,   460,     0,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,    58,   543,   473,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,     0,    13,     0,     0,   477,   478,     0,
       0,   421,   422,   423,   424,   425,     0,   479,     0,   480,
     481,   426,   283,   428,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
     436,     0,     0,   437,     0,     0,   438,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,     0,     0,   445,   307,   308,   309,     0,   311,   312,
     313,   314,   315,   446,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,     0,   329,   330,   331,     0,
       0,   334,   335,   336,   337,   447,     0,   448,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     450,   451,     0,     0,     0,     0,     0,     0,     0,  1330,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   455,   456,   457,   458,   459,     0,   460,
       0,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,    58,   543,   473,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,   476,     0,    13,
       0,     0,   477,   478,     0,     0,   421,   422,   423,   424,
     425,     0,   479,     0,   480,   481,   426,   283,   428,     0,
       0,   391,   431,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,   436,     0,     0,   437,     0,
       0,   438,     0,     0,     0,   441,     0,   392,   393,     0,
       0,     0,     0,     0,     0,   542,     0,     0,   445,   307,
     308,   309,     0,   311,   312,   313,   314,   315,   446,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
       0,   329,   330,   331,     0,     0,   334,   335,   336,   337,
     447,     0,   448,     0,     0,     0,     0,     0,   391,     0,
       0,     0,     0,     0,     0,   450,   451,     0,   394,   -67,
       0,     0,   395,     0,     0,     0,     0,     0,     0,     0,
     686,   687,     0,     0,   392,   393,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,   455,   456,
     457,   458,   459,     0,   460,     0,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,    58,   543,   473,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,   397,     0,  1138,   398,     0,     0,
     474,   475,   476,     0,    13,   394,     0,   477,   478,   395,
       0,     0,   399,     0,     0,     0,     0,   479,     0,   480,
     481,   400,   283,   688,   689,   690,   691,   692,   693,   694,
     695,   696,   697,   698,   699,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,   710,   711,   686,   687,     0,
       0,   712,   713,   714,   715,   716,   717,     0,     0,   718,
     719,   720,   721,   722,   723,   724,     0,   396,     0,     0,
       0,   397,     0,  1139,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
     725,   726,     0,   727,   728,   729,   730,   731,   732,   733,
     734,   735,   736,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   737,   738,     0,     0,     0,     0,     0,     0,
     688,   689,   690,   691,   692,   693,   694,   695,   696,   697,
     698,   699,   700,   701,   702,   703,   704,   705,   706,   802,
     708,   709,   710,   711,   686,   687,     0,     0,   712,   713,
     714,   715,   716,   717,     0,     0,   718,   719,   720,   721,
     722,   723,   724,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   686,   687,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   803,   726,     0,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   737,
     738,     0,     0,     0,     0,     0,   959,   688,   689,   690,
     691,   692,   693,   694,   695,   696,   697,   698,   699,   700,
     701,   702,   703,   704,   705,   706,   802,   708,   709,   710,
     711,     0,     0,     0,     0,   712,   713,   714,   715,   716,
     717,     0,     0,   718,   719,   720,   721,   722,   723,   724,
       0,     0,   696,   697,   698,   699,   700,     0,     0,   703,
     704,   705,   706,     0,   708,   709,   710,   711,   686,   687,
       0,     0,   712,     0,   714,   715,     0,     0,     0,     0,
     718,   719,   720,     0,   803,   726,   724,   727,   728,   729,
     730,   731,   732,   733,   734,   735,   736,     0,     0,     0,
       0,     0,     0,   686,   687,     0,   737,   738,     0,     0,
       0,     0,     0,  1105,     0,     0,     0,     0,     0,     0,
       0,     0,   726,     0,   727,   728,   729,   730,   731,   732,
     733,   734,   735,   736,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   737,   738,     0,     0,     0,     0,     0,
    1364,     0,     0,     0,     0,     0,     0,     0,   391,   696,
     697,   698,   699,   700,     0,     0,   703,   704,   705,   706,
       0,   708,   709,   710,   711,     0,     0,     0,     0,   712,
       0,   714,   715,     0,   392,   393,     0,   718,   719,   720,
       0,     0,     0,   724,   696,   697,   698,   699,   700,   686,
     687,   703,   704,   705,   706,     0,   708,   709,   710,   711,
       0,     0,     0,     0,   712,     0,   714,   715,     0,     0,
       0,     0,   718,   719,   720,     0,     0,     0,   724,   726,
       0,   727,   728,   729,   730,   731,   732,   733,   734,   735,
     736,     0,     0,     0,     0,   394,     0,     0,     0,   395,
     737,   738,     0,     0,     0,     0,     0,  1366,     0,     0,
       0,     0,     0,     0,   726,     0,   727,   728,   729,   730,
     731,   732,   733,   734,   735,   736,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   737,   738,     0,     0,   748,
     696,   697,   698,   699,   700,   686,   687,   703,   704,   705,
     706,     0,   708,   709,   710,   711,     0,   396,     0,     0,
     712,   397,   714,   715,   398,     0,     0,     0,   718,   719,
     720,     0,     0,     0,   724,     0,     0,     0,     0,   399,
     686,   687,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     726,     0,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   737,   738,     0,     0,   772,   696,   697,   698,   699,
     700,     0,     0,   703,   704,   705,   706,     0,   708,   709,
     710,   711,     0,     0,     0,     0,   712,     0,   714,   715,
       0,     0,     0,     0,   718,   719,   720,     0,     0,     0,
     724,   696,   697,   698,   699,   700,   686,   687,   703,   704,
     705,   706,     0,   708,   709,   710,   711,     0,     0,     0,
       0,   712,     0,   714,   715,     0,     0,     0,     0,   718,
     719,   720,     0,     0,     0,   724,   726,     0,   727,   728,
     729,   730,   731,   732,   733,   734,   735,   736,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   737,   738,     0,
       0,   783,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   726,     0,   727,   728,   729,   730,   731,   732,   733,
     734,   735,   736,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   737,   738,     0,     0,   858,   696,   697,   698,
     699,   700,   686,   687,   703,   704,   705,   706,     0,   708,
     709,   710,   711,     0,     0,     0,     0,   712,     0,   714,
     715,     0,     0,     0,     0,   718,   719,   720,     0,     0,
       0,   724,     0,     0,     0,     0,     0,   686,   687,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   726,     0,   727,
     728,   729,   730,   731,   732,   733,   734,   735,   736,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   737,   738,
       0,     0,   979,   696,   697,   698,   699,   700,     0,     0,
     703,   704,   705,   706,     0,   708,   709,   710,   711,     0,
       0,     0,     0,   712,     0,   714,   715,     0,     0,     0,
       0,   718,   719,   720,     0,     0,     0,   724,   696,   697,
     698,   699,   700,   686,   687,   703,   704,   705,   706,     0,
     708,   709,   710,   711,     0,     0,     0,     0,   712,     0,
     714,   715,     0,     0,     0,     0,   718,   719,   720,     0,
       0,     0,   724,   726,     0,   727,   728,   729,   730,   731,
     732,   733,   734,   735,   736,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   737,   738,     0,     0,   997,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   726,     0,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   737,
     738,     0,     0,  1001,   696,   697,   698,   699,   700,   686,
     687,   703,   704,   705,   706,     0,   708,   709,   710,   711,
       0,     0,     0,     0,   712,     0,   714,   715,     0,     0,
       0,     0,   718,   719,   720,     0,     0,     0,   724,     0,
       0,     0,     0,     0,   686,   687,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   726,     0,   727,   728,   729,   730,
     731,   732,   733,   734,   735,   736,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   737,   738,     0,     0,  1003,
     696,   697,   698,   699,   700,     0,     0,   703,   704,   705,
     706,     0,   708,   709,   710,   711,     0,     0,     0,     0,
     712,     0,   714,   715,     0,     0,     0,     0,   718,   719,
     720,     0,     0,     0,   724,   696,   697,   698,   699,   700,
     686,   687,   703,   704,   705,   706,     0,   708,   709,   710,
     711,     0,     0,     0,     0,   712,     0,   714,   715,     0,
       0,     0,     0,   718,   719,   720,     0,     0,     0,   724,
     726,     0,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   737,   738,     0,     0,  1008,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   726,     0,   727,   728,   729,
     730,   731,   732,   733,   734,   735,   736,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   737,   738,     0,     0,
    1009,   696,   697,   698,   699,   700,   686,   687,   703,   704,
     705,   706,     0,   708,   709,   710,   711,     0,     0,     0,
       0,   712,     0,   714,   715,     0,     0,     0,     0,   718,
     719,   720,     0,     0,     0,   724,     0,     0,     0,     0,
       0,   686,   687,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   726,     0,   727,   728,   729,   730,   731,   732,   733,
     734,   735,   736,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   737,   738,     0,     0,  1010,   696,   697,   698,
     699,   700,     0,     0,   703,   704,   705,   706,     0,   708,
     709,   710,   711,     0,     0,     0,     0,   712,     0,   714,
     715,     0,     0,     0,     0,   718,   719,   720,     0,     0,
       0,   724,   696,   697,   698,   699,   700,   686,   687,   703,
     704,   705,   706,     0,   708,   709,   710,   711,     0,     0,
       0,     0,   712,     0,   714,   715,     0,     0,     0,     0,
     718,   719,   720,     0,     0,     0,   724,   726,     0,   727,
     728,   729,   730,   731,   732,   733,   734,   735,   736,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   737,   738,
       0,     0,  1011,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   726,     0,   727,   728,   729,   730,   731,   732,
     733,   734,   735,   736,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   737,   738,     0,     0,  1012,   696,   697,
     698,   699,   700,   686,   687,   703,   704,   705,   706,     0,
     708,   709,   710,   711,     0,     0,     0,     0,   712,     0,
     714,   715,     0,     0,     0,     0,   718,   719,   720,     0,
       0,     0,   724,     0,     0,     0,     0,     0,   686,   687,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   726,     0,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   737,
     738,     0,     0,  1013,   696,   697,   698,   699,   700,     0,
       0,   703,   704,   705,   706,     0,   708,   709,   710,   711,
       0,     0,     0,     0,   712,     0,   714,   715,     0,     0,
       0,     0,   718,   719,   720,     0,     0,     0,   724,   696,
     697,   698,   699,   700,   686,   687,   703,   704,   705,   706,
       0,   708,   709,   710,   711,     0,     0,     0,     0,   712,
       0,   714,   715,     0,     0,     0,     0,   718,   719,   720,
       0,     0,     0,   724,   726,     0,   727,   728,   729,   730,
     731,   732,   733,   734,   735,   736,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   737,   738,     0,     0,  1129,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   726,
       0,   727,   728,   729,   730,   731,   732,   733,   734,   735,
     736,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     737,   738,     0,     0,  1227,   696,   697,   698,   699,   700,
     686,   687,   703,   704,   705,   706,     0,   708,   709,   710,
     711,     0,     0,     0,     0,   712,     0,   714,   715,     0,
       0,     0,     0,   718,   719,   720,     0,     0,     0,   724,
       0,     0,     0,     0,     0,   686,   687,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   726,     0,   727,   728,   729,
     730,   731,   732,   733,   734,   735,   736,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   737,   738,     0,     0,
    1237,   696,   697,   698,   699,   700,     0,     0,   703,   704,
     705,   706,     0,   708,   709,   710,   711,     0,     0,     0,
       0,   712,     0,   714,   715,     0,     0,     0,     0,   718,
     719,   720,     0,     0,     0,   724,   696,   697,   698,   699,
     700,   686,   687,   703,   704,   705,   706,     0,   708,   709,
     710,   711,     0,     0,     0,     0,   712,     0,   714,   715,
       0,     0,     0,     0,   718,   719,   720,     0,     0,     0,
     724,   726,     0,   727,   728,   729,   730,   731,   732,   733,
     734,   735,   736,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   737,   738,     0,     0,  1238,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   726,     0,   727,   728,
     729,   730,   731,   732,   733,   734,   735,   736,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   737,   738,     0,
       0,  1254,   696,   697,   698,   699,   700,   686,   687,   703,
     704,   705,   706,     0,   708,   709,   710,   711,     0,     0,
       0,     0,   712,     0,   714,   715,     0,     0,     0,     0,
     718,   719,   720,     0,     0,     0,   724,     0,     0,     0,
       0,     0,   686,   687,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   726,     0,   727,   728,   729,   730,   731,   732,
     733,   734,   735,   736,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   737,   738,     0,     0,  1256,   696,   697,
     698,   699,   700,     0,     0,   703,   704,   705,   706,     0,
     708,   709,   710,   711,     0,     0,     0,     0,   712,     0,
     714,   715,     0,     0,     0,     0,   718,   719,   720,     0,
       0,     0,   724,   696,   697,   698,   699,   700,   686,   687,
     703,   704,   705,   706,     0,   708,   709,   710,   711,     0,
       0,     0,     0,   712,     0,   714,   715,     0,     0,     0,
       0,   718,   719,   720,     0,     0,     0,   724,   726,     0,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   737,
     738,     0,     0,  1258,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   726,     0,   727,   728,   729,   730,   731,
     732,   733,   734,   735,   736,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   737,   738,     0,     0,  1262,   696,
     697,   698,   699,   700,   686,   687,   703,   704,   705,   706,
       0,   708,   709,   710,   711,     0,     0,     0,     0,   712,
       0,   714,   715,     0,     0,     0,     0,   718,   719,   720,
       0,     0,     0,   724,     0,     0,     0,     0,     0,   686,
     687,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   726,
       0,   727,   728,   729,   730,   731,   732,   733,   734,   735,
     736,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     737,   738,     0,     0,  1309,   696,   697,   698,   699,   700,
       0,     0,   703,   704,   705,   706,     0,   708,   709,   710,
     711,     0,     0,     0,     0,   712,     0,   714,   715,     0,
       0,     0,     0,   718,   719,   720,     0,     0,     0,   724,
     696,   697,   698,   699,   700,   686,   687,   703,   704,   705,
     706,     0,   708,   709,   710,   711,     0,     0,     0,     0,
     712,     0,   714,   715,     0,     0,     0,     0,   718,   719,
     720,     0,     0,     0,   724,   726,     0,   727,   728,   729,
     730,   731,   732,   733,   734,   735,   736,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   737,   738,     0,     0,
    1310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     726,     0,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   737,   738,     0,     0,  1311,   696,   697,   698,   699,
     700,   686,   687,   703,   704,   705,   706,     0,   708,   709,
     710,   711,     0,     0,     0,     0,   712,     0,   714,   715,
       0,     0,     0,     0,   718,   719,   720,     0,     0,     0,
     724,     0,     0,     0,     0,     0,   686,   687,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   726,     0,   727,   728,
     729,   730,   731,   732,   733,   734,   735,   736,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   737,   738,     0,
       0,  1315,   696,   697,   698,   699,   700,     0,     0,   703,
     704,   705,   706,     0,   708,   709,   710,   711,     0,     0,
       0,     0,   712,     0,   714,   715,     0,     0,     0,     0,
     718,   719,   720,     0,     0,     0,   724,   696,   697,   698,
     699,   700,   686,   687,   703,   704,   705,   706,     0,   708,
     709,   710,   711,     0,     0,     0,     0,   712,     0,   714,
     715,     0,     0,     0,     0,   718,   719,   720,     0,     0,
       0,   724,   726,     0,   727,   728,   729,   730,   731,   732,
     733,   734,   735,   736,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   737,   738,     0,     0,  1320,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   726,     0,   727,
     728,   729,   730,   731,   732,   733,   734,   735,   736,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   737,   738,
       0,     0,  1325,   696,   697,   698,   699,   700,     0,     0,
     703,   704,   705,   706,     0,   708,   709,   710,   711,     0,
       0,     0,     0,   712,     0,   714,   715,     0,     0,     0,
       0,   718,   719,   720,     0,     0,     0,   724,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   686,   687,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   303,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   726,     0,   727,   728,   729,   730,   731,
     732,   733,   734,   735,   736,   305,     0,     0,     0,     0,
       0,     0,     0,     0,   737,   738,     0,     0,  1357,     0,
     306,     0,     0,     0,     0,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   696,   697,
     698,   699,   700,     0,     0,   703,   704,   705,   706,     0,
     708,   709,   710,   711,     0,     0,     0,     0,   712,     0,
     714,   715,   686,   687,     0,     0,   718,   719,   720,     0,
       0,     0,   724,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,     0,     0,     0,     0,   686,   687,     0,
       0,     0,     0,   580,     0,     0,     0,     0,   726,     0,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   737,
     738,   774,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   581,     0,     0,
       0,     0,     0,   696,   697,   698,   699,   700,     0,     0,
     703,   704,   705,   706,     0,   708,   709,   710,   711,     0,
       0,     0,     0,   712,     0,   714,   715,     0,     0,     0,
       0,   718,   719,   720,     0,     0,     0,   724,   696,   697,
     698,   699,   700,   686,   687,   703,   704,   705,   706,     0,
     708,   709,   710,   711,     0,     0,     0,     0,   712,     0,
     714,   715,     0,     0,     0,     0,   718,   719,   720,     0,
       0,     0,   724,   726,     0,   727,   728,   729,   730,   731,
     732,   733,   734,   735,   736,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   737,   738,  1077,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   726,     0,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
       0,   876,     0,     0,     0,     0,     0,     0,     0,   737,
     738,  1094,     0,     0,   696,   697,   698,   699,   700,   686,
     687,   703,   704,   705,   706,     0,   708,   709,   710,   711,
       0,     0,     0,     0,   712,     0,   714,   715,     0,     0,
       0,     0,   718,   719,   720,     0,     0,     0,   724,     0,
     307,   308,   309,     0,   311,   312,   313,   314,   315,   446,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,     0,   329,   330,   331,     0,     0,   334,   335,   336,
     337,     0,     0,     0,   726,     0,   727,   728,   729,   730,
     731,   732,   733,   734,   735,   736,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   737,   738,  1197,   391,     0,
     696,   697,   698,   699,   700,     0,     0,   703,   704,   705,
     706,     0,   708,   709,   710,   711,   686,   687,     0,     0,
     712,     0,   714,   715,   392,   393,   877,     0,   718,   719,
     720,     0,     0,     0,   724,     0,     0,     0,   878,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     726,     0,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,     0,     0,     0,   394,     0,     0,     0,   395,
       0,   737,   738,  1200,     0,     0,     0,     0,     0,   688,
     689,   690,   691,   692,   693,   694,   695,   696,   697,   698,
     699,   700,   701,   702,   703,   704,   705,   706,   802,   708,
     709,   710,   711,  -326,     0,   228,   229,   712,   713,   714,
     715,   716,   717,     0,     0,   718,   719,   720,   721,   722,
     723,   724,   230,     0,     0,     0,     0,   396,     0,     0,
       0,   397,     0,  1164,   398,     0,     0,     0,     0,     0,
     686,   687,     0,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,   803,   726,   400,   727,
     728,   729,   730,   731,   732,   733,   734,   735,   736,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   737,   738,
       0,     0,     0,     0,     0,     0,     0,     0,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,     0,     0,   249,   250,
     251,     0,     0,     0,     0,     0,     0,   252,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,   261,   262,
     263,   696,   697,   698,   699,   700,   686,   687,   703,   704,
     705,   706,     0,   708,   709,   710,   711,     0,     0,     0,
       0,   712,     0,   714,   715,     0,     0,   848,     0,   718,
     719,   720,     0,     0,     0,   724,   264,     0,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,     0,     0,
     275,   276,     0,     0,     0,     0,     0,   277,   278,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   726,     0,   727,   728,   729,   730,   731,   732,   733,
     734,   735,   736,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   737,   738,     0,     0,     0,   696,   697,   698,
     699,   700,   686,   687,   703,   704,   705,   706,     0,   708,
     709,   710,   711,     0,     0,     0,     0,   712,     0,   714,
     715,     0,     0,  1020,     0,   718,   719,   720,     0,     0,
       0,   724,     0,     0,     0,     0,     0,   686,   687,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   726,     0,   727,
     728,   729,   730,   731,   732,   733,   734,   735,   736,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   737,   738,
       0,     0,     0,   696,   697,   698,   699,   700,     0,     0,
     703,   704,   705,   706,     0,   708,   709,   710,   711,     0,
       0,     0,     0,   712,     0,   714,   715,     0,     0,     0,
       0,   718,   719,   720,     0,     0,     0,   724,   696,   697,
     698,   699,   700,   686,   687,   703,   704,   705,   706,     0,
     708,   709,   710,   711,     0,     0,     0,     0,   712,     0,
     714,   715,     0,     0,     0,     0,   718,   719,   720,     0,
       0,     0,   724,   726,  1085,   727,   728,   729,   730,   731,
     732,   733,   734,   735,   736,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   737,   738,     0,     0,     0,  1165,
       0,     0,     0,     0,     0,     0,     0,     0,   726,     0,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   737,
     738,     0,     0,     0,   696,   697,   698,   699,   700,   686,
     687,   703,   704,   705,   706,     0,   708,   709,   710,   711,
       0,     0,     0,     0,   712,     0,   714,   715,     0,     0,
       0,     0,   718,   719,   720,     0,     0,     0,   724,     0,
       0,     0,     0,     0,   686,   687,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   726,     0,   727,   728,   729,   730,
     731,   732,   733,   734,   735,   736,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   737,   738,     0,     0,     0,
     696,   697,   698,   699,   700,     0,     0,   703,   704,   705,
     706,     0,   708,   709,   710,   711,     0,     0,     0,     0,
     712,     0,   714,   715,     0,     0,     0,     0,   718,   719,
     720,     0,     0,     0,  -684,   696,   697,   698,   699,   700,
     686,   687,   703,   704,   705,   706,     0,   708,   709,   710,
     711,     0,     0,     0,     0,   712,     0,   714,   715,     0,
       0,     0,     0,   718,     0,   720,     0,     0,     0,     0,
     726,     0,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,   686,   687,     0,     0,     0,     0,     0,     0,
       0,   737,   738,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   727,   728,   729,
     730,   731,   732,   733,   734,   735,   736,     0,     0,     0,
       0,     0,     0,   686,   687,     0,   737,   738,     0,     0,
       0,   696,   697,   698,   699,   700,     0,     0,   703,   704,
     705,   706,     0,   708,   709,   710,   711,     0,     0,     0,
       0,   712,     0,   714,   715,     0,     0,     0,     0,   718,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   391,     0,   696,   697,   698,   699,   700,     0,     0,
     703,   704,   705,   706,     0,   708,   709,   710,   711,     0,
       0,     0,     0,   712,     0,   714,   715,   392,   393,     0,
       0,     0,     0,   727,   728,   729,   730,   731,   732,   733,
     734,   735,   736,   391,   696,   697,   698,   699,   700,     0,
       0,   703,   737,   738,   706,     0,   708,   709,   710,   711,
     391,     0,     0,     0,   712,     0,   714,   715,     0,   392,
     393,     0,     0,     0,     0,     0,   728,   729,   730,   731,
     732,   733,   734,   735,   736,     0,   392,   393,   394,     0,
       0,     0,   395,     0,   737,   738,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   730,
     731,   732,   733,   734,   735,   736,     0,     0,     0,     0,
     394,     0,     0,     0,   395,   737,   738,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   394,     0,     0,
     396,   395,     0,     0,   397,     0,  1218,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,   652,
       0,     0,   396,     0,     0,     0,   397,     0,  1239,   398,
       0,   303,     0,     0,     0,     0,     0,   304,     0,   396,
       0,     0,     0,   397,   399,  1306,   398,     0,     0,     0,
       0,     0,     0,   400,   305,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,   306,
     400,     0,     0,     0,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   654,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     303,     0,     0,     0,     0,     0,   304,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,   305,     0,     0,     0,   880,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   306,     0,
       0,     0,    58,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   307,   308,   309,     0,
     311,   312,   313,   314,   315,   446,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,     0,   329,   330,
     331,     0,     0,   334,   335,   336,   337,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,  1081,     0,     0,     0,     0,   340,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   881,     0,     0,     0,     0,     0,     0,     0,
       0,   307,   308,   309,   882,   311,   312,   313,   314,   315,
     446,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,     0,   329,   330,   331,     0,     0,   334,   335,
     336,   337,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,     0,     0,
       0,     0,     0,   304,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,     0,     0,     0,     0,     0,     0,  1082,     0,     0,
       0,     0,     0,     0,     0,   306,     0,     0,     0,  1083,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   303,     0,     0,     0,
       0,     0,   304,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,   305,
       0,     0,     0,     0,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   306,     0,     0,     0,    58,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   180,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   340,     0,     0,     0,     0,   181,     0,
     182,     0,   183,   184,   185,   186,   187,   580,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
     199,   200,   201,     0,     0,   202,   203,   204,   205,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   206,   207,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   208
};

static const yytype_int16 yycheck[] =
{
      13,    14,   210,   224,   408,   412,   547,   365,   376,   559,
     160,   561,   464,   563,   609,   610,   565,   655,   371,   564,
     452,   453,   454,  1023,   763,   420,   765,     7,   767,   840,
     427,   224,    55,   401,    21,   846,   867,    19,    61,   634,
     635,    14,    15,    19,    51,    19,    32,    32,   102,    62,
      63,    64,   449,  1132,   127,    33,   137,    48,   121,    18,
      19,   124,   125,    14,    15,   165,   150,    51,    55,     0,
     122,   155,    20,    21,   165,     6,   122,   122,   165,    48,
     675,   138,   165,    61,   139,   140,   141,   132,   176,   177,
     103,   104,   105,   106,   462,   463,   182,   147,    29,    44,
      31,   122,    33,   122,   122,  1290,   165,   166,    39,   209,
    1312,   132,   165,   132,   132,   165,   204,    48,   209,   138,
     207,    90,   206,    54,   210,   182,   209,   179,   180,   210,
     182,   124,   125,   185,   180,   180,   209,   183,  1340,   202,
     203,   179,   363,  1328,   739,    76,   296,  1147,   158,   174,
     180,   206,   205,   131,   551,   376,   147,   102,   209,   180,
     138,   180,   180,   217,   185,   113,   114,    98,    99,   153,
     363,   209,   516,   121,   165,   123,   124,   125,   126,   209,
     401,   165,   403,   131,   209,   158,   211,   165,   211,    60,
     163,  1022,   165,  1272,   167,   539,   791,   179,   793,   185,
     185,   598,   209,   179,   191,   179,   165,   158,  1019,   202,
     203,   224,   163,   610,   165,   202,   167,   203,   196,   122,
     138,   179,   430,   203,   187,   150,   212,   205,  1228,   132,
     155,   211,   205,   179,   186,   175,   444,    24,   124,   125,
     637,   462,   463,    30,   179,   179,   209,   165,   616,   165,
     166,   209,   684,   205,   202,   203,   122,   652,   204,   654,
      47,   192,   206,   122,   179,   209,   132,   153,   636,   209,
     204,   211,   203,   132,   209,    62,   192,   180,   209,   165,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   138,   180,    11,   202,   203,   183,   179,
     175,   180,   147,   178,   874,   176,    22,    23,  1067,   682,
     179,   552,   553,   554,   555,  1146,   139,   558,   141,   560,
     165,   562,    24,   564,   209,  1176,   206,  1158,    30,   179,
     363,   138,   179,   204,   751,   368,   211,   206,   209,   552,
     553,   554,   555,   376,   151,   558,   179,   560,   210,   562,
     179,     4,     5,   138,     7,   596,   206,   179,   165,   206,
     787,  1019,   147,   180,    66,   179,   179,   152,   401,   179,
     403,   179,   600,   206,   179,   616,  1034,   206,   850,   179,
     165,   833,    35,   596,   206,   179,   179,    20,    21,   179,
     423,   204,   206,   621,   204,   636,    98,   179,   206,  1014,
     186,   186,   186,   179,   187,   212,   206,   976,   796,   974,
    1169,   179,   206,   206,   179,   179,   206,   187,  1033,   205,
     187,   205,   204,   153,   812,   813,   209,   815,   204,   462,
     463,   819,   660,   183,   147,   165,   138,   187,   206,   209,
     204,   206,   209,    71,   672,   686,   687,    75,   182,   901,
     902,   903,   165,   155,   156,   157,   182,  1072,    62,    63,
      64,   187,   165,   165,   153,   853,    94,    95,    96,    97,
     138,   712,   924,   925,   926,    45,   165,  1047,   205,   205,
     113,   114,   182,   210,   186,   168,   169,   187,   121,   182,
     123,   124,   125,   126,   187,    65,    20,    21,   131,   103,
     104,   105,   106,   153,   153,   205,   205,    55,   202,   182,
     147,    55,   205,    61,   187,   165,   165,    61,   155,   552,
     553,   554,   555,   995,   158,   558,  1141,   560,   165,   562,
     153,   564,   205,    20,    21,   179,   777,    55,   182,   780,
      55,   185,   165,    61,    55,   183,    61,    55,   183,   187,
      61,   792,   187,    61,   795,   796,    55,   190,   191,   192,
     165,   183,    61,   596,   777,   187,   165,   780,   209,   202,
     203,   812,   813,   606,   815,   165,   804,   747,   819,   792,
     183,   183,   795,   616,   187,   187,   168,   111,   112,   113,
     114,   115,   168,   169,   118,   119,   120,   121,   209,   123,
     124,   125,   126,   636,   183,   165,   994,   131,   187,   133,
     134,   183,   853,   165,   183,   187,   183,   210,   187,   182,
     187,    55,   185,   165,   111,   112,   113,   114,   115,    85,
      86,   118,   119,   120,   121,   668,   123,   124,   125,   126,
     168,   169,   170,   171,   131,   165,   133,   134,   207,   208,
     139,   210,   139,   140,   141,  1270,   209,   102,  1046,   168,
     155,  1276,   186,   187,   188,   189,   190,   191,   192,   210,
    1048,   139,   140,   141,   168,   169,   170,   180,   202,   203,
      10,    11,    12,   210,    20,    21,    34,   928,   210,    34,
     165,   205,   211,   210,   181,   165,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   165,   165,   204,    21,
     165,   744,   204,   182,  1329,   202,   203,   210,   182,    32,
     210,   962,   205,  1150,    41,   186,   186,   205,   205,   186,
     183,   132,   186,   974,   186,   186,    20,    21,   186,   186,
     186,   165,  1303,   186,   777,    58,    59,   780,   165,   962,
     365,   165,   165,   994,    41,   205,   205,   186,   373,   792,
     204,   165,   795,   796,   205,   205,   186,   186,   205,   158,
     209,   186,  1333,   205,   186,   390,    36,   113,   114,   812,
     813,   208,   815,   205,   399,   121,   819,   123,   124,   125,
     126,   209,   407,   209,   205,   131,   205,   205,  1186,   205,
    1227,     9,   417,   205,   209,  1046,   119,    64,   205,   209,
     123,   209,   427,   209,   209,   203,   186,   432,   211,   434,
     853,  1049,   205,    41,   206,   165,   165,   204,   443,   113,
     114,   209,   211,   209,   449,   450,   451,   121,   211,   182,
     124,   125,   126,  1267,   209,   211,   165,   131,   205,   464,
     165,   179,   188,   189,   190,   191,   192,   472,   127,   474,
     475,   476,   477,   478,   479,    13,   202,   203,   181,   180,
     158,   208,   185,   488,   187,   188,   182,   210,    11,   155,
     165,     7,   165,   187,    41,   205,   179,    20,    21,   179,
     203,   205,   165,   211,   186,   205,   209,  1125,  1325,   212,
     205,     1,   205,   205,   186,   205,   205,    65,   187,   165,
     211,   209,   187,   187,  1302,   165,   165,   165,   202,   203,
     165,   165,   165,   186,   205,   205,   204,   204,   165,   962,
      32,   165,   204,   187,   205,   205,   551,   205,    51,   205,
     205,   974,   209,   204,  1172,  1186,   187,  1188,   206,   204,
     209,   204,   661,   205,   205,    78,     1,  1185,   205,    43,
     206,   994,   204,   132,    81,  1241,   204,  1242,   204,    79,
     209,   211,   744,  1242,   864,  1188,   591,  1242,   111,   112,
     113,   114,   115,   598,  1242,   118,   119,   120,   121,   222,
     123,   124,   125,   126,  1242,   610,     1,  1183,   131,  1007,
     133,   134,   617,   618,  1204,   620,   139,   140,   141,  1266,
     625,  1207,   145,  1046,  1101,   414,   631,  1247,  1259,    51,
     423,   534,   637,  1234,   655,  1287,    -1,    -1,  1031,   423,
      -1,   646,   647,   648,   649,   650,   651,    -1,   653,    -1,
     655,    -1,    -1,    -1,    -1,    -1,  1259,    -1,   181,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      -1,  1302,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,   688,   689,   690,   691,   692,   693,   694,
     695,   696,   697,    -1,    -1,   700,   701,   702,   703,   704,
     705,    -1,   707,   708,    -1,   710,   711,   712,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,   723,   724,
     725,   726,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,    -1,   738,    -1,    -1,   741,    -1,  1356,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   751,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1186,    -1,  1188,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   794,
      -1,    -1,    -1,    -1,    -1,    -1,   801,   802,   803,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   836,   837,    -1,    -1,    -1,  1259,    -1,    -1,    -1,
     845,    -1,    -1,   848,    -1,   850,    -1,    -1,    -1,    -1,
      -1,   856,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   866,    -1,    -1,    -1,    -1,    -1,  1290,    -1,    -1,
      -1,    32,    -1,   111,   112,   113,   114,   115,    -1,  1302,
     118,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,   131,    -1,   133,   134,    58,    59,    -1,
      -1,   139,   140,   141,    -1,  1328,    -1,   145,   111,   112,
     113,   114,   115,    -1,    -1,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   941,    -1,    -1,    -1,
     945,    -1,    -1,   181,    -1,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,    -1,    -1,    -1,   119,    -1,
      -1,   122,   123,    -1,   202,   203,    -1,    -1,    -1,    -1,
      -1,   132,   210,    -1,    -1,    -1,   981,    -1,    -1,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     995,    -1,    -1,   998,    20,    21,    -1,    -1,    -1,   202,
     203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1016,  1017,  1018,    -1,  1020,    -1,    -1,    -1,   180,
     181,    -1,    -1,  1028,   185,    -1,  1031,   188,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1044,
    1045,    -1,   203,  1048,    -1,    -1,    -1,  1052,   209,    -1,
      -1,   212,  1057,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1069,    -1,  1071,    -1,    -1,    -1,
      -1,  1076,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1085,    -1,    -1,    -1,  1089,   111,   112,   113,   114,   115,
      -1,    -1,   118,   119,   120,   121,    -1,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,   111,   112,   113,   114,   115,    -1,  1132,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,   181,    -1,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,
      -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,  1194,
      -1,  1196,    -1,    -1,  1199,   185,   186,   187,   188,   189,
     190,   191,   192,    -1,    -1,    -1,    -1,    -1,  1213,    -1,
      -1,    -1,   202,   203,  1219,  1220,  1221,    -1,  1223,    -1,
      -1,  1226,    -1,    -1,    -1,  1230,  1231,  1232,  1233,    -1,
      -1,  1236,    -1,    -1,    -1,    -1,    -1,  1242,    -1,    -1,
     111,   112,   113,   114,   115,  1250,    -1,   118,   119,   120,
     121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
     131,    -1,   133,   134,    -1,    -1,    -1,  1272,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,  1281,  1282,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1294,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1304,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,  1316,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,    -1,    -1,    -1,  1330,    -1,    -1,    -1,  1334,
      -1,   202,   203,    -1,    -1,    -1,    -1,    -1,   209,    -1,
    1345,     1,  1347,  1348,    -1,    -1,     6,  1352,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    25,    26,    27,    28,    -1,
      30,    20,    21,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    -1,    42,    43,    -1,    -1,    46,    -1,    48,    49,
      50,    -1,    52,    53,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    -1,    94,    95,    96,    97,    98,    -1,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,    -1,    -1,   128,   129,
     130,    -1,   121,    -1,   123,   124,   125,   126,   138,    -1,
      -1,    -1,   131,    -1,   133,   134,   146,   147,   148,   149,
     150,    -1,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,
     190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,   188,
     189,   190,   191,   192,    -1,   205,    -1,   207,   208,   209,
     210,   211,     1,   202,   203,    -1,    -1,     6,    -1,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    25,    26,    27,    28,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    -1,    42,    43,    -1,    -1,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    -1,    56,    57,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    -1,    94,    95,    96,    97,    98,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
     129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    32,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
     189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,   207,   208,
     209,   210,   211,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,   119,
      -1,    -1,    43,   123,    -1,    46,    -1,    -1,    49,    -1,
      51,    -1,    53,    -1,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    -1,    94,    95,    96,    97,    98,    -1,   100,
      -1,   181,    -1,    -1,    -1,   185,    -1,   187,   188,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    -1,   123,   203,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   212,    -1,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,   185,    -1,    -1,   188,   189,   190,
      -1,   192,    -1,    -1,   195,   196,    -1,    -1,    -1,    -1,
      -1,    -1,   203,    -1,   205,    -1,   207,   208,    -1,   210,
      -1,   212,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,
      -1,    63,    -1,    -1,    66,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    -1,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,   185,    -1,    -1,   188,   189,   190,    -1,
     192,    -1,    -1,   195,   196,    -1,    -1,    -1,    -1,    -1,
      -1,   203,    -1,   205,    -1,   207,   208,    -1,   210,    -1,
     212,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    32,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      -1,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,   119,    -1,    -1,   122,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,    -1,    -1,
      -1,   185,   185,    -1,   188,   188,   189,   190,    -1,   192,
      -1,    -1,   195,   196,    -1,    -1,    -1,    -1,    -1,   203,
     203,    -1,   205,    -1,   207,   208,    -1,   210,   212,   212,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    66,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    -1,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,   128,   129,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,   147,   148,   149,   150,    -1,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,   190,    -1,   192,    -1,
      -1,   195,   196,    -1,    -1,    14,    15,    16,    17,    18,
      -1,   205,    -1,   207,   208,    24,   210,    26,    -1,    -1,
      32,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    58,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    -1,    94,    95,    96,    97,    98,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,   119,    -1,    -1,
      -1,   123,    -1,   122,    -1,    -1,    -1,    -1,    -1,   128,
     129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,
     149,   150,    -1,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,   185,    -1,   187,   188,    -1,    -1,   188,
     189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,
      -1,   203,    -1,    -1,    -1,    -1,   205,    -1,   207,   208,
     212,   210,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    25,    26,    27,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    66,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    -1,    94,    95,    96,    97,    98,    -1,   100,   101,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,   119,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
     185,    -1,   187,   188,    -1,    -1,   188,   189,   190,    -1,
     192,    -1,    -1,   195,   196,    -1,    -1,    -1,   203,    -1,
      -1,   203,    -1,   205,   206,   207,   208,   212,   210,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    -1,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,   149,   150,    -1,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,   190,    -1,   192,    -1,    -1,
     195,   196,    -1,    -1,    14,    15,    16,    17,    18,   204,
     205,    -1,   207,   208,    24,   210,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    -1,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,
     150,    -1,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,
     190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,    14,
      15,    16,    17,    18,    -1,   205,    -1,   207,   208,    24,
     210,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    -1,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,   149,   150,    -1,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,   190,    -1,   192,    -1,    -1,
     195,   196,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     205,    -1,   207,   208,    24,   210,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    -1,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,
     150,    -1,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,   188,   189,
     190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,    14,
      15,    16,    17,    18,    -1,   205,    21,   207,   208,    24,
     210,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    -1,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,   149,   150,    -1,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,   190,    -1,   192,    -1,    -1,
     195,   196,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     205,    -1,   207,   208,    24,   210,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    -1,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,
     150,    -1,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,
     190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,    14,
      15,    16,    17,    18,    -1,   205,   206,   207,   208,    24,
     210,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    -1,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,   149,   150,    -1,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,   190,    -1,   192,    -1,    -1,
     195,   196,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     205,   206,   207,   208,    24,   210,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    -1,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,
     150,    -1,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,   188,   189,
     190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,    14,
      15,    16,    17,    18,    -1,   205,    -1,   207,   208,    24,
     210,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    -1,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,   149,   150,    -1,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,   190,    -1,   192,    -1,    -1,
     195,   196,    -1,    -1,    14,    15,    16,    17,    18,    -1,
     205,   206,   207,   208,    24,   210,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    -1,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,   147,   148,   149,
     150,    -1,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,
     190,    -1,   192,    -1,    -1,   195,   196,    -1,    -1,    14,
      15,    16,    17,    18,    -1,   205,   206,   207,   208,    24,
     210,    26,    -1,    -1,    32,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    -1,    94,
      95,    96,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   146,   147,   148,   149,   150,    -1,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,   185,    -1,   187,
     188,    -1,    -1,   188,   189,   190,    -1,   192,    -1,    -1,
     195,   196,    -1,    -1,    -1,   203,    -1,    -1,   203,    -1,
     205,    -1,   207,   208,   212,   210,    14,    15,    16,    17,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,   189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,
      -1,    14,    15,    16,    17,    18,    -1,   205,   206,   207,
     208,    24,   210,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      -1,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,   190,    -1,   192,
      -1,    -1,   195,   196,    -1,    -1,    14,    15,    16,    17,
      18,    -1,   205,   206,   207,   208,    24,   210,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    91,    -1,    -1,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,   189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,
      -1,    14,    15,    16,    17,    18,    -1,   205,   206,   207,
     208,    24,   210,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      -1,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,   190,    -1,   192,
      -1,    -1,   195,   196,    -1,    -1,    14,    15,    16,    17,
      18,    -1,   205,    -1,   207,   208,    24,   210,    26,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,   189,   190,    -1,   192,    -1,    -1,   195,   196,    -1,
      -1,    14,    15,    16,    17,    18,    -1,   205,    -1,   207,
     208,    24,   210,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      -1,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   146,   147,   148,   149,   150,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,   190,    -1,   192,
      -1,    -1,   195,   196,    -1,    -1,    14,    15,    16,    17,
      18,    -1,   205,    -1,   207,   208,    24,   210,    26,    -1,
      -1,    32,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    58,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    91,    -1,    -1,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,   119,     9,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,   147,
     148,   149,   150,    -1,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,   185,    -1,   187,   188,    -1,    -1,
     188,   189,   190,    -1,   192,   119,    -1,   195,   196,   123,
      -1,    -1,   203,    -1,    -1,    -1,    -1,   205,    -1,   207,
     208,   212,   210,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,    20,    21,    -1,
      -1,   131,   132,   133,   134,   135,   136,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,    -1,   181,    -1,    -1,
      -1,   185,    -1,   187,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,    -1,
     180,   181,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    20,    21,    -1,    -1,   131,   132,
     133,   134,   135,   136,    -1,    -1,   139,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,    -1,    -1,    -1,   209,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
     136,    -1,    -1,   139,   140,   141,   142,   143,   144,   145,
      -1,    -1,   111,   112,   113,   114,   115,    -1,    -1,   118,
     119,   120,   121,    -1,   123,   124,   125,   126,    20,    21,
      -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,   180,   181,   145,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,   202,   203,    -1,    -1,
      -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,    -1,
     209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,   111,
     112,   113,   114,   115,    -1,    -1,   118,   119,   120,   121,
      -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,
      -1,   133,   134,    -1,    58,    59,    -1,   139,   140,   141,
      -1,    -1,    -1,   145,   111,   112,   113,   114,   115,    20,
      21,   118,   119,   120,   121,    -1,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,   181,
      -1,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,
     202,   203,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,
     111,   112,   113,   114,   115,    20,    21,   118,   119,   120,
     121,    -1,   123,   124,   125,   126,    -1,   181,    -1,    -1,
     131,   185,   133,   134,   188,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,   203,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   212,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,   206,   111,   112,   113,   114,
     115,    -1,    -1,   118,   119,   120,   121,    -1,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
     145,   111,   112,   113,   114,   115,    20,    21,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,   181,    -1,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,   206,   111,   112,   113,
     114,   115,    20,    21,   118,   119,   120,   121,    -1,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   206,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,   181,    -1,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,   206,    -1,
      62,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   111,   112,
     113,   114,   115,    -1,    -1,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,
     133,   134,    20,    21,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,   181,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,   111,   112,
     113,   114,   115,    20,    21,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,   181,    -1,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,    -1,   111,   112,   113,   114,   115,    20,
      21,   118,   119,   120,   121,    -1,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,    -1,   181,    -1,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,   204,    32,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,   119,   120,
     121,    -1,   123,   124,   125,   126,    20,    21,    -1,    -1,
     131,    -1,   133,   134,    58,    59,   153,    -1,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,
      -1,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,    20,    21,   131,   132,   133,
     134,   135,   136,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,    37,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,   185,    -1,   187,   188,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,
      -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   212,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
     135,   136,    -1,    -1,   139,   140,   141,   142,   143,   144,
     145,   111,   112,   113,   114,   115,    20,    21,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,   131,    -1,   133,   134,    -1,    -1,   137,    -1,   139,
     140,   141,    -1,    -1,    -1,   145,   181,    -1,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,    -1,    -1,
     195,   196,    -1,    -1,    -1,    -1,    -1,   202,   203,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,    -1,    -1,    -1,   111,   112,   113,
     114,   115,    20,    21,   118,   119,   120,   121,    -1,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,   133,
     134,    -1,    -1,   137,    -1,   139,   140,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,    -1,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,   145,   111,   112,
     113,   114,   115,    20,    21,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,
     133,   134,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,   145,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,    -1,    -1,    -1,   111,   112,   113,   114,   115,    20,
      21,   118,   119,   120,   121,    -1,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,   145,    -1,
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
      -1,    -1,    -1,   139,    -1,   141,    -1,    -1,    -1,    -1,
     181,    -1,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,   202,   203,    -1,    -1,
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,   119,
     120,   121,    -1,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,   131,    -1,   133,   134,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,   131,    -1,   133,   134,    58,    59,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,    32,   111,   112,   113,   114,   115,    -1,
      -1,   118,   202,   203,   121,    -1,   123,   124,   125,   126,
      32,    -1,    -1,    -1,   131,    -1,   133,   134,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,   184,   185,   186,   187,
     188,   189,   190,   191,   192,    -1,    58,    59,   119,    -1,
      -1,    -1,   123,    -1,   202,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
     187,   188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,   123,   202,   203,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,
     181,   123,    -1,    -1,   185,    -1,   187,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,   181,    -1,    -1,    -1,   185,    -1,   187,   188,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,   181,
      -1,    -1,    -1,   185,   203,   187,   188,    -1,    -1,    -1,
      -1,    -1,    -1,   212,    47,    -1,    -1,    -1,    -1,    -1,
      -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
     212,    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    -1,    18,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,   165,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    -1,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,   165,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    -1,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
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
      76,    98,    99,   192,   203,   209,   215,   218,   224,   226,
     227,   232,   260,   264,   290,   363,   370,   374,   384,   424,
     429,   434,    18,    19,   165,   253,   254,   255,   158,   233,
     234,   165,   166,   192,   228,   229,   230,   210,   371,   165,
     207,   217,    55,    61,   366,   366,   366,   138,   165,   277,
      33,    61,   131,   196,   205,   256,   257,   258,   259,   277,
     209,     4,     5,     7,    35,   382,    60,   361,   180,   179,
     182,   179,   229,    21,    55,   191,   202,   231,   366,   367,
     369,   367,   361,   435,   425,   430,   165,   138,   225,   258,
     258,   258,   205,   139,   140,   141,   179,   204,    55,    61,
     265,   267,    55,    61,   372,    55,    61,   383,    55,    61,
     362,    14,    15,   158,   163,   165,   167,   205,   220,   254,
     158,   234,   165,   228,   228,   165,   209,   211,   367,   209,
      55,    61,   216,   165,   165,   165,   165,   168,   223,   206,
     255,   258,   258,   258,   258,   268,   165,   373,   385,   210,
     364,   168,   169,   219,    14,    15,   158,   163,   165,   220,
     251,   252,   231,   368,   209,   436,   426,   431,   168,   206,
      34,    67,    69,    71,    72,    73,    74,    75,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    89,
      90,    91,    94,    95,    96,    97,   113,   114,   165,   263,
     266,   182,   210,   102,   380,   381,   359,   155,   335,   168,
     169,   170,   179,   206,   180,   210,   210,   210,    20,    21,
      37,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   123,
     124,   125,   132,   133,   134,   135,   136,   139,   140,   141,
     142,   143,   144,   145,   181,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   195,   196,   202,   203,    34,
      34,   205,   261,   210,   269,    71,    75,    94,    95,    96,
      97,   389,   375,   165,   386,   211,   360,   255,   147,   165,
     357,   358,   251,    24,    30,    47,    62,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     151,   277,   388,   390,   391,   394,   398,   423,   437,   427,
     432,   165,   165,   165,   204,    21,   165,   204,   150,   206,
     335,   345,   346,   182,   262,   271,   210,   365,   182,   379,
     210,   335,   204,   205,    41,   179,   182,   185,   356,   186,
     186,   186,   186,   205,   186,   186,   186,   186,   186,   186,
     205,    32,    58,    59,   119,   123,   181,   185,   188,   203,
     212,   183,   393,   349,   352,   165,   132,   205,    48,   147,
     165,   354,   387,   206,   209,   423,     1,     8,     9,    10,
      12,    14,    15,    16,    17,    18,    24,    25,    26,    27,
      28,    30,    37,    38,    40,    42,    43,    46,    49,    50,
      52,    53,    56,    57,    63,    66,    76,    98,   100,   101,
     113,   114,   128,   129,   130,   146,   147,   148,   149,   150,
     152,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   166,   167,   188,   189,   190,   195,   196,   205,
     207,   208,   209,   211,   222,   224,   235,   236,   239,   242,
     243,   245,   246,   247,   248,   249,   269,   270,   272,   273,
     276,   277,   278,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   294,   295,   302,   305,   308,   312,   313,   314,
     315,   316,   317,   319,   324,   327,   334,   388,   439,   448,
     451,   453,   460,   462,   377,   165,   209,   376,   277,   341,
     358,   204,    63,   166,   327,   165,   165,   398,   122,   132,
     180,   355,   401,   403,   405,   399,   165,   395,   407,   409,
     411,   413,   415,   417,   419,   421,   327,    32,   185,    32,
     185,   203,   212,   204,   327,   212,   398,   165,   209,   438,
     165,   209,   347,   423,   428,   165,   209,   350,   433,   327,
     354,   205,    41,   122,   179,   180,   182,   185,   353,   345,
     327,   387,   186,   279,   390,   439,   205,   186,   186,   205,
     122,   276,   306,   314,   327,   269,   186,   205,    59,   327,
     205,   327,   165,   186,   186,   205,   209,   186,   158,    56,
     327,   205,   269,   186,   205,   205,   186,   122,   276,   327,
     327,   327,   208,   312,   312,   312,   205,   205,   205,   205,
     205,   205,    12,   398,    12,   398,   327,   450,   458,   186,
     327,   221,   327,   327,   327,   327,   327,   327,   208,    36,
     209,   209,   327,     9,   165,   205,   209,   209,   209,   209,
     209,    64,   291,   260,   127,   209,    20,    21,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   131,   132,   133,   134,   135,   136,   139,   140,
     141,   142,   143,   144,   145,   180,   181,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   202,   203,   205,
     365,   180,   209,   211,   203,   211,   260,   342,   206,    41,
     209,   355,   276,   327,   423,   397,   423,   423,   423,   206,
     165,   392,   423,   261,   423,   261,   423,   261,   347,   348,
     350,   351,   206,   204,   204,   209,   211,   182,   209,   211,
     182,   209,   211,   206,   327,   165,   165,   423,   276,   327,
     269,    19,   280,   205,   277,   303,   454,   306,   327,   276,
     306,   179,   122,   180,    13,   398,   450,   459,   327,   327,
     269,   180,   296,   298,   327,   300,   182,   158,   327,   456,
     306,   306,   398,   276,   327,   208,   327,   327,   327,   327,
     327,   327,   387,    51,   153,   165,   188,   205,   327,   440,
     441,   442,   449,   452,   387,   205,   441,   452,   137,   174,
     209,   211,   443,   274,   269,   170,   171,   219,   206,   147,
     152,   186,   277,   318,   210,   269,   244,   203,   206,   306,
     155,   293,   358,   155,   311,   312,    18,   153,   165,   388,
      18,   153,   165,   388,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     165,   128,   129,   130,   327,   327,   153,   165,   327,   327,
     327,   388,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   128,   129,   130,   327,    21,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     124,   125,   153,   165,   202,   203,   325,   327,   206,   306,
     378,   327,   259,     7,   335,   340,   165,   276,   327,   209,
     187,   187,   209,   187,   187,   187,   209,   187,   262,   187,
     262,   187,   262,   187,   209,   187,   209,   423,   423,   206,
      41,   353,   306,   423,   206,   306,   186,   327,   423,   398,
     206,   327,   269,   187,   209,   179,   206,   206,   250,   398,
     398,   206,   398,   206,   398,   206,   206,   187,   206,   206,
     206,   206,   206,   206,    19,   312,   205,   132,   353,   206,
     137,   179,   209,   442,   176,   177,   204,   446,   179,   176,
     204,   209,   445,    19,   206,   442,   175,   211,   444,   327,
     450,   211,   398,   327,   205,   205,   320,   322,   271,    11,
      22,    23,   237,   238,   327,   165,   440,   179,   206,   165,
     292,   122,   132,   180,   185,   309,   310,   261,   186,   205,
     186,   205,   205,   312,   312,   312,   205,   204,   312,   312,
     312,    18,   153,   165,   388,   182,   153,   165,   327,   205,
     153,   165,   327,     1,   204,   206,   211,   209,   204,    55,
      61,   338,    65,   339,   209,   209,   402,   404,   423,   406,
     400,   396,   165,   408,   187,   412,   187,   416,   187,   420,
     347,   422,   350,   165,   327,   209,   187,   206,   165,   206,
     187,   187,   205,   398,   450,   327,   187,   187,   187,   187,
     311,   205,   306,   327,   327,   327,   441,   442,   327,   153,
     165,   440,   446,   204,   327,   204,   449,   306,   441,   175,
     178,   211,   447,   204,   187,   172,   327,   327,   398,   261,
     211,   269,   327,    11,   240,   204,   203,   179,   204,   165,
     165,   165,   165,   179,   204,   262,   328,   327,   330,   327,
     206,   306,   327,   186,   205,   327,   205,   204,   327,   205,
     204,   326,   209,    45,   339,    44,   102,   336,   410,   414,
     418,   269,   281,   187,   209,   304,   455,   459,   187,   297,
     299,   301,   457,   205,   206,   306,   209,   206,   442,   446,
     205,   132,   353,   209,   442,   204,   275,   206,   206,   187,
     262,   269,   241,   209,   206,   440,   165,   204,   309,   204,
     137,   269,   307,   398,   206,   423,   206,   206,   206,   332,
     327,   327,   206,   327,    32,   337,   336,   338,   327,   165,
     205,   206,   205,   327,   327,   327,   205,   206,   327,   206,
     327,   132,   353,   446,   327,   327,   327,   327,   447,   327,
     321,   187,   238,    25,   101,   242,   283,   284,   285,   287,
     327,   204,   182,   356,   122,   327,   187,   187,   423,   206,
     206,   206,   343,   337,   354,   206,   187,   306,   459,   306,
     206,   209,   461,   327,   327,   206,   461,   318,   323,   122,
     122,   327,   206,   398,   355,   327,   329,   331,   187,   266,
     344,   327,   206,   206,   206,    51,   204,   132,   353,   211,
     318,   327,   355,   327,   333,   209,   266,   206,   327,   204,
     327,   327,   204,   327,   209,   269,   209
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   213,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   215,   216,   216,
     216,   217,   217,   218,   219,   219,   219,   219,   220,   221,
     221,   221,   222,   223,   223,   225,   224,   226,   227,   228,
     228,   229,   229,   229,   229,   230,   230,   231,   231,   232,
     233,   233,   234,   234,   235,   236,   236,   237,   237,   238,
     238,   238,   239,   239,   240,   241,   240,   242,   242,   242,
     242,   242,   243,   244,   243,   245,   246,   247,   248,   250,
     249,   251,   251,   251,   251,   251,   251,   252,   252,   253,
     253,   253,   254,   254,   254,   254,   254,   254,   254,   254,
     255,   255,   256,   256,   256,   257,   257,   258,   258,   258,
     258,   258,   258,   258,   259,   259,   260,   260,   261,   261,
     261,   262,   262,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   264,   265,
     265,   265,   266,   268,   267,   269,   269,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   271,   271,
     271,   272,   274,   275,   273,   276,   276,   276,   276,   277,
     277,   277,   278,   278,   280,   281,   279,   279,   282,   282,
     282,   282,   283,   284,   285,   285,   285,   286,   286,   286,
     287,   287,   288,   288,   288,   289,   290,   290,   291,   291,
     292,   292,   293,   293,   294,   294,   296,   297,   295,   298,
     299,   295,   300,   301,   295,   303,   304,   302,   305,   305,
     305,   306,   306,   307,   307,   307,   308,   308,   308,   309,
     309,   309,   309,   310,   310,   311,   311,   312,   312,   313,
     313,   313,   313,   313,   313,   313,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   315,   315,   315,   315,   315,
     315,   316,   316,   317,   317,   318,   318,   319,   320,   321,
     319,   322,   323,   319,   324,   324,   325,   326,   324,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   328,   329,
     327,   327,   327,   327,   330,   331,   327,   327,   327,   332,
     333,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   335,   335,   336,   336,   336,   337,   337,   338,
     338,   338,   339,   339,   340,   341,   342,   341,   343,   341,
     344,   341,   341,   345,   345,   345,   346,   346,   347,   347,
     348,   348,   349,   349,   349,   350,   351,   351,   352,   352,
     352,   353,   353,   354,   354,   354,   354,   354,   354,   355,
     355,   355,   356,   356,   357,   357,   357,   357,   357,   358,
     358,   358,   358,   358,   359,   360,   359,   361,   361,   362,
     362,   362,   363,   364,   363,   365,   365,   365,   365,   366,
     366,   366,   368,   367,   369,   369,   370,   371,   370,   372,
     372,   372,   373,   375,   376,   374,   377,   378,   374,   379,
     379,   380,   380,   381,   382,   382,   383,   383,   383,   385,
     386,   384,   387,   387,   387,   387,   387,   388,   388,   388,
     388,   388,   388,   388,   388,   388,   388,   388,   388,   388,
     388,   388,   388,   388,   388,   388,   388,   388,   388,   388,
     388,   388,   388,   388,   389,   389,   389,   389,   389,   389,
     390,   391,   391,   391,   392,   392,   393,   393,   393,   395,
     396,   394,   397,   397,   398,   398,   398,   398,   398,   398,
     398,   398,   398,   398,   398,   398,   398,   398,   398,   398,
     398,   398,   399,   400,   398,   398,   401,   402,   398,   403,
     404,   398,   405,   406,   398,   398,   407,   408,   398,   409,
     410,   398,   398,   411,   412,   398,   413,   414,   398,   398,
     415,   416,   398,   417,   418,   398,   419,   420,   398,   421,
     422,   398,   423,   423,   423,   425,   426,   427,   428,   424,
     430,   431,   432,   433,   429,   435,   436,   437,   438,   434,
     439,   439,   439,   439,   439,   440,   440,   440,   440,   440,
     440,   440,   440,   441,   441,   442,   442,   443,   443,   444,
     444,   445,   445,   446,   446,   446,   447,   447,   447,   448,
     448,   448,   448,   448,   448,   449,   449,   449,   450,   450,
     451,   452,   452,   453,   453,   453,   454,   455,   453,   453,
     456,   457,   453,   458,   458,   459,   459,   460,   460,   460,
     460,   461,   461,   462,   462
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       1,     1,     1,     4,     1,     1,     2,     2,     3,     0,
       2,     4,     3,     1,     2,     0,     4,     2,     2,     1,
       1,     1,     2,     3,     3,     2,     4,     0,     1,     2,
       1,     3,     1,     3,     3,     3,     2,     1,     1,     0,
       2,     4,     1,     1,     0,     0,     3,     1,     1,     1,
       1,     1,     4,     0,     6,     6,     2,     3,     3,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     1,     5,
       1,     3,     1,     1,     1,     1,     4,     1,     2,     3,
       3,     3,     3,     2,     1,     3,     0,     3,     0,     2,
       3,     0,     2,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     3,     3,     2,
       2,     3,     4,     3,     2,     2,     2,     2,     2,     3,
       3,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     1,     3,     0,     4,     3,     7,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     2,     2,     2,     0,     2,
       2,     3,     0,     0,     7,     3,     2,     2,     2,     1,
       3,     2,     2,     3,     0,     0,     5,     1,     2,     4,
       5,     2,     1,     1,     1,     2,     3,     2,     2,     3,
       2,     3,     2,     2,     3,     4,     1,     1,     1,     0,
       1,     3,     9,     8,     3,     3,     0,     0,     7,     0,
       0,     7,     0,     0,     7,     0,     0,     6,     5,     8,
      10,     1,     3,     1,     2,     3,     1,     1,     2,     2,
       2,     2,     2,     1,     3,     0,     4,     1,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     4,     4,     4,     4,
       4,     6,     8,     5,     6,     1,     4,     3,     0,     0,
       8,     0,     0,     9,     3,     4,     0,     0,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
       4,     5,     4,     5,     3,     4,     1,     3,     4,     3,
       4,     2,     4,     4,     7,     8,     3,     5,     0,     0,
       8,     3,     3,     3,     0,     0,     8,     3,     4,     0,
       0,     9,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     2,     4,     1,     4,     4,     4,     4,
       4,     1,     6,     7,     6,     6,     7,     7,     6,     7,
       6,     6,     0,     4,     0,     1,     1,     0,     1,     0,
       1,     1,     0,     1,     5,     0,     0,     4,     0,     9,
       0,    10,     5,     2,     3,     4,     1,     3,     1,     3,
       1,     3,     0,     2,     3,     3,     1,     3,     0,     2,
       3,     1,     1,     1,     2,     3,     5,     3,     3,     1,
       1,     1,     0,     1,     1,     4,     3,     3,     5,     4,
       6,     5,     5,     4,     0,     0,     4,     0,     1,     0,
       1,     1,     6,     0,     6,     0,     2,     3,     5,     0,
       1,     1,     0,     5,     2,     3,     4,     0,     4,     0,
       1,     1,     1,     0,     0,     9,     0,     0,    11,     0,
       2,     0,     1,     3,     1,     1,     0,     1,     1,     0,
       0,     9,     1,     4,     3,     3,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     1,     3,     0,     2,     3,     0,
       0,     6,     1,     3,     1,     1,     1,     1,     4,     3,
       4,     2,     2,     3,     2,     3,     2,     2,     3,     3,
       3,     2,     0,     0,     6,     2,     0,     0,     6,     0,
       0,     6,     0,     0,     6,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     0,     0,     6,     0,
       0,     6,     1,     3,     3,     0,     0,     0,     0,    10,
       0,     0,     0,     0,    10,     0,     0,     0,     0,    10,
       1,     1,     1,     1,     1,     3,     3,     5,     5,     6,
       6,     8,     8,     1,     3,     0,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     2,     1,     1,     5,
       4,     6,     7,     5,     7,     1,     3,     3,     3,     1,
       4,     1,     3,     4,     4,     4,     0,     0,     9,     4,
       0,     0,     9,     1,     3,     1,     3,     3,     4,     7,
       9,     0,     3,    10,    10
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

    case YYSYMBOL_tuple_alias_type_list: /* tuple_alias_type_list  */
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

  case 13: /* program: program tuple_alias_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 14: /* program: program bitfield_alias_declaration  */
                                                { yyextra->das_has_type_declarations = true; }
    break;

  case 17: /* top_level_reader_macro: expr_reader ';'  */
                             {
        delete (yyvsp[-1].pExpression);    // we do nothing, we don't even attemp to 'visit'
    }
    break;

  case 18: /* optional_public_or_private_module: %empty  */
                        { (yyval.b) = yyextra->g_Program->policies.default_module_public; }
    break;

  case 19: /* optional_public_or_private_module: "public"  */
                        { (yyval.b) = true; }
    break;

  case 20: /* optional_public_or_private_module: "private"  */
                        { (yyval.b) = false; }
    break;

  case 21: /* module_name: '$'  */
                    { (yyval.s) = new string("$"); }
    break;

  case 22: /* module_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 23: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module  */
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

  case 24: /* character_sequence: STRING_CHARACTER  */
                                                            { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 25: /* character_sequence: STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 26: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 27: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 28: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 29: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 30: /* string_builder_body: string_builder_body character_sequence  */
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

  case 31: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 32: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 33: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 34: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 35: /* $@1: %empty  */
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

  case 36: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
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

  case 37: /* options_declaration: "options" annotation_argument_list  */
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

  case 39: /* keyword_or_name: "name"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 40: /* keyword_or_name: "keyword"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 41: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 42: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 43: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 44: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 45: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 46: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 47: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 48: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 52: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 53: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 54: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 55: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 56: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 57: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 58: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 59: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 60: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 61: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 62: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 63: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 64: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 65: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 66: /* expression_else_one_liner: "else" $@2 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 67: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 69: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 70: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 71: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 72: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 73: /* $@3: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 74: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner ';'  */
                                                                                                                                                   {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 75: /* expression_for_loop: "for" variable_name_with_pos_list "in" expr_list ';' expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 76: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 77: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 78: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 79: /* $@4: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 80: /* expression_with_alias: "assume" "name" '=' $@4 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 81: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 82: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 83: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 84: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 85: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 86: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 87: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 88: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 89: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 90: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 91: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 92: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 93: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 94: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 95: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 96: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 97: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 98: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 99: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 100: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 101: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 102: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 103: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 104: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 105: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
    break;

  case 106: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 107: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 108: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 109: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 110: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 111: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 112: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 113: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 114: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 115: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 116: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 117: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 118: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 119: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 120: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 121: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 122: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 123: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 124: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 125: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 126: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 127: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 128: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 129: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 130: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 131: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 132: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 133: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 134: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 135: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 136: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 137: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 138: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 139: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 140: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 141: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 142: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 143: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 144: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 145: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 146: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 147: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 148: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 149: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 150: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 151: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 152: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 153: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 154: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 155: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 156: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 157: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 158: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 159: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 160: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 161: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 162: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 163: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 164: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 165: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 166: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 167: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 168: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 169: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 170: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 171: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 172: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 173: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 174: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 175: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 176: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 177: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 178: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 179: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 180: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 181: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 182: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 183: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 184: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 185: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 186: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 187: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 188: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 189: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 190: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 191: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 192: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 193: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 194: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 195: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 196: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 197: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 198: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 199: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 200: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 201: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 202: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 203: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 204: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 205: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 206: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 207: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 208: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 209: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 210: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 211: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 212: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 213: /* $@5: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 214: /* function_declaration: optional_public_or_private_function $@5 function_declaration_header expression_block  */
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

  case 215: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 216: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 217: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 218: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 219: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 220: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 221: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 222: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 223: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 226: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 227: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 230: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 231: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 235: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 236: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 237: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 238: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 239: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 240: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 241: /* expr_keyword: "keyword" expr expression_block  */
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

  case 242: /* $@6: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 243: /* $@7: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 244: /* expression_keyword: "keyword" '<' $@6 type_declaration_no_options '>' $@7 expr  */
                                                                                                                                               {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),*(yyvsp[-6].s));
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCall->arguments.push_back(ExpressionPtr(td));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 245: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 246: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 247: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 248: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 249: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 250: /* name_in_namespace: "name" "::" "name"  */
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

  case 251: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 252: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 253: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 254: /* $@8: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 255: /* $@9: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 256: /* new_type_declaration: '<' $@8 type_declaration '>' $@9  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 257: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 258: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 259: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 260: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 261: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 262: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 263: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 264: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 265: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 266: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 267: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 268: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 269: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 270: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 271: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 272: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 273: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 274: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 275: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 276: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 277: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 278: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 279: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 280: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 281: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 282: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 283: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr ';'  */
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

  case 284: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 285: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 286: /* $@10: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 287: /* $@11: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 288: /* expr_cast: "cast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 289: /* $@12: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 290: /* $@13: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 291: /* expr_cast: "upcast" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 292: /* $@14: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 293: /* $@15: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 294: /* expr_cast: "reinterpret" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 295: /* $@16: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 296: /* $@17: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 297: /* expr_type_decl: "type" '<' $@16 type_declaration '>' $@17  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 298: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 299: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 300: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 301: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 302: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 303: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 304: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 305: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 306: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 307: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 308: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 309: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 310: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 311: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 312: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 313: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 314: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 315: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 316: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 317: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 318: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 319: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 320: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 321: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 322: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 323: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 324: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 325: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 326: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 327: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 335: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 343: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 344: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 345: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr_assign_pipe: expr '=' "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr_assign_pipe: expr "<-" "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 352: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 353: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 354: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 355: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 356: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 357: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 358: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 359: /* $@19: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 360: /* func_addr_expr: '@' '@' '<' $@18 type_declaration_no_options '>' $@19 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 361: /* $@20: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 362: /* $@21: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 363: /* func_addr_expr: '@' '@' '<' $@20 optional_function_argument_list optional_function_type '>' $@21 func_addr_name  */
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

  case 364: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 365: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 366: /* $@22: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 367: /* $@23: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 368: /* expr_field: expr '.' $@22 error $@23  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 369: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 370: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 371: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 372: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 373: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 374: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 375: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 376: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 377: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 378: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 379: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 402: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 404: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 405: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 406: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 407: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 408: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 409: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 410: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 411: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 412: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 413: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 414: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 415: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 416: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 417: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 418: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 419: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 420: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 421: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 422: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 423: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 424: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 425: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 426: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 427: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 428: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 429: /* $@25: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 430: /* expr: expr "is" "type" '<' $@24 type_declaration_no_options '>' $@25  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 431: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 432: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 433: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 434: /* $@26: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 435: /* $@27: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 436: /* expr: expr "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 437: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 438: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 439: /* $@28: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 440: /* $@29: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 441: /* expr: expr '?' "as" "type" '<' $@28 type_declaration '>' $@29  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 442: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 443: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 444: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 445: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 446: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 447: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 448: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 449: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 450: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 451: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 452: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 453: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 454: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 455: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 456: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 457: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 458: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 459: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 460: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 461: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 462: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 463: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 464: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 465: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 466: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 467: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 468: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 469: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 470: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 471: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 472: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 473: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 474: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 475: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 476: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 477: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 478: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 479: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 480: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 481: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 482: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 483: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 484: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 485: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 486: /* $@30: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 487: /* struct_variable_declaration_list: struct_variable_declaration_list $@30 structure_variable_declaration ';'  */
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

  case 488: /* $@31: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 489: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@31 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 490: /* $@32: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 491: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@32 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 492: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 493: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 494: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 495: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 496: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 497: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 498: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 499: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 500: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 501: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 502: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 503: /* tuple_alias_type_list: tuple_alias_type_list ';'  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 504: /* tuple_alias_type_list: tuple_alias_type_list tuple_type ';'  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
        /*
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,@decl);
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *($decl->pNameList) ) {
                    crd->afterVariantEntry(nl.name.c_str(), nl.at);
                }
            }
        }
        */
    }
    break;

  case 505: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 506: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 507: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 508: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 509: /* variant_alias_type_list: variant_alias_type_list ';'  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 510: /* variant_alias_type_list: variant_alias_type_list variant_type ';'  */
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

  case 511: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 512: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 513: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 514: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 515: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 516: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 517: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 518: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 519: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 520: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 521: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 522: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 523: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 524: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 525: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 526: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 527: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 528: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 529: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 530: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 531: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 532: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 533: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 534: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 535: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 536: /* global_variable_declaration_list: global_variable_declaration_list $@33 optional_field_annotation let_variable_declaration  */
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

  case 537: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 538: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 539: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 540: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 541: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 542: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 543: /* $@34: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 544: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@34 optional_field_annotation let_variable_declaration  */
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

  case 545: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 546: /* enum_list: enum_list ';'  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 547: /* enum_list: enum_list "name" ';'  */
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

  case 548: /* enum_list: enum_list "name" '=' expr ';'  */
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

  case 549: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 550: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 551: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 552: /* $@35: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 553: /* single_alias: optional_public_or_private_alias "name" $@35 '=' type_declaration  */
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

  case 557: /* $@36: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 559: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 560: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 561: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 562: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 563: /* $@37: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 564: /* $@38: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 565: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' $@37 enum_list $@38 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 566: /* $@39: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 567: /* $@40: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 568: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' $@39 enum_list $@40 '}'  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 569: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 570: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 571: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 572: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 573: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 574: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 575: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 576: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 577: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 578: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 579: /* $@41: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 580: /* $@42: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 581: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@41 structure_name $@42 '{' struct_variable_declaration_list '}'  */
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

  case 582: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 583: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 584: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 585: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 586: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 587: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 588: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 589: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 590: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 591: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 592: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 593: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 594: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 595: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 596: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 597: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 598: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 599: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 600: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 601: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 602: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 603: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 604: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 605: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 606: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 607: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 608: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 609: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 610: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 611: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 612: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 613: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 614: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 615: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 616: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 617: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 618: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 619: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 620: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 621: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 622: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 623: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 624: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 625: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 626: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 627: /* bitfield_alias_bits: bitfield_alias_bits ';'  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 628: /* bitfield_alias_bits: bitfield_alias_bits "name" ';'  */
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

  case 629: /* $@43: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 630: /* $@44: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 631: /* bitfield_type_declaration: "bitfield" '<' $@43 bitfield_bits '>' $@44  */
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

  case 632: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 633: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 634: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 635: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 636: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 637: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 638: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
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

  case 639: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 640: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 641: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 642: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 643: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 644: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 645: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 646: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 647: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 648: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 649: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 650: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 651: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 652: /* $@45: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 653: /* $@46: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 654: /* type_declaration_no_options: "smart_ptr" '<' $@45 type_declaration '>' $@46  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 655: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 656: /* $@47: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 657: /* $@48: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 658: /* type_declaration_no_options: "array" '<' $@47 type_declaration '>' $@48  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 659: /* $@49: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 660: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 661: /* type_declaration_no_options: "table" '<' $@49 table_type_pair '>' $@50  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 662: /* $@51: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 663: /* $@52: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 664: /* type_declaration_no_options: "iterator" '<' $@51 type_declaration '>' $@52  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 665: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 666: /* $@53: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 667: /* $@54: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 668: /* type_declaration_no_options: "block" '<' $@53 type_declaration '>' $@54  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 669: /* $@55: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 670: /* $@56: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 671: /* type_declaration_no_options: "block" '<' $@55 optional_function_argument_list optional_function_type '>' $@56  */
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

  case 672: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 673: /* $@57: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 674: /* $@58: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 675: /* type_declaration_no_options: "function" '<' $@57 type_declaration '>' $@58  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 676: /* $@59: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 677: /* $@60: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 678: /* type_declaration_no_options: "function" '<' $@59 optional_function_argument_list optional_function_type '>' $@60  */
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

  case 679: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 680: /* $@61: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 681: /* $@62: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 682: /* type_declaration_no_options: "lambda" '<' $@61 type_declaration '>' $@62  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 683: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 684: /* $@64: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 685: /* type_declaration_no_options: "lambda" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 686: /* $@65: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 687: /* $@66: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 688: /* type_declaration_no_options: "tuple" '<' $@65 tuple_type_list '>' $@66  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 689: /* $@67: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 690: /* $@68: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 691: /* type_declaration_no_options: "variant" '<' $@67 variant_type_list '>' $@68  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 692: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 693: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 694: /* type_declaration: type_declaration '|' '#'  */
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

  case 695: /* $@69: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 696: /* $@70: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 697: /* $@71: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 698: /* $@72: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 699: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@69 "name" $@70 '{' $@71 tuple_alias_type_list $@72 '}'  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tTuple);
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
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTuple((yyvsp[-6].s)->c_str(),atvname);
        }
        delete (yyvsp[-6].s);
    }
    break;

  case 700: /* $@73: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 701: /* $@74: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 702: /* $@75: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 703: /* $@76: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 704: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@73 "name" $@74 '{' $@75 variant_alias_type_list $@76 '}'  */
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

  case 705: /* $@77: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 706: /* $@78: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 707: /* $@79: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 708: /* $@80: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 709: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@77 "name" $@78 '{' $@79 bitfield_alias_bits $@80 '}'  */
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

  case 710: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 711: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 712: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 713: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 714: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 715: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 716: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 717: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 718: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 719: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 720: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 721: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 722: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 723: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 724: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 725: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 726: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 739: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 740: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 741: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 742: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 743: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 744: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 745: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 746: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 747: /* make_tuple: make_tuple ',' expr  */
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

  case 748: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 749: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 750: /* make_tuple_call: "tuple" '(' expr_list ')'  */
                                                     {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-3])));
        mkt->values = sequenceToList((yyvsp[-1].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 751: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 752: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 753: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 754: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 755: /* make_dim_decl: "array" '(' expr_list ')'  */
                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 756: /* $@81: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 757: /* $@82: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 758: /* make_dim_decl: "array" '<' $@81 type_declaration_no_options '>' $@82 '(' expr_list ')'  */
                                                                                                                                                               {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 759: /* make_dim_decl: "fixed_array" '(' expr_list ')'  */
                                                          {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 760: /* $@83: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 761: /* $@84: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 762: /* make_dim_decl: "fixed_array" '<' $@83 type_declaration_no_options '>' $@84 '(' expr_list ')'  */
                                                                                                                                                                     {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-8])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 763: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 764: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 765: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 766: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 767: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 768: /* make_table_decl: "table" '(' expr_map_tuple_list ')'  */
                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 769: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list ')'  */
                                                                                                        {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-6])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 770: /* make_table_decl: "table" '<' type_declaration_no_options ';' type_declaration_no_options '>' '(' expr_map_tuple_list ')'  */
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

  case 771: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 772: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 773: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 774: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
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


