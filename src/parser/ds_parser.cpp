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
  YYSYMBOL_DAS_TYPEDECL = 40,              /* "typedecl"  */
  YYSYMBOL_DAS_WITH = 41,                  /* "with"  */
  YYSYMBOL_DAS_AKA = 42,                   /* "aka"  */
  YYSYMBOL_DAS_ASSUME = 43,                /* "assume"  */
  YYSYMBOL_DAS_CAST = 44,                  /* "cast"  */
  YYSYMBOL_DAS_OVERRIDE = 45,              /* "override"  */
  YYSYMBOL_DAS_ABSTRACT = 46,              /* "abstract"  */
  YYSYMBOL_DAS_UPCAST = 47,                /* "upcast"  */
  YYSYMBOL_DAS_ITERATOR = 48,              /* "iterator"  */
  YYSYMBOL_DAS_VAR = 49,                   /* "var"  */
  YYSYMBOL_DAS_ADDR = 50,                  /* "addr"  */
  YYSYMBOL_DAS_CONTINUE = 51,              /* "continue"  */
  YYSYMBOL_DAS_WHERE = 52,                 /* "where"  */
  YYSYMBOL_DAS_PASS = 53,                  /* "pass"  */
  YYSYMBOL_DAS_REINTERPRET = 54,           /* "reinterpret"  */
  YYSYMBOL_DAS_MODULE = 55,                /* "module"  */
  YYSYMBOL_DAS_PUBLIC = 56,                /* "public"  */
  YYSYMBOL_DAS_LABEL = 57,                 /* "label"  */
  YYSYMBOL_DAS_GOTO = 58,                  /* "goto"  */
  YYSYMBOL_DAS_IMPLICIT = 59,              /* "implicit"  */
  YYSYMBOL_DAS_EXPLICIT = 60,              /* "explicit"  */
  YYSYMBOL_DAS_SHARED = 61,                /* "shared"  */
  YYSYMBOL_DAS_PRIVATE = 62,               /* "private"  */
  YYSYMBOL_DAS_SMART_PTR = 63,             /* "smart_ptr"  */
  YYSYMBOL_DAS_UNSAFE = 64,                /* "unsafe"  */
  YYSYMBOL_DAS_INSCOPE = 65,               /* "inscope"  */
  YYSYMBOL_DAS_STATIC = 66,                /* "static"  */
  YYSYMBOL_DAS_FIXED_ARRAY = 67,           /* "fixed_array"  */
  YYSYMBOL_DAS_DEFAULT = 68,               /* "default"  */
  YYSYMBOL_DAS_TBOOL = 69,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 70,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 71,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 72,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 73,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 74,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 75,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 76,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 77,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 78,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 79,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 80,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 81,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 82,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 83,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 84,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 85,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 86,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 87,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 88,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 89,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 90,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 91,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 92,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 93,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 94,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 95,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 96,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 97,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 98,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 99,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 100,               /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 101,             /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 102,            /* "generator"  */
  YYSYMBOL_DAS_YIELD = 103,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 104,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 105,                   /* "+="  */
  YYSYMBOL_SUBEQU = 106,                   /* "-="  */
  YYSYMBOL_DIVEQU = 107,                   /* "/="  */
  YYSYMBOL_MULEQU = 108,                   /* "*="  */
  YYSYMBOL_MODEQU = 109,                   /* "%="  */
  YYSYMBOL_ANDEQU = 110,                   /* "&="  */
  YYSYMBOL_OREQU = 111,                    /* "|="  */
  YYSYMBOL_XOREQU = 112,                   /* "^="  */
  YYSYMBOL_SHL = 113,                      /* "<<"  */
  YYSYMBOL_SHR = 114,                      /* ">>"  */
  YYSYMBOL_ADDADD = 115,                   /* "++"  */
  YYSYMBOL_SUBSUB = 116,                   /* "--"  */
  YYSYMBOL_LEEQU = 117,                    /* "<="  */
  YYSYMBOL_SHLEQU = 118,                   /* "<<="  */
  YYSYMBOL_SHREQU = 119,                   /* ">>="  */
  YYSYMBOL_GREQU = 120,                    /* ">="  */
  YYSYMBOL_EQUEQU = 121,                   /* "=="  */
  YYSYMBOL_NOTEQU = 122,                   /* "!="  */
  YYSYMBOL_RARROW = 123,                   /* "->"  */
  YYSYMBOL_LARROW = 124,                   /* "<-"  */
  YYSYMBOL_QQ = 125,                       /* "??"  */
  YYSYMBOL_QDOT = 126,                     /* "?."  */
  YYSYMBOL_QBRA = 127,                     /* "?["  */
  YYSYMBOL_LPIPE = 128,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 129,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 130,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 131,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 132,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 133,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 134,                 /* ":="  */
  YYSYMBOL_ROTL = 135,                     /* "<<<"  */
  YYSYMBOL_ROTR = 136,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 137,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 138,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 139,                    /* "=>"  */
  YYSYMBOL_COLCOL = 140,                   /* "::"  */
  YYSYMBOL_ANDAND = 141,                   /* "&&"  */
  YYSYMBOL_OROR = 142,                     /* "||"  */
  YYSYMBOL_XORXOR = 143,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 144,                /* "&&="  */
  YYSYMBOL_OROREQU = 145,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 146,                /* "^^="  */
  YYSYMBOL_DOTDOT = 147,                   /* ".."  */
  YYSYMBOL_MTAG_E = 148,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 149,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 150,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 151,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 152,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 153,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 154,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 155,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 156,           /* "..."  */
  YYSYMBOL_BRABRAB = 157,                  /* "[["  */
  YYSYMBOL_BRACBRB = 158,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 159,                  /* "{{"  */
  YYSYMBOL_INTEGER = 160,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 161,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 162,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 163,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 164,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 165,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 166,                   /* "double constant"  */
  YYSYMBOL_NAME = 167,                     /* "name"  */
  YYSYMBOL_KEYWORD = 168,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 169,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 170,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 171,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 172,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 173,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 174,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 175,          /* "}"  */
  YYSYMBOL_END_OF_READ = 176,              /* "end of failed eader macro"  */
  YYSYMBOL_177_begin_of_code_block_ = 177, /* "begin of code block"  */
  YYSYMBOL_178_end_of_code_block_ = 178,   /* "end of code block"  */
  YYSYMBOL_179_end_of_expression_ = 179,   /* "end of expression"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 180,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 181,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 182,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 183,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 184,            /* ",}]"  */
  YYSYMBOL_185_ = 185,                     /* ','  */
  YYSYMBOL_186_ = 186,                     /* '='  */
  YYSYMBOL_187_ = 187,                     /* '?'  */
  YYSYMBOL_188_ = 188,                     /* ':'  */
  YYSYMBOL_189_ = 189,                     /* '|'  */
  YYSYMBOL_190_ = 190,                     /* '^'  */
  YYSYMBOL_191_ = 191,                     /* '&'  */
  YYSYMBOL_192_ = 192,                     /* '<'  */
  YYSYMBOL_193_ = 193,                     /* '>'  */
  YYSYMBOL_194_ = 194,                     /* '-'  */
  YYSYMBOL_195_ = 195,                     /* '+'  */
  YYSYMBOL_196_ = 196,                     /* '*'  */
  YYSYMBOL_197_ = 197,                     /* '/'  */
  YYSYMBOL_198_ = 198,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 199,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 200,               /* UNARY_PLUS  */
  YYSYMBOL_201_ = 201,                     /* '~'  */
  YYSYMBOL_202_ = 202,                     /* '!'  */
  YYSYMBOL_PRE_INC = 203,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 204,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 205,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 206,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 207,                    /* DEREF  */
  YYSYMBOL_208_ = 208,                     /* '.'  */
  YYSYMBOL_209_ = 209,                     /* '['  */
  YYSYMBOL_210_ = 210,                     /* ']'  */
  YYSYMBOL_211_ = 211,                     /* '('  */
  YYSYMBOL_212_ = 212,                     /* ')'  */
  YYSYMBOL_213_ = 213,                     /* '$'  */
  YYSYMBOL_214_ = 214,                     /* '@'  */
  YYSYMBOL_215_ = 215,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 216,                 /* $accept  */
  YYSYMBOL_program = 217,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 218,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 219, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 220,              /* module_name  */
  YYSYMBOL_module_declaration = 221,       /* module_declaration  */
  YYSYMBOL_character_sequence = 222,       /* character_sequence  */
  YYSYMBOL_string_constant = 223,          /* string_constant  */
  YYSYMBOL_string_builder_body = 224,      /* string_builder_body  */
  YYSYMBOL_string_builder = 225,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 226, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 227,              /* expr_reader  */
  YYSYMBOL_228_1 = 228,                    /* $@1  */
  YYSYMBOL_options_declaration = 229,      /* options_declaration  */
  YYSYMBOL_require_declaration = 230,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 231,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 232,      /* require_module_name  */
  YYSYMBOL_require_module = 233,           /* require_module  */
  YYSYMBOL_is_public_module = 234,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 235,       /* expect_declaration  */
  YYSYMBOL_expect_list = 236,              /* expect_list  */
  YYSYMBOL_expect_error = 237,             /* expect_error  */
  YYSYMBOL_expression_label = 238,         /* expression_label  */
  YYSYMBOL_expression_goto = 239,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 240,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 241,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 242,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 243, /* expression_else_one_liner  */
  YYSYMBOL_244_2 = 244,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 245,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 246,  /* expression_if_then_else  */
  YYSYMBOL_247_3 = 247,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 248,      /* expression_for_loop  */
  YYSYMBOL_249_4 = 249,                    /* $@4  */
  YYSYMBOL_expression_unsafe = 250,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 251,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 252,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 253,    /* expression_with_alias  */
  YYSYMBOL_254_5 = 254,                    /* $@5  */
  YYSYMBOL_annotation_argument_value = 255, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 256, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 257, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 258,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 259, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 260, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 261, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 262,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 263,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 264, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 265, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 266,   /* optional_function_type  */
  YYSYMBOL_function_name = 267,            /* function_name  */
  YYSYMBOL_global_function_declaration = 268, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 269, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 270, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 271,     /* function_declaration  */
  YYSYMBOL_272_6 = 272,                    /* $@6  */
  YYSYMBOL_expression_block = 273,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 274,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 275,           /* expression_any  */
  YYSYMBOL_expressions = 276,              /* expressions  */
  YYSYMBOL_expr_keyword = 277,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 278,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 279, /* optional_expr_list_in_braces  */
  YYSYMBOL_type_declaration_no_options_list = 280, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 281,       /* expression_keyword  */
  YYSYMBOL_282_7 = 282,                    /* $@7  */
  YYSYMBOL_283_8 = 283,                    /* $@8  */
  YYSYMBOL_284_9 = 284,                    /* $@9  */
  YYSYMBOL_285_10 = 285,                   /* $@10  */
  YYSYMBOL_expr_pipe = 286,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 287,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 288,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 289,     /* new_type_declaration  */
  YYSYMBOL_290_11 = 290,                   /* $@11  */
  YYSYMBOL_291_12 = 291,                   /* $@12  */
  YYSYMBOL_expr_new = 292,                 /* expr_new  */
  YYSYMBOL_expression_break = 293,         /* expression_break  */
  YYSYMBOL_expression_continue = 294,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 295, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 296,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 297, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 298,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 299,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 300,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 301,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 302,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 303, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 304,           /* expression_let  */
  YYSYMBOL_expr_cast = 305,                /* expr_cast  */
  YYSYMBOL_306_13 = 306,                   /* $@13  */
  YYSYMBOL_307_14 = 307,                   /* $@14  */
  YYSYMBOL_308_15 = 308,                   /* $@15  */
  YYSYMBOL_309_16 = 309,                   /* $@16  */
  YYSYMBOL_310_17 = 310,                   /* $@17  */
  YYSYMBOL_311_18 = 311,                   /* $@18  */
  YYSYMBOL_expr_type_decl = 312,           /* expr_type_decl  */
  YYSYMBOL_313_19 = 313,                   /* $@19  */
  YYSYMBOL_314_20 = 314,                   /* $@20  */
  YYSYMBOL_expr_type_info = 315,           /* expr_type_info  */
  YYSYMBOL_expr_list = 316,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 317,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 318,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 319,            /* capture_entry  */
  YYSYMBOL_capture_list = 320,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 321,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 322,               /* expr_block  */
  YYSYMBOL_expr_full_block = 323,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 324, /* expr_full_block_assumed_piped  */
  YYSYMBOL_325_21 = 325,                   /* $@21  */
  YYSYMBOL_expr_numeric_const = 326,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 327,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 328,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 329,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 330,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 331,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 332,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 333,           /* func_addr_expr  */
  YYSYMBOL_334_22 = 334,                   /* $@22  */
  YYSYMBOL_335_23 = 335,                   /* $@23  */
  YYSYMBOL_336_24 = 336,                   /* $@24  */
  YYSYMBOL_337_25 = 337,                   /* $@25  */
  YYSYMBOL_expr_field = 338,               /* expr_field  */
  YYSYMBOL_339_26 = 339,                   /* $@26  */
  YYSYMBOL_340_27 = 340,                   /* $@27  */
  YYSYMBOL_expr_call = 341,                /* expr_call  */
  YYSYMBOL_expr = 342,                     /* expr  */
  YYSYMBOL_343_28 = 343,                   /* $@28  */
  YYSYMBOL_344_29 = 344,                   /* $@29  */
  YYSYMBOL_345_30 = 345,                   /* $@30  */
  YYSYMBOL_346_31 = 346,                   /* $@31  */
  YYSYMBOL_347_32 = 347,                   /* $@32  */
  YYSYMBOL_348_33 = 348,                   /* $@33  */
  YYSYMBOL_expr_mtag = 349,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 350, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 351,        /* optional_override  */
  YYSYMBOL_optional_constant = 352,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 353, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 354, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 355, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 356, /* struct_variable_declaration_list  */
  YYSYMBOL_357_34 = 357,                   /* $@34  */
  YYSYMBOL_358_35 = 358,                   /* $@35  */
  YYSYMBOL_359_36 = 359,                   /* $@36  */
  YYSYMBOL_function_argument_declaration = 360, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 361,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 362,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 363,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 364,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 365,             /* variant_type  */
  YYSYMBOL_variant_type_list = 366,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 367,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 368,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 369,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 370,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 371,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 372, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 373, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 374, /* global_variable_declaration_list  */
  YYSYMBOL_375_37 = 375,                   /* $@37  */
  YYSYMBOL_optional_shared = 376,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 377, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 378,               /* global_let  */
  YYSYMBOL_379_38 = 379,                   /* $@38  */
  YYSYMBOL_enum_list = 380,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 381, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 382,             /* single_alias  */
  YYSYMBOL_383_39 = 383,                   /* $@39  */
  YYSYMBOL_alias_list = 384,               /* alias_list  */
  YYSYMBOL_alias_declaration = 385,        /* alias_declaration  */
  YYSYMBOL_386_40 = 386,                   /* $@40  */
  YYSYMBOL_optional_public_or_private_enum = 387, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 388,                /* enum_name  */
  YYSYMBOL_enum_declaration = 389,         /* enum_declaration  */
  YYSYMBOL_390_41 = 390,                   /* $@41  */
  YYSYMBOL_391_42 = 391,                   /* $@42  */
  YYSYMBOL_392_43 = 392,                   /* $@43  */
  YYSYMBOL_393_44 = 393,                   /* $@44  */
  YYSYMBOL_optional_structure_parent = 394, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 395,          /* optional_sealed  */
  YYSYMBOL_structure_name = 396,           /* structure_name  */
  YYSYMBOL_class_or_struct = 397,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 398, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 399, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 400,    /* structure_declaration  */
  YYSYMBOL_401_45 = 401,                   /* $@45  */
  YYSYMBOL_402_46 = 402,                   /* $@46  */
  YYSYMBOL_variable_name_with_pos_list = 403, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 404,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 405, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 406, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 407,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 408,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 409,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 410, /* bitfield_type_declaration  */
  YYSYMBOL_411_47 = 411,                   /* $@47  */
  YYSYMBOL_412_48 = 412,                   /* $@48  */
  YYSYMBOL_table_type_pair = 413,          /* table_type_pair  */
  YYSYMBOL_dim_list = 414,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 415, /* type_declaration_no_options  */
  YYSYMBOL_416_49 = 416,                   /* $@49  */
  YYSYMBOL_417_50 = 417,                   /* $@50  */
  YYSYMBOL_418_51 = 418,                   /* $@51  */
  YYSYMBOL_419_52 = 419,                   /* $@52  */
  YYSYMBOL_420_53 = 420,                   /* $@53  */
  YYSYMBOL_421_54 = 421,                   /* $@54  */
  YYSYMBOL_422_55 = 422,                   /* $@55  */
  YYSYMBOL_423_56 = 423,                   /* $@56  */
  YYSYMBOL_424_57 = 424,                   /* $@57  */
  YYSYMBOL_425_58 = 425,                   /* $@58  */
  YYSYMBOL_426_59 = 426,                   /* $@59  */
  YYSYMBOL_427_60 = 427,                   /* $@60  */
  YYSYMBOL_428_61 = 428,                   /* $@61  */
  YYSYMBOL_429_62 = 429,                   /* $@62  */
  YYSYMBOL_430_63 = 430,                   /* $@63  */
  YYSYMBOL_431_64 = 431,                   /* $@64  */
  YYSYMBOL_432_65 = 432,                   /* $@65  */
  YYSYMBOL_433_66 = 433,                   /* $@66  */
  YYSYMBOL_434_67 = 434,                   /* $@67  */
  YYSYMBOL_435_68 = 435,                   /* $@68  */
  YYSYMBOL_436_69 = 436,                   /* $@69  */
  YYSYMBOL_437_70 = 437,                   /* $@70  */
  YYSYMBOL_438_71 = 438,                   /* $@71  */
  YYSYMBOL_439_72 = 439,                   /* $@72  */
  YYSYMBOL_440_73 = 440,                   /* $@73  */
  YYSYMBOL_441_74 = 441,                   /* $@74  */
  YYSYMBOL_442_75 = 442,                   /* $@75  */
  YYSYMBOL_type_declaration = 443,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 444,  /* tuple_alias_declaration  */
  YYSYMBOL_445_76 = 445,                   /* $@76  */
  YYSYMBOL_446_77 = 446,                   /* $@77  */
  YYSYMBOL_447_78 = 447,                   /* $@78  */
  YYSYMBOL_448_79 = 448,                   /* $@79  */
  YYSYMBOL_variant_alias_declaration = 449, /* variant_alias_declaration  */
  YYSYMBOL_450_80 = 450,                   /* $@80  */
  YYSYMBOL_451_81 = 451,                   /* $@81  */
  YYSYMBOL_452_82 = 452,                   /* $@82  */
  YYSYMBOL_453_83 = 453,                   /* $@83  */
  YYSYMBOL_bitfield_alias_declaration = 454, /* bitfield_alias_declaration  */
  YYSYMBOL_455_84 = 455,                   /* $@84  */
  YYSYMBOL_456_85 = 456,                   /* $@85  */
  YYSYMBOL_457_86 = 457,                   /* $@86  */
  YYSYMBOL_458_87 = 458,                   /* $@87  */
  YYSYMBOL_make_decl = 459,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 460,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 461,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 462,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 463, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 464, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 465, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 466, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 467, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_make_struct_decl = 468,         /* make_struct_decl  */
  YYSYMBOL_469_88 = 469,                   /* $@88  */
  YYSYMBOL_470_89 = 470,                   /* $@89  */
  YYSYMBOL_471_90 = 471,                   /* $@90  */
  YYSYMBOL_472_91 = 472,                   /* $@91  */
  YYSYMBOL_473_92 = 473,                   /* $@92  */
  YYSYMBOL_474_93 = 474,                   /* $@93  */
  YYSYMBOL_475_94 = 475,                   /* $@94  */
  YYSYMBOL_476_95 = 476,                   /* $@95  */
  YYSYMBOL_make_tuple = 477,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 478,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 479,          /* make_tuple_call  */
  YYSYMBOL_480_96 = 480,                   /* $@96  */
  YYSYMBOL_481_97 = 481,                   /* $@97  */
  YYSYMBOL_make_dim = 482,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 483,            /* make_dim_decl  */
  YYSYMBOL_484_98 = 484,                   /* $@98  */
  YYSYMBOL_485_99 = 485,                   /* $@99  */
  YYSYMBOL_486_100 = 486,                  /* $@100  */
  YYSYMBOL_487_101 = 487,                  /* $@101  */
  YYSYMBOL_make_table = 488,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 489,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 490,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 491, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 492,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 493       /* array_comprehension  */
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
#define YYLAST   14396

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  216
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  278
/* YYNRULES -- Number of rules.  */
#define YYNRULES  846
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1578

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   443


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
       2,     2,     2,   202,     2,   215,   213,   198,   191,     2,
     211,   212,   196,   195,   185,   194,   208,   197,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   188,   179,
     192,   186,   193,   187,   214,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   209,     2,   210,   190,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   177,   189,   178,   201,     2,     2,     2,
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
     175,   176,   180,   181,   182,   183,   184,   199,   200,   203,
     204,   205,   206,   207
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   530,   530,   531,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   552,   558,   559,
     560,   564,   565,   569,   587,   588,   589,   590,   594,   598,
     603,   612,   620,   636,   641,   649,   649,   688,   718,   722,
     723,   724,   728,   731,   735,   741,   750,   753,   759,   760,
     764,   768,   769,   773,   776,   782,   788,   791,   797,   798,
     802,   803,   804,   813,   814,   818,   819,   819,   825,   826,
     827,   828,   829,   833,   839,   839,   845,   845,   851,   859,
     869,   878,   878,   885,   886,   887,   888,   889,   890,   894,
     899,   907,   908,   909,   913,   914,   915,   916,   917,   918,
     919,   920,   926,   929,   935,   936,   937,   941,   954,   972,
     975,   983,   994,  1005,  1016,  1019,  1026,  1030,  1037,  1038,
    1042,  1043,  1044,  1048,  1051,  1058,  1062,  1063,  1064,  1065,
    1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,  1075,
    1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,  1085,
    1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,
    1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,
    1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,
    1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,
    1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,
    1136,  1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,
    1150,  1168,  1169,  1170,  1174,  1180,  1180,  1197,  1201,  1212,
    1221,  1233,  1234,  1235,  1236,  1237,  1238,  1239,  1240,  1241,
    1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,  1251,
    1252,  1253,  1257,  1262,  1268,  1274,  1285,  1286,  1290,  1291,
    1295,  1299,  1306,  1306,  1306,  1312,  1312,  1312,  1321,  1355,
    1358,  1361,  1364,  1370,  1371,  1382,  1386,  1389,  1397,  1397,
    1397,  1400,  1406,  1409,  1412,  1416,  1423,  1429,  1433,  1437,
    1440,  1443,  1451,  1454,  1457,  1465,  1468,  1476,  1479,  1482,
    1490,  1496,  1497,  1501,  1502,  1506,  1511,  1519,  1525,  1531,
    1540,  1552,  1555,  1561,  1561,  1561,  1564,  1564,  1564,  1569,
    1569,  1569,  1577,  1577,  1577,  1583,  1593,  1604,  1619,  1622,
    1628,  1629,  1636,  1647,  1648,  1649,  1653,  1654,  1655,  1656,
    1660,  1665,  1673,  1674,  1678,  1683,  1690,  1697,  1697,  1706,
    1707,  1708,  1709,  1710,  1711,  1712,  1716,  1717,  1718,  1719,
    1720,  1721,  1722,  1723,  1724,  1725,  1726,  1727,  1728,  1729,
    1730,  1731,  1732,  1733,  1734,  1738,  1739,  1740,  1741,  1746,
    1747,  1748,  1749,  1750,  1751,  1752,  1753,  1754,  1755,  1756,
    1757,  1758,  1759,  1760,  1761,  1762,  1767,  1774,  1786,  1791,
    1801,  1805,  1812,  1815,  1815,  1815,  1820,  1820,  1820,  1833,
    1837,  1841,  1847,  1854,  1854,  1854,  1861,  1865,  1874,  1878,
    1881,  1887,  1888,  1889,  1890,  1891,  1892,  1893,  1894,  1895,
    1896,  1897,  1898,  1899,  1900,  1901,  1902,  1903,  1904,  1905,
    1906,  1907,  1908,  1909,  1910,  1911,  1912,  1913,  1914,  1915,
    1916,  1917,  1918,  1919,  1920,  1921,  1922,  1928,  1929,  1930,
    1931,  1932,  1945,  1946,  1947,  1948,  1949,  1950,  1951,  1952,
    1953,  1954,  1955,  1956,  1959,  1962,  1963,  1966,  1966,  1966,
    1969,  1974,  1978,  1982,  1982,  1982,  1987,  1990,  1994,  1994,
    1994,  1999,  2002,  2003,  2004,  2005,  2006,  2007,  2008,  2009,
    2010,  2012,  2016,  2017,  2022,  2026,  2027,  2028,  2029,  2030,
    2031,  2032,  2036,  2040,  2044,  2048,  2052,  2056,  2060,  2064,
    2068,  2075,  2076,  2080,  2081,  2082,  2086,  2087,  2091,  2092,
    2093,  2097,  2098,  2102,  2113,  2116,  2116,  2135,  2134,  2149,
    2148,  2161,  2170,  2176,  2181,  2191,  2192,  2196,  2199,  2208,
    2209,  2213,  2216,  2219,  2235,  2244,  2245,  2249,  2252,  2255,
    2269,  2270,  2274,  2280,  2286,  2289,  2293,  2299,  2308,  2309,
    2310,  2314,  2315,  2319,  2326,  2331,  2340,  2346,  2357,  2360,
    2365,  2370,  2378,  2389,  2392,  2392,  2412,  2413,  2417,  2418,
    2419,  2423,  2426,  2426,  2445,  2448,  2451,  2466,  2485,  2486,
    2487,  2492,  2492,  2518,  2519,  2523,  2524,  2524,  2528,  2529,
    2530,  2534,  2544,  2549,  2544,  2561,  2566,  2561,  2581,  2582,
    2586,  2587,  2591,  2597,  2598,  2602,  2603,  2604,  2608,  2611,
    2617,  2622,  2617,  2636,  2643,  2648,  2657,  2663,  2674,  2675,
    2676,  2677,  2678,  2679,  2680,  2681,  2682,  2683,  2684,  2685,
    2686,  2687,  2688,  2689,  2690,  2691,  2692,  2693,  2694,  2695,
    2696,  2697,  2698,  2699,  2700,  2704,  2705,  2706,  2707,  2708,
    2709,  2713,  2724,  2728,  2735,  2747,  2754,  2763,  2768,  2771,
    2784,  2784,  2784,  2797,  2801,  2808,  2812,  2819,  2820,  2821,
    2822,  2823,  2838,  2844,  2844,  2844,  2848,  2853,  2860,  2860,
    2867,  2871,  2875,  2880,  2885,  2890,  2895,  2899,  2903,  2908,
    2912,  2916,  2921,  2921,  2921,  2927,  2934,  2934,  2934,  2939,
    2939,  2939,  2945,  2945,  2945,  2950,  2954,  2954,  2954,  2959,
    2959,  2959,  2968,  2972,  2972,  2972,  2977,  2977,  2977,  2986,
    2990,  2990,  2990,  2995,  2995,  2995,  3004,  3004,  3004,  3010,
    3010,  3010,  3019,  3022,  3033,  3049,  3049,  3054,  3059,  3049,
    3084,  3084,  3089,  3095,  3084,  3120,  3120,  3125,  3130,  3120,
    3160,  3161,  3162,  3163,  3164,  3168,  3175,  3182,  3188,  3194,
    3201,  3208,  3214,  3224,  3229,  3236,  3237,  3242,  3243,  3247,
    3248,  3252,  3253,  3257,  3258,  3259,  3263,  3264,  3265,  3270,
    3276,  3283,  3291,  3298,  3306,  3315,  3315,  3315,  3323,  3323,
    3323,  3330,  3330,  3330,  3337,  3337,  3337,  3348,  3351,  3357,
    3371,  3377,  3383,  3389,  3389,  3389,  3399,  3404,  3411,  3419,
    3424,  3431,  3439,  3439,  3439,  3447,  3454,  3454,  3454,  3464,
    3469,  3476,  3479,  3485,  3493,  3502,  3510,  3518,  3531,  3532,
    3536,  3537,  3542,  3545,  3548,  3551,  3554
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
  "\"finally\"", "\"delete\"", "\"deref\"", "\"typedef\"", "\"typedecl\"",
  "\"with\"", "\"aka\"", "\"assume\"", "\"cast\"", "\"override\"",
  "\"abstract\"", "\"upcast\"", "\"iterator\"", "\"var\"", "\"addr\"",
  "\"continue\"", "\"where\"", "\"pass\"", "\"reinterpret\"", "\"module\"",
  "\"public\"", "\"label\"", "\"goto\"", "\"implicit\"", "\"explicit\"",
  "\"shared\"", "\"private\"", "\"smart_ptr\"", "\"unsafe\"",
  "\"inscope\"", "\"static\"", "\"fixed_array\"", "\"default\"",
  "\"bool\"", "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"",
  "\"int3\"", "\"int4\"", "\"uint\"", "\"bitfield\"", "\"uint2\"",
  "\"uint3\"", "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"",
  "\"float4\"", "\"range\"", "\"urange\"", "\"range64\"", "\"urange64\"",
  "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"", "\"function\"",
  "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"",
  "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"", "\"sealed\"",
  "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"",
  "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"",
  "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"",
  "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"$ <|\"", "\"@ <|\"",
  "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"",
  "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"",
  "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"", "\"[{\"",
  "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"keyword\"", "\"type function\"",
  "\"start of the string\"", "STRING_CHARACTER", "STRING_CHARACTER_ESC",
  "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "\"begin of code block\"",
  "\"end of code block\"", "\"end of expression\"", "\";}}\"", "\";}]\"",
  "\";]]\"", "\",]]\"", "\",}]\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "POST_INC", "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('", "')'",
  "'$'", "'@'", "'#'", "$accept", "program", "top_level_reader_macro",
  "optional_public_or_private_module", "module_name", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "reader_character_sequence", "expr_reader", "$@1",
  "options_declaration", "require_declaration", "keyword_or_name",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "expression_else",
  "if_or_static_if", "expression_else_one_liner", "$@2",
  "expression_if_one_liner", "expression_if_then_else", "$@3",
  "expression_for_loop", "$@4", "expression_unsafe",
  "expression_while_loop", "expression_with", "expression_with_alias",
  "$@5", "annotation_argument_value", "annotation_argument_value_list",
  "annotation_argument_name", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@6", "expression_block", "expr_call_pipe",
  "expression_any", "expressions", "expr_keyword", "optional_expr_list",
  "optional_expr_list_in_braces", "type_declaration_no_options_list",
  "expression_keyword", "$@7", "$@8", "$@9", "$@10", "expr_pipe",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@11",
  "$@12", "expr_new", "expression_break", "expression_continue",
  "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let", "optional_in_scope", "tuple_expansion",
  "tuple_expansion_variable_declaration", "expression_let", "expr_cast",
  "$@13", "$@14", "$@15", "$@16", "$@17", "$@18", "expr_type_decl", "$@19",
  "$@20", "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_block", "expr_full_block",
  "expr_full_block_assumed_piped", "$@21", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe_right", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@22", "$@23", "$@24", "$@25", "expr_field", "$@26",
  "$@27", "expr_call", "expr", "$@28", "$@29", "$@30", "$@31", "$@32",
  "$@33", "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@34", "$@35", "$@36",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@37", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@38", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@39", "alias_list",
  "alias_declaration", "$@40", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "$@41", "$@42", "$@43", "$@44",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@45", "$@46", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_alias_bits",
  "bitfield_type_declaration", "$@47", "$@48", "table_type_pair",
  "dim_list", "type_declaration_no_options", "$@49", "$@50", "$@51",
  "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60",
  "$@61", "$@62", "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69",
  "$@70", "$@71", "$@72", "$@73", "$@74", "$@75", "type_declaration",
  "tuple_alias_declaration", "$@76", "$@77", "$@78", "$@79",
  "variant_alias_declaration", "$@80", "$@81", "$@82", "$@83",
  "bitfield_alias_declaration", "$@84", "$@85", "$@86", "$@87",
  "make_decl", "make_struct_fields", "make_struct_dim", "optional_block",
  "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "make_struct_decl", "$@88", "$@89",
  "$@90", "$@91", "$@92", "$@93", "$@94", "$@95", "make_tuple",
  "make_map_tuple", "make_tuple_call", "$@96", "$@97", "make_dim",
  "make_dim_decl", "$@98", "$@99", "$@100", "$@101", "make_table",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1397)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-734)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1397,    74, -1397, -1397,    36,    48,   254,   249, -1397,   -94,
     433,   433,   433, -1397,   -39,    97, -1397, -1397,    -9, -1397,
   -1397, -1397,   403, -1397,   143, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397,    65, -1397,    89,   118,   176,
   -1397, -1397, -1397, -1397,   254, -1397,     2, -1397,   433,   433,
   -1397, -1397,   143, -1397, -1397, -1397, -1397, -1397,   379,   388,
   -1397, -1397, -1397,    97,    97,    97,   237, -1397,   617,   -90,
   -1397, -1397, -1397, -1397,   463,   503,   513, -1397,   585,    73,
      36,   411,    48,   372,   461, -1397,   610,   610, -1397,   519,
     428,   -28,   544,   595,   563,   575,   604, -1397,   644,   566,
   -1397, -1397,   -75,    36,    97,    97,    97,    97, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397,   647, -1397, -1397, -1397, -1397,
   -1397,   612, -1397, -1397, -1397, -1397, -1397,   -60,   175, -1397,
   -1397, -1397, -1397,   764, -1397, -1397, -1397, -1397, -1397,   642,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,   651,
   -1397,   129, -1397,   658,   684,   617, 14229, -1397,    10,   722,
   -1397,   671, -1397, -1397,   622, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397,   188, -1397,   643, -1397,   655,   656,   657, -1397,
   -1397, 12977, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397,   801,   803, -1397,
     627,   662, -1397,   454, -1397,   673, -1397,   663,    36,   -53,
   -1397, -1397, -1397,   175, -1397,  9934, -1397, -1397, -1397,   675,
     676, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397,   677,   636, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397,   826, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,   681,   641,
   -1397, -1397,   -73,   664, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397,   678,   665,   679, -1397,   671,   255,   648,
     812,   500, -1397, -1397,   670,   672,   674,   654,   682,   683,
   -1397, -1397, -1397,   666, -1397, -1397, -1397, -1397, -1397,   686,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397,   689, -1397, -1397, -1397,   696,   699, -1397, -1397, -1397,
   -1397,   700,   702,   688,   -39, -1397, -1397, -1397, -1397, -1397,
     263,   687, -1397, -1397, -1397, -1397, -1397, -1397, -1397,   701,
     739, -1397,   690, -1397,    -4, -1397,    -7,  9934, -1397,  2306,
     192, -1397,   -39, -1397, -1397, -1397,   -53,   692,  9165,   720,
     733,  9934, -1397,    -2, -1397, -1397, -1397,  9165, -1397, -1397,
     737, -1397,   293,   361,   391, -1397, -1397,  9165,   120, -1397,
   -1397, -1397,    -5, -1397, -1397, -1397,    45,  5340, -1397,   703,
    9724,   215,  9823,   308, -1397, -1397,  9165,   151,   697,   868,
   -1397,   125,   570, -1397,   687, -1397,   721,   727,  9165, -1397,
   -1397, -1397, -1397, -1397,   112,   705,   728,   281,  3324, -1397,
   -1397,   662,   296,  5541,   710,  9165,   755,   731,   738,   714,
   -1397,   735,   742,   769,  5742,   -96,   298,   744, -1397,   326,
     746,   748,  3525,  9165,  9165,   119,   119,   119,   730,   732,
     734,   736,   745,   749, -1397,  9526,  9625,  5945, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397,  6146,   750, -1397,  6349,   895,
   -1397,  9165,  9165,  9165,  9165,  9165,  6552,  9165, -1397,   741,
   -1397, -1397,   765,   767,  9165,   939, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397,   462, -1397,   -84,   771, -1397,
     774,   778,   779, -1397,   782, -1397, -1397,   884, -1397, -1397,
   -1397, -1397,   753, -1397, -1397,   -54, -1397, -1397, -1397, -1397,
   -1397, -1397,  1876, -1397,   752, -1397, -1397, -1397, -1397, -1397,
   -1397,   425, -1397,   791, -1397, -1397,    15, -1397, -1397,   762,
     783, -1397, 10379, -1397,   932,    94, -1397, -1397, -1397,  3927,
    9934,  9934,  9934, 10488,  9934,  9934,   768,   809,  9934,   627,
    9934,   627,  9934,   627, 10033,   810, 10523, -1397,  9165, -1397,
   -1397, -1397, -1397,   777, -1397, -1397, 12504,  9165, -1397,   263,
     802, -1397,   820,    51, -1397,   821,   687,   823,   800, -1397,
     824,   828, 10632, -1397,  9165,   832, -1397,   840, -1397,  9934,
   -1397,  3927, -1397, -1397, -1397, 13012,   151, -1397,   804, -1397,
   -1397,   -39, -1397, -1397,  9165,  3927, -1397,   833,   890, 10115,
    1008,  9934,  9165,  9165, 13743,  9165, 13012,   836, -1397, -1397,
    9165, -1397, -1397,   837,   864, 13743,  9165, -1397, -1397,  9165,
   -1397, -1397,  9165, -1397,  9934,  3927, -1397, 10115,    50,    50,
     813, -1397,   753, -1397, -1397, -1397,  9165,  9165,  9165,  9165,
    9165,  9165,   151,  2718,   151,  2920,   151, 13110, -1397,   618,
   -1397, 13012, -1397,   614,   151, -1397,   841,   851,    50,    50,
       8,    50,    50,   151,   844, 13743,   844,   381, -1397, -1397,
   13012, -1397, -1397, -1397, -1397,  4329, -1397, -1397, -1397, -1397,
   -1397, -1397,   -49,   873,   119, -1397, 14026, 14121,  4128,  4128,
    4128,  4128,  4128,  4128,  4128,  4128,  9165,  9165, -1397, -1397,
    9165,  4128,  4128,  9165,  9165,  9165,   865,  4128,  9165,   436,
    9165,  9165,  9165,  9165,  9165,  9165,  4128,  4128,  9165,  9165,
    9165,  4128,  4128,  4128,  9165,  4128,  6753,  9165,  9165,  9165,
    9165,  9165,  9165,  9165,  9165,  9165,  9165,   171,  9165, -1397,
    6954, -1397,  9165, -1397,   192, -1397,    97,  1024,   671, -1397,
     866, -1397,  3927, -1397, 10234,   246,   437,   842,   202, -1397,
     536,   542, -1397, -1397,    69,   550,   664,   559,   664,   560,
     664, -1397,   184, -1397,   343, -1397,  9934,   822,   833, -1397,
   -1397, 12539, -1397, -1397,  9934, -1397, -1397,  9934, -1397, -1397,
   -1397, 10667, -1397,   995,   -65, -1397, 10115,  9934,  9934, -1397,
       7,  9934,  4530,  7155,  9934,  9934,   844, -1397,   833,  9165,
    9165,  9165,  9165,  9165,  9165,  9165,  9165,  9165,  9165,  9165,
    9165,  9165,  9165,  9165,  9165,  9165,  9165,   662,   553,   841,
   13743, 10776, -1397, -1397,  9934,  9934, 10811,  9934, -1397, -1397,
   10920,  9934,   844,  9934,  9934,   844,  9934,   218, -1397, 10115,
   -1397,   873, 10955, 11064, 11099, 11208, 11243, 11352,    16,   119,
     829,   -30,  3122,  4733,  7356, 13208,   854,   -22,   173,   856,
     196,    29,  7557,   -22,   516,    37,  9165,   870,  9165, -1397,
   -1397,  9934, -1397,  9934, -1397,  9165,   635,    41,  9165,   872,
   -1397,    44,  9165,   834,   831,   835,   843,   404, -1397, -1397,
     626,  9165,   753,  4936, -1397,   216,   113,   880,   880, -1397,
   -1397,   -48,   627, -1397,   859,   846, -1397, -1397,   863,   857,
   -1397, -1397,   119,   119,   119, -1397, -1397,  1661, -1397,  1661,
   -1397,  1661, -1397,  1661, -1397,  1661, -1397,  1661, -1397,  1661,
   -1397,  1661,   988,   988,   770, -1397,  1661, -1397,  1661,   770,
    9852,  9852,   858, -1397,  1661,   221,   860, -1397, 12637,   -62,
     -62,   752, 13743,   988,   988, -1397,  1661, -1397,  1661,  1623,
     473,  2060, -1397,  1661, -1397,  1661, -1397,  1661, 13833, -1397,
    1661, 14152, 13243,  1925, 13868, 13958,   770,   770,  1151,  1151,
     221,   221,   221,   465,  9165,   861,   862,   479,  9165,  1073,
   12735, -1397,   230, 13333,   897,   256,   649,  1010,   901, -1397,
   -1397, 10344, -1397, -1397, -1397, -1397,  9934, -1397, -1397,   914,
   -1397, -1397,   889, -1397,   891, -1397,   894, 10033, -1397,   810,
   -1397,   347,   687, -1397, -1397,   687,   687, -1397,   921,  9165,
     724,   892,  9165,   561, -1397,   234,   122,   922, 11387,   562,
    1106,   878, 13743, 13743, 13743, 13743, 13743, 13743, 13743, 13743,
   13743, 13743, 13743, 13743, 13743, 13743, 13743, 13743, 13743, 13743,
   -1397,  9934,   881,   879, -1397,  9165,  1119,  1148, -1397,  1201,
   -1397,  1258,   883,  1272,  1510,   887,  1741,   873,   627, -1397,
   -1397, -1397, -1397, -1397,   882,  9165, -1397,  9165,  9165,  9165,
    5139, 12504,    33,  9165,   488,   506,   173, -1397, -1397,   886,
   -1397,  9165,  9165, -1397,   896, -1397,  9165,   506,   525,   898,
   -1397, -1397,  9165, 13743, -1397, -1397,   383,   384, 13368,  9165,
   -1397, -1397,  2517,  9165, -1397, -1397,  9165,  9165,  9934,   627,
     662, -1397, -1397,  9165, -1397,   869,   873,   260,  9366, -1397,
   -1397, -1397,   264,   239,   933,   935,   943,   950, -1397,   270,
     664, -1397,  9165, -1397,  9165, -1397, -1397, -1397,  7758,  9165,
   -1397,   926,   908, -1397, -1397,  9165,   909, -1397, 12770,  9165,
    7959,   911, -1397, 12868, -1397, -1397, -1397, -1397, -1397,   944,
   -1397, -1397,    43, -1397,    27, -1397, -1397, -1397, -1397, -1397,
     687, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397,  9934,   913, -1397, 13743, -1397,
   -1397,   495, -1397, -1397, -1397,   440, -1397, -1397, -1397, -1397,
    2104,  9165, -1397, 13743, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397,   915,   664,  8160,   529, 11496, 13743, 13743,
      61,   -22,   173, 13743,   917,   121,   854, -1397, -1397, 13743,
     856, -1397,   531,   -22,   919, -1397, -1397, -1397, -1397,   534,
   -1397, -1397, -1397,   539, -1397,   541, 11531, 11640,  2233,   664,
   -1397, 13012, -1397,   946,   627,   920,  4936,   964,   924,   578,
   -1397, -1397, -1397, -1397,   -48,   925,    25,  9934, 11675,  9934,
   11784, -1397,   259, 11819, -1397,  9165,  1464,  9165, -1397, 11928,
   -1397,   273,  9165, -1397, -1397, -1397,  1108,    27, -1397, -1397,
     649, -1397, -1397, -1397,   687,  9165,   930,   931, -1397, -1397,
     976,  9165, -1397,   934,   936,   841,  9165,  9165,  9165,   938,
   -1397,   942,   945,  8361,    25, -1397,   279,  9165,   133,   173,
   -1397,  9165,  9165,  9165,  9165,   525, -1397,  9165,  9165,   948,
    9165,  9165, -1397, -1397, -1397,   951,   626,  3726, -1397,   664,
   -1397,   292, -1397,   582,  9934,    -2, -1397, -1397,  8562, -1397,
   -1397,  2638, -1397,   568, -1397, -1397, -1397,  9934, 11963, 12072,
   -1397, -1397, 12107, -1397, -1397,  1108,   151,   949,   506,   506,
     953, 12216,  9165,  9165,   955,   770,   770,   770,  9165,   506,
     506, -1397, 12251, -1397, -1397, 13458,  9165,  9165, -1397, 12360,
   13743, 13743, 13458, -1397,   971,   770,  9165, -1397,   971, 13458,
     153, -1397, -1397,  8763,  8964, -1397, -1397, -1397, -1397, -1397,
   13743,   662,   957,  9934,    -2,  1077,  9165,  9165, 13833, -1397,
   -1397,   574, -1397, -1397, -1397, 14229, -1397, -1397, -1397,   138,
     145,  9165, -1397,   844,   841, -1397,   844,   148,   150, -1397,
    1103,   947, 13743, 13743,   142,   982,   984,   958,   985,   963,
   -1397,   153,  9165,  9165, 13743, -1397, -1397,  1077,  9165,  9165,
   13493,  1464, -1397, -1397, -1397,   997, 14229, -1397, -1397, 12395,
     962,   969,   975, -1397, -1397,  9165,   978,  9165,  9165,   980,
    1013, -1397, -1397, -1397, -1397, 13743,  9165, 13583, 13618, -1397,
   -1397, -1397,   662, -1397, -1397, -1397, -1397, 13743, -1397, 13743,
   13743, -1397, -1397, 13708, -1397, -1397, -1397, -1397
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   118,     1,   291,     0,     0,     0,   596,   292,     0,
     588,   588,   588,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   576,     6,    11,     5,     4,    13,
      12,    14,    92,    93,    91,   100,   102,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   588,   588,
      22,    21,   576,   590,   589,   755,   745,   750,     0,   263,
      35,   105,   106,     0,     0,     0,   107,   109,   116,     0,
     104,    17,   614,   613,   211,   598,   615,   577,   578,     0,
       0,     0,     0,    43,     0,    49,     0,     0,    46,     0,
       0,   588,     0,    18,     0,     0,     0,   265,     0,     0,
     115,   110,     0,     0,     0,     0,     0,     0,   119,   213,
     212,   215,   210,   600,   599,     0,   617,   616,   620,   580,
     579,   582,    98,    99,    96,    97,    95,     0,     0,    94,
     103,    54,    52,    48,    45,    44,   591,   593,   595,     0,
     597,    19,    20,    23,   756,   746,   751,   264,    33,    36,
     114,     0,   111,   112,   113,   117,     0,   601,     0,   610,
     573,   511,    24,    25,     0,    87,    88,    85,    86,    84,
      83,    89,     0,    47,     0,   594,     0,     0,     0,    34,
     108,     0,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,     0,     0,   125,
     120,     0,   602,     0,   611,     0,   621,   574,     0,     0,
      26,    27,    28,     0,   101,     0,   757,   747,   752,   179,
     180,   177,   128,   129,   131,   130,   132,   133,   134,   135,
     161,   162,   159,   160,   152,   163,   164,   153,   150,   151,
     178,   172,     0,   176,   165,   166,   167,   168,   139,   140,
     141,   136,   137,   138,   149,     0,   155,   156,   154,   147,
     148,   143,   142,   144,   145,   146,   127,   126,   171,     0,
     157,   158,   511,   123,   242,   216,   584,   655,   658,   656,
     659,   657,   660,     0,   608,   618,   581,   511,     0,     0,
     563,   561,   583,    90,     0,     0,     0,     0,     0,     0,
     628,   648,   629,   662,   630,   634,   635,   636,   637,   654,
     641,   642,   643,   644,   645,   646,   647,   649,   650,   651,
     652,   715,   633,   640,   653,   722,   729,   631,   638,   632,
     639,     0,     0,     0,     0,   661,   677,   680,   678,   679,
     742,   592,   667,   541,   547,   181,   182,   175,   170,   183,
     173,   169,     0,   121,     0,   535,     0,     0,   214,     0,
     603,   605,     0,   612,   524,   622,     0,     0,     0,     0,
       0,     0,   562,     0,   683,   706,   709,     0,   712,   702,
       0,   670,   716,   723,   730,   736,   739,     0,     0,   692,
     697,   691,     0,   705,   701,   694,     0,     0,   696,   681,
       0,   758,   748,   753,   184,   174,     0,     0,     0,   623,
     532,   552,   511,   122,   124,   244,     0,     0,     0,    63,
      64,    76,   417,   418,     0,     0,     0,     0,   279,   411,
     277,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     278,     0,     0,     0,     0,     0,     0,     0,   654,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   500,     0,     0,     0,   339,   341,
     340,   342,   343,   344,   345,     0,     0,    29,     0,   217,
     221,     0,     0,     0,     0,     0,     0,     0,   323,   324,
     415,   414,     0,     0,     0,     0,   237,   232,   229,   228,
     230,   231,   262,   243,   223,   494,   222,   412,     0,   485,
      71,    72,    69,   235,    70,   236,   238,   294,   227,   484,
     483,   482,   118,   488,   413,     0,   224,   487,   486,   458,
     419,   459,   346,   420,     0,   416,   760,   764,   761,   762,
     763,     0,   585,     0,   584,   609,   525,   575,   512,     0,
       0,   494,     0,   565,   566,     0,   559,   560,   558,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   120,
       0,   120,     0,   120,     0,     0,     0,   688,   246,   699,
     700,   693,   695,     0,   698,   682,     0,     0,   744,   743,
       0,   668,     0,   263,   542,     0,   537,     0,     0,   548,
       0,     0,     0,   533,     0,     0,   551,     0,   550,     0,
     553,     0,   536,   795,   798,     0,     0,   268,   272,   271,
     276,     0,   312,   822,     0,     0,   283,   280,     0,   318,
       0,     0,     0,     0,   266,     0,     0,     0,   303,   306,
       0,   241,   309,     0,     0,    57,     0,    78,   826,     0,
     804,   813,     0,   801,     0,     0,   288,   285,   447,   448,
     324,   334,   118,   261,   259,   260,     0,     0,     0,     0,
       0,     0,     0,   775,     0,     0,     0,   811,   829,     0,
     252,     0,   255,     0,     0,   831,   840,     0,   424,   423,
     460,   422,   421,     0,   840,   318,   840,   325,   239,   240,
       0,    74,   337,   220,   492,     0,   226,   233,   234,   282,
     287,   293,     0,   332,     0,   225,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   449,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,   219,
       0,   586,     0,   604,   606,   619,     0,     0,   511,   564,
       0,   568,     0,   572,   346,     0,     0,     0,   673,   686,
       0,     0,   663,   665,     0,     0,   123,     0,   123,     0,
     123,   539,     0,   545,     0,   664,     0,     0,   247,   690,
     675,     0,   669,   759,     0,   543,   749,     0,   549,   754,
     534,     0,   625,   626,   554,   557,   556,     0,     0,    79,
       0,     0,     0,     0,     0,     0,   840,   284,   281,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   840,
     267,     0,    80,    81,     0,     0,     0,     0,    55,    56,
       0,     0,   840,     0,     0,   840,     0,     0,   289,   286,
     325,   332,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   263,     0,     0,     0,   807,   773,   775,     0,   816,
       0,     0,     0,   775,     0,     0,     0,     0,     0,   778,
     834,     0,   245,     0,    32,     0,    30,     0,   841,     0,
     242,     0,   841,     0,     0,     0,     0,   393,   390,   392,
      60,     0,   118,     0,   406,     0,     0,     0,     0,   302,
     301,     0,   120,   258,     0,     0,   471,   470,     0,     0,
     472,   476,     0,     0,     0,   368,   377,   356,   378,   357,
     380,   359,   379,   358,   381,   360,   371,   350,   372,   351,
     373,   352,   425,   426,   438,   382,   361,   383,   362,   439,
     436,   437,     0,   370,   348,   465,     0,   456,     0,   489,
     490,   491,   349,   427,   428,   384,   363,   385,   364,   443,
     444,   445,   374,   353,   375,   354,   376,   355,   446,   369,
     347,     0,     0,   441,   442,   440,   434,   435,   430,   429,
     431,   432,   433,     0,     0,     0,   399,     0,     0,     0,
       0,   409,     0,     0,     0,     0,   518,   521,     0,   567,
     570,   346,   571,   684,   707,   710,     0,   713,   703,     0,
     671,   717,     0,   724,     0,   731,     0,     0,   737,     0,
     740,     0,   250,   687,   676,   538,   544,   624,     0,     0,
       0,     0,     0,     0,   273,     0,     0,     0,     0,     0,
       0,     0,   319,   356,   357,   359,   358,   360,   350,   351,
     352,   361,   362,   348,   363,   364,   353,   354,   355,   347,
     290,     0,     0,     0,   461,     0,     0,     0,   462,     0,
     493,     0,     0,     0,     0,     0,     0,   332,   120,   495,
     496,   497,   498,   499,     0,     0,   776,     0,     0,     0,
       0,   318,   775,     0,     0,     0,     0,   784,   785,     0,
     790,     0,     0,   782,     0,   819,     0,     0,     0,     0,
     780,   820,     0,   810,   777,   830,     0,     0,     0,     0,
     832,   833,     0,     0,   818,   451,     0,     0,     0,   120,
       0,    58,    59,     0,    73,    65,   332,     0,     0,   408,
     407,   295,     0,     0,     0,     0,     0,     0,   330,     0,
     123,   467,     0,   473,     0,   367,   365,   366,     0,     0,
     454,     0,     0,   477,   481,     0,     0,   457,     0,     0,
       0,     0,   400,     0,   404,   452,   410,   587,   607,   119,
     519,   520,   521,   522,   513,   526,   569,   685,   708,   711,
     674,   714,   704,   666,   672,   718,   720,   725,   727,   732,
     734,   540,   738,   546,   741,     0,     0,   627,   555,   796,
     799,     0,   269,   274,   275,     0,   315,   313,   823,   821,
       0,     0,   835,    82,   304,   307,   310,   827,   825,   805,
     814,   812,   802,     0,   123,     0,     0,     0,   766,   765,
       0,   775,     0,   808,     0,     0,   774,   789,   783,   809,
     817,   781,     0,   775,     0,   787,   788,   793,   779,     0,
     253,   256,    31,     0,   218,     0,     0,     0,     0,   123,
      61,     0,    66,     0,   120,     0,     0,     0,     0,   561,
     328,   329,   327,   326,     0,     0,     0,     0,     0,     0,
       0,   388,     0,     0,   478,     0,   466,     0,   455,     0,
     401,     0,     0,   453,   405,   531,   516,   513,   514,   515,
     518,   721,   728,   735,   251,   246,     0,     0,    77,   270,
       0,     0,   314,     0,     0,   840,     0,     0,     0,     0,
     806,     0,     0,     0,     0,   501,     0,     0,     0,     0,
     791,     0,     0,     0,     0,     0,   786,     0,     0,   248,
       0,     0,   391,   510,   394,     0,    60,     0,    75,   123,
     386,     0,   296,   561,     0,     0,   331,   333,     0,   320,
     336,     0,   509,     0,   507,   389,   504,     0,     0,     0,
     503,   402,     0,   517,   527,   516,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   305,   308,   311,     0,     0,
       0,   463,     0,   335,   502,   838,     0,     0,   792,     0,
     768,   767,   838,   794,   838,   254,   246,   257,   838,   838,
       0,   397,    62,   279,     0,    67,    71,    72,    69,    70,
      68,     0,     0,     0,     0,     0,     0,     0,   321,   468,
     474,     0,   508,   506,   505,     0,   529,   523,   689,     0,
       0,     0,   316,   840,   840,   836,   840,     0,     0,   464,
       0,     0,   770,   769,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,   285,   338,   387,     0,     0,     0,
       0,   322,   469,   475,   479,     0,     0,   797,   800,     0,
       0,     0,     0,   815,   803,     0,     0,     0,     0,     0,
       0,   249,   845,   842,   398,   286,     0,     0,     0,   300,
     480,   528,     0,   317,   824,   837,   828,   839,   843,   772,
     771,   844,   846,     0,   299,   298,   530,   297
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1397, -1397, -1397, -1397, -1397, -1397,   499,  1114, -1397, -1397,
   -1397,  1193, -1397, -1397, -1397,   695,  1156, -1397,  1062, -1397,
   -1397,  1121, -1397, -1397, -1397,  -201, -1397, -1397, -1397,  -198,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,   987,
   -1397, -1397,  1136,   -51, -1397, -1397,   518,   431,  -517,  -550,
    -764, -1397, -1397, -1397, -1396, -1397, -1397,  -209,  -360, -1397,
     288, -1397, -1322, -1397,  -104,  -218, -1397, -1397, -1397, -1397,
    -400,   -14, -1397, -1397, -1397, -1397, -1397,  -188,  -187,  -181,
   -1397,  -179, -1397, -1397,  1221, -1397,   282, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
    -428,  -152,   747,  -100, -1397,  -871,  -430, -1397,   743, -1397,
   -1397,  -363,  1160, -1397, -1397, -1397, -1388, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397, -1397,   761, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397,  -148,  -122,  -199,  -123,    11, -1397,
   -1397, -1397, -1397, -1397,   817, -1397,  -567, -1397, -1397,  -571,
   -1397, -1397,  -416,  -413,  -557, -1272, -1397,  -352, -1397, -1397,
    1195, -1397, -1397, -1397,   691,   793,    57, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397,    18,  -185, -1397,
     814, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,  -378,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397,  -206, -1397, -1397,
   -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397, -1397, -1397,   816,  -922,  -674,  -860, -1397, -1397, -1397,
   -1121,  -144, -1397, -1397, -1397, -1397, -1397, -1397, -1397, -1397,
   -1397,   105,  -465, -1397, -1397, -1397,   590, -1397, -1397, -1397,
   -1397, -1397, -1397,  -635, -1397,  -785,  -688, -1397
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    52,    17,   164,   170,   693,   500,
     149,   501,    99,    19,    20,    45,    46,    47,    88,    21,
      39,    40,   502,   503,  1183,  1184,   504,  1323,  1407,   505,
     506,   941,   507,   626,   508,   509,   510,   511,  1115,   171,
     172,    35,    36,    37,    66,    67,    68,    69,    22,   283,
     368,   210,    23,   111,   211,   112,   156,   671,   965,   513,
     369,   514,   817,  1467,  1071,   561,   921,  1398,   923,  1399,
     516,   517,   518,   628,   841,  1369,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   722,  1192,   949,   528,   529,
     874,  1376,   875,  1377,   877,  1378,   530,   844,  1372,   531,
     704,  1420,   532,  1198,  1199,   952,   673,   533,   779,   942,
     534,   638,   966,   536,   537,   538,   939,   539,  1178,  1470,
    1179,  1521,   540,  1039,  1354,   541,   705,  1337,  1532,  1339,
    1533,  1427,  1560,   543,   364,  1360,  1434,  1232,  1234,  1048,
     556,   788,  1495,  1536,   365,   366,   605,   812,   412,   610,
     814,   413,  1139,   420,   569,   383,   301,   302,   217,   297,
      78,   121,    25,   161,   370,    89,    90,   174,    91,    26,
      49,   115,   158,    27,   286,   553,   554,  1044,   373,   215,
     216,    76,   118,   375,    28,   159,   295,   421,   544,   293,
     347,   348,   804,   411,   349,   577,  1244,   797,   409,   350,
     570,  1237,   816,   575,  1242,   571,  1238,   572,  1239,   574,
    1241,   578,  1245,   579,  1361,   580,  1247,   581,  1362,   582,
    1249,   583,  1363,   584,  1252,   585,  1254,   606,    29,    95,
     177,   353,   607,    30,    96,   178,   354,   611,    31,    94,
     176,   352,   602,   545,   906,   907,   908,   920,  1161,  1155,
    1150,  1307,   546,   837,  1366,   838,  1367,   886,  1382,   883,
    1380,   909,   695,   547,   884,  1381,   910,   548,   845,  1373,
     881,  1379,   689,   696,   549,  1511,   929,   550
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    70,   285,   565,   613,   621,   535,   869,   792,   512,
     637,   913,   688,   219,   813,   723,   933,   811,   934,   351,
    1128,  1187,  -118,    84,   557,  1297,  1082,   589,    53,   806,
     899,   808,   599,   810,    54,  1135,   674,   675,   636,   787,
     346,   946,  1062,  1437,  1064,   417,  1066,  1146,  1156,    70,
      70,    70,   151,  1158,    32,    33,  1162,  1415,    85,   616,
    1169,   746,   666,  1173,   749,   750,   104,   105,   106,   706,
     726,   727,  1358,    50,     2,   724,  1194,   591,   512,   362,
       3,   284,  1520,   714,   218,   899,  1195,   122,   123,  1356,
      70,    70,    70,    70,   616,   107,   299,   683,   685,  1535,
     299,    58,   512,     4,  1138,     5,    92,     6,   947,  1233,
      98,   162,   163,     7,   300,   656,   426,   427,   300,    51,
     108,   618,   566,     8,   410,   725,   399,   715,    59,     9,
      61,  1359,   567,  1554,   749,   750,   437,   150,  1196,   363,
    1562,  1484,   442,  1197,  1517,   418,   777,   778,   139,   376,
     138,   515,    10,   400,   401,   891,   618,  1145,  1091,    62,
     818,   424,   948,   419,  1418,   738,   739,   298,  1086,   793,
      71,  1390,   422,   746,    11,    12,   749,   750,   751,   456,
     457,  1113,   346,   752,   568,   616,   590,   212,   900,   165,
     166,    98,   617,   785,  1122,  1138,   346,  1125,   213,    86,
    1290,   617,   284,    34,    77,   423,   846,   848,    38,   512,
      87,   345,   459,   460,   617,   402,   777,   778,   566,   403,
     515,   835,   617,  1296,   786,   346,   617,   346,   567,   617,
      63,   882,   640,   124,   885,   847,   592,    58,   125,   824,
     126,   726,   727,   127,   515,   616,   657,   618,  1059,   616,
     399,    79,    58,    13,   593,  1392,  1283,   616,   777,   778,
     594,   512,  1060,   868,    59,   888,   616,  1456,  1458,   475,
     476,   477,    14,   791,    80,   512,  1547,   400,   401,    59,
     568,   404,  1292,    15,   128,   405,   887,   945,   406,   488,
     346,   346,  1145,    58,   953,   399,   284,  1033,  1034,    64,
     418,  1145,   935,   407,   627,   512,    81,   618,    65,   408,
     617,   618,   587,   619,    80,  1324,   620,  1145,   419,   618,
      59,   496,   400,   401,  1145,  1190,  1035,  1145,   618,  1145,
     398,   588,   498,   670,  1264,   167,   738,   739,  1036,   402,
     168,   180,   169,   403,   746,   127,   748,   749,   750,   751,
    1537,   515,  1042,   345,   752,  1147,  1148,  1538,   555,   551,
    1543,    82,  1544,  1067,   795,   796,   798,   345,   800,   801,
     950,   552,   805,   223,   807,  1152,   809,  1068,  1153,  1037,
    1038,  1056,   600,  1149,   402,   346,   346,   346,   403,   346,
     346,   410,  1050,   346,   601,   346,   345,   346,   345,   346,
     224,  1188,  1200,   515,  1411,   404,  1154,    72,    73,   405,
      74,  1127,   406,   834,  1085,   849,   839,   515,  1079,   849,
     345,    41,    42,    43,  1327,  1186,    48,   407,  1189,   777,
     778,  1389,   512,   408,   346,   410,  1336,   872,    75,  1053,
      80,   107,  1226,  1395,   849,  1144,  1263,   515,   103,  1327,
     404,  1329,    44,  1165,   405,  1334,   346,   406,   849,  1080,
    1081,   345,   345,  1170,   849,   377,  1229,  1090,  1291,  1136,
    1325,  1425,   407,   633,  1328,   608,   706,  1144,   408,   346,
    1335,  -719,   922,  1303,   706,  1431,  -719,   609,   641,    53,
     658,  1454,   634,   726,   727,    54,  1116,  1117,  1253,  1119,
    1251,   940,  1482,  1121,  -719,  1123,  1124,   642,  1126,   659,
     515,   515,   515,   515,   515,   515,   515,   515,   661,   109,
    1384,    58,  1069,   515,   515,   110,  1255,   287,    98,   515,
     935,   288,  1205,  1206,  1207,   936,  1070,   662,   515,   515,
    1256,   957,   961,   515,   515,   515,    97,   515,    59,  -726,
     289,   290,   291,   292,  -726,  1405,   345,   345,   345,   113,
     345,   345,  1255,  1255,   345,   114,   345,  1001,   345,   116,
     345,   131,  -726,   937,   515,   117,  1310,  1311,  1284,  -733,
      87,   100,   101,   102,  -733,   399,   736,   737,   738,   739,
     740,   996,  -396,   743,   744,   745,   746,  -396,   748,   749,
     750,   751,  -733,   997,   781,   345,   752,   137,   754,   755,
    1072,   782,   400,   401,   758,  -396,   760,   843,  1075,  1370,
    1216,  1076,   152,   153,   154,   155,   410,   345,   133,  1319,
    1054,   346,  1217,  1371,  1221,  1083,  1375,  1180,  1089,   346,
    1047,   119,   346,  1294,   840,  1481,  1222,   120,  1181,  1182,
     345,   141,   346,   346,  1261,  1295,   346,   142,  1110,   346,
     346,   900,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,   284,  1290,   402,   498,   670,  1515,   403,  1516,
     849,   777,   778,  1518,  1519,   380,   136,  1444,   381,   346,
     346,   382,   346,   938,  1159,  1152,   346,  1160,   346,   346,
     898,   346,   911,  1304,   915,  1230,  1305,  1286,  1387,  1306,
    1394,  1231,   927,  1397,   849,  1072,   849,  1072,  1400,   849,
    1401,   931,   362,   140,   849,   410,   849,   218,  1302,  1057,
     144,   410,  1111,  1270,  1309,  1058,   346,   148,   346,   410,
     404,  1313,   145,  1061,   405,  1315,  1112,   406,   410,   410,
     410,   410,  1063,  1065,  1262,  1267,   399,   410,   104,   105,
     106,  1490,   407,   410,  1499,  1500,  1414,  1534,   408,   382,
    1483,   146,    70,   382,  1409,  1507,  1508,    41,    42,    43,
    1342,   134,   135,   400,   401,   162,   163,   924,   925,   160,
     726,   727,  1351,   220,   221,   222,   917,   918,   919,   104,
    1318,   106,   345,    55,    56,    57,   220,   221,  1504,   535,
     345,   147,   512,   345,   157,  1540,  1541,  1166,  1542,  1167,
      85,   175,   179,   345,   345,   104,   214,   345,   218,   225,
     345,   345,   226,   227,   228,   280,  1214,   281,   282,   284,
     294,   296,   355,   356,   357,   402,   358,   359,   360,   403,
    1240,   361,   367,   372,   379,   371,   374,  1386,  1486,   378,
     345,   345,   384,   345,   385,   387,   386,   345,   414,   345,
     345,   346,   345,   415,   388,   389,   410,   390,   391,  1393,
    1322,   392,   346,   736,   737,   738,   739,   563,   393,   726,
     727,   394,   395,   746,   396,   748,   749,   750,   751,   397,
     564,   416,   558,   752,   576,   754,   755,   345,   614,   345,
     615,   404,   597,   623,   651,   405,   631,  1259,   406,   624,
     632,   645,   647,   648,   399,   650,   346,  1528,  1529,   653,
     649,   697,  1464,   407,   652,  1468,   660,   818,   663,   408,
     664,   676,   692,   677,   708,   678,   709,   679,   711,   721,
     716,   400,   401,   717,   515,   707,   680,   718,   719,  1421,
     681,   720,    15,   780,   772,   773,   774,   775,   776,   783,
    1556,  1320,  1457,   656,   790,   690,   803,   608,   777,   778,
     802,   822,   736,   737,   738,   739,   740,   819,   827,   743,
     744,   745,   746,   346,   748,   749,   750,   751,   823,   832,
     825,   826,   752,   828,   754,   755,   829,   833,   726,   727,
     758,   759,   760,   402,  1503,   842,   764,   403,   849,   724,
    1506,   867,   873,  1497,   879,   878,   928,   890,   930,   932,
     951,  1046,   992,  1049,  1073,  1055,  1485,  1078,   818,  1144,
    1137,  1151,   345,  1175,  1174,   637,  1176,  1191,  1164,  1364,
    1171,  1201,  1368,   345,  1177,  1203,   766,  1202,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,  1204,  1208,
     346,  1209,  1219,  1220,  1224,  1228,  1233,   777,   778,   404,
    1235,  1243,  1246,   405,  1248,  1260,   406,  1250,  1257,  1265,
    1269,  1272,  1271,  1285,   848,  1278,  1298,   345,  1548,  1281,
    1330,   407,  1331,   738,   739,  1527,  1301,   408,  1308,   399,
    1332,   746,  1406,   748,   749,   750,   751,  1333,  1344,  1345,
    1347,   752,  1352,  1355,  1365,  1408,  1383,  1419,  1391,  1396,
     542,  1412,  1410,  1423,  1413,  1417,   400,   401,   399,   562,
    1433,  1438,  1439,  1440,  1471,  1442,  1501,  1443,   573,  1448,
    1510,   399,   346,  1449,   346,  1545,  1450,  1546,   586,  1466,
    1549,  1498,  1550,  1552,   345,   400,   401,  1505,   596,  1526,
    1551,   726,   727,  1553,  1564,  1419,  1561,   612,   400,   401,
     399,  1565,   772,   773,   774,   775,   776,  1566,  1568,   625,
    1571,  1572,   926,   129,    18,   173,   777,   778,   402,   639,
      83,   566,   403,   132,   644,  1472,   646,   400,   401,  1475,
     303,   567,   672,   672,   672,   655,   130,  1045,  1172,  1476,
    1477,  1491,    24,   667,   668,   669,  1478,   402,  1479,   346,
    1193,   403,  1453,   399,  1416,  1435,  1496,  1436,   687,   622,
     402,   345,   346,  1357,   403,   784,   691,    93,   629,   687,
     630,  1463,   698,   699,   700,   701,   702,  1300,   713,     0,
     400,   401,   712,   568,   404,   710,   738,   739,   405,   402,
       0,   406,  1525,   403,   746,   914,   748,   749,   750,   751,
       0,     0,     0,     0,   752,     0,   407,     0,     0,   712,
     399,     0,   408,   404,     0,     0,     0,   405,   346,  1268,
     406,     0,     0,     0,   399,     0,   404,     0,     0,     0,
     405,     0,  1274,   406,     0,   407,     0,   400,   401,     0,
       0,   408,   402,   345,     0,   345,   403,     0,   407,     0,
     794,   400,   401,     0,   408,   404,     0,     0,     0,   405,
       0,  1275,   406,     0,     0,     0,     0,   774,   775,   776,
       0,     0,     0,  1576,     0,     0,     0,   407,   821,   777,
     778,     0,     0,   408,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   831,     0,     0,     0,   402,
       0,     0,   836,   403,     0,     0,   712,     0,   404,     0,
       0,     0,   405,   402,  1276,   406,   639,   403,     0,     0,
     345,     0,     0,   687,   870,     0,   871,     0,     0,     0,
     407,   876,     0,   345,   712,     0,   408,   880,     0,     0,
       0,     0,     0,     0,     0,     0,   889,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   892,   893,   894,
     895,   896,   897,     0,   905,   404,   905,     0,     0,   405,
       0,  1277,   406,     0,     0,     0,   938,     0,     0,   404,
       0,     0,     0,   405,     0,  1279,   406,   407,     0,   345,
       0,   672,     0,   408,     0,     0,     0,     0,     0,     0,
       0,   407,     0,     0,   726,   727,     0,   408,     0,   967,
     969,   971,   973,   975,   977,   979,   981,   982,   983,     0,
       0,   984,   986,   988,   989,   990,   991,   938,   994,   995,
       0,   998,   999,  1000,  1002,  1003,  1004,  1006,  1008,  1009,
    1010,  1011,  1013,  1015,  1017,  1018,  1020,  1022,  1023,  1024,
    1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,     0,  1040,
       0,   712,   399,  1043,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1051,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     401,     0,     0,     0,     0,     0,     0,   736,   737,   738,
     739,   740,     0,   712,   743,   744,   745,   746,     0,   748,
     749,   750,   751,     0,     0,     0,     0,   752,     0,   754,
     755,     0,     0,     0,  1088,   758,   759,   760,     0,     0,
    1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,
    1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,     0,     0,
       0,   402,     0,     0,     0,   403,   712,     0,     0,     0,
       0,     0,     0,   726,   727,     0,   672,     0,     0,     0,
       0,   766,     0,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,   698,  1141,     0,     0,     0,     0,     0,
       0,     0,   777,   778,     0,     0,     0,  1163,     0,   687,
       0,   726,   727,     0,     0,     0,  1168,     0,     0,   687,
       0,     0,     0,  1092,     0,     0,     0,   404,     0,     0,
       0,   405,  1185,  1280,   406,     0,     0,     0,     0,   672,
     672,   672,     0,     0,   712,     0,   712,     0,   712,   407,
     712,     0,   712,     0,   712,   408,   712,     0,   712,     0,
       0,     0,     0,   712,     0,   712,   736,   737,   738,   739,
     740,   712,     0,   743,   744,   745,   746,     0,   748,   749,
     750,   751,     0,   712,     0,   712,   752,     0,   754,   755,
     712,     0,   712,     0,   712,     0,     0,   712,     0,     0,
       0,     0,     0,   399,   736,   737,   738,   739,   740,     0,
       0,   743,   744,   745,   746,     0,   748,   749,   750,   751,
       0,     0,     0,     0,   752,  1218,   754,   755,   712,  1223,
     400,   401,   758,   759,   760,     0,     0,     0,   764,     0,
       0,     0,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   777,   778,     0,     0,     0,     0,     0,     0,     0,
    1258,     0,     0,     0,     0,     0,     0,     0,   766,     0,
     767,   768,   769,   770,   771,   772,   773,   774,   775,   776,
       0,     0,   402,     0,     0,     0,   403,     0,     0,   777,
     778,     0,     0,     0,   498,   670,  1273,     0,     0,     0,
       0,     0,     0,     0,     0,   -68,     0,     0,     0,   968,
     970,   972,   974,   976,   978,   980,   726,   727,  1287,  1288,
    1289,   985,   987,     0,  1293,     0,     0,   993,     0,     0,
       0,     0,  1299,   905,     0,     0,  1005,  1007,     0,     0,
       0,  1012,  1014,  1016,     0,  1019,     0,     0,   404,     0,
       0,     0,   405,   542,  1282,   406,     0,  1316,  1317,     0,
       0,     0,     0,     0,  1321,   726,   727,     0,     0,  1092,
     407,     0,     0,     0,     0,     0,   408,     0,     0,     0,
       0,     0,     0,  1338,     0,  1340,     0,     0,     0,     0,
    1343,     0,     0,     0,     0,     0,  1346,     0,     0,     0,
    1349,   728,   729,   730,   731,   732,   733,   734,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   750,   751,     0,     0,     0,     0,   752,
     753,   754,   755,   756,   757,     0,     0,   758,   759,   760,
     761,   762,   763,   764,     0,     0,     0,     0,     0,     0,
       0,     0,   687,     0,     0,     0,     0,     0,   736,   737,
     738,   739,   740,     0,     0,   743,   744,   745,   746,     0,
     748,   749,   750,   751,     0,     0,     0,     0,   752,     0,
     754,   755,   765,   766,     0,   767,   768,   769,   770,   771,
     772,   773,   774,   775,   776,     0,     0,     0,     0,     0,
     726,   727,     0,     0,   777,   778,     0,     0,     0,   498,
     670,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1428,     0,  1429,     0,
       0,     0,     0,  1432,     0,   768,   769,   770,   771,   772,
     773,   774,   775,   776,     0,     0,     0,     0,     0,     0,
       0,     0,  1441,   777,   778,     0,   399,  1445,  1446,  1447,
       0,     0,     0,     0,  1452,     0,     0,     0,  1455,     0,
       0,     0,  1459,  1460,  1461,  1462,     0,     0,   687,  1465,
       0,   687,  1469,   400,   401,     0,     0,     0,  1480,     0,
       0,     0,     0,   736,   737,   738,   739,   740,     0,  1488,
     743,   744,   745,   746,     0,   748,   749,   750,   751,     0,
       0,     0,     0,   752,     0,   754,   755,     0,     0,     0,
       0,   758,     0,     0,   687,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1512,  1513,     0,
       0,     0,     0,     0,     0,   402,     0,     0,     0,   403,
       0,     0,     0,     0,     0,  1524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1530,  1531,   767,
     768,   769,   770,   771,   772,   773,   774,   775,   776,     0,
       0,     0,  1539,     0,     0,   399,     0,     0,   777,   778,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1555,     0,     0,     0,     0,  1557,
    1558,   404,   400,   401,     0,   405,     0,  1374,   406,     0,
       0,     0,     0,     0,     0,     0,  1567,   425,  1569,  1570,
     426,   427,     3,   407,   428,   429,   430,  1573,   431,   408,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,   438,   439,   440,   441,     0,   442,     0,     0,     0,
       0,     0,     0,   443,   444,     0,     0,   445,     0,   446,
     447,     0,     0,   448,   402,     8,   449,   450,   403,   451,
     452,     0,     0,   453,   454,     0,     0,     0,     0,     0,
     455,     0,     0,   456,   457,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   458,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,   459,   460,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   463,   464,     0,   405,     0,  1404,   406,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   466,   467,     0,
       0,     0,   407,     0,     0,     0,    58,     0,   408,     0,
       0,     0,     0,     0,   468,   469,   470,   471,   472,     0,
     473,     0,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,    59,   485,   486,   487,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     491,   492,   493,     0,    14,     0,     0,   494,   495,     0,
       0,     0,     0,     0,     0,   496,     0,   497,   425,   498,
     499,   426,   427,     3,     0,   428,   429,   430,     0,   431,
       0,   432,   433,   434,   435,   436,     0,     0,     0,     0,
       0,   437,   438,   439,   440,   441,     0,   442,     0,     0,
       0,     0,     0,     0,   443,   444,     0,     0,   445,     0,
     446,   447,     0,     0,   448,     0,     8,   449,   450,     0,
     451,   452,     0,     0,   453,   454,     0,     0,     0,     0,
       0,   455,     0,     0,   456,   457,   310,   311,   312,     0,
     314,   315,   316,   317,   318,   458,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   459,   460,   461,
     462,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   465,   466,   467,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   468,   469,   470,   471,   472,
     399,   473,     0,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,    59,   485,   486,   487,     0,     0,
       0,     0,     0,     0,   488,  1314,   490,   400,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,     0,    14,     0,     0,   494,   495,
       0,     0,   426,   427,     0,     0,   496,     0,   497,     0,
     498,   499,   432,   433,   434,   435,   436,     0,     0,     0,
       0,     0,   437,     0,   439,     0,     0,     0,   442,     0,
     399,     0,     0,     0,     0,     0,   444,     0,     0,   402,
       0,     0,   447,   403,     0,   448,     0,     0,   449,     0,
     899,     0,   452,     0,     0,     0,     0,   400,   401,     0,
       0,     0,   559,     0,     0,   456,   457,   310,   311,   312,
       0,   314,   315,   316,   317,   318,   458,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     0,   332,
     333,   334,     0,     0,   337,   338,   339,   340,   459,   460,
     461,     0,     0,     0,     0,   404,     0,     0,     0,   405,
       0,  1489,   406,   463,   464,     0,     0,     0,     0,   402,
       0,     0,     0,   403,     0,     0,     0,   407,     0,     0,
       0,     0,     0,   408,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   470,   471,
     472,     0,   473,   900,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   901,   560,   486,   487,     0,
       0,     0,     0,     0,     0,   488,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,     0,     0,     0,   405,
       0,     0,   902,   492,   493,     0,    14,     0,     0,   494,
     495,     0,     0,     0,   426,   427,     0,   903,     0,   904,
       0,   498,   499,   408,   432,   433,   434,   435,   436,     0,
       0,     0,     0,     0,   437,     0,   439,     0,     0,     0,
     442,     0,   399,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,   447,     0,     0,   448,     0,     0,
     449,     0,     0,     0,   452,     0,     0,     0,     0,   400,
     401,     0,     0,     0,   559,     0,     0,   456,   457,   310,
     311,   312,     0,   314,   315,   316,   317,   318,   458,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       0,   332,   333,   334,     0,     0,   337,   338,   339,   340,
     459,   460,   461,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   463,   464,     0,     0,     0,
       0,   402,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   468,   469,
     470,   471,   472,     0,   473,   900,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   901,   560,   486,
     487,     0,     0,     0,     0,     0,     0,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,     0,     0,
       0,   405,     0,     0,   902,   492,   493,     0,    14,     0,
       0,   494,   495,     0,     0,     0,   426,   427,     0,   903,
       0,   912,     0,   498,   499,   408,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,   591,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,   488,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   592,     0,     0,   491,   492,   493,     0,
      14,     0,     0,   494,   495,     0,     0,     0,   426,   427,
       0,  1140,     0,   497,     0,   498,   499,   594,   432,   433,
     434,   435,   436,     0,     0,     0,     0,     0,   437,     0,
     439,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,     0,   444,     0,     0,     0,     0,     0,   447,     0,
       0,   448,     0,     0,   449,     0,     0,     0,   452,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
       0,   456,   457,   310,   311,   312,     0,   314,   315,   316,
     317,   318,   458,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,     0,   332,   333,   334,     0,     0,
     337,   338,   339,   340,   459,   460,   461,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   463,
     464,     0,     0,     0,     0,     0,     0,     0,   635,     0,
       0,     0,     0,     0,   465,   466,   467,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   470,   471,   472,     0,   473,     0,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,    59,   560,   486,   487,     0,     0,     0,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   491,   492,
     493,     0,    14,     0,     0,   494,   495,     0,     0,   426,
     427,     0,     0,   496,     0,   497,     0,   498,   499,   432,
     433,   434,   435,   436,     0,     0,     0,     0,     0,   437,
       0,   439,     0,     0,     0,   442,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,   447,
       0,     0,   448,     0,     0,   449,     0,     0,     0,   452,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   559,
       0,     0,   456,   457,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   458,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   459,   460,   461,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     463,   464,     0,     0,     0,     0,     0,     0,     0,   665,
       0,     0,     0,     0,     0,   465,   466,   467,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,   471,   472,     0,   473,
       0,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,    59,   560,   486,   487,     0,     0,     0,     0,
       0,     0,   488,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,     0,    14,     0,     0,   494,   495,     0,     0,
     426,   427,     0,     0,   496,     0,   497,     0,   498,   499,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,  1473,   439,   440,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
     447,     0,     0,   448,     0,     0,   449,   450,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   456,   457,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   458,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,   459,   460,   461,  1474,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   463,   464,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,   471,   472,     0,
     473,     0,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,    59,   560,   486,   487,     0,     0,     0,
       0,     0,     0,   488,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     491,   492,   493,     0,    14,     0,     0,   494,   495,     0,
       0,   426,   427,     0,     0,   496,     0,   497,     0,   498,
     499,   432,   433,   434,   435,   436,     0,     0,     0,     0,
       0,   437,     0,   439,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,   444,     0,     0,     0,     0,
       0,   447,     0,     0,   448,     0,     0,   449,     0,     0,
       0,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   559,     0,     0,   456,   457,   310,   311,   312,     0,
     314,   315,   316,   317,   318,   458,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   459,   460,   461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   465,   466,   467,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   468,   469,   470,   471,   472,
       0,   473,     0,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,    59,   560,   486,   487,     0,     0,
       0,     0,     0,     0,   488,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,     0,    14,     0,     0,   494,   495,
       0,     0,   426,   427,     0,     0,   496,     0,   497,     0,
     498,   499,   432,   433,   434,   435,   436,     0,     0,     0,
       0,     0,   437,     0,   439,     0,     0,     0,   442,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,   447,     0,     0,   448,     0,     0,   449,     0,
       0,     0,   452,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,     0,   456,   457,   310,   311,   312,
       0,   314,   315,   316,   317,   318,   458,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     0,   332,
     333,   334,     0,     0,   337,   338,   339,   340,   459,   460,
     461,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   463,   464,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   962,   963,
     964,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   470,   471,
     472,     0,   473,     0,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,    59,   560,   486,   487,     0,
       0,     0,     0,     0,     0,   488,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   491,   492,   493,     0,    14,     0,     0,   494,
     495,     0,     0,   426,   427,     0,     0,   496,     0,   497,
       0,   498,   499,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,   437,     0,   439,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,   447,     0,     0,   448,     0,     0,   449,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   559,     0,     0,   456,   457,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   458,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   459,
     460,   461,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   463,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   468,   469,   470,
     471,   472,     0,   473,   900,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   901,   560,   486,   487,
       0,     0,     0,     0,     0,     0,   488,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,     0,    14,     0,     0,
     494,   495,     0,     0,   426,   427,     0,     0,   943,     0,
     497,   944,   498,   499,   432,   433,   434,   435,   436,     0,
       0,     0,     0,     0,   437,     0,   439,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,   447,     0,     0,   448,     0,     0,
     449,     0,     0,     0,   452,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   456,   457,   310,
     311,   312,     0,   314,   315,   316,   317,   318,   458,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       0,   332,   333,   334,     0,     0,   337,   338,   339,   340,
     459,   460,   461,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   463,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   468,   469,
     470,   471,   472,     0,   473,   900,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   901,   560,   486,
     487,     0,     0,     0,     0,     0,     0,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   491,   492,   493,     0,    14,     0,
       0,   494,   495,     0,     0,     0,     0,   426,   427,   496,
       0,   497,  1084,   498,   499,   703,     0,   432,   433,   434,
     435,   436,     0,     0,     0,     0,     0,   437,     0,   439,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,   444,     0,     0,     0,     0,     0,   447,     0,     0,
     448,     0,     0,   449,     0,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,     0,
     456,   457,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   458,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   459,   460,   461,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   470,   471,   472,     0,   473,     0,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
      59,   560,   486,   487,     0,     0,     0,     0,     0,     0,
     488,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
       0,    14,     0,     0,   494,   495,     0,     0,     0,     0,
     426,   427,   496,   595,   497,     0,   498,   499,   703,     0,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,     0,   439,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
     447,     0,     0,   448,     0,     0,   449,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   456,   457,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   458,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,   459,   460,   461,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   463,   464,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,   471,   472,     0,
     473,   900,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   901,   560,   486,   487,     0,     0,     0,
       0,     0,     0,   488,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     491,   492,   493,     0,    14,     0,     0,   494,   495,     0,
       0,     0,     0,   426,   427,   496,     0,   497,     0,   498,
     499,   703,     0,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,   437,     0,   439,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,   447,     0,     0,   448,     0,     0,   449,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   559,     0,     0,   456,   457,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   458,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   459,
     460,   461,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   463,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   468,   469,   470,
     471,   472,     0,   473,     0,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,    59,   560,   486,   487,
       0,     0,     0,     0,     0,     0,   488,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,     0,    14,     0,     0,
     494,   495,     0,     0,   426,   427,     0,     0,   496,   819,
     497,     0,   498,   499,   432,   433,   434,   435,   436,     0,
       0,     0,     0,     0,   437,     0,   439,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,   447,     0,     0,   448,     0,     0,
     449,     0,     0,     0,   452,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   456,   457,   310,
     311,   312,     0,   314,   315,   316,   317,   318,   458,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       0,   332,   333,   334,     0,     0,   337,   338,   339,   340,
     459,   460,   461,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   463,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   468,   469,
     470,   471,   472,     0,   473,     0,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,    59,   560,   486,
     487,     0,     0,     0,     0,     0,     0,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   491,   492,   493,     0,    14,     0,
       0,   494,   495,     0,     0,   426,   427,     0,     0,   496,
     595,   497,     0,   498,   499,   432,   433,   434,   435,   436,
       0,     0,     0,     0,     0,   437,     0,   439,     0,     0,
       0,   442,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,   447,     0,     0,   448,     0,
       0,   449,     0,     0,     0,   452,     0,     0,     0,     0,
       0,   643,     0,     0,     0,   559,     0,     0,   456,   457,
     310,   311,   312,     0,   314,   315,   316,   317,   318,   458,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   459,   460,   461,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   463,   464,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   470,   471,   472,     0,   473,     0,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,    59,   560,
     486,   487,     0,     0,     0,     0,     0,     0,   488,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,     0,    14,
       0,     0,   494,   495,     0,     0,   426,   427,     0,     0,
     496,     0,   497,     0,   498,   499,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,   654,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,   488,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   491,   492,   493,     0,
      14,     0,     0,   494,   495,     0,     0,     0,     0,   426,
     427,   496,     0,   497,     0,   498,   499,   686,     0,   432,
     433,   434,   435,   436,     0,     0,     0,     0,     0,   437,
       0,   439,     0,     0,     0,   442,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,   447,
       0,     0,   448,     0,     0,   449,     0,     0,     0,   452,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   559,
       0,     0,   456,   457,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   458,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   459,   460,   461,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     463,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,   471,   472,     0,   473,
       0,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,    59,   560,   486,   487,     0,     0,     0,     0,
       0,     0,   488,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,     0,    14,     0,     0,   494,   495,     0,     0,
     426,   427,     0,     0,   496,     0,   497,     0,   498,   499,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,     0,   439,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
     447,     0,     0,   448,     0,     0,   449,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   456,   457,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   458,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,   459,   460,   461,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   463,   464,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,   471,   472,     0,
     473,     0,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,    59,   560,   486,   487,     0,     0,     0,
       0,     0,     0,   488,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   690,     0,
     491,   492,   493,     0,    14,     0,     0,   494,   495,     0,
       0,     0,     0,   426,   427,   496,     0,   497,     0,   498,
     499,   694,     0,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,   437,     0,   439,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,   447,     0,     0,   448,     0,     0,   449,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   559,     0,     0,   456,   457,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   458,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   459,
     460,   461,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   463,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   468,   469,   470,
     471,   472,     0,   473,     0,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,    59,   560,   486,   487,
       0,     0,     0,     0,     0,     0,   488,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,     0,    14,     0,     0,
     494,   495,     0,     0,     0,     0,   426,   427,   496,     0,
     497,     0,   498,   499,   703,     0,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,   488,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   491,   492,   493,     0,
      14,     0,     0,   494,   495,     0,     0,   426,   427,     0,
       0,   496,     0,   497,     0,   498,   499,   432,   433,   434,
     435,   436,     0,     0,  1021,     0,     0,   437,     0,   439,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,   444,     0,     0,     0,     0,     0,   447,     0,     0,
     448,     0,     0,   449,     0,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,     0,
     456,   457,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   458,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   459,   460,   461,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   470,   471,   472,     0,   473,     0,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
      59,   560,   486,   487,     0,     0,     0,     0,     0,     0,
     488,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
       0,    14,     0,     0,   494,   495,     0,     0,   426,   427,
       0,     0,   496,     0,   497,     0,   498,   499,   432,   433,
     434,   435,   436,     0,     0,     0,     0,     0,   437,     0,
     439,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,     0,   444,     0,     0,     0,     0,     0,   447,     0,
       0,   448,     0,     0,   449,     0,     0,     0,   452,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
       0,   456,   457,   310,   311,   312,     0,   314,   315,   316,
     317,   318,   458,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,     0,   332,   333,   334,     0,     0,
     337,   338,   339,   340,   459,   460,   461,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   463,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   470,   471,   472,     0,   473,     0,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,    59,   560,   486,   487,     0,     0,     0,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   491,   492,
     493,     0,    14,     0,     0,   494,   495,     0,     0,   426,
     427,     0,     0,   496,     0,   497,  1041,   498,   499,   432,
     433,   434,   435,   436,     0,     0,     0,     0,     0,   437,
       0,   439,     0,     0,     0,   442,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,   447,
       0,     0,   448,     0,     0,   449,     0,     0,     0,   452,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   559,
       0,     0,   456,   457,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   458,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   459,   460,   461,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     463,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,   471,   472,     0,   473,
       0,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,    59,   560,   486,   487,     0,     0,     0,     0,
       0,     0,   488,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1087,     0,   491,
     492,   493,     0,    14,     0,     0,   494,   495,     0,     0,
     426,   427,     0,     0,   496,     0,   497,     0,   498,   499,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,     0,   439,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
     447,     0,     0,   448,     0,     0,   449,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   456,   457,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   458,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,   459,   460,   461,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   463,   464,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,   471,   472,     0,
     473,     0,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,    59,   560,   486,   487,     0,     0,     0,
       0,     0,     0,   488,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     491,   492,   493,     0,    14,     0,     0,   494,   495,     0,
       0,   426,   427,     0,     0,   496,     0,   497,  1142,   498,
     499,   432,   433,   434,   435,   436,     0,     0,     0,     0,
       0,   437,     0,   439,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,   444,     0,     0,     0,     0,
       0,   447,     0,     0,   448,     0,     0,   449,     0,     0,
       0,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   559,     0,     0,   456,   457,   310,   311,   312,     0,
     314,   315,   316,   317,   318,   458,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   459,   460,   461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   468,   469,   470,   471,   472,
       0,   473,     0,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,    59,   560,   486,   487,     0,     0,
       0,     0,     0,     0,   488,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,     0,    14,     0,     0,   494,   495,
       0,     0,   426,   427,     0,     0,   496,     0,   497,  1157,
     498,   499,   432,   433,   434,   435,   436,     0,     0,     0,
       0,     0,   437,     0,   439,     0,     0,     0,   442,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,   447,     0,     0,   448,     0,     0,   449,     0,
       0,     0,   452,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,     0,   456,   457,   310,   311,   312,
       0,   314,   315,   316,   317,   318,   458,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,     0,   332,
     333,   334,     0,     0,   337,   338,   339,   340,   459,   460,
     461,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   463,   464,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   470,   471,
     472,     0,   473,     0,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,    59,   560,   486,   487,     0,
       0,     0,     0,     0,     0,   488,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   491,   492,   493,     0,    14,     0,     0,   494,
     495,     0,     0,   426,   427,     0,     0,   496,     0,   497,
    1341,   498,   499,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,   437,     0,   439,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,   447,     0,     0,   448,     0,     0,   449,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   559,     0,     0,   456,   457,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   458,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   459,
     460,   461,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   463,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   468,   469,   470,
     471,   472,     0,   473,     0,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,    59,   560,   486,   487,
       0,     0,     0,     0,     0,     0,   488,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,     0,    14,     0,     0,
     494,   495,     0,     0,   426,   427,     0,     0,   496,     0,
     497,  1350,   498,   499,   432,   433,   434,   435,   436,     0,
       0,     0,     0,     0,   437,     0,   439,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,   447,     0,     0,   448,     0,     0,
     449,     0,     0,     0,   452,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   456,   457,   310,
     311,   312,     0,   314,   315,   316,   317,   318,   458,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
       0,   332,   333,   334,     0,     0,   337,   338,   339,   340,
     459,   460,   461,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   463,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   468,   469,
     470,   471,   472,     0,   473,     0,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,    59,   560,   486,
     487,     0,     0,     0,     0,     0,     0,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   491,   492,   493,     0,    14,     0,
       0,   494,   495,     0,     0,   426,   427,     0,     0,   496,
       0,   497,  1385,   498,   499,   432,   433,   434,   435,   436,
       0,     0,     0,     0,     0,   437,     0,   439,     0,     0,
       0,   442,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,   447,     0,     0,   448,     0,
       0,   449,     0,     0,     0,   452,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   559,     0,     0,   456,   457,
     310,   311,   312,     0,   314,   315,   316,   317,   318,   458,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   459,   460,   461,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   463,   464,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   470,   471,   472,     0,   473,     0,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,    59,   560,
     486,   487,     0,     0,     0,     0,     0,     0,   488,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,     0,    14,
       0,     0,   494,   495,     0,     0,   426,   427,     0,     0,
     496,     0,   497,  1451,   498,   499,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,  1487,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,   488,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   491,   492,   493,     0,
      14,     0,     0,   494,   495,     0,     0,   426,   427,     0,
       0,   496,     0,   497,     0,   498,   499,   432,   433,   434,
     435,   436,     0,     0,     0,     0,     0,   437,     0,   439,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,   444,     0,     0,     0,     0,     0,   447,     0,     0,
     448,     0,     0,   449,     0,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,     0,
     456,   457,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   458,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   459,   460,   461,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   463,   464,
       0,     0,     0,     0,     0,     0,     0,  1522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   470,   471,   472,     0,   473,     0,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
      59,   560,   486,   487,     0,     0,     0,     0,     0,     0,
     488,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
       0,    14,     0,     0,   494,   495,     0,     0,   426,   427,
       0,     0,   496,     0,   497,     0,   498,   499,   432,   433,
     434,   435,   436,     0,     0,     0,     0,     0,   437,     0,
     439,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,     0,   444,     0,     0,     0,     0,     0,   447,     0,
       0,   448,     0,     0,   449,     0,     0,     0,   452,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
       0,   456,   457,   310,   311,   312,     0,   314,   315,   316,
     317,   318,   458,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,     0,   332,   333,   334,     0,     0,
     337,   338,   339,   340,   459,   460,   461,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   463,
     464,     0,     0,     0,     0,     0,     0,     0,  1523,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   470,   471,   472,     0,   473,     0,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,    59,   560,   486,   487,     0,     0,     0,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   491,   492,
     493,     0,    14,     0,     0,   494,   495,     0,     0,   426,
     427,     0,     0,   496,     0,   497,     0,   498,   499,   432,
     433,   434,   435,   436,     0,     0,     0,     0,     0,   437,
       0,   439,     0,     0,     0,   442,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,   447,
       0,     0,   448,     0,     0,   449,     0,     0,     0,   452,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   559,
       0,     0,   456,   457,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   458,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   459,   460,   461,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     463,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,   471,   472,     0,   473,
       0,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,    59,   560,   486,   487,     0,     0,     0,     0,
       0,     0,   488,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,     0,    14,     0,     0,   494,   495,     0,     0,
     426,   427,     0,     0,   496,     0,   497,     0,   498,   499,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,     0,   439,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
     447,     0,     0,   448,     0,     0,   449,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   456,   457,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   458,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,   459,   460,   461,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   463,   464,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,   471,   472,     0,
     473,     0,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,    59,   560,   486,   487,     0,   682,     0,
       0,     0,     0,   488,   304,     0,     0,     0,     0,     0,
     305,     0,     0,     0,     0,     0,   306,     0,     0,     0,
     491,   492,   493,     0,    14,     0,   307,   494,   495,     0,
       0,     0,     0,     0,   308,  1326,     0,   497,     0,   498,
     499,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,     0,     0,     0,     0,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   684,     0,     0,
       0,     0,     0,   304,     0,     0,     0,     0,     0,   305,
       0,     0,     0,     0,     0,   306,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   307,    58,     0,     0,     0,
       0,     0,     0,   308,     0,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,     0,     0,     0,   309,     0,
       0,     0,     0,    59,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,   304,     0,     0,     0,     0,     0,   305,     0,
       0,     0,     0,     0,   306,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   307,    58,     0,     0,     0,     0,
       0,     0,   308,     0,     0,     0,     0,     0,   343,     0,
       0,     0,     0,     0,     0,     0,     0,   309,     0,     0,
       0,     0,    59,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   344,     0,
       0,   304,     0,     0,     0,     0,     0,   305,     0,     0,
       0,     0,     0,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,    58,     0,     0,     0,     0,     0,
       0,   308,   726,   727,     0,     0,     0,   343,     0,     0,
       0,     0,     0,     0,     0,     0,   309,     0,     0,     0,
       0,    59,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,   598,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,     0,     0,     0,     0,     0,   305,     0,
       0,     0,     0,    58,   306,   736,   737,   738,   739,   740,
       0,     0,   743,     0,   307,   746,   343,   748,   749,   750,
     751,     0,   308,     0,     0,   752,     0,   754,   755,     0,
     603,     0,     0,     0,     0,     0,     0,   309,     0,     0,
       0,     0,   604,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   344,     0,     0,     0,
       0,     0,     0,     0,   770,   771,   772,   773,   774,   775,
     776,   304,     0,     0,     0,     0,     0,   305,     0,     0,
     777,   778,     0,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,    58,     0,     0,     0,     0,     0,
       0,   308,     0,     0,     0,     0,     0,   343,     0,     0,
       0,     0,     0,     0,     0,     0,   309,     0,     0,     0,
       0,    59,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   726,   727,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     603,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     850,   851,   852,   853,   854,   855,   856,   857,   736,   737,
     738,   739,   740,   858,   859,   743,   744,   745,   746,   860,
     748,   749,   750,   751,  -346,     0,   344,     0,   752,   753,
     754,   755,   861,   862,   726,   727,   758,   759,   760,   863,
     864,   865,   764,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   866,   766,     0,   767,   768,   769,   770,   771,   772,
     773,   774,   775,   776,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   777,   778,     0,     0,     0,   498,   670,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   850,
     851,   852,   853,   854,   855,   856,   857,   736,   737,   738,
     739,   740,   858,   859,   743,   744,   745,   746,   860,   748,
     749,   750,   751,     0,   726,   727,     0,   752,   753,   754,
     755,   861,   862,     0,     0,   758,   759,   760,   863,   864,
     865,   764,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   726,
     727,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1052,     0,     0,     0,     0,     0,     0,
     866,   766,     0,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   777,   778,     0,     0,     0,   498,   670,   850,
     851,   852,   853,   854,   855,   856,   857,   736,   737,   738,
     739,   740,   858,   859,   743,   744,   745,   746,   860,   748,
     749,   750,   751,     0,     0,     0,     0,   752,   753,   754,
     755,   861,   862,     0,     0,   758,   759,   760,   863,   864,
     865,   764,   736,   737,   738,   739,   740,     0,     0,   743,
     744,   745,   746,     0,   748,   749,   750,   751,   726,   727,
       0,     0,   752,     0,   754,   755,     0,     0,     0,     0,
     758,   759,   760,  1236,     0,     0,   764,     0,     0,     0,
     866,   766,     0,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,   726,   727,     0,     0,     0,     0,     0,
       0,     0,   777,   778,     0,     0,     0,   498,   670,     0,
       0,     0,     0,     0,     0,     0,   766,     0,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   777,   778,     0,
       0,   789,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   736,   737,   738,   739,   740,     0,     0,   743,   744,
     745,   746,     0,   748,   749,   750,   751,     0,     0,     0,
       0,   752,     0,   754,   755,     0,     0,     0,     0,   758,
     759,   760,     0,     0,     0,   764,   736,   737,   738,   739,
     740,     0,     0,   743,   744,   745,   746,     0,   748,   749,
     750,   751,   726,   727,     0,     0,   752,     0,   754,   755,
       0,     0,     0,     0,   758,   759,   760,     0,     0,     0,
     764,     0,     0,     0,     0,   766,     0,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,   726,   727,     0,
       0,     0,     0,     0,     0,     0,   777,   778,     0,     0,
     799,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     766,     0,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   777,   778,     0,     0,   815,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   736,   737,   738,   739,   740,
       0,     0,   743,   744,   745,   746,     0,   748,   749,   750,
     751,     0,     0,     0,     0,   752,     0,   754,   755,     0,
       0,     0,     0,   758,   759,   760,     0,     0,     0,   764,
     736,   737,   738,   739,   740,     0,     0,   743,   744,   745,
     746,     0,   748,   749,   750,   751,   726,   727,     0,     0,
     752,     0,   754,   755,     0,     0,     0,     0,   758,   759,
     760,     0,     0,     0,   764,     0,     0,     0,     0,   766,
       0,   767,   768,   769,   770,   771,   772,   773,   774,   775,
     776,   726,   727,     0,     0,     0,     0,     0,     0,     0,
     777,   778,     0,     0,   830,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   766,     0,   767,   768,   769,   770,
     771,   772,   773,   774,   775,   776,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   777,   778,     0,     0,  1077,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   736,
     737,   738,   739,   740,     0,     0,   743,   744,   745,   746,
       0,   748,   749,   750,   751,     0,     0,     0,     0,   752,
       0,   754,   755,     0,     0,     0,     0,   758,   759,   760,
       0,     0,     0,   764,   736,   737,   738,   739,   740,     0,
       0,   743,   744,   745,   746,     0,   748,   749,   750,   751,
     726,   727,     0,     0,   752,     0,   754,   755,     0,     0,
       0,     0,   758,   759,   760,     0,     0,     0,   764,     0,
       0,     0,     0,   766,     0,   767,   768,   769,   770,   771,
     772,   773,   774,   775,   776,   726,   727,     0,     0,     0,
       0,     0,     0,     0,   777,   778,     0,     0,  1114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   766,     0,
     767,   768,   769,   770,   771,   772,   773,   774,   775,   776,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   777,
     778,     0,     0,  1118,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   736,   737,   738,   739,   740,     0,     0,
     743,   744,   745,   746,     0,   748,   749,   750,   751,     0,
       0,     0,     0,   752,     0,   754,   755,     0,     0,     0,
       0,   758,   759,   760,     0,     0,     0,   764,   736,   737,
     738,   739,   740,     0,     0,   743,   744,   745,   746,     0,
     748,   749,   750,   751,   726,   727,     0,     0,   752,     0,
     754,   755,     0,     0,     0,     0,   758,   759,   760,     0,
       0,     0,   764,     0,     0,     0,     0,   766,     0,   767,
     768,   769,   770,   771,   772,   773,   774,   775,   776,   726,
     727,     0,     0,     0,     0,     0,     0,     0,   777,   778,
       0,     0,  1120,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   766,     0,   767,   768,   769,   770,   771,   772,
     773,   774,   775,   776,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   777,   778,     0,     0,  1129,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   736,   737,   738,
     739,   740,     0,     0,   743,   744,   745,   746,     0,   748,
     749,   750,   751,     0,     0,     0,     0,   752,     0,   754,
     755,     0,     0,     0,     0,   758,   759,   760,     0,     0,
       0,   764,   736,   737,   738,   739,   740,     0,     0,   743,
     744,   745,   746,     0,   748,   749,   750,   751,   726,   727,
       0,     0,   752,     0,   754,   755,     0,     0,     0,     0,
     758,   759,   760,     0,     0,     0,   764,     0,     0,     0,
       0,   766,     0,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,   726,   727,     0,     0,     0,     0,     0,
       0,     0,   777,   778,     0,     0,  1130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   766,     0,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   777,   778,     0,
       0,  1131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   736,   737,   738,   739,   740,     0,     0,   743,   744,
     745,   746,     0,   748,   749,   750,   751,     0,     0,     0,
       0,   752,     0,   754,   755,     0,     0,     0,     0,   758,
     759,   760,     0,     0,     0,   764,   736,   737,   738,   739,
     740,     0,     0,   743,   744,   745,   746,     0,   748,   749,
     750,   751,   726,   727,     0,     0,   752,     0,   754,   755,
       0,     0,     0,     0,   758,   759,   760,     0,     0,     0,
     764,     0,     0,     0,     0,   766,     0,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,   726,   727,     0,
       0,     0,     0,     0,     0,     0,   777,   778,     0,     0,
    1132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     766,     0,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   777,   778,     0,     0,  1133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   736,   737,   738,   739,   740,
       0,     0,   743,   744,   745,   746,     0,   748,   749,   750,
     751,     0,     0,     0,     0,   752,     0,   754,   755,     0,
       0,     0,     0,   758,   759,   760,     0,     0,     0,   764,
     736,   737,   738,   739,   740,     0,     0,   743,   744,   745,
     746,     0,   748,   749,   750,   751,   726,   727,     0,     0,
     752,     0,   754,   755,     0,     0,     0,     0,   758,   759,
     760,     0,     0,     0,   764,     0,     0,     0,     0,   766,
       0,   767,   768,   769,   770,   771,   772,   773,   774,   775,
     776,   726,   727,     0,     0,     0,     0,     0,     0,     0,
     777,   778,     0,     0,  1134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   766,     0,   767,   768,   769,   770,
     771,   772,   773,   774,   775,   776,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   777,   778,     0,     0,  1266,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   736,
     737,   738,   739,   740,     0,     0,   743,   744,   745,   746,
       0,   748,   749,   750,   751,     0,     0,     0,     0,   752,
       0,   754,   755,     0,     0,     0,     0,   758,   759,   760,
       0,     0,     0,   764,   736,   737,   738,   739,   740,     0,
       0,   743,   744,   745,   746,     0,   748,   749,   750,   751,
     726,   727,     0,     0,   752,     0,   754,   755,     0,     0,
       0,     0,   758,   759,   760,     0,     0,     0,   764,     0,
       0,     0,     0,   766,     0,   767,   768,   769,   770,   771,
     772,   773,   774,   775,   776,   726,   727,     0,     0,     0,
       0,     0,     0,     0,   777,   778,     0,     0,  1388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   766,     0,
     767,   768,   769,   770,   771,   772,   773,   774,   775,   776,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   777,
     778,     0,     0,  1402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   736,   737,   738,   739,   740,     0,     0,
     743,   744,   745,   746,     0,   748,   749,   750,   751,     0,
       0,     0,     0,   752,     0,   754,   755,     0,     0,     0,
       0,   758,   759,   760,     0,     0,     0,   764,   736,   737,
     738,   739,   740,     0,     0,   743,   744,   745,   746,     0,
     748,   749,   750,   751,   726,   727,     0,     0,   752,     0,
     754,   755,     0,     0,     0,     0,   758,   759,   760,     0,
       0,     0,   764,     0,     0,     0,     0,   766,     0,   767,
     768,   769,   770,   771,   772,   773,   774,   775,   776,   726,
     727,     0,     0,     0,     0,     0,     0,     0,   777,   778,
       0,     0,  1403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   766,     0,   767,   768,   769,   770,   771,   772,
     773,   774,   775,   776,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   777,   778,     0,     0,  1422,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   736,   737,   738,
     739,   740,     0,     0,   743,   744,   745,   746,     0,   748,
     749,   750,   751,     0,     0,     0,     0,   752,     0,   754,
     755,     0,     0,     0,     0,   758,   759,   760,     0,     0,
       0,   764,   736,   737,   738,   739,   740,     0,     0,   743,
     744,   745,   746,     0,   748,   749,   750,   751,   726,   727,
       0,     0,   752,     0,   754,   755,     0,     0,     0,     0,
     758,   759,   760,     0,     0,     0,   764,     0,     0,     0,
       0,   766,     0,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,   726,   727,     0,     0,     0,     0,     0,
       0,     0,   777,   778,     0,     0,  1424,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   766,     0,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   777,   778,     0,
       0,  1426,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   736,   737,   738,   739,   740,     0,     0,   743,   744,
     745,   746,     0,   748,   749,   750,   751,     0,     0,     0,
       0,   752,     0,   754,   755,     0,     0,     0,     0,   758,
     759,   760,     0,     0,     0,   764,   736,   737,   738,   739,
     740,     0,     0,   743,   744,   745,   746,     0,   748,   749,
     750,   751,   726,   727,     0,     0,   752,     0,   754,   755,
       0,     0,     0,     0,   758,   759,   760,     0,     0,     0,
     764,     0,     0,     0,     0,   766,     0,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,   726,   727,     0,
       0,     0,     0,     0,     0,     0,   777,   778,     0,     0,
    1430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     766,     0,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   777,   778,     0,     0,  1492,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   736,   737,   738,   739,   740,
       0,     0,   743,   744,   745,   746,     0,   748,   749,   750,
     751,     0,     0,     0,     0,   752,     0,   754,   755,     0,
       0,     0,     0,   758,   759,   760,     0,     0,     0,   764,
     736,   737,   738,   739,   740,     0,     0,   743,   744,   745,
     746,     0,   748,   749,   750,   751,   726,   727,     0,     0,
     752,     0,   754,   755,     0,     0,     0,     0,   758,   759,
     760,     0,     0,     0,   764,     0,     0,     0,     0,   766,
       0,   767,   768,   769,   770,   771,   772,   773,   774,   775,
     776,   726,   727,     0,     0,     0,     0,     0,     0,     0,
     777,   778,     0,     0,  1493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   766,     0,   767,   768,   769,   770,
     771,   772,   773,   774,   775,   776,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   777,   778,     0,     0,  1494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   736,
     737,   738,   739,   740,     0,     0,   743,   744,   745,   746,
       0,   748,   749,   750,   751,     0,     0,     0,     0,   752,
       0,   754,   755,     0,     0,     0,     0,   758,   759,   760,
       0,     0,     0,   764,   736,   737,   738,   739,   740,     0,
       0,   743,   744,   745,   746,     0,   748,   749,   750,   751,
     726,   727,     0,     0,   752,     0,   754,   755,     0,     0,
       0,     0,   758,   759,   760,     0,     0,     0,   764,     0,
       0,     0,     0,   766,     0,   767,   768,   769,   770,   771,
     772,   773,   774,   775,   776,   726,   727,     0,     0,     0,
       0,     0,     0,     0,   777,   778,     0,     0,  1502,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   766,     0,
     767,   768,   769,   770,   771,   772,   773,   774,   775,   776,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   777,
     778,     0,     0,  1509,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   736,   737,   738,   739,   740,     0,     0,
     743,   744,   745,   746,     0,   748,   749,   750,   751,     0,
       0,     0,     0,   752,     0,   754,   755,     0,     0,     0,
       0,   758,   759,   760,     0,     0,     0,   764,   736,   737,
     738,   739,   740,     0,     0,   743,   744,   745,   746,     0,
     748,   749,   750,   751,   726,   727,     0,     0,   752,     0,
     754,   755,     0,     0,     0,     0,   758,   759,   760,     0,
       0,     0,   764,     0,     0,     0,     0,   766,     0,   767,
     768,   769,   770,   771,   772,   773,   774,   775,   776,   726,
     727,     0,     0,     0,     0,     0,     0,     0,   777,   778,
       0,     0,  1514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   766,     0,   767,   768,   769,   770,   771,   772,
     773,   774,   775,   776,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   777,   778,     0,     0,  1563,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   736,   737,   738,
     739,   740,     0,     0,   743,   744,   745,   746,     0,   748,
     749,   750,   751,     0,     0,     0,     0,   752,     0,   754,
     755,     0,     0,     0,     0,   758,   759,   760,     0,     0,
       0,   764,   736,   737,   738,   739,   740,   726,   727,   743,
     744,   745,   746,     0,   748,   749,   750,   751,     0,     0,
       0,     0,   752,     0,   754,   755,     0,     0,     0,     0,
     758,   759,   760,     0,     0,     0,   764,     0,     0,     0,
       0,   766,     0,   767,   768,   769,   770,   771,   772,   773,
     774,   775,   776,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   777,   778,   820,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   766,     0,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   777,   778,  1074,
     736,   737,   738,   739,   740,   726,   727,   743,   744,   745,
     746,     0,   748,   749,   750,   751,     0,     0,     0,     0,
     752,     0,   754,   755,     0,     0,     0,     0,   758,   759,
     760,     0,     0,     0,   764,     0,     0,     0,     0,     0,
     726,   727,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   766,     0,   767,   768,   769,   770,
     771,   772,   773,   774,   775,   776,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   777,   778,  1210,   736,   737,
     738,   739,   740,     0,     0,   743,   744,   745,   746,     0,
     748,   749,   750,   751,     0,     0,     0,     0,   752,     0,
     754,   755,     0,     0,     0,     0,   758,   759,   760,     0,
       0,     0,   764,   736,   737,   738,   739,   740,   726,   727,
     743,   744,   745,   746,     0,   748,   749,   750,   751,     0,
       0,     0,     0,   752,     0,   754,   755,     0,     0,     0,
       0,   758,   759,   760,     0,     0,     0,   764,     0,     0,
       0,     0,   766,     0,   767,   768,   769,   770,   771,   772,
     773,   774,   775,   776,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   777,   778,  1225,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   766,     0,   767,
     768,   769,   770,   771,   772,   773,   774,   775,   776,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   777,   778,
    1348,   736,   737,   738,   739,   740,     0,     0,   743,   744,
     745,   746,     0,   748,   749,   750,   751,   229,   230,     0,
       0,   752,     0,   754,   755,     0,     0,     0,     0,   758,
     759,   760,     0,     0,   231,   764,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   726,   727,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   766,     0,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   777,   778,  1353,     0,
       0,     0,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
       0,     0,   250,   251,   252,     0,     0,     0,     0,     0,
       0,   253,   254,   255,   256,   257,     0,     0,   258,   259,
     260,   261,   262,   263,   264,   736,   737,   738,   739,   740,
     726,   727,   743,   744,   745,   746,     0,   748,   749,   750,
     751,     0,     0,     0,     0,   752,     0,   754,   755,     0,
       0,     0,     0,   758,   759,   760,     0,     0,     0,   764,
       0,     0,     0,     0,   265,     0,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,     0,     0,   276,   277,
       0,     0,     0,     0,     0,   278,   279,     0,     0,   284,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   766,
       0,   767,   768,   769,   770,   771,   772,   773,   774,   775,
     776,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     777,   778,     0,   736,   737,   738,   739,   740,   726,   727,
     743,   744,   745,   746,     0,   748,   749,   750,   751,     0,
       0,     0,     0,   752,     0,   754,   755,     0,     0,   916,
       0,   758,   759,   760,     0,     0,     0,   764,     0,     0,
       0,     0,     0,   726,   727,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   766,     0,   767,
     768,   769,   770,   771,   772,   773,   774,   775,   776,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   777,   778,
       0,   736,   737,   738,   739,   740,     0,     0,   743,   744,
     745,   746,     0,   748,   749,   750,   751,     0,     0,     0,
       0,   752,     0,   754,   755,     0,     0,  1143,     0,   758,
     759,   760,     0,   726,   727,   764,   736,   737,   738,   739,
     740,     0,     0,   743,   744,   745,   746,     0,   748,   749,
     750,   751,     0,     0,     0,     0,   752,     0,   754,   755,
       0,     0,     0,     0,   758,   759,   760,     0,   726,   727,
     764,     0,     0,     0,     0,   766,     0,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   777,   778,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     766,  1215,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,     0,     0,     0,     0,   736,   737,   738,   739,
     740,   777,   778,   743,   744,   745,   746,     0,   748,   749,
     750,   751,     0,     0,     0,     0,   752,     0,   754,   755,
       0,     0,     0,     0,   758,   759,   760,     0,   726,   727,
     764,   736,   737,   738,   739,   740,     0,     0,   743,   744,
     745,   746,     0,   748,   749,   750,   751,     0,     0,     0,
       0,   752,     0,   754,   755,     0,     0,     0,     0,   758,
     759,   760,  1227,   726,   727,   764,     0,     0,     0,     0,
     766,     0,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   777,   778,  1312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   766,     0,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,     0,     0,     0,
       0,   736,   737,   738,   739,   740,   777,   778,   743,   744,
     745,   746,     0,   748,   749,   750,   751,     0,     0,     0,
       0,   752,     0,   754,   755,     0,     0,     0,     0,   758,
     759,   760,     0,   726,   727,   764,   736,   737,   738,   739,
     740,     0,     0,   743,   744,   745,   746,     0,   748,   749,
     750,   751,     0,     0,     0,     0,   752,     0,   754,   755,
       0,     0,     0,     0,   758,   759,   760,  1510,   726,   727,
     764,     0,     0,     0,     0,   766,     0,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   777,   778,     0,     0,
       0,     0,  1559,     0,     0,     0,     0,     0,     0,     0,
     766,     0,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,     0,     0,     0,     0,   736,   737,   738,   739,
     740,   777,   778,   743,   744,   745,   746,     0,   748,   749,
     750,   751,     0,     0,     0,     0,   752,     0,   754,   755,
       0,     0,     0,     0,   758,   759,   760,     0,   726,   727,
     764,   736,   737,   738,   739,   740,     0,     0,   743,   744,
     745,   746,     0,   748,   749,   750,   751,     0,     0,     0,
       0,   752,     0,   754,   755,     0,     0,     0,     0,   758,
     759,   760,  1574,   726,   727,   764,     0,     0,     0,     0,
     766,     0,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   777,   778,     0,     0,     0,     0,  1575,     0,     0,
       0,     0,     0,     0,     0,   766,     0,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,     0,     0,     0,
       0,   736,   737,   738,   739,   740,   777,   778,   743,   744,
     745,   746,     0,   748,   749,   750,   751,     0,     0,     0,
       0,   752,     0,   754,   755,     0,     0,     0,     0,   758,
     759,   760,     0,   726,   727,   764,   736,   737,   738,   739,
     740,     0,     0,   743,   744,   745,   746,     0,   748,   749,
     750,   751,     0,     0,     0,     0,   752,     0,   754,   755,
       0,     0,     0,     0,   758,   759,   760,  1577,   726,   727,
     764,     0,     0,     0,     0,   766,     0,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   777,   778,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     766,     0,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,     0,     0,     0,     0,   736,   737,   738,   739,
     740,   777,   778,   743,   744,   745,   746,     0,   748,   749,
     750,   751,     0,     0,     0,     0,   752,     0,   754,   755,
       0,     0,     0,     0,   758,   759,   760,     0,   726,   727,
    -734,   736,   737,   738,   739,   740,     0,     0,   743,   744,
     745,   746,     0,   748,   749,   750,   751,     0,     0,     0,
       0,   752,     0,   754,   755,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     766,     0,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   777,   778,     0,   954,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   769,
     770,   771,   772,   773,   774,   775,   776,     0,     0,     0,
       0,   736,   737,   738,   739,   740,   777,   778,   743,   744,
     745,   746,     0,   748,   749,   750,   751,     0,     0,     0,
       0,   752,     0,   754,   755,   310,   311,   312,     0,   314,
     315,   316,   317,   318,   458,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,     0,   332,   333,   334,
       0,     0,   337,   338,   339,   340,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   958,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     770,   771,   772,   773,   774,   775,   776,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   777,   778,     0,     0,
    1211,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   955,     0,     0,     0,     0,     0,     0,     0,     0,
     310,   311,   312,   956,   314,   315,   316,   317,   318,   458,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   310,   311,   312,     0,   314,   315,   316,   317,   318,
     458,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,     0,   332,   333,   334,     0,     0,   337,   338,
     339,   340,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   959,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   960,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   182,     0,
     183,     0,   184,   185,   186,   187,   188,  1212,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,  1213,
     200,   201,   202,     0,     0,   203,   204,   205,   206,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   207,   208,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   209
};

