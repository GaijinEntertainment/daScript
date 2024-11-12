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
  YYSYMBOL_DAS_CAPTURE = 4,                /* "capture"  */
  YYSYMBOL_DAS_STRUCT = 5,                 /* "struct"  */
  YYSYMBOL_DAS_CLASS = 6,                  /* "class"  */
  YYSYMBOL_DAS_LET = 7,                    /* "let"  */
  YYSYMBOL_DAS_DEF = 8,                    /* "def"  */
  YYSYMBOL_DAS_WHILE = 9,                  /* "while"  */
  YYSYMBOL_DAS_IF = 10,                    /* "if"  */
  YYSYMBOL_DAS_STATIC_IF = 11,             /* "static_if"  */
  YYSYMBOL_DAS_ELSE = 12,                  /* "else"  */
  YYSYMBOL_DAS_FOR = 13,                   /* "for"  */
  YYSYMBOL_DAS_CATCH = 14,                 /* "recover"  */
  YYSYMBOL_DAS_TRUE = 15,                  /* "true"  */
  YYSYMBOL_DAS_FALSE = 16,                 /* "false"  */
  YYSYMBOL_DAS_NEWT = 17,                  /* "new"  */
  YYSYMBOL_DAS_TYPEINFO = 18,              /* "typeinfo"  */
  YYSYMBOL_DAS_TYPE = 19,                  /* "type"  */
  YYSYMBOL_DAS_IN = 20,                    /* "in"  */
  YYSYMBOL_DAS_IS = 21,                    /* "is"  */
  YYSYMBOL_DAS_AS = 22,                    /* "as"  */
  YYSYMBOL_DAS_ELIF = 23,                  /* "elif"  */
  YYSYMBOL_DAS_STATIC_ELIF = 24,           /* "static_elif"  */
  YYSYMBOL_DAS_ARRAY = 25,                 /* "array"  */
  YYSYMBOL_DAS_RETURN = 26,                /* "return"  */
  YYSYMBOL_DAS_NULL = 27,                  /* "null"  */
  YYSYMBOL_DAS_BREAK = 28,                 /* "break"  */
  YYSYMBOL_DAS_TRY = 29,                   /* "try"  */
  YYSYMBOL_DAS_OPTIONS = 30,               /* "options"  */
  YYSYMBOL_DAS_TABLE = 31,                 /* "table"  */
  YYSYMBOL_DAS_EXPECT = 32,                /* "expect"  */
  YYSYMBOL_DAS_CONST = 33,                 /* "const"  */
  YYSYMBOL_DAS_REQUIRE = 34,               /* "require"  */
  YYSYMBOL_DAS_OPERATOR = 35,              /* "operator"  */
  YYSYMBOL_DAS_ENUM = 36,                  /* "enum"  */
  YYSYMBOL_DAS_FINALLY = 37,               /* "finally"  */
  YYSYMBOL_DAS_DELETE = 38,                /* "delete"  */
  YYSYMBOL_DAS_DEREF = 39,                 /* "deref"  */
  YYSYMBOL_DAS_TYPEDEF = 40,               /* "typedef"  */
  YYSYMBOL_DAS_TYPEDECL = 41,              /* "typedecl"  */
  YYSYMBOL_DAS_WITH = 42,                  /* "with"  */
  YYSYMBOL_DAS_AKA = 43,                   /* "aka"  */
  YYSYMBOL_DAS_ASSUME = 44,                /* "assume"  */
  YYSYMBOL_DAS_CAST = 45,                  /* "cast"  */
  YYSYMBOL_DAS_OVERRIDE = 46,              /* "override"  */
  YYSYMBOL_DAS_ABSTRACT = 47,              /* "abstract"  */
  YYSYMBOL_DAS_UPCAST = 48,                /* "upcast"  */
  YYSYMBOL_DAS_ITERATOR = 49,              /* "iterator"  */
  YYSYMBOL_DAS_VAR = 50,                   /* "var"  */
  YYSYMBOL_DAS_ADDR = 51,                  /* "addr"  */
  YYSYMBOL_DAS_CONTINUE = 52,              /* "continue"  */
  YYSYMBOL_DAS_WHERE = 53,                 /* "where"  */
  YYSYMBOL_DAS_PASS = 54,                  /* "pass"  */
  YYSYMBOL_DAS_REINTERPRET = 55,           /* "reinterpret"  */
  YYSYMBOL_DAS_MODULE = 56,                /* "module"  */
  YYSYMBOL_DAS_PUBLIC = 57,                /* "public"  */
  YYSYMBOL_DAS_LABEL = 58,                 /* "label"  */
  YYSYMBOL_DAS_GOTO = 59,                  /* "goto"  */
  YYSYMBOL_DAS_IMPLICIT = 60,              /* "implicit"  */
  YYSYMBOL_DAS_EXPLICIT = 61,              /* "explicit"  */
  YYSYMBOL_DAS_SHARED = 62,                /* "shared"  */
  YYSYMBOL_DAS_PRIVATE = 63,               /* "private"  */
  YYSYMBOL_DAS_SMART_PTR = 64,             /* "smart_ptr"  */
  YYSYMBOL_DAS_UNSAFE = 65,                /* "unsafe"  */
  YYSYMBOL_DAS_INSCOPE = 66,               /* "inscope"  */
  YYSYMBOL_DAS_STATIC = 67,                /* "static"  */
  YYSYMBOL_DAS_FIXED_ARRAY = 68,           /* "fixed_array"  */
  YYSYMBOL_DAS_DEFAULT = 69,               /* "default"  */
  YYSYMBOL_DAS_UNINITIALIZED = 70,         /* "uninitialized"  */
  YYSYMBOL_DAS_TBOOL = 71,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 72,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 73,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 74,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 75,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 76,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 77,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 78,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 79,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 80,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 81,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 82,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 83,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 84,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 85,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 86,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 87,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 88,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 89,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 90,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 91,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 92,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 93,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 94,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 95,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 96,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 97,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 98,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 99,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 100,               /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 101,              /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 102,               /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 103,             /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 104,            /* "generator"  */
  YYSYMBOL_DAS_YIELD = 105,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 106,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 107,                   /* "+="  */
  YYSYMBOL_SUBEQU = 108,                   /* "-="  */
  YYSYMBOL_DIVEQU = 109,                   /* "/="  */
  YYSYMBOL_MULEQU = 110,                   /* "*="  */
  YYSYMBOL_MODEQU = 111,                   /* "%="  */
  YYSYMBOL_ANDEQU = 112,                   /* "&="  */
  YYSYMBOL_OREQU = 113,                    /* "|="  */
  YYSYMBOL_XOREQU = 114,                   /* "^="  */
  YYSYMBOL_SHL = 115,                      /* "<<"  */
  YYSYMBOL_SHR = 116,                      /* ">>"  */
  YYSYMBOL_ADDADD = 117,                   /* "++"  */
  YYSYMBOL_SUBSUB = 118,                   /* "--"  */
  YYSYMBOL_LEEQU = 119,                    /* "<="  */
  YYSYMBOL_SHLEQU = 120,                   /* "<<="  */
  YYSYMBOL_SHREQU = 121,                   /* ">>="  */
  YYSYMBOL_GREQU = 122,                    /* ">="  */
  YYSYMBOL_EQUEQU = 123,                   /* "=="  */
  YYSYMBOL_NOTEQU = 124,                   /* "!="  */
  YYSYMBOL_RARROW = 125,                   /* "->"  */
  YYSYMBOL_LARROW = 126,                   /* "<-"  */
  YYSYMBOL_QQ = 127,                       /* "??"  */
  YYSYMBOL_QDOT = 128,                     /* "?."  */
  YYSYMBOL_QBRA = 129,                     /* "?["  */
  YYSYMBOL_LPIPE = 130,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 131,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 132,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 133,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 134,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 135,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 136,                 /* ":="  */
  YYSYMBOL_ROTL = 137,                     /* "<<<"  */
  YYSYMBOL_ROTR = 138,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 139,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 140,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 141,                    /* "=>"  */
  YYSYMBOL_COLCOL = 142,                   /* "::"  */
  YYSYMBOL_ANDAND = 143,                   /* "&&"  */
  YYSYMBOL_OROR = 144,                     /* "||"  */
  YYSYMBOL_XORXOR = 145,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 146,                /* "&&="  */
  YYSYMBOL_OROREQU = 147,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 148,                /* "^^="  */
  YYSYMBOL_DOTDOT = 149,                   /* ".."  */
  YYSYMBOL_MTAG_E = 150,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 151,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 152,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 153,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 154,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 155,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 156,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 157,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 158,           /* "..."  */
  YYSYMBOL_BRABRAB = 159,                  /* "[["  */
  YYSYMBOL_BRACBRB = 160,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 161,                  /* "{{"  */
  YYSYMBOL_INTEGER = 162,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 163,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 164,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 165,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 166,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 167,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 168,                   /* "double constant"  */
  YYSYMBOL_NAME = 169,                     /* "name"  */
  YYSYMBOL_KEYWORD = 170,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 171,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 172,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 173,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 174,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 175,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 176,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 177,          /* "}"  */
  YYSYMBOL_END_OF_READ = 178,              /* "end of failed eader macro"  */
  YYSYMBOL_179_begin_of_code_block_ = 179, /* "begin of code block"  */
  YYSYMBOL_180_end_of_code_block_ = 180,   /* "end of code block"  */
  YYSYMBOL_181_end_of_expression_ = 181,   /* "end of expression"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 182,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 183,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 184,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 185,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 186,            /* ",}]"  */
  YYSYMBOL_187_ = 187,                     /* ','  */
  YYSYMBOL_188_ = 188,                     /* '='  */
  YYSYMBOL_189_ = 189,                     /* '?'  */
  YYSYMBOL_190_ = 190,                     /* ':'  */
  YYSYMBOL_191_ = 191,                     /* '|'  */
  YYSYMBOL_192_ = 192,                     /* '^'  */
  YYSYMBOL_193_ = 193,                     /* '&'  */
  YYSYMBOL_194_ = 194,                     /* '<'  */
  YYSYMBOL_195_ = 195,                     /* '>'  */
  YYSYMBOL_196_ = 196,                     /* '-'  */
  YYSYMBOL_197_ = 197,                     /* '+'  */
  YYSYMBOL_198_ = 198,                     /* '*'  */
  YYSYMBOL_199_ = 199,                     /* '/'  */
  YYSYMBOL_200_ = 200,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 201,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 202,               /* UNARY_PLUS  */
  YYSYMBOL_203_ = 203,                     /* '~'  */
  YYSYMBOL_204_ = 204,                     /* '!'  */
  YYSYMBOL_PRE_INC = 205,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 206,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 207,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 208,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 209,                    /* DEREF  */
  YYSYMBOL_210_ = 210,                     /* '.'  */
  YYSYMBOL_211_ = 211,                     /* '['  */
  YYSYMBOL_212_ = 212,                     /* ']'  */
  YYSYMBOL_213_ = 213,                     /* '('  */
  YYSYMBOL_214_ = 214,                     /* ')'  */
  YYSYMBOL_215_ = 215,                     /* '$'  */
  YYSYMBOL_216_ = 216,                     /* '@'  */
  YYSYMBOL_217_ = 217,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 218,                 /* $accept  */
  YYSYMBOL_program = 219,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 220,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 221, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 222,              /* module_name  */
  YYSYMBOL_module_declaration = 223,       /* module_declaration  */
  YYSYMBOL_character_sequence = 224,       /* character_sequence  */
  YYSYMBOL_string_constant = 225,          /* string_constant  */
  YYSYMBOL_string_builder_body = 226,      /* string_builder_body  */
  YYSYMBOL_string_builder = 227,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 228, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 229,              /* expr_reader  */
  YYSYMBOL_230_1 = 230,                    /* $@1  */
  YYSYMBOL_options_declaration = 231,      /* options_declaration  */
  YYSYMBOL_require_declaration = 232,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 233,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 234,      /* require_module_name  */
  YYSYMBOL_require_module = 235,           /* require_module  */
  YYSYMBOL_is_public_module = 236,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 237,       /* expect_declaration  */
  YYSYMBOL_expect_list = 238,              /* expect_list  */
  YYSYMBOL_expect_error = 239,             /* expect_error  */
  YYSYMBOL_expression_label = 240,         /* expression_label  */
  YYSYMBOL_expression_goto = 241,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 242,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 243,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 244,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 245, /* expression_else_one_liner  */
  YYSYMBOL_246_2 = 246,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 247,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 248,  /* expression_if_then_else  */
  YYSYMBOL_249_3 = 249,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 250,      /* expression_for_loop  */
  YYSYMBOL_251_4 = 251,                    /* $@4  */
  YYSYMBOL_expression_unsafe = 252,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 253,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 254,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 255,    /* expression_with_alias  */
  YYSYMBOL_256_5 = 256,                    /* $@5  */
  YYSYMBOL_annotation_argument_value = 257, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 258, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 259, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 260,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 261, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 262,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 263, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 264, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 265,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 266,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 267, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 268, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 269,   /* optional_function_type  */
  YYSYMBOL_function_name = 270,            /* function_name  */
  YYSYMBOL_global_function_declaration = 271, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 272, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 273, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 274,     /* function_declaration  */
  YYSYMBOL_275_6 = 275,                    /* $@6  */
  YYSYMBOL_expression_block = 276,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 277,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 278,           /* expression_any  */
  YYSYMBOL_expressions = 279,              /* expressions  */
  YYSYMBOL_expr_keyword = 280,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 281,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 282, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 283, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 284, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 285,       /* expression_keyword  */
  YYSYMBOL_286_7 = 286,                    /* $@7  */
  YYSYMBOL_287_8 = 287,                    /* $@8  */
  YYSYMBOL_288_9 = 288,                    /* $@9  */
  YYSYMBOL_289_10 = 289,                   /* $@10  */
  YYSYMBOL_expr_pipe = 290,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 291,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 292,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 293,     /* new_type_declaration  */
  YYSYMBOL_294_11 = 294,                   /* $@11  */
  YYSYMBOL_295_12 = 295,                   /* $@12  */
  YYSYMBOL_expr_new = 296,                 /* expr_new  */
  YYSYMBOL_expression_break = 297,         /* expression_break  */
  YYSYMBOL_expression_continue = 298,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 299, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 300,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 301, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 302,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 303,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 304,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 305,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 306,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 307,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 308, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 309,           /* expression_let  */
  YYSYMBOL_expr_cast = 310,                /* expr_cast  */
  YYSYMBOL_311_13 = 311,                   /* $@13  */
  YYSYMBOL_312_14 = 312,                   /* $@14  */
  YYSYMBOL_313_15 = 313,                   /* $@15  */
  YYSYMBOL_314_16 = 314,                   /* $@16  */
  YYSYMBOL_315_17 = 315,                   /* $@17  */
  YYSYMBOL_316_18 = 316,                   /* $@18  */
  YYSYMBOL_expr_type_decl = 317,           /* expr_type_decl  */
  YYSYMBOL_318_19 = 318,                   /* $@19  */
  YYSYMBOL_319_20 = 319,                   /* $@20  */
  YYSYMBOL_expr_type_info = 320,           /* expr_type_info  */
  YYSYMBOL_expr_list = 321,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 322,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 323,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 324,            /* capture_entry  */
  YYSYMBOL_capture_list = 325,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 326,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 327,               /* expr_block  */
  YYSYMBOL_expr_full_block = 328,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 329, /* expr_full_block_assumed_piped  */
  YYSYMBOL_330_21 = 330,                   /* $@21  */
  YYSYMBOL_expr_numeric_const = 331,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 332,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 333,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 334,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 335,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 336,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 337,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 338,           /* func_addr_expr  */
  YYSYMBOL_339_22 = 339,                   /* $@22  */
  YYSYMBOL_340_23 = 340,                   /* $@23  */
  YYSYMBOL_341_24 = 341,                   /* $@24  */
  YYSYMBOL_342_25 = 342,                   /* $@25  */
  YYSYMBOL_expr_field = 343,               /* expr_field  */
  YYSYMBOL_344_26 = 344,                   /* $@26  */
  YYSYMBOL_345_27 = 345,                   /* $@27  */
  YYSYMBOL_expr_call = 346,                /* expr_call  */
  YYSYMBOL_expr = 347,                     /* expr  */
  YYSYMBOL_348_28 = 348,                   /* $@28  */
  YYSYMBOL_349_29 = 349,                   /* $@29  */
  YYSYMBOL_350_30 = 350,                   /* $@30  */
  YYSYMBOL_351_31 = 351,                   /* $@31  */
  YYSYMBOL_352_32 = 352,                   /* $@32  */
  YYSYMBOL_353_33 = 353,                   /* $@33  */
  YYSYMBOL_expr_mtag = 354,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 355, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 356,        /* optional_override  */
  YYSYMBOL_optional_constant = 357,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 358, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 359, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 360, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 361, /* struct_variable_declaration_list  */
  YYSYMBOL_362_34 = 362,                   /* $@34  */
  YYSYMBOL_363_35 = 363,                   /* $@35  */
  YYSYMBOL_364_36 = 364,                   /* $@36  */
  YYSYMBOL_function_argument_declaration = 365, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 366,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 367,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 368,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 369,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 370,             /* variant_type  */
  YYSYMBOL_variant_type_list = 371,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 372,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 373,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 374,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 375,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 376,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 377, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 378, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 379, /* global_variable_declaration_list  */
  YYSYMBOL_380_37 = 380,                   /* $@37  */
  YYSYMBOL_optional_shared = 381,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 382, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 383,               /* global_let  */
  YYSYMBOL_384_38 = 384,                   /* $@38  */
  YYSYMBOL_enum_list = 385,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 386, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 387,             /* single_alias  */
  YYSYMBOL_388_39 = 388,                   /* $@39  */
  YYSYMBOL_alias_list = 389,               /* alias_list  */
  YYSYMBOL_alias_declaration = 390,        /* alias_declaration  */
  YYSYMBOL_391_40 = 391,                   /* $@40  */
  YYSYMBOL_optional_public_or_private_enum = 392, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 393,                /* enum_name  */
  YYSYMBOL_enum_declaration = 394,         /* enum_declaration  */
  YYSYMBOL_395_41 = 395,                   /* $@41  */
  YYSYMBOL_396_42 = 396,                   /* $@42  */
  YYSYMBOL_397_43 = 397,                   /* $@43  */
  YYSYMBOL_398_44 = 398,                   /* $@44  */
  YYSYMBOL_optional_structure_parent = 399, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 400,          /* optional_sealed  */
  YYSYMBOL_structure_name = 401,           /* structure_name  */
  YYSYMBOL_class_or_struct = 402,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 403, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 404, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 405,    /* structure_declaration  */
  YYSYMBOL_406_45 = 406,                   /* $@45  */
  YYSYMBOL_407_46 = 407,                   /* $@46  */
  YYSYMBOL_variable_name_with_pos_list = 408, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 409,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 410, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 411, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 412,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 413,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 414,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 415, /* bitfield_type_declaration  */
  YYSYMBOL_416_47 = 416,                   /* $@47  */
  YYSYMBOL_417_48 = 417,                   /* $@48  */
  YYSYMBOL_c_or_s = 418,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 419,          /* table_type_pair  */
  YYSYMBOL_dim_list = 420,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 421, /* type_declaration_no_options  */
  YYSYMBOL_422_49 = 422,                   /* $@49  */
  YYSYMBOL_423_50 = 423,                   /* $@50  */
  YYSYMBOL_424_51 = 424,                   /* $@51  */
  YYSYMBOL_425_52 = 425,                   /* $@52  */
  YYSYMBOL_426_53 = 426,                   /* $@53  */
  YYSYMBOL_427_54 = 427,                   /* $@54  */
  YYSYMBOL_428_55 = 428,                   /* $@55  */
  YYSYMBOL_429_56 = 429,                   /* $@56  */
  YYSYMBOL_430_57 = 430,                   /* $@57  */
  YYSYMBOL_431_58 = 431,                   /* $@58  */
  YYSYMBOL_432_59 = 432,                   /* $@59  */
  YYSYMBOL_433_60 = 433,                   /* $@60  */
  YYSYMBOL_434_61 = 434,                   /* $@61  */
  YYSYMBOL_435_62 = 435,                   /* $@62  */
  YYSYMBOL_436_63 = 436,                   /* $@63  */
  YYSYMBOL_437_64 = 437,                   /* $@64  */
  YYSYMBOL_438_65 = 438,                   /* $@65  */
  YYSYMBOL_439_66 = 439,                   /* $@66  */
  YYSYMBOL_440_67 = 440,                   /* $@67  */
  YYSYMBOL_441_68 = 441,                   /* $@68  */
  YYSYMBOL_442_69 = 442,                   /* $@69  */
  YYSYMBOL_443_70 = 443,                   /* $@70  */
  YYSYMBOL_444_71 = 444,                   /* $@71  */
  YYSYMBOL_445_72 = 445,                   /* $@72  */
  YYSYMBOL_446_73 = 446,                   /* $@73  */
  YYSYMBOL_447_74 = 447,                   /* $@74  */
  YYSYMBOL_448_75 = 448,                   /* $@75  */
  YYSYMBOL_type_declaration = 449,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 450,  /* tuple_alias_declaration  */
  YYSYMBOL_451_76 = 451,                   /* $@76  */
  YYSYMBOL_452_77 = 452,                   /* $@77  */
  YYSYMBOL_453_78 = 453,                   /* $@78  */
  YYSYMBOL_454_79 = 454,                   /* $@79  */
  YYSYMBOL_variant_alias_declaration = 455, /* variant_alias_declaration  */
  YYSYMBOL_456_80 = 456,                   /* $@80  */
  YYSYMBOL_457_81 = 457,                   /* $@81  */
  YYSYMBOL_458_82 = 458,                   /* $@82  */
  YYSYMBOL_459_83 = 459,                   /* $@83  */
  YYSYMBOL_bitfield_alias_declaration = 460, /* bitfield_alias_declaration  */
  YYSYMBOL_461_84 = 461,                   /* $@84  */
  YYSYMBOL_462_85 = 462,                   /* $@85  */
  YYSYMBOL_463_86 = 463,                   /* $@86  */
  YYSYMBOL_464_87 = 464,                   /* $@87  */
  YYSYMBOL_make_decl = 465,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 466,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 467,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 468,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 469,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 470,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 471,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 472, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 473,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 474, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 475, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 476, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 477, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 478, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 479,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 480,         /* make_struct_decl  */
  YYSYMBOL_481_88 = 481,                   /* $@88  */
  YYSYMBOL_482_89 = 482,                   /* $@89  */
  YYSYMBOL_483_90 = 483,                   /* $@90  */
  YYSYMBOL_484_91 = 484,                   /* $@91  */
  YYSYMBOL_485_92 = 485,                   /* $@92  */
  YYSYMBOL_486_93 = 486,                   /* $@93  */
  YYSYMBOL_487_94 = 487,                   /* $@94  */
  YYSYMBOL_488_95 = 488,                   /* $@95  */
  YYSYMBOL_make_tuple = 489,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 490,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 491,          /* make_tuple_call  */
  YYSYMBOL_492_96 = 492,                   /* $@96  */
  YYSYMBOL_493_97 = 493,                   /* $@97  */
  YYSYMBOL_make_dim = 494,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 495,            /* make_dim_decl  */
  YYSYMBOL_496_98 = 496,                   /* $@98  */
  YYSYMBOL_497_99 = 497,                   /* $@99  */
  YYSYMBOL_498_100 = 498,                  /* $@100  */
  YYSYMBOL_499_101 = 499,                  /* $@101  */
  YYSYMBOL_500_102 = 500,                  /* $@102  */
  YYSYMBOL_501_103 = 501,                  /* $@103  */
  YYSYMBOL_502_104 = 502,                  /* $@104  */
  YYSYMBOL_503_105 = 503,                  /* $@105  */
  YYSYMBOL_504_106 = 504,                  /* $@106  */
  YYSYMBOL_505_107 = 505,                  /* $@107  */
  YYSYMBOL_make_table = 506,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 507,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 508,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 509, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 510,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 511       /* array_comprehension  */
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
#define YYLAST   14784

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  218
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  294
/* YYNRULES -- Number of rules.  */
#define YYNRULES  887
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1684

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   445


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
       2,     2,     2,   204,     2,   217,   215,   200,   193,     2,
     213,   214,   198,   197,   187,   196,   210,   199,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   190,   181,
     194,   188,   195,   189,   216,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   211,     2,   212,   192,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   179,   191,   180,   203,     2,     2,     2,
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
     175,   176,   177,   178,   182,   183,   184,   185,   186,   201,
     202,   205,   206,   207,   208,   209
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   543,   543,   544,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   565,   571,   572,
     573,   577,   578,   582,   600,   601,   602,   603,   607,   611,
     616,   625,   633,   649,   654,   662,   662,   701,   731,   735,
     736,   737,   741,   744,   748,   754,   763,   766,   772,   773,
     777,   781,   782,   786,   789,   795,   801,   804,   810,   811,
     815,   816,   817,   826,   827,   831,   832,   832,   838,   839,
     840,   841,   842,   846,   852,   852,   858,   858,   864,   872,
     882,   891,   891,   898,   899,   900,   901,   902,   903,   907,
     912,   920,   921,   922,   926,   927,   928,   929,   930,   931,
     932,   933,   939,   942,   948,   951,   954,   960,   961,   962,
     966,   979,   997,  1000,  1008,  1019,  1030,  1041,  1044,  1051,
    1055,  1062,  1063,  1067,  1068,  1069,  1073,  1076,  1083,  1087,
    1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,
    1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,
    1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,
    1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,
    1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,
    1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,
    1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,  1156,  1157,
    1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,
    1168,  1169,  1170,  1175,  1193,  1194,  1195,  1199,  1205,  1205,
    1222,  1226,  1237,  1246,  1255,  1261,  1262,  1263,  1264,  1265,
    1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,  1274,  1275,
    1276,  1277,  1278,  1279,  1280,  1281,  1285,  1290,  1296,  1302,
    1313,  1314,  1318,  1319,  1323,  1324,  1328,  1332,  1339,  1339,
    1339,  1345,  1345,  1345,  1354,  1388,  1391,  1394,  1397,  1403,
    1404,  1415,  1419,  1422,  1430,  1430,  1430,  1433,  1439,  1442,
    1446,  1450,  1457,  1464,  1470,  1474,  1478,  1481,  1484,  1492,
    1495,  1498,  1506,  1509,  1517,  1520,  1523,  1531,  1537,  1538,
    1539,  1543,  1544,  1548,  1549,  1553,  1558,  1566,  1572,  1578,
    1587,  1599,  1602,  1608,  1608,  1608,  1611,  1611,  1611,  1616,
    1616,  1616,  1624,  1624,  1624,  1630,  1640,  1651,  1664,  1674,
    1685,  1700,  1703,  1709,  1710,  1717,  1728,  1729,  1730,  1734,
    1735,  1736,  1737,  1738,  1742,  1747,  1755,  1756,  1757,  1761,
    1766,  1773,  1780,  1780,  1789,  1790,  1791,  1792,  1793,  1794,
    1795,  1799,  1800,  1801,  1802,  1803,  1804,  1805,  1806,  1807,
    1808,  1809,  1810,  1811,  1812,  1813,  1814,  1815,  1816,  1817,
    1821,  1822,  1823,  1824,  1829,  1830,  1831,  1832,  1833,  1834,
    1835,  1836,  1837,  1838,  1839,  1840,  1841,  1842,  1843,  1844,
    1845,  1850,  1857,  1869,  1874,  1884,  1888,  1895,  1898,  1898,
    1898,  1903,  1903,  1903,  1916,  1920,  1924,  1929,  1936,  1936,
    1936,  1943,  1947,  1957,  1966,  1975,  1979,  1982,  1988,  1989,
    1990,  1991,  1992,  1993,  1994,  1995,  1996,  1997,  1998,  1999,
    2000,  2001,  2002,  2003,  2004,  2005,  2006,  2007,  2008,  2009,
    2010,  2011,  2012,  2013,  2014,  2015,  2016,  2017,  2018,  2019,
    2020,  2021,  2022,  2023,  2029,  2030,  2031,  2032,  2033,  2046,
    2047,  2048,  2049,  2050,  2051,  2052,  2053,  2054,  2055,  2056,
    2057,  2060,  2063,  2064,  2067,  2067,  2067,  2070,  2075,  2079,
    2083,  2083,  2083,  2088,  2091,  2095,  2095,  2095,  2100,  2103,
    2104,  2105,  2106,  2107,  2108,  2109,  2110,  2111,  2113,  2117,
    2118,  2123,  2127,  2128,  2129,  2130,  2131,  2132,  2133,  2137,
    2141,  2145,  2149,  2153,  2157,  2161,  2165,  2169,  2176,  2177,
    2178,  2182,  2183,  2184,  2188,  2189,  2193,  2194,  2195,  2199,
    2200,  2204,  2215,  2218,  2218,  2237,  2236,  2251,  2250,  2263,
    2272,  2281,  2291,  2292,  2296,  2299,  2308,  2309,  2313,  2316,
    2319,  2335,  2344,  2345,  2349,  2352,  2355,  2369,  2370,  2374,
    2380,  2386,  2389,  2393,  2399,  2408,  2409,  2410,  2414,  2415,
    2419,  2426,  2431,  2440,  2446,  2457,  2460,  2465,  2470,  2478,
    2489,  2492,  2492,  2512,  2513,  2517,  2518,  2519,  2523,  2526,
    2526,  2545,  2548,  2551,  2566,  2585,  2586,  2587,  2592,  2592,
    2618,  2619,  2623,  2624,  2624,  2628,  2629,  2630,  2634,  2644,
    2649,  2644,  2661,  2666,  2661,  2681,  2682,  2686,  2687,  2691,
    2697,  2698,  2702,  2703,  2704,  2708,  2711,  2717,  2722,  2717,
    2736,  2743,  2748,  2757,  2763,  2774,  2775,  2776,  2777,  2778,
    2779,  2780,  2781,  2782,  2783,  2784,  2785,  2786,  2787,  2788,
    2789,  2790,  2791,  2792,  2793,  2794,  2795,  2796,  2797,  2798,
    2799,  2800,  2804,  2805,  2806,  2807,  2808,  2809,  2810,  2811,
    2815,  2826,  2830,  2837,  2849,  2856,  2865,  2870,  2873,  2886,
    2886,  2886,  2899,  2900,  2904,  2908,  2915,  2919,  2926,  2927,
    2928,  2929,  2930,  2945,  2951,  2951,  2951,  2955,  2960,  2967,
    2967,  2974,  2978,  2982,  2987,  2992,  2997,  3002,  3006,  3010,
    3015,  3019,  3023,  3028,  3028,  3028,  3034,  3041,  3041,  3041,
    3046,  3046,  3046,  3052,  3052,  3052,  3057,  3061,  3061,  3061,
    3066,  3066,  3066,  3075,  3079,  3079,  3079,  3084,  3084,  3084,
    3093,  3097,  3097,  3097,  3102,  3102,  3102,  3111,  3111,  3111,
    3117,  3117,  3117,  3126,  3129,  3140,  3156,  3156,  3161,  3166,
    3156,  3191,  3191,  3196,  3202,  3191,  3227,  3227,  3232,  3237,
    3227,  3267,  3268,  3269,  3270,  3271,  3275,  3282,  3289,  3295,
    3301,  3308,  3315,  3321,  3330,  3336,  3344,  3349,  3356,  3361,
    3368,  3373,  3379,  3380,  3384,  3385,  3390,  3391,  3395,  3396,
    3400,  3401,  3405,  3406,  3407,  3411,  3412,  3413,  3417,  3418,
    3422,  3428,  3435,  3443,  3450,  3458,  3467,  3467,  3467,  3475,
    3475,  3475,  3482,  3482,  3482,  3489,  3489,  3489,  3500,  3503,
    3509,  3523,  3529,  3535,  3541,  3541,  3541,  3551,  3556,  3563,
    3571,  3576,  3583,  3583,  3583,  3593,  3593,  3593,  3603,  3603,
    3603,  3613,  3621,  3621,  3621,  3640,  3647,  3647,  3647,  3657,
    3662,  3669,  3672,  3678,  3686,  3695,  3703,  3723,  3748,  3749,
    3753,  3754,  3759,  3762,  3765,  3768,  3771,  3774
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
  "\"capture\"", "\"struct\"", "\"class\"", "\"let\"", "\"def\"",
  "\"while\"", "\"if\"", "\"static_if\"", "\"else\"", "\"for\"",
  "\"recover\"", "\"true\"", "\"false\"", "\"new\"", "\"typeinfo\"",
  "\"type\"", "\"in\"", "\"is\"", "\"as\"", "\"elif\"", "\"static_elif\"",
  "\"array\"", "\"return\"", "\"null\"", "\"break\"", "\"try\"",
  "\"options\"", "\"table\"", "\"expect\"", "\"const\"", "\"require\"",
  "\"operator\"", "\"enum\"", "\"finally\"", "\"delete\"", "\"deref\"",
  "\"typedef\"", "\"typedecl\"", "\"with\"", "\"aka\"", "\"assume\"",
  "\"cast\"", "\"override\"", "\"abstract\"", "\"upcast\"", "\"iterator\"",
  "\"var\"", "\"addr\"", "\"continue\"", "\"where\"", "\"pass\"",
  "\"reinterpret\"", "\"module\"", "\"public\"", "\"label\"", "\"goto\"",
  "\"implicit\"", "\"explicit\"", "\"shared\"", "\"private\"",
  "\"smart_ptr\"", "\"unsafe\"", "\"inscope\"", "\"static\"",
  "\"fixed_array\"", "\"default\"", "\"uninitialized\"", "\"bool\"",
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
  "\"?[\"", "\"<|\"", "\" <|\"", "\"$ <|\"", "\"@ <|\"", "\"@@ <|\"",
  "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"",
  "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"",
  "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"",
  "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
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
  "annotation_argument_list", "metadata_argument_list",
  "annotation_declaration_name", "annotation_declaration_basic",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@6", "expression_block", "expr_call_pipe",
  "expression_any", "expressions", "expr_keyword", "optional_expr_list",
  "optional_expr_list_in_braces", "optional_expr_map_tuple_list",
  "type_declaration_no_options_list", "expression_keyword", "$@7", "$@8",
  "$@9", "$@10", "expr_pipe", "name_in_namespace", "expression_delete",
  "new_type_declaration", "$@11", "$@12", "expr_new", "expression_break",
  "expression_continue", "expression_return_no_pipe", "expression_return",
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
  "bitfield_type_declaration", "$@47", "$@48", "c_or_s", "table_type_pair",
  "dim_list", "type_declaration_no_options", "$@49", "$@50", "$@51",
  "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60",
  "$@61", "$@62", "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69",
  "$@70", "$@71", "$@72", "$@73", "$@74", "$@75", "type_declaration",
  "tuple_alias_declaration", "$@76", "$@77", "$@78", "$@79",
  "variant_alias_declaration", "$@80", "$@81", "$@82", "$@83",
  "bitfield_alias_declaration", "$@84", "$@85", "$@86", "$@87",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim", "make_struct_dim_list",
  "make_struct_dim_decl", "optional_make_struct_dim_decl",
  "optional_block", "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "use_initializer", "make_struct_decl",
  "$@88", "$@89", "$@90", "$@91", "$@92", "$@93", "$@94", "$@95",
  "make_tuple", "make_map_tuple", "make_tuple_call", "$@96", "$@97",
  "make_dim", "make_dim_decl", "$@98", "$@99", "$@100", "$@101", "$@102",
  "$@103", "$@104", "$@105", "$@106", "$@107", "make_table",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1460)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-755)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1460,   102, -1460, -1460,    45,   -74,   215,   -75, -1460,   -51,
     519,   519,   519, -1460,    93,    51, -1460, -1460,     7, -1460,
   -1460, -1460,   388, -1460,   198, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460,   -39, -1460,   121,   128,   160,
   -1460, -1460, -1460, -1460,   215, -1460,    32, -1460,   519,   519,
   -1460, -1460,   198, -1460, -1460, -1460, -1460, -1460,   188,   281,
   -1460, -1460, -1460,    51,    51,    51,   309, -1460,   524,   201,
   -1460, -1460, -1460, -1460,   593,   607,   608, -1460,   609,    40,
      45,   303,   -74,   334,   440, -1460,   605,   605, -1460,   447,
     397,    10,   471,   619,   468,   485,   510, -1460,   550,   508,
   -1460, -1460,   -63,    45,    51,    51,    51,    51, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460,   571, -1460, -1460, -1460, -1460,
   -1460,   569, -1460, -1460, -1460, -1460, -1460,   228,   115, -1460,
   -1460, -1460, -1460,   649, -1460, -1460, -1460, -1460, -1460,   544,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,   615,
   -1460,   130, -1460,   651,   652,   524, 14615, -1460,   444,   696,
   -1460,   -93, -1460, -1460,   589, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460,   193, -1460,   616, -1460,   626,   629,   630, -1460,
   -1460, 13193, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460,   776,   778, -1460,
     601,   636, -1460,   398, -1460,   648, -1460,   638,    45,    45,
     126,   385, -1460, -1460, -1460,   115, -1460,  9893, -1460, -1460,
   -1460,   650,   653, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460,   655,   613, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460,   799, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
     657,   618, -1460, -1460,    65,   641, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460,   663,   656,   664,
   -1460,   -93,   274, -1460, -1460,    45,   634,   785,   210, -1460,
   -1460,   658,   659,   660,   637,   661,   662, -1460, -1460, -1460,
     645, -1460, -1460, -1460, -1460, -1460,   665, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,   666, -1460,
   -1460, -1460,   667,   671, -1460, -1460, -1460, -1460,   672,   673,
     669,    93, -1460, -1460, -1460, -1460, -1460,   976,   677, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460,   680,   733, -1460,   670,
   -1460,    79, -1460,   111,  9893, -1460,  1743,   381, -1460,    93,
   -1460, -1460, -1460,   385,   668, -1460,  9088,   702,   707,  9893,
   -1460,   -28, -1460, -1460, -1460,  9088, -1460, -1460,   708, -1460,
     344,   345,   356, -1460, -1460,  9088,   127, -1460, -1460, -1460,
      13, -1460, -1460, -1460,    30,  5448, -1460,   675,  9649,   403,
    9748,   368, -1460, -1460,  9088, -1460, -1460,   387,   -32, -1460,
     677, -1460,   695,   697,  9088, -1460, -1460, -1460, -1460, -1460,
     772,   -45,   698,    36,  3218, -1460, -1460,   636,   205,  5650,
     687,  9088,   724,   709,   710,   692, -1460,   726,   714,   747,
    5852,   109,   295,   717, -1460,   307,   718,   719,  3420,  9088,
    9088,   133,   133,   133,   705,   711,   713,   715,   716,   721,
   -1460,  9451,  9550,  6056, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460,  6258,   727, -1460,  6462,   878, -1460,  9088,  9088,  9088,
    9088,  9088,  5044,  9088, -1460,   704, -1460, -1460,   741,   759,
    9088,   933, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460,   278, -1460,   -52,   763, -1460,   764,   765,   766, -1460,
     768, -1460, -1460,   884, -1460, -1460, -1460, -1460,   742, -1460,
   -1460,   -10, -1460, -1460, -1460, -1460, -1460, -1460,  1953, -1460,
     743, -1460, -1460, -1460, -1460, -1460, -1460,   340, -1460,   774,
   -1460, -1460,    37, -1460, -1460,   744,   761,   767, -1460, 10332,
   -1460,   915,   812, -1460, -1460, -1460,  3824,  9893,  9893,  9893,
   10442,  9893,  9893,   745,   793,  9893,   601,  9893,   601,  9893,
     601,  9992,   794, 10477, -1460,  9088, -1460, -1460, -1460, -1460,
     756, -1460, -1460, 12652,  9088, -1460,   976,   788, -1460,   790,
       6, -1460, -1460,   497, -1460,   677,   796,   784,   497, -1460,
     798, 10587,   771,   936, -1460,   -47, -1460, -1460, -1460, 13228,
     387, -1460,   773, -1460, -1460,    93,   311, -1460,   786,   787,
     797, -1460,  9088,  3824, -1460,   800,   857, 10075,   978,  9893,
    9088,  9088, 13969,  9088, 13228,   806, -1460, -1460,  9088, -1460,
   -1460,   805,   835, 13969,  9088, -1460, -1460,  9088, -1460, -1460,
    9088, -1460,  9893,  3824, -1460, 10075,   104,   104,   782, -1460,
     742, -1460, -1460, -1460,  9088,  9088,  9088,  9088,  9088,  9088,
     387,  2609,   387,  2812,   387, 13327, -1460,   549, -1460, 13228,
   -1460,   518,   387, -1460,   815,   820,   104,   104,   -16,   104,
     104,   387,   994,   823, 13969,   823,   187, -1460, -1460, 13228,
   -1460, -1460, -1460, -1460,  4026, -1460, -1460, -1460, -1460, -1460,
   -1460,   140,    38,   133, -1460, 13067, 14489,  4228,  4228,  4228,
    4228,  4228,  4228,  4228,  4228,  9088,  9088, -1460, -1460,  9088,
    4228,  4228,  9088,  9088,  9088,   842,  4228,  9088,   428,  9088,
    9088,  9088,  9088,  9088,  9088,  4228,  4228,  9088,  9088,  9088,
    4228,  4228,  4228,  9088,  4228,  6664,  9088,  9088,  9088,  9088,
    9088,  9088,  9088,  9088,  9088,  9088,   166,  9088, -1460,  6866,
   -1460,  9088, -1460,   381, -1460,    51,  1004,   -93,  9893, -1460,
     845, -1460,  3824, -1460, 10187,   435,   462,   821,   464, -1460,
     512,   527, -1460, -1460,   173,   537,   641,   546,   641,   547,
     641, -1460,   379, -1460,   408, -1460,  9893,   801,   823, -1460,
   -1460, 12751, -1460, -1460,  9893, -1460, -1460,  9893, -1460, -1460,
   -1460,  9088,   848, -1460,   850, -1460,  9893, -1460,  3824,  9893,
    9893, -1460,    24,  9893,  5246,  7068,   852,  9088,  9893, -1460,
   -1460, -1460,  9893,   823, -1460,   800,  9088,  9088,  9088,  9088,
    9088,  9088,  9088,  9088,  9088,  9088,  9088,  9088,  9088,  9088,
    9088,  9088,  9088,  9088,   636,   837,   808,   815, 13969, 10622,
   -1460, -1460,  9893,  9893, 10732,  9893, -1460, -1460, 10767,  9893,
     823,  9893,  9893,   823,  9893,   706, -1460, 10075, -1460,    38,
   10877, 10912, 11022, 11057, 11167, 11202,    57,   133,   814,   194,
    3015,  4432,  7270, 13426,   838,    -1,   167,   844,   177,    63,
    7472,    -1,   599,    64,  9088,   854,  9088, -1460, -1460,  9893,
   -1460,  9893, -1460,  9088,   583,    67,  9088,   855, -1460,    74,
     387,  9088,   826,   827,   832,   833,   357, -1460, -1460,   448,
    9088,   742,   -95,  4636, -1460,   200,   838,   836,   871,   871,
   -1460, -1460,   843,   131,   601, -1460,   864,   847, -1460, -1460,
     868,   856, -1460, -1460,   133,   133,   133, -1460, -1460,  2152,
   -1460,  2152, -1460,  2152, -1460,  2152, -1460,  2152, -1460,  2152,
   -1460,  2152, -1460,  2152,   463,   463,  2190, -1460,  2152, -1460,
    2152,  2190, 10037, 10037,   858, -1460,  2152,    48,   859, -1460,
   12850,   -57,   -57,   743, 13969,   463,   463, -1460,  2152, -1460,
    2152, 14213,  1231, 14184, -1460,  2152, -1460,  2152, -1460,  2152,
   14059, -1460,  2152, 14584, 13461, 14303, 14331, 14421,  2190,  2190,
    1141,  1141,    48,    48,    48,   456,  9088,   860,   862,   460,
    9088,  1062, 12885, -1460,   211, 13551,   887,   275,   623,  1003,
     895,  1019, -1460, -1460, 10297, -1460, -1460, -1460, -1460,  9893,
   -1460, -1460,   909, -1460, -1460,   886, -1460,   888, -1460,   889,
   -1460,  9992, -1460,   794,   413,   677, -1460, -1460, -1460,   677,
     677, 11312, -1460,  1042,   -21, -1460, 10075,  1095,  1663,  9088,
     554,   467,   229,   873,   875,   921, 11347,   364, 11457,   556,
    9893,  9893,  9893,  1673,   882, 13969, 13969, 13969, 13969, 13969,
   13969, 13969, 13969, 13969, 13969, 13969, 13969, 13969, 13969, 13969,
   13969, 13969, 13969, -1460,   879,  9893, -1460, -1460, -1460,  9088,
    1918,  2333, -1460,  2529, -1460,  2731,   890,  2934,  3137,   891,
    3340,    38,   601, -1460, -1460, -1460, -1460, -1460,   885,  9088,
   -1460,  9088,  9088,  9088,  4840, 12652,     5,  9088,   470,   467,
     167, -1460, -1460,   897, -1460,  9088,  9088, -1460,   898, -1460,
    9088,   467,   461,   899, -1460, -1460,  9088, 13969, -1460, -1460,
     420,   423, 13586,  9088, -1460, -1460,  2407,  9088,    87, -1460,
   -1460,  9088,  9088,  9893,   601,   636, -1460, -1460,  9088, -1460,
    1490,    38,    11, -1460,   900,   283,  9290, -1460, -1460, -1460,
     291,   230,   131,   932,   943,   903,   944,   948, -1460,   292,
     641, -1460,  9088, -1460,  9088, -1460, -1460, -1460,  7674,  9088,
   -1460,   908,   905, -1460, -1460,  9088,   906, -1460, 12984,  9088,
    7876,   907, -1460, 13083, -1460, -1460, -1460, -1460, -1460,   941,
   -1460, -1460,   209, -1460,    29, -1460,    38, -1460, -1460, -1460,
   -1460,   677, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460,   910,  9893, -1460,   955,
    9088, -1460, -1460,   374, -1460,   911, -1460, -1460, -1460,   433,
   -1460,   957,   914, -1460, -1460,  3542,  3744,  3946, -1460, -1460,
    9088,  4148, 13969, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460,   585,   641,  8078,   502, 11492, 13969, 13969,    -1,
     167, 13969,   916,    90,   838, -1460, -1460, 13969,   844, -1460,
     509,    -1,   918, -1460, -1460, -1460, -1460,   529, -1460, -1460,
   -1460,   533, -1460,   534,  9088, 11602, 11637,  4225,   641, -1460,
   13228, -1460,   952,   601, -1460,   920,  4636,   966,   925,   141,
     235, -1460, -1460,   969, -1460, -1460,   131,   927,   162,  9893,
   11747,  9893, 11782, -1460,   244, 11892, -1460,  9088, 14094,  9088,
   -1460, 11927, -1460,   252,  9088, -1460, -1460, -1460,  1107,    29,
   -1460, -1460,   623,   930, -1460, -1460, -1460,  9088,   677, -1460,
   13969,   931,   934, -1460, -1460, -1460,  9088,   981,   950,  9088,
   -1460, -1460, -1460, -1460,   938,   939,   945,  9088,  9088,  9088,
     946,  1082,   947,   953,  8280, -1460,   162, -1460,   261,  9088,
      92,   167, -1460,  9088,  9088,  9088,  9088,   461, -1460,  9088,
    9088,   954,  9088,  9088,   536, -1460, -1460, -1460,   959,   448,
    3622, -1460,   641, -1460,   315, -1460,   409,  9893,   -28, -1460,
     960, -1460, -1460,  8482, -1460, -1460,  4352, -1460,   563, -1460,
   -1460, -1460,  9893, 12037, 12072, -1460, -1460, 12182, -1460, -1460,
    1107,   387,   961,  1082,  1082, 12217,   962,   964, 12327,   965,
     967,   970,  9088, -1460,  9088,  2190,  2190,  2190,  9088, -1460,
   -1460,  1082,   467, -1460, 12362, -1460, -1460, 13676,  9088,  9088,
   -1460, 12472, 13969, 13969, 13676, -1460,   987,  2190,  9088, -1460,
     987, 13676,  9088,   204, -1460, -1460,  8684,  8886, -1460, -1460,
   -1460, -1460, -1460, 13969,   636,   968,  9893,   -28,   572,  9088,
   -1460,  9088, 14059, -1460, -1460,   578, -1460, -1460, -1460, 14615,
   -1460, -1460, -1460,   132,   132, -1460,  9088,  9088, -1460,  1082,
    1082,   467,   971,   972,   823,   132,   838,   974, -1460,  1120,
     977, 13969, 13969,   190,  1011,  1012,   997,  1014,   983, 13676,
   -1460,   204,  9088,  9088, 13969, -1460, -1460,   572,  9088,  9088,
   13715, 14094, -1460, -1460, -1460,  1015, 14615,   467,   838,  1010,
   -1460,   984,   988, 12507, 12617,   132,   132,   990, -1460, -1460,
     992,   993, -1460,  9088,   989,  9088,  9088,   999,  1029, -1460,
    1002, -1460, -1460,  1001, -1460, 13969,  9088, 13805, 13840, -1460,
   -1460, -1460,   636,   267,  1007, -1460, -1460, -1460, -1460, -1460,
    1009,  1013, -1460, -1460, -1460, 13969, -1460, 13969, 13969, -1460,
   -1460, -1460, -1460, 13930, -1460, -1460, -1460, -1460,   467, -1460,
   -1460, -1460,   269, -1460
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   121,     1,   301,     0,     0,     0,   613,   302,     0,
     605,   605,   605,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   593,     6,    11,     5,     4,    13,
      12,    14,    92,    93,    91,   100,   102,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   605,   605,
      22,    21,   593,   607,   606,   776,   766,   771,     0,   269,
      35,   108,   109,     0,     0,     0,   110,   112,   119,     0,
     107,    17,   631,   630,   214,   615,   632,   594,   595,     0,
       0,     0,     0,    43,     0,    49,     0,     0,    46,     0,
       0,   605,     0,    18,     0,     0,     0,   271,     0,     0,
     118,   113,     0,     0,     0,     0,     0,     0,   122,   216,
     215,   218,   213,   617,   616,     0,   634,   633,   637,   597,
     596,   599,    98,    99,    96,    97,    95,     0,     0,    94,
     103,    54,    52,    48,    45,    44,   608,   610,   612,     0,
     614,    19,    20,    23,   777,   767,   772,   270,    33,    36,
     117,     0,   114,   115,   116,   120,     0,   618,     0,   627,
     590,   528,    24,    25,     0,    87,    88,    85,    86,    84,
      83,    89,     0,    47,     0,   611,     0,     0,     0,    34,
     111,     0,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,     0,     0,   128,
     123,     0,   619,     0,   628,     0,   638,   591,     0,     0,
     530,     0,    26,    27,    28,     0,   101,     0,   778,   768,
     773,   182,   183,   180,   131,   132,   134,   133,   135,   136,
     137,   138,   164,   165,   162,   163,   155,   166,   167,   156,
     153,   154,   181,   175,     0,   179,   168,   169,   170,   171,
     142,   143,   144,   139,   140,   141,   152,     0,   158,   159,
     157,   150,   151,   146,   145,   147,   148,   149,   130,   129,
     174,     0,   160,   161,   528,   126,   246,   219,   601,   672,
     675,   678,   679,   673,   676,   674,   677,     0,   625,   635,
     598,   528,     0,   104,   106,     0,     0,   580,   578,   600,
      90,     0,     0,     0,     0,     0,     0,   645,   665,   646,
     681,   647,   651,   652,   653,   654,   671,   658,   659,   660,
     661,   662,   663,   664,   666,   667,   668,   669,   736,   650,
     657,   670,   743,   750,   648,   655,   649,   656,     0,     0,
       0,     0,   680,   698,   701,   699,   700,   763,   609,   686,
     558,   564,   184,   185,   178,   173,   186,   176,   172,     0,
     124,   300,   552,     0,     0,   217,     0,   620,   622,     0,
     629,   542,   639,     0,     0,   105,     0,     0,     0,     0,
     579,     0,   704,   727,   730,     0,   733,   723,     0,   689,
     737,   744,   751,   757,   760,     0,     0,   713,   718,   712,
       0,   726,   722,   715,     0,     0,   717,   702,     0,   779,
     769,   774,   187,   177,     0,   298,   299,     0,   528,   125,
     127,   248,     0,     0,     0,    63,    64,    76,   434,   435,
       0,     0,     0,     0,   286,   428,   284,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   285,     0,     0,     0,
       0,     0,     0,     0,   671,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     517,     0,     0,     0,   354,   356,   355,   357,   358,   359,
     360,     0,     0,    29,     0,   220,   225,     0,     0,     0,
       0,     0,     0,     0,   336,   337,   432,   431,     0,     0,
       0,     0,   241,   236,   233,   232,   234,   235,   268,   247,
     227,   511,   226,   429,     0,   502,    71,    72,    69,   239,
      70,   240,   242,   304,   231,   501,   500,   499,   121,   505,
     430,     0,   228,   504,   503,   475,   436,   476,   361,   437,
       0,   433,   781,   785,   782,   783,   784,     0,   602,     0,
     601,   626,   543,   592,   529,     0,     0,     0,   511,     0,
     582,   583,     0,   576,   577,   575,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   123,     0,   123,     0,
     123,     0,     0,     0,   709,   250,   720,   721,   714,   716,
       0,   719,   703,     0,     0,   765,   764,     0,   687,     0,
     269,   693,   692,     0,   559,   554,     0,     0,     0,   565,
       0,     0,     0,   640,   550,   569,   553,   826,   829,     0,
       0,   274,   278,   277,   283,     0,     0,   322,     0,     0,
       0,   862,     0,     0,   290,   287,     0,   331,     0,     0,
     254,     0,   272,     0,     0,     0,   313,   316,     0,   245,
     319,     0,     0,    57,     0,    78,   866,     0,   835,   844,
       0,   832,     0,     0,   295,   292,   464,   465,   337,   349,
     121,   267,   265,   266,     0,     0,     0,     0,     0,     0,
       0,   804,     0,     0,     0,   842,   869,     0,   258,     0,
     261,     0,     0,   871,   880,     0,   441,   440,   477,   439,
     438,     0,     0,   880,   331,   880,   338,   243,   244,     0,
      74,   352,   223,   509,     0,   230,   237,   238,   289,   294,
     303,     0,   346,     0,   229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   466,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   418,     0,   222,     0,
     603,     0,   621,   623,   636,     0,     0,   528,     0,   581,
       0,   585,     0,   589,   361,     0,     0,     0,   694,   707,
       0,     0,   682,   684,     0,     0,   126,     0,   126,     0,
     126,   556,     0,   562,     0,   683,     0,     0,   880,   711,
     696,     0,   688,   780,     0,   560,   770,     0,   566,   775,
     551,     0,     0,   568,     0,   567,     0,   570,     0,     0,
       0,    79,     0,     0,   818,     0,     0,     0,     0,   852,
     855,   858,     0,   880,   291,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   880,   273,     0,
      80,    81,     0,     0,     0,     0,    55,    56,     0,     0,
     880,     0,     0,   880,     0,     0,   296,   293,   338,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   269,
       0,     0,     0,   838,   796,   804,     0,   847,     0,     0,
       0,   804,     0,     0,     0,     0,     0,   807,   874,     0,
     249,     0,    32,     0,    30,     0,   881,     0,   246,     0,
       0,   881,     0,     0,     0,     0,   408,   405,   407,    60,
       0,   121,     0,     0,   421,     0,   795,     0,     0,     0,
     312,   311,     0,     0,   123,   264,     0,     0,   488,   487,
       0,     0,   489,   493,     0,     0,     0,   383,   392,   371,
     393,   372,   395,   374,   394,   373,   396,   375,   386,   365,
     387,   366,   388,   367,   442,   443,   455,   397,   376,   398,
     377,   456,   453,   454,     0,   385,   363,   482,     0,   473,
       0,   506,   507,   508,   364,   444,   445,   399,   378,   400,
     379,   460,   461,   462,   389,   368,   390,   369,   391,   370,
     463,   384,   362,     0,     0,   458,   459,   457,   451,   452,
     447,   446,   448,   449,   450,     0,     0,     0,   414,     0,
       0,     0,     0,   426,     0,     0,     0,     0,   536,   539,
       0,     0,   584,   587,   361,   588,   705,   728,   731,     0,
     734,   724,     0,   690,   738,     0,   745,     0,   752,     0,
     758,     0,   761,     0,     0,   256,   708,   251,   697,   555,
     561,     0,   642,   643,   571,   574,   573,     0,     0,     0,
       0,   819,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   332,   371,   372,   374,   373,
     375,   365,   366,   367,   376,   377,   363,   378,   379,   368,
     369,   370,   362,   297,     0,     0,   875,   255,   478,     0,
       0,     0,   479,     0,   510,     0,     0,     0,     0,     0,
       0,   346,   123,   512,   513,   514,   515,   516,     0,     0,
     805,     0,     0,     0,     0,   331,   804,     0,     0,     0,
       0,   813,   814,     0,   821,     0,     0,   811,     0,   850,
       0,     0,     0,     0,   809,   851,     0,   841,   806,   870,
       0,     0,     0,     0,   872,   873,     0,     0,     0,   849,
     468,     0,     0,     0,   123,     0,    58,    59,     0,    73,
      65,   346,     0,   422,     0,     0,     0,   425,   423,   305,
       0,     0,     0,     0,     0,     0,     0,     0,   344,     0,
     126,   484,     0,   490,     0,   382,   380,   381,     0,     0,
     471,     0,     0,   494,   498,     0,     0,   474,     0,     0,
       0,     0,   415,     0,   419,   469,   427,   604,   624,   122,
     537,   538,   539,   540,   531,   544,   346,   586,   706,   729,
     732,   695,   735,   725,   685,   691,   739,   741,   746,   748,
     753,   755,   759,   557,   762,   563,     0,     0,   641,     0,
       0,   827,   830,     0,   275,     0,   280,   281,   279,     0,
     325,     0,     0,   328,   323,     0,     0,     0,   863,   861,
     254,     0,    82,   314,   317,   320,   867,   865,   836,   845,
     843,   833,     0,   126,     0,     0,     0,   787,   786,   804,
       0,   839,     0,     0,   797,   820,   812,   840,   848,   810,
       0,   804,     0,   816,   817,   824,   808,     0,   259,   262,
      31,     0,   221,     0,     0,     0,     0,     0,   126,    61,
       0,    66,     0,   123,   424,     0,     0,     0,     0,   578,
       0,   341,   342,     0,   340,   339,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,   495,     0,   483,     0,
     472,     0,   416,     0,     0,   470,   420,   549,   534,   531,
     532,   533,   536,     0,   742,   749,   756,   250,   257,   644,
     572,     0,     0,    77,   276,   282,     0,     0,     0,     0,
     324,   853,   856,   859,     0,     0,     0,     0,     0,     0,
       0,   818,     0,     0,     0,   224,     0,   518,     0,     0,
       0,     0,   822,     0,     0,     0,     0,     0,   815,     0,
       0,   252,     0,     0,     0,   406,   527,   409,     0,    60,
       0,    75,   126,   401,     0,   306,   578,     0,     0,   348,
       0,   345,   347,     0,   333,   351,     0,   526,     0,   524,
     404,   521,     0,     0,     0,   520,   417,     0,   535,   545,
     534,     0,     0,   818,   818,     0,     0,     0,     0,     0,
       0,     0,   250,   876,   254,   315,   318,   321,     0,   819,
     837,   818,     0,   480,     0,   350,   519,   878,     0,     0,
     823,     0,   789,   788,   878,   825,   878,   260,   250,   263,
     878,   878,     0,     0,   412,    62,   286,     0,    67,    71,
      72,    69,    70,    68,     0,     0,     0,     0,     0,     0,
     343,     0,   334,   485,   491,     0,   525,   523,   522,     0,
     547,   541,   710,   803,   803,   326,     0,     0,   329,   818,
     818,     0,     0,     0,   880,   803,   794,     0,   481,     0,
       0,   791,   790,     0,     0,     0,   880,     0,     0,   878,
     410,     0,     0,     0,   292,   353,   402,     0,     0,     0,
       0,   335,   486,   492,   496,     0,     0,     0,   800,   880,
     802,     0,     0,     0,     0,   803,   803,     0,   864,   877,
       0,     0,   834,     0,     0,     0,     0,     0,     0,   881,
       0,   886,   882,     0,   413,   293,     0,     0,     0,   310,
     497,   546,     0,     0,   881,   801,   828,   831,   327,   330,
       0,     0,   860,   868,   846,   879,   884,   793,   792,   885,
     887,   253,   883,     0,   309,   308,   548,   798,     0,   854,
     857,   307,     0,   799
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1460, -1460, -1460, -1460, -1460, -1460,   516,  1142, -1460, -1460,
   -1460,  1223, -1460, -1460, -1460,   684,  1181, -1460,  1093, -1460,
   -1460,  1146, -1460, -1460, -1460,  -238, -1460, -1460, -1460,  -236,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,  1016,
   -1460, -1460,   -40,   -46, -1460, -1460, -1460,   346,   437,  -524,
    -566,  -785, -1460, -1460, -1460, -1459, -1460, -1460,  -209,  -368,
   -1460,   287, -1460, -1357, -1460, -1277,  -299,   560, -1460, -1460,
   -1460, -1460,  -407,   -14, -1460, -1460, -1460, -1460, -1460,  -232,
    -227,  -225, -1460,  -223, -1460, -1460, -1460,  1247, -1460,   280,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460,  -397,  -196,    58,  -135,    22,  -893,  -443,
   -1460,  -516, -1460, -1460,  -369,   644, -1460, -1460, -1460, -1316,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,   795,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460,  -151,  -153,  -243,
    -152,   -11, -1460, -1460, -1460, -1460, -1460,   834, -1460,  -572,
   -1460, -1460,  -581, -1460, -1460,  -616,  -234,  -568, -1331, -1460,
    -360, -1460, -1460,  1220, -1460, -1460, -1460,   720,   775,   152,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
    -591,  -149, -1460,   839, -1460, -1460, -1460, -1460, -1460, -1460,
    -403, -1460, -1460,  -386, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460,  -215, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460,   841,  -665,  -308,
    -818,  -659, -1460, -1460, -1235,  -904, -1460, -1460, -1460, -1155,
    -183, -1388, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,
   -1460,    89,  -477, -1460, -1460, -1460,   592, -1460, -1460, -1460,
   -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460, -1460,   813,
   -1460, -1230,  -700, -1460
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    52,    17,   164,   170,   701,   506,
     149,   507,    99,    19,    20,    45,    46,    47,    88,    21,
      39,    40,   508,   509,  1218,  1219,   510,  1372,  1470,   511,
     512,   960,   513,   630,   514,   515,   516,   517,  1149,   171,
     172,    35,    36,    37,   220,    66,    67,    68,    69,    22,
     285,   375,   210,    23,   111,   211,   112,   156,   679,   987,
     519,   376,   520,   827,  1539,   886,  1094,   568,   939,  1460,
     941,  1461,   522,   523,   524,   632,   853,  1424,   525,   526,
     527,   528,   529,   530,   531,   532,   427,   533,   731,  1230,
     970,   534,   535,   892,  1437,   893,  1438,   895,  1439,   536,
     858,  1430,   537,   713,  1485,   538,  1238,  1239,   974,   681,
     539,   788,   961,   540,   646,   988,   542,   543,   544,   958,
     545,  1213,  1543,  1214,  1601,   546,  1061,  1406,   547,   714,
    1389,  1612,  1391,  1613,  1492,  1650,   549,   371,  1412,  1499,
    1272,  1274,  1070,   562,   797,  1569,  1616,   372,   373,   613,
     822,   420,   618,   824,   421,  1173,   624,   576,   391,   308,
     309,   217,   301,    78,   121,    25,   161,   377,    89,    90,
     174,    91,    26,    49,   115,   158,    27,   288,   559,   560,
    1066,   380,   215,   216,    76,   118,   382,    28,   159,   299,
     625,   550,   297,   354,   355,   814,   419,   356,   584,  1285,
    1297,   807,   417,   357,   577,  1278,   826,   582,  1283,   578,
    1279,   579,  1280,   581,  1282,   585,  1286,   586,  1414,   587,
    1288,   588,  1415,   589,  1290,   590,  1416,   591,  1292,   592,
    1294,   615,    29,    95,   177,   360,   616,    30,    96,   178,
     361,   620,    31,    94,   176,   359,   609,   551,  1618,  1587,
     967,   925,  1619,  1620,  1621,   926,   938,  1195,  1189,  1184,
    1355,  1114,   552,   849,  1421,   850,  1422,   904,  1443,   901,
    1441,   927,   703,   553,   902,  1442,   928,   554,  1120,  1509,
    1121,  1510,  1122,  1511,   862,  1434,   899,  1440,   697,   887,
     555,  1590,   947,   556
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    70,   287,   572,   802,   722,   696,   541,   518,   848,
     221,   823,   358,   952,   732,   953,  1162,   614,   619,   821,
     816,  1180,   818,   563,   820,  1345,   924,  1192,   924,   682,
     683,  1085,   606,  1087,   931,  1089,  1113,   644,   796,   852,
     130,   638,   972,  1435,  1109,  -121,   596,   645,  1478,    70,
      70,    70,   917,  1520,    84,   122,   123,   151,   917,   966,
    1502,   674,   918,   598,    32,    33,   218,    53,   755,   735,
     736,   758,   759,    54,  1222,  1410,   518,  1169,   353,   843,
     104,   105,   106,  1190,  1196,    61,   425,  1203,    38,    85,
      70,    70,    70,    70,  1207,   691,   693,    58,   573,   916,
     518,   929,     2,   933,    48,   843,   715,  1364,   574,     3,
    1615,   945,   758,   759,    62,  1573,  1574,   723,    50,  1223,
     949,   733,   369,   219,    59,   735,   736,   218,  1097,   426,
     165,   166,     4,  1585,     5,  1411,     6,   843,   639,   640,
     844,   845,     7,   846,  1224,  1557,   847,  1172,    98,    79,
     383,   150,     8,   786,   787,  1582,   909,  1652,     9,   430,
     575,   724,   918,  1124,    51,   747,   748,   845,   635,   803,
     418,   734,   302,   755,  1222,   757,   758,   759,   760,   303,
    1179,  1596,    10,   761,   219,  1452,    63,  1147,    71,   966,
     138,  1625,  1626,    58,   786,   787,   834,   973,   828,   845,
    1156,    92,   124,  1159,    11,    12,   597,   125,   518,   126,
     835,   844,   127,   352,    34,   838,   843,   794,   843,   369,
      59,   747,   748,   599,   218,   353,  1454,  1600,  1528,   755,
     641,    86,   758,   759,   760,    58,   864,  1583,   648,   761,
     353,   600,    87,   139,   844,   863,   865,   601,   795,   642,
     844,   844,   665,   128,   844,    64,  1408,  1233,   786,   787,
      77,   844,    59,   885,    65,   385,   906,  1234,  1332,   353,
     900,   353,  1340,   903,   844,   518,  1273,   167,   845,   370,
     845,   219,   168,    13,   169,  1644,   905,   127,   286,   918,
     975,   306,   428,  1305,  1055,  1056,  1530,   966,  1225,   968,
    1235,  1222,    14,  1483,  1594,   518,  1595,   304,    80,   307,
    1597,  1598,   286,    15,   786,   787,   843,    80,    81,  1236,
     843,   594,   664,  1057,  1237,   429,  1635,   965,  1373,    58,
    1172,  1477,   353,   353,   390,  1058,    98,   406,   954,  1622,
     595,   286,   305,   955,   180,  1617,    58,    82,   504,   678,
    1631,  1181,  1182,   969,  1082,   954,    59,    97,  1186,  1208,
     352,  1187,   805,   806,   808,   561,   810,   811,  1083,  1643,
     815,   971,   817,    59,   819,   352,  1059,  1060,   845,  1183,
     225,   956,   845,  1413,    41,    42,    43,  1226,   107,  1188,
    1660,  1661,  1064,    72,    73,  1073,    74,   388,   866,   649,
     389,   162,   163,   390,   352,  1079,   352,   226,  1240,   100,
     101,   102,  1071,   108,  1227,    44,   866,  1377,   650,  1091,
     851,  1093,  1386,    98,    75,  1266,   352,   636,   353,   353,
     353,   866,   353,   353,   518,  1451,   353,  1221,   353,   866,
     353,  1105,   353,  1306,  1379,   890,   966,  1457,   866,  1479,
     152,   153,   154,   155,  1178,  1388,  1178,  1112,  1490,  1199,
    1215,    80,   107,  1107,  1108,   131,  1496,   352,   352,  1204,
    1178,  1216,  1217,   289,  1170,  1526,  1123,   290,  1377,  1386,
     518,  1677,  1145,  1683,   735,   736,   384,  1269,  1300,   666,
     940,   291,   292,   504,   678,  1375,   293,   294,   295,   296,
     353,   669,  1178,  1378,  1387,   856,  1150,  1151,   667,  1153,
     959,   924,  1295,  1155,  1344,  1157,  1158,  1339,  1160,  1293,
     670,   790,   103,   353,   857,   715,   924,  1555,   791,   680,
     680,   680,  1351,   715,  -740,  -747,   306,   617,   622,  -740,
    -747,  1245,  1246,  1247,    87,  1311,  -754,  -411,  1446,   611,
     557,  -754,  -411,   286,   307,   612,   623,  -740,  -747,  1312,
     611,   866,   558,   352,   352,   352,   612,   352,   352,  -754,
    -411,   352,   607,   352,  1090,   352,    53,   352,   137,   721,
     747,   748,    54,  1468,   608,  1018,   979,   983,   755,   611,
     757,   758,   759,   760,   611,   612,  1333,  1019,   761,  1556,
     612,   611,   390,  1092,   611,   407,   721,   612,  1296,   133,
     612,  1095,  1023,  1256,   611,  1358,   136,  1261,  1359,  1099,
     612,   855,  1100,   212,   918,  1257,   418,  1342,  1426,  1262,
    1076,  1104,   408,   409,   213,   352,  1222,   144,  1110,  1343,
    1200,  1352,  1201,  1119,  1353,   611,  1069,  1354,  1368,   353,
     109,   612,   140,   418,   145,   418,   110,  1077,   352,   781,
     782,   783,   784,   785,   113,   116,   119,   104,   105,   106,
     114,   117,   120,   786,   787,  1143,   141,   353,   611,   146,
    1270,   148,   142,  1449,   612,   353,  1271,  1554,   353,   866,
    1456,   162,   163,   942,   943,   410,   866,   353,   573,   411,
     353,   353,   957,   418,   353,   721,    85,  1080,   574,   353,
    1459,  1474,  1303,   353,  1462,  1463,   866,  1542,   418,   147,
     866,   866,  1081,   866,  1095,   175,  1095,  1455,   418,   935,
     936,   937,  1084,   721,  1315,  1316,  1317,   418,   418,   407,
     157,  1086,  1088,   353,   353,   418,   353,   418,   160,  1304,
     353,  1314,   353,   353,   418,   353,   222,   223,  1564,  1321,
     575,   412,   222,   223,   224,   413,   408,   409,   414,   418,
     134,   135,  1335,  1614,    41,    42,    43,   432,   433,  1193,
    1186,    70,  1194,   415,   352,    55,    56,    57,   179,   416,
     353,   680,   353,  1350,   104,   104,   106,   443,  1444,  1357,
     504,   678,   214,   448,   227,   228,  1361,  1472,   229,   230,
    1363,   282,   352,   283,   284,   286,  1445,   298,   300,   362,
     352,   366,   363,   352,   364,   365,   367,  1367,   387,   410,
     368,   374,   352,   411,  1529,   352,   352,   541,   518,   352,
     462,   463,   378,   381,   352,   407,   379,   386,   352,   422,
     395,  1394,   392,   393,   394,   396,   397,  1586,   398,   399,
     400,   401,   721,  1403,  1281,   402,   403,   404,   418,   423,
     407,   570,   408,   409,   465,   466,   571,   583,   352,   352,
     564,   352,   405,   424,  1630,   352,   604,   352,   352,   627,
     352,   628,   637,   655,  1254,   412,  1640,   408,   409,   413,
     653,  1161,   414,   656,   657,   658,  1427,   659,   660,   661,
    1559,   668,   671,   672,    58,   705,  1586,   415,   684,  1655,
     716,   700,   717,   416,   685,   352,   686,   352,   687,   688,
     353,   481,   482,   483,   689,   410,   521,  1448,   573,   411,
     718,    59,   353,   720,   725,   726,   727,   728,   574,   729,
     730,   494,  1653,    15,   792,   798,   789,   664,   800,   812,
     410,   698,   813,   617,   411,   721,   631,  1464,   829,   832,
     833,   353,   353,   353,   837,   680,   836,  1636,   839,   842,
     859,   860,  1536,   502,   841,  1540,   854,   866,   733,  1608,
    1609,   861,   884,   801,   891,   896,   353,   897,   908,   948,
     575,   412,   946,  1486,   521,   413,  1369,   950,   414,   407,
     951,  1014,  1068,  1682,  1072,  1096,  1078,  1102,   611,  1103,
     828,  1117,  1146,   415,   612,  1178,   412,  1171,   521,   416,
     413,  1185,  1144,   414,  1198,  1205,   408,   409,  1209,  1646,
    1229,  1210,   680,   680,   680,  1211,  1212,   721,   415,   721,
    1228,   721,   407,   721,   416,   721,  1232,   721,  1241,   721,
    1242,   721,  1243,  1264,   353,   352,   721,  1268,   721,  1244,
    1273,  1248,  1249,  1259,   721,  1260,  1275,   352,  1284,   408,
     409,  1287,  1418,  1289,  1291,  1299,   721,  1307,   721,  1308,
    1309,  1558,  1320,   721,  1423,   721,  1319,   721,  1334,   410,
     721,  1381,  1396,   411,  1327,  1330,   352,   352,   352,  1346,
    1349,  1356,  1382,  1384,  1374,   828,  1383,  1385,  1397,  1399,
    1404,  1584,  1407,  1417,  1419,  1425,  1428,  1429,   407,  1453,
    1458,   352,   721,  1471,  1473,  1475,   521,  1476,  1480,  1482,
    1498,   828,   410,  1444,  1503,  1507,   411,  1504,   353,   645,
    1506,  1512,  1519,  1513,  1544,   408,   409,  1576,  1514,  1518,
    1521,  1469,   735,   736,   721,   412,  1522,  1538,  1589,   413,
    1607,   548,   414,  1633,  1560,  1572,  1488,  1577,  1579,  1484,
    1580,   569,  1606,  1581,  1639,  1628,  1629,   415,  1632,  1634,
     580,  1637,  1638,   416,  1641,  1642,  1651,  1654,  1656,   352,
     593,  1666,  1657,   521,  1662,   865,  1663,  1664,   412,  1670,
     603,  1669,   413,  1672,  1276,   414,  1671,   944,   410,   621,
    1678,   129,   411,  1679,    18,    83,   173,  1680,   132,   629,
     415,  1545,  1067,   521,  1548,  1206,   416,  1484,  1549,   647,
     353,   310,   353,  1550,   652,  1551,   654,  1552,    24,  1231,
    1525,  1481,   735,   736,  1380,   663,  1500,  1570,   747,   748,
    1501,  1409,   626,   675,   676,   677,   755,  1571,   757,   758,
     759,   760,    93,  1627,  1535,  1348,   761,  1565,   695,   633,
     793,   634,     0,   352,   412,   932,   699,     0,   413,   695,
    1301,   414,   706,   707,   708,   709,   710,   521,   521,   521,
     521,   521,   521,   521,   521,   719,   415,   704,     0,     0,
     521,   521,   416,     0,     0,     0,   521,     0,     0,     0,
       0,     0,     0,     0,     0,   521,   521,     0,   353,     0,
     521,   521,   521,     0,   521,     0,     0,     0,     0,   783,
     784,   785,     0,   353,     0,  1605,   745,   746,   747,   748,
     749,   786,   787,   752,   753,   754,   755,     0,   757,   758,
     759,   760,   521,     0,     0,     0,   761,     0,   763,   764,
       0,   804,     0,     0,   767,   352,   769,   352,     0,     0,
       0,     0,   990,   992,   994,   996,   998,  1000,  1002,     0,
     721,     0,     0,     0,  1007,  1009,     0,     0,     0,   831,
    1015,     0,     0,     0,     0,     0,     0,   353,   521,  1027,
    1029,     0,     0,     0,  1034,  1036,  1038,     0,  1041,     0,
       0,     0,   776,   777,   778,   779,   780,   781,   782,   783,
     784,   785,     0,     0,     0,     0,     0,     0,   647,     0,
       0,   786,   787,  1676,     0,   695,   888,     0,   889,     0,
       0,     0,     0,   894,     0,     0,     0,     0,     0,   898,
       0,     0,     0,   352,     0,     0,     0,     0,   907,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   352,   910,
     911,   912,   913,   914,   915,     0,   923,     0,   923,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1371,     0,     0,     0,     0,     0,     0,     0,
       0,   735,   736,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   957,
       0,     0,   989,   991,   993,   995,   997,   999,  1001,  1003,
    1004,  1005,   352,     0,  1006,  1008,  1010,  1011,  1012,  1013,
       0,  1016,  1017,     0,  1020,  1021,  1022,  1024,  1025,  1026,
    1028,  1030,  1031,  1032,  1033,  1035,  1037,  1039,  1040,  1042,
    1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,
    1054,     0,  1062,     0,     0,     0,  1065,   957,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1074,     0,     0,
       0,     0,     0,     0,     0,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,  1101,     0,     0,   773,
       0,     0,     0,  1106,     0,     0,     0,     0,     0,     0,
    1116,     0,  1118,     0,     0,     0,     0,     0,     0,     0,
       0,  1125,  1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,
    1134,  1135,  1136,  1137,  1138,  1139,  1140,  1141,  1142,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,   407,     0,     0,     0,
     786,   787,     0,     0,     0,     0,   407,     0,     0,     0,
       0,     0,     0,     0,     0,   706,  1175,     0,     0,     0,
       0,     0,     0,   408,   409,     0,     0,     0,     0,  1197,
       0,   695,     0,   408,   409,     0,     0,     0,  1202,     0,
       0,   695,     0,     0,   431,     0,  1125,     0,   432,   433,
       3,     0,   434,   435,   436,  1220,   437,     0,   438,   439,
     440,   441,   442,     0,     0,     0,   521,     0,   443,   444,
     445,   446,   447,     0,   448,     0,     0,     0,     0,     0,
       0,   449,   450,     0,     0,   451,   410,   452,   453,     0,
     411,   454,     0,     8,   455,   456,   410,   457,   458,     0,
     411,   459,   460,     0,     0,     0,     0,     0,   461,     0,
       0,   462,   463,     0,   317,   318,   319,     0,   321,   322,
     323,   324,   325,   464,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,     0,   339,   340,   341,     0,
       0,   344,   345,   346,   347,   465,   466,   467,   468,     0,
       0,  1258,   412,     0,     0,  1263,   413,     0,  1302,   414,
     469,   470,   412,     0,     0,     0,   413,     0,  1318,   414,
       0,     0,     0,     0,   415,   471,   472,   473,     0,     0,
     416,     0,     0,     0,   415,    58,     0,     0,     0,     0,
     416,     0,     0,   474,   475,   476,   477,   478,     0,   479,
       0,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,    59,   491,   492,   493,     0,     0,     0,     0,
       0,     0,   494,   495,   496,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   497,
     498,   499,     0,    14,  1322,     0,   500,   501,     0,     0,
       0,   407,     0,     0,   502,     0,   503,     0,   504,   505,
       0,     0,     0,   -68,     0,     0,  1336,  1337,  1338,     0,
       0,     0,  1341,     0,   735,   736,     0,     0,   408,   409,
    1347,   923,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   548,     0,     0,     0,     0,  1365,  1366,     0,     0,
       0,     0,     0,  1370,     0,     0,     0,     0,     0,     0,
       0,  1125,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1390,     0,  1392,
       0,   410,     0,     0,  1395,   411,     0,     0,     0,     0,
    1398,     0,     0,     0,  1401,     0,     0,     0,     0,     0,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,   760,     0,     0,     0,     0,   761,   762,
     763,   764,   765,   766,     0,  1420,   767,   768,   769,   770,
     771,   772,   773,     0,     0,     0,     0,   412,     0,     0,
       0,   413,     0,  1323,   414,   695,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   415,
       0,     0,     0,     0,     0,   416,     0,     0,     0,     0,
       0,   774,   775,     0,   776,   777,   778,   779,   780,   781,
     782,   783,   784,   785,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   786,   787,     0,     0,     0,   504,   678,
       0,     0,     0,   735,   736,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1493,     0,  1494,     0,     0,     0,     0,  1497,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   735,   736,     0,     0,     0,     0,     0,     0,     0,
       0,  1505,     0,     0,  1508,     0,     0,     0,     0,     0,
       0,     0,  1515,  1516,  1517,     0,     0,     0,     0,  1524,
       0,     0,     0,     0,  1527,     0,     0,     0,  1531,  1532,
    1533,  1534,     0,     0,   695,  1537,     0,   695,  1541,     0,
       0,     0,     0,     0,     0,  1553,     0,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,  1562,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,     0,     0,     0,   745,   746,   747,   748,   695,
       0,     0,     0,     0,     0,   755,     0,   757,   758,   759,
     760,     0,     0,  1591,  1592,   761,     0,   763,   764,     0,
       0,     0,     0,     0,     0,     0,     0,  1599,     0,     0,
       0,   775,  1604,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,  1610,     0,  1611,     0,     0,     0,
       0,     0,   786,   787,     0,     0,   407,   504,   678,     0,
       0,  1623,  1624,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   781,   782,   783,   784,
     785,     0,     0,   408,   409,     0,     0,     0,  1645,     0,
     786,   787,     0,  1647,  1648,     0,     0,     0,   431,     0,
       0,     0,   432,   433,     3,     0,   434,   435,   436,     0,
     437,     0,   438,   439,   440,   441,   442,     0,  1665,     0,
    1667,  1668,   443,   444,   445,   446,   447,     0,   448,     0,
       0,  1673,     0,     0,     0,   449,   450,     0,     0,   451,
       0,   452,   453,     0,     0,   454,   410,     8,   455,   456,
     411,   457,   458,     0,     0,   459,   460,     0,     0,     0,
       0,     0,   461,     0,     0,   462,   463,     0,   317,   318,
     319,     0,   321,   322,   323,   324,   325,   464,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,     0,
     339,   340,   341,     0,     0,   344,   345,   346,   347,   465,
     466,   467,   468,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   412,     0,   469,   470,   413,     0,  1324,   414,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   471,
     472,   473,     0,     0,   415,     0,     0,     0,     0,    58,
     416,     0,     0,     0,     0,     0,     0,   474,   475,   476,
     477,   478,   407,   479,     0,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,    59,   491,   492,   493,
       0,     0,     0,     0,     0,     0,   494,  1362,   496,   408,
     409,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   497,   498,   499,     0,    14,     0,     0,
     500,   501,     0,     0,   432,   433,     0,     0,   502,     0,
     503,     0,   504,   505,   438,   439,   440,   441,   442,     0,
       0,     0,     0,     0,   443,     0,   445,     0,     0,     0,
     448,     0,   407,     0,     0,     0,     0,     0,   450,     0,
       0,     0,   410,     0,   453,     0,   411,   454,     0,     0,
     455,     0,   917,     0,   458,     0,     0,     0,     0,   408,
     409,     0,     0,     0,   565,     0,     0,   462,   463,     0,
     317,   318,   319,     0,   321,   322,   323,   324,   325,   464,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,     0,   339,   340,   341,     0,     0,   344,   345,   346,
     347,   465,   466,   566,     0,     0,     0,     0,   412,     0,
       0,     0,   413,     0,  1325,   414,   469,   470,     0,     0,
       0,     0,   410,     0,     0,     0,   411,     0,     0,     0,
     415,     0,     0,     0,     0,     0,   416,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   474,
     475,   476,   477,   478,   407,   479,   918,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   919,   567,
     492,   493,     0,     0,     0,     0,     0,     0,   494,     0,
       0,   408,   409,     0,     0,     0,     0,     0,   412,     0,
       0,     0,   413,     0,     0,   920,   498,   499,     0,    14,
       0,     0,   500,   501,     0,     0,     0,   432,   433,     0,
     921,     0,   922,     0,   504,   505,   416,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,     0,   448,     0,   407,     0,     0,     0,     0,
       0,   450,     0,     0,   410,     0,     0,   453,   411,     0,
     454,     0,     0,   455,     0,     0,     0,   458,     0,     0,
       0,     0,   408,   409,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,     0,     0,     0,
     412,     0,     0,     0,   413,     0,  1326,   414,     0,   469,
     470,     0,     0,     0,     0,   410,     0,     0,     0,   411,
       0,     0,   415,     0,     0,     0,     0,     0,   416,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,   407,   479,   918,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   919,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,   408,   409,     0,     0,     0,     0,
       0,   412,     0,     0,     0,   413,     0,     0,   920,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,     0,
     432,   433,     0,   921,     0,   930,     0,   504,   505,   416,
     438,   439,   440,   441,   442,     0,     0,     0,     0,     0,
     443,     0,   445,     0,     0,     0,   448,     0,   598,     0,
       0,     0,     0,     0,   450,     0,     0,   410,     0,     0,
     453,   411,     0,   454,     0,     0,   455,     0,     0,     0,
     458,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     565,     0,     0,   462,   463,     0,   317,   318,   319,     0,
     321,   322,   323,   324,   325,   464,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,     0,   339,   340,
     341,     0,     0,   344,   345,   346,   347,   465,   466,   566,
       0,     0,     0,   412,     0,     0,     0,   413,     0,  1328,
     414,     0,   469,   470,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   415,     0,     0,     0,     0,
       0,   416,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   474,   475,   476,   477,   478,
     407,   479,     0,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,    59,   567,   492,   493,     0,     0,
       0,     0,     0,     0,   494,     0,     0,   408,   409,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   599,     0,
       0,   497,   498,   499,     0,    14,     0,     0,   500,   501,
       0,     0,     0,   432,   433,     0,  1174,     0,   503,     0,
     504,   505,   601,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,     0,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
     410,     0,     0,   453,   411,     0,   454,     0,     0,   455,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   467,     0,     0,     0,   412,     0,     0,     0,
     413,     0,  1329,   414,     0,   469,   470,     0,     0,     0,
       0,     0,     0,     0,   643,     0,     0,     0,   415,     0,
     471,   472,   473,     0,   416,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,   407,   479,     0,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
     408,   409,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,   432,   433,     0,     0,   502,
       0,   503,     0,   504,   505,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,   410,     0,   453,     0,   411,   454,     0,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   467,     0,     0,     0,     0,   412,
       0,     0,     0,   413,     0,  1331,   414,   469,   470,     0,
       0,     0,     0,     0,     0,     0,   673,     0,     0,     0,
       0,   415,   471,   472,   473,     0,     0,   416,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,   407,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,   408,   409,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,   432,   433,     0,
       0,   502,     0,   503,     0,   504,   505,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,  1546,   445,
     446,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,   410,     0,   453,     0,   411,
     454,     0,     0,   455,   456,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,  1547,     0,     0,
       0,   412,     0,     0,     0,   413,     0,  1431,   414,   469,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   415,     0,     0,     0,     0,     0,   416,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,   407,   479,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,   408,   409,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,   432,
     433,     0,     0,   502,     0,   503,     0,   504,   505,   438,
     439,   440,   441,   442,     0,     0,     0,     0,     0,   443,
       0,   445,     0,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,     0,   410,     0,   453,
       0,   411,   454,     0,     0,   455,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   467,     0,
       0,     0,     0,   412,     0,     0,     0,   413,     0,  1432,
     414,   469,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   471,   472,   473,     0,
       0,   416,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,   407,
     479,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,    59,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,   408,   409,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,   432,   433,     0,     0,   502,     0,   503,     0,   504,
     505,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,   410,
       0,   453,     0,   411,   454,     0,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,   962,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,   412,     0,     0,     0,   413,
       0,  1433,   414,   469,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   415,     0,     0,
       0,     0,     0,   416,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,   407,   479,   918,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   919,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,   408,   409,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,   432,   433,     0,     0,   963,     0,   503,
     964,   504,   505,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,   407,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,   410,     0,   453,     0,   411,   454,     0,     0,   455,
       0,     0,     0,   458,     0,   408,   409,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   467,     0,     0,     0,     0,   412,     0,     0,
       0,   413,     0,  1436,   414,   469,   470,     0,   410,     0,
       0,     0,   411,     0,     0,     0,     0,     0,     0,   415,
     984,   985,   986,     0,     0,   416,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,   407,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,   408,   409,   412,     0,     0,     0,   413,     0,
    1467,   414,     0,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,     0,   415,   432,   433,   502,
       0,   503,   416,   504,   505,   711,     0,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,   410,     0,   453,     0,   411,
     454,   712,     0,   455,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,     0,     0,     0,
       0,   412,     0,     0,     0,   413,     0,  1563,   414,   469,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   415,     0,     0,     0,     0,     0,   416,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,     0,   479,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,     0,
       0,   432,   433,   502,   602,   503,     0,   504,   505,   711,
       0,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,     0,
       0,   453,     0,     0,   454,   712,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,     0,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,     0,   479,   918,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   919,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,     0,     0,   432,   433,   502,     0,   503,
       0,   504,   505,   711,     0,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,   453,     0,     0,   454,   712,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,     0,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,     0,     0,   432,
     433,   502,   829,   503,     0,   504,   505,   711,     0,   438,
     439,   440,   441,   442,     0,     0,     0,     0,     0,   443,
       0,   445,     0,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,     0,     0,     0,   453,
       0,     0,   454,   712,     0,   455,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,     0,
     479,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,    59,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,   432,   433,     0,     0,   502,     0,   503,     0,   504,
     505,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,     0,
       0,   453,     0,     0,   454,     0,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,  1111,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,     0,   479,   918,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   919,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,   432,   433,     0,     0,   502,     0,   503,
       0,   504,   505,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,     0,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,     0,     0,   453,     0,     0,   454,     0,     0,   455,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,     0,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,   432,   433,     0,     0,   502,
     602,   503,     0,   504,   505,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,   453,     0,     0,   454,     0,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,   651,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,     0,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,   432,   433,     0,
       0,   502,     0,   503,     0,   504,   505,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,     0,     0,   453,     0,     0,
     454,     0,     0,   455,     0,     0,     0,   458,     0,     0,
     662,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,     0,   479,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,     0,
       0,   432,   433,   502,     0,   503,     0,   504,   505,   694,
       0,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,     0,
       0,   453,     0,     0,   454,     0,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,     0,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,     0,   479,     0,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,    59,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,   432,   433,     0,     0,   502,     0,   503,
       0,   504,   505,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,     0,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,     0,     0,   453,     0,     0,   454,     0,     0,   455,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,     0,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   698,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,     0,     0,   432,   433,   502,
       0,   503,     0,   504,   505,   702,     0,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,     0,     0,   453,     0,     0,
     454,     0,     0,   455,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,     0,   479,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,   432,
     433,     0,     0,   502,     0,   503,     0,   504,   505,   438,
     439,   440,   441,   442,     0,     0,  1043,     0,     0,   443,
       0,   445,     0,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,     0,     0,     0,   453,
       0,     0,   454,     0,     0,   455,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,     0,
     479,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,    59,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,   432,   433,     0,     0,   502,     0,   503,     0,   504,
     505,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,     0,
       0,   453,     0,     0,   454,     0,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,     0,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,     0,   479,     0,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,    59,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,   432,   433,     0,     0,   502,     0,   503,
    1063,   504,   505,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,     0,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,     0,     0,   453,     0,     0,   454,     0,     0,   455,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,     0,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1115,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,   432,   433,     0,     0,   502,
       0,   503,     0,   504,   505,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,   453,     0,     0,   454,     0,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,     0,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,   432,   433,     0,
       0,   502,     0,   503,  1176,   504,   505,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,     0,     0,   453,     0,     0,
     454,     0,     0,   455,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,     0,   479,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,   432,
     433,     0,     0,   502,     0,   503,  1191,   504,   505,   438,
     439,   440,   441,   442,     0,     0,     0,     0,     0,   443,
       0,   445,     0,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,     0,     0,     0,   453,
       0,     0,   454,     0,     0,   455,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,     0,
     479,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,    59,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,   432,   433,     0,     0,   502,     0,   503,  1393,   504,
     505,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,     0,
       0,   453,     0,     0,   454,     0,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,     0,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,     0,   479,     0,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,    59,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,   432,   433,     0,     0,   502,     0,   503,
    1402,   504,   505,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,     0,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,     0,     0,   453,     0,     0,   454,     0,     0,   455,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,     0,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,   432,   433,     0,     0,   502,
       0,   503,  1447,   504,   505,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,   453,     0,     0,   454,     0,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,     0,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,   432,   433,     0,
       0,   502,     0,   503,  1523,   504,   505,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,     0,     0,   453,     0,     0,
     454,     0,     0,   455,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,     0,     0,     0,     0,     0,     0,     0,  1561,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,     0,   479,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,   432,
     433,     0,     0,   502,     0,   503,     0,   504,   505,   438,
     439,   440,   441,   442,     0,     0,     0,     0,     0,   443,
       0,   445,     0,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,     0,     0,     0,   453,
       0,     0,   454,     0,     0,   455,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,     0,     0,     0,     0,     0,     0,     0,
    1602,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,     0,
     479,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,    59,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,   432,   433,     0,     0,   502,     0,   503,     0,   504,
     505,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,     0,
       0,   453,     0,     0,   454,     0,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,     0,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,     0,     0,     0,     0,     0,
       0,     0,  1603,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,     0,   479,     0,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,    59,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,   432,   433,     0,     0,   502,     0,   503,
       0,   504,   505,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,     0,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,     0,     0,   453,     0,     0,   454,     0,     0,   455,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,     0,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,   432,   433,     0,     0,   502,
       0,   503,     0,   504,   505,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,   453,     0,     0,   454,     0,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,     0,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,   690,     0,     0,     0,     0,   494,
     311,     0,     0,     0,     0,     0,   312,     0,     0,     0,
       0,     0,   313,     0,     0,     0,   497,   498,   499,     0,
      14,     0,   314,   500,   501,     0,     0,     0,     0,     0,
     315,  1376,     0,   503,     0,   504,   505,     0,     0,     0,
       0,     0,     0,     0,     0,   316,     0,     0,     0,     0,
       0,     0,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,     0,     0,     0,     0,     0,
       0,     0,     0,   692,     0,     0,     0,     0,     0,   311,
       0,     0,     0,     0,     0,   312,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   314,     0,    58,     0,     0,     0,     0,     0,   315,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,   316,     0,     0,     0,     0,     0,
      59,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,     0,   311,     0,
       0,     0,     0,     0,   312,     0,     0,     0,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     314,     0,    58,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,     0,   350,     0,     0,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,    59,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,     0,   311,     0,     0,
       0,     0,     0,   312,     0,     0,     0,     0,     0,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   314,
       0,    58,     0,     0,     0,     0,     0,   315,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,    59,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,     0,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,     0,     0,     0,     0,     0,     0,
       0,     0,   311,     0,     0,     0,     0,   610,   312,     0,
       0,     0,     0,     0,   313,     0,     0,     0,     0,   611,
       0,     0,     0,     0,   314,   612,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,   351,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   314,     0,    58,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,   316,     0,   735,   736,
       0,     0,    59,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   735,   736,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   350,     0,     0,
       0,     0,   745,   746,   747,   748,   749,     0,     0,   752,
       0,   610,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
       0,     0,   867,   868,   869,   870,   871,   872,   873,   874,
     745,   746,   747,   748,   749,   875,   876,   752,   753,   754,
     755,   877,   757,   758,   759,   760,  -361,   351,   735,   736,
     761,   762,   763,   764,   878,   879,     0,     0,   767,   768,
     769,   880,   881,   882,   773,     0,     0,     0,     0,     0,
       0,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   883,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,     0,
     504,   678,     0,     0,   867,   868,   869,   870,   871,   872,
     873,   874,   745,   746,   747,   748,   749,   875,   876,   752,
     753,   754,   755,   877,   757,   758,   759,   760,   735,   736,
       0,     0,   761,   762,   763,   764,   878,   879,     0,     0,
     767,   768,   769,   880,   881,   882,   773,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   735,   736,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1075,     0,
       0,     0,     0,     0,     0,   883,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,     0,   504,   678,   867,   868,   869,   870,   871,   872,
     873,   874,   745,   746,   747,   748,   749,   875,   876,   752,
     753,   754,   755,   877,   757,   758,   759,   760,     0,     0,
       0,     0,   761,   762,   763,   764,   878,   879,     0,     0,
     767,   768,   769,   880,   881,   882,   773,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,   735,   736,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,  1277,     0,
       0,   773,     0,     0,     0,   883,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   735,   736,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,     0,   504,   678,     0,     0,     0,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,   799,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,   735,   736,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   735,   736,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,   809,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,   825,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,   735,   736,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   735,   736,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,   840,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1148,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,   735,   736,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   735,   736,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1152,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1154,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,   735,   736,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   735,   736,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1163,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1164,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,   735,   736,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   735,   736,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1165,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,   735,   736,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   735,   736,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1167,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,   735,   736,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   735,   736,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1298,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,   735,   736,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   735,   736,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1313,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,   735,   736,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   735,   736,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1465,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,   735,   736,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   735,   736,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,   735,   736,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   735,   736,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1491,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,   735,   736,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   735,   736,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1566,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1567,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,   735,   736,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   735,   736,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1568,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1575,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,   735,   736,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,   735,   736,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1578,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1588,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,   735,   736,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   735,   736,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,  1593,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1658,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,     0,     0,     0,   773,   745,   746,   747,
     748,   749,   735,   736,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,  1659,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,   787,   830,     0,   745,   746,   747,   748,
     749,   735,   736,   752,   753,   754,   755,     0,   757,   758,
     759,   760,     0,     0,     0,     0,   761,     0,   763,   764,
       0,     0,     0,     0,   767,   768,   769,     0,     0,     0,
     773,     0,     0,     0,     0,     0,   735,   736,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     775,     0,   776,   777,   778,   779,   780,   781,   782,   783,
     784,   785,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   786,   787,  1098,     0,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
     745,   746,   747,   748,   749,   735,   736,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,  1250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   976,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,  1265,     0,   745,
     746,   747,   748,   749,   735,   736,   752,   753,   754,   755,
       0,   757,   758,   759,   760,     0,     0,     0,     0,   761,
       0,   763,   764,     0,     0,     0,     0,   767,   768,   769,
       0,     0,     0,   773,     0,     0,     0,     0,   317,   318,
     319,     0,   321,   322,   323,   324,   325,   464,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,     0,
     339,   340,   341,     0,     0,   344,   345,   346,   347,     0,
       0,     0,     0,   775,     0,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,   787,  1400,     0,   745,   746,
     747,   748,   749,     0,     0,   752,   753,   754,   755,     0,
     757,   758,   759,   760,   231,   232,     0,     0,   761,     0,
     763,   764,     0,     0,   977,     0,   767,   768,   769,     0,
       0,   233,   773,     0,     0,     0,   978,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   735,
     736,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   775,     0,   776,   777,   778,   779,   780,   781,
     782,   783,   784,   785,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   786,   787,  1405,     0,     0,     0,     0,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,     0,
     252,   253,   254,     0,     0,     0,     0,     0,     0,   255,
     256,   257,   258,   259,     0,     0,   260,   261,   262,   263,
     264,   265,   266,   745,   746,   747,   748,   749,   735,   736,
     752,   753,   754,   755,     0,   757,   758,   759,   760,     0,
       0,     0,     0,   761,     0,   763,   764,     0,     0,     0,
       0,   767,   768,   769,     0,     0,     0,   773,     0,     0,
       0,     0,   267,     0,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,     0,     0,   278,   279,     0,     0,
       0,     0,     0,   280,   281,     0,     0,   286,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   775,     0,   776,
     777,   778,   779,   780,   781,   782,   783,   784,   785,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   786,   787,
       0,     0,   745,   746,   747,   748,   749,   735,   736,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,   934,     0,
     767,   768,   769,     0,     0,     0,   773,     0,     0,     0,
       0,     0,   735,   736,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,   745,   746,   747,   748,   749,     0,     0,   752,   753,
     754,   755,     0,   757,   758,   759,   760,     0,     0,     0,
       0,   761,     0,   763,   764,     0,     0,  1177,     0,   767,
     768,   769,   735,   736,     0,   773,   745,   746,   747,   748,
     749,     0,     0,   752,   753,   754,   755,     0,   757,   758,
     759,   760,     0,     0,     0,     0,   761,     0,   763,   764,
       0,     0,     0,     0,   767,   768,   769,   735,   736,     0,
     773,     0,     0,     0,     0,   775,     0,   776,   777,   778,
     779,   780,   781,   782,   783,   784,   785,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   786,   787,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     775,  1255,   776,   777,   778,   779,   780,   781,   782,   783,
     784,   785,     0,     0,     0,     0,   745,   746,   747,   748,
     749,   786,   787,   752,   753,   754,   755,     0,   757,   758,
     759,   760,     0,     0,     0,     0,   761,     0,   763,   764,
       0,     0,     0,     0,   767,   768,   769,   735,   736,     0,
     773,   745,   746,   747,   748,   749,     0,     0,   752,   753,
     754,   755,     0,   757,   758,   759,   760,     0,     0,     0,
       0,   761,     0,   763,   764,     0,     0,     0,     0,   767,
     768,   769,  1267,     0,     0,   773,   735,   736,     0,     0,
     775,     0,   776,   777,   778,   779,   780,   781,   782,   783,
     784,   785,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   786,   787,  1360,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   775,     0,   776,   777,   778,
     779,   780,   781,   782,   783,   784,   785,     0,     0,     0,
       0,   745,   746,   747,   748,   749,   786,   787,   752,   753,
     754,   755,     0,   757,   758,   759,   760,     0,     0,     0,
       0,   761,     0,   763,   764,     0,     0,     0,     0,   767,
     768,   769,     0,     0,     0,   773,   735,   736,     0,     0,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,  1589,   767,   768,
     769,   735,   736,     0,   773,   775,     0,   776,   777,   778,
     779,   780,   781,   782,   783,   784,   785,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   786,   787,     0,     0,
       0,     0,     0,     0,     0,     0,  1649,     0,     0,     0,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
     745,   746,   747,   748,   749,   786,   787,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,   735,   736,     0,   773,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,  1674,     0,     0,   773,
     735,   736,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,     0,
       0,  1675,     0,     0,     0,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,   745,   746,   747,   748,   749,
     786,   787,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
     735,   736,     0,     0,   745,   746,   747,   748,   749,     0,
       0,   752,   753,   754,   755,     0,   757,   758,   759,   760,
       0,     0,     0,     0,   761,     0,   763,   764,     0,     0,
       0,  1681,   767,   768,   769,   735,   736,     0,   773,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   775,     0,
     776,   777,   778,   779,   780,   781,   782,   783,   784,   785,
       0,     0,     0,     0,   745,   746,   747,   748,   749,   786,
     787,   752,   753,   754,   755,     0,   757,   758,   759,   760,
       0,     0,     0,     0,   761,     0,   763,   764,     0,     0,
       0,     0,   767,   768,   769,   735,   736,     0,  -755,   745,
     746,   747,   748,   749,     0,     0,   752,   753,   754,   755,
       0,   757,   758,   759,   760,     0,     0,     0,     0,   761,
       0,   763,   764,     0,   735,   736,     0,   767,   768,   769,
       0,     0,     0,     0,     0,     0,     0,     0,   775,     0,
     776,   777,   778,   779,   780,   781,   782,   783,   784,   785,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
     787,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   775,     0,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,     0,     0,     0,     0,   745,
     746,   747,   748,   749,   786,   787,   752,   753,   754,   755,
       0,   757,   758,   759,   760,     0,     0,     0,     0,   761,
       0,   763,   764,     0,   735,   736,     0,   767,   745,   746,
     747,   748,   749,     0,     0,   752,   753,   754,   755,     0,
     757,   758,   759,   760,     0,     0,     0,     0,   761,     0,
     763,   764,   735,   736,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,   787,     0,     0,     0,     0,
       0,     0,     0,     0,   776,   777,   778,   779,   780,   781,
     782,   783,   784,   785,     0,     0,     0,     0,   745,   746,
     747,   748,   749,   786,   787,   752,   753,   754,   755,     0,
     757,   758,   759,   760,     0,     0,     0,     0,   761,     0,
     763,   764,   735,   736,     0,     0,   745,   746,   747,   748,
     749,     0,     0,   752,   753,   754,   755,     0,   757,   758,
     759,   760,     0,     0,     0,     0,   761,     0,   763,   764,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   777,   778,   779,   780,   781,
     782,   783,   784,   785,     0,     0,     0,     0,   980,     0,
       0,     0,     0,   786,   787,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   778,   779,   780,   781,   782,   783,
     784,   785,     0,     0,     0,     0,   745,   746,   747,   748,
     749,   786,   787,   752,   753,   754,   755,     0,   757,   758,
     759,   760,     0,     0,     0,     0,   761,     0,   763,   764,
     317,   318,   319,     0,   321,   322,   323,   324,   325,   464,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,     0,   339,   340,   341,     0,     0,   344,   345,   346,
     347,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1251,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   779,   780,   781,   782,   783,
     784,   785,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   786,   787,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   981,     0,     0,     0,
     181,     0,     0,     0,     0,   317,   318,   319,   982,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   182,     0,   183,     0,
     184,   185,   186,   187,   188,     0,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,     0,   200,   201,
     202,     0,     0,   203,   204,   205,   206,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   207,   208,     0,     0,     0,     0,     0,     0,
       0,  1252,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1253,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   209
};

