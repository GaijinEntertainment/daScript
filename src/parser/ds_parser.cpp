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
  YYSYMBOL_kwd_let_var_or_nothing = 300,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 301,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 302,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 303,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 304, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 305,           /* expression_let  */
  YYSYMBOL_expr_cast = 306,                /* expr_cast  */
  YYSYMBOL_307_13 = 307,                   /* $@13  */
  YYSYMBOL_308_14 = 308,                   /* $@14  */
  YYSYMBOL_309_15 = 309,                   /* $@15  */
  YYSYMBOL_310_16 = 310,                   /* $@16  */
  YYSYMBOL_311_17 = 311,                   /* $@17  */
  YYSYMBOL_312_18 = 312,                   /* $@18  */
  YYSYMBOL_expr_type_decl = 313,           /* expr_type_decl  */
  YYSYMBOL_314_19 = 314,                   /* $@19  */
  YYSYMBOL_315_20 = 315,                   /* $@20  */
  YYSYMBOL_expr_type_info = 316,           /* expr_type_info  */
  YYSYMBOL_expr_list = 317,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 318,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 319,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 320,            /* capture_entry  */
  YYSYMBOL_capture_list = 321,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 322,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 323,               /* expr_block  */
  YYSYMBOL_expr_full_block = 324,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 325, /* expr_full_block_assumed_piped  */
  YYSYMBOL_326_21 = 326,                   /* $@21  */
  YYSYMBOL_expr_numeric_const = 327,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 328,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 329,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 330,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 331,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 332,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 333,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 334,           /* func_addr_expr  */
  YYSYMBOL_335_22 = 335,                   /* $@22  */
  YYSYMBOL_336_23 = 336,                   /* $@23  */
  YYSYMBOL_337_24 = 337,                   /* $@24  */
  YYSYMBOL_338_25 = 338,                   /* $@25  */
  YYSYMBOL_expr_field = 339,               /* expr_field  */
  YYSYMBOL_340_26 = 340,                   /* $@26  */
  YYSYMBOL_341_27 = 341,                   /* $@27  */
  YYSYMBOL_expr_call = 342,                /* expr_call  */
  YYSYMBOL_expr = 343,                     /* expr  */
  YYSYMBOL_344_28 = 344,                   /* $@28  */
  YYSYMBOL_345_29 = 345,                   /* $@29  */
  YYSYMBOL_346_30 = 346,                   /* $@30  */
  YYSYMBOL_347_31 = 347,                   /* $@31  */
  YYSYMBOL_348_32 = 348,                   /* $@32  */
  YYSYMBOL_349_33 = 349,                   /* $@33  */
  YYSYMBOL_expr_mtag = 350,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 351, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 352,        /* optional_override  */
  YYSYMBOL_optional_constant = 353,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 354, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 355, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 356, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 357, /* struct_variable_declaration_list  */
  YYSYMBOL_358_34 = 358,                   /* $@34  */
  YYSYMBOL_359_35 = 359,                   /* $@35  */
  YYSYMBOL_360_36 = 360,                   /* $@36  */
  YYSYMBOL_function_argument_declaration = 361, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 362,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 363,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 364,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 365,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 366,             /* variant_type  */
  YYSYMBOL_variant_type_list = 367,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 368,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 369,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 370,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 371,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 372,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 373, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 374, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 375, /* global_variable_declaration_list  */
  YYSYMBOL_376_37 = 376,                   /* $@37  */
  YYSYMBOL_optional_shared = 377,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 378, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 379,               /* global_let  */
  YYSYMBOL_380_38 = 380,                   /* $@38  */
  YYSYMBOL_enum_list = 381,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 382, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 383,             /* single_alias  */
  YYSYMBOL_384_39 = 384,                   /* $@39  */
  YYSYMBOL_alias_list = 385,               /* alias_list  */
  YYSYMBOL_alias_declaration = 386,        /* alias_declaration  */
  YYSYMBOL_387_40 = 387,                   /* $@40  */
  YYSYMBOL_optional_public_or_private_enum = 388, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 389,                /* enum_name  */
  YYSYMBOL_enum_declaration = 390,         /* enum_declaration  */
  YYSYMBOL_391_41 = 391,                   /* $@41  */
  YYSYMBOL_392_42 = 392,                   /* $@42  */
  YYSYMBOL_393_43 = 393,                   /* $@43  */
  YYSYMBOL_394_44 = 394,                   /* $@44  */
  YYSYMBOL_optional_structure_parent = 395, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 396,          /* optional_sealed  */
  YYSYMBOL_structure_name = 397,           /* structure_name  */
  YYSYMBOL_class_or_struct = 398,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 399, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 400, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 401,    /* structure_declaration  */
  YYSYMBOL_402_45 = 402,                   /* $@45  */
  YYSYMBOL_403_46 = 403,                   /* $@46  */
  YYSYMBOL_variable_name_with_pos_list = 404, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 405,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 406, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 407, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 408,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 409,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 410,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 411, /* bitfield_type_declaration  */
  YYSYMBOL_412_47 = 412,                   /* $@47  */
  YYSYMBOL_413_48 = 413,                   /* $@48  */
  YYSYMBOL_table_type_pair = 414,          /* table_type_pair  */
  YYSYMBOL_dim_list = 415,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 416, /* type_declaration_no_options  */
  YYSYMBOL_417_49 = 417,                   /* $@49  */
  YYSYMBOL_418_50 = 418,                   /* $@50  */
  YYSYMBOL_419_51 = 419,                   /* $@51  */
  YYSYMBOL_420_52 = 420,                   /* $@52  */
  YYSYMBOL_421_53 = 421,                   /* $@53  */
  YYSYMBOL_422_54 = 422,                   /* $@54  */
  YYSYMBOL_423_55 = 423,                   /* $@55  */
  YYSYMBOL_424_56 = 424,                   /* $@56  */
  YYSYMBOL_425_57 = 425,                   /* $@57  */
  YYSYMBOL_426_58 = 426,                   /* $@58  */
  YYSYMBOL_427_59 = 427,                   /* $@59  */
  YYSYMBOL_428_60 = 428,                   /* $@60  */
  YYSYMBOL_429_61 = 429,                   /* $@61  */
  YYSYMBOL_430_62 = 430,                   /* $@62  */
  YYSYMBOL_431_63 = 431,                   /* $@63  */
  YYSYMBOL_432_64 = 432,                   /* $@64  */
  YYSYMBOL_433_65 = 433,                   /* $@65  */
  YYSYMBOL_434_66 = 434,                   /* $@66  */
  YYSYMBOL_435_67 = 435,                   /* $@67  */
  YYSYMBOL_436_68 = 436,                   /* $@68  */
  YYSYMBOL_437_69 = 437,                   /* $@69  */
  YYSYMBOL_438_70 = 438,                   /* $@70  */
  YYSYMBOL_439_71 = 439,                   /* $@71  */
  YYSYMBOL_440_72 = 440,                   /* $@72  */
  YYSYMBOL_441_73 = 441,                   /* $@73  */
  YYSYMBOL_442_74 = 442,                   /* $@74  */
  YYSYMBOL_443_75 = 443,                   /* $@75  */
  YYSYMBOL_type_declaration = 444,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 445,  /* tuple_alias_declaration  */
  YYSYMBOL_446_76 = 446,                   /* $@76  */
  YYSYMBOL_447_77 = 447,                   /* $@77  */
  YYSYMBOL_448_78 = 448,                   /* $@78  */
  YYSYMBOL_449_79 = 449,                   /* $@79  */
  YYSYMBOL_variant_alias_declaration = 450, /* variant_alias_declaration  */
  YYSYMBOL_451_80 = 451,                   /* $@80  */
  YYSYMBOL_452_81 = 452,                   /* $@81  */
  YYSYMBOL_453_82 = 453,                   /* $@82  */
  YYSYMBOL_454_83 = 454,                   /* $@83  */
  YYSYMBOL_bitfield_alias_declaration = 455, /* bitfield_alias_declaration  */
  YYSYMBOL_456_84 = 456,                   /* $@84  */
  YYSYMBOL_457_85 = 457,                   /* $@85  */
  YYSYMBOL_458_86 = 458,                   /* $@86  */
  YYSYMBOL_459_87 = 459,                   /* $@87  */
  YYSYMBOL_make_decl = 460,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 461,       /* make_struct_fields  */
  YYSYMBOL_make_struct_single = 462,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 463,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 464,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 465, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 466, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 467, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 468, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 469, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_make_struct_decl = 470,         /* make_struct_decl  */
  YYSYMBOL_471_88 = 471,                   /* $@88  */
  YYSYMBOL_472_89 = 472,                   /* $@89  */
  YYSYMBOL_473_90 = 473,                   /* $@90  */
  YYSYMBOL_474_91 = 474,                   /* $@91  */
  YYSYMBOL_475_92 = 475,                   /* $@92  */
  YYSYMBOL_476_93 = 476,                   /* $@93  */
  YYSYMBOL_477_94 = 477,                   /* $@94  */
  YYSYMBOL_478_95 = 478,                   /* $@95  */
  YYSYMBOL_make_tuple = 479,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 480,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 481,          /* make_tuple_call  */
  YYSYMBOL_482_96 = 482,                   /* $@96  */
  YYSYMBOL_483_97 = 483,                   /* $@97  */
  YYSYMBOL_make_dim = 484,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 485,            /* make_dim_decl  */
  YYSYMBOL_486_98 = 486,                   /* $@98  */
  YYSYMBOL_487_99 = 487,                   /* $@99  */
  YYSYMBOL_488_100 = 488,                  /* $@100  */
  YYSYMBOL_489_101 = 489,                  /* $@101  */
  YYSYMBOL_make_table = 490,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 491,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 492,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 493, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 494,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 495       /* array_comprehension  */
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
#define YYLAST   14253

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  216
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  280
/* YYNRULES -- Number of rules.  */
#define YYNRULES  851
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1582

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
       0,   532,   532,   533,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   554,   560,   561,
     562,   566,   567,   571,   589,   590,   591,   592,   596,   600,
     605,   614,   622,   638,   643,   651,   651,   690,   720,   724,
     725,   726,   730,   733,   737,   743,   752,   755,   761,   762,
     766,   770,   771,   775,   778,   784,   790,   793,   799,   800,
     804,   805,   806,   815,   816,   820,   821,   821,   827,   828,
     829,   830,   831,   835,   841,   841,   847,   847,   853,   861,
     871,   880,   880,   887,   888,   889,   890,   891,   892,   896,
     901,   909,   910,   911,   915,   916,   917,   918,   919,   920,
     921,   922,   928,   931,   937,   938,   939,   943,   956,   974,
     977,   985,   996,  1007,  1018,  1021,  1028,  1032,  1039,  1040,
    1044,  1045,  1046,  1050,  1053,  1060,  1064,  1065,  1066,  1067,
    1068,  1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,
    1078,  1079,  1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,
    1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,
    1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,
    1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,
    1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,
    1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,
    1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,
    1152,  1170,  1171,  1172,  1176,  1182,  1182,  1199,  1203,  1214,
    1223,  1235,  1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,
    1244,  1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,
    1254,  1255,  1259,  1264,  1270,  1276,  1287,  1288,  1292,  1293,
    1297,  1301,  1308,  1308,  1308,  1314,  1314,  1314,  1323,  1357,
    1360,  1363,  1366,  1372,  1373,  1384,  1388,  1391,  1399,  1399,
    1399,  1402,  1408,  1411,  1414,  1418,  1425,  1431,  1435,  1439,
    1442,  1445,  1453,  1456,  1459,  1467,  1470,  1478,  1481,  1484,
    1492,  1498,  1499,  1500,  1504,  1505,  1509,  1510,  1514,  1519,
    1527,  1533,  1539,  1548,  1560,  1563,  1569,  1569,  1569,  1572,
    1572,  1572,  1577,  1577,  1577,  1585,  1585,  1585,  1591,  1601,
    1612,  1627,  1630,  1636,  1637,  1644,  1655,  1656,  1657,  1661,
    1662,  1663,  1664,  1668,  1673,  1681,  1682,  1686,  1691,  1698,
    1705,  1705,  1714,  1715,  1716,  1717,  1718,  1719,  1720,  1724,
    1725,  1726,  1727,  1728,  1729,  1730,  1731,  1732,  1733,  1734,
    1735,  1736,  1737,  1738,  1739,  1740,  1741,  1742,  1746,  1747,
    1748,  1749,  1754,  1755,  1756,  1757,  1758,  1759,  1760,  1761,
    1762,  1763,  1764,  1765,  1766,  1767,  1768,  1769,  1770,  1775,
    1782,  1794,  1799,  1809,  1813,  1820,  1823,  1823,  1823,  1828,
    1828,  1828,  1841,  1845,  1849,  1854,  1861,  1861,  1861,  1868,
    1872,  1881,  1885,  1888,  1894,  1895,  1896,  1897,  1898,  1899,
    1900,  1901,  1902,  1903,  1904,  1905,  1906,  1907,  1908,  1909,
    1910,  1911,  1912,  1913,  1914,  1915,  1916,  1917,  1918,  1919,
    1920,  1921,  1922,  1923,  1924,  1925,  1926,  1927,  1928,  1929,
    1935,  1936,  1937,  1938,  1939,  1952,  1953,  1954,  1955,  1956,
    1957,  1958,  1959,  1960,  1961,  1962,  1963,  1966,  1969,  1970,
    1973,  1973,  1973,  1976,  1981,  1985,  1989,  1989,  1989,  1994,
    1997,  2001,  2001,  2001,  2006,  2009,  2010,  2011,  2012,  2013,
    2014,  2015,  2016,  2017,  2019,  2023,  2024,  2029,  2033,  2034,
    2035,  2036,  2037,  2038,  2039,  2043,  2047,  2051,  2055,  2059,
    2063,  2067,  2071,  2075,  2082,  2083,  2087,  2088,  2089,  2093,
    2094,  2098,  2099,  2100,  2104,  2105,  2109,  2120,  2123,  2123,
    2142,  2141,  2156,  2155,  2168,  2177,  2186,  2196,  2197,  2201,
    2204,  2213,  2214,  2218,  2221,  2224,  2240,  2249,  2250,  2254,
    2257,  2260,  2274,  2275,  2279,  2285,  2291,  2294,  2298,  2304,
    2313,  2314,  2315,  2319,  2320,  2324,  2331,  2336,  2345,  2351,
    2362,  2365,  2370,  2375,  2383,  2394,  2397,  2397,  2417,  2418,
    2422,  2423,  2424,  2428,  2431,  2431,  2450,  2453,  2456,  2471,
    2490,  2491,  2492,  2497,  2497,  2523,  2524,  2528,  2529,  2529,
    2533,  2534,  2535,  2539,  2549,  2554,  2549,  2566,  2571,  2566,
    2586,  2587,  2591,  2592,  2596,  2602,  2603,  2607,  2608,  2609,
    2613,  2616,  2622,  2627,  2622,  2641,  2648,  2653,  2662,  2668,
    2679,  2680,  2681,  2682,  2683,  2684,  2685,  2686,  2687,  2688,
    2689,  2690,  2691,  2692,  2693,  2694,  2695,  2696,  2697,  2698,
    2699,  2700,  2701,  2702,  2703,  2704,  2705,  2709,  2710,  2711,
    2712,  2713,  2714,  2715,  2716,  2720,  2731,  2735,  2742,  2754,
    2761,  2770,  2775,  2778,  2791,  2791,  2791,  2804,  2808,  2815,
    2819,  2826,  2827,  2828,  2829,  2830,  2845,  2851,  2851,  2851,
    2855,  2860,  2867,  2867,  2874,  2878,  2882,  2887,  2892,  2897,
    2902,  2906,  2910,  2915,  2919,  2923,  2928,  2928,  2928,  2934,
    2941,  2941,  2941,  2946,  2946,  2946,  2952,  2952,  2952,  2957,
    2961,  2961,  2961,  2966,  2966,  2966,  2975,  2979,  2979,  2979,
    2984,  2984,  2984,  2993,  2997,  2997,  2997,  3002,  3002,  3002,
    3011,  3011,  3011,  3017,  3017,  3017,  3026,  3029,  3040,  3056,
    3056,  3061,  3066,  3056,  3091,  3091,  3096,  3102,  3091,  3127,
    3127,  3132,  3137,  3127,  3167,  3168,  3169,  3170,  3171,  3175,
    3182,  3189,  3195,  3201,  3208,  3215,  3221,  3231,  3239,  3244,
    3251,  3252,  3257,  3258,  3262,  3263,  3267,  3268,  3272,  3273,
    3274,  3278,  3279,  3280,  3285,  3291,  3298,  3306,  3313,  3321,
    3330,  3330,  3330,  3338,  3338,  3338,  3345,  3345,  3345,  3352,
    3352,  3352,  3363,  3366,  3372,  3386,  3392,  3398,  3404,  3404,
    3404,  3414,  3419,  3426,  3434,  3439,  3446,  3454,  3454,  3454,
    3462,  3469,  3469,  3469,  3479,  3484,  3491,  3494,  3500,  3508,
    3517,  3525,  3533,  3546,  3547,  3551,  3552,  3557,  3560,  3563,
    3566,  3569
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
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@13", "$@14", "$@15", "$@16", "$@17",
  "$@18", "expr_type_decl", "$@19", "$@20", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block", "expr_full_block",
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
  "make_decl", "make_struct_fields", "make_struct_single",
  "make_struct_dim", "optional_block",
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