static const yytype_int16 yycheck[] =
{
      14,    15,   211,   381,   417,   421,   369,   642,   565,   369,
     438,   685,   477,   161,   585,   532,   704,   584,   706,   225,
     891,   943,     7,    21,   376,  1146,    19,    32,    56,   579,
      52,   581,   410,   583,    62,    19,   466,   467,   438,   556,
     225,   715,   806,  1365,   808,    49,   810,   907,    19,    63,
      64,    65,   103,   913,    18,    19,    19,  1329,    56,   124,
      19,   123,   462,    19,   126,   127,   141,   142,   143,   497,
      20,    21,    45,   167,     0,   129,   124,    32,   438,   152,
       6,   177,  1470,   167,   157,    52,   134,    14,    15,    46,
     104,   105,   106,   107,   124,   185,   149,   475,   476,  1495,
     149,   140,   462,    29,   134,    31,    49,    33,   157,    66,
     140,   171,   172,    39,   167,   211,     4,     5,   167,   213,
     210,   186,   124,    49,   189,   179,    32,   211,   167,    55,
      33,   104,   134,  1521,   126,   127,    24,   212,   186,   212,
    1536,  1413,    30,   191,  1466,   149,   208,   209,    91,   297,
     178,   369,    78,    59,    60,   672,   186,   179,   846,    62,
     588,   367,   211,   167,   139,   115,   116,   218,   842,   569,
     179,  1292,   179,   123,   100,   101,   126,   127,   128,    67,
      68,   869,   367,   133,   186,   124,   191,   177,   155,    14,
      15,   140,   185,   178,   882,   134,   381,   885,   188,   197,
     167,   185,   177,   167,    61,   212,   634,   635,   160,   569,
     208,   225,   100,   101,   185,   121,   208,   209,   124,   125,
     438,   621,   185,  1145,   209,   410,   185,   412,   134,   185,
     133,   659,   441,   160,   662,   635,   191,   140,   165,   188,
     167,    20,    21,   170,   462,   124,   455,   186,   179,   124,
      32,   186,   140,   179,   209,   134,  1127,   124,   208,   209,
     215,   621,   193,   641,   167,   665,   124,   134,  1389,   157,
     158,   159,   198,   179,   185,   635,   134,    59,    60,   167,
     186,   187,  1142,   209,   211,   191,   664,   715,   194,   177,
     475,   476,   179,   140,   724,    32,   177,   126,   127,   202,
     149,   179,   149,   209,   192,   665,   188,   186,   211,   215,
     185,   186,   192,   188,   185,  1186,   191,   179,   167,   186,
     167,   209,    59,    60,   179,   212,   155,   179,   186,   179,
     344,   211,   213,   214,   212,   160,   115,   116,   167,   121,
     165,   212,   167,   125,   123,   170,   125,   126,   127,   128,
     212,   569,   780,   367,   133,   182,   183,   212,   372,   167,
     212,   185,   212,   179,   570,   571,   572,   381,   574,   575,
     722,   179,   578,   185,   580,   179,   582,   193,   182,   208,
     209,   179,   167,   210,   121,   570,   571,   572,   125,   574,
     575,   189,   792,   578,   179,   580,   410,   582,   412,   584,
     212,   185,   952,   621,  1326,   187,   210,     4,     5,   191,
       7,   193,   194,   619,   842,   185,   625,   635,   834,   185,
     434,   167,   168,   169,   185,   942,   177,   209,   212,   208,
     209,  1291,   792,   215,   619,   189,  1200,   646,    35,   193,
     185,   185,   212,  1303,   185,   185,   212,   665,   211,   185,
     187,   212,   198,   918,   191,   185,   641,   194,   185,   837,
     838,   475,   476,   928,   185,   210,   210,   845,  1142,   899,
     210,   212,   209,   192,   210,   167,   904,   185,   215,   664,
     210,   188,   691,  1157,   912,   212,   193,   179,   192,    56,
     192,   212,   211,    20,    21,    62,   874,   875,  1069,   877,
    1067,   710,   210,   881,   211,   883,   884,   211,   886,   211,
     728,   729,   730,   731,   732,   733,   734,   735,   192,    56,
    1284,   140,   179,   741,   742,    62,   179,    73,   140,   747,
     149,    77,   962,   963,   964,   154,   193,   211,   756,   757,
     193,   726,   727,   761,   762,   763,   167,   765,   167,   188,
      96,    97,    98,    99,   193,  1319,   570,   571,   572,    56,
     574,   575,   179,   179,   578,    62,   580,   752,   582,    56,
     584,   160,   211,   192,   792,    62,   193,   193,  1128,   188,
     208,    63,    64,    65,   193,    32,   113,   114,   115,   116,
     117,   155,   188,   120,   121,   122,   123,   193,   125,   126,
     127,   128,   211,   167,   179,   619,   133,   179,   135,   136,
     816,   186,    59,    60,   141,   211,   143,   631,   824,   179,
     155,   827,   104,   105,   106,   107,   189,   641,   167,  1179,
     193,   816,   167,   193,   155,   841,  1271,    11,   844,   824,
     788,    56,   827,   155,   626,  1409,   167,    62,    22,    23,
     664,    56,   837,   838,  1082,   167,   841,    62,   867,   844,
     845,   155,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   177,   167,   121,   213,   214,  1462,   125,  1464,
     185,   208,   209,  1468,  1469,   185,   167,  1375,   188,   874,
     875,   191,   877,   707,   178,   179,   881,   181,   883,   884,
     682,   886,   684,   178,   686,    56,   181,  1135,   179,   184,
     179,    62,   694,   179,   185,   921,   185,   923,   179,   185,
     179,   703,   152,   179,   185,   189,   185,   157,  1156,   193,
     167,   189,   179,  1111,  1162,   193,   921,   171,   923,   189,
     187,  1169,   167,   193,   191,  1173,   193,   194,   189,   189,
     189,   189,   193,   193,   193,   193,    32,   189,   141,   142,
     143,   193,   209,   189,  1438,  1439,   188,   193,   215,   191,
     188,   167,   786,   191,  1324,  1449,  1450,   167,   168,   169,
    1208,    86,    87,    59,    60,   171,   172,   173,   174,   177,
      20,    21,  1220,   171,   172,   173,   178,   179,   180,   141,
    1178,   143,   816,    10,    11,    12,   171,   172,  1443,  1172,
     824,   167,  1172,   827,   167,  1503,  1504,   921,  1506,   923,
      56,   179,   171,   837,   838,   141,   104,   841,   157,   186,
     844,   845,   177,   177,   177,    34,  1021,    34,   211,   177,
     167,   178,   167,   167,   167,   121,   210,    21,   167,   125,
    1056,   210,   188,   188,    42,   177,   177,  1285,  1415,   211,
     874,   875,   192,   877,   192,   211,   192,   881,   167,   883,
     884,  1056,   886,   134,   192,   192,   189,   211,   192,  1295,
      11,   192,  1067,   113,   114,   115,   116,   167,   192,    20,
      21,   192,   192,   123,   192,   125,   126,   127,   128,   211,
     167,   211,   210,   133,   167,   135,   136,   921,   211,   923,
      42,   187,   209,   192,   179,   191,   211,   193,   194,   192,
     192,   211,   167,   192,    32,   211,  1111,  1484,  1485,   160,
     192,    36,  1397,   209,   192,  1400,   192,  1365,   192,   215,
     192,   211,   192,   211,   179,   211,   179,   211,     9,    65,
     179,    59,    60,   179,  1172,   214,   211,   179,   179,  1337,
     211,   179,   209,   211,   194,   195,   196,   197,   198,   178,
    1527,  1180,  1388,   211,    42,   192,   167,   167,   208,   209,
     212,   179,   113,   114,   115,   116,   117,   210,   188,   120,
     121,   122,   123,  1178,   125,   126,   127,   128,   178,   167,
     179,   178,   133,   179,   135,   136,   178,   167,    20,    21,
     141,   142,   143,   121,  1442,   211,   147,   125,   185,   129,
    1448,    13,   186,  1436,   160,   188,   185,   214,   177,   185,
     157,     7,   167,   167,   212,   193,  1414,    42,  1466,   185,
     211,   185,  1056,   212,   210,  1473,   211,   167,   178,  1255,
     178,   192,  1261,  1067,   211,   192,   187,   211,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   211,   211,
    1255,   211,   211,   211,     1,   178,    66,   208,   209,   187,
     179,   167,   193,   191,   193,   193,   194,   193,   167,   167,
     212,   212,   211,   211,  1522,   212,   210,  1111,  1514,   212,
     167,   209,   167,   115,   116,  1483,   210,   215,   210,    32,
     167,   123,  1321,   125,   126,   127,   128,   167,   192,   211,
     211,   133,   211,   179,   211,   179,   211,  1336,   211,   210,
     369,   167,   212,  1339,   210,   210,    59,    60,    32,   378,
      32,   211,   211,   167,   193,   211,   193,   211,   387,   211,
     179,    32,  1337,   211,  1339,    52,   211,   210,   397,   211,
     178,   212,   178,   178,  1178,    59,    60,   212,   407,   212,
     212,    20,    21,   210,   212,  1384,   179,   416,    59,    60,
      32,   212,   194,   195,   196,   197,   198,   212,   210,   428,
     210,   178,   693,    79,     1,   133,   208,   209,   121,   438,
      44,   124,   125,    82,   443,  1406,   445,    59,    60,  1407,
     223,   134,   465,   466,   467,   454,    80,   786,   930,  1407,
    1407,  1427,     1,   462,   463,   464,  1407,   121,  1407,  1414,
     948,   125,  1384,    32,  1334,  1357,  1435,  1360,   477,   422,
     121,  1255,  1427,  1232,   125,   554,   485,    52,   434,   488,
     434,  1395,   491,   492,   493,   494,   495,  1152,   515,    -1,
      59,    60,   515,   186,   187,   504,   115,   116,   191,   121,
      -1,   194,  1481,   125,   123,   685,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   209,    -1,    -1,   542,
      32,    -1,   215,   187,    -1,    -1,    -1,   191,  1483,   193,
     194,    -1,    -1,    -1,    32,    -1,   187,    -1,    -1,    -1,
     191,    -1,   193,   194,    -1,   209,    -1,    59,    60,    -1,
      -1,   215,   121,  1337,    -1,  1339,   125,    -1,   209,    -1,
     569,    59,    60,    -1,   215,   187,    -1,    -1,    -1,   191,
      -1,   193,   194,    -1,    -1,    -1,    -1,   196,   197,   198,
      -1,    -1,    -1,  1562,    -1,    -1,    -1,   209,   597,   208,
     209,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   614,    -1,    -1,    -1,   121,
      -1,    -1,   621,   125,    -1,    -1,   639,    -1,   187,    -1,
      -1,    -1,   191,   121,   193,   194,   635,   125,    -1,    -1,
    1414,    -1,    -1,   642,   643,    -1,   645,    -1,    -1,    -1,
     209,   650,    -1,  1427,   667,    -1,   215,   656,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   665,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   676,   677,   678,
     679,   680,   681,    -1,   683,   187,   685,    -1,    -1,   191,
      -1,   193,   194,    -1,    -1,    -1,  1470,    -1,    -1,   187,
      -1,    -1,    -1,   191,    -1,   193,   194,   209,    -1,  1483,
      -1,   724,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,    -1,    -1,    20,    21,    -1,   215,    -1,   728,
     729,   730,   731,   732,   733,   734,   735,   736,   737,    -1,
      -1,   740,   741,   742,   743,   744,   745,  1521,   747,   748,
      -1,   750,   751,   752,   753,   754,   755,   756,   757,   758,
     759,   760,   761,   762,   763,   764,   765,   766,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,    -1,   778,
      -1,   794,    32,   782,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   792,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,    -1,   836,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,   843,   141,   142,   143,    -1,    -1,
     849,   850,   851,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,   866,    -1,    -1,
      -1,   121,    -1,    -1,    -1,   125,   889,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,   899,    -1,    -1,    -1,
      -1,   187,    -1,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   902,   903,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   208,   209,    -1,    -1,    -1,   916,    -1,   918,
      -1,    20,    21,    -1,    -1,    -1,   925,    -1,    -1,   928,
      -1,    -1,    -1,   932,    -1,    -1,    -1,   187,    -1,    -1,
      -1,   191,   941,   193,   194,    -1,    -1,    -1,    -1,   962,
     963,   964,    -1,    -1,   967,    -1,   969,    -1,   971,   209,
     973,    -1,   975,    -1,   977,   215,   979,    -1,   981,    -1,
      -1,    -1,    -1,   986,    -1,   988,   113,   114,   115,   116,
     117,   994,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,  1006,    -1,  1008,   133,    -1,   135,   136,
    1013,    -1,  1015,    -1,  1017,    -1,    -1,  1020,    -1,    -1,
      -1,    -1,    -1,    32,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,  1034,   135,   136,  1051,  1038,
      59,    60,   141,   142,   143,    -1,    -1,    -1,   147,    -1,
      -1,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1079,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,   208,
     209,    -1,    -1,    -1,   213,   214,  1115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,   729,
     730,   731,   732,   733,   734,   735,    20,    21,  1137,  1138,
    1139,   741,   742,    -1,  1143,    -1,    -1,   747,    -1,    -1,
      -1,    -1,  1151,  1152,    -1,    -1,   756,   757,    -1,    -1,
      -1,   761,   762,   763,    -1,   765,    -1,    -1,   187,    -1,
      -1,    -1,   191,  1172,   193,   194,    -1,  1176,  1177,    -1,
      -1,    -1,    -1,    -1,  1183,    20,    21,    -1,    -1,  1188,
     209,    -1,    -1,    -1,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,  1202,    -1,  1204,    -1,    -1,    -1,    -1,
    1209,    -1,    -1,    -1,    -1,    -1,  1215,    -1,    -1,    -1,
    1219,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,   138,    -1,    -1,   141,   142,   143,
     144,   145,   146,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1271,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,   186,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,   208,   209,    -1,    -1,    -1,   213,
     214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1345,    -1,  1347,    -1,
      -1,    -1,    -1,  1352,    -1,   190,   191,   192,   193,   194,
     195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1371,   208,   209,    -1,    32,  1376,  1377,  1378,
      -1,    -1,    -1,    -1,  1383,    -1,    -1,    -1,  1387,    -1,
      -1,    -1,  1391,  1392,  1393,  1394,    -1,    -1,  1397,  1398,
      -1,  1400,  1401,    59,    60,    -1,    -1,    -1,  1407,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,   117,    -1,  1418,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,    -1,    -1,  1443,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1456,  1457,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,  1474,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1486,  1487,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    -1,
      -1,    -1,  1501,    -1,    -1,    32,    -1,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1523,    -1,    -1,    -1,    -1,  1528,
    1529,   187,    59,    60,    -1,   191,    -1,   193,   194,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1545,     1,  1547,  1548,
       4,     5,     6,   209,     8,     9,    10,  1556,    12,   215,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    25,    26,    27,    28,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    41,    -1,    43,
      44,    -1,    -1,    47,   121,    49,    50,    51,   125,    53,
      54,    -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   115,   116,    -1,   191,    -1,   193,   194,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,   209,    -1,    -1,    -1,   140,    -1,   215,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,   211,     1,   213,
     214,     4,     5,     6,    -1,     8,     9,    10,    -1,    12,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    25,    26,    27,    28,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
      32,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,    -1,   198,    -1,    -1,   201,   202,
      -1,    -1,     4,     5,    -1,    -1,   209,    -1,   211,    -1,
     213,   214,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   121,
      -1,    -1,    44,   125,    -1,    47,    -1,    -1,    50,    -1,
      52,    -1,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,    71,
      -1,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    -1,    96,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,   191,
      -1,   193,   194,   115,   116,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,   209,    -1,    -1,
      -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,   191,
      -1,    -1,   194,   195,   196,    -1,   198,    -1,    -1,   201,
     202,    -1,    -1,    -1,     4,     5,    -1,   209,    -1,   211,
      -1,   213,   214,   215,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    59,
      60,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,
      -1,   191,    -1,    -1,   194,   195,   196,    -1,   198,    -1,
      -1,   201,   202,    -1,    -1,    -1,     4,     5,    -1,   209,
      -1,   211,    -1,   213,   214,   215,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,    -1,    -1,   194,   195,   196,    -1,
     198,    -1,    -1,   201,   202,    -1,    -1,    -1,     4,     5,
      -1,   209,    -1,   211,    -1,   213,   214,   215,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    -1,
      96,    97,    98,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,   151,   152,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,    -1,   198,    -1,    -1,   201,   202,    -1,    -1,     4,
       5,    -1,    -1,   209,    -1,   211,    -1,   213,   214,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,    -1,
       4,     5,    -1,    -1,   209,    -1,   211,    -1,   213,   214,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    25,    26,    27,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    51,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,
      -1,     4,     5,    -1,    -1,   209,    -1,   211,    -1,   213,
     214,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,    -1,   198,    -1,    -1,   201,   202,
      -1,    -1,     4,     5,    -1,    -1,   209,    -1,   211,    -1,
     213,   214,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,    71,
      -1,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    -1,    96,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,
     152,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,    -1,   198,    -1,    -1,   201,
     202,    -1,    -1,     4,     5,    -1,    -1,   209,    -1,   211,
      -1,   213,   214,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,    -1,   198,    -1,    -1,
     201,   202,    -1,    -1,     4,     5,    -1,    -1,   209,    -1,
     211,   212,   213,   214,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,    -1,   198,    -1,
      -1,   201,   202,    -1,    -1,    -1,    -1,     4,     5,   209,
      -1,   211,   212,   213,   214,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,
       4,     5,   209,   210,   211,    -1,   213,   214,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,
      -1,    -1,    -1,     4,     5,   209,    -1,   211,    -1,   213,
     214,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,    -1,   198,    -1,    -1,
     201,   202,    -1,    -1,     4,     5,    -1,    -1,   209,   210,
     211,    -1,   213,   214,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,   151,   152,    -1,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,    -1,   198,    -1,
      -1,   201,   202,    -1,    -1,     4,     5,    -1,    -1,   209,
     210,   211,    -1,   213,   214,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,    -1,   198,
      -1,    -1,   201,   202,    -1,    -1,     4,     5,    -1,    -1,
     209,    -1,   211,    -1,   213,   214,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,    -1,
     198,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,     4,
       5,   209,    -1,   211,    -1,   213,   214,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,    -1,
       4,     5,    -1,    -1,   209,    -1,   211,    -1,   213,   214,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,
     194,   195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,
      -1,    -1,    -1,     4,     5,   209,    -1,   211,    -1,   213,
     214,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,    -1,   198,    -1,    -1,
     201,   202,    -1,    -1,    -1,    -1,     4,     5,   209,    -1,
     211,    -1,   213,   214,    12,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,    -1,
     198,    -1,    -1,   201,   202,    -1,    -1,     4,     5,    -1,
      -1,   209,    -1,   211,    -1,   213,   214,    14,    15,    16,
      17,    18,    -1,    -1,    21,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,   201,   202,    -1,    -1,     4,     5,
      -1,    -1,   209,    -1,   211,    -1,   213,   214,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    -1,
      96,    97,    98,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,   151,   152,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,    -1,   198,    -1,    -1,   201,   202,    -1,    -1,     4,
       5,    -1,    -1,   209,    -1,   211,   212,   213,   214,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,   194,
     195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,    -1,
       4,     5,    -1,    -1,   209,    -1,   211,    -1,   213,   214,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,
      -1,     4,     5,    -1,    -1,   209,    -1,   211,   212,   213,
     214,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,    -1,   198,    -1,    -1,   201,   202,
      -1,    -1,     4,     5,    -1,    -1,   209,    -1,   211,   212,
     213,   214,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,    71,
      -1,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    -1,    96,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,
     152,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,    -1,   198,    -1,    -1,   201,
     202,    -1,    -1,     4,     5,    -1,    -1,   209,    -1,   211,
     212,   213,   214,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,    -1,   198,    -1,    -1,
     201,   202,    -1,    -1,     4,     5,    -1,    -1,   209,    -1,
     211,   212,   213,   214,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,   151,   152,    -1,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,    -1,   198,    -1,
      -1,   201,   202,    -1,    -1,     4,     5,    -1,    -1,   209,
      -1,   211,   212,   213,   214,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,    -1,   198,
      -1,    -1,   201,   202,    -1,    -1,     4,     5,    -1,    -1,
     209,    -1,   211,   212,   213,   214,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,    -1,
     198,    -1,    -1,   201,   202,    -1,    -1,     4,     5,    -1,
      -1,   209,    -1,   211,    -1,   213,   214,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,   201,   202,    -1,    -1,     4,     5,
      -1,    -1,   209,    -1,   211,    -1,   213,   214,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    -1,
      96,    97,    98,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,   151,   152,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,    -1,   198,    -1,    -1,   201,   202,    -1,    -1,     4,
       5,    -1,    -1,   209,    -1,   211,    -1,   213,   214,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,    -1,
       4,     5,    -1,    -1,   209,    -1,   211,    -1,   213,   214,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    12,    -1,
      -1,    -1,    -1,   177,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
     194,   195,   196,    -1,   198,    -1,    40,   201,   202,    -1,
      -1,    -1,    -1,    -1,    48,   209,    -1,   211,    -1,   213,
     214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,   167,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,   167,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    20,    21,    -1,    -1,    -1,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,   167,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,   140,    30,   113,   114,   115,   116,   117,
      -1,    -1,   120,    -1,    40,   123,   153,   125,   126,   127,
     128,    -1,    48,    -1,    -1,   133,    -1,   135,   136,    -1,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,   179,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   192,   193,   194,   195,   196,   197,
     198,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
     208,   209,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,   167,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,   213,    -1,   133,   134,
     135,   136,   137,   138,    20,    21,   141,   142,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,   187,    -1,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   208,   209,    -1,    -1,    -1,   213,   214,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,    20,    21,    -1,   133,   134,   135,
     136,   137,   138,    -1,    -1,   141,   142,   143,   144,   145,
     146,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
     186,   187,    -1,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   208,   209,    -1,    -1,    -1,   213,   214,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,   138,    -1,    -1,   141,   142,   143,   144,   145,
     146,   147,   113,   114,   115,   116,   117,    -1,    -1,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    20,    21,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,   179,    -1,    -1,   147,    -1,    -1,    -1,
     186,   187,    -1,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   208,   209,    -1,    -1,    -1,   213,   214,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,
      -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    20,    21,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,    -1,
     212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   209,    -1,    -1,   212,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
     113,   114,   115,   116,   117,    -1,    -1,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    20,    21,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,   187,
      -1,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     208,   209,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   208,   209,    -1,    -1,   212,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      20,    21,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   208,   209,    -1,    -1,   212,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,
     209,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,   117,    -1,    -1,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    20,    21,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,   187,    -1,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,   209,
      -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,    -1,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   208,   209,    -1,    -1,   212,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,   113,   114,   115,   116,   117,    -1,    -1,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    20,    21,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,   187,    -1,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   208,   209,    -1,    -1,   212,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,
      -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    20,    21,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,    -1,
     212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   209,    -1,    -1,   212,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
     113,   114,   115,   116,   117,    -1,    -1,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    20,    21,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,   187,
      -1,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     208,   209,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   208,   209,    -1,    -1,   212,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      20,    21,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   208,   209,    -1,    -1,   212,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,
     209,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,   117,    -1,    -1,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    20,    21,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,   187,    -1,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,   209,
      -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,    -1,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   208,   209,    -1,    -1,   212,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,   113,   114,   115,   116,   117,    -1,    -1,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    20,    21,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,   187,    -1,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   208,   209,    -1,    -1,   212,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,
      -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    20,    21,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,    -1,
     212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   209,    -1,    -1,   212,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
     113,   114,   115,   116,   117,    -1,    -1,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    20,    21,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,   187,
      -1,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     208,   209,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   208,   209,    -1,    -1,   212,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      20,    21,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   208,   209,    -1,    -1,   212,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,
     209,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,   117,    -1,    -1,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    20,    21,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,   187,    -1,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,   209,
      -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,    -1,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   208,   209,    -1,    -1,   212,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,   113,   114,   115,   116,   117,    20,    21,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,   187,    -1,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   208,   209,   210,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,   209,   210,
     113,   114,   115,   116,   117,    20,    21,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   208,   209,   210,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,   187,    -1,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   208,   209,   210,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,   209,
     210,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    20,    21,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    37,   147,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,   209,   210,    -1,
      -1,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
      -1,    -1,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,   141,   142,
     143,   144,   145,   146,   147,   113,   114,   115,   116,   117,
      20,    21,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,    -1,    -1,   201,   202,
      -1,    -1,    -1,    -1,    -1,   208,   209,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
      -1,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     208,   209,    -1,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,   139,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,   209,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,   139,    -1,   141,
     142,   143,    -1,    20,    21,   147,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    20,    21,
     147,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   208,   209,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    20,    21,
     147,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,   179,    20,    21,   147,    -1,    -1,    -1,    -1,
     187,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   209,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   208,   209,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    20,    21,   147,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,   179,    20,    21,
     147,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   208,   209,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    20,    21,
     147,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,   179,    20,    21,   147,    -1,    -1,    -1,    -1,
     187,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   209,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   208,   209,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    20,    21,   147,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,   179,    20,    21,
     147,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   208,   209,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    20,    21,
     147,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   209,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   208,   209,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     192,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,   167,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    -1,    96,    97,    98,
      99,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      71,    -1,    73,    74,    75,    76,    77,   155,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,   167,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   217,     0,     6,    29,    31,    33,    39,    49,    55,
      78,   100,   101,   179,   198,   209,   218,   221,   227,   229,
     230,   235,   264,   268,   300,   378,   385,   389,   400,   444,
     449,   454,    18,    19,   167,   257,   258,   259,   160,   236,
     237,   167,   168,   169,   198,   231,   232,   233,   177,   386,
     167,   213,   220,    56,    62,   381,   381,   381,   140,   167,
     287,    33,    62,   133,   202,   211,   260,   261,   262,   263,
     287,   179,     4,     5,     7,    35,   397,    61,   376,   186,
     185,   188,   185,   232,    21,    56,   197,   208,   234,   381,
     382,   384,   382,   376,   455,   445,   450,   167,   140,   228,
     262,   262,   262,   211,   141,   142,   143,   185,   210,    56,
      62,   269,   271,    56,    62,   387,    56,    62,   398,    56,
      62,   377,    14,    15,   160,   165,   167,   170,   211,   223,
     258,   160,   237,   167,   231,   231,   167,   179,   178,   382,
     179,    56,    62,   219,   167,   167,   167,   167,   171,   226,
     212,   259,   262,   262,   262,   262,   272,   167,   388,   401,
     177,   379,   171,   172,   222,    14,    15,   160,   165,   167,
     223,   255,   256,   234,   383,   179,   456,   446,   451,   171,
     212,    34,    69,    71,    73,    74,    75,    76,    77,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      91,    92,    93,    96,    97,    98,    99,   115,   116,   167,
     267,   270,   177,   188,   104,   395,   396,   374,   157,   350,
     171,   172,   173,   185,   212,   186,   177,   177,   177,    20,
      21,    37,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     125,   126,   127,   134,   135,   136,   137,   138,   141,   142,
     143,   144,   145,   146,   147,   187,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   201,   202,   208,   209,
      34,    34,   211,   265,   177,   273,   390,    73,    77,    96,
      97,    98,    99,   405,   167,   402,   178,   375,   259,   149,
     167,   372,   373,   255,    18,    24,    30,    40,    48,    63,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   153,   213,   287,   404,   406,   407,   410,
     415,   443,   457,   447,   452,   167,   167,   167,   210,    21,
     167,   210,   152,   212,   350,   360,   361,   188,   266,   276,
     380,   177,   188,   394,   177,   399,   350,   210,   211,    42,
     185,   188,   191,   371,   192,   192,   192,   211,   192,   192,
     211,   192,   192,   192,   192,   192,   192,   211,   287,    32,
      59,    60,   121,   125,   187,   191,   194,   209,   215,   414,
     189,   409,   364,   367,   167,   134,   211,    49,   149,   167,
     369,   403,   179,   212,   443,     1,     4,     5,     8,     9,
      10,    12,    14,    15,    16,    17,    18,    24,    25,    26,
      27,    28,    30,    37,    38,    41,    43,    44,    47,    50,
      51,    53,    54,    57,    58,    64,    67,    68,    78,   100,
     101,   102,   103,   115,   116,   130,   131,   132,   148,   149,
     150,   151,   152,   154,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   168,   169,   170,   177,   178,
     179,   194,   195,   196,   201,   202,   209,   211,   213,   214,
     225,   227,   238,   239,   242,   245,   246,   248,   250,   251,
     252,   253,   274,   275,   277,   281,   286,   287,   288,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   304,   305,
     312,   315,   318,   323,   326,   327,   329,   330,   331,   333,
     338,   341,   342,   349,   404,   459,   468,   479,   483,   490,
     493,   167,   179,   391,   392,   287,   356,   373,   210,    64,
     168,   281,   342,   167,   167,   415,   124,   134,   186,   370,
     416,   421,   423,   342,   425,   419,   167,   411,   427,   429,
     431,   433,   435,   437,   439,   441,   342,   192,   211,    32,
     191,    32,   191,   209,   215,   210,   342,   209,   215,   415,
     167,   179,   458,   167,   179,   362,   443,   448,   167,   179,
     365,   453,   342,   369,   211,    42,   124,   185,   186,   188,
     191,   368,   360,   192,   192,   342,   249,   192,   289,   406,
     459,   211,   192,   192,   211,   124,   286,   316,   327,   342,
     273,   192,   211,    60,   342,   211,   342,   167,   192,   192,
     211,   179,   192,   160,    57,   342,   211,   273,   192,   211,
     192,   192,   211,   192,   192,   124,   286,   342,   342,   342,
     214,   273,   318,   322,   322,   322,   211,   211,   211,   211,
     211,   211,    12,   415,    12,   415,    12,   342,   478,   488,
     192,   342,   192,   224,    12,   478,   489,    36,   342,   342,
     342,   342,   342,    12,   316,   342,   316,   214,   179,   179,
     342,     9,   318,   324,   167,   211,   179,   179,   179,   179,
     179,    65,   301,   264,   129,   179,    20,    21,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   133,   134,   135,   136,   137,   138,   141,   142,
     143,   144,   145,   146,   147,   186,   187,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   208,   209,   324,
     211,   179,   186,   178,   380,   178,   209,   264,   357,   212,
      42,   179,   370,   286,   342,   443,   443,   413,   443,   212,
     443,   443,   212,   167,   408,   443,   265,   443,   265,   443,
     265,   362,   363,   365,   366,   212,   418,   278,   316,   210,
     210,   342,   179,   178,   188,   179,   178,   188,   179,   178,
     212,   342,   167,   167,   443,   286,   342,   469,   471,   273,
     403,   290,   211,   287,   313,   484,   316,   286,   316,   185,
     105,   106,   107,   108,   109,   110,   111,   112,   118,   119,
     124,   137,   138,   144,   145,   146,   186,    13,   415,   489,
     342,   342,   273,   186,   306,   308,   342,   310,   188,   160,
     342,   486,   316,   475,   480,   316,   473,   415,   286,   342,
     214,   264,   342,   342,   342,   342,   342,   342,   403,    52,
     155,   167,   194,   209,   211,   342,   460,   461,   462,   477,
     482,   403,   211,   461,   482,   403,   139,   178,   179,   180,
     463,   282,   273,   284,   173,   174,   222,   403,   185,   492,
     177,   403,   185,   492,   492,   149,   154,   192,   287,   332,
     273,   247,   325,   209,   212,   316,   461,   157,   211,   303,
     373,   157,   321,   322,    18,   155,   167,   404,    18,   155,
     167,   404,   130,   131,   132,   274,   328,   342,   328,   342,
     328,   342,   328,   342,   328,   342,   328,   342,   328,   342,
     328,   342,   342,   342,   342,   328,   342,   328,   342,   342,
     342,   342,   167,   328,   342,   342,   155,   167,   342,   342,
     342,   404,   342,   342,   342,   328,   342,   328,   342,   342,
     342,   342,   328,   342,   328,   342,   328,   342,   342,   328,
     342,    21,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   126,   127,   155,   167,   208,   209,   339,
     342,   212,   316,   342,   393,   263,     7,   350,   355,   167,
     286,   342,   179,   193,   193,   193,   179,   193,   193,   179,
     193,   193,   266,   193,   266,   193,   266,   179,   193,   179,
     193,   280,   443,   212,   210,   443,   443,   212,    42,   368,
     415,   415,    19,   443,   212,   316,   461,   192,   342,   443,
     415,   492,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     273,   179,   193,   492,   212,   254,   415,   415,   212,   415,
     212,   415,   492,   415,   415,   492,   415,   193,   321,   212,
     212,   212,   212,   212,   212,    19,   322,   211,   134,   368,
     209,   342,   212,   139,   185,   179,   462,   182,   183,   210,
     466,   185,   179,   182,   210,   465,    19,   212,   462,   178,
     181,   464,    19,   342,   178,   478,   280,   280,   342,    19,
     478,   178,   276,    19,   210,   212,   211,   211,   334,   336,
      11,    22,    23,   240,   241,   342,   264,   460,   185,   212,
     212,   167,   302,   302,   124,   134,   186,   191,   319,   320,
     265,   192,   211,   192,   211,   322,   322,   322,   211,   211,
     210,    18,   155,   167,   404,   188,   155,   167,   342,   211,
     211,   155,   167,   342,     1,   210,   212,   179,   178,   210,
      56,    62,   353,    66,   354,   179,   179,   417,   422,   424,
     443,   426,   420,   167,   412,   428,   193,   432,   193,   436,
     193,   362,   440,   365,   442,   179,   193,   167,   342,   193,
     193,   316,   193,   212,   212,   167,   212,   193,   193,   212,
     415,   211,   212,   342,   193,   193,   193,   193,   212,   193,
     193,   212,   193,   321,   265,   211,   316,   342,   342,   342,
     167,   461,   462,   342,   155,   167,   460,   466,   210,   342,
     477,   210,   316,   461,   178,   181,   184,   467,   210,   316,
     193,   193,   175,   316,   178,   316,   342,   342,   415,   265,
     273,   342,    11,   243,   321,   210,   209,   185,   210,   212,
     167,   167,   167,   167,   185,   210,   266,   343,   342,   345,
     342,   212,   316,   342,   192,   211,   342,   211,   210,   342,
     212,   316,   211,   210,   340,   179,    46,   354,    45,   104,
     351,   430,   434,   438,   443,   211,   470,   472,   273,   291,
     179,   193,   314,   485,   193,   489,   307,   309,   311,   487,
     476,   481,   474,   211,   266,   212,   316,   179,   212,   462,
     466,   211,   134,   368,   179,   462,   210,   179,   283,   285,
     179,   179,   212,   212,   193,   266,   273,   244,   179,   265,
     212,   460,   167,   210,   188,   371,   319,   210,   139,   273,
     317,   415,   212,   443,   212,   212,   212,   347,   342,   342,
     212,   212,   342,    32,   352,   351,   353,   278,   211,   211,
     167,   342,   211,   211,   492,   342,   342,   342,   211,   211,
     211,   212,   342,   317,   212,   342,   134,   368,   466,   342,
     342,   342,   342,   467,   478,   342,   211,   279,   478,   342,
     335,   193,   241,    25,   103,   245,   293,   294,   295,   297,
     342,   266,   210,   188,   371,   415,   370,   124,   342,   193,
     193,   443,   212,   212,   212,   358,   352,   369,   212,   461,
     461,   193,   212,   316,   489,   212,   316,   461,   461,   212,
     179,   491,   342,   342,   212,   491,   491,   278,   491,   491,
     332,   337,   124,   124,   342,   273,   212,   415,   370,   370,
     342,   342,   344,   346,   193,   270,   359,   212,   212,   342,
     492,   492,   492,   212,   212,    52,   210,   134,   368,   178,
     178,   212,   178,   210,   332,   342,   370,   342,   342,   179,
     348,   179,   270,   212,   212,   212,   212,   342,   210,   342,
     342,   210,   178,   342,   179,   179,   273,   179
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   216,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   218,   219,   219,
     219,   220,   220,   221,   222,   222,   222,   222,   223,   224,
     224,   224,   225,   226,   226,   228,   227,   229,   230,   231,
     231,   231,   232,   232,   232,   232,   233,   233,   234,   234,
     235,   236,   236,   237,   237,   238,   239,   239,   240,   240,
     241,   241,   241,   242,   242,   243,   244,   243,   245,   245,
     245,   245,   245,   246,   247,   246,   249,   248,   250,   251,
     252,   254,   253,   255,   255,   255,   255,   255,   255,   256,
     256,   257,   257,   257,   258,   258,   258,   258,   258,   258,
     258,   258,   259,   259,   260,   260,   260,   261,   261,   262,
     262,   262,   262,   262,   262,   262,   263,   263,   264,   264,
     265,   265,   265,   266,   266,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     268,   269,   269,   269,   270,   272,   271,   273,   273,   274,
     274,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   276,   276,   276,   277,   278,   278,   279,   279,
     280,   280,   282,   283,   281,   284,   285,   281,   286,   286,
     286,   286,   286,   287,   287,   287,   288,   288,   290,   291,
     289,   289,   292,   292,   292,   292,   292,   293,   294,   295,
     295,   295,   296,   296,   296,   297,   297,   298,   298,   298,
     299,   300,   300,   301,   301,   302,   302,   303,   303,   303,
     303,   304,   304,   306,   307,   305,   308,   309,   305,   310,
     311,   305,   313,   314,   312,   315,   315,   315,   316,   316,
     317,   317,   317,   318,   318,   318,   319,   319,   319,   319,
     320,   320,   321,   321,   322,   322,   323,   325,   324,   326,
     326,   326,   326,   326,   326,   326,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   328,   328,   328,   328,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   330,   330,   331,   331,
     332,   332,   333,   334,   335,   333,   336,   337,   333,   338,
     338,   338,   338,   339,   340,   338,   341,   341,   341,   341,
     341,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   343,   344,   342,
     342,   342,   342,   345,   346,   342,   342,   342,   347,   348,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   350,   350,   351,   351,   351,   352,   352,   353,   353,
     353,   354,   354,   355,   356,   357,   356,   358,   356,   359,
     356,   356,   360,   360,   360,   361,   361,   362,   362,   363,
     363,   364,   364,   364,   365,   366,   366,   367,   367,   367,
     368,   368,   369,   369,   369,   369,   369,   369,   370,   370,
     370,   371,   371,   372,   372,   372,   372,   372,   373,   373,
     373,   373,   373,   374,   375,   374,   376,   376,   377,   377,
     377,   378,   379,   378,   380,   380,   380,   380,   381,   381,
     381,   383,   382,   384,   384,   385,   386,   385,   387,   387,
     387,   388,   390,   391,   389,   392,   393,   389,   394,   394,
     395,   395,   396,   397,   397,   398,   398,   398,   399,   399,
     401,   402,   400,   403,   403,   403,   403,   403,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   405,   405,   405,   405,   405,
     405,   406,   407,   407,   407,   408,   408,   409,   409,   409,
     411,   412,   410,   413,   413,   414,   414,   415,   415,   415,
     415,   415,   415,   416,   417,   415,   415,   415,   418,   415,
     415,   415,   415,   415,   415,   415,   415,   415,   415,   415,
     415,   415,   419,   420,   415,   415,   421,   422,   415,   423,
     424,   415,   425,   426,   415,   415,   427,   428,   415,   429,
     430,   415,   415,   431,   432,   415,   433,   434,   415,   415,
     435,   436,   415,   437,   438,   415,   439,   440,   415,   441,
     442,   415,   443,   443,   443,   445,   446,   447,   448,   444,
     450,   451,   452,   453,   449,   455,   456,   457,   458,   454,
     459,   459,   459,   459,   459,   460,   460,   460,   460,   460,
     460,   460,   460,   461,   461,   462,   462,   463,   463,   464,
     464,   465,   465,   466,   466,   466,   467,   467,   467,   468,
     468,   468,   468,   468,   468,   469,   470,   468,   471,   472,
     468,   473,   474,   468,   475,   476,   468,   477,   477,   477,
     478,   478,   479,   480,   481,   479,   482,   482,   483,   483,
     483,   483,   484,   485,   483,   483,   486,   487,   483,   488,
     488,   489,   489,   490,   490,   490,   490,   490,   491,   491,
     492,   492,   493,   493,   493,   493,   493
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       1,     1,     1,     4,     1,     1,     2,     2,     3,     0,
       2,     4,     3,     1,     2,     0,     4,     2,     2,     1,
       1,     1,     1,     2,     3,     3,     2,     4,     0,     1,
       2,     1,     3,     1,     3,     3,     3,     2,     1,     1,
       0,     2,     4,     1,     1,     0,     0,     3,     1,     1,
       1,     1,     1,     4,     0,     6,     0,     6,     2,     3,
       3,     0,     5,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     5,     1,     3,     1,     1,     1,     1,     4,     1,
       2,     3,     3,     3,     3,     2,     1,     3,     0,     3,
       0,     2,     3,     0,     2,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     3,     4,     3,     2,     2,     2,     2,
       2,     3,     3,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     1,     3,     0,     4,     3,     7,     2,
       2,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     2,
       2,     2,     0,     2,     2,     3,     0,     1,     0,     3,
       1,     3,     0,     0,     7,     0,     0,     7,     3,     2,
       2,     2,     1,     1,     3,     2,     2,     3,     0,     0,
       5,     1,     2,     4,     5,     5,     2,     1,     1,     1,
       2,     3,     2,     2,     3,     2,     3,     2,     2,     3,
       4,     1,     1,     1,     0,     1,     3,     9,     8,     8,
       7,     3,     3,     0,     0,     7,     0,     0,     7,     0,
       0,     7,     0,     0,     6,     5,     8,    10,     1,     3,
       1,     2,     3,     1,     1,     2,     2,     2,     2,     2,
       1,     3,     0,     4,     1,     6,     6,     0,     7,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     6,     8,     5,     6,
       1,     4,     3,     0,     0,     8,     0,     0,     9,     3,
       4,     5,     6,     0,     0,     5,     3,     4,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     4,     4,     5,     4,     5,     3,     4,     1,     1,
       2,     4,     4,     7,     8,     3,     5,     0,     0,     8,
       3,     3,     3,     0,     0,     8,     3,     4,     0,     0,
       9,     4,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     2,     4,     1,     4,     4,     4,     4,     4,
       1,     6,     7,     6,     6,     7,     7,     6,     7,     6,
       6,     0,     4,     0,     1,     1,     0,     1,     0,     1,
       1,     0,     1,     5,     0,     0,     4,     0,     9,     0,
      10,     5,     2,     3,     4,     1,     3,     1,     3,     1,
       3,     0,     2,     3,     3,     1,     3,     0,     2,     3,
       1,     1,     1,     2,     3,     5,     3,     3,     1,     1,
       1,     0,     1,     1,     4,     3,     3,     5,     4,     6,
       5,     5,     4,     0,     0,     4,     0,     1,     0,     1,
       1,     6,     0,     6,     0,     2,     3,     5,     0,     1,
       1,     0,     5,     2,     3,     4,     0,     4,     0,     1,
       1,     1,     0,     0,     9,     0,     0,    11,     0,     2,
       0,     1,     3,     1,     1,     0,     1,     1,     0,     3,
       0,     0,     7,     1,     4,     3,     3,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     1,     3,     0,     2,     3,
       0,     0,     6,     1,     3,     3,     4,     1,     1,     1,
       1,     2,     3,     0,     0,     6,     4,     5,     0,     9,
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
       4,     6,     7,     5,     7,     0,     0,     9,     0,     0,
       9,     0,     0,     9,     0,     0,     6,     1,     3,     3,
       3,     1,     5,     0,     0,     9,     1,     3,     4,     4,
       4,     5,     0,     0,    10,     5,     0,     0,    10,     1,
       3,     1,     3,     4,     3,     5,     8,    10,     0,     3,
       0,     1,     9,    10,    10,     9,    10
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

    case YYSYMBOL_TYPE_FUNCTION: /* "type function"  */
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

    case YYSYMBOL_expr_call_pipe: /* expr_call_pipe  */
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

    case YYSYMBOL_optional_expr_list: /* optional_expr_list  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_optional_expr_list_in_braces: /* optional_expr_list_in_braces  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_type_declaration_no_options_list: /* type_declaration_no_options_list  */
            { deleteTypeDeclarationList(((*yyvaluep).pTypeDeclList)); }
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

    case YYSYMBOL_expr_full_block: /* expr_full_block  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_full_block_assumed_piped: /* expr_full_block_assumed_piped  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_numeric_const: /* expr_numeric_const  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_assign: /* expr_assign  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_assign_pipe_right: /* expr_assign_pipe_right  */
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

    case YYSYMBOL_expr_call: /* expr_call  */
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

    case YYSYMBOL_optional_struct_variable_declaration_list: /* optional_struct_variable_declaration_list  */
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
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

    case YYSYMBOL_dim_list: /* dim_list  */
            { delete ((*yyvaluep).pTypeDecl); }
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

  case 17: /* top_level_reader_macro: expr_reader "end of expression"  */
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
        for ( auto & opt : *(yyvsp[0].aaList) ) {
            if ( opt.name=="indenting" && opt.type==Type::tInt ) {
                if (opt.iValue != 0 && opt.iValue != 2 && opt.iValue != 4 && opt.iValue != 8) { //this is error
                    yyextra->das_tab_size = yyextra->das_def_tab_size;
                } else {
                    yyextra->das_tab_size = opt.iValue ? opt.iValue : yyextra->das_def_tab_size;//0 is default
                }
                yyextra->g_FileAccessStack.back()->tabSize = yyextra->das_tab_size;
            } else if ( opt.name=="gen2_make_syntax" && opt.type==Type::tBool ) {
                yyextra->das_gen2_make_syntax = opt.bValue;
            }
        }
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
        delete (yyvsp[0].aaList);
    }
    break;

  case 39: /* keyword_or_name: "name"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 40: /* keyword_or_name: "keyword"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 41: /* keyword_or_name: "type function"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 42: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 43: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 44: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 45: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 46: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 47: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 48: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 49: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 53: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 54: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 55: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 56: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 57: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 58: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 59: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 60: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 61: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 62: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 63: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 64: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 65: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 66: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 67: /* expression_else_one_liner: "else" $@2 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 68: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 69: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 70: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 71: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 72: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 73: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 74: /* $@3: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 75: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner "end of expression"  */
                                                                                                                                                   {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 76: /* $@4: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 77: /* expression_for_loop: "for" $@4 variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                                                                 {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 78: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 79: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 80: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 81: /* $@5: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 82: /* expression_with_alias: "assume" "name" '=' $@5 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 83: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 84: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 85: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 86: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 87: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 88: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 89: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 90: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 91: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 92: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 93: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 94: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 95: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 96: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 97: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 98: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 99: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 100: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 101: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 102: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 103: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 104: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 105: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 106: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 107: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[0].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
                (yyval.fa)->annotation = ann;
            }
        } else {
            das_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[0])), CompilationError::invalid_annotation);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 108: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
                                                                                 {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[-3].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
                (yyval.fa)->annotation = ann;
            }
        } else {
            das_yyerror(scanner,"annotation " + *(yyvsp[-3].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[-3])), CompilationError::invalid_annotation);
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
    break;

  case 109: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 110: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 111: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 112: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 113: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 114: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 115: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 116: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 117: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 118: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 119: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 120: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 121: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 122: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 123: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 124: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 125: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 126: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 127: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 128: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 129: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 130: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 131: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 132: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 133: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 134: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 135: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 136: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 137: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 138: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 139: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 140: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 141: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 142: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 143: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 144: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 145: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 146: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 147: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 148: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 149: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 150: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 151: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 152: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 153: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 154: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 155: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 156: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 157: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 158: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 159: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 160: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 161: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 162: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 163: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 164: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 165: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 166: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 167: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 168: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 169: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 170: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 171: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 172: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 173: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 174: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 175: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 176: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 177: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 178: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 179: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 180: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 181: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 182: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 183: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 184: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 185: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 186: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 187: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 188: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 189: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 190: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 191: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 192: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 193: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 194: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 195: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 196: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 197: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 198: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 199: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 200: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 201: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 202: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 203: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 204: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 205: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 206: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 207: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 208: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 209: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 210: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
                                                                                {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-1]),(yylsp[0]));
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations(scanner, yyextra, (yyvsp[0].pFuncDecl), (yyvsp[-2].faList), tokAt(scanner,(yylsp[-2])));
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            implAddGenericFunction(scanner,(yyvsp[0].pFuncDecl));
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

  case 211: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 212: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 213: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 214: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 215: /* $@6: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 216: /* function_declaration: optional_public_or_private_function $@6 function_declaration_header expression_block  */
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

  case 217: /* expression_block: "begin of code block" expressions "end of code block"  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 218: /* expression_block: "begin of code block" expressions "end of code block" "finally" "begin of code block" expressions "end of code block"  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 219: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
                                                      {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            delete (yyvsp[0].pExpression);
        }
    }
    break;

  case 220: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
                                                                    {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            delete (yyvsp[0].pExpression);
        }
    }
    break;

  case 221: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 222: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 226: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 227: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 234: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 235: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 236: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 237: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 238: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 239: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 240: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 241: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 242: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 243: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 244: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 245: /* expr_keyword: "keyword" expr expression_block  */
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

  case 246: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 247: /* optional_expr_list: expr_list  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 248: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 249: /* optional_expr_list_in_braces: '(' optional_expr_list ')'  */
                                              { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 250: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 251: /* type_declaration_no_options_list: type_declaration_no_options_list "end of expression" type_declaration  */
                                                                           {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 252: /* $@7: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 253: /* $@8: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 254: /* expression_keyword: "keyword" '<' $@7 type_declaration_no_options_list '>' $@8 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 255: /* $@9: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 256: /* $@10: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 257: /* expression_keyword: "type function" '<' $@9 type_declaration_no_options_list '>' $@10 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 258: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 259: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 260: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 261: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 262: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 263: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 264: /* name_in_namespace: "name" "::" "name"  */
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

  case 265: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 266: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 267: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 268: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 269: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 270: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 271: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 272: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 273: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 274: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 275: /* expr_new: "new" new_type_declaration '(' make_struct_dim ')'  */
                                                                                   {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 276: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 277: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 278: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 279: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 280: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 281: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 282: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 283: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 284: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 285: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 286: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 287: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 288: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 289: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 290: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 291: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 292: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 293: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 294: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 295: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 296: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 297: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 298: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 299: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 300: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                  {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-5]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 301: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 302: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 303: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 304: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 305: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 306: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 307: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 308: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 309: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 310: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 311: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 312: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 313: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 314: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 315: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 316: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 317: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" "end of expression" "name" '>' expr ')'  */
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

  case 318: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 319: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 320: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 321: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 322: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 323: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 324: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 325: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 326: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 327: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 328: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 329: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 330: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 331: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 332: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 333: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 334: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 335: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 336: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 337: /* $@21: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 338: /* expr_full_block_assumed_piped: block_or_lambda $@21 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 339: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 340: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 341: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 342: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 343: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 344: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 345: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 346: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 347: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 366: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 367: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 368: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 369: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 387: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 388: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 389: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 390: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 391: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 392: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 393: /* $@22: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 394: /* $@23: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 395: /* func_addr_expr: '@' '@' '<' $@22 type_declaration_no_options '>' $@23 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 396: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 397: /* $@25: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 398: /* func_addr_expr: '@' '@' '<' $@24 optional_function_argument_list optional_function_type '>' $@25 func_addr_name  */
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

  case 399: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 400: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 401: /* expr_field: expr '.' "name" '(' ')'  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        das_yyerror(scanner,"foo.bar() method call syntax is not supported, use foo->bar() instead",
            tokAt(scanner,(yylsp[-3])),CompilationError::syntax_error);
    }
    break;

  case 402: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                       {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-5].pExpression)), *(yyvsp[-3].s));
        delete (yyvsp[-3].s);
        das_yyerror(scanner,"foo.bar(...) method call syntax is not supported, use foo->bar(...) instead",
            tokAt(scanner,(yylsp[-4])),CompilationError::syntax_error);
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 403: /* $@26: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 404: /* $@27: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 405: /* expr_field: expr '.' $@26 error $@27  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 406: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 407: /* expr_call: name_in_namespace '(' make_struct_dim ')'  */
                                                            {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = new TypeDecl(Type::alias);
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType->alias = *(yyvsp[-3].s);
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 408: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 409: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 410: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 411: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 412: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 413: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 414: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 415: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 416: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 417: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 418: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 419: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 420: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 421: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 422: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 423: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 424: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 425: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 426: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 427: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 428: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 429: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 430: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 431: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 432: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 433: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 435: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 436: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 439: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 440: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 441: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 442: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 443: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 444: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 445: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 447: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 448: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 449: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 450: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 451: /* expr: '(' expr_list optional_comma ')'  */
                                                         {
            if ( (yyvsp[-2].pExpression)->rtti_isSequence() ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
                mkt->values = sequenceToList((yyvsp[-2].pExpression));
                (yyval.pExpression) = mkt;
            } else if ( (yyvsp[-1].b) ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
                mkt->values.push_back((yyvsp[-2].pExpression));
                (yyval.pExpression) = mkt;
            } else {
                (yyval.pExpression) = (yyvsp[-2].pExpression);
            }
        }
    break;

  case 452: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 453: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 454: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 455: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 456: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 457: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 458: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 459: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 460: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 461: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 462: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 463: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 464: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 465: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 466: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 467: /* $@28: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 468: /* $@29: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 469: /* expr: expr "is" "type" '<' $@28 type_declaration_no_options '>' $@29  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 470: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 471: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 472: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 473: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 474: /* $@31: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 475: /* expr: expr "as" "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 476: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 477: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 478: /* $@32: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 479: /* $@33: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 480: /* expr: expr '?' "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 481: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 482: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 483: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 484: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 485: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 486: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 487: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 488: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 489: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 490: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 491: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 492: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 493: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 494: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 495: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 496: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 497: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 498: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 499: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 500: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 501: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 502: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 503: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 504: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 505: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 506: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 507: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 508: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 509: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 510: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 511: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 512: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 513: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 514: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 515: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 516: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 517: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 518: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 519: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 520: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 521: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 522: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 523: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 524: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 525: /* $@34: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 526: /* struct_variable_declaration_list: struct_variable_declaration_list $@34 structure_variable_declaration "end of expression"  */
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

  case 527: /* $@35: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 528: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@35 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 529: /* $@36: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 530: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@36 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 531: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' "end of expression"  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 532: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 533: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 534: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 535: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 536: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 537: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 538: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 539: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 540: /* tuple_type_list: tuple_type_list "end of expression" tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 541: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 542: /* tuple_alias_type_list: tuple_alias_type_list "end of expression"  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 543: /* tuple_alias_type_list: tuple_alias_type_list tuple_type "end of expression"  */
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

  case 544: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 545: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 546: /* variant_type_list: variant_type_list "end of expression" variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 547: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 548: /* variant_alias_type_list: variant_alias_type_list "end of expression"  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 549: /* variant_alias_type_list: variant_alias_type_list variant_type "end of expression"  */
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

  case 550: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 551: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 552: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 553: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 554: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 555: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 556: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 557: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 558: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 559: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 560: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 561: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 562: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 563: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 564: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 565: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 566: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 567: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 568: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 569: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 570: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 571: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 572: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 573: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 574: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 575: /* global_variable_declaration_list: global_variable_declaration_list $@37 optional_field_annotation let_variable_declaration  */
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

  case 576: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 577: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 578: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 579: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 580: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 581: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 582: /* $@38: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 583: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@38 optional_field_annotation let_variable_declaration  */
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

  case 584: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 585: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 586: /* enum_list: enum_list "name" "end of expression"  */
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

  case 587: /* enum_list: enum_list "name" '=' expr "end of expression"  */
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

  case 588: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 589: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 590: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 591: /* $@39: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 592: /* single_alias: optional_public_or_private_alias "name" $@39 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
        if ( (yyvsp[0].pTypeDecl)->baseType == Type::alias ) {
            das_yyerror(scanner,"alias cannot be defined in terms of another alias "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type);
        }
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

  case 596: /* $@40: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 598: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 599: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 600: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 601: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 602: /* $@41: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 603: /* $@42: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 604: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@41 enum_list $@42 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 605: /* $@43: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 606: /* $@44: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 607: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@43 enum_list $@44 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 608: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 609: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 610: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 611: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 612: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 613: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 614: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 615: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 616: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 617: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 618: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 619: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 620: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 621: /* $@46: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 622: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
                                                                                                                                                    {
        if ( (yyvsp[-2].pStructure) ) {
            ast_structureDeclaration ( scanner, (yyvsp[-6].faList), tokAt(scanner,(yylsp[-5])), (yyvsp[-2].pStructure), tokAt(scanner,(yylsp[-2])), (yyvsp[0].pVarDeclList) );
            if ( !yyextra->g_CommentReaders.empty() ) {
                auto tak = tokAt(scanner,(yylsp[-5]));
                for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-2].pStructure),tak);
            }
        } else {
            deleteVariableDeclarationList((yyvsp[0].pVarDeclList));
        }
    }
    break;

  case 623: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 624: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 625: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 626: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 627: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 628: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 629: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 630: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 631: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 632: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 633: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 634: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 635: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 636: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 637: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 638: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 639: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 640: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 641: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 642: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 643: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 644: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 645: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 646: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 647: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 648: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 649: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 650: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 651: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 652: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 653: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 654: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 655: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 656: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 657: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 658: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 659: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 660: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 661: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 662: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 663: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 664: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 665: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 666: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 667: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 668: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 669: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 670: /* $@47: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 671: /* $@48: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 672: /* bitfield_type_declaration: "bitfield" '<' $@47 bitfield_bits '>' $@48  */
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

  case 673: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 674: /* table_type_pair: type_declaration "end of expression" type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 675: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 676: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 677: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 678: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 679: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 680: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 681: /* type_declaration_no_options: type_declaration_no_options dim_list  */
                                                                {
        if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeDecl ) {
            das_yyerror(scanner,"type declaration can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_type);
        } else if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeMacro ) {
            das_yyerror(scanner,"macro can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_type);
        }
        (yyvsp[-1].pTypeDecl)->dim.insert((yyvsp[-1].pTypeDecl)->dim.begin(), (yyvsp[0].pTypeDecl)->dim.begin(), (yyvsp[0].pTypeDecl)->dim.end());
        (yyvsp[-1].pTypeDecl)->dimExpr.insert((yyvsp[-1].pTypeDecl)->dimExpr.begin(), (yyvsp[0].pTypeDecl)->dimExpr.begin(), (yyvsp[0].pTypeDecl)->dimExpr.end());
        (yyvsp[-1].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyvsp[0].pTypeDecl)->dimExpr.clear();
        delete (yyvsp[0].pTypeDecl);
    }
    break;

  case 682: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 683: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 684: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 685: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 686: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 687: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 688: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 689: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 690: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 691: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 692: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 693: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 694: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 695: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 696: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 697: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 698: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 699: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 700: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 701: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 702: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 703: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 704: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 705: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 706: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 707: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 708: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 709: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 710: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 711: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 712: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 713: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 714: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 715: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 716: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 717: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 718: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 719: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 720: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 721: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 722: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 723: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 724: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 725: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 726: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 727: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 728: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 729: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 730: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 731: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 732: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 733: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 735: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 736: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 737: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 738: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 739: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 740: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 741: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 742: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 743: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 744: /* type_declaration: type_declaration '|' '#'  */
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

  case 745: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 746: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 747: /* $@78: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 748: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 749: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 "begin of code block" $@78 tuple_alias_type_list $@79 "end of code block"  */
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

  case 750: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 751: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 752: /* $@82: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 753: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 754: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 "begin of code block" $@82 variant_alias_type_list $@83 "end of code block"  */
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

  case 755: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 756: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 757: /* $@86: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 758: /* $@87: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 759: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 "begin of code block" $@86 bitfield_alias_bits $@87 "end of code block"  */
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

  case 760: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 761: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 762: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 763: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 764: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 765: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 766: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 767: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 768: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 769: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 770: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 771: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 772: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 773: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 774: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 775: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 776: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 789: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 790: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 791: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 792: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 793: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 794: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 795: /* $@88: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 796: /* $@89: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 797: /* make_struct_decl: "struct" '<' $@88 type_declaration_no_options '>' $@89 '(' make_struct_dim ')'  */
                                                                                                                                                                                                        {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 798: /* $@90: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 799: /* $@91: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 800: /* make_struct_decl: "class" '<' $@90 type_declaration_no_options '>' $@91 '(' make_struct_dim ')'  */
                                                                                                                                                                                                       {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 801: /* $@92: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 802: /* $@93: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 803: /* make_struct_decl: "variant" '<' $@92 type_declaration_no_options '>' $@93 '(' make_struct_dim ')'  */
                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 804: /* $@94: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 805: /* $@95: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 806: /* make_struct_decl: "default" '<' $@94 type_declaration_no_options '>' $@95  */
                                                                                                                                      {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-5]));
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->useInitializer = true;
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 807: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 808: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 809: /* make_tuple: make_tuple ',' expr  */
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

  case 810: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 811: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 812: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 813: /* $@96: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 814: /* $@97: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 815: /* make_tuple_call: "tuple" '<' $@96 type_declaration_no_options '>' $@97 '(' make_struct_dim ')'  */
                                                                                                                                                                                                        {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 816: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 817: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 818: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 819: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 820: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 821: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 822: /* $@98: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 823: /* $@99: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 824: /* make_dim_decl: "array" '<' $@98 type_declaration_no_options '>' $@99 '(' expr_list optional_comma ')'  */
                                                                                                                                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 825: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 826: /* $@100: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 827: /* $@101: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 828: /* make_dim_decl: "fixed_array" '<' $@100 type_declaration_no_options '>' $@101 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 829: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 830: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 831: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 832: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 833: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 834: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 835: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 836: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-7])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 837: /* make_table_decl: "table" '<' type_declaration_no_options "end of expression" type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                                                                {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::tTuple);
        mka->makeType->argTypes.push_back((yyvsp[-7].pTypeDecl));
        mka->makeType->argTypes.push_back((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 838: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 839: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 840: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 841: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 842: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 843: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 844: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 845: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 846: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
                                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
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