static const yytype_int16 yycheck[] =
{
      14,    15,   211,   389,   572,   521,   483,   376,   376,   625,
     161,   592,   227,   713,   538,   715,   909,   420,   421,   591,
     586,   925,   588,   383,   590,  1180,   691,   931,   693,   472,
     473,   816,   418,   818,   693,   820,   854,   444,   562,   630,
      80,     5,     4,  1320,    20,     8,    33,   444,  1379,    63,
      64,    65,    53,  1441,    22,    15,    16,   103,    53,   724,
    1417,   468,   157,    33,    19,    20,   159,    57,   125,    21,
      22,   128,   129,    63,   169,    46,   444,    20,   227,   126,
     143,   144,   145,    20,    20,    34,     7,    20,   162,    57,
     104,   105,   106,   107,    20,   481,   482,   142,   126,   690,
     468,   692,     0,   694,   179,   126,   503,    20,   136,     7,
    1569,   702,   128,   129,    63,  1503,  1504,   169,   169,   214,
     711,   131,   154,   216,   169,    21,    22,   159,   828,    50,
      15,    16,    30,  1521,    32,   106,    34,   126,   102,   103,
     187,   188,    40,   190,   962,  1476,   193,   136,   142,   188,
     301,   214,    50,   210,   211,  1512,   680,  1616,    56,   374,
     188,   213,   157,   863,   215,   117,   118,   188,   213,   576,
     191,   181,   218,   125,   169,   127,   128,   129,   130,   219,
     181,  1538,    80,   135,   216,  1340,   135,   887,   181,   854,
     180,  1579,  1580,   142,   210,   211,   190,   159,   595,   188,
     900,    49,   162,   903,   102,   103,   193,   167,   576,   169,
     613,   187,   172,   227,   169,   618,   126,   180,   126,   154,
     169,   117,   118,   193,   159,   374,   136,  1543,   136,   125,
     194,   199,   128,   129,   130,   142,   643,  1514,   447,   135,
     389,   211,   210,    91,   187,   642,   643,   217,   211,   213,
     187,   187,   461,   213,   187,   204,    47,   126,   210,   211,
      62,   187,   169,   649,   213,   305,   673,   136,  1161,   418,
     667,   420,  1176,   670,   187,   643,    67,   162,   188,   214,
     188,   216,   167,   181,   169,  1601,   672,   172,   179,   157,
     733,   151,   181,  1111,   128,   129,  1451,   962,   963,   159,
     169,   169,   200,   141,  1534,   673,  1536,   181,   187,   169,
    1540,  1541,   179,   211,   210,   211,   126,   187,   190,   188,
     126,   194,   213,   157,   193,   214,   136,   724,  1221,   142,
     136,   190,   481,   482,   193,   169,   142,   351,   151,  1574,
     213,   179,   216,   156,   214,   213,   142,   187,   215,   216,
    1585,   184,   185,   213,   181,   151,   169,   169,   181,   950,
     374,   184,   577,   578,   579,   379,   581,   582,   195,  1599,
     585,   731,   587,   169,   589,   389,   210,   211,   188,   212,
     187,   194,   188,  1276,   169,   170,   171,   187,   187,   212,
    1625,  1626,   789,     5,     6,   802,     8,   187,   187,   194,
     190,   173,   174,   193,   418,   808,   420,   214,   974,    63,
      64,    65,   798,   212,   214,   200,   187,   187,   213,   822,
     629,   824,   187,   142,    36,   214,   440,   441,   577,   578,
     579,   187,   581,   582,   802,  1339,   585,   961,   587,   187,
     589,   848,   591,   214,   214,   654,  1111,  1351,   187,   214,
     104,   105,   106,   107,   187,  1240,   187,   854,   214,   936,
      12,   187,   187,   849,   850,   162,   214,   481,   482,   946,
     187,    23,    24,    75,   917,   214,   862,    79,   187,   187,
     848,   214,   885,   214,    21,    22,   212,   212,  1104,   194,
     699,    93,    94,   215,   216,   212,    98,    99,   100,   101,
     649,   194,   187,   212,   212,   194,   892,   893,   213,   895,
     719,  1176,  1093,   899,  1179,   901,   902,  1176,   904,  1091,
     213,   181,   213,   672,   213,   922,  1191,   212,   188,   471,
     472,   473,  1191,   930,   190,   190,   151,   169,   151,   195,
     195,   984,   985,   986,   210,   181,   190,   190,  1333,   181,
     169,   195,   195,   179,   169,   187,   169,   213,   213,   195,
     181,   187,   181,   577,   578,   579,   187,   581,   582,   213,
     213,   585,   169,   587,   195,   589,    57,   591,   181,   521,
     117,   118,    63,  1368,   181,   157,   735,   736,   125,   181,
     127,   128,   129,   130,   181,   187,  1162,   169,   135,   190,
     187,   181,   193,   195,   181,    33,   548,   187,   195,   169,
     187,   826,   761,   157,   181,   195,   169,   157,   195,   834,
     187,   635,   837,   179,   157,   169,   191,   157,   195,   169,
     195,   846,    60,    61,   190,   649,   169,   169,   853,   169,
     939,   180,   941,   858,   183,   181,   797,   186,  1214,   798,
      57,   187,   181,   191,   169,   191,    63,   195,   672,   196,
     197,   198,   199,   200,    57,    57,    57,   143,   144,   145,
      63,    63,    63,   210,   211,   884,    57,   826,   181,   169,
      57,   173,    63,   181,   187,   834,    63,  1472,   837,   187,
     181,   173,   174,   175,   176,   123,   187,   846,   126,   127,
     849,   850,   716,   191,   853,   647,    57,   195,   136,   858,
     181,  1376,  1109,   862,   181,   181,   187,   181,   191,   169,
     187,   187,   195,   187,   939,   181,   941,  1343,   191,   180,
     181,   182,   195,   675,  1120,  1121,  1122,   191,   191,    33,
     169,   195,   195,   892,   893,   191,   895,   191,   179,   195,
     899,   195,   901,   902,   191,   904,   173,   174,   195,  1145,
     188,   189,   173,   174,   175,   193,    60,    61,   196,   191,
      86,    87,  1169,   195,   169,   170,   171,     5,     6,   180,
     181,   795,   183,   211,   798,    10,    11,    12,   173,   217,
     939,   733,   941,  1190,   143,   143,   145,    25,   213,  1196,
     215,   216,   106,    31,   188,   179,  1203,  1373,   179,   179,
    1207,    35,   826,    35,   213,   179,  1332,   169,   180,   169,
     834,    22,   169,   837,   169,   212,   169,  1213,    43,   123,
     212,   190,   846,   127,  1450,   849,   850,  1206,  1206,   853,
      68,    69,   179,   179,   858,    33,   190,   213,   862,   169,
     213,  1248,   194,   194,   194,   194,   194,  1522,   213,   194,
     194,   194,   804,  1260,  1079,   194,   194,   194,   191,   136,
      33,   169,    60,    61,   102,   103,   169,   169,   892,   893,
     212,   895,   213,   213,  1584,   899,   211,   901,   902,   194,
     904,   194,   194,   169,  1043,   189,  1596,    60,    61,   193,
     213,   195,   196,   194,   194,   213,  1309,   181,   194,   162,
    1478,   194,   194,   194,   142,    37,  1581,   211,   213,  1619,
     216,   194,   181,   217,   213,   939,   213,   941,   213,   213,
    1079,   159,   160,   161,   213,   123,   376,  1334,   126,   127,
     181,   169,  1091,    10,   181,   181,   181,   181,   136,   181,
      66,   179,  1617,   211,   180,   194,   213,   213,    43,   214,
     123,   194,   169,   169,   127,   907,   194,  1364,   212,   181,
     180,  1120,  1121,  1122,   190,   917,   180,  1593,   180,    43,
     194,   194,  1459,   211,   213,  1462,   213,   187,   131,  1557,
    1558,   194,    14,   181,   188,   190,  1145,   162,   216,   179,
     188,   189,   187,  1389,   444,   193,  1215,    13,   196,    33,
     187,   169,     8,  1678,   169,   214,   195,   169,   181,   169,
    1417,   169,   214,   211,   187,   187,   189,   213,   468,   217,
     193,   187,   195,   196,   180,   180,    60,    61,   212,  1607,
     169,   214,   984,   985,   986,   213,   213,   989,   211,   991,
     214,   993,    33,   995,   217,   997,   213,   999,   194,  1001,
     213,  1003,   194,     1,  1213,  1079,  1008,   180,  1010,   213,
      67,   213,   213,   213,  1016,   213,   181,  1091,   169,    60,
      61,   195,  1297,   195,   195,    43,  1028,   214,  1030,   214,
     169,  1477,   213,  1035,  1303,  1037,   214,  1039,   213,   123,
    1042,   169,   194,   127,   214,   214,  1120,  1121,  1122,   212,
     212,   212,   169,   169,   214,  1512,   213,   169,   213,   213,
     213,  1518,   181,   213,   169,   214,   169,   213,    33,   213,
     212,  1145,  1074,   181,   214,   169,   576,   212,   169,   212,
      33,  1538,   123,   213,   213,   195,   127,   213,  1297,  1546,
     169,   213,    70,   214,   195,    60,    61,   195,   213,   213,
     213,  1370,    21,    22,  1106,   189,   213,   213,   181,   193,
    1556,   376,   196,    53,   214,   214,  1391,   213,   213,  1388,
     213,   386,   214,   213,   187,   214,   214,   211,   214,   212,
     395,   180,   180,   217,   180,   212,   181,   187,   214,  1213,
     405,   212,   214,   643,   214,  1602,   214,   214,   189,   180,
     415,   212,   193,   212,   195,   196,   214,   701,   123,   424,
     213,    79,   127,   214,     1,    44,   133,   214,    82,   434,
     211,  1469,   795,   673,  1470,   948,   217,  1446,  1470,   444,
    1389,   225,  1391,  1470,   449,  1470,   451,  1470,     1,   969,
    1446,  1386,    21,    22,  1232,   460,  1409,  1500,   117,   118,
    1412,  1272,   428,   468,   469,   470,   125,  1501,   127,   128,
     129,   130,    52,  1581,  1457,  1186,   135,  1492,   483,   440,
     560,   440,    -1,  1297,   189,   693,   491,    -1,   193,   494,
     195,   196,   497,   498,   499,   500,   501,   737,   738,   739,
     740,   741,   742,   743,   744,   510,   211,   494,    -1,    -1,
     750,   751,   217,    -1,    -1,    -1,   756,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   765,   766,    -1,  1477,    -1,
     770,   771,   772,    -1,   774,    -1,    -1,    -1,    -1,   198,
     199,   200,    -1,  1492,    -1,  1554,   115,   116,   117,   118,
     119,   210,   211,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   802,    -1,    -1,    -1,   135,    -1,   137,   138,
      -1,   576,    -1,    -1,   143,  1389,   145,  1391,    -1,    -1,
      -1,    -1,   738,   739,   740,   741,   742,   743,   744,    -1,
    1332,    -1,    -1,    -1,   750,   751,    -1,    -1,    -1,   604,
     756,    -1,    -1,    -1,    -1,    -1,    -1,  1556,   848,   765,
     766,    -1,    -1,    -1,   770,   771,   772,    -1,   774,    -1,
      -1,    -1,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,    -1,    -1,    -1,    -1,    -1,   643,    -1,
      -1,   210,   211,  1652,    -1,   650,   651,    -1,   653,    -1,
      -1,    -1,    -1,   658,    -1,    -1,    -1,    -1,    -1,   664,
      -1,    -1,    -1,  1477,    -1,    -1,    -1,    -1,   673,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1492,   684,
     685,   686,   687,   688,   689,    -1,   691,    -1,   693,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1543,
      -1,    -1,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,  1556,    -1,   749,   750,   751,   752,   753,   754,
      -1,   756,   757,    -1,   759,   760,   761,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,    -1,   787,    -1,    -1,    -1,   791,  1601,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   802,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,    -1,
      -1,    -1,    -1,   143,   144,   145,   841,    -1,    -1,   149,
      -1,    -1,    -1,   848,    -1,    -1,    -1,    -1,    -1,    -1,
     855,    -1,   857,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   866,   867,   868,   869,   870,   871,   872,   873,   874,
     875,   876,   877,   878,   879,   880,   881,   882,   883,   189,
      -1,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
     210,   211,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   920,   921,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,   934,
      -1,   936,    -1,    60,    61,    -1,    -1,    -1,   943,    -1,
      -1,   946,    -1,    -1,     1,    -1,   951,    -1,     5,     6,
       7,    -1,     9,    10,    11,   960,    13,    -1,    15,    16,
      17,    18,    19,    -1,    -1,    -1,  1206,    -1,    25,    26,
      27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    39,    -1,    -1,    42,   123,    44,    45,    -1,
     127,    48,    -1,    50,    51,    52,   123,    54,    55,    -1,
     127,    58,    59,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,  1056,   189,    -1,    -1,  1060,   193,    -1,   195,   196,
     117,   118,   189,    -1,    -1,    -1,   193,    -1,   195,   196,
      -1,    -1,    -1,    -1,   211,   132,   133,   134,    -1,    -1,
     217,    -1,    -1,    -1,   211,   142,    -1,    -1,    -1,    -1,
     217,    -1,    -1,   150,   151,   152,   153,   154,    -1,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,    -1,    -1,    -1,    -1,
      -1,    -1,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,
     197,   198,    -1,   200,  1149,    -1,   203,   204,    -1,    -1,
      -1,    33,    -1,    -1,   211,    -1,   213,    -1,   215,   216,
      -1,    -1,    -1,    10,    -1,    -1,  1171,  1172,  1173,    -1,
      -1,    -1,  1177,    -1,    21,    22,    -1,    -1,    60,    61,
    1185,  1186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1206,    -1,    -1,    -1,    -1,  1211,  1212,    -1,    -1,
      -1,    -1,    -1,  1218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1226,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1242,    -1,  1244,
      -1,   123,    -1,    -1,  1249,   127,    -1,    -1,    -1,    -1,
    1255,    -1,    -1,    -1,  1259,    -1,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,   136,
     137,   138,   139,   140,    -1,  1300,   143,   144,   145,   146,
     147,   148,   149,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,   193,    -1,   195,   196,  1320,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,   188,   189,    -1,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   210,   211,    -1,    -1,    -1,   215,   216,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1397,    -1,  1399,    -1,    -1,    -1,    -1,  1404,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1426,    -1,    -1,  1429,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1437,  1438,  1439,    -1,    -1,    -1,    -1,  1444,
      -1,    -1,    -1,    -1,  1449,    -1,    -1,    -1,  1453,  1454,
    1455,  1456,    -1,    -1,  1459,  1460,    -1,  1462,  1463,    -1,
      -1,    -1,    -1,    -1,    -1,  1470,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,  1483,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,    -1,    -1,    -1,   115,   116,   117,   118,  1514,
      -1,    -1,    -1,    -1,    -1,   125,    -1,   127,   128,   129,
     130,    -1,    -1,  1528,  1529,   135,    -1,   137,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1542,    -1,    -1,
      -1,   189,  1547,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,  1559,    -1,  1561,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,    33,   215,   216,    -1,
      -1,  1576,  1577,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   198,   199,
     200,    -1,    -1,    60,    61,    -1,    -1,    -1,  1603,    -1,
     210,   211,    -1,  1608,  1609,    -1,    -1,    -1,     1,    -1,
      -1,    -1,     5,     6,     7,    -1,     9,    10,    11,    -1,
      13,    -1,    15,    16,    17,    18,    19,    -1,  1633,    -1,
    1635,  1636,    25,    26,    27,    28,    29,    -1,    31,    -1,
      -1,  1646,    -1,    -1,    -1,    38,    39,    -1,    -1,    42,
      -1,    44,    45,    -1,    -1,    48,   123,    50,    51,    52,
     127,    54,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,   117,   118,   193,    -1,   195,   196,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,    -1,    -1,   211,    -1,    -1,    -1,    -1,   142,
     217,    -1,    -1,    -1,    -1,    -1,    -1,   150,   151,   152,
     153,   154,    33,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
      -1,    -1,    -1,    -1,    -1,    -1,   179,   180,   181,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,   198,    -1,   200,    -1,    -1,
     203,   204,    -1,    -1,     5,     6,    -1,    -1,   211,    -1,
     213,    -1,   215,   216,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,   123,    -1,    45,    -1,   127,    48,    -1,    -1,
      51,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,
      61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,   193,    -1,   195,   196,   117,   118,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,   127,    -1,    -1,    -1,
     211,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
     151,   152,   153,   154,    33,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,   193,    -1,    -1,   196,   197,   198,    -1,   200,
      -1,    -1,   203,   204,    -1,    -1,    -1,     5,     6,    -1,
     211,    -1,   213,    -1,   215,   216,   217,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,   123,    -1,    -1,    45,   127,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    60,    61,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
     189,    -1,    -1,    -1,   193,    -1,   195,   196,    -1,   117,
     118,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,   127,
      -1,    -1,   211,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,   151,   152,   153,   154,    33,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    -1,    -1,   193,    -1,    -1,   196,   197,
     198,    -1,   200,    -1,    -1,   203,   204,    -1,    -1,    -1,
       5,     6,    -1,   211,    -1,   213,    -1,   215,   216,   217,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,   123,    -1,    -1,
      45,   127,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,   189,    -1,    -1,    -1,   193,    -1,   195,
     196,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,   151,   152,   153,   154,
      33,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,    -1,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
      -1,   196,   197,   198,    -1,   200,    -1,    -1,   203,   204,
      -1,    -1,    -1,     5,     6,    -1,   211,    -1,   213,    -1,
     215,   216,   217,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
     123,    -1,    -1,    45,   127,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,   189,    -1,    -1,    -1,
     193,    -1,   195,   196,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,   211,    -1,
     132,   133,   134,    -1,   217,    -1,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,   151,
     152,   153,   154,    33,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,   198,    -1,   200,    -1,
      -1,   203,   204,    -1,    -1,     5,     6,    -1,    -1,   211,
      -1,   213,    -1,   215,   216,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,   123,    -1,    45,    -1,   127,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,   189,
      -1,    -1,    -1,   193,    -1,   195,   196,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,   211,   132,   133,   134,    -1,    -1,   217,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,   151,   152,   153,   154,    33,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,    -1,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   198,    -1,
     200,    -1,    -1,   203,   204,    -1,    -1,     5,     6,    -1,
      -1,   211,    -1,   213,    -1,   215,   216,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,   123,    -1,    45,    -1,   127,
      48,    -1,    -1,    51,    52,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,   189,    -1,    -1,    -1,   193,    -1,   195,   196,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,   151,   152,   153,   154,    33,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     198,    -1,   200,    -1,    -1,   203,   204,    -1,    -1,     5,
       6,    -1,    -1,   211,    -1,   213,    -1,   215,   216,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,   123,    -1,    45,
      -1,   127,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,   193,    -1,   195,
     196,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   132,   133,   134,    -1,
      -1,   217,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,   151,   152,   153,   154,    33,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,   198,    -1,   200,    -1,    -1,   203,   204,    -1,
      -1,     5,     6,    -1,    -1,   211,    -1,   213,    -1,   215,
     216,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,   123,
      -1,    45,    -1,   127,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    70,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,   193,
      -1,   195,   196,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,   151,   152,   153,
     154,    33,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,   198,    -1,   200,    -1,    -1,   203,
     204,    -1,    -1,     5,     6,    -1,    -1,   211,    -1,   213,
     214,   215,   216,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    33,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,   123,    -1,    45,    -1,   127,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,   193,    -1,   195,   196,   117,   118,    -1,   123,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,   211,
     132,   133,   134,    -1,    -1,   217,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,   151,
     152,   153,   154,    -1,   156,    33,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    60,    61,   189,    -1,    -1,    -1,   193,    -1,
     195,   196,    -1,    -1,   196,   197,   198,    -1,   200,    -1,
      -1,   203,   204,    -1,    -1,    -1,   211,     5,     6,   211,
      -1,   213,   217,   215,   216,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,   123,    -1,    45,    -1,   127,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,   189,    -1,    -1,    -1,   193,    -1,   195,   196,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,   151,   152,   153,   154,    -1,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     198,    -1,   200,    -1,    -1,   203,   204,    -1,    -1,    -1,
      -1,     5,     6,   211,   212,   213,    -1,   215,   216,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,   198,    -1,   200,    -1,    -1,   203,
     204,    -1,    -1,    -1,    -1,     5,     6,   211,    -1,   213,
      -1,   215,   216,    13,    -1,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,   151,   152,   153,   154,    -1,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,    -1,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   198,    -1,
     200,    -1,    -1,   203,   204,    -1,    -1,    -1,    -1,     5,
       6,   211,   212,   213,    -1,   215,   216,    13,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,   151,   152,   153,   154,    -1,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,   198,    -1,   200,    -1,    -1,   203,   204,    -1,
      -1,     5,     6,    -1,    -1,   211,    -1,   213,    -1,   215,
     216,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    70,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,   198,    -1,   200,    -1,    -1,   203,
     204,    -1,    -1,     5,     6,    -1,    -1,   211,    -1,   213,
      -1,   215,   216,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,   151,
     152,   153,   154,    -1,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,   198,    -1,   200,    -1,
      -1,   203,   204,    -1,    -1,     5,     6,    -1,    -1,   211,
     212,   213,    -1,   215,   216,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,   151,   152,   153,   154,    -1,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,    -1,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   198,    -1,
     200,    -1,    -1,   203,   204,    -1,    -1,     5,     6,    -1,
      -1,   211,    -1,   213,    -1,   215,   216,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,   151,   152,   153,   154,    -1,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     198,    -1,   200,    -1,    -1,   203,   204,    -1,    -1,    -1,
      -1,     5,     6,   211,    -1,   213,    -1,   215,   216,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,   151,   152,   153,
     154,    -1,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,   198,    -1,   200,    -1,    -1,   203,
     204,    -1,    -1,     5,     6,    -1,    -1,   211,    -1,   213,
      -1,   215,   216,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,   151,
     152,   153,   154,    -1,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,    -1,   196,   197,   198,    -1,   200,    -1,
      -1,   203,   204,    -1,    -1,    -1,    -1,     5,     6,   211,
      -1,   213,    -1,   215,   216,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,   151,   152,   153,   154,    -1,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     198,    -1,   200,    -1,    -1,   203,   204,    -1,    -1,     5,
       6,    -1,    -1,   211,    -1,   213,    -1,   215,   216,    15,
      16,    17,    18,    19,    -1,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,   151,   152,   153,   154,    -1,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,   198,    -1,   200,    -1,    -1,   203,   204,    -1,
      -1,     5,     6,    -1,    -1,   211,    -1,   213,    -1,   215,
     216,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,   151,   152,   153,
     154,    -1,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,   198,    -1,   200,    -1,    -1,   203,
     204,    -1,    -1,     5,     6,    -1,    -1,   211,    -1,   213,
     214,   215,   216,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,   151,
     152,   153,   154,    -1,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,    -1,   196,   197,   198,    -1,   200,    -1,
      -1,   203,   204,    -1,    -1,     5,     6,    -1,    -1,   211,
      -1,   213,    -1,   215,   216,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,   151,   152,   153,   154,    -1,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,    -1,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   198,    -1,
     200,    -1,    -1,   203,   204,    -1,    -1,     5,     6,    -1,
      -1,   211,    -1,   213,   214,   215,   216,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,   151,   152,   153,   154,    -1,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     198,    -1,   200,    -1,    -1,   203,   204,    -1,    -1,     5,
       6,    -1,    -1,   211,    -1,   213,   214,   215,   216,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,   151,   152,   153,   154,    -1,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,   198,    -1,   200,    -1,    -1,   203,   204,    -1,
      -1,     5,     6,    -1,    -1,   211,    -1,   213,   214,   215,
     216,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,   151,   152,   153,
     154,    -1,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,   198,    -1,   200,    -1,    -1,   203,
     204,    -1,    -1,     5,     6,    -1,    -1,   211,    -1,   213,
     214,   215,   216,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,   151,
     152,   153,   154,    -1,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,   198,    -1,   200,    -1,
      -1,   203,   204,    -1,    -1,     5,     6,    -1,    -1,   211,
      -1,   213,   214,   215,   216,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,   151,   152,   153,   154,    -1,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,    -1,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   198,    -1,
     200,    -1,    -1,   203,   204,    -1,    -1,     5,     6,    -1,
      -1,   211,    -1,   213,   214,   215,   216,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,   151,   152,   153,   154,    -1,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     198,    -1,   200,    -1,    -1,   203,   204,    -1,    -1,     5,
       6,    -1,    -1,   211,    -1,   213,    -1,   215,   216,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,   151,   152,   153,   154,    -1,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,   198,    -1,   200,    -1,    -1,   203,   204,    -1,
      -1,     5,     6,    -1,    -1,   211,    -1,   213,    -1,   215,
     216,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,   151,   152,   153,
     154,    -1,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,   198,    -1,   200,    -1,    -1,   203,
     204,    -1,    -1,     5,     6,    -1,    -1,   211,    -1,   213,
      -1,   215,   216,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,   151,
     152,   153,   154,    -1,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,   198,    -1,   200,    -1,
      -1,   203,   204,    -1,    -1,     5,     6,    -1,    -1,   211,
      -1,   213,    -1,   215,   216,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,   151,   152,   153,   154,    -1,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,    -1,    13,    -1,    -1,    -1,    -1,   179,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,   196,   197,   198,    -1,
     200,    -1,    41,   203,   204,    -1,    -1,    -1,    -1,    -1,
      49,   211,    -1,   213,    -1,   215,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,   142,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
     169,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   215,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,   142,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   169,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   215,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   169,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   215,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,   169,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    41,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   215,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    21,    22,
      -1,    -1,   169,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
      -1,   169,   125,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   215,    21,    22,
     135,   136,   137,   138,   139,   140,    -1,    -1,   143,   144,
     145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,   189,    -1,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   210,   211,    -1,    -1,    -1,
     215,   216,    -1,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    21,    22,
      -1,    -1,   135,   136,   137,   138,   139,   140,    -1,    -1,
     143,   144,   145,   146,   147,   148,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,    -1,   215,   216,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,   136,   137,   138,   139,   140,    -1,    -1,
     143,   144,   145,   146,   147,   148,   149,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    21,    22,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,   181,    -1,
      -1,   149,    -1,    -1,    -1,   188,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,    -1,   215,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    21,    22,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    21,    22,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    21,    22,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    21,    22,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    21,    22,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    21,    22,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    21,    22,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    21,    22,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    21,    22,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    21,    22,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    21,    22,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    21,    22,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    21,    22,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    21,    22,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
     118,   119,    -1,    -1,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    21,    22,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,   118,   119,    -1,    -1,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,   115,   116,   117,
     118,   119,    21,    22,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,    -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   211,   212,    -1,   115,   116,   117,   118,
     119,    21,    22,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,
      -1,    -1,    -1,    -1,   143,   144,   145,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   210,   211,   212,    -1,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,    -1,
      -1,    -1,    -1,   143,   144,   145,    -1,    -1,    -1,   149,
     115,   116,   117,   118,   119,    21,    22,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,   137,   138,    -1,    -1,    -1,    -1,   143,   144,
     145,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,   189,
      -1,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     210,   211,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   210,   211,   212,    -1,   115,
     116,   117,   118,   119,    21,    22,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,   137,   138,    -1,    -1,    -1,    -1,   143,   144,   145,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,   189,    -1,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   210,   211,   212,    -1,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,    21,    22,    -1,    -1,   135,    -1,
     137,   138,    -1,    -1,   157,    -1,   143,   144,   145,    -1,
      -1,    38,   149,    -1,    -1,    -1,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   210,   211,   212,    -1,    -1,    -1,    -1,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    -1,    -1,
     127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,    -1,    -1,   143,   144,   145,   146,
     147,   148,   149,   115,   116,   117,   118,   119,    21,    22,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,   135,    -1,   137,   138,    -1,    -1,    -1,
      -1,   143,   144,   145,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,   189,    -1,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,    -1,   203,   204,    -1,    -1,
      -1,    -1,    -1,   210,   211,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,
      -1,    -1,   115,   116,   117,   118,   119,    21,    22,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,   135,    -1,   137,   138,    -1,    -1,   141,    -1,
     143,   144,   145,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,
      -1,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,   137,   138,    -1,    -1,   141,    -1,   143,
     144,   145,    21,    22,    -1,   149,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,
      -1,    -1,    -1,    -1,   143,   144,   145,    21,    22,    -1,
     149,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,    -1,    -1,    -1,   115,   116,   117,   118,
     119,   210,   211,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,
      -1,    -1,    -1,    -1,   143,   144,   145,    21,    22,    -1,
     149,   115,   116,   117,   118,   119,    -1,    -1,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,   143,
     144,   145,   181,    -1,    -1,   149,    21,    22,    -1,    -1,
     189,    -1,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   210,   211,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,   115,   116,   117,   118,   119,   210,   211,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,   137,   138,    -1,    -1,    -1,    -1,   143,
     144,   145,    -1,    -1,    -1,   149,    21,    22,    -1,    -1,
     115,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,   137,   138,    -1,    -1,    -1,   181,   143,   144,
     145,    21,    22,    -1,   149,   189,    -1,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   210,   211,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
     115,   116,   117,   118,   119,   210,   211,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
     135,    -1,   137,   138,    -1,    -1,    -1,    -1,   143,   144,
     145,    21,    22,    -1,   149,   115,   116,   117,   118,   119,
      -1,    -1,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,    -1,
      -1,    -1,    -1,   143,   144,   145,   181,    -1,    -1,   149,
      21,    22,    -1,    -1,   189,    -1,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   210,   211,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
     210,   211,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,    -1,
      -1,    -1,    -1,   143,   144,   145,    -1,    -1,    -1,   149,
      21,    22,    -1,    -1,   115,   116,   117,   118,   119,    -1,
      -1,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,   137,   138,    -1,    -1,
      -1,   181,   143,   144,   145,    21,    22,    -1,   149,   189,
      -1,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     210,   211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
      -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,   210,
     211,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,   135,    -1,   137,   138,    -1,    -1,
      -1,    -1,   143,   144,   145,    21,    22,    -1,   149,   115,
     116,   117,   118,   119,    -1,    -1,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,   137,   138,    -1,    21,    22,    -1,   143,   144,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,
     211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,   115,
     116,   117,   118,   119,   210,   211,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,
      -1,   137,   138,    -1,    21,    22,    -1,   143,   115,   116,
     117,   118,   119,    -1,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,
     137,   138,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   210,   211,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,    -1,    -1,    -1,   115,   116,
     117,   118,   119,   210,   211,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,   135,    -1,
     137,   138,    21,    22,    -1,    -1,   115,   116,   117,   118,
     119,    -1,    -1,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,   210,   211,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,    -1,    -1,    -1,   115,   116,   117,   118,
     119,   210,   211,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,
     199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   210,   211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    71,    72,    73,   169,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,    71,    -1,    73,    -1,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   219,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   181,   200,   211,   220,   223,   229,   231,
     232,   237,   267,   271,   305,   383,   390,   394,   405,   450,
     455,   460,    19,    20,   169,   259,   260,   261,   162,   238,
     239,   169,   170,   171,   200,   233,   234,   235,   179,   391,
     169,   215,   222,    57,    63,   386,   386,   386,   142,   169,
     291,    34,    63,   135,   204,   213,   263,   264,   265,   266,
     291,   181,     5,     6,     8,    36,   402,    62,   381,   188,
     187,   190,   187,   234,    22,    57,   199,   210,   236,   386,
     387,   389,   387,   381,   461,   451,   456,   169,   142,   230,
     265,   265,   265,   213,   143,   144,   145,   187,   212,    57,
      63,   272,   274,    57,    63,   392,    57,    63,   403,    57,
      63,   382,    15,    16,   162,   167,   169,   172,   213,   225,
     260,   162,   239,   169,   233,   233,   169,   181,   180,   387,
     181,    57,    63,   221,   169,   169,   169,   169,   173,   228,
     214,   261,   265,   265,   265,   265,   275,   169,   393,   406,
     179,   384,   173,   174,   224,    15,    16,   162,   167,   169,
     225,   257,   258,   236,   388,   181,   462,   452,   457,   173,
     214,    35,    71,    73,    75,    76,    77,    78,    79,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      93,    94,    95,    98,    99,   100,   101,   117,   118,   169,
     270,   273,   179,   190,   106,   400,   401,   379,   159,   216,
     262,   355,   173,   174,   175,   187,   214,   188,   179,   179,
     179,    21,    22,    38,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   127,   128,   129,   136,   137,   138,   139,   140,
     143,   144,   145,   146,   147,   148,   149,   189,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   203,   204,
     210,   211,    35,    35,   213,   268,   179,   276,   395,    75,
      79,    93,    94,    98,    99,   100,   101,   410,   169,   407,
     180,   380,   261,   260,   181,   216,   151,   169,   377,   378,
     257,    19,    25,    31,    41,    49,    64,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     155,   215,   291,   409,   411,   412,   415,   421,   449,   463,
     453,   458,   169,   169,   169,   212,    22,   169,   212,   154,
     214,   355,   365,   366,   190,   269,   279,   385,   179,   190,
     399,   179,   404,   355,   212,   260,   213,    43,   187,   190,
     193,   376,   194,   194,   194,   213,   194,   194,   213,   194,
     194,   194,   194,   194,   194,   213,   291,    33,    60,    61,
     123,   127,   189,   193,   196,   211,   217,   420,   191,   414,
     369,   372,   169,   136,   213,     7,    50,   304,   181,   214,
     449,     1,     5,     6,     9,    10,    11,    13,    15,    16,
      17,    18,    19,    25,    26,    27,    28,    29,    31,    38,
      39,    42,    44,    45,    48,    51,    52,    54,    55,    58,
      59,    65,    68,    69,    80,   102,   103,   104,   105,   117,
     118,   132,   133,   134,   150,   151,   152,   153,   154,   156,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   170,   171,   172,   179,   180,   181,   196,   197,   198,
     203,   204,   211,   213,   215,   216,   227,   229,   240,   241,
     244,   247,   248,   250,   252,   253,   254,   255,   277,   278,
     280,   285,   290,   291,   292,   296,   297,   298,   299,   300,
     301,   302,   303,   305,   309,   310,   317,   320,   323,   328,
     331,   332,   334,   335,   336,   338,   343,   346,   347,   354,
     409,   465,   480,   491,   495,   508,   511,   169,   181,   396,
     397,   291,   361,   378,   212,    65,   104,   170,   285,   347,
     169,   169,   421,   126,   136,   188,   375,   422,   427,   429,
     347,   431,   425,   169,   416,   433,   435,   437,   439,   441,
     443,   445,   447,   347,   194,   213,    33,   193,    33,   193,
     211,   217,   212,   347,   211,   217,   421,   169,   181,   464,
     169,   181,   187,   367,   418,   449,   454,   169,   370,   418,
     459,   347,   151,   169,   374,   408,   365,   194,   194,   347,
     251,   194,   293,   411,   465,   213,   291,   194,     5,   102,
     103,   194,   213,   126,   290,   321,   332,   347,   276,   194,
     213,    61,   347,   213,   347,   169,   194,   194,   213,   181,
     194,   162,    58,   347,   213,   276,   194,   213,   194,   194,
     213,   194,   194,   126,   290,   347,   347,   347,   216,   276,
     323,   327,   327,   327,   213,   213,   213,   213,   213,   213,
      13,   421,    13,   421,    13,   347,   490,   506,   194,   347,
     194,   226,    13,   490,   507,    37,   347,   347,   347,   347,
     347,    13,    49,   321,   347,   321,   216,   181,   181,   347,
      10,   323,   329,   169,   213,   181,   181,   181,   181,   181,
      66,   306,   267,   131,   181,    21,    22,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   135,   136,   137,   138,   139,   140,   143,   144,   145,
     146,   147,   148,   149,   188,   189,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   210,   211,   329,   213,
     181,   188,   180,   385,   180,   211,   267,   362,   194,   214,
      43,   181,   375,   290,   347,   449,   449,   419,   449,   214,
     449,   449,   214,   169,   413,   449,   268,   449,   268,   449,
     268,   367,   368,   370,   371,   214,   424,   281,   321,   212,
     212,   347,   181,   180,   190,   418,   180,   190,   418,   180,
     214,   213,    43,   126,   187,   188,   190,   193,   373,   481,
     483,   276,   408,   294,   213,   291,   194,   213,   318,   194,
     194,   194,   502,   321,   290,   321,   187,   107,   108,   109,
     110,   111,   112,   113,   114,   120,   121,   126,   139,   140,
     146,   147,   148,   188,    14,   421,   283,   507,   347,   347,
     276,   188,   311,   313,   347,   315,   190,   162,   347,   504,
     321,   487,   492,   321,   485,   421,   290,   347,   216,   267,
     347,   347,   347,   347,   347,   347,   408,    53,   157,   169,
     196,   211,   213,   347,   466,   469,   473,   489,   494,   408,
     213,   469,   494,   408,   141,   180,   181,   182,   474,   286,
     276,   288,   175,   176,   224,   408,   187,   510,   179,   408,
      13,   187,   510,   510,   151,   156,   194,   291,   337,   276,
     249,   330,    70,   211,   214,   321,   466,   468,   159,   213,
     308,   378,     4,   159,   326,   327,    19,   157,   169,   409,
      19,   157,   169,   409,   132,   133,   134,   277,   333,   347,
     333,   347,   333,   347,   333,   347,   333,   347,   333,   347,
     333,   347,   333,   347,   347,   347,   347,   333,   347,   333,
     347,   347,   347,   347,   169,   333,   347,   347,   157,   169,
     347,   347,   347,   409,   347,   347,   347,   333,   347,   333,
     347,   347,   347,   347,   333,   347,   333,   347,   333,   347,
     347,   333,   347,    22,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   128,   129,   157,   169,   210,
     211,   344,   347,   214,   321,   347,   398,   266,     8,   355,
     360,   421,   169,   290,   347,   181,   195,   195,   195,   418,
     195,   195,   181,   195,   195,   269,   195,   269,   195,   269,
     195,   418,   195,   418,   284,   449,   214,   510,   212,   449,
     449,   347,   169,   169,   449,   290,   347,   421,   421,    20,
     449,    70,   321,   468,   479,   194,   347,   169,   347,   449,
     496,   498,   500,   421,   510,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   276,   195,   418,   214,   510,   214,   256,
     421,   421,   214,   421,   214,   421,   510,   421,   421,   510,
     421,   195,   326,   214,   214,   214,   214,   214,   214,    20,
     327,   213,   136,   373,   211,   347,   214,   141,   187,   181,
     473,   184,   185,   212,   477,   187,   181,   184,   212,   476,
      20,   214,   473,   180,   183,   475,    20,   347,   180,   490,
     284,   284,   347,    20,   490,   180,   279,    20,   408,   212,
     214,   213,   213,   339,   341,    12,    23,    24,   242,   243,
     347,   267,   169,   214,   468,   466,   187,   214,   214,   169,
     307,   307,   213,   126,   136,   169,   188,   193,   324,   325,
     268,   194,   213,   194,   213,   327,   327,   327,   213,   213,
     212,    19,   157,   169,   409,   190,   157,   169,   347,   213,
     213,   157,   169,   347,     1,   212,   214,   181,   180,   212,
      57,    63,   358,    67,   359,   181,   195,   181,   423,   428,
     430,   449,   432,   426,   169,   417,   434,   195,   438,   195,
     442,   195,   446,   367,   448,   370,   195,   418,   214,    43,
     373,   195,   195,   321,   195,   468,   214,   214,   214,   169,
     214,   181,   195,   214,   195,   421,   421,   421,   195,   214,
     213,   421,   347,   195,   195,   195,   195,   214,   195,   195,
     214,   195,   326,   268,   213,   321,   347,   347,   347,   469,
     473,   347,   157,   169,   466,   477,   212,   347,   489,   212,
     321,   469,   180,   183,   186,   478,   212,   321,   195,   195,
     177,   321,   180,   321,    20,   347,   347,   421,   268,   276,
     347,    12,   245,   326,   214,   212,   211,   187,   212,   214,
     325,   169,   169,   213,   169,   169,   187,   212,   269,   348,
     347,   350,   347,   214,   321,   347,   194,   213,   347,   213,
     212,   347,   214,   321,   213,   212,   345,   181,    47,   359,
      46,   106,   356,   326,   436,   440,   444,   213,   449,   169,
     347,   482,   484,   276,   295,   214,   195,   418,   169,   213,
     319,   195,   195,   195,   503,   283,   195,   312,   314,   316,
     505,   488,   493,   486,   213,   329,   269,   214,   321,   181,
     214,   473,   477,   213,   136,   373,   181,   473,   212,   181,
     287,   289,   181,   181,   321,   214,   214,   195,   269,   276,
     246,   181,   268,   214,   466,   169,   212,   190,   376,   214,
     169,   324,   212,   141,   276,   322,   421,   214,   449,   214,
     214,   214,   352,   347,   347,   214,   214,   347,    33,   357,
     356,   358,   281,   213,   213,   347,   169,   195,   347,   497,
     499,   501,   213,   214,   213,   347,   347,   347,   213,    70,
     479,   213,   213,   214,   347,   322,   214,   347,   136,   373,
     477,   347,   347,   347,   347,   478,   490,   347,   213,   282,
     490,   347,   181,   340,   195,   243,    26,   105,   247,   297,
     298,   299,   301,   347,   269,   212,   190,   376,   421,   375,
     214,   126,   347,   195,   195,   449,   214,   214,   214,   363,
     357,   374,   214,   479,   479,   214,   195,   213,   214,   213,
     213,   213,   281,   283,   321,   479,   466,   467,   214,   181,
     509,   347,   347,   214,   509,   509,   281,   509,   509,   347,
     337,   342,   126,   126,   347,   276,   214,   421,   375,   375,
     347,   347,   349,   351,   195,   273,   364,   213,   466,   470,
     471,   472,   472,   347,   347,   479,   479,   467,   214,   214,
     510,   472,   214,    53,   212,   136,   373,   180,   180,   187,
     510,   180,   212,   509,   337,   347,   375,   347,   347,   181,
     353,   181,   273,   466,   187,   510,   214,   214,   214,   214,
     472,   472,   214,   214,   214,   347,   212,   347,   347,   212,
     180,   214,   212,   347,   181,   181,   276,   214,   213,   214,
     214,   181,   466,   214
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   218,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   220,   221,   221,
     221,   222,   222,   223,   224,   224,   224,   224,   225,   226,
     226,   226,   227,   228,   228,   230,   229,   231,   232,   233,
     233,   233,   234,   234,   234,   234,   235,   235,   236,   236,
     237,   238,   238,   239,   239,   240,   241,   241,   242,   242,
     243,   243,   243,   244,   244,   245,   246,   245,   247,   247,
     247,   247,   247,   248,   249,   248,   251,   250,   252,   253,
     254,   256,   255,   257,   257,   257,   257,   257,   257,   258,
     258,   259,   259,   259,   260,   260,   260,   260,   260,   260,
     260,   260,   261,   261,   262,   262,   262,   263,   263,   263,
     264,   264,   265,   265,   265,   265,   265,   265,   265,   266,
     266,   267,   267,   268,   268,   268,   269,   269,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   271,   272,   272,   272,   273,   275,   274,
     276,   276,   277,   277,   277,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   279,   279,   279,   280,
     281,   281,   282,   282,   283,   283,   284,   284,   286,   287,
     285,   288,   289,   285,   290,   290,   290,   290,   290,   291,
     291,   291,   292,   292,   294,   295,   293,   293,   296,   296,
     296,   296,   296,   296,   297,   298,   299,   299,   299,   300,
     300,   300,   301,   301,   302,   302,   302,   303,   304,   304,
     304,   305,   305,   306,   306,   307,   307,   308,   308,   308,
     308,   309,   309,   311,   312,   310,   313,   314,   310,   315,
     316,   310,   318,   319,   317,   320,   320,   320,   320,   320,
     320,   321,   321,   322,   322,   322,   323,   323,   323,   324,
     324,   324,   324,   324,   325,   325,   326,   326,   326,   327,
     327,   328,   330,   329,   331,   331,   331,   331,   331,   331,
     331,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     333,   333,   333,   333,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   335,   335,   336,   336,   337,   337,   338,   339,   340,
     338,   341,   342,   338,   343,   343,   343,   343,   344,   345,
     343,   346,   346,   346,   346,   346,   346,   346,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   348,   349,   347,   347,   347,   347,
     350,   351,   347,   347,   347,   352,   353,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   355,   355,
     355,   356,   356,   356,   357,   357,   358,   358,   358,   359,
     359,   360,   361,   362,   361,   363,   361,   364,   361,   361,
     365,   365,   366,   366,   367,   367,   368,   368,   369,   369,
     369,   370,   371,   371,   372,   372,   372,   373,   373,   374,
     374,   374,   374,   374,   374,   375,   375,   375,   376,   376,
     377,   377,   377,   377,   377,   378,   378,   378,   378,   378,
     379,   380,   379,   381,   381,   382,   382,   382,   383,   384,
     383,   385,   385,   385,   385,   386,   386,   386,   388,   387,
     389,   389,   390,   391,   390,   392,   392,   392,   393,   395,
     396,   394,   397,   398,   394,   399,   399,   400,   400,   401,
     402,   402,   403,   403,   403,   404,   404,   406,   407,   405,
     408,   408,   408,   408,   408,   409,   409,   409,   409,   409,
     409,   409,   409,   409,   409,   409,   409,   409,   409,   409,
     409,   409,   409,   409,   409,   409,   409,   409,   409,   409,
     409,   409,   410,   410,   410,   410,   410,   410,   410,   410,
     411,   412,   412,   412,   413,   413,   414,   414,   414,   416,
     417,   415,   418,   418,   419,   419,   420,   420,   421,   421,
     421,   421,   421,   421,   422,   423,   421,   421,   421,   424,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   425,   426,   421,   421,   427,   428,   421,
     429,   430,   421,   431,   432,   421,   421,   433,   434,   421,
     435,   436,   421,   421,   437,   438,   421,   439,   440,   421,
     421,   441,   442,   421,   443,   444,   421,   445,   446,   421,
     447,   448,   421,   449,   449,   449,   451,   452,   453,   454,
     450,   456,   457,   458,   459,   455,   461,   462,   463,   464,
     460,   465,   465,   465,   465,   465,   466,   466,   466,   466,
     466,   466,   466,   466,   467,   468,   469,   469,   470,   470,
     471,   471,   472,   472,   473,   473,   474,   474,   475,   475,
     476,   476,   477,   477,   477,   478,   478,   478,   479,   479,
     480,   480,   480,   480,   480,   480,   481,   482,   480,   483,
     484,   480,   485,   486,   480,   487,   488,   480,   489,   489,
     489,   490,   490,   491,   492,   493,   491,   494,   494,   495,
     495,   495,   496,   497,   495,   498,   499,   495,   500,   501,
     495,   495,   502,   503,   495,   495,   504,   505,   495,   506,
     506,   507,   507,   508,   508,   508,   508,   508,   509,   509,
     510,   510,   511,   511,   511,   511,   511,   511
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
       1,     5,     1,     3,     2,     3,     2,     1,     1,     1,
       1,     4,     1,     2,     3,     3,     3,     3,     2,     1,
       3,     0,     3,     0,     2,     3,     0,     2,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     2,     2,     3,     4,     3,     2,
       2,     2,     2,     2,     3,     3,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     1,     1,     3,     0,     4,
       3,     7,     2,     2,     6,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     2,     2,     2,     0,     2,     2,     3,
       0,     2,     0,     4,     0,     2,     1,     3,     0,     0,
       7,     0,     0,     7,     3,     2,     2,     2,     1,     1,
       3,     2,     2,     3,     0,     0,     5,     1,     2,     5,
       5,     5,     6,     2,     1,     1,     1,     2,     3,     2,
       2,     3,     2,     3,     2,     2,     3,     4,     1,     1,
       0,     1,     1,     1,     0,     1,     3,     9,     8,     8,
       7,     3,     3,     0,     0,     7,     0,     0,     7,     0,
       0,     7,     0,     0,     6,     5,     8,    10,     5,     8,
      10,     1,     3,     1,     2,     3,     1,     1,     2,     2,
       2,     2,     2,     4,     1,     3,     0,     4,     4,     1,
       6,     6,     0,     7,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     6,     8,     5,     6,     1,     4,     3,     0,     0,
       8,     0,     0,     9,     3,     4,     5,     6,     0,     0,
       5,     3,     4,     4,     5,     4,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     4,     4,
       5,     4,     5,     3,     4,     1,     1,     2,     4,     4,
       7,     8,     3,     5,     0,     0,     8,     3,     3,     3,
       0,     0,     8,     3,     4,     0,     0,     9,     4,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     2,
       4,     1,     4,     4,     4,     4,     4,     1,     6,     7,
       6,     6,     7,     7,     6,     7,     6,     6,     0,     4,
       1,     0,     1,     1,     0,     1,     0,     1,     1,     0,
       1,     5,     0,     0,     4,     0,     9,     0,    10,     5,
       3,     4,     1,     3,     1,     3,     1,     3,     0,     2,
       3,     3,     1,     3,     0,     2,     3,     1,     1,     1,
       2,     3,     5,     3,     3,     1,     1,     1,     0,     1,
       1,     4,     3,     3,     5,     4,     6,     5,     5,     4,
       0,     0,     4,     0,     1,     0,     1,     1,     6,     0,
       6,     0,     2,     3,     5,     0,     1,     1,     0,     5,
       2,     3,     4,     0,     4,     0,     1,     1,     1,     0,
       0,     9,     0,     0,    11,     0,     2,     0,     1,     3,
       1,     1,     0,     1,     1,     0,     3,     0,     0,     7,
       1,     4,     3,     3,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     1,     3,     0,     2,     3,     0,
       0,     6,     1,     1,     1,     3,     3,     4,     1,     1,
       1,     1,     2,     3,     0,     0,     6,     4,     5,     0,
       9,     4,     2,     2,     3,     2,     3,     2,     2,     3,
       3,     3,     2,     0,     0,     6,     2,     0,     0,     6,
       0,     0,     6,     0,     0,     6,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     0,     0,     6,
       0,     0,     6,     1,     3,     3,     0,     0,     0,     0,
      10,     0,     0,     0,     0,    10,     0,     0,     0,     0,
      10,     1,     1,     1,     1,     1,     3,     3,     5,     5,
       6,     6,     8,     8,     1,     1,     1,     3,     3,     5,
       1,     2,     1,     0,     0,     2,     2,     1,     2,     1,
       2,     1,     2,     1,     1,     2,     1,     1,     0,     1,
       5,     4,     6,     7,     5,     7,     0,     0,    10,     0,
       0,    10,     0,     0,     9,     0,     0,     7,     1,     3,
       3,     3,     1,     5,     0,     0,    10,     1,     3,     4,
       4,     4,     0,     0,    11,     0,     0,    11,     0,     0,
      10,     5,     0,     0,     9,     5,     0,     0,    10,     1,
       3,     1,     3,     4,     3,     4,     7,     9,     0,     3,
       0,     1,     9,    10,    10,    10,     9,    10
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

    case YYSYMBOL_metadata_argument_list: /* metadata_argument_list  */
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

    case YYSYMBOL_optional_expr_map_tuple_list: /* optional_expr_map_tuple_list  */
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

    case YYSYMBOL_make_variant_dim: /* make_variant_dim  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_single: /* make_struct_single  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_dim: /* make_struct_dim  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_dim_list: /* make_struct_dim_list  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_dim_decl: /* make_struct_dim_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_optional_make_struct_dim_decl: /* optional_make_struct_dim_decl  */
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

  case 104: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 105: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 106: /* metadata_argument_list: metadata_argument_list "end of expression"  */
                                         {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 107: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 108: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 109: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 110: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 111: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 112: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 113: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 114: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 115: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 116: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 117: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 118: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 119: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 120: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 121: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 122: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 123: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 124: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 125: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 126: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 127: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 128: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 129: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 130: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 131: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 132: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 133: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 134: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 135: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 136: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 137: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 138: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 139: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 140: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 141: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 142: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 143: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 144: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 145: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 146: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 147: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 148: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 149: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 150: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 151: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 152: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 153: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 154: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 155: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 156: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 157: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 158: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 159: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 160: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 161: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 162: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 163: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 164: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 165: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 166: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 167: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 168: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 169: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 170: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 171: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 172: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 173: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 174: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 175: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 176: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 177: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 178: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 179: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 180: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 181: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 182: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 183: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 184: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 185: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 186: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 187: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 188: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 189: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 190: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 191: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 192: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 193: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 194: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 195: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 196: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 197: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 198: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 199: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 200: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 201: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 202: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 203: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 204: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 205: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 206: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 207: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 208: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 209: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 210: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 211: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 212: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 213: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 214: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 215: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 216: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 217: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 218: /* $@6: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 219: /* function_declaration: optional_public_or_private_function $@6 function_declaration_header expression_block  */
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

  case 220: /* expression_block: "begin of code block" expressions "end of code block"  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 221: /* expression_block: "begin of code block" expressions "end of code block" "finally" "begin of code block" expressions "end of code block"  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 222: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 223: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 224: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 225: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 226: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 227: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 230: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 231: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 235: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 236: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 237: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 238: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 239: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 240: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 241: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 242: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 243: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 244: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 245: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 246: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 247: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 248: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 249: /* expr_keyword: "keyword" expr expression_block  */
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

  case 250: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 251: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 252: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 253: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 254: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 255: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 256: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 257: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 258: /* $@7: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 259: /* $@8: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 260: /* expression_keyword: "keyword" '<' $@7 type_declaration_no_options_list '>' $@8 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 261: /* $@9: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 262: /* $@10: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 263: /* expression_keyword: "type function" '<' $@9 type_declaration_no_options_list '>' $@10 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 264: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 265: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 266: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 267: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 268: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 269: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 270: /* name_in_namespace: "name" "::" "name"  */
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

  case 271: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 272: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 273: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 274: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 275: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 276: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 277: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 278: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 279: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 280: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 281: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 282: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 283: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 284: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 285: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 286: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 287: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 288: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 289: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 290: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 291: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 292: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 293: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 294: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 295: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 296: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 297: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 298: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 299: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 300: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 301: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 302: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 303: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 304: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 305: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 306: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 307: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 308: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 309: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 310: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 311: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 312: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 313: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 314: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 315: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 316: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 317: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 318: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 319: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 320: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 321: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 322: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 323: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 324: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 325: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 326: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 327: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 328: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
                                                                          {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)));
            }
            delete (yyvsp[-3].s);
    }
    break;

  case 329: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ptd->typeexpr,*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-4].s));
            }
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 330: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
                                                                                                                     {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ptd->typeexpr,*(yyvsp[-6].s),*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            }
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 331: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 332: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 333: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 334: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 335: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 336: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 337: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 338: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 339: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 340: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 341: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 342: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 343: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 344: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 345: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 346: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 347: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 348: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 349: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 350: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 351: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 352: /* $@21: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 353: /* expr_full_block_assumed_piped: block_or_lambda $@21 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 354: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 355: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 356: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 357: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 358: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 359: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 360: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 361: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 362: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 381: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 382: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 383: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 384: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 402: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 403: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 404: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 405: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 406: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 407: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 408: /* $@22: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 409: /* $@23: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 410: /* func_addr_expr: '@' '@' '<' $@22 type_declaration_no_options '>' $@23 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 411: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 412: /* $@25: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 413: /* func_addr_expr: '@' '@' '<' $@24 optional_function_argument_list optional_function_type '>' $@25 func_addr_name  */
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

  case 414: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 415: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 416: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 417: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 418: /* $@26: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 419: /* $@27: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 420: /* expr_field: expr '.' $@26 error $@27  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 421: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 422: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
                                                          {
            auto dd = new ExprMakeStruct(tokAt(scanner,(yylsp[-3])));
            dd->at = tokAt(scanner,(yylsp[-3]));
            dd->makeType = new TypeDecl(Type::alias);
            dd->makeType->alias = *(yyvsp[-3].s);
            dd->useInitializer = false;
            dd->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = dd;
    }
    break;

  case 423: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 424: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = new TypeDecl(Type::alias);
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType->alias = *(yyvsp[-4].s);
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 425: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 426: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 427: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 428: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 429: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 430: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 431: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 432: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 433: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 434: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 435: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 436: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 437: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 438: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 439: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 440: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 441: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 442: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 443: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 444: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 445: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 447: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 448: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 449: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 450: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 451: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 452: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 453: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 454: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 455: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 456: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 457: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 458: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 459: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 460: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 461: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 462: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 463: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 464: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 465: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 466: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 467: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 468: /* expr: '(' expr_list optional_comma ')'  */
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

  case 469: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 470: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 471: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 472: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 473: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 474: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 475: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 476: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 477: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 478: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 479: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 480: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 481: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 482: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 483: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 484: /* $@28: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 485: /* $@29: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 486: /* expr: expr "is" "type" '<' $@28 type_declaration_no_options '>' $@29  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 487: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 488: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 489: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 490: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 491: /* $@31: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 492: /* expr: expr "as" "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 493: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 494: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 495: /* $@32: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 496: /* $@33: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 497: /* expr: expr '?' "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 498: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 499: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 500: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 501: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 502: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 503: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 504: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 505: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 506: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 507: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 508: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 509: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 510: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 511: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 512: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 513: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 514: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 515: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 516: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 517: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 518: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 519: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 520: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 521: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 522: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 523: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 524: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 525: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 526: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 527: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 528: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 529: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 530: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 531: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 532: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 533: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 534: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 535: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 536: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 537: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 538: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 539: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 540: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 541: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 542: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 543: /* $@34: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 544: /* struct_variable_declaration_list: struct_variable_declaration_list $@34 structure_variable_declaration "end of expression"  */
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

  case 545: /* $@35: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 546: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@35 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 547: /* $@36: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 548: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@36 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 549: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' "end of expression"  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 550: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 551: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 552: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 553: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 554: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 555: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 556: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 557: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 558: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 559: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 560: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 561: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 562: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 563: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 564: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 565: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 566: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 567: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 568: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 569: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 570: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 571: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 572: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 573: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 574: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 575: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 576: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 577: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 578: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 579: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 580: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 581: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 582: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 583: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 584: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 585: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 586: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 587: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 588: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 589: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 590: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 591: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 592: /* global_variable_declaration_list: global_variable_declaration_list $@37 optional_field_annotation let_variable_declaration  */
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

  case 593: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 594: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 595: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 596: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 597: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 598: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 599: /* $@38: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 600: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@38 optional_field_annotation let_variable_declaration  */
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

  case 601: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 602: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 603: /* enum_list: enum_list "name" "end of expression"  */
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

  case 604: /* enum_list: enum_list "name" '=' expr "end of expression"  */
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

  case 605: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 606: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 607: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 608: /* $@39: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 609: /* single_alias: optional_public_or_private_alias "name" $@39 '=' type_declaration  */
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

  case 613: /* $@40: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 615: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 616: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 617: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 618: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 619: /* $@41: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 620: /* $@42: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 621: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@41 enum_list $@42 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 622: /* $@43: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 623: /* $@44: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 624: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@43 enum_list $@44 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 625: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 626: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 627: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 628: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 629: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 630: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 631: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 632: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 633: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 634: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 635: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 636: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 637: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 638: /* $@46: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 639: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
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

  case 640: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 641: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 642: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 643: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 644: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 645: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 646: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 647: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 648: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 649: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 650: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 651: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 652: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 653: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 654: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 655: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 656: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 657: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 658: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 659: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 660: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 661: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 662: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 663: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 664: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 665: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 666: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 667: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 668: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 669: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 670: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 671: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 672: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 673: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 674: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 675: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 676: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 677: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 678: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 679: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 680: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 681: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 682: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 683: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 684: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 685: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 686: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 687: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 688: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 689: /* $@47: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 690: /* $@48: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 691: /* bitfield_type_declaration: "bitfield" '<' $@47 bitfield_bits '>' $@48  */
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

  case 694: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 695: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 696: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 697: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 698: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 699: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 700: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 701: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 702: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 703: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 704: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 705: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 706: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 707: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 708: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 709: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 710: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 711: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 712: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 713: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 714: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 715: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 716: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 717: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 718: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 719: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 720: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 721: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 722: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 723: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 724: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 725: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 726: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 727: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 728: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 729: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 730: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 731: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 732: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 733: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 735: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 736: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 737: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 738: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 739: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 740: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 741: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 742: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 743: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 744: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 745: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 746: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 747: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 748: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 749: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 750: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 751: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 752: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 753: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 754: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 755: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 756: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 757: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 758: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 759: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 760: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 761: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 762: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 763: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 764: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 765: /* type_declaration: type_declaration '|' '#'  */
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

  case 766: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 767: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 768: /* $@78: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 769: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 770: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 "begin of code block" $@78 tuple_alias_type_list $@79 "end of code block"  */
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

  case 771: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 772: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 773: /* $@82: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 774: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 775: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 "begin of code block" $@82 variant_alias_type_list $@83 "end of code block"  */
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

  case 776: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 777: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 778: /* $@86: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 779: /* $@87: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 780: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 "begin of code block" $@86 bitfield_alias_bits $@87 "end of code block"  */
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

  case 781: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 782: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 783: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 784: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 785: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 786: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 787: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 788: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 789: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 790: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 791: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 792: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 793: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 794: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 795: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 796: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 797: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 798: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 799: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 800: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 801: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 802: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 803: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 804: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 805: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 818: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 819: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 820: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 821: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 822: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 823: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 824: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 825: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 826: /* $@88: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 827: /* $@89: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 828: /* make_struct_decl: "struct" '<' $@88 type_declaration_no_options '>' $@89 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 829: /* $@90: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 830: /* $@91: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 831: /* make_struct_decl: "class" '<' $@90 type_declaration_no_options '>' $@91 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 832: /* $@92: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 833: /* $@93: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 834: /* make_struct_decl: "variant" '<' $@92 type_declaration_no_options '>' $@93 '(' make_variant_dim ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 835: /* $@94: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 836: /* $@95: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 837: /* make_struct_decl: "default" '<' $@94 type_declaration_no_options '>' $@95 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 838: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 839: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 840: /* make_tuple: make_tuple ',' expr  */
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

  case 841: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 842: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 843: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 844: /* $@96: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 845: /* $@97: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 846: /* make_tuple_call: "tuple" '<' $@96 type_declaration_no_options '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 847: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 848: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 849: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 850: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 851: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 852: /* $@98: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 853: /* $@99: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 854: /* make_dim_decl: "array" "struct" '<' $@98 type_declaration_no_options '>' $@99 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 855: /* $@100: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 856: /* $@101: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 857: /* make_dim_decl: "array" "tuple" '<' $@100 type_declaration_no_options '>' $@101 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 858: /* $@102: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 859: /* $@103: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 860: /* make_dim_decl: "array" "variant" '<' $@102 type_declaration_no_options '>' $@103 '(' make_variant_dim ')'  */
                                                                                                                                                                               {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 861: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 862: /* $@104: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 863: /* $@105: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 864: /* make_dim_decl: "array" '<' $@104 type_declaration_no_options '>' $@105 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = make_smart<TypeDecl>(Type::tArray);
            msd->makeType->firstType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
            msd->at = tokAt(scanner,(yylsp[-5]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 865: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 866: /* $@106: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 867: /* $@107: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 868: /* make_dim_decl: "fixed_array" '<' $@106 type_declaration_no_options '>' $@107 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 869: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 870: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 871: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 872: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 873: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 874: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 875: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 876: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                 {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-6])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->makeType = make_smart<TypeDecl>(Type::tTable);
            msd->makeType->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
            msd->makeType->secondType = make_smart<TypeDecl>(Type::tVoid);
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 877: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                                                             {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = make_smart<TypeDecl>(Type::tTuple);
            mka->makeType->argTypes.push_back((yyvsp[-6].pTypeDecl));
            mka->makeType->argTypes.push_back((yyvsp[-4].pTypeDecl));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = make_smart<TypeDecl>(Type::tTable);
            msd->makeType->firstType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
            msd->makeType->secondType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 878: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 879: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 880: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 881: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 882: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 883: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 884: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 885: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 886: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 887: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
                                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,true);
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