#define YYPACT_NINF (-1426)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-738)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1426,    40, -1426, -1426,    49,  -103,   137,    18, -1426,   -92,
     577,   577,   577, -1426,    23,    63, -1426, -1426,    64, -1426,
   -1426, -1426,   303, -1426,    74, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426,    78, -1426,   111,   192,   221,
   -1426, -1426, -1426, -1426,   137, -1426,    16, -1426,   577,   577,
   -1426, -1426,    74, -1426, -1426, -1426, -1426, -1426,   252,   294,
   -1426, -1426, -1426,    63,    63,    63,   307, -1426,   638,  -104,
   -1426, -1426, -1426, -1426,   587,   592,   620, -1426,   644,    47,
      49,   345,  -103,   323,   436, -1426,   624,   624, -1426,   441,
     446,    32,   505,   648,   489,   526,   529, -1426,   563,   554,
   -1426, -1426,   -44,    49,    63,    63,    63,    63, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426,   589, -1426, -1426, -1426, -1426,
   -1426,   584, -1426, -1426, -1426, -1426, -1426,     7,   115, -1426,
   -1426, -1426, -1426,   714, -1426, -1426, -1426, -1426, -1426,   598,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,   600,
   -1426,    91, -1426,   578,   661,   638, 14086, -1426,   143,   685,
   -1426,   653, -1426, -1426,   623, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426,   161, -1426,   628, -1426,   629,   639,   641, -1426,
   -1426, 12715, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426,   785,   795, -1426,
     631,   654, -1426,   677, -1426,   665, -1426,   656,    49,   368,
   -1426, -1426, -1426,   115, -1426,  9672, -1426, -1426, -1426,   668,
     676, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426,   678,   636, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426,   826, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,   681,   642,
   -1426, -1426,   -26,   663, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426,   672,   671,   684, -1426,   653,
      -1,   652,   811,   495, -1426, -1426,   673,   679,   680,   657,
     682,   683, -1426, -1426, -1426,   659, -1426, -1426, -1426, -1426,
   -1426,   686, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426,   687, -1426, -1426, -1426,   688,   689, -1426,
   -1426, -1426, -1426,   690,   691,   662,    23, -1426, -1426, -1426,
   -1426, -1426,    79,   695, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426,   710,   751, -1426,   675, -1426,    53, -1426,   -24,  9672,
   -1426,  2345,   430, -1426,    23, -1426, -1426, -1426,   368,   692,
    9002,   720,   721,  9672, -1426,   103, -1426, -1426, -1426,  9002,
   -1426, -1426,   722, -1426,   296,   300,   326, -1426, -1426,  9002,
      70, -1426, -1426, -1426,    -4, -1426, -1426, -1426,    48,  5177,
   -1426,   696,  9462,   444,  9561,   457, -1426, -1426,  9002, -1426,
   -1426,   416,   184, -1426,   695, -1426,   698,   708,  9002, -1426,
   -1426, -1426, -1426, -1426,   736,   699,   709,   122,  3161, -1426,
   -1426,   654,   258,  5378,   700,  9002,   730,   715,   717,   701,
   -1426,   737,   723,   757,  5579,   -28,   305,   726, -1426,   338,
     727,   729,  3362,  9002,  9002,    84,    84,    84,   711,   712,
     713,   718,   724,   728, -1426,   986,  9363,  5782, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426,  5983,   733, -1426,  6186,   890,
   -1426,  9002,  9002,  9002,  9002,  9002,  6389,  9002, -1426,   719,
   -1426, -1426,   755,   758,  9002,   929, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426,   151, -1426,   -55,   761, -1426,
     762,   763,   764, -1426,   765, -1426, -1426,   881, -1426, -1426,
   -1426, -1426,   742, -1426, -1426,   -42, -1426, -1426, -1426, -1426,
   -1426, -1426,  1966, -1426,   741, -1426, -1426, -1426, -1426, -1426,
   -1426,   257, -1426,   775, -1426, -1426,    24, -1426, -1426,   743,
     771, -1426, 10117, -1426,   913,  1647, -1426, -1426, -1426,  3764,
    9672,  9672,  9672, 10226,  9672,  9672,   744,   797,  9672,   631,
    9672,   631,  9672,   631,  9771,   798, 10261, -1426,  9002, -1426,
   -1426, -1426, -1426,   756, -1426, -1426, 12242,  9002, -1426,    79,
     789, -1426,   804,  -102, -1426,   794,   695,   805,   801, -1426,
     806,   808, 10370,   773,   948, -1426,   -69, -1426, -1426, -1426,
   12750,   416, -1426,   780, -1426, -1426,    23, -1426, -1426,  9002,
    3764, -1426,   807,   867,  9853,   984,  9672,  9002,  9002, 13481,
    9002, 12750,   813, -1426, -1426,  9002, -1426, -1426,   812,   841,
   13481,  9002, -1426, -1426,  9002, -1426, -1426,  9002, -1426,  9672,
    3764, -1426,  9853,   575,   575,   791, -1426,   742, -1426, -1426,
   -1426,  9002,  9002,  9002,  9002,  9002,  9002,   416,  1715,   416,
    2757,   416, 12848, -1426,   621, -1426, 12750, -1426,   614,   416,
   -1426,   821,   835,   575,   575,   196,   575,   575,   416,   828,
   13481,   828,   293, -1426, -1426, 12750, -1426, -1426, -1426, -1426,
    4166, -1426, -1426, -1426, -1426, -1426, -1426,   110,   857,    84,
   -1426, 13883, 13978,  3965,  3965,  3965,  3965,  3965,  3965,  3965,
    3965,  9002,  9002, -1426, -1426,  9002,  3965,  3965,  9002,  9002,
    9002,   850,  3965,  9002,   473,  9002,  9002,  9002,  9002,  9002,
    9002,  3965,  3965,  9002,  9002,  9002,  3965,  3965,  3965,  9002,
    3965,  6590,  9002,  9002,  9002,  9002,  9002,  9002,  9002,  9002,
    9002,  9002,   175,  9002, -1426,  6791, -1426,  9002, -1426,   430,
   -1426,    63,  1011,   653, -1426,   852, -1426,  3764, -1426,  9972,
     -89,   256,   827,   440, -1426,   361,   492, -1426, -1426,   169,
     531,   663,   549,   663,   558,   663, -1426,   316, -1426,   383,
   -1426,  9672,   810,   807, -1426, -1426, 12277, -1426, -1426,  9672,
   -1426, -1426,  9672, -1426, -1426, -1426,  9002,   856, -1426,   858,
   -1426,  9672, -1426,  3764,  9672,  9672, -1426,    33,  9672,  4367,
    6992,  9672,  9672,   828, -1426,   807,  9002,  9002,  9002,  9002,
    9002,  9002,  9002,  9002,  9002,  9002,  9002,  9002,  9002,  9002,
    9002,  9002,  9002,  9002,   654,   354,   821, 13481, 10405, -1426,
   -1426,  9672,  9672, 10514,  9672, -1426, -1426, 10549,  9672,   828,
    9672,  9672,   828,  9672,   188, -1426,  9853, -1426,   857, 10658,
   10693, 10802, 10837, 10946, 10981,    51,    84,   816,    42,  2959,
    4570,  7193, 12946,   839,     1,   129,   848,   -18,    59,  7394,
       1,   408,    65,  9002,   859,  9002, -1426, -1426,  9672, -1426,
    9672, -1426,  9002,   574,    66,  9002,   860, -1426,    86,  9002,
     831,   824,   833,   836,   332, -1426, -1426,   583,  9002,   742,
    4773, -1426,   200,   839,   876,   878,   878, -1426, -1426,   101,
     631, -1426,   861,   837, -1426, -1426,   899,   882, -1426, -1426,
      84,    84,    84, -1426, -1426,  1473, -1426,  1473, -1426,  1473,
   -1426,  1473, -1426,  1473, -1426,  1473, -1426,  1473, -1426,  1473,
     974,   974,   834, -1426,  1473, -1426,  1473,   834,  9590,  9590,
     883, -1426,  1473,   201,   885, -1426, 12375,    -3,    -3,   741,
   13481,   974,   974, -1426,  1473, -1426,  1473,  1216,  1925, 13696,
   -1426,  1473, -1426,  1473, -1426,  1473, 13571, -1426,  1473, 14009,
   12981, 13725,  2111, 13815,   834,   834,    44,    44,   201,   201,
     201,   475,  9002,   892,   893,   477,  9002,  1091, 12473, -1426,
     244, 13071,   927,    46,   651,  1040,   930, -1426, -1426, 10082,
   -1426, -1426, -1426, -1426,  9672, -1426, -1426,   941, -1426, -1426,
     917, -1426,   918, -1426,   919,  9771, -1426,   798, -1426,   388,
     695, -1426, -1426,   695,   695, 11090, -1426,  1071,   -76, -1426,
    9853,   540,   705,  9002,   559, -1426,   245,   903,   949, 11125,
     564,  2677,   905, 13481, 13481, 13481, 13481, 13481, 13481, 13481,
   13481, 13481, 13481, 13481, 13481, 13481, 13481, 13481, 13481, 13481,
   13481, -1426,  9672,   910,   911, -1426,  9002,  2897,  3282, -1426,
    3483, -1426,  3684,   912,  3885,  4086,   915,  4287,   857,   631,
   -1426, -1426, -1426, -1426, -1426,   914,  9002, -1426,  9002,  9002,
    9002,  4976, 12242,   -22,  9002,   491,   512,   129, -1426, -1426,
     920, -1426,  9002,  9002, -1426,   921, -1426,  9002,   512,   511,
     922, -1426, -1426,  9002, 13481, -1426, -1426,   398,   400, 13106,
    9002, -1426, -1426,  2556,  9002, -1426, -1426,  9002,  9002,  9672,
     631,   654, -1426, -1426,  9002, -1426,  1181,   857,   130,  9203,
   -1426, -1426, -1426,   133,   246,   955,   961,   966,   967, -1426,
     263,   663, -1426,  9002, -1426,  9002, -1426, -1426, -1426,  7595,
    9002, -1426,   943,   925, -1426, -1426,  9002,   931, -1426, 12508,
    9002,  7796,   932, -1426, 12606, -1426, -1426, -1426, -1426, -1426,
     959, -1426, -1426,   389, -1426,    38, -1426, -1426, -1426, -1426,
   -1426,   695, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426,  9672,   933, -1426,   973,
    9002, -1426, -1426,   407, -1426, -1426, -1426,   409, -1426, -1426,
   -1426, -1426,  4488,  9002, -1426, 13481, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426,   934,   663,  7997,   530, 11234,
   13481, 13481,   159,     1,   129, 13481,   935,   166,   839, -1426,
   -1426, 13481,   848, -1426,   532,     1,   937, -1426, -1426, -1426,
   -1426,   533, -1426, -1426, -1426,   543, -1426,   544, 11269, 11378,
    4567,   663, -1426, 12750, -1426,   962,   631,   936,  4773,   982,
     940,   364, -1426, -1426, -1426, -1426,   101,   942,    12,  9672,
   11413,  9672, 11522, -1426,   259, 11557, -1426,  9002, 13606,  9002,
   -1426, 11666, -1426,   274,  9002, -1426, -1426, -1426,  1119,    38,
   -1426, -1426,   651, -1426, -1426, -1426,   695,  9002, -1426, 13481,
     944,   945, -1426, -1426,   987,  9002, -1426,   946,   947,   821,
    9002,  9002,  9002,   950, -1426,   951,   952,  8198,    12, -1426,
     280,  9002,   213,   129, -1426,  9002,  9002,  9002,  9002,   511,
   -1426,  9002,  9002,   953,  9002,  9002, -1426, -1426, -1426,   980,
     583,  3563, -1426,   663, -1426,   298, -1426,   447,  9672,   103,
   -1426, -1426,  8399, -1426, -1426,  4691, -1426,   569, -1426, -1426,
   -1426,  9672, 11701, 11810, -1426, -1426, 11845, -1426, -1426,  1119,
     416,   954,   512,   512,   981, 11954,  9002,  9002,   963,   834,
     834,   834,  9002,   512,   512, -1426, 11989, -1426, -1426, 13196,
    9002,  9002, -1426, 12098, 13481, 13481, 13196, -1426,   998,   834,
    9002, -1426,   998, 13196,   297, -1426, -1426,  8600,  8801, -1426,
   -1426, -1426, -1426, -1426, 13481,   654,   968,  9672,   103,  1294,
    9002,  9002, 13571, -1426, -1426,   570, -1426, -1426, -1426, 14086,
   -1426, -1426, -1426,   969,   972,  9002, -1426,   828,   821, -1426,
     828,   975,   976, -1426,  1107,   979, 13481, 13481,   253,  1000,
    1008,   978,  1013,   983, -1426,   297,  9002,  9002, 13481, -1426,
   -1426,  1294,  9002,  9002, 13231, 13606, -1426, -1426, -1426,  1015,
   14086, -1426, -1426, 12133,   988,   991,   992, -1426, -1426,  9002,
     996,  9002,  9002,   997,  1018, -1426, -1426, -1426, -1426, 13481,
    9002, 13321, 13356, -1426, -1426, -1426,   654, -1426, -1426, -1426,
   -1426, 13481, -1426, 13481, 13481, -1426, -1426, 13446, -1426, -1426,
   -1426, -1426
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   118,     1,   294,     0,     0,     0,   598,   295,     0,
     590,   590,   590,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   578,     6,    11,     5,     4,    13,
      12,    14,    92,    93,    91,   100,   102,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   590,   590,
      22,    21,   578,   592,   591,   759,   749,   754,     0,   263,
      35,   105,   106,     0,     0,     0,   107,   109,   116,     0,
     104,    17,   616,   615,   211,   600,   617,   579,   580,     0,
       0,     0,     0,    43,     0,    49,     0,     0,    46,     0,
       0,   590,     0,    18,     0,     0,     0,   265,     0,     0,
     115,   110,     0,     0,     0,     0,     0,     0,   119,   213,
     212,   215,   210,   602,   601,     0,   619,   618,   622,   582,
     581,   584,    98,    99,    96,    97,    95,     0,     0,    94,
     103,    54,    52,    48,    45,    44,   593,   595,   597,     0,
     599,    19,    20,    23,   760,   750,   755,   264,    33,    36,
     114,     0,   111,   112,   113,   117,     0,   603,     0,   612,
     575,   514,    24,    25,     0,    87,    88,    85,    86,    84,
      83,    89,     0,    47,     0,   596,     0,     0,     0,    34,
     108,     0,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,     0,     0,   125,
     120,     0,   604,     0,   613,     0,   623,   576,     0,     0,
      26,    27,    28,     0,   101,     0,   761,   751,   756,   179,
     180,   177,   128,   129,   131,   130,   132,   133,   134,   135,
     161,   162,   159,   160,   152,   163,   164,   153,   150,   151,
     178,   172,     0,   176,   165,   166,   167,   168,   139,   140,
     141,   136,   137,   138,   149,     0,   155,   156,   154,   147,
     148,   143,   142,   144,   145,   146,   127,   126,   171,     0,
     157,   158,   514,   123,   242,   216,   586,   657,   660,   663,
     664,   658,   661,   659,   662,     0,   610,   620,   583,   514,
       0,     0,   565,   563,   585,    90,     0,     0,     0,     0,
       0,     0,   630,   650,   631,   666,   632,   636,   637,   638,
     639,   656,   643,   644,   645,   646,   647,   648,   649,   651,
     652,   653,   654,   719,   635,   642,   655,   726,   733,   633,
     640,   634,   641,     0,     0,     0,     0,   665,   681,   684,
     682,   683,   746,   594,   671,   543,   549,   181,   182,   175,
     170,   183,   173,   169,     0,   121,   293,   537,     0,     0,
     214,     0,   605,   607,     0,   614,   527,   624,     0,     0,
       0,     0,     0,     0,   564,     0,   687,   710,   713,     0,
     716,   706,     0,   674,   720,   727,   734,   740,   743,     0,
       0,   696,   701,   695,     0,   709,   705,   698,     0,     0,
     700,   685,     0,   762,   752,   757,   184,   174,     0,   291,
     292,     0,   514,   122,   124,   244,     0,     0,     0,    63,
      64,    76,   420,   421,     0,     0,     0,     0,   279,   414,
     277,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     278,     0,     0,     0,     0,     0,     0,     0,   656,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   503,     0,     0,     0,   342,   344,
     343,   345,   346,   347,   348,     0,     0,    29,     0,   217,
     221,     0,     0,     0,     0,     0,     0,     0,   326,   327,
     418,   417,     0,     0,     0,     0,   237,   232,   229,   228,
     230,   231,   262,   243,   223,   497,   222,   415,     0,   488,
      71,    72,    69,   235,    70,   236,   238,   297,   227,   487,
     486,   485,   118,   491,   416,     0,   224,   490,   489,   461,
     422,   462,   349,   423,     0,   419,   764,   768,   765,   766,
     767,     0,   587,     0,   586,   611,   528,   577,   515,     0,
       0,   497,     0,   567,   568,     0,   561,   562,   560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   120,
       0,   120,     0,   120,     0,     0,     0,   692,   246,   703,
     704,   697,   699,     0,   702,   686,     0,     0,   748,   747,
       0,   672,     0,   263,   544,     0,   539,     0,     0,   550,
       0,     0,     0,     0,   625,   535,   554,   538,   800,   803,
       0,     0,   268,   272,   271,   276,     0,   315,   827,     0,
       0,   283,   280,     0,   321,     0,     0,     0,     0,   266,
       0,     0,     0,   306,   309,     0,   241,   312,     0,     0,
      57,     0,    78,   831,     0,   809,   818,     0,   806,     0,
       0,   288,   285,   450,   451,   327,   337,   118,   261,   259,
     260,     0,     0,     0,     0,     0,     0,     0,   780,     0,
       0,     0,   816,   834,     0,   252,     0,   255,     0,     0,
     836,   845,     0,   427,   426,   463,   425,   424,     0,   845,
     321,   845,   328,   239,   240,     0,    74,   340,   220,   495,
       0,   226,   233,   234,   282,   287,   296,     0,   335,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   452,   453,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   406,     0,   219,     0,   588,     0,   606,   608,
     621,     0,     0,   514,   566,     0,   570,     0,   574,   349,
       0,     0,     0,   677,   690,     0,     0,   667,   669,     0,
       0,   123,     0,   123,     0,   123,   541,     0,   547,     0,
     668,     0,     0,   247,   694,   679,     0,   673,   763,     0,
     545,   753,     0,   551,   758,   536,     0,     0,   553,     0,
     552,     0,   555,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,   845,   284,   281,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   845,   267,     0,    80,
      81,     0,     0,     0,     0,    55,    56,     0,     0,   845,
       0,     0,   845,     0,     0,   289,   286,   328,   335,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   263,     0,
       0,     0,   812,   778,   780,     0,   821,     0,     0,     0,
     780,     0,     0,     0,     0,     0,   783,   839,     0,   245,
       0,    32,     0,    30,     0,   846,     0,   242,     0,   846,
       0,     0,     0,     0,   396,   393,   395,    60,     0,   118,
       0,   409,     0,   777,     0,     0,     0,   305,   304,     0,
     120,   258,     0,     0,   474,   473,     0,     0,   475,   479,
       0,     0,     0,   371,   380,   359,   381,   360,   383,   362,
     382,   361,   384,   363,   374,   353,   375,   354,   376,   355,
     428,   429,   441,   385,   364,   386,   365,   442,   439,   440,
       0,   373,   351,   468,     0,   459,     0,   492,   493,   494,
     352,   430,   431,   387,   366,   388,   367,   446,   447,   448,
     377,   356,   378,   357,   379,   358,   449,   372,   350,     0,
       0,   444,   445,   443,   437,   438,   433,   432,   434,   435,
     436,     0,     0,     0,   402,     0,     0,     0,     0,   412,
       0,     0,     0,     0,   521,   524,     0,   569,   572,   349,
     573,   688,   711,   714,     0,   717,   707,     0,   675,   721,
       0,   728,     0,   735,     0,     0,   741,     0,   744,     0,
     250,   691,   680,   540,   546,     0,   627,   628,   556,   559,
     558,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,     0,   322,   359,   360,   362,   361,   363,   353,
     354,   355,   364,   365,   351,   366,   367,   356,   357,   358,
     350,   290,     0,     0,     0,   464,     0,     0,     0,   465,
       0,   496,     0,     0,     0,     0,     0,     0,   335,   120,
     498,   499,   500,   501,   502,     0,     0,   781,     0,     0,
       0,     0,   321,   780,     0,     0,     0,     0,   789,   790,
       0,   795,     0,     0,   787,     0,   824,     0,     0,     0,
       0,   785,   825,     0,   815,   782,   835,     0,     0,     0,
       0,   837,   838,     0,     0,   823,   454,     0,     0,     0,
     120,     0,    58,    59,     0,    73,    65,   335,     0,     0,
     411,   410,   298,     0,     0,     0,     0,     0,     0,   333,
       0,   123,   470,     0,   476,     0,   370,   368,   369,     0,
       0,   457,     0,     0,   480,   484,     0,     0,   460,     0,
       0,     0,     0,   403,     0,   407,   455,   413,   589,   609,
     119,   522,   523,   524,   525,   516,   529,   571,   689,   712,
     715,   678,   718,   708,   670,   676,   722,   724,   729,   731,
     736,   738,   542,   742,   548,   745,     0,     0,   626,     0,
       0,   801,   804,     0,   269,   274,   275,     0,   318,   316,
     828,   826,     0,     0,   840,    82,   307,   310,   313,   832,
     830,   810,   819,   817,   807,     0,   123,     0,     0,     0,
     770,   769,     0,   780,     0,   813,     0,     0,   779,   794,
     788,   814,   822,   786,     0,   780,     0,   792,   793,   798,
     784,     0,   253,   256,    31,     0,   218,     0,     0,     0,
       0,   123,    61,     0,    66,     0,   120,     0,     0,     0,
       0,   563,   331,   332,   330,   329,     0,     0,     0,     0,
       0,     0,     0,   391,     0,     0,   481,     0,   469,     0,
     458,     0,   404,     0,     0,   456,   408,   534,   519,   516,
     517,   518,   521,   725,   732,   739,   251,   246,   629,   557,
       0,     0,    77,   270,     0,     0,   317,     0,     0,   845,
       0,     0,     0,     0,   811,     0,     0,     0,     0,   504,
       0,     0,     0,     0,   796,     0,     0,     0,     0,     0,
     791,     0,     0,   248,     0,     0,   394,   513,   397,     0,
      60,     0,    75,   123,   389,     0,   299,   563,     0,     0,
     334,   336,     0,   323,   339,     0,   512,     0,   510,   392,
     507,     0,     0,     0,   506,   405,     0,   520,   530,   519,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   308,
     311,   314,     0,     0,     0,   466,     0,   338,   505,   843,
       0,     0,   797,     0,   772,   771,   843,   799,   843,   254,
     246,   257,   843,   843,     0,   400,    62,   279,     0,    67,
      71,    72,    69,    70,    68,     0,     0,     0,     0,     0,
       0,     0,   324,   471,   477,     0,   511,   509,   508,     0,
     532,   526,   693,     0,     0,     0,   319,   845,   845,   841,
     845,     0,     0,   467,     0,     0,   774,   773,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,   285,   341,
     390,     0,     0,     0,     0,   325,   472,   478,   482,     0,
       0,   802,   805,     0,     0,     0,     0,   820,   808,     0,
       0,     0,     0,     0,     0,   249,   850,   847,   401,   286,
       0,     0,     0,   303,   483,   531,     0,   320,   829,   842,
     833,   844,   848,   776,   775,   849,   851,     0,   302,   301,
     533,   300
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1426, -1426, -1426, -1426, -1426, -1426,   472,  1129, -1426, -1426,
   -1426,  1197, -1426, -1426, -1426,   649,  1165, -1426,  1078, -1426,
   -1426,  1130, -1426, -1426, -1426,  -197, -1426, -1426, -1426,  -196,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,   993,
   -1426, -1426,  1137,   -68, -1426, -1426,   417,   437,  -515,  -554,
    -747, -1426, -1426, -1426, -1425, -1426, -1426,  -209,  -361, -1426,
     292, -1426, -1324, -1426,  -111,  -335, -1426, -1426, -1426, -1426,
    -415,   -14, -1426, -1426, -1426, -1426, -1426,  -191,  -190,  -189,
   -1426,  -188, -1426, -1426, -1426,  1224, -1426,   282, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426,  -431,  -162,  -433,  -107, -1426,  -868,  -292, -1426,   716,
   -1426, -1426,  -360,   251, -1426, -1426, -1426, -1411, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426,   796, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426,  -152,  -129,  -207,  -127,     0,
   -1426, -1426, -1426, -1426, -1426,   818, -1426,  -566, -1426, -1426,
    -573, -1426, -1426,  -610,  -202,  -557, -1289, -1426,  -359, -1426,
   -1426,  1191, -1426, -1426, -1426,   693,   829,    94, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,  -545,  -221,
   -1426,   814, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
    -368, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,  -204, -1426,
   -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426, -1426,   815,  -656,  -779,  -667,  -865, -1426,
   -1426, -1426, -1121,  -155, -1426, -1426, -1426, -1426, -1426, -1426,
   -1426, -1426, -1426,    92,  -474, -1426, -1426, -1426,   566, -1426,
   -1426, -1426, -1426, -1426, -1426,  -632, -1426,  -795,  -685, -1426
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    52,    17,   164,   170,   688,   500,
     149,   501,    99,    19,    20,    45,    46,    47,    88,    21,
      39,    40,   502,   503,  1184,  1185,   504,  1325,  1411,   505,
     506,   938,   507,   621,   508,   509,   510,   511,  1116,   171,
     172,    35,    36,    37,    66,    67,    68,    69,    22,   283,
     370,   210,    23,   111,   211,   112,   156,   666,   963,   513,
     371,   514,   812,  1471,  1069,   561,   918,  1402,   920,  1403,
     516,   517,   518,   623,   838,  1373,   519,   520,   521,   522,
     523,   524,   525,   526,   421,   527,   717,  1193,   947,   528,
     529,   871,  1380,   872,  1381,   874,  1382,   530,   841,  1376,
     531,   699,  1424,   532,  1199,  1200,   950,   668,   533,   774,
     939,   534,   633,   964,   536,   537,   538,   936,   539,  1179,
    1474,  1180,  1525,   540,  1037,  1356,   541,   700,  1339,  1536,
    1341,  1537,  1431,  1564,   543,   366,  1362,  1438,  1233,  1235,
    1046,   556,   783,  1499,  1540,   367,   368,   605,   807,   414,
     610,   809,   415,  1140,   615,   569,   385,   303,   304,   217,
     299,    78,   121,    25,   161,   372,    89,    90,   174,    91,
      26,    49,   115,   158,    27,   286,   553,   554,  1042,   375,
     215,   216,    76,   118,   377,    28,   159,   297,   616,   544,
     295,   349,   350,   799,   413,   351,   577,  1245,   792,   411,
     352,   570,  1238,   811,   575,  1243,   571,  1239,   572,  1240,
     574,  1242,   578,  1246,   579,  1363,   580,  1248,   581,  1364,
     582,  1250,   583,  1365,   584,  1253,   585,  1255,   606,    29,
      95,   177,   355,   607,    30,    96,   178,   356,   611,    31,
      94,   176,   354,   602,   545,   943,   944,   904,   905,   917,
    1162,  1156,  1151,  1309,   546,   834,  1370,   835,  1371,   883,
    1386,   880,  1384,   906,   690,   547,   881,  1385,   907,   548,
     842,  1377,   878,  1383,   684,   691,   549,  1515,   926,   550
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    70,   285,   683,   348,   866,   833,   632,   787,   219,
     512,   535,   808,   910,   930,   565,   931,   718,   806,   557,
    1129,   353,   903,   631,   903,   801,  1299,   803,   589,   805,
     896,  -118,   667,   667,   667,   151,   515,    84,    98,  1147,
       2,   782,  1419,  1441,   599,  1159,     3,   661,   828,    70,
      70,    70,  1083,   896,  1060,   828,  1062,    38,  1064,   419,
    1087,   122,   123,  1524,   721,   722,   701,    32,    33,     4,
    1136,     5,    85,     6,  1539,    50,   837,   512,  1157,     7,
     591,   107,   707,  1360,  1163,  1170,   819,   719,    53,     8,
      70,    70,    70,    70,    54,     9,    61,   104,   105,   106,
     412,   512,   420,   515,  1051,  1174,   108,   678,   680,   707,
     830,   401,   709,   412,  1558,  1566,   829,   830,    10,   831,
     741,    51,   832,   744,   745,    62,   364,   515,  1488,   165,
     166,   218,   895,   897,   908,    77,   912,   720,   402,   403,
      11,    12,  1361,    92,   924,  1292,  1521,   378,   348,   284,
     300,  1422,   888,   928,   788,   422,   710,   813,  1092,   733,
     734,  1153,   348,    58,  1154,   424,   828,   741,   150,   743,
     744,   745,   746,  1394,   669,   670,  1139,   747,   162,   163,
    1146,  1114,    98,   651,    80,   139,   365,   590,   423,   284,
      59,   348,  1155,   348,  1123,    48,    63,  1126,   843,   845,
     404,   707,   780,    58,   405,   772,   773,   124,   512,   379,
     138,   347,   125,    86,   126,   844,    34,   127,   829,    13,
     401,   721,   722,   879,    87,  1195,   882,   566,   830,   707,
      59,   107,   635,   781,   515,  1196,   829,   567,    14,   592,
     769,   770,   771,    71,   829,   885,   652,   402,   403,    15,
     829,   829,   772,   773,   348,   348,  1230,   593,   128,   301,
    1285,   284,   587,   594,    79,    64,   406,   945,   865,   512,
     407,   829,  1462,   408,    65,   167,    80,   302,  1294,   942,
     168,   588,   169,   828,  1188,   127,   667,  1197,   409,   568,
     828,   884,  1198,  1139,   410,   515,    80,   498,   665,   512,
    1396,  1031,  1032,   180,    41,    42,    43,    72,    73,   404,
      74,  1148,  1149,   405,   628,  1145,   733,   734,  1329,  1326,
     212,   946,   744,   745,   741,   515,   743,   744,   745,   746,
    1033,   213,   400,   629,   747,    44,   364,   828,    75,  1150,
    1327,   218,  1034,  1330,  1040,   830,   223,  1460,  1057,   348,
     348,   348,   830,   348,   348,   347,   707,   348,   948,   348,
     555,   348,  1058,   348,   498,   665,   790,   791,   793,   347,
     795,   796,  1048,   224,   800,   406,   802,   828,   804,   407,
      81,  1128,   408,  1035,  1036,  1189,   401,  1551,   515,   515,
     515,   515,   515,   515,   515,   515,  1201,   409,   347,   830,
     347,   515,   515,   410,   772,   773,    82,   515,  1086,   772,
     773,   836,  1190,   402,   403,   348,   515,   515,  1079,    97,
     347,   515,   515,   515,  1187,   515,   512,   951,  1393,   846,
     846,  1329,   869,    58,    98,  1358,   776,    58,   348,   830,
    1399,  1166,   932,   777,   846,   412,   932,   933,  1336,  1052,
     636,  1171,   515,   707,  1338,  1234,  1227,  1265,  1331,   846,
      59,   347,   347,   667,    59,   846,  1081,  1082,  1260,   637,
     701,  1429,   512,  1337,  1091,   404,  1293,   919,   701,   405,
     100,   101,   102,  1145,  -723,   934,  1435,   903,  -730,  -723,
    1298,  1305,  1458,  -730,  1254,  1065,   937,   653,   515,  1252,
     955,   959,   903,  1117,  1118,   131,  1120,  -723,  1486,  1066,
    1122,  -730,  1124,  1125,  -737,  1127,   654,   301,   103,  -737,
    -399,   152,   153,   154,   155,  -399,   999,   667,   667,   667,
     656,    87,   707,  1112,   707,   302,   707,  -737,   707,  1388,
     707,   406,   707,  -399,   707,   407,   707,  1113,   408,   657,
     412,   707,  1418,   707,  1055,   384,   347,   347,   347,   707,
     347,   347,  1067,   409,   347,   613,   347,  1256,   347,   410,
     347,   707,   401,   707,  1409,  1286,  1068,  1256,   707,  1256,
     707,  1257,   707,   614,   284,   707,  1160,  1153,  1374,  1161,
     348,  1312,   846,  1313,  1181,   721,   722,   551,   348,   402,
     403,   348,  1375,   133,  1137,  1182,  1183,  1070,   136,   552,
     348,   600,   840,   348,   348,  1073,   707,   348,  1074,  1054,
     348,   348,   347,   601,   608,   137,  1321,  1078,   994,   412,
    1217,  1045,  1222,    53,  1084,  1487,   609,  1090,   384,    54,
     995,  1379,  1218,   109,  1223,   347,  1296,   707,   113,   110,
     348,   348,  1263,   348,   114,  1111,   144,   348,  1297,   348,
     348,   404,   348,  1503,  1504,   405,  1485,   897,  1206,  1207,
    1208,  1519,  1415,  1520,  1511,  1512,   116,  1522,  1523,  1292,
     382,   412,   117,   383,   140,  1056,   384,  1397,   935,  1306,
     733,   734,  1307,   145,  1448,  1308,   146,   348,   741,   348,
     119,   744,   745,   746,   141,  1288,   120,  1231,   747,  1391,
     142,  1398,  1401,  1232,  1070,   846,  1070,   846,   846,   104,
     412,   106,  1404,  1405,  1059,   148,  1304,   406,   846,   846,
     147,   407,  1311,  1261,   408,   134,   135,   401,   412,  1315,
     426,   427,  1061,  1317,  1272,   220,   221,   412,   412,   409,
     287,  1063,  1264,   412,   288,   410,   157,  1269,   412,   412,
     437,   160,  1494,  1538,   402,   403,   442,    70,   289,   290,
      85,   179,  1413,   291,   292,   293,   294,   175,  1344,   104,
     105,   106,  1461,   772,   773,   162,   163,   921,   922,   214,
    1353,    41,    42,    43,   220,   221,   222,   347,  1215,   914,
     915,   916,   104,   456,   457,   347,   226,  1167,   347,  1168,
     218,  1320,   512,   535,   225,  1508,   227,   347,   228,   280,
     347,   347,  1544,  1545,   347,  1546,   404,   347,   347,   281,
     405,   284,   296,   348,   298,   357,   459,   460,   515,    55,
      56,    57,   282,   358,   348,   359,   360,   361,   362,   373,
    1241,   369,   363,   381,   721,   722,  1390,   347,   347,   374,
     347,   376,  1490,   380,   347,   386,   347,   347,   389,   347,
     392,   387,   388,   399,   390,   391,    58,   416,   393,   394,
     395,   396,   397,   398,   412,   417,   418,   563,   564,   576,
     618,   348,   406,   475,   476,   477,   407,   642,  1262,   408,
     619,   627,   558,    59,   347,   597,   347,   643,  1552,   644,
     626,   640,   645,   488,   409,   647,   646,   648,   655,   658,
     410,   659,   671,   672,   673,   687,   692,  1468,   622,   674,
    1472,  1532,  1533,   702,   703,   675,   813,   704,   706,   676,
     711,   712,   713,   714,   715,   496,   716,   731,   732,   733,
     734,    15,   775,   778,   651,   785,   797,   741,   348,   743,
     744,   745,   746,   685,   798,   608,   814,   747,   817,   749,
     750,  1425,  1322,   820,  1560,   966,   968,   970,   972,   974,
     976,   978,   818,   821,   826,   823,   824,   983,   985,   822,
     827,   839,   846,   991,   721,   722,   719,   864,   677,   870,
     875,   876,  1003,  1005,   306,   887,   925,  1010,  1012,  1014,
     307,  1017,   927,   929,   949,  1507,   308,   990,  1044,  1047,
    1053,  1510,  1071,  1076,  1145,  1077,   309,  1138,   767,   768,
     769,   770,   771,  1152,   310,   348,  1176,  1165,  1172,   813,
     347,  1175,   772,   773,  1177,  1192,   632,  1178,  1203,   311,
    1489,   347,  1366,  1202,  1372,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,  1191,   733,
     734,  1204,  1225,  1205,  1209,   845,  1210,   741,   347,   743,
     744,   745,   746,  1220,  1221,  1229,  1234,   747,  1244,  1236,
    1247,  1249,  1251,  1259,  1410,  1266,  1267,  1271,   348,  1531,
     348,  1273,  1332,  1274,  1280,  1287,    58,  1283,  1333,  1423,
    1300,  1303,  1310,  1334,  1335,  1346,  1347,  1427,  1357,   345,
    1368,  1412,  1349,  1354,  1367,  1387,  1395,  1400,  1414,  1416,
    1417,  1437,  1421,    59,  1444,  1442,  1443,  1446,  1447,  1549,
     923,  1452,  1453,  1454,  1470,   347,  1502,   542,   767,   768,
     769,   770,   771,  1475,  1505,  1509,   562,  1514,  1553,  1423,
    1530,  1541,   772,   773,  1542,   573,  1554,  1547,  1548,  1550,
    1555,  1556,  1324,  1557,  1565,   586,  1576,   348,    18,   346,
    1568,   721,   722,  1569,  1570,   596,  1572,  1575,   129,    83,
     348,   173,   132,  1476,   612,  1479,   305,   130,  1043,  1173,
    1480,  1481,  1482,  1483,   620,    24,  1457,  1495,  1194,  1420,
    1439,   708,  1500,  1359,   634,  1440,   721,   722,  1501,   639,
     617,   641,   347,    93,  1467,  1302,   911,   779,   624,   625,
     650,     0,     0,     0,     0,     0,     0,     0,   662,   663,
     664,     0,     0,     0,     0,     0,   348,     0,     0,     0,
       0,     0,     0,   682,     0,     0,  1529,     0,     0,     0,
       0,   686,     0,     0,   682,     0,     0,   693,   694,   695,
     696,   697,     0,     0,   731,   732,   733,   734,   735,     0,
     705,   738,   739,   740,   741,     0,   743,   744,   745,   746,
       0,     0,     0,     0,   747,     0,   749,   750,     0,     0,
       0,     0,   753,   754,   755,   347,   401,   347,   759,   731,
     732,   733,   734,   735,     0,     0,   738,   739,   740,   741,
       0,   743,   744,   745,   746,     0,     0,     0,     0,   747,
       0,   749,   750,   402,   403,     0,     0,  1580,     0,     0,
       0,     0,     0,     0,     0,   789,     0,     0,   761,     0,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   772,
     773,     0,     0,   816,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,   404,     0,   347,   566,   405,
       0,     0,     0,     0,   772,   773,   634,     0,   567,     0,
       0,     0,     0,   682,   867,     0,   868,     0,     0,     0,
       0,   873,     0,     0,     0,     0,     0,   877,     0,     0,
       0,     0,     0,     0,     0,     0,   886,     0,     0,     0,
     935,     0,     0,     0,     0,     0,     0,   889,   890,   891,
     892,   893,   894,   347,   902,     0,   902,     0,     0,     0,
     568,   406,     0,     0,     0,   407,     0,     0,   408,     0,
       0,     0,     0,   721,   722,     0,     0,     0,     0,     0,
       0,     0,     0,   409,     0,     0,     0,     0,     0,   410,
       0,   935,     0,     0,     0,     0,     0,     0,     0,   965,
     967,   969,   971,   973,   975,   977,   979,   980,   981,     0,
       0,   982,   984,   986,   987,   988,   989,     0,   992,   993,
       0,   996,   997,   998,  1000,  1001,  1002,  1004,  1006,  1007,
    1008,  1009,  1011,  1013,  1015,  1016,  1018,  1020,  1021,  1022,
    1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,     0,  1038,
       0,     0,     0,  1041,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1049,     0,     0,   731,   732,   733,   734,
     735,     0,     0,   738,   739,   740,   741,     0,   743,   744,
     745,   746,     0,     0,     0,     0,   747,     0,   749,   750,
       0,     0,     0,     0,   753,   754,   755,     0,     0,     0,
     759,     0,  1075,     0,     0,     0,     0,     0,     0,  1080,
       0,     0,     0,     0,     0,     0,  1089,     0,     0,     0,
       0,     0,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,
    1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,
     761,     0,   762,   763,   764,   765,   766,   767,   768,   769,
     770,   771,     0,     0,     0,     0,     0,     0,     0,   401,
       0,   772,   773,     0,     0,     0,   498,   665,     0,     0,
       0,     0,     0,     0,     0,   693,  1142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   402,   403,     0,  1164,
       0,   682,     0,     0,     0,     0,     0,     0,  1169,   426,
     427,   682,     0,     0,     0,  1093,     0,     0,     0,   432,
     433,   434,   435,   436,  1186,     0,     0,     0,     0,   437,
       0,   439,     0,     0,     0,   442,     0,   401,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,   447,
       0,     0,   448,     0,     0,   449,     0,   896,   404,   452,
       0,   566,   405,     0,   402,   403,     0,     0,     0,   559,
       0,   567,   456,   457,   312,   313,   314,     0,   316,   317,
     318,   319,   320,   458,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,     0,   334,   335,   336,     0,
       0,   339,   340,   341,   342,   459,   460,   461,     0,     0,
       0,     0,     0,     0,     0,     0,   786,     0,  1219,     0,
     463,   464,  1224,   568,   406,     0,   404,     0,   407,     0,
     405,   408,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,   409,     0,     0,     0,
       0,     0,   410,   468,   469,   470,   471,   472,     0,   473,
     897,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   898,   560,   486,   487,     0,     0,     0,     0,
       0,     0,   488,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   406,     0,     0,     0,   407,     0,     0,   899,
     492,   493,  1275,    14,     0,     0,   494,   495,     0,     0,
       0,     0,     0,     0,   900,     0,   901,     0,   498,   499,
     410,     0,     0,     0,  1289,  1290,  1291,     0,     0,     0,
    1295,     0,     0,     0,     0,   721,   722,     0,  1301,   902,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
       0,     0,     0,  1318,  1319,   -68,     0,     0,     0,     0,
    1323,     0,     0,     0,     0,  1093,   721,   722,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1340,
       0,  1342,     0,     0,     0,     0,  1345,     0,     0,     0,
       0,     0,  1348,     0,     0,     0,  1351,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   731,   732,
     733,   734,   735,     0,     0,   738,   739,   740,   741,     0,
     743,   744,   745,   746,     0,     0,  1369,     0,   747,     0,
     749,   750,     0,     0,     0,     0,   753,     0,   755,   682,
       0,   723,   724,   725,   726,   727,   728,   729,   730,   731,
     732,   733,   734,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,     0,     0,     0,     0,   747,
     748,   749,   750,   751,   752,     0,     0,   753,   754,   755,
     756,   757,   758,   759,   762,   763,   764,   765,   766,   767,
     768,   769,   770,   771,     0,     0,     0,     0,     0,     0,
       0,   721,   722,   772,   773,     0,     0,     0,     0,     0,
       0,     0,     0,  1432,     0,  1433,     0,     0,     0,     0,
    1436,     0,   760,   761,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,     0,     0,     0,     0,     0,
       0,  1445,     0,     0,   772,   773,  1449,  1450,  1451,   498,
     665,     0,     0,  1456,     0,     0,     0,  1459,     0,     0,
       0,  1463,  1464,  1465,  1466,     0,     0,   682,  1469,     0,
     682,  1473,     0,     0,     0,     0,     0,  1484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1492,     0,
       0,     0,     0,     0,   731,   732,   733,   734,   735,     0,
       0,   738,   739,   740,   741,     0,   743,   744,   745,   746,
       0,     0,     0,   682,   747,     0,   749,   750,     0,     0,
       0,     0,     0,     0,     0,     0,  1516,  1517,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1528,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1534,  1535,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1543,   764,   765,   766,   767,   768,   769,   770,   771,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   772,
     773,     0,     0,  1559,     0,     0,     0,     0,  1561,  1562,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1571,   425,  1573,  1574,   426,
     427,     3,     0,   428,   429,   430,  1577,   431,     0,   432,
     433,   434,   435,   436,     0,     0,     0,     0,     0,   437,
     438,   439,   440,   441,     0,   442,     0,     0,     0,     0,
       0,     0,   443,   444,     0,     0,   445,     0,   446,   447,
       0,     0,   448,     0,     8,   449,   450,     0,   451,   452,
       0,     0,   453,   454,     0,     0,     0,     0,     0,   455,
       0,     0,   456,   457,   312,   313,   314,     0,   316,   317,
     318,   319,   320,   458,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,     0,   334,   335,   336,     0,
       0,   339,   340,   341,   342,   459,   460,   461,   462,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     463,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   465,   466,   467,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,   471,   472,     0,   473,
       0,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,    59,   485,   486,   487,     0,     0,     0,     0,
       0,     0,   488,   489,   490,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,     0,    14,     0,     0,   494,   495,     0,     0,
       0,     0,     0,     0,   496,     0,   497,   425,   498,   499,
     426,   427,     3,     0,   428,   429,   430,     0,   431,     0,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,   438,   439,   440,   441,     0,   442,     0,     0,     0,
       0,     0,     0,   443,   444,     0,     0,   445,     0,   446,
     447,     0,     0,   448,     0,     8,   449,   450,     0,   451,
     452,     0,     0,   453,   454,     0,     0,     0,     0,     0,
     455,     0,     0,   456,   457,   312,   313,   314,     0,   316,
     317,   318,   319,   320,   458,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,     0,   334,   335,   336,
       0,     0,   339,   340,   341,   342,   459,   460,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   463,   464,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   465,   466,   467,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,   471,   472,   401,
     473,     0,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,    59,   485,   486,   487,     0,     0,     0,
       0,     0,     0,   488,  1316,   490,   402,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     491,   492,   493,     0,    14,     0,     0,   494,   495,     0,
       0,   426,   427,     0,     0,   496,     0,   497,     0,   498,
     499,   432,   433,   434,   435,   436,     0,     0,     0,     0,
       0,   437,     0,   439,     0,     0,     0,   442,     0,   401,
       0,     0,     0,     0,     0,   444,     0,     0,   404,     0,
       0,   447,   405,     0,   448,     0,     0,   449,     0,     0,
       0,   452,     0,     0,     0,     0,   402,   403,     0,     0,
       0,   559,     0,     0,   456,   457,   312,   313,   314,     0,
     316,   317,   318,   319,   320,   458,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,     0,   334,   335,
     336,     0,     0,   339,   340,   341,   342,   459,   460,   461,
       0,     0,     0,     0,   406,     0,     0,     0,   407,     0,
    1270,   408,   463,   464,     0,     0,     0,     0,   404,     0,
       0,     0,   405,     0,     0,     0,   409,     0,     0,     0,
       0,     0,   410,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   468,   469,   470,   471,   472,
       0,   473,   897,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   898,   560,   486,   487,     0,   401,
       0,     0,     0,     0,   488,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   406,     0,     0,     0,   407,     0,
       0,   899,   492,   493,     0,    14,   402,   403,   494,   495,
       0,     0,     0,   426,   427,     0,   900,     0,   909,     0,
     498,   499,   410,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,   437,     0,   439,     0,     0,     0,   442,
       0,   591,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,   447,     0,     0,   448,     0,     0,   449,
       0,     0,     0,   452,     0,     0,     0,     0,   404,     0,
       0,     0,   405,   559,     0,     0,   456,   457,   312,   313,
     314,     0,   316,   317,   318,   319,   320,   458,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,     0,
     334,   335,   336,     0,     0,   339,   340,   341,   342,   459,
     460,   461,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   463,   464,     0,     0,     0,     0,
       0,     0,     0,     0,   406,     0,     0,     0,   407,     0,
    1276,   408,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,   409,   468,   469,   470,
     471,   472,   410,   473,     0,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,    59,   560,   486,   487,
       0,     0,     0,     0,     0,     0,   488,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     592,     0,     0,   491,   492,   493,     0,    14,     0,     0,
     494,   495,     0,     0,     0,   426,   427,     0,  1141,     0,
     497,     0,   498,   499,   594,   432,   433,   434,   435,   436,
       0,     0,     0,     0,     0,   437,     0,   439,     0,     0,
       0,   442,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,   447,     0,     0,   448,     0,
       0,   449,     0,     0,     0,   452,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   559,     0,     0,   456,   457,
     312,   313,   314,     0,   316,   317,   318,   319,   320,   458,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,     0,   334,   335,   336,     0,     0,   339,   340,   341,
     342,   459,   460,   461,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   463,   464,     0,     0,
       0,     0,     0,     0,     0,   630,     0,     0,     0,     0,
       0,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   470,   471,   472,   401,   473,     0,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,    59,   560,
     486,   487,     0,     0,     0,     0,     0,     0,   488,     0,
       0,   402,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,     0,    14,
       0,     0,   494,   495,     0,     0,   426,   427,     0,     0,
     496,     0,   497,     0,   498,   499,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,   404,     0,     0,   447,   405,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   312,   313,   314,     0,   316,   317,   318,   319,   320,
     458,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,     0,   334,   335,   336,     0,     0,   339,   340,
     341,   342,   459,   460,   461,     0,     0,     0,     0,   406,
       0,     0,     0,   407,     0,  1277,   408,   463,   464,     0,
       0,     0,     0,     0,     0,     0,   660,     0,     0,     0,
       0,   409,   465,   466,   467,     0,     0,   410,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,   401,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,   488,
       0,     0,   402,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   491,   492,   493,     0,
      14,     0,     0,   494,   495,     0,     0,   426,   427,     0,
       0,   496,     0,   497,     0,   498,   499,   432,   433,   434,
     435,   436,     0,     0,     0,     0,     0,   437,  1477,   439,
     440,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,   444,     0,     0,   404,     0,     0,   447,   405,     0,
     448,     0,     0,   449,   450,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,     0,
     456,   457,   312,   313,   314,     0,   316,   317,   318,   319,
     320,   458,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,     0,   334,   335,   336,     0,     0,   339,
     340,   341,   342,   459,   460,   461,  1478,     0,     0,     0,
     406,     0,     0,     0,   407,     0,  1278,   408,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   409,     0,     0,     0,     0,     0,   410,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   470,   471,   472,   401,   473,     0,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
      59,   560,   486,   487,     0,     0,     0,     0,     0,     0,
     488,     0,     0,   402,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
       0,    14,     0,     0,   494,   495,     0,     0,   426,   427,
       0,     0,   496,     0,   497,     0,   498,   499,   432,   433,
     434,   435,   436,     0,     0,     0,     0,     0,   437,     0,
     439,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,     0,   444,     0,     0,   404,     0,     0,   447,   405,
       0,   448,     0,     0,   449,     0,     0,     0,   452,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
       0,   456,   457,   312,   313,   314,     0,   316,   317,   318,
     319,   320,   458,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,     0,   334,   335,   336,     0,     0,
     339,   340,   341,   342,   459,   460,   461,     0,     0,     0,
       0,   406,     0,     0,     0,   407,     0,  1279,   408,   463,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   409,   465,   466,   467,     0,     0,   410,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   470,   471,   472,   401,   473,     0,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,    59,   560,   486,   487,     0,     0,     0,     0,     0,
       0,   488,     0,     0,   402,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   491,   492,
     493,     0,    14,     0,     0,   494,   495,     0,     0,   426,
     427,     0,     0,   496,     0,   497,     0,   498,   499,   432,
     433,   434,   435,   436,     0,     0,     0,     0,     0,   437,
       0,   439,     0,     0,     0,   442,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,   404,     0,     0,   447,
     405,     0,   448,     0,     0,   449,     0,     0,     0,   452,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   559,
       0,     0,   456,   457,   312,   313,   314,     0,   316,   317,
     318,   319,   320,   458,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,     0,   334,   335,   336,     0,
       0,   339,   340,   341,   342,   459,   460,   461,     0,     0,
       0,     0,   406,     0,     0,     0,   407,     0,  1281,   408,
     463,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   409,   960,   961,   962,     0,     0,
     410,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,   471,   472,   401,   473,
       0,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,    59,   560,   486,   487,     0,     0,     0,     0,
       0,     0,   488,     0,     0,   402,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,     0,    14,     0,     0,   494,   495,     0,     0,
     426,   427,     0,     0,   496,     0,   497,     0,   498,   499,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,     0,   439,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,   404,     0,     0,
     447,   405,     0,   448,     0,     0,   449,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   456,   457,   312,   313,   314,     0,   316,
     317,   318,   319,   320,   458,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,     0,   334,   335,   336,
       0,     0,   339,   340,   341,   342,   459,   460,   461,     0,
       0,     0,     0,   406,     0,     0,     0,   407,     0,  1282,
     408,   463,   464,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   409,     0,     0,     0,     0,
       0,   410,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   468,   469,   470,   471,   472,   401,
     473,   897,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   898,   560,   486,   487,     0,     0,     0,
       0,     0,     0,   488,     0,     0,   402,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     491,   492,   493,     0,    14,     0,     0,   494,   495,     0,
       0,   426,   427,     0,     0,   940,     0,   497,   941,   498,
     499,   432,   433,   434,   435,   436,     0,     0,     0,     0,
       0,   437,     0,   439,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,   444,     0,     0,   404,     0,
       0,   447,   405,     0,   448,     0,     0,   449,     0,     0,
       0,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   559,     0,     0,   456,   457,   312,   313,   314,     0,
     316,   317,   318,   319,   320,   458,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,     0,   334,   335,
     336,     0,     0,   339,   340,   341,   342,   459,   460,   461,
       0,     0,     0,     0,   406,     0,     0,     0,   407,     0,
    1284,   408,   463,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   409,     0,     0,     0,
       0,     0,   410,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   468,   469,   470,   471,   472,
     401,   473,   897,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   898,   560,   486,   487,     0,     0,
       0,     0,     0,     0,   488,     0,     0,   402,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,     0,    14,     0,     0,   494,   495,
       0,     0,     0,     0,   426,   427,   496,     0,   497,  1085,
     498,   499,   698,     0,   432,   433,   434,   435,   436,     0,
       0,     0,     0,     0,   437,     0,   439,     0,     0,   401,
     442,     0,     0,     0,     0,     0,     0,     0,   444,   404,
       0,     0,     0,   405,   447,     0,     0,   448,     0,     0,
     449,     0,     0,     0,   452,     0,   402,   403,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   456,   457,   312,
     313,   314,     0,   316,   317,   318,   319,   320,   458,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
       0,   334,   335,   336,     0,     0,   339,   340,   341,   342,
     459,   460,   461,     0,     0,   406,     0,     0,     0,   407,
       0,  1378,   408,     0,     0,   463,   464,     0,   404,     0,
       0,     0,   405,     0,     0,     0,     0,   409,     0,     0,
       0,     0,     0,   410,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   468,   469,
     470,   471,   472,   401,   473,     0,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,    59,   560,   486,
     487,     0,     0,     0,     0,     0,     0,   488,     0,     0,
     402,   403,     0,     0,   406,     0,     0,     0,   407,     0,
    1408,   408,     0,     0,   491,   492,   493,     0,    14,     0,
       0,   494,   495,     0,     0,     0,   409,   426,   427,   496,
     595,   497,   410,   498,   499,   698,     0,   432,   433,   434,
     435,   436,     0,     0,     0,     0,     0,   437,     0,   439,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,   444,   404,     0,     0,     0,   405,   447,     0,     0,
     448,     0,     0,   449,     0,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,     0,
     456,   457,   312,   313,   314,     0,   316,   317,   318,   319,
     320,   458,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,     0,   334,   335,   336,     0,     0,   339,
     340,   341,   342,   459,   460,   461,     0,     0,   406,     0,
       0,     0,   407,     0,  1493,   408,     0,     0,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     409,     0,     0,     0,     0,     0,   410,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   470,   471,   472,     0,   473,   897,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     898,   560,   486,   487,     0,     0,     0,     0,     0,     0,
     488,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
       0,    14,     0,     0,   494,   495,     0,     0,     0,     0,
     426,   427,   496,     0,   497,     0,   498,   499,   698,     0,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,     0,   439,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
     447,     0,     0,   448,     0,     0,   449,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   456,   457,   312,   313,   314,     0,   316,
     317,   318,   319,   320,   458,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,     0,   334,   335,   336,
       0,     0,   339,   340,   341,   342,   459,   460,   461,     0,
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
       0,   426,   427,     0,     0,   496,   814,   497,     0,   498,
     499,   432,   433,   434,   435,   436,     0,     0,     0,     0,
       0,   437,     0,   439,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,   444,     0,     0,     0,     0,
       0,   447,     0,     0,   448,     0,     0,   449,     0,     0,
       0,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   559,     0,     0,   456,   457,   312,   313,   314,     0,
     316,   317,   318,   319,   320,   458,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,     0,   334,   335,
     336,     0,     0,   339,   340,   341,   342,   459,   460,   461,
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
       0,     0,   426,   427,     0,     0,   496,   595,   497,     0,
     498,   499,   432,   433,   434,   435,   436,     0,     0,     0,
       0,     0,   437,     0,   439,     0,     0,     0,   442,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,   447,     0,     0,   448,     0,     0,   449,     0,
       0,     0,   452,     0,     0,     0,     0,     0,   638,     0,
       0,     0,   559,     0,     0,   456,   457,   312,   313,   314,
       0,   316,   317,   318,   319,   320,   458,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,     0,   334,
     335,   336,     0,     0,   339,   340,   341,   342,   459,   460,
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
       0,   498,   499,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,   437,     0,   439,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,   447,     0,     0,   448,     0,     0,   449,
       0,     0,     0,   452,     0,     0,   649,     0,     0,     0,
       0,     0,     0,   559,     0,     0,   456,   457,   312,   313,
     314,     0,   316,   317,   318,   319,   320,   458,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,     0,
     334,   335,   336,     0,     0,   339,   340,   341,   342,   459,
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
     497,     0,   498,   499,   681,     0,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   312,   313,   314,     0,   316,   317,   318,   319,   320,
     458,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,     0,   334,   335,   336,     0,     0,   339,   340,
     341,   342,   459,   460,   461,     0,     0,     0,     0,     0,
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
     435,   436,     0,     0,     0,     0,     0,   437,     0,   439,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,   444,     0,     0,     0,     0,     0,   447,     0,     0,
     448,     0,     0,   449,     0,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,     0,
     456,   457,   312,   313,   314,     0,   316,   317,   318,   319,
     320,   458,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,     0,   334,   335,   336,     0,     0,   339,
     340,   341,   342,   459,   460,   461,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   470,   471,   472,     0,   473,     0,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
      59,   560,   486,   487,     0,     0,     0,     0,     0,     0,
     488,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   685,     0,   491,   492,   493,
       0,    14,     0,     0,   494,   495,     0,     0,     0,     0,
     426,   427,   496,     0,   497,     0,   498,   499,   689,     0,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,     0,   439,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
     447,     0,     0,   448,     0,     0,   449,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   456,   457,   312,   313,   314,     0,   316,
     317,   318,   319,   320,   458,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,     0,   334,   335,   336,
       0,     0,   339,   340,   341,   342,   459,   460,   461,     0,
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
       0,     0,     0,   426,   427,   496,     0,   497,     0,   498,
     499,   698,     0,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,   437,     0,   439,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,   447,     0,     0,   448,     0,     0,   449,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   559,     0,     0,   456,   457,   312,   313,
     314,     0,   316,   317,   318,   319,   320,   458,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,     0,
     334,   335,   336,     0,     0,   339,   340,   341,   342,   459,
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
     497,     0,   498,   499,   432,   433,   434,   435,   436,     0,
       0,  1019,     0,     0,   437,     0,   439,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,   447,     0,     0,   448,     0,     0,
     449,     0,     0,     0,   452,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   456,   457,   312,
     313,   314,     0,   316,   317,   318,   319,   320,   458,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
       0,   334,   335,   336,     0,     0,   339,   340,   341,   342,
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
       0,   497,     0,   498,   499,   432,   433,   434,   435,   436,
       0,     0,     0,     0,     0,   437,     0,   439,     0,     0,
       0,   442,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,   447,     0,     0,   448,     0,
       0,   449,     0,     0,     0,   452,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   559,     0,     0,   456,   457,
     312,   313,   314,     0,   316,   317,   318,   319,   320,   458,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,     0,   334,   335,   336,     0,     0,   339,   340,   341,
     342,   459,   460,   461,     0,     0,     0,     0,     0,     0,
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
     496,     0,   497,  1039,   498,   499,   432,   433,   434,   435,
     436,     0,     0,     0,     0,     0,   437,     0,   439,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,   447,     0,     0,   448,
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   312,   313,   314,     0,   316,   317,   318,   319,   320,
     458,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,     0,   334,   335,   336,     0,     0,   339,   340,
     341,   342,   459,   460,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   472,     0,   473,     0,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,    59,
     560,   486,   487,     0,     0,     0,     0,     0,     0,   488,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1088,     0,   491,   492,   493,     0,
      14,     0,     0,   494,   495,     0,     0,   426,   427,     0,
       0,   496,     0,   497,     0,   498,   499,   432,   433,   434,
     435,   436,     0,     0,     0,     0,     0,   437,     0,   439,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,   444,     0,     0,     0,     0,     0,   447,     0,     0,
     448,     0,     0,   449,     0,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,     0,
     456,   457,   312,   313,   314,     0,   316,   317,   318,   319,
     320,   458,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,     0,   334,   335,   336,     0,     0,   339,
     340,   341,   342,   459,   460,   461,     0,     0,     0,     0,
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
       0,     0,   496,     0,   497,  1143,   498,   499,   432,   433,
     434,   435,   436,     0,     0,     0,     0,     0,   437,     0,
     439,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,     0,   444,     0,     0,     0,     0,     0,   447,     0,
       0,   448,     0,     0,   449,     0,     0,     0,   452,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
       0,   456,   457,   312,   313,   314,     0,   316,   317,   318,
     319,   320,   458,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,     0,   334,   335,   336,     0,     0,
     339,   340,   341,   342,   459,   460,   461,     0,     0,     0,
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
     427,     0,     0,   496,     0,   497,  1158,   498,   499,   432,
     433,   434,   435,   436,     0,     0,     0,     0,     0,   437,
       0,   439,     0,     0,     0,   442,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,   447,
       0,     0,   448,     0,     0,   449,     0,     0,     0,   452,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   559,
       0,     0,   456,   457,   312,   313,   314,     0,   316,   317,
     318,   319,   320,   458,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,     0,   334,   335,   336,     0,
       0,   339,   340,   341,   342,   459,   460,   461,     0,     0,
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
     426,   427,     0,     0,   496,     0,   497,  1343,   498,   499,
     432,   433,   434,   435,   436,     0,     0,     0,     0,     0,
     437,     0,   439,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
     447,     0,     0,   448,     0,     0,   449,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   456,   457,   312,   313,   314,     0,   316,
     317,   318,   319,   320,   458,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,     0,   334,   335,   336,
       0,     0,   339,   340,   341,   342,   459,   460,   461,     0,
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
       0,   426,   427,     0,     0,   496,     0,   497,  1352,   498,
     499,   432,   433,   434,   435,   436,     0,     0,     0,     0,
       0,   437,     0,   439,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,   444,     0,     0,     0,     0,
       0,   447,     0,     0,   448,     0,     0,   449,     0,     0,
       0,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   559,     0,     0,   456,   457,   312,   313,   314,     0,
     316,   317,   318,   319,   320,   458,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,     0,   334,   335,
     336,     0,     0,   339,   340,   341,   342,   459,   460,   461,
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
       0,     0,   426,   427,     0,     0,   496,     0,   497,  1389,
     498,   499,   432,   433,   434,   435,   436,     0,     0,     0,
       0,     0,   437,     0,   439,     0,     0,     0,   442,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,   447,     0,     0,   448,     0,     0,   449,     0,
       0,     0,   452,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,     0,   456,   457,   312,   313,   314,
       0,   316,   317,   318,   319,   320,   458,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,     0,   334,
     335,   336,     0,     0,   339,   340,   341,   342,   459,   460,
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
    1455,   498,   499,   432,   433,   434,   435,   436,     0,     0,
       0,     0,     0,   437,     0,   439,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,   447,     0,     0,   448,     0,     0,   449,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   559,     0,     0,   456,   457,   312,   313,
     314,     0,   316,   317,   318,   319,   320,   458,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,     0,
     334,   335,   336,     0,     0,   339,   340,   341,   342,   459,
     460,   461,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   463,   464,     0,     0,     0,     0,
       0,     0,     0,  1491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   468,   469,   470,
     471,   472,     0,   473,     0,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,    59,   560,   486,   487,
       0,     0,     0,     0,     0,     0,   488,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,     0,    14,     0,     0,
     494,   495,     0,     0,   426,   427,     0,     0,   496,     0,
     497,     0,   498,   499,   432,   433,   434,   435,   436,     0,
       0,     0,     0,     0,   437,     0,   439,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,   447,     0,     0,   448,     0,     0,
     449,     0,     0,     0,   452,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   456,   457,   312,
     313,   314,     0,   316,   317,   318,   319,   320,   458,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
       0,   334,   335,   336,     0,     0,   339,   340,   341,   342,
     459,   460,   461,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   463,   464,     0,     0,     0,
       0,     0,     0,     0,  1526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   468,   469,
     470,   471,   472,     0,   473,     0,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,    59,   560,   486,
     487,     0,     0,     0,     0,     0,     0,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   491,   492,   493,     0,    14,     0,
       0,   494,   495,     0,     0,   426,   427,     0,     0,   496,
       0,   497,     0,   498,   499,   432,   433,   434,   435,   436,
       0,     0,     0,     0,     0,   437,     0,   439,     0,     0,
       0,   442,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,   447,     0,     0,   448,     0,
       0,   449,     0,     0,     0,   452,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   559,     0,     0,   456,   457,
     312,   313,   314,     0,   316,   317,   318,   319,   320,   458,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,     0,   334,   335,   336,     0,     0,   339,   340,   341,
     342,   459,   460,   461,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   463,   464,     0,     0,
       0,     0,     0,     0,     0,  1527,     0,     0,     0,     0,
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
       0,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   456,
     457,   312,   313,   314,     0,   316,   317,   318,   319,   320,
     458,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,     0,   334,   335,   336,     0,     0,   339,   340,
     341,   342,   459,   460,   461,     0,     0,     0,     0,     0,
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
     435,   436,     0,     0,     0,     0,     0,   437,     0,   439,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,   444,     0,     0,     0,     0,     0,   447,     0,     0,
     448,     0,     0,   449,     0,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,     0,
     456,   457,   312,   313,   314,     0,   316,   317,   318,   319,
     320,   458,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,     0,   334,   335,   336,     0,     0,   339,
     340,   341,   342,   459,   460,   461,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   468,   469,   470,   471,   472,     0,   473,     0,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
      59,   560,   486,   487,     0,   679,     0,     0,     0,     0,
     488,   306,     0,     0,     0,     0,     0,   307,     0,     0,
       0,     0,     0,   308,     0,     0,     0,   491,   492,   493,
       0,    14,     0,   309,   494,   495,     0,     0,     0,     0,
       0,   310,  1328,     0,   497,     0,   498,   499,     0,     0,
       0,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,     0,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     306,     0,     0,     0,     0,     0,   307,     0,     0,     0,
       0,     0,   308,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,    58,     0,     0,     0,     0,     0,     0,
     310,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
      59,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,     0,     0,   306,
       0,     0,     0,     0,     0,   307,     0,     0,     0,     0,
       0,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   309,    58,     0,     0,     0,     0,     0,     0,   310,
     721,   722,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,     0,     0,   311,     0,     0,     0,     0,    59,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   346,     0,   598,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     306,     0,     0,     0,     0,     0,   307,     0,     0,     0,
       0,    58,   308,   731,   732,   733,   734,   735,     0,     0,
     738,     0,   309,   741,   345,   743,   744,   745,   746,     0,
     310,     0,     0,   747,     0,   749,   750,     0,   603,     0,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
     604,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   346,     0,     0,     0,     0,     0,
       0,     0,   765,   766,   767,   768,   769,   770,   771,   306,
       0,     0,     0,     0,     0,   307,     0,     0,   772,   773,
       0,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   309,    58,     0,     0,     0,     0,     0,     0,   310,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,     0,     0,   311,     0,     0,     0,     0,    59,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   721,   722,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   603,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   847,   848,
     849,   850,   851,   852,   853,   854,   731,   732,   733,   734,
     735,   855,   856,   738,   739,   740,   741,   857,   743,   744,
     745,   746,  -349,     0,   346,     0,   747,   748,   749,   750,
     858,   859,   721,   722,   753,   754,   755,   860,   861,   862,
     759,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   863,
     761,     0,   762,   763,   764,   765,   766,   767,   768,   769,
     770,   771,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   772,   773,     0,     0,     0,   498,   665,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   847,   848,   849,
     850,   851,   852,   853,   854,   731,   732,   733,   734,   735,
     855,   856,   738,   739,   740,   741,   857,   743,   744,   745,
     746,     0,   721,   722,     0,   747,   748,   749,   750,   858,
     859,     0,     0,   753,   754,   755,   860,   861,   862,   759,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   721,   722,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1050,     0,     0,     0,     0,     0,     0,   863,   761,
       0,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     772,   773,     0,     0,     0,   498,   665,   847,   848,   849,
     850,   851,   852,   853,   854,   731,   732,   733,   734,   735,
     855,   856,   738,   739,   740,   741,   857,   743,   744,   745,
     746,     0,     0,     0,     0,   747,   748,   749,   750,   858,
     859,     0,     0,   753,   754,   755,   860,   861,   862,   759,
     731,   732,   733,   734,   735,     0,     0,   738,   739,   740,
     741,     0,   743,   744,   745,   746,   721,   722,     0,     0,
     747,     0,   749,   750,     0,     0,     0,     0,   753,   754,
     755,  1237,     0,     0,   759,     0,     0,     0,   863,   761,
       0,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,   721,   722,     0,     0,     0,     0,     0,     0,     0,
     772,   773,     0,     0,     0,   498,   665,     0,     0,     0,
       0,     0,     0,     0,   761,     0,   762,   763,   764,   765,
     766,   767,   768,   769,   770,   771,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   772,   773,     0,     0,   784,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   731,
     732,   733,   734,   735,     0,     0,   738,   739,   740,   741,
       0,   743,   744,   745,   746,     0,     0,     0,     0,   747,
       0,   749,   750,     0,     0,     0,     0,   753,   754,   755,
       0,     0,     0,   759,   731,   732,   733,   734,   735,     0,
       0,   738,   739,   740,   741,     0,   743,   744,   745,   746,
     721,   722,     0,     0,   747,     0,   749,   750,     0,     0,
       0,     0,   753,   754,   755,     0,     0,     0,   759,     0,
       0,     0,     0,   761,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,   721,   722,     0,     0,     0,
       0,     0,     0,     0,   772,   773,     0,     0,   794,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   761,     0,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   772,
     773,     0,     0,   810,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   731,   732,   733,   734,   735,     0,     0,
     738,   739,   740,   741,     0,   743,   744,   745,   746,     0,
       0,     0,     0,   747,     0,   749,   750,     0,     0,     0,
       0,   753,   754,   755,     0,     0,     0,   759,   731,   732,
     733,   734,   735,     0,     0,   738,   739,   740,   741,     0,
     743,   744,   745,   746,   721,   722,     0,     0,   747,     0,
     749,   750,     0,     0,     0,     0,   753,   754,   755,     0,
       0,     0,   759,     0,     0,     0,     0,   761,     0,   762,
     763,   764,   765,   766,   767,   768,   769,   770,   771,   721,
     722,     0,     0,     0,     0,     0,     0,     0,   772,   773,
       0,     0,   825,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   761,     0,   762,   763,   764,   765,   766,   767,
     768,   769,   770,   771,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   772,   773,     0,     0,  1115,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   731,   732,   733,
     734,   735,     0,     0,   738,   739,   740,   741,     0,   743,
     744,   745,   746,     0,     0,     0,     0,   747,     0,   749,
     750,     0,     0,     0,     0,   753,   754,   755,     0,     0,
       0,   759,   731,   732,   733,   734,   735,     0,     0,   738,
     739,   740,   741,     0,   743,   744,   745,   746,   721,   722,
       0,     0,   747,     0,   749,   750,     0,     0,     0,     0,
     753,   754,   755,     0,     0,     0,   759,     0,     0,     0,
       0,   761,     0,   762,   763,   764,   765,   766,   767,   768,
     769,   770,   771,   721,   722,     0,     0,     0,     0,     0,
       0,     0,   772,   773,     0,     0,  1119,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   761,     0,   762,   763,
     764,   765,   766,   767,   768,   769,   770,   771,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,   773,     0,
       0,  1121,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   731,   732,   733,   734,   735,     0,     0,   738,   739,
     740,   741,     0,   743,   744,   745,   746,     0,     0,     0,
       0,   747,     0,   749,   750,     0,     0,     0,     0,   753,
     754,   755,     0,     0,     0,   759,   731,   732,   733,   734,
     735,     0,     0,   738,   739,   740,   741,     0,   743,   744,
     745,   746,   721,   722,     0,     0,   747,     0,   749,   750,
       0,     0,     0,     0,   753,   754,   755,     0,     0,     0,
     759,     0,     0,     0,     0,   761,     0,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,   721,   722,     0,
       0,     0,     0,     0,     0,     0,   772,   773,     0,     0,
    1130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     761,     0,   762,   763,   764,   765,   766,   767,   768,   769,
     770,   771,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   772,   773,     0,     0,  1131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   731,   732,   733,   734,   735,
       0,     0,   738,   739,   740,   741,     0,   743,   744,   745,
     746,     0,     0,     0,     0,   747,     0,   749,   750,     0,
       0,     0,     0,   753,   754,   755,     0,     0,     0,   759,
     731,   732,   733,   734,   735,     0,     0,   738,   739,   740,
     741,     0,   743,   744,   745,   746,   721,   722,     0,     0,
     747,     0,   749,   750,     0,     0,     0,     0,   753,   754,
     755,     0,     0,     0,   759,     0,     0,     0,     0,   761,
       0,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,   721,   722,     0,     0,     0,     0,     0,     0,     0,
     772,   773,     0,     0,  1132,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   761,     0,   762,   763,   764,   765,
     766,   767,   768,   769,   770,   771,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   772,   773,     0,     0,  1133,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   731,
     732,   733,   734,   735,     0,     0,   738,   739,   740,   741,
       0,   743,   744,   745,   746,     0,     0,     0,     0,   747,
       0,   749,   750,     0,     0,     0,     0,   753,   754,   755,
       0,     0,     0,   759,   731,   732,   733,   734,   735,     0,
       0,   738,   739,   740,   741,     0,   743,   744,   745,   746,
     721,   722,     0,     0,   747,     0,   749,   750,     0,     0,
       0,     0,   753,   754,   755,     0,     0,     0,   759,     0,
       0,     0,     0,   761,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,   721,   722,     0,     0,     0,
       0,     0,     0,     0,   772,   773,     0,     0,  1134,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   761,     0,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   772,
     773,     0,     0,  1135,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   731,   732,   733,   734,   735,     0,     0,
     738,   739,   740,   741,     0,   743,   744,   745,   746,     0,
       0,     0,     0,   747,     0,   749,   750,     0,     0,     0,
       0,   753,   754,   755,     0,     0,     0,   759,   731,   732,
     733,   734,   735,     0,     0,   738,   739,   740,   741,     0,
     743,   744,   745,   746,   721,   722,     0,     0,   747,     0,
     749,   750,     0,     0,     0,     0,   753,   754,   755,     0,
       0,     0,   759,     0,     0,     0,     0,   761,     0,   762,
     763,   764,   765,   766,   767,   768,   769,   770,   771,   721,
     722,     0,     0,     0,     0,     0,     0,     0,   772,   773,
       0,     0,  1258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   761,     0,   762,   763,   764,   765,   766,   767,
     768,   769,   770,   771,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   772,   773,     0,     0,  1268,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   731,   732,   733,
     734,   735,     0,     0,   738,   739,   740,   741,     0,   743,
     744,   745,   746,     0,     0,     0,     0,   747,     0,   749,
     750,     0,     0,     0,     0,   753,   754,   755,     0,     0,
       0,   759,   731,   732,   733,   734,   735,     0,     0,   738,
     739,   740,   741,     0,   743,   744,   745,   746,   721,   722,
       0,     0,   747,     0,   749,   750,     0,     0,     0,     0,
     753,   754,   755,     0,     0,     0,   759,     0,     0,     0,
       0,   761,     0,   762,   763,   764,   765,   766,   767,   768,
     769,   770,   771,   721,   722,     0,     0,     0,     0,     0,
       0,     0,   772,   773,     0,     0,  1392,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   761,     0,   762,   763,
     764,   765,   766,   767,   768,   769,   770,   771,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,   773,     0,
       0,  1406,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   731,   732,   733,   734,   735,     0,     0,   738,   739,
     740,   741,     0,   743,   744,   745,   746,     0,     0,     0,
       0,   747,     0,   749,   750,     0,     0,     0,     0,   753,
     754,   755,     0,     0,     0,   759,   731,   732,   733,   734,
     735,     0,     0,   738,   739,   740,   741,     0,   743,   744,
     745,   746,   721,   722,     0,     0,   747,     0,   749,   750,
       0,     0,     0,     0,   753,   754,   755,     0,     0,     0,
     759,     0,     0,     0,     0,   761,     0,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,   721,   722,     0,
       0,     0,     0,     0,     0,     0,   772,   773,     0,     0,
    1407,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     761,     0,   762,   763,   764,   765,   766,   767,   768,   769,
     770,   771,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   772,   773,     0,     0,  1426,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   731,   732,   733,   734,   735,
       0,     0,   738,   739,   740,   741,     0,   743,   744,   745,
     746,     0,     0,     0,     0,   747,     0,   749,   750,     0,
       0,     0,     0,   753,   754,   755,     0,     0,     0,   759,
     731,   732,   733,   734,   735,     0,     0,   738,   739,   740,
     741,     0,   743,   744,   745,   746,   721,   722,     0,     0,
     747,     0,   749,   750,     0,     0,     0,     0,   753,   754,
     755,     0,     0,     0,   759,     0,     0,     0,     0,   761,
       0,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,   721,   722,     0,     0,     0,     0,     0,     0,     0,
     772,   773,     0,     0,  1428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   761,     0,   762,   763,   764,   765,
     766,   767,   768,   769,   770,   771,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   772,   773,     0,     0,  1430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   731,
     732,   733,   734,   735,     0,     0,   738,   739,   740,   741,
       0,   743,   744,   745,   746,     0,     0,     0,     0,   747,
       0,   749,   750,     0,     0,     0,     0,   753,   754,   755,
       0,     0,     0,   759,   731,   732,   733,   734,   735,     0,
       0,   738,   739,   740,   741,     0,   743,   744,   745,   746,
     721,   722,     0,     0,   747,     0,   749,   750,     0,     0,
       0,     0,   753,   754,   755,     0,     0,     0,   759,     0,
       0,     0,     0,   761,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,   721,   722,     0,     0,     0,
       0,     0,     0,     0,   772,   773,     0,     0,  1434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   761,     0,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   772,
     773,     0,     0,  1496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   731,   732,   733,   734,   735,     0,     0,
     738,   739,   740,   741,     0,   743,   744,   745,   746,     0,
       0,     0,     0,   747,     0,   749,   750,     0,     0,     0,
       0,   753,   754,   755,     0,     0,     0,   759,   731,   732,
     733,   734,   735,     0,     0,   738,   739,   740,   741,     0,
     743,   744,   745,   746,   721,   722,     0,     0,   747,     0,
     749,   750,     0,     0,     0,     0,   753,   754,   755,     0,
       0,     0,   759,     0,     0,     0,     0,   761,     0,   762,
     763,   764,   765,   766,   767,   768,   769,   770,   771,   721,
     722,     0,     0,     0,     0,     0,     0,     0,   772,   773,
       0,     0,  1497,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   761,     0,   762,   763,   764,   765,   766,   767,
     768,   769,   770,   771,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   772,   773,     0,     0,  1498,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   731,   732,   733,
     734,   735,     0,     0,   738,   739,   740,   741,     0,   743,
     744,   745,   746,     0,     0,     0,     0,   747,     0,   749,
     750,     0,     0,     0,     0,   753,   754,   755,     0,     0,
       0,   759,   731,   732,   733,   734,   735,     0,     0,   738,
     739,   740,   741,     0,   743,   744,   745,   746,   721,   722,
       0,     0,   747,     0,   749,   750,     0,     0,     0,     0,
     753,   754,   755,     0,     0,     0,   759,     0,     0,     0,
       0,   761,     0,   762,   763,   764,   765,   766,   767,   768,
     769,   770,   771,   721,   722,     0,     0,     0,     0,     0,
       0,     0,   772,   773,     0,     0,  1506,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   761,     0,   762,   763,
     764,   765,   766,   767,   768,   769,   770,   771,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   772,   773,     0,
       0,  1513,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   731,   732,   733,   734,   735,     0,     0,   738,   739,
     740,   741,     0,   743,   744,   745,   746,     0,     0,     0,
       0,   747,     0,   749,   750,     0,     0,     0,     0,   753,
     754,   755,     0,     0,     0,   759,   731,   732,   733,   734,
     735,     0,     0,   738,   739,   740,   741,     0,   743,   744,
     745,   746,   721,   722,     0,     0,   747,     0,   749,   750,
       0,     0,     0,     0,   753,   754,   755,     0,     0,     0,
     759,     0,     0,     0,     0,   761,     0,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,   721,   722,     0,
       0,     0,     0,     0,     0,     0,   772,   773,     0,     0,
    1518,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     761,     0,   762,   763,   764,   765,   766,   767,   768,   769,
     770,   771,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   772,   773,     0,     0,  1567,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   731,   732,   733,   734,   735,
       0,     0,   738,   739,   740,   741,     0,   743,   744,   745,
     746,     0,     0,     0,     0,   747,     0,   749,   750,     0,
       0,     0,     0,   753,   754,   755,     0,     0,     0,   759,
     731,   732,   733,   734,   735,   721,   722,   738,   739,   740,
     741,     0,   743,   744,   745,   746,     0,     0,     0,     0,
     747,     0,   749,   750,     0,     0,     0,     0,   753,   754,
     755,     0,     0,     0,   759,     0,     0,     0,     0,   761,
       0,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     772,   773,   815,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   761,     0,   762,   763,   764,   765,
     766,   767,   768,   769,   770,   771,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   772,   773,  1072,   731,   732,
     733,   734,   735,   721,   722,   738,   739,   740,   741,     0,
     743,   744,   745,   746,     0,     0,     0,     0,   747,     0,
     749,   750,     0,     0,     0,     0,   753,   754,   755,     0,
       0,     0,   759,     0,     0,     0,     0,     0,   721,   722,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   761,     0,   762,   763,   764,   765,   766,   767,
     768,   769,   770,   771,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   772,   773,  1211,   731,   732,   733,   734,
     735,     0,     0,   738,   739,   740,   741,     0,   743,   744,
     745,   746,     0,     0,     0,     0,   747,     0,   749,   750,
       0,     0,     0,     0,   753,   754,   755,     0,     0,     0,
     759,   731,   732,   733,   734,   735,   721,   722,   738,   739,
     740,   741,     0,   743,   744,   745,   746,     0,     0,     0,
       0,   747,     0,   749,   750,     0,     0,     0,     0,   753,
     754,   755,     0,     0,     0,   759,     0,     0,     0,     0,
     761,     0,   762,   763,   764,   765,   766,   767,   768,   769,
     770,   771,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   772,   773,  1226,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   761,     0,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   772,   773,  1350,   731,
     732,   733,   734,   735,     0,     0,   738,   739,   740,   741,
       0,   743,   744,   745,   746,   229,   230,     0,     0,   747,
       0,   749,   750,     0,     0,     0,     0,   753,   754,   755,
       0,     0,   231,   759,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     721,   722,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   761,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   772,   773,  1355,     0,     0,     0,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,     0,     0,
     250,   251,   252,     0,     0,     0,     0,     0,     0,   253,
     254,   255,   256,   257,     0,     0,   258,   259,   260,   261,
     262,   263,   264,   731,   732,   733,   734,   735,   721,   722,
     738,   739,   740,   741,     0,   743,   744,   745,   746,     0,
       0,     0,     0,   747,     0,   749,   750,     0,     0,     0,
       0,   753,   754,   755,     0,     0,     0,   759,     0,     0,
       0,     0,   265,     0,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,     0,     0,   276,   277,     0,     0,
       0,     0,     0,   278,   279,     0,     0,   284,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   761,     0,   762,
     763,   764,   765,   766,   767,   768,   769,   770,   771,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   772,   773,
       0,   731,   732,   733,   734,   735,   721,   722,   738,   739,
     740,   741,     0,   743,   744,   745,   746,     0,     0,     0,
       0,   747,     0,   749,   750,     0,     0,   913,     0,   753,
     754,   755,     0,     0,     0,   759,     0,     0,     0,     0,
       0,   721,   722,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   761,     0,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   772,   773,     0,   731,
     732,   733,   734,   735,     0,     0,   738,   739,   740,   741,
       0,   743,   744,   745,   746,     0,     0,     0,     0,   747,
       0,   749,   750,     0,     0,  1144,     0,   753,   754,   755,
       0,   721,   722,   759,   731,   732,   733,   734,   735,     0,
       0,   738,   739,   740,   741,     0,   743,   744,   745,   746,
       0,     0,     0,     0,   747,     0,   749,   750,     0,     0,
       0,     0,   753,   754,   755,     0,   721,   722,   759,     0,
       0,     0,     0,   761,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   772,   773,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   761,  1216,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
       0,     0,     0,     0,   731,   732,   733,   734,   735,   772,
     773,   738,   739,   740,   741,     0,   743,   744,   745,   746,
       0,     0,     0,     0,   747,     0,   749,   750,     0,     0,
       0,     0,   753,   754,   755,     0,   721,   722,   759,   731,
     732,   733,   734,   735,     0,     0,   738,   739,   740,   741,
       0,   743,   744,   745,   746,     0,     0,     0,     0,   747,
       0,   749,   750,     0,     0,     0,     0,   753,   754,   755,
    1228,   721,   722,   759,     0,     0,     0,     0,   761,     0,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   772,
     773,  1314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   761,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,     0,     0,     0,     0,   731,
     732,   733,   734,   735,   772,   773,   738,   739,   740,   741,
       0,   743,   744,   745,   746,     0,     0,     0,     0,   747,
       0,   749,   750,     0,     0,     0,     0,   753,   754,   755,
       0,   721,   722,   759,   731,   732,   733,   734,   735,     0,
       0,   738,   739,   740,   741,     0,   743,   744,   745,   746,
       0,     0,     0,     0,   747,     0,   749,   750,     0,     0,
       0,     0,   753,   754,   755,  1514,   721,   722,   759,     0,
       0,     0,     0,   761,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   772,   773,     0,     0,     0,     0,
    1563,     0,     0,     0,     0,     0,     0,     0,   761,     0,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
       0,     0,     0,     0,   731,   732,   733,   734,   735,   772,
     773,   738,   739,   740,   741,     0,   743,   744,   745,   746,
       0,     0,     0,     0,   747,     0,   749,   750,     0,     0,
       0,     0,   753,   754,   755,     0,   721,   722,   759,   731,
     732,   733,   734,   735,     0,     0,   738,   739,   740,   741,
       0,   743,   744,   745,   746,     0,     0,     0,     0,   747,
       0,   749,   750,     0,     0,     0,     0,   753,   754,   755,
    1578,   721,   722,   759,     0,     0,     0,     0,   761,     0,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   772,
     773,     0,     0,     0,     0,  1579,     0,     0,     0,     0,
       0,     0,     0,   761,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,     0,     0,     0,     0,   731,
     732,   733,   734,   735,   772,   773,   738,   739,   740,   741,
       0,   743,   744,   745,   746,     0,     0,     0,     0,   747,
       0,   749,   750,     0,     0,     0,     0,   753,   754,   755,
       0,   721,   722,   759,   731,   732,   733,   734,   735,     0,
       0,   738,   739,   740,   741,     0,   743,   744,   745,   746,
       0,     0,     0,     0,   747,     0,   749,   750,     0,     0,
       0,     0,   753,   754,   755,  1581,   721,   722,   759,     0,
       0,     0,     0,   761,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   772,   773,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   761,     0,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
       0,     0,     0,     0,   731,   732,   733,   734,   735,   772,
     773,   738,   739,   740,   741,     0,   743,   744,   745,   746,
       0,     0,     0,     0,   747,     0,   749,   750,     0,     0,
       0,     0,   753,   754,   755,     0,   721,   722,  -738,   731,
     732,   733,   734,   735,     0,     0,   738,   739,   740,   741,
       0,   743,   744,   745,   746,     0,     0,     0,     0,   747,
       0,   749,   750,     0,     0,   721,   722,   753,   754,   755,
       0,     0,     0,     0,     0,     0,     0,     0,   761,     0,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   772,
     773,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   761,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,     0,     0,     0,     0,   731,
     732,   733,   734,   735,   772,   773,   738,   739,   740,   741,
       0,   743,   744,   745,   746,     0,     0,     0,     0,   747,
       0,   749,   750,     0,     0,   721,   722,   753,   731,   732,
     733,   734,   735,     0,     0,   738,   739,   740,   741,     0,
     743,   744,   745,   746,     0,     0,     0,     0,   747,     0,
     749,   750,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,     0,     0,     0,     0,     0,
       0,   952,     0,     0,   772,   773,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   763,   764,   765,   766,   767,
     768,   769,   770,   771,     0,     0,     0,     0,   731,   732,
     733,   734,   735,   772,   773,   738,   739,   740,   741,     0,
     743,   744,   745,   746,     0,     0,     0,     0,   747,     0,
     749,   750,   312,   313,   314,     0,   316,   317,   318,   319,
     320,   458,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,     0,   334,   335,   336,     0,     0,   339,
     340,   341,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   956,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   765,   766,   767,
     768,   769,   770,   771,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   772,   773,     0,     0,  1212,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   953,     0,
       0,     0,     0,     0,     0,     0,     0,   312,   313,   314,
     954,   316,   317,   318,   319,   320,   458,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,     0,   334,
     335,   336,     0,     0,   339,   340,   341,   342,   312,   313,
     314,     0,   316,   317,   318,   319,   320,   458,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,     0,
     334,   335,   336,     0,     0,   339,   340,   341,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     181,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   957,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   958,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   182,     0,   183,     0,   184,
     185,   186,   187,   188,  1213,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,  1214,   200,   201,   202,
       0,     0,   203,   204,   205,   206,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   207,   208,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   209
};

static const yytype_int16 yycheck[] =
{
      14,    15,   211,   477,   225,   637,   616,   438,   565,   161,
     371,   371,   585,   680,   699,   383,   701,   532,   584,   378,
     888,   225,   678,   438,   680,   579,  1147,   581,    32,   583,
      52,     7,   465,   466,   467,   103,   371,    21,   140,   904,
       0,   556,  1331,  1367,   412,   910,     6,   462,   124,    63,
      64,    65,    19,    52,   801,   124,   803,   160,   805,     6,
     839,    14,    15,  1474,    20,    21,   497,    18,    19,    29,
      19,    31,    56,    33,  1499,   167,   621,   438,    19,    39,
      32,   185,   515,    45,    19,    19,   188,   129,    56,    49,
     104,   105,   106,   107,    62,    55,    33,   141,   142,   143,
     189,   462,    49,   438,   193,    19,   210,   475,   476,   542,
     186,    32,   167,   189,  1525,  1540,   185,   186,    78,   188,
     123,   213,   191,   126,   127,    62,   152,   462,  1417,    14,
      15,   157,   677,   155,   679,    61,   681,   179,    59,    60,
     100,   101,   104,    49,   689,   167,  1470,   299,   369,   177,
     218,   139,   667,   698,   569,   179,   211,   588,   843,   115,
     116,   179,   383,   140,   182,   369,   124,   123,   212,   125,
     126,   127,   128,  1294,   466,   467,   134,   133,   171,   172,
     179,   866,   140,   211,   185,    91,   212,   191,   212,   177,
     167,   412,   210,   414,   879,   177,   133,   882,   629,   630,
     121,   634,   178,   140,   125,   208,   209,   160,   569,   210,
     178,   225,   165,   197,   167,   630,   167,   170,   185,   179,
      32,    20,    21,   654,   208,   124,   657,   124,   186,   662,
     167,   185,   441,   209,   569,   134,   185,   134,   198,   191,
     196,   197,   198,   179,   185,   660,   455,    59,    60,   209,
     185,   185,   208,   209,   475,   476,   210,   209,   211,   149,
    1128,   177,   192,   215,   186,   202,   187,   157,   636,   630,
     191,   185,  1393,   194,   211,   160,   185,   167,  1143,   710,
     165,   211,   167,   124,   940,   170,   719,   186,   209,   186,
     124,   659,   191,   134,   215,   630,   185,   213,   214,   660,
     134,   126,   127,   212,   167,   168,   169,     4,     5,   121,
       7,   182,   183,   125,   192,   185,   115,   116,   185,  1187,
     177,   211,   126,   127,   123,   660,   125,   126,   127,   128,
     155,   188,   346,   211,   133,   198,   152,   124,    35,   210,
     210,   157,   167,   210,   775,   186,   185,   134,   179,   570,
     571,   572,   186,   574,   575,   369,   789,   578,   717,   580,
     374,   582,   193,   584,   213,   214,   570,   571,   572,   383,
     574,   575,   787,   212,   578,   187,   580,   124,   582,   191,
     188,   193,   194,   208,   209,   185,    32,   134,   723,   724,
     725,   726,   727,   728,   729,   730,   950,   209,   412,   186,
     414,   736,   737,   215,   208,   209,   185,   742,   839,   208,
     209,   620,   212,    59,    60,   636,   751,   752,   833,   167,
     434,   756,   757,   758,   939,   760,   787,   719,  1293,   185,
     185,   185,   641,   140,   140,    46,   179,   140,   659,   186,
    1305,   915,   149,   186,   185,   189,   149,   154,   185,   193,
     192,   925,   787,   886,  1201,    66,   212,   212,   212,   185,
     167,   475,   476,   896,   167,   185,   834,   835,  1078,   211,
     901,   212,   833,   210,   842,   121,  1143,   686,   909,   125,
      63,    64,    65,   185,   188,   192,   212,  1143,   188,   193,
    1146,  1158,   212,   193,  1067,   179,   705,   192,   833,  1065,
     721,   722,  1158,   871,   872,   160,   874,   211,   210,   193,
     878,   211,   880,   881,   188,   883,   211,   149,   211,   193,
     188,   104,   105,   106,   107,   193,   747,   960,   961,   962,
     192,   208,   965,   179,   967,   167,   969,   211,   971,  1286,
     973,   187,   975,   211,   977,   191,   979,   193,   194,   211,
     189,   984,   188,   986,   193,   191,   570,   571,   572,   992,
     574,   575,   179,   209,   578,   149,   580,   179,   582,   215,
     584,  1004,    32,  1006,  1321,  1129,   193,   179,  1011,   179,
    1013,   193,  1015,   167,   177,  1018,   178,   179,   179,   181,
     811,   193,   185,   193,    11,    20,    21,   167,   819,    59,
      60,   822,   193,   167,   896,    22,    23,   811,   167,   179,
     831,   167,   626,   834,   835,   819,  1049,   838,   822,   179,
     841,   842,   636,   179,   167,   179,  1180,   831,   155,   189,
     155,   783,   155,    56,   838,   188,   179,   841,   191,    62,
     167,  1273,   167,    56,   167,   659,   155,  1080,    56,    62,
     871,   872,  1083,   874,    62,   864,   167,   878,   167,   880,
     881,   121,   883,  1442,  1443,   125,  1413,   155,   960,   961,
     962,  1466,  1328,  1468,  1453,  1454,    56,  1472,  1473,   167,
     185,   189,    62,   188,   179,   193,   191,  1297,   702,   178,
     115,   116,   181,   167,  1379,   184,   167,   918,   123,   920,
      56,   126,   127,   128,    56,  1136,    62,    56,   133,   179,
      62,   179,   179,    62,   918,   185,   920,   185,   185,   141,
     189,   143,   179,   179,   193,   171,  1157,   187,   185,   185,
     167,   191,  1163,   193,   194,    86,    87,    32,   189,  1170,
       4,     5,   193,  1174,  1112,   171,   172,   189,   189,   209,
      73,   193,   193,   189,    77,   215,   167,   193,   189,   189,
      24,   177,   193,   193,    59,    60,    30,   781,    91,    92,
      56,   171,  1326,    96,    97,    98,    99,   179,  1209,   141,
     142,   143,  1392,   208,   209,   171,   172,   173,   174,   104,
    1221,   167,   168,   169,   171,   172,   173,   811,  1019,   178,
     179,   180,   141,    67,    68,   819,   177,   918,   822,   920,
     157,  1179,  1173,  1173,   186,  1447,   177,   831,   177,    34,
     834,   835,  1507,  1508,   838,  1510,   121,   841,   842,    34,
     125,   177,   167,  1054,   178,   167,   100,   101,  1173,    10,
      11,    12,   211,   167,  1065,   167,   210,    21,   167,   177,
    1054,   188,   210,    42,    20,    21,  1287,   871,   872,   188,
     874,   177,  1419,   211,   878,   192,   880,   881,   211,   883,
     211,   192,   192,   211,   192,   192,   140,   167,   192,   192,
     192,   192,   192,   192,   189,   134,   211,   167,   167,   167,
     192,  1112,   187,   157,   158,   159,   191,   167,   193,   194,
     192,   192,   210,   167,   918,   209,   920,   192,  1518,   192,
     211,   211,   211,   177,   209,   192,   179,   160,   192,   192,
     215,   192,   211,   211,   211,   192,    36,  1401,   192,   211,
    1404,  1488,  1489,   214,   179,   211,  1367,   179,     9,   211,
     179,   179,   179,   179,   179,   209,    65,   113,   114,   115,
     116,   209,   211,   178,   211,    42,   212,   123,  1179,   125,
     126,   127,   128,   192,   167,   167,   210,   133,   179,   135,
     136,  1339,  1181,   179,  1531,   724,   725,   726,   727,   728,
     729,   730,   178,   178,   211,   179,   178,   736,   737,   188,
      42,   211,   185,   742,    20,    21,   129,    13,    12,   186,
     188,   160,   751,   752,    18,   214,   185,   756,   757,   758,
      24,   760,   177,   185,   157,  1446,    30,   167,     7,   167,
     193,  1452,   212,   167,   185,   167,    40,   211,   194,   195,
     196,   197,   198,   185,    48,  1256,   212,   178,   178,  1470,
    1054,   210,   208,   209,   211,   167,  1477,   211,   211,    63,
    1418,  1065,  1256,   192,  1263,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   212,   115,
     116,   192,     1,   211,   211,  1526,   211,   123,  1112,   125,
     126,   127,   128,   211,   211,   178,    66,   133,   167,   179,
     193,   193,   193,    42,  1323,   212,   167,   212,  1339,  1487,
    1341,   211,   167,   212,   212,   211,   140,   212,   167,  1338,
     210,   210,   210,   167,   167,   192,   211,  1341,   179,   153,
     167,   179,   211,   211,   211,   211,   211,   210,   212,   167,
     210,    32,   210,   167,   167,   211,   211,   211,   211,    52,
     688,   211,   211,   211,   211,  1179,   212,   371,   194,   195,
     196,   197,   198,   193,   193,   212,   380,   179,   178,  1388,
     212,   212,   208,   209,   212,   389,   178,   212,   212,   210,
     212,   178,    11,   210,   179,   399,   178,  1418,     1,   213,
     212,    20,    21,   212,   212,   409,   210,   210,    79,    44,
    1431,   133,    82,  1410,   418,  1411,   223,    80,   781,   927,
    1411,  1411,  1411,  1411,   428,     1,  1388,  1431,   946,  1336,
    1359,   515,  1439,  1233,   438,  1362,    20,    21,  1440,   443,
     422,   445,  1256,    52,  1399,  1153,   680,   554,   434,   434,
     454,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   462,   463,
     464,    -1,    -1,    -1,    -1,    -1,  1487,    -1,    -1,    -1,
      -1,    -1,    -1,   477,    -1,    -1,  1485,    -1,    -1,    -1,
      -1,   485,    -1,    -1,   488,    -1,    -1,   491,   492,   493,
     494,   495,    -1,    -1,   113,   114,   115,   116,   117,    -1,
     504,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,  1339,    32,  1341,   147,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    59,    60,    -1,    -1,  1566,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   569,    -1,    -1,   187,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,
     209,    -1,    -1,   597,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1418,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   121,    -1,  1431,   124,   125,
      -1,    -1,    -1,    -1,   208,   209,   630,    -1,   134,    -1,
      -1,    -1,    -1,   637,   638,    -1,   640,    -1,    -1,    -1,
      -1,   645,    -1,    -1,    -1,    -1,    -1,   651,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   660,    -1,    -1,    -1,
    1474,    -1,    -1,    -1,    -1,    -1,    -1,   671,   672,   673,
     674,   675,   676,  1487,   678,    -1,   680,    -1,    -1,    -1,
     186,   187,    -1,    -1,    -1,   191,    -1,    -1,   194,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,   215,
      -1,  1525,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   723,
     724,   725,   726,   727,   728,   729,   730,   731,   732,    -1,
      -1,   735,   736,   737,   738,   739,   740,    -1,   742,   743,
      -1,   745,   746,   747,   748,   749,   750,   751,   752,   753,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
     764,   765,   766,   767,   768,   769,   770,   771,    -1,   773,
      -1,    -1,    -1,   777,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   787,    -1,    -1,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,    -1,   826,    -1,    -1,    -1,    -1,    -1,    -1,   833,
      -1,    -1,    -1,    -1,    -1,    -1,   840,    -1,    -1,    -1,
      -1,    -1,   846,   847,   848,   849,   850,   851,   852,   853,
     854,   855,   856,   857,   858,   859,   860,   861,   862,   863,
     187,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,   208,   209,    -1,    -1,    -1,   213,   214,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   899,   900,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    -1,   913,
      -1,   915,    -1,    -1,    -1,    -1,    -1,    -1,   922,     4,
       5,   925,    -1,    -1,    -1,   929,    -1,    -1,    -1,    14,
      15,    16,    17,    18,   938,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    52,   121,    54,
      -1,   124,   125,    -1,    59,    60,    -1,    -1,    -1,    64,
      -1,   134,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,  1032,    -1,
     115,   116,  1036,   186,   187,    -1,   121,    -1,   191,    -1,
     125,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   209,    -1,    -1,    -1,
      -1,    -1,   215,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,    -1,    -1,    -1,   191,    -1,    -1,   194,
     195,   196,  1116,   198,    -1,    -1,   201,   202,    -1,    -1,
      -1,    -1,    -1,    -1,   209,    -1,   211,    -1,   213,   214,
     215,    -1,    -1,    -1,  1138,  1139,  1140,    -1,    -1,    -1,
    1144,    -1,    -1,    -1,    -1,    20,    21,    -1,  1152,  1153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1173,
      -1,    -1,    -1,  1177,  1178,     9,    -1,    -1,    -1,    -1,
    1184,    -1,    -1,    -1,    -1,  1189,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1203,
      -1,  1205,    -1,    -1,    -1,    -1,  1210,    -1,    -1,    -1,
      -1,    -1,  1216,    -1,    -1,    -1,  1220,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,  1260,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,    -1,   143,  1273,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,   138,    -1,    -1,   141,   142,   143,
     144,   145,   146,   147,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1347,    -1,  1349,    -1,    -1,    -1,    -1,
    1354,    -1,   186,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,  1375,    -1,    -1,   208,   209,  1380,  1381,  1382,   213,
     214,    -1,    -1,  1387,    -1,    -1,    -1,  1391,    -1,    -1,
      -1,  1395,  1396,  1397,  1398,    -1,    -1,  1401,  1402,    -1,
    1404,  1405,    -1,    -1,    -1,    -1,    -1,  1411,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1422,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,  1447,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1460,  1461,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1478,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1490,  1491,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1505,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,
     209,    -1,    -1,  1527,    -1,    -1,    -1,    -1,  1532,  1533,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1549,     1,  1551,  1552,     4,
       5,     6,    -1,     8,     9,    10,  1560,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      25,    26,    27,    28,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    -1,    41,    -1,    43,    44,
      -1,    -1,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,    -1,
      -1,    -1,    -1,    -1,   209,    -1,   211,     1,   213,   214,
       4,     5,     6,    -1,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    25,    26,    27,    28,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    41,    -1,    43,
      44,    -1,    -1,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,    32,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,   179,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,
      -1,     4,     5,    -1,    -1,   209,    -1,   211,    -1,   213,
     214,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   121,    -1,
      -1,    44,   125,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,   191,    -1,
     193,   194,   115,   116,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,   215,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,    -1,    32,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,   191,    -1,
      -1,   194,   195,   196,    -1,   198,    59,    60,   201,   202,
      -1,    -1,    -1,     4,     5,    -1,   209,    -1,   211,    -1,
     213,   214,   215,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    64,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,   191,    -1,
     193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   148,   149,   150,
     151,   152,   215,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,    -1,    -1,   194,   195,   196,    -1,   198,    -1,    -1,
     201,   202,    -1,    -1,    -1,     4,     5,    -1,   209,    -1,
     211,    -1,   213,   214,   215,    14,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,    32,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,    -1,   198,
      -1,    -1,   201,   202,    -1,    -1,     4,     5,    -1,    -1,
     209,    -1,   211,    -1,   213,   214,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,   121,    -1,    -1,    44,   125,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,   191,    -1,   193,   194,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,   209,   130,   131,   132,    -1,    -1,   215,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    32,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,    -1,
     198,    -1,    -1,   201,   202,    -1,    -1,     4,     5,    -1,
      -1,   209,    -1,   211,    -1,   213,   214,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,
      27,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,   121,    -1,    -1,    44,   125,    -1,
      47,    -1,    -1,    50,    51,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
     187,    -1,    -1,    -1,   191,    -1,   193,   194,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,   215,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    32,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,   201,   202,    -1,    -1,     4,     5,
      -1,    -1,   209,    -1,   211,    -1,   213,   214,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,   121,    -1,    -1,    44,   125,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    -1,
      96,    97,    98,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   187,    -1,    -1,    -1,   191,    -1,   193,   194,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   130,   131,   132,    -1,    -1,   215,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,   151,   152,    32,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,    -1,   198,    -1,    -1,   201,   202,    -1,    -1,     4,
       5,    -1,    -1,   209,    -1,   211,    -1,   213,   214,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,   121,    -1,    -1,    44,
     125,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   187,    -1,    -1,    -1,   191,    -1,   193,   194,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   209,   130,   131,   132,    -1,    -1,
     215,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    32,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,    -1,
       4,     5,    -1,    -1,   209,    -1,   211,    -1,   213,   214,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,   121,    -1,    -1,
      44,   125,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   187,    -1,    -1,    -1,   191,    -1,   193,
     194,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,
      -1,   215,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,    32,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,
      -1,     4,     5,    -1,    -1,   209,    -1,   211,   212,   213,
     214,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   121,    -1,
      -1,    44,   125,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,   191,    -1,
     193,   194,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,   215,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
      32,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,    -1,   198,    -1,    -1,   201,   202,
      -1,    -1,    -1,    -1,     4,     5,   209,    -1,   211,   212,
     213,   214,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    32,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,   121,
      -1,    -1,    -1,   125,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,   187,    -1,    -1,    -1,   191,
      -1,   193,   194,    -1,    -1,   115,   116,    -1,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,   209,    -1,    -1,
      -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,   151,   152,    32,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      59,    60,    -1,    -1,   187,    -1,    -1,    -1,   191,    -1,
     193,   194,    -1,    -1,   194,   195,   196,    -1,   198,    -1,
      -1,   201,   202,    -1,    -1,    -1,   209,     4,     5,   209,
     210,   211,   215,   213,   214,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,   121,    -1,    -1,    -1,   125,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,   187,    -1,
      -1,    -1,   191,    -1,   193,   194,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,    -1,    -1,    -1,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,
       4,     5,   209,    -1,   211,    -1,   213,   214,    12,    -1,
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
      -1,     4,     5,    -1,    -1,   209,   210,   211,    -1,   213,
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
      -1,    -1,     4,     5,    -1,    -1,   209,   210,   211,    -1,
     213,   214,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,    -1,
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
      -1,   213,   214,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    57,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   192,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,
       4,     5,   209,    -1,   211,    -1,   213,   214,    12,    -1,
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
     201,   202,    -1,    -1,     4,     5,    -1,    -1,   209,    -1,
     211,    -1,   213,   214,    14,    15,    16,    17,    18,    -1,
      -1,    21,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
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
      -1,   211,    -1,   213,   214,    14,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   192,    -1,   194,   195,   196,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,   201,   202,    -1,    -1,     4,     5,
      -1,    -1,   209,    -1,   211,   212,   213,   214,    14,    15,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,    -1,   198,    -1,    -1,   201,   202,    -1,    -1,
       4,     5,    -1,    -1,   209,    -1,   211,   212,   213,   214,
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
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,    -1,   198,    -1,    -1,
     201,   202,    -1,    -1,     4,     5,    -1,    -1,   209,    -1,
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
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,   151,   152,    -1,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,    -1,   198,    -1,
      -1,   201,   202,    -1,    -1,     4,     5,    -1,    -1,   209,
      -1,   211,    -1,   213,   214,    14,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
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
     167,   168,   169,   170,    -1,    12,    -1,    -1,    -1,    -1,
     177,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,   194,   195,   196,
      -1,   198,    -1,    40,   201,   202,    -1,    -1,    -1,    -1,
      -1,    48,   209,    -1,   211,    -1,   213,   214,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
     167,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,   140,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      20,    21,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,   167,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   213,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,   140,    30,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,    40,   123,   153,   125,   126,   127,   128,    -1,
      48,    -1,    -1,   133,    -1,   135,   136,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
     179,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   192,   193,   194,   195,   196,   197,   198,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,   208,   209,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,   140,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,   167,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,   213,    -1,   133,   134,   135,   136,
     137,   138,    20,    21,   141,   142,   143,   144,   145,   146,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
     187,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   209,    -1,    -1,    -1,   213,   214,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    20,    21,    -1,   133,   134,   135,   136,   137,
     138,    -1,    -1,   141,   142,   143,   144,   145,   146,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,
      -1,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     208,   209,    -1,    -1,    -1,   213,   214,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,
     138,    -1,    -1,   141,   142,   143,   144,   145,   146,   147,
     113,   114,   115,   116,   117,    -1,    -1,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    20,    21,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,   179,    -1,    -1,   147,    -1,    -1,    -1,   186,   187,
      -1,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     208,   209,    -1,    -1,    -1,   213,   214,    -1,    -1,    -1,
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
     113,   114,   115,   116,   117,    20,    21,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,   187,
      -1,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     208,   209,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   208,   209,   210,   113,   114,
     115,   116,   117,    20,    21,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,    -1,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   208,   209,   210,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
     187,    -1,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   209,   210,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,   209,   210,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    20,    21,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    37,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   208,   209,   210,    -1,    -1,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    -1,    -1,
     125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,   141,   142,   143,   144,
     145,   146,   147,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,   187,    -1,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,    -1,    -1,   201,   202,    -1,    -1,
      -1,    -1,    -1,   208,   209,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,   209,
      -1,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,   139,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,   139,    -1,   141,   142,   143,
      -1,    20,    21,   147,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    20,    21,   147,    -1,
      -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   208,   209,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   208,
     209,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    20,    21,   147,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
     179,    20,    21,   147,    -1,    -1,    -1,    -1,   187,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,
     209,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   208,   209,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    20,    21,   147,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,   179,    20,    21,   147,    -1,
      -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   208,   209,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   208,
     209,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    20,    21,   147,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
     179,    20,    21,   147,    -1,    -1,    -1,    -1,   187,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,
     209,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   208,   209,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    20,    21,   147,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,   179,    20,    21,   147,    -1,
      -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   208,   209,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   208,
     209,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    20,    21,   147,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    20,    21,   141,   142,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   208,   209,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    20,    21,   141,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,   208,   209,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,   191,   192,   193,   194,
     195,   196,   197,   198,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   208,   209,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,   193,   194,
     195,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   208,   209,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
     167,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    -1,    96,    97,    98,    99,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    71,    -1,    73,
      74,    75,    76,    77,   155,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,   167,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   217,     0,     6,    29,    31,    33,    39,    49,    55,
      78,   100,   101,   179,   198,   209,   218,   221,   227,   229,
     230,   235,   264,   268,   301,   379,   386,   390,   401,   445,
     450,   455,    18,    19,   167,   257,   258,   259,   160,   236,
     237,   167,   168,   169,   198,   231,   232,   233,   177,   387,
     167,   213,   220,    56,    62,   382,   382,   382,   140,   167,
     287,    33,    62,   133,   202,   211,   260,   261,   262,   263,
     287,   179,     4,     5,     7,    35,   398,    61,   377,   186,
     185,   188,   185,   232,    21,    56,   197,   208,   234,   382,
     383,   385,   383,   377,   456,   446,   451,   167,   140,   228,
     262,   262,   262,   211,   141,   142,   143,   185,   210,    56,
      62,   269,   271,    56,    62,   388,    56,    62,   399,    56,
      62,   378,    14,    15,   160,   165,   167,   170,   211,   223,
     258,   160,   237,   167,   231,   231,   167,   179,   178,   383,
     179,    56,    62,   219,   167,   167,   167,   167,   171,   226,
     212,   259,   262,   262,   262,   262,   272,   167,   389,   402,
     177,   380,   171,   172,   222,    14,    15,   160,   165,   167,
     223,   255,   256,   234,   384,   179,   457,   447,   452,   171,
     212,    34,    69,    71,    73,    74,    75,    76,    77,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      91,    92,    93,    96,    97,    98,    99,   115,   116,   167,
     267,   270,   177,   188,   104,   396,   397,   375,   157,   351,
     171,   172,   173,   185,   212,   186,   177,   177,   177,    20,
      21,    37,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     125,   126,   127,   134,   135,   136,   137,   138,   141,   142,
     143,   144,   145,   146,   147,   187,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   201,   202,   208,   209,
      34,    34,   211,   265,   177,   273,   391,    73,    77,    91,
      92,    96,    97,    98,    99,   406,   167,   403,   178,   376,
     259,   149,   167,   373,   374,   255,    18,    24,    30,    40,
      48,    63,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   153,   213,   287,   405,   407,
     408,   411,   416,   444,   458,   448,   453,   167,   167,   167,
     210,    21,   167,   210,   152,   212,   351,   361,   362,   188,
     266,   276,   381,   177,   188,   395,   177,   400,   351,   210,
     211,    42,   185,   188,   191,   372,   192,   192,   192,   211,
     192,   192,   211,   192,   192,   192,   192,   192,   192,   211,
     287,    32,    59,    60,   121,   125,   187,   191,   194,   209,
     215,   415,   189,   410,   365,   368,   167,   134,   211,     6,
      49,   300,   179,   212,   444,     1,     4,     5,     8,     9,
      10,    12,    14,    15,    16,    17,    18,    24,    25,    26,
      27,    28,    30,    37,    38,    41,    43,    44,    47,    50,
      51,    53,    54,    57,    58,    64,    67,    68,    78,   100,
     101,   102,   103,   115,   116,   130,   131,   132,   148,   149,
     150,   151,   152,   154,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   168,   169,   170,   177,   178,
     179,   194,   195,   196,   201,   202,   209,   211,   213,   214,
     225,   227,   238,   239,   242,   245,   246,   248,   250,   251,
     252,   253,   274,   275,   277,   281,   286,   287,   288,   292,
     293,   294,   295,   296,   297,   298,   299,   301,   305,   306,
     313,   316,   319,   324,   327,   328,   330,   331,   332,   334,
     339,   342,   343,   350,   405,   460,   470,   481,   485,   492,
     495,   167,   179,   392,   393,   287,   357,   374,   210,    64,
     168,   281,   343,   167,   167,   416,   124,   134,   186,   371,
     417,   422,   424,   343,   426,   420,   167,   412,   428,   430,
     432,   434,   436,   438,   440,   442,   343,   192,   211,    32,
     191,    32,   191,   209,   215,   210,   343,   209,   215,   416,
     167,   179,   459,   167,   179,   363,   444,   449,   167,   179,
     366,   454,   343,   149,   167,   370,   404,   361,   192,   192,
     343,   249,   192,   289,   407,   460,   211,   192,   192,   211,
     124,   286,   317,   328,   343,   273,   192,   211,    60,   343,
     211,   343,   167,   192,   192,   211,   179,   192,   160,    57,
     343,   211,   273,   192,   211,   192,   192,   211,   192,   192,
     124,   286,   343,   343,   343,   214,   273,   319,   323,   323,
     323,   211,   211,   211,   211,   211,   211,    12,   416,    12,
     416,    12,   343,   480,   490,   192,   343,   192,   224,    12,
     480,   491,    36,   343,   343,   343,   343,   343,    12,   317,
     343,   317,   214,   179,   179,   343,     9,   319,   325,   167,
     211,   179,   179,   179,   179,   179,    65,   302,   264,   129,
     179,    20,    21,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   133,   134,   135,
     136,   137,   138,   141,   142,   143,   144,   145,   146,   147,
     186,   187,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   208,   209,   325,   211,   179,   186,   178,   381,
     178,   209,   264,   358,   212,    42,   179,   371,   286,   343,
     444,   444,   414,   444,   212,   444,   444,   212,   167,   409,
     444,   265,   444,   265,   444,   265,   363,   364,   366,   367,
     212,   419,   278,   317,   210,   210,   343,   179,   178,   188,
     179,   178,   188,   179,   178,   212,   211,    42,   124,   185,
     186,   188,   191,   369,   471,   473,   273,   404,   290,   211,
     287,   314,   486,   317,   286,   317,   185,   105,   106,   107,
     108,   109,   110,   111,   112,   118,   119,   124,   137,   138,
     144,   145,   146,   186,    13,   416,   491,   343,   343,   273,
     186,   307,   309,   343,   311,   188,   160,   343,   488,   317,
     477,   482,   317,   475,   416,   286,   343,   214,   264,   343,
     343,   343,   343,   343,   343,   404,    52,   155,   167,   194,
     209,   211,   343,   461,   463,   464,   479,   484,   404,   211,
     463,   484,   404,   139,   178,   179,   180,   465,   282,   273,
     284,   173,   174,   222,   404,   185,   494,   177,   404,   185,
     494,   494,   149,   154,   192,   287,   333,   273,   247,   326,
     209,   212,   317,   461,   462,   157,   211,   304,   374,   157,
     322,   323,    18,   155,   167,   405,    18,   155,   167,   405,
     130,   131,   132,   274,   329,   343,   329,   343,   329,   343,
     329,   343,   329,   343,   329,   343,   329,   343,   329,   343,
     343,   343,   343,   329,   343,   329,   343,   343,   343,   343,
     167,   329,   343,   343,   155,   167,   343,   343,   343,   405,
     343,   343,   343,   329,   343,   329,   343,   343,   343,   343,
     329,   343,   329,   343,   329,   343,   343,   329,   343,    21,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   126,   127,   155,   167,   208,   209,   340,   343,   212,
     317,   343,   394,   263,     7,   351,   356,   167,   286,   343,
     179,   193,   193,   193,   179,   193,   193,   179,   193,   193,
     266,   193,   266,   193,   266,   179,   193,   179,   193,   280,
     444,   212,   210,   444,   444,   343,   167,   167,   444,   286,
     343,   416,   416,    19,   444,   212,   317,   462,   192,   343,
     444,   416,   494,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   273,   179,   193,   494,   212,   254,   416,   416,   212,
     416,   212,   416,   494,   416,   416,   494,   416,   193,   322,
     212,   212,   212,   212,   212,   212,    19,   323,   211,   134,
     369,   209,   343,   212,   139,   185,   179,   464,   182,   183,
     210,   468,   185,   179,   182,   210,   467,    19,   212,   464,
     178,   181,   466,    19,   343,   178,   480,   280,   280,   343,
      19,   480,   178,   276,    19,   210,   212,   211,   211,   335,
     337,    11,    22,    23,   240,   241,   343,   264,   461,   185,
     212,   212,   167,   303,   303,   124,   134,   186,   191,   320,
     321,   265,   192,   211,   192,   211,   323,   323,   323,   211,
     211,   210,    18,   155,   167,   405,   188,   155,   167,   343,
     211,   211,   155,   167,   343,     1,   210,   212,   179,   178,
     210,    56,    62,   354,    66,   355,   179,   179,   418,   423,
     425,   444,   427,   421,   167,   413,   429,   193,   433,   193,
     437,   193,   363,   441,   366,   443,   179,   193,   212,    42,
     369,   193,   193,   317,   193,   212,   212,   167,   212,   193,
     193,   212,   416,   211,   212,   343,   193,   193,   193,   193,
     212,   193,   193,   212,   193,   322,   265,   211,   317,   343,
     343,   343,   167,   463,   464,   343,   155,   167,   461,   468,
     210,   343,   479,   210,   317,   463,   178,   181,   184,   469,
     210,   317,   193,   193,   175,   317,   178,   317,   343,   343,
     416,   265,   273,   343,    11,   243,   322,   210,   209,   185,
     210,   212,   167,   167,   167,   167,   185,   210,   266,   344,
     343,   346,   343,   212,   317,   343,   192,   211,   343,   211,
     210,   343,   212,   317,   211,   210,   341,   179,    46,   355,
      45,   104,   352,   431,   435,   439,   444,   211,   167,   343,
     472,   474,   273,   291,   179,   193,   315,   487,   193,   491,
     308,   310,   312,   489,   478,   483,   476,   211,   266,   212,
     317,   179,   212,   464,   468,   211,   134,   369,   179,   464,
     210,   179,   283,   285,   179,   179,   212,   212,   193,   266,
     273,   244,   179,   265,   212,   461,   167,   210,   188,   372,
     320,   210,   139,   273,   318,   416,   212,   444,   212,   212,
     212,   348,   343,   343,   212,   212,   343,    32,   353,   352,
     354,   278,   211,   211,   167,   343,   211,   211,   494,   343,
     343,   343,   211,   211,   211,   212,   343,   318,   212,   343,
     134,   369,   468,   343,   343,   343,   343,   469,   480,   343,
     211,   279,   480,   343,   336,   193,   241,    25,   103,   245,
     293,   294,   295,   297,   343,   266,   210,   188,   372,   416,
     371,   124,   343,   193,   193,   444,   212,   212,   212,   359,
     353,   370,   212,   462,   462,   193,   212,   317,   491,   212,
     317,   462,   462,   212,   179,   493,   343,   343,   212,   493,
     493,   278,   493,   493,   333,   338,   124,   124,   343,   273,
     212,   416,   371,   371,   343,   343,   345,   347,   193,   270,
     360,   212,   212,   343,   494,   494,   494,   212,   212,    52,
     210,   134,   369,   178,   178,   212,   178,   210,   333,   343,
     371,   343,   343,   179,   349,   179,   270,   212,   212,   212,
     212,   343,   210,   343,   343,   210,   178,   343,   179,   179,
     273,   179
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
     299,   300,   300,   300,   301,   301,   302,   302,   303,   303,
     304,   304,   304,   304,   305,   305,   307,   308,   306,   309,
     310,   306,   311,   312,   306,   314,   315,   313,   316,   316,
     316,   317,   317,   318,   318,   318,   319,   319,   319,   320,
     320,   320,   320,   321,   321,   322,   322,   323,   323,   324,
     326,   325,   327,   327,   327,   327,   327,   327,   327,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   329,   329,
     329,   329,   330,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   331,
     331,   332,   332,   333,   333,   334,   335,   336,   334,   337,
     338,   334,   339,   339,   339,   339,   340,   341,   339,   342,
     342,   342,   342,   342,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     344,   345,   343,   343,   343,   343,   346,   347,   343,   343,
     343,   348,   349,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   351,   351,   352,   352,   352,   353,
     353,   354,   354,   354,   355,   355,   356,   357,   358,   357,
     359,   357,   360,   357,   357,   361,   361,   362,   362,   363,
     363,   364,   364,   365,   365,   365,   366,   367,   367,   368,
     368,   368,   369,   369,   370,   370,   370,   370,   370,   370,
     371,   371,   371,   372,   372,   373,   373,   373,   373,   373,
     374,   374,   374,   374,   374,   375,   376,   375,   377,   377,
     378,   378,   378,   379,   380,   379,   381,   381,   381,   381,
     382,   382,   382,   384,   383,   385,   385,   386,   387,   386,
     388,   388,   388,   389,   391,   392,   390,   393,   394,   390,
     395,   395,   396,   396,   397,   398,   398,   399,   399,   399,
     400,   400,   402,   403,   401,   404,   404,   404,   404,   404,
     405,   405,   405,   405,   405,   405,   405,   405,   405,   405,
     405,   405,   405,   405,   405,   405,   405,   405,   405,   405,
     405,   405,   405,   405,   405,   405,   405,   406,   406,   406,
     406,   406,   406,   406,   406,   407,   408,   408,   408,   409,
     409,   410,   410,   410,   412,   413,   411,   414,   414,   415,
     415,   416,   416,   416,   416,   416,   416,   417,   418,   416,
     416,   416,   419,   416,   416,   416,   416,   416,   416,   416,
     416,   416,   416,   416,   416,   416,   420,   421,   416,   416,
     422,   423,   416,   424,   425,   416,   426,   427,   416,   416,
     428,   429,   416,   430,   431,   416,   416,   432,   433,   416,
     434,   435,   416,   416,   436,   437,   416,   438,   439,   416,
     440,   441,   416,   442,   443,   416,   444,   444,   444,   446,
     447,   448,   449,   445,   451,   452,   453,   454,   450,   456,
     457,   458,   459,   455,   460,   460,   460,   460,   460,   461,
     461,   461,   461,   461,   461,   461,   461,   462,   463,   463,
     464,   464,   465,   465,   466,   466,   467,   467,   468,   468,
     468,   469,   469,   469,   470,   470,   470,   470,   470,   470,
     471,   472,   470,   473,   474,   470,   475,   476,   470,   477,
     478,   470,   479,   479,   479,   480,   480,   481,   482,   483,
     481,   484,   484,   485,   485,   485,   485,   486,   487,   485,
     485,   488,   489,   485,   490,   490,   491,   491,   492,   492,
     492,   492,   492,   493,   493,   494,   494,   495,   495,   495,
     495,   495
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
       4,     1,     1,     0,     1,     1,     1,     0,     1,     3,
       9,     8,     8,     7,     3,     3,     0,     0,     7,     0,
       0,     7,     0,     0,     7,     0,     0,     6,     5,     8,
      10,     1,     3,     1,     2,     3,     1,     1,     2,     2,
       2,     2,     2,     1,     3,     0,     4,     1,     6,     6,
       0,     7,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     6,
       8,     5,     6,     1,     4,     3,     0,     0,     8,     0,
       0,     9,     3,     4,     5,     6,     0,     0,     5,     3,
       4,     4,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     4,     4,     5,     4,     5,     3,
       4,     1,     1,     2,     4,     4,     7,     8,     3,     5,
       0,     0,     8,     3,     3,     3,     0,     0,     8,     3,
       4,     0,     0,     9,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     2,     4,     1,     4,     4,
       4,     4,     4,     1,     6,     7,     6,     6,     7,     7,
       6,     7,     6,     6,     0,     4,     0,     1,     1,     0,
       1,     0,     1,     1,     0,     1,     5,     0,     0,     4,
       0,     9,     0,    10,     5,     3,     4,     1,     3,     1,
       3,     1,     3,     0,     2,     3,     3,     1,     3,     0,
       2,     3,     1,     1,     1,     2,     3,     5,     3,     3,
       1,     1,     1,     0,     1,     1,     4,     3,     3,     5,
       4,     6,     5,     5,     4,     0,     0,     4,     0,     1,
       0,     1,     1,     6,     0,     6,     0,     2,     3,     5,
       0,     1,     1,     0,     5,     2,     3,     4,     0,     4,
       0,     1,     1,     1,     0,     0,     9,     0,     0,    11,
       0,     2,     0,     1,     3,     1,     1,     0,     1,     1,
       0,     3,     0,     0,     7,     1,     4,     3,     3,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     1,
       3,     0,     2,     3,     0,     0,     6,     1,     3,     3,
       4,     1,     1,     1,     1,     2,     3,     0,     0,     6,
       4,     5,     0,     9,     4,     2,     2,     3,     2,     3,
       2,     2,     3,     3,     3,     2,     0,     0,     6,     2,
       0,     0,     6,     0,     0,     6,     0,     0,     6,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       0,     0,     6,     0,     0,     6,     1,     3,     3,     0,
       0,     0,     0,    10,     0,     0,     0,     0,    10,     0,
       0,     0,     0,    10,     1,     1,     1,     1,     1,     3,
       3,     5,     5,     6,     6,     8,     8,     1,     1,     3,
       0,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       1,     2,     1,     1,     5,     4,     6,     7,     5,     7,
       0,     0,     9,     0,     0,     9,     0,     0,     9,     0,
       0,     6,     1,     3,     3,     3,     1,     5,     0,     0,
       9,     1,     3,     4,     4,     4,     5,     0,     0,    10,
       5,     0,     0,    10,     1,     3,     1,     3,     4,     3,
       5,     8,    10,     0,     3,     0,     1,     9,    10,    10,
       9,    10
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

    case YYSYMBOL_make_struct_single: /* make_struct_single  */
            { delete ((*yyvaluep).pExpression); }
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

  case 275: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
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

  case 291: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 292: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 293: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 294: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 295: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 296: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 297: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 298: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 299: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 300: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 301: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 302: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 303: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 304: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 305: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 306: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 307: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 308: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 309: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 310: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 311: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 312: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 313: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 314: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 315: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 316: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 317: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 318: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 319: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 320: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" "end of expression" "name" '>' expr ')'  */
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

  case 321: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 322: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 323: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 324: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 325: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 326: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 327: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 328: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 329: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 330: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 331: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 332: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 333: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 334: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 335: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 336: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 337: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 338: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 339: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 340: /* $@21: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 341: /* expr_full_block_assumed_piped: block_or_lambda $@21 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 342: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 343: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 344: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 345: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 346: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 347: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 348: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 349: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 350: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 369: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 370: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 371: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 372: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 390: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 391: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 392: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 393: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 394: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 395: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 396: /* $@22: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 397: /* $@23: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 398: /* func_addr_expr: '@' '@' '<' $@22 type_declaration_no_options '>' $@23 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 399: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 400: /* $@25: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 401: /* func_addr_expr: '@' '@' '<' $@24 optional_function_argument_list optional_function_type '>' $@25 func_addr_name  */
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

  case 402: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 403: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 404: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 405: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 406: /* $@26: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 407: /* $@27: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 408: /* expr_field: expr '.' $@26 error $@27  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 409: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 410: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 411: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 412: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 413: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 414: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 415: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 416: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 417: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 418: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 419: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 420: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 421: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 422: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 423: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 424: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 425: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 426: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 427: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 428: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 429: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 430: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 431: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 432: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 433: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 435: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 436: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 439: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 440: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 441: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 442: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 443: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 444: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 445: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 447: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 448: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 449: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 450: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 451: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 452: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 453: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 454: /* expr: '(' expr_list optional_comma ')'  */
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

  case 455: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 456: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 457: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 458: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 459: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 460: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 461: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 462: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 463: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 464: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 465: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 466: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 467: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 468: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 469: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 470: /* $@28: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 471: /* $@29: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 472: /* expr: expr "is" "type" '<' $@28 type_declaration_no_options '>' $@29  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 473: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 474: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 475: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 476: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 477: /* $@31: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 478: /* expr: expr "as" "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 479: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 480: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 481: /* $@32: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 482: /* $@33: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 483: /* expr: expr '?' "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 484: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 485: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 486: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 487: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 488: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 489: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 490: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 491: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 492: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 493: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 494: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 495: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 496: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 497: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 498: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 499: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 500: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 501: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 502: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 503: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 504: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 505: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 506: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 507: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 508: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 509: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 510: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 511: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 512: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 513: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 514: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 515: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 516: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 517: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 518: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 519: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 520: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 521: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 522: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 523: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 524: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 525: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 526: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 527: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 528: /* $@34: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 529: /* struct_variable_declaration_list: struct_variable_declaration_list $@34 structure_variable_declaration "end of expression"  */
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

  case 530: /* $@35: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 531: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@35 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 532: /* $@36: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 533: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@36 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 534: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' "end of expression"  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 535: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
                                                                                                  {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            if ( (yyvsp[-1].b) ) {
                (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            } else {
                (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            }
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 536: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 537: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 538: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 539: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 540: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 541: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 542: /* tuple_type_list: tuple_type_list "end of expression" tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 543: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 544: /* tuple_alias_type_list: tuple_alias_type_list "end of expression"  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 545: /* tuple_alias_type_list: tuple_alias_type_list tuple_type "end of expression"  */
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

  case 546: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 547: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 548: /* variant_type_list: variant_type_list "end of expression" variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 549: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 550: /* variant_alias_type_list: variant_alias_type_list "end of expression"  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 551: /* variant_alias_type_list: variant_alias_type_list variant_type "end of expression"  */
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

  case 552: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 553: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 554: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 555: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 556: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 557: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 558: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 559: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 560: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 561: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 562: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 563: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 564: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 565: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 566: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 567: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 568: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 569: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 570: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 571: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 572: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 573: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 574: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 575: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 576: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 577: /* global_variable_declaration_list: global_variable_declaration_list $@37 optional_field_annotation let_variable_declaration  */
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

  case 578: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 579: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 580: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 581: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 582: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 583: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 584: /* $@38: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 585: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@38 optional_field_annotation let_variable_declaration  */
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

  case 586: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 587: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 588: /* enum_list: enum_list "name" "end of expression"  */
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

  case 589: /* enum_list: enum_list "name" '=' expr "end of expression"  */
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

  case 590: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 591: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 592: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 593: /* $@39: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 594: /* single_alias: optional_public_or_private_alias "name" $@39 '=' type_declaration  */
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

  case 598: /* $@40: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 600: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 601: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 602: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 603: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 604: /* $@41: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 605: /* $@42: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 606: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@41 enum_list $@42 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 607: /* $@43: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 608: /* $@44: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 609: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@43 enum_list $@44 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 610: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 611: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 612: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 613: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 614: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 615: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 616: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 617: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 618: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 619: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 620: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 621: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 622: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 623: /* $@46: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 624: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
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

  case 625: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 626: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 627: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 628: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 629: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 630: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 631: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 632: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 633: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 634: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 635: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 636: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 637: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 638: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 639: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 640: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 641: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 642: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 643: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 644: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 645: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 646: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 647: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 648: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 649: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 650: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 651: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 652: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 653: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 654: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 655: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 656: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 657: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 658: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 659: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 660: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 661: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 662: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 663: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 664: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 665: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 666: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 667: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 668: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 669: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 670: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 671: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 672: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 673: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 674: /* $@47: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 675: /* $@48: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 676: /* bitfield_type_declaration: "bitfield" '<' $@47 bitfield_bits '>' $@48  */
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

  case 677: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 678: /* table_type_pair: type_declaration "end of expression" type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 679: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 680: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 681: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 682: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 683: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 684: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 685: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 686: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 687: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 688: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 689: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 690: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 691: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 692: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 693: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 694: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 695: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 696: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 697: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 698: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 699: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 700: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 701: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 702: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 703: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 704: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 705: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 706: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 707: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 708: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 709: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 710: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 711: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 712: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 713: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 714: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 715: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 716: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 717: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 718: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 719: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 720: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 721: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 722: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 723: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 724: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 725: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 726: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 727: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 728: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 729: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 730: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 731: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 732: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 733: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 734: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 735: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 736: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 737: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 738: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 739: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 740: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 741: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 742: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 743: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 744: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 745: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 746: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 747: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 748: /* type_declaration: type_declaration '|' '#'  */
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

  case 749: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 750: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 751: /* $@78: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 752: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 753: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 "begin of code block" $@78 tuple_alias_type_list $@79 "end of code block"  */
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

  case 754: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 755: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 756: /* $@82: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 757: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 758: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 "begin of code block" $@82 variant_alias_type_list $@83 "end of code block"  */
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

  case 759: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 760: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 761: /* $@86: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 762: /* $@87: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 763: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 "begin of code block" $@86 bitfield_alias_bits $@87 "end of code block"  */
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

  case 764: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 765: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 766: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 767: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 768: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 769: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 770: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 771: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 772: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 773: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 774: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 775: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 776: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 777: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 778: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 779: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 780: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 781: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 794: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 795: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 796: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 797: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 798: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 799: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 800: /* $@88: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 801: /* $@89: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 802: /* make_struct_decl: "struct" '<' $@88 type_declaration_no_options '>' $@89 '(' make_struct_single ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 803: /* $@90: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 804: /* $@91: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 805: /* make_struct_decl: "class" '<' $@90 type_declaration_no_options '>' $@91 '(' make_struct_single ')'  */
                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 806: /* $@92: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 807: /* $@93: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 808: /* make_struct_decl: "variant" '<' $@92 type_declaration_no_options '>' $@93 '(' make_struct_single ')'  */
                                                                                                                                                                                                             {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 809: /* $@94: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 810: /* $@95: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 811: /* make_struct_decl: "default" '<' $@94 type_declaration_no_options '>' $@95  */
                                                                                                                                      {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-5]));
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->useInitializer = true;
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 812: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 813: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 814: /* make_tuple: make_tuple ',' expr  */
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

  case 815: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 816: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 817: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 818: /* $@96: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 819: /* $@97: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 820: /* make_tuple_call: "tuple" '<' $@96 type_declaration_no_options '>' $@97 '(' make_struct_single ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 821: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 822: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 823: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 824: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 825: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 826: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 827: /* $@98: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 828: /* $@99: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 829: /* make_dim_decl: "array" '<' $@98 type_declaration_no_options '>' $@99 '(' expr_list optional_comma ')'  */
                                                                                                                                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 830: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 831: /* $@100: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 832: /* $@101: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 833: /* make_dim_decl: "fixed_array" '<' $@100 type_declaration_no_options '>' $@101 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 834: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 835: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 836: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 837: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 838: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 839: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 840: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 841: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-7])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 842: /* make_table_decl: "table" '<' type_declaration_no_options "end of expression" type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
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

  case 843: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 844: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 845: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 846: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 847: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 848: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 849: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 850: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 851: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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


