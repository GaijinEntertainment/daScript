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
  YYSYMBOL_DAS_TEMPLATE = 107,             /* "template"  */
  YYSYMBOL_ADDEQU = 108,                   /* "+="  */
  YYSYMBOL_SUBEQU = 109,                   /* "-="  */
  YYSYMBOL_DIVEQU = 110,                   /* "/="  */
  YYSYMBOL_MULEQU = 111,                   /* "*="  */
  YYSYMBOL_MODEQU = 112,                   /* "%="  */
  YYSYMBOL_ANDEQU = 113,                   /* "&="  */
  YYSYMBOL_OREQU = 114,                    /* "|="  */
  YYSYMBOL_XOREQU = 115,                   /* "^="  */
  YYSYMBOL_SHL = 116,                      /* "<<"  */
  YYSYMBOL_SHR = 117,                      /* ">>"  */
  YYSYMBOL_ADDADD = 118,                   /* "++"  */
  YYSYMBOL_SUBSUB = 119,                   /* "--"  */
  YYSYMBOL_LEEQU = 120,                    /* "<="  */
  YYSYMBOL_SHLEQU = 121,                   /* "<<="  */
  YYSYMBOL_SHREQU = 122,                   /* ">>="  */
  YYSYMBOL_GREQU = 123,                    /* ">="  */
  YYSYMBOL_EQUEQU = 124,                   /* "=="  */
  YYSYMBOL_NOTEQU = 125,                   /* "!="  */
  YYSYMBOL_RARROW = 126,                   /* "->"  */
  YYSYMBOL_LARROW = 127,                   /* "<-"  */
  YYSYMBOL_QQ = 128,                       /* "??"  */
  YYSYMBOL_QDOT = 129,                     /* "?."  */
  YYSYMBOL_QBRA = 130,                     /* "?["  */
  YYSYMBOL_LPIPE = 131,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 132,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 133,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 134,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 135,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 136,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 137,                 /* ":="  */
  YYSYMBOL_ROTL = 138,                     /* "<<<"  */
  YYSYMBOL_ROTR = 139,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 140,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 141,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 142,                    /* "=>"  */
  YYSYMBOL_COLCOL = 143,                   /* "::"  */
  YYSYMBOL_ANDAND = 144,                   /* "&&"  */
  YYSYMBOL_OROR = 145,                     /* "||"  */
  YYSYMBOL_XORXOR = 146,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 147,                /* "&&="  */
  YYSYMBOL_OROREQU = 148,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 149,                /* "^^="  */
  YYSYMBOL_DOTDOT = 150,                   /* ".."  */
  YYSYMBOL_MTAG_E = 151,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 152,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 153,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 154,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 155,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 156,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 157,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 158,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 159,           /* "..."  */
  YYSYMBOL_BRABRAB = 160,                  /* "[["  */
  YYSYMBOL_BRACBRB = 161,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 162,                  /* "{{"  */
  YYSYMBOL_INTEGER = 163,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 164,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 165,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 166,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 167,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 168,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 169,                   /* "double constant"  */
  YYSYMBOL_NAME = 170,                     /* "name"  */
  YYSYMBOL_KEYWORD = 171,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 172,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 173,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 174,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 175,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 176,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 177,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 178,          /* "}"  */
  YYSYMBOL_END_OF_READ = 179,              /* "end of failed eader macro"  */
  YYSYMBOL_180_begin_of_code_block_ = 180, /* "begin of code block"  */
  YYSYMBOL_181_end_of_code_block_ = 181,   /* "end of code block"  */
  YYSYMBOL_182_end_of_expression_ = 182,   /* "end of expression"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 183,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 184,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 185,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 186,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 187,            /* ",}]"  */
  YYSYMBOL_188_ = 188,                     /* ','  */
  YYSYMBOL_189_ = 189,                     /* '='  */
  YYSYMBOL_190_ = 190,                     /* '?'  */
  YYSYMBOL_191_ = 191,                     /* ':'  */
  YYSYMBOL_192_ = 192,                     /* '|'  */
  YYSYMBOL_193_ = 193,                     /* '^'  */
  YYSYMBOL_194_ = 194,                     /* '&'  */
  YYSYMBOL_195_ = 195,                     /* '<'  */
  YYSYMBOL_196_ = 196,                     /* '>'  */
  YYSYMBOL_197_ = 197,                     /* '-'  */
  YYSYMBOL_198_ = 198,                     /* '+'  */
  YYSYMBOL_199_ = 199,                     /* '*'  */
  YYSYMBOL_200_ = 200,                     /* '/'  */
  YYSYMBOL_201_ = 201,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 202,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 203,               /* UNARY_PLUS  */
  YYSYMBOL_204_ = 204,                     /* '~'  */
  YYSYMBOL_205_ = 205,                     /* '!'  */
  YYSYMBOL_PRE_INC = 206,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 207,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 208,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 209,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 210,                    /* DEREF  */
  YYSYMBOL_211_ = 211,                     /* '.'  */
  YYSYMBOL_212_ = 212,                     /* '['  */
  YYSYMBOL_213_ = 213,                     /* ']'  */
  YYSYMBOL_214_ = 214,                     /* '('  */
  YYSYMBOL_215_ = 215,                     /* ')'  */
  YYSYMBOL_216_ = 216,                     /* '$'  */
  YYSYMBOL_217_ = 217,                     /* '@'  */
  YYSYMBOL_218_ = 218,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 219,                 /* $accept  */
  YYSYMBOL_program = 220,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 221,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 222, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 223,              /* module_name  */
  YYSYMBOL_module_declaration = 224,       /* module_declaration  */
  YYSYMBOL_character_sequence = 225,       /* character_sequence  */
  YYSYMBOL_string_constant = 226,          /* string_constant  */
  YYSYMBOL_string_builder_body = 227,      /* string_builder_body  */
  YYSYMBOL_string_builder = 228,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 229, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 230,              /* expr_reader  */
  YYSYMBOL_231_1 = 231,                    /* $@1  */
  YYSYMBOL_options_declaration = 232,      /* options_declaration  */
  YYSYMBOL_require_declaration = 233,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 234,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 235,      /* require_module_name  */
  YYSYMBOL_require_module = 236,           /* require_module  */
  YYSYMBOL_is_public_module = 237,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 238,       /* expect_declaration  */
  YYSYMBOL_expect_list = 239,              /* expect_list  */
  YYSYMBOL_expect_error = 240,             /* expect_error  */
  YYSYMBOL_expression_label = 241,         /* expression_label  */
  YYSYMBOL_expression_goto = 242,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 243,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 244,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 245,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 246, /* expression_else_one_liner  */
  YYSYMBOL_247_2 = 247,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 248,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 249,  /* expression_if_then_else  */
  YYSYMBOL_250_3 = 250,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 251,      /* expression_for_loop  */
  YYSYMBOL_252_4 = 252,                    /* $@4  */
  YYSYMBOL_expression_unsafe = 253,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 254,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 255,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 256,    /* expression_with_alias  */
  YYSYMBOL_257_5 = 257,                    /* $@5  */
  YYSYMBOL_annotation_argument_value = 258, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 259, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 260, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 261,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 262, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 263,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 264, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 265, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 266,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 267,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 268, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 269, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 270,   /* optional_function_type  */
  YYSYMBOL_function_name = 271,            /* function_name  */
  YYSYMBOL_global_function_declaration = 272, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 273, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 274, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 275,     /* function_declaration  */
  YYSYMBOL_276_6 = 276,                    /* $@6  */
  YYSYMBOL_expression_block = 277,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 278,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 279,           /* expression_any  */
  YYSYMBOL_expressions = 280,              /* expressions  */
  YYSYMBOL_expr_keyword = 281,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 282,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 283, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 284, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 285, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 286,       /* expression_keyword  */
  YYSYMBOL_287_7 = 287,                    /* $@7  */
  YYSYMBOL_288_8 = 288,                    /* $@8  */
  YYSYMBOL_289_9 = 289,                    /* $@9  */
  YYSYMBOL_290_10 = 290,                   /* $@10  */
  YYSYMBOL_expr_pipe = 291,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 292,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 293,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 294,     /* new_type_declaration  */
  YYSYMBOL_295_11 = 295,                   /* $@11  */
  YYSYMBOL_296_12 = 296,                   /* $@12  */
  YYSYMBOL_expr_new = 297,                 /* expr_new  */
  YYSYMBOL_expression_break = 298,         /* expression_break  */
  YYSYMBOL_expression_continue = 299,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 300, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 301,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 302, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 303,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 304,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 305,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 306,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 307,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 308,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 309, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 310,           /* expression_let  */
  YYSYMBOL_expr_cast = 311,                /* expr_cast  */
  YYSYMBOL_312_13 = 312,                   /* $@13  */
  YYSYMBOL_313_14 = 313,                   /* $@14  */
  YYSYMBOL_314_15 = 314,                   /* $@15  */
  YYSYMBOL_315_16 = 315,                   /* $@16  */
  YYSYMBOL_316_17 = 316,                   /* $@17  */
  YYSYMBOL_317_18 = 317,                   /* $@18  */
  YYSYMBOL_expr_type_decl = 318,           /* expr_type_decl  */
  YYSYMBOL_319_19 = 319,                   /* $@19  */
  YYSYMBOL_320_20 = 320,                   /* $@20  */
  YYSYMBOL_expr_type_info = 321,           /* expr_type_info  */
  YYSYMBOL_expr_list = 322,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 323,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 324,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 325,            /* capture_entry  */
  YYSYMBOL_capture_list = 326,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 327,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 328,               /* expr_block  */
  YYSYMBOL_expr_full_block = 329,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 330, /* expr_full_block_assumed_piped  */
  YYSYMBOL_331_21 = 331,                   /* $@21  */
  YYSYMBOL_expr_numeric_const = 332,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 333,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 334,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 335,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 336,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 337,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 338,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 339,           /* func_addr_expr  */
  YYSYMBOL_340_22 = 340,                   /* $@22  */
  YYSYMBOL_341_23 = 341,                   /* $@23  */
  YYSYMBOL_342_24 = 342,                   /* $@24  */
  YYSYMBOL_343_25 = 343,                   /* $@25  */
  YYSYMBOL_expr_field = 344,               /* expr_field  */
  YYSYMBOL_345_26 = 345,                   /* $@26  */
  YYSYMBOL_346_27 = 346,                   /* $@27  */
  YYSYMBOL_expr_call = 347,                /* expr_call  */
  YYSYMBOL_expr = 348,                     /* expr  */
  YYSYMBOL_349_28 = 349,                   /* $@28  */
  YYSYMBOL_350_29 = 350,                   /* $@29  */
  YYSYMBOL_351_30 = 351,                   /* $@30  */
  YYSYMBOL_352_31 = 352,                   /* $@31  */
  YYSYMBOL_353_32 = 353,                   /* $@32  */
  YYSYMBOL_354_33 = 354,                   /* $@33  */
  YYSYMBOL_expr_mtag = 355,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 356, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 357,        /* optional_override  */
  YYSYMBOL_optional_constant = 358,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 359, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 360, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 361, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 362, /* struct_variable_declaration_list  */
  YYSYMBOL_363_34 = 363,                   /* $@34  */
  YYSYMBOL_364_35 = 364,                   /* $@35  */
  YYSYMBOL_365_36 = 365,                   /* $@36  */
  YYSYMBOL_function_argument_declaration = 366, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 367,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 368,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 369,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 370,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 371,             /* variant_type  */
  YYSYMBOL_variant_type_list = 372,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 373,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 374,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 375,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 376,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 377,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 378, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 379, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 380, /* global_variable_declaration_list  */
  YYSYMBOL_381_37 = 381,                   /* $@37  */
  YYSYMBOL_optional_shared = 382,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 383, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 384,               /* global_let  */
  YYSYMBOL_385_38 = 385,                   /* $@38  */
  YYSYMBOL_enum_list = 386,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 387, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 388,             /* single_alias  */
  YYSYMBOL_389_39 = 389,                   /* $@39  */
  YYSYMBOL_alias_list = 390,               /* alias_list  */
  YYSYMBOL_alias_declaration = 391,        /* alias_declaration  */
  YYSYMBOL_392_40 = 392,                   /* $@40  */
  YYSYMBOL_optional_public_or_private_enum = 393, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 394,                /* enum_name  */
  YYSYMBOL_enum_declaration = 395,         /* enum_declaration  */
  YYSYMBOL_396_41 = 396,                   /* $@41  */
  YYSYMBOL_397_42 = 397,                   /* $@42  */
  YYSYMBOL_398_43 = 398,                   /* $@43  */
  YYSYMBOL_399_44 = 399,                   /* $@44  */
  YYSYMBOL_optional_structure_parent = 400, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 401,          /* optional_sealed  */
  YYSYMBOL_structure_name = 402,           /* structure_name  */
  YYSYMBOL_class_or_struct = 403,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 404, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 405, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 406,    /* structure_declaration  */
  YYSYMBOL_407_45 = 407,                   /* $@45  */
  YYSYMBOL_408_46 = 408,                   /* $@46  */
  YYSYMBOL_variable_name_with_pos_list = 409, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 410,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 411, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 412, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 413,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 414,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 415,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 416, /* bitfield_type_declaration  */
  YYSYMBOL_417_47 = 417,                   /* $@47  */
  YYSYMBOL_418_48 = 418,                   /* $@48  */
  YYSYMBOL_c_or_s = 419,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 420,          /* table_type_pair  */
  YYSYMBOL_dim_list = 421,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 422, /* type_declaration_no_options  */
  YYSYMBOL_423_49 = 423,                   /* $@49  */
  YYSYMBOL_424_50 = 424,                   /* $@50  */
  YYSYMBOL_425_51 = 425,                   /* $@51  */
  YYSYMBOL_426_52 = 426,                   /* $@52  */
  YYSYMBOL_427_53 = 427,                   /* $@53  */
  YYSYMBOL_428_54 = 428,                   /* $@54  */
  YYSYMBOL_429_55 = 429,                   /* $@55  */
  YYSYMBOL_430_56 = 430,                   /* $@56  */
  YYSYMBOL_431_57 = 431,                   /* $@57  */
  YYSYMBOL_432_58 = 432,                   /* $@58  */
  YYSYMBOL_433_59 = 433,                   /* $@59  */
  YYSYMBOL_434_60 = 434,                   /* $@60  */
  YYSYMBOL_435_61 = 435,                   /* $@61  */
  YYSYMBOL_436_62 = 436,                   /* $@62  */
  YYSYMBOL_437_63 = 437,                   /* $@63  */
  YYSYMBOL_438_64 = 438,                   /* $@64  */
  YYSYMBOL_439_65 = 439,                   /* $@65  */
  YYSYMBOL_440_66 = 440,                   /* $@66  */
  YYSYMBOL_441_67 = 441,                   /* $@67  */
  YYSYMBOL_442_68 = 442,                   /* $@68  */
  YYSYMBOL_443_69 = 443,                   /* $@69  */
  YYSYMBOL_444_70 = 444,                   /* $@70  */
  YYSYMBOL_445_71 = 445,                   /* $@71  */
  YYSYMBOL_446_72 = 446,                   /* $@72  */
  YYSYMBOL_447_73 = 447,                   /* $@73  */
  YYSYMBOL_448_74 = 448,                   /* $@74  */
  YYSYMBOL_449_75 = 449,                   /* $@75  */
  YYSYMBOL_type_declaration = 450,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 451,  /* tuple_alias_declaration  */
  YYSYMBOL_452_76 = 452,                   /* $@76  */
  YYSYMBOL_453_77 = 453,                   /* $@77  */
  YYSYMBOL_454_78 = 454,                   /* $@78  */
  YYSYMBOL_455_79 = 455,                   /* $@79  */
  YYSYMBOL_variant_alias_declaration = 456, /* variant_alias_declaration  */
  YYSYMBOL_457_80 = 457,                   /* $@80  */
  YYSYMBOL_458_81 = 458,                   /* $@81  */
  YYSYMBOL_459_82 = 459,                   /* $@82  */
  YYSYMBOL_460_83 = 460,                   /* $@83  */
  YYSYMBOL_bitfield_alias_declaration = 461, /* bitfield_alias_declaration  */
  YYSYMBOL_462_84 = 462,                   /* $@84  */
  YYSYMBOL_463_85 = 463,                   /* $@85  */
  YYSYMBOL_464_86 = 464,                   /* $@86  */
  YYSYMBOL_465_87 = 465,                   /* $@87  */
  YYSYMBOL_make_decl = 466,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 467,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 468,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 469,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 470,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 471,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 472,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 473, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 474,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 475, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 476, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 477, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 478, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 479, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 480,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 481,         /* make_struct_decl  */
  YYSYMBOL_482_88 = 482,                   /* $@88  */
  YYSYMBOL_483_89 = 483,                   /* $@89  */
  YYSYMBOL_484_90 = 484,                   /* $@90  */
  YYSYMBOL_485_91 = 485,                   /* $@91  */
  YYSYMBOL_486_92 = 486,                   /* $@92  */
  YYSYMBOL_487_93 = 487,                   /* $@93  */
  YYSYMBOL_488_94 = 488,                   /* $@94  */
  YYSYMBOL_489_95 = 489,                   /* $@95  */
  YYSYMBOL_make_tuple = 490,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 491,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 492,          /* make_tuple_call  */
  YYSYMBOL_493_96 = 493,                   /* $@96  */
  YYSYMBOL_494_97 = 494,                   /* $@97  */
  YYSYMBOL_make_dim = 495,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 496,            /* make_dim_decl  */
  YYSYMBOL_497_98 = 497,                   /* $@98  */
  YYSYMBOL_498_99 = 498,                   /* $@99  */
  YYSYMBOL_499_100 = 499,                  /* $@100  */
  YYSYMBOL_500_101 = 500,                  /* $@101  */
  YYSYMBOL_501_102 = 501,                  /* $@102  */
  YYSYMBOL_502_103 = 502,                  /* $@103  */
  YYSYMBOL_503_104 = 503,                  /* $@104  */
  YYSYMBOL_504_105 = 504,                  /* $@105  */
  YYSYMBOL_505_106 = 505,                  /* $@106  */
  YYSYMBOL_506_107 = 506,                  /* $@107  */
  YYSYMBOL_make_table = 507,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 508,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 509,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 510, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 511,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 512       /* array_comprehension  */
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
#define YYLAST   15826

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  219
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  294
/* YYNRULES -- Number of rules.  */
#define YYNRULES  899
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1704

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   446


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
       2,     2,     2,   205,     2,   218,   216,   201,   194,     2,
     214,   215,   199,   198,   188,   197,   211,   200,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   191,   182,
     195,   189,   196,   190,   217,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   212,     2,   213,   193,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   180,   192,   181,   204,     2,     2,     2,
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
     175,   176,   177,   178,   179,   183,   184,   185,   186,   187,
     202,   203,   206,   207,   208,   209,   210
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   551,   551,   552,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   573,   579,   580,
     581,   585,   586,   590,   608,   609,   610,   611,   615,   616,
     620,   625,   634,   642,   658,   663,   671,   671,   710,   740,
     744,   745,   746,   750,   753,   757,   763,   772,   775,   781,
     782,   786,   790,   791,   795,   798,   804,   810,   813,   819,
     820,   824,   825,   826,   835,   836,   840,   841,   841,   847,
     848,   849,   850,   851,   855,   861,   861,   867,   867,   873,
     881,   891,   900,   900,   907,   908,   909,   910,   911,   912,
     916,   921,   929,   930,   931,   935,   936,   937,   938,   939,
     940,   941,   942,   948,   951,   957,   960,   963,   969,   970,
     971,   972,   976,   989,  1007,  1010,  1018,  1029,  1040,  1051,
    1054,  1061,  1065,  1072,  1073,  1077,  1078,  1079,  1083,  1086,
    1093,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,
    1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,
    1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,
    1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,
    1136,  1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,
    1146,  1147,  1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,
    1156,  1157,  1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,
    1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,
    1176,  1177,  1178,  1179,  1180,  1185,  1203,  1204,  1205,  1209,
    1215,  1215,  1232,  1236,  1247,  1256,  1265,  1271,  1272,  1273,
    1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,  1283,
    1284,  1285,  1286,  1287,  1288,  1289,  1290,  1291,  1295,  1300,
    1306,  1312,  1323,  1324,  1328,  1329,  1333,  1334,  1338,  1342,
    1349,  1349,  1349,  1355,  1355,  1355,  1364,  1398,  1401,  1404,
    1407,  1413,  1414,  1425,  1429,  1432,  1440,  1440,  1440,  1443,
    1449,  1452,  1456,  1460,  1467,  1474,  1480,  1484,  1488,  1491,
    1494,  1502,  1505,  1508,  1516,  1519,  1527,  1530,  1533,  1541,
    1547,  1548,  1549,  1553,  1554,  1558,  1559,  1563,  1568,  1576,
    1582,  1588,  1594,  1600,  1609,  1618,  1627,  1639,  1642,  1648,
    1648,  1648,  1651,  1651,  1651,  1656,  1656,  1656,  1664,  1664,
    1664,  1670,  1680,  1691,  1704,  1714,  1725,  1740,  1743,  1749,
    1750,  1757,  1768,  1769,  1770,  1774,  1775,  1776,  1777,  1778,
    1782,  1787,  1795,  1796,  1797,  1801,  1806,  1813,  1820,  1820,
    1829,  1830,  1831,  1832,  1833,  1834,  1835,  1839,  1840,  1841,
    1842,  1843,  1844,  1845,  1846,  1847,  1848,  1849,  1850,  1851,
    1852,  1853,  1854,  1855,  1856,  1857,  1861,  1862,  1863,  1864,
    1869,  1870,  1871,  1872,  1873,  1874,  1875,  1876,  1877,  1878,
    1879,  1880,  1881,  1882,  1883,  1884,  1885,  1890,  1897,  1909,
    1914,  1924,  1928,  1935,  1938,  1938,  1938,  1943,  1943,  1943,
    1956,  1960,  1964,  1969,  1976,  1985,  1990,  1997,  1997,  1997,
    2004,  2008,  2018,  2027,  2036,  2040,  2043,  2049,  2050,  2051,
    2052,  2053,  2054,  2055,  2056,  2057,  2058,  2059,  2060,  2061,
    2062,  2063,  2064,  2065,  2066,  2067,  2068,  2069,  2070,  2071,
    2072,  2073,  2074,  2075,  2076,  2077,  2078,  2079,  2080,  2081,
    2082,  2083,  2084,  2090,  2091,  2092,  2093,  2094,  2107,  2116,
    2117,  2118,  2119,  2120,  2121,  2122,  2123,  2124,  2125,  2126,
    2127,  2130,  2133,  2134,  2137,  2137,  2137,  2140,  2145,  2149,
    2153,  2153,  2153,  2158,  2161,  2165,  2165,  2165,  2170,  2173,
    2174,  2175,  2176,  2177,  2178,  2179,  2180,  2181,  2183,  2187,
    2188,  2193,  2197,  2198,  2199,  2200,  2201,  2202,  2203,  2207,
    2211,  2215,  2219,  2223,  2227,  2231,  2235,  2239,  2246,  2247,
    2248,  2252,  2253,  2254,  2258,  2259,  2263,  2264,  2265,  2269,
    2270,  2274,  2285,  2288,  2288,  2307,  2306,  2322,  2321,  2335,
    2344,  2353,  2363,  2364,  2368,  2371,  2380,  2381,  2385,  2388,
    2391,  2407,  2416,  2417,  2421,  2424,  2427,  2441,  2442,  2446,
    2452,  2458,  2461,  2465,  2471,  2480,  2481,  2482,  2486,  2487,
    2491,  2498,  2503,  2512,  2518,  2529,  2532,  2537,  2542,  2550,
    2561,  2564,  2564,  2584,  2585,  2589,  2590,  2591,  2595,  2598,
    2598,  2617,  2620,  2623,  2638,  2657,  2658,  2659,  2664,  2664,
    2690,  2691,  2695,  2696,  2696,  2700,  2701,  2702,  2706,  2716,
    2721,  2716,  2733,  2738,  2733,  2753,  2754,  2758,  2759,  2763,
    2769,  2770,  2771,  2772,  2776,  2777,  2778,  2782,  2785,  2791,
    2796,  2791,  2816,  2823,  2828,  2837,  2843,  2854,  2855,  2856,
    2857,  2858,  2859,  2860,  2861,  2862,  2863,  2864,  2865,  2866,
    2867,  2868,  2869,  2870,  2871,  2872,  2873,  2874,  2875,  2876,
    2877,  2878,  2879,  2880,  2884,  2885,  2886,  2887,  2888,  2889,
    2890,  2891,  2895,  2906,  2910,  2917,  2929,  2936,  2945,  2950,
    2953,  2966,  2966,  2966,  2979,  2980,  2984,  2988,  2995,  2999,
    3006,  3007,  3008,  3009,  3010,  3025,  3031,  3031,  3031,  3035,
    3040,  3047,  3047,  3054,  3058,  3062,  3067,  3072,  3077,  3082,
    3086,  3090,  3095,  3099,  3103,  3108,  3108,  3108,  3114,  3121,
    3121,  3121,  3126,  3126,  3126,  3132,  3132,  3132,  3137,  3141,
    3141,  3141,  3146,  3146,  3146,  3155,  3159,  3159,  3159,  3164,
    3164,  3164,  3173,  3177,  3177,  3177,  3182,  3182,  3182,  3191,
    3191,  3191,  3197,  3197,  3197,  3206,  3209,  3220,  3236,  3236,
    3241,  3246,  3236,  3271,  3271,  3276,  3282,  3271,  3307,  3307,
    3312,  3317,  3307,  3347,  3348,  3349,  3350,  3351,  3355,  3362,
    3369,  3375,  3381,  3388,  3395,  3401,  3410,  3416,  3424,  3429,
    3436,  3441,  3448,  3453,  3459,  3460,  3464,  3465,  3470,  3471,
    3475,  3476,  3480,  3481,  3485,  3486,  3487,  3491,  3492,  3493,
    3497,  3498,  3502,  3508,  3515,  3523,  3530,  3538,  3547,  3547,
    3547,  3555,  3555,  3555,  3562,  3562,  3562,  3569,  3569,  3569,
    3580,  3583,  3589,  3603,  3609,  3615,  3621,  3621,  3621,  3631,
    3636,  3643,  3652,  3657,  3664,  3664,  3664,  3674,  3674,  3674,
    3684,  3684,  3684,  3694,  3703,  3703,  3703,  3723,  3730,  3730,
    3730,  3740,  3745,  3752,  3755,  3761,  3769,  3778,  3786,  3806,
    3831,  3832,  3836,  3837,  3842,  3845,  3848,  3851,  3854,  3857
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
  "\"variant\"", "\"generator\"", "\"yield\"", "\"sealed\"",
  "\"template\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"",
  "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"",
  "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"",
  "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"$ <|\"",
  "\"@ <|\"", "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"",
  "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"",
  "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"",
  "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"",
  "\"[{\"", "\"{{\"", "\"integer constant\"", "\"long integer constant\"",
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

#define YYPACT_NINF (-1481)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-767)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1481,   128, -1481, -1481,    47,   -58,   202,   101, -1481,    35,
     684,   684,   684, -1481,   183,   423, -1481, -1481,   123, -1481,
   -1481, -1481,   303, -1481,   158, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481,   161, -1481,   288,   185,   296,
   -1481, -1481, -1481, -1481,   202, -1481,    32, -1481,   684,   684,
   -1481, -1481,   158, -1481, -1481, -1481, -1481, -1481,   379,   367,
   -1481, -1481, -1481, -1481,   423,   423,   423,   343, -1481,   692,
     -91, -1481, -1481, -1481, -1481,   704,   705,   174,   720, -1481,
     725,    33,    47,   434,   -58,   393,   455, -1481,   713,   713,
   -1481,   485,   430,    13,   486,   729,   533,   535,   538, -1481,
     545,   519, -1481, -1481,   -62,    47,   423,   423,   423,   423,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481,   576, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481,   579, -1481, -1481, -1481, -1481,
   -1481,   773,   126, -1481, -1481, -1481, -1481,   716, -1481, -1481,
   -1481, -1481, -1481,   603, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481,   601, -1481,   220, -1481,   544,   652,   692,
   15656, -1481,   298,   699, -1481,   -73, -1481, -1481, -1481,   783,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481,   252, -1481,   660,
   -1481,   663,   685,   696, -1481, -1481, 14412, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481,   829,   837, -1481,   679,   697, -1481,   523, -1481,
     728, -1481,   714,    47,    47,    75,    69, -1481, -1481, -1481,
     126, -1481, 10449, -1481, -1481, -1481,   738,   740, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,   743,   694,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481,   892, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481,   746,   707, -1481, -1481,    86,
     734, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481,   750,   742,   751, -1481,   -73,   169, -1481, -1481,
      47,   703,   895,   487, -1481, -1481,   744,   747,   749,   721,
     755,   757, -1481, -1481, -1481,   727, -1481, -1481, -1481, -1481,
   -1481,   758, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481,   760, -1481, -1481, -1481,   765,   767, -1481,
   -1481, -1481, -1481,   768,   774,   731,   183, -1481, -1481, -1481,
   -1481, -1481,   910,   787, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481,   805,   840, -1481,   769, -1481,    97, -1481,   146, 10449,
   -1481,  1817,   272, -1481,   183, -1481, -1481, -1481,    69,   772,
   -1481,  9601,   810,   811, 10449, -1481,    73, -1481, -1481, -1481,
    9601, -1481, -1481,   812, -1481,   396,   415,   418, -1481, -1481,
    9601,  -105, -1481, -1481, -1481,     8, -1481, -1481, -1481,    30,
    5943, -1481,   775, 10203,   395, 10303,   216, -1481, -1481,  9601,
   -1481, -1481,   193,   -74, -1481,   787, -1481,   798,   799,  9601,
   -1481, -1481, -1481, -1481, -1481,   766,   -69,   800,    31,  3296,
   -1481, -1481,   697,    72,  6146,   782,  9601,   827,   804,   807,
     789, -1481,   822,   813,   844,  6349,   -84,   204,   814, -1481,
     386,   815,   816,  3499,  9601,  9601,   127,   127,   127,   801,
     802,   803,   806,   808,   817, -1481,  2062, 10103,  6554, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481,  6757,   818, -1481,  6962,
     984, -1481,  9601,  9601,  9601,  9601,  9601,  5131,  7165, -1481,
     809, -1481, -1481,   842,   843,  9601,  1017, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481,   428, -1481,   -82,   846,
   -1481,   847,   848,   850, -1481,   851, -1481, -1481,   969, -1481,
   -1481, -1481, -1481,   824, -1481, -1481,   -55, -1481, -1481, -1481,
   -1481, -1481, -1481,  9914, -1481,   823, -1481, -1481, -1481, -1481,
   -1481, -1481,   453, -1481,   858, -1481, -1481,    38, -1481, -1481,
     826,   849,   852, -1481, 11446, -1481,   998,  1064, -1481, -1481,
   -1481,  3905, 10449, 10449, 10449, 11481, 10449, 10449,   828,   872,
   10449,   679, 10449,   679, 10449,   679, 10549,   875, 11581, -1481,
    9601, -1481, -1481, -1481, -1481,   833, -1481, -1481, 13931,  9601,
   -1481,   910,   866, -1481,   868,   -32, -1481, -1481,   607, -1481,
     787,   870,   861,   607, -1481,   877, 11681,   845,  1018, -1481,
     -24, -1481, -1481, -1481, 14447,   193, -1481,   853, -1481, -1481,
     183,   424, -1481,   874,   876,   878, -1481,  9601,  3905, -1481,
     887,   931, 10632,  1051, 10449,  9601,  9601, 15017,  9601, 14447,
     889, -1481, -1481,  9601, -1481, -1481,   885,   917, 15017,  9601,
   -1481, -1481,  9601, -1481, -1481,  9601, -1481, 10449,  3905, -1481,
   10632,    57,    57,   865, -1481,   824, -1481, -1481, -1481,  9601,
    9601,  9601,  9601,  9601,  9601,   193,  2684,   193,  2888,   193,
   14547, -1481,   785, -1481, 14447, -1481,   726,   193, -1481,   896,
     906,    57,    57,   -72,    57,    57,   193,  1075,   902, 15017,
     880,   244,   902,   903,   881,   404, -1481, -1481, 14447, -1481,
   -1481, -1481, -1481,  4108, -1481, -1481, -1481, -1481, -1481, -1481,
     299,    51,   127, -1481, 15420, 15451,  4311,  4311,  4311,  4311,
    4311,  4311,  4311,  4311,  9601,  9601, -1481, -1481,  9601,  4311,
    4311,  9601,  9601,  9601,   922,  4311,  9601,   450,  9601,  9601,
    9601,  9601,  9601,  9601,  4311,  4311,  9601,  9601,  9601,  4311,
    4311,  4311,  9601,  4311,  7368,  9601,  9601,  9601,  9601,  9601,
    9601,  9601,  9601,  9601,  9601, 15578,  9601, -1481,  7571, -1481,
    9601, -1481,   272, -1481,   423,  1087,   -73, 10449, -1481,   928,
   -1481,  3905, -1481, 10745,   -78,    87,   909,   525, -1481,   145,
     499, -1481, -1481,   201,   542,   734,   578,   734,   654,   734,
   -1481,   -76, -1481,   248, -1481, 10449,   894,   902, -1481, -1481,
   14031, -1481, -1481, 10449, -1481, -1481, 10449, -1481, -1481, -1481,
    9601,   932, -1481,   941, -1481, 10449, -1481,  3905, 10449, 10449,
   -1481,    27, 10449,  5334,  7774,   942,  9601, 10449, -1481, -1481,
   -1481, 10449,   902, -1481,   887,  9601,  9601,  9601,  9601,  9601,
    9601,  9601,  9601,  9601,  9601,  9601,  9601,  9601,  9601,  9601,
    9601,  9601,  9601,   697,    88,   899,   896, 15017, 11716, -1481,
   -1481, 10449, 10449, 11816, 10449, -1481, -1481, 11916, 10449,   902,
   10449, 10449,   902, 10449,   291, -1481, 10632, -1481,    51, 11951,
   12051, 12151, 12186, 12286, 12386,    39,   127,  3092,  4516,  5537,
   14647,   903,     9,   199,   929,   242,    40,  5740,     9,   555,
      41,  9601,   935,  9601, -1481, -1481, 10449, -1481, 10449, -1481,
    9601,   677,    45,  9601,   940, -1481,    48,   193,  9601,   913,
    9601,  9601,  9601,   908,   483, -1481,   915,   916,   475, -1481,
   -1481,   571,  9601,   824,   -34,  4721, -1481,   253,   912,   961,
     961, -1481, -1481,   919,   269,   679, -1481,   939,   921, -1481,
   -1481,   943,   923, -1481, -1481,   127,   127,   127, -1481, -1481,
   11335, -1481, 11335, -1481, 11335, -1481, 11335, -1481, 11335, -1481,
   11335, -1481, 11335, -1481, 11335,   135,   135,  1263, -1481, 11335,
   -1481, 11335,  1263,   531,   531,   925, -1481, 11335,   568,   926,
   -1481, 14066,   -13,   -13,   823, 15017,   135,   135, -1481, 11335,
   -1481, 11335,  2091, 15152,  9961, -1481, 11335, -1481, 11335, -1481,
   11335, 15117, -1481, 11335, 15547, 14682, 15252,  1032, 15278,  1263,
    1263,   680,   680,   568,   568,   568,   488,  9601,   927,   930,
     507,  9601,  1135,   933, 14166, -1481,   276, 14782,   964,   282,
     737,  1086,   960,   794, -1481, -1481, 10856, -1481, -1481, -1481,
   -1481, 10449, -1481, -1481,   989, -1481, -1481,   958, -1481,   968,
   -1481,   970, -1481, 10549, -1481,   875,   518,   787, -1481, -1481,
   -1481,   787,   787, 12421, -1481,  1122,   -63, -1481, 10632,  1259,
    1288,  9601,   674,   514,   278,   952,   957,  1003, 12521,   264,
   12621,   675, 10449, 10449, 10449,  1364,   965, 15017, 15017, 15017,
   15017, 15017, 15017, 15017, 15017, 15017, 15017, 15017, 15017, 15017,
   15017, 15017, 15017, 15017, 15017, -1481,   963, 10449, -1481, -1481,
   -1481,  9601,  1381,  1978, -1481,  2265, -1481,  2807,   971,  3011,
    3030,   974,  4308,    51,   679, -1481, -1481, -1481, -1481, -1481,
     976,  9601, -1481,  4926, 13931,    19,  9601,   514,   199, -1481,
   -1481,   966, -1481,  9601,  9601, -1481,   972, -1481,  9601,   514,
     564,   980, -1481, -1481,  9601, 15017, -1481, -1481,   528,   537,
   14882,  9601, -1481, -1481,  2481,  9601,    49, -1481, 12656, 15017,
   15017, -1481,   981,   186,  9601,  9601, 10449,   679,   697, -1481,
   -1481,  9601, -1481,  2232,    51,   188, -1481,   982,   289,  9804,
   -1481, -1481, -1481,   327,   336,   269,  1005,  1013,   985,  1014,
    1026, -1481,   330,   734, -1481,  9601, -1481,  9601, -1481, -1481,
   -1481,  7977,  9601, -1481,  1008,   986, -1481, -1481,  9601,   990,
   -1481, 14266,  9601,  8180,   991, -1481, 14301, -1481,  8383, -1481,
   -1481, -1481, -1481,  1016, -1481, -1481,   255, -1481,    25, -1481,
      51, -1481, -1481, -1481, -1481,   787, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
     992, 10449, -1481,  1037,  9601, -1481, -1481,   294, -1481,   993,
   -1481, -1481, -1481,   562, -1481,  1039,   996, -1481, -1481,  4434,
    4513,  4639, -1481, -1481,  9601,  4718, 15017, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481,   608,   734,  8586,   619,
       9,   199, 15017,   903, -1481, -1481, 15017,   929, -1481,   630,
       9,  1000, -1481, -1481, -1481, -1481,   631, -1481, -1481, -1481,
     632, -1481,   635,  9601,   190,  9601,  9601,  9601, 12756, 12856,
    4844,   734, -1481, 14447, -1481,  1029,   679, -1481,   999,  4721,
    1045,  1004,   442,   339, -1481, -1481,  1046, -1481, -1481,   269,
    1006,   -57, 10449, 12891, 10449, 12991, -1481,   340, 13091, -1481,
    9601,  1566,  9601, -1481, 13126,  4721, -1481,   345,  9601, -1481,
   -1481, -1481,   347, -1481,  1187,    25, -1481, -1481,   737,  1009,
   -1481, -1481, -1481,  9601,   787, -1481, 15017,  1010,  1020, -1481,
   -1481, -1481,  9601,  1052,  1040,  9601, -1481, -1481, -1481, -1481,
    1021,  1022,  1024,  9601,  9601,  9601,  1025,  1171,  1028,  1031,
    8789, -1481,   -57, -1481,   360,  9601,   199, -1481,  9601,   564,
   -1481,  9601,  9601,  1035,  9601,  9601,   651,  9601,  9601, 13226,
   15017, 15017, -1481, -1481, -1481,  1059,   571,  3702, -1481,   734,
   -1481,   375, -1481,   593, 10449,    73, -1481,  1041, -1481, -1481,
    8992, -1481, -1481,  4923, -1481,   682, -1481, -1481, -1481, 10449,
   13326, 13361, -1481,   383, -1481, 13461, -1481, -1481, -1481,  1187,
     193,  1042,  1171,  1171, 13561,  1066,  1049, 13596,  1050,  1056,
    1057,  9601, -1481,  9601,  1263,  1263,  1263,  9601, -1481, -1481,
    1171,   514, -1481, 13696, -1481, -1481, 14917, -1481, 14917, -1481,
    1078,  1263,  9601, -1481,  1078, 14917,  9601, 15017, 15017,   211,
     295, -1481, -1481,  9195,  9398, -1481, -1481, -1481, -1481, -1481,
   15017,   697,  1058, 10449,    73,  1214,  3905, -1481,  9601, 15117,
   -1481, -1481,   690, -1481, -1481,  1063, -1481, 15656, -1481, -1481,
   -1481,   102,   102, -1481,  9601,  9601, -1481,  1171,  1171,   514,
    1065,  1068,   902,   102,   903,  1071, -1481,  1212,  1074,  1091,
    1108,  1102,  1112,  1081, 14917,  9601,  9601, -1481,   295,  9601,
    9601, 15017, -1481, -1481,  1214,  3905,  3905, -1481, 10967,  1566,
   -1481, -1481, -1481, -1481,  1113, 15656,   514,   903,  1111, -1481,
    1085,  1088, 13796, 13831,   102,   102,  1089, -1481, -1481,  1090,
    1094, -1481,  9601,  1083,  1093,  1120, -1481,  1095, -1481, -1481,
    1098, 15017, 15017, -1481, 15017,  3905, -1481, 11078, -1481, 11189,
   -1481, -1481, -1481,   697,   370,  1100, -1481, -1481, -1481, -1481,
   -1481,  1097,  1101, -1481, -1481, -1481, 15017, -1481, -1481, -1481,
   -1481, -1481, -1481, 11300, -1481, -1481, -1481, -1481,   514, -1481,
   -1481, -1481,   376, -1481
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   123,     1,   303,     0,     0,     0,   623,   304,     0,
     615,   615,   615,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   603,     6,    11,     5,     4,    13,
      12,    14,    93,    94,    92,   101,   103,    38,    54,    51,
      52,    40,    41,    42,     0,    43,    49,    39,   615,   615,
      22,    21,   603,   617,   616,   788,   778,   783,     0,   271,
      36,   109,   110,   111,     0,     0,     0,   112,   114,   121,
       0,   108,    17,   641,   640,   216,   625,     0,   644,   604,
     605,     0,     0,     0,     0,    44,     0,    50,     0,     0,
      47,     0,     0,   615,     0,    18,     0,     0,     0,   273,
       0,     0,   120,   115,     0,     0,     0,     0,     0,     0,
     124,   218,   217,   220,   215,   627,   626,     0,   643,   642,
     646,   645,   649,   607,   606,   609,    99,   100,    97,    98,
      96,     0,     0,    95,   104,    55,    53,    49,    46,    45,
     618,   620,   622,     0,   624,    19,    20,    23,   789,   779,
     784,   272,    34,    37,   119,     0,   116,   117,   118,   122,
       0,   628,     0,   637,   600,   538,    24,    25,    29,     0,
      88,    89,    86,    87,    85,    84,    90,     0,    48,     0,
     621,     0,     0,     0,    35,   113,     0,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,     0,     0,   130,   125,     0,   629,     0,   638,
       0,   650,   601,     0,     0,   540,     0,    26,    27,    28,
       0,   102,     0,   790,   780,   785,   184,   185,   182,   133,
     134,   136,   135,   137,   138,   139,   140,   166,   167,   164,
     165,   157,   168,   169,   158,   155,   156,   183,   177,     0,
     181,   170,   171,   172,   173,   144,   145,   146,   141,   142,
     143,   154,     0,   160,   161,   159,   152,   153,   148,   147,
     149,   150,   151,   132,   131,   176,     0,   162,   163,   538,
     128,   248,   221,   611,   684,   687,   690,   691,   685,   688,
     686,   689,     0,   635,   647,   608,   538,     0,   105,   107,
       0,     0,   590,   588,   610,    91,     0,     0,     0,     0,
       0,     0,   657,   677,   658,   693,   659,   663,   664,   665,
     666,   683,   670,   671,   672,   673,   674,   675,   676,   678,
     679,   680,   681,   748,   662,   669,   682,   755,   762,   660,
     667,   661,   668,     0,     0,     0,     0,   692,   710,   713,
     711,   712,   775,   619,   698,   568,   574,   186,   187,   180,
     175,   188,   178,   174,     0,   126,   302,   562,     0,     0,
     219,     0,   630,   632,     0,   639,   552,   651,     0,     0,
     106,     0,     0,     0,     0,   589,     0,   716,   739,   742,
       0,   745,   735,     0,   701,   749,   756,   763,   769,   772,
       0,     0,   725,   730,   724,     0,   738,   734,   727,     0,
       0,   729,   714,     0,   791,   781,   786,   189,   179,     0,
     300,   301,     0,   538,   127,   129,   250,     0,     0,     0,
      64,    65,    77,   443,   444,     0,     0,     0,     0,   288,
     437,   286,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   287,     0,     0,     0,     0,     0,     0,     0,   683,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   527,     0,     0,     0,   360,
     362,   361,   363,   364,   365,   366,     0,     0,    30,     0,
     222,   227,     0,     0,     0,     0,     0,     0,     0,   342,
     343,   441,   440,     0,     0,     0,     0,   243,   238,   235,
     234,   236,   237,   270,   249,   229,   521,   228,   438,     0,
     512,    72,    73,    70,   241,    71,   242,   244,   306,   233,
     511,   510,   509,   123,   515,   439,     0,   230,   514,   513,
     485,   445,   486,   367,   446,     0,   442,   793,   797,   794,
     795,   796,     0,   612,     0,   611,   636,   553,   602,   539,
       0,     0,     0,   521,     0,   592,   593,     0,   586,   587,
     585,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   125,     0,   125,     0,   125,     0,     0,     0,   721,
     252,   732,   733,   726,   728,     0,   731,   715,     0,     0,
     777,   776,     0,   699,     0,   271,   705,   704,     0,   569,
     564,     0,     0,     0,   575,     0,     0,     0,   652,   560,
     579,   563,   838,   841,     0,     0,   276,   280,   279,   285,
       0,     0,   328,     0,     0,     0,   874,     0,     0,   292,
     289,     0,   337,     0,     0,   256,     0,   274,     0,     0,
       0,   319,   322,     0,   247,   325,     0,     0,    58,     0,
      79,   878,     0,   847,   856,     0,   844,     0,     0,   297,
     294,   473,   474,   343,   355,   123,   269,   267,   268,     0,
       0,     0,     0,     0,     0,     0,   816,     0,     0,     0,
     854,   881,     0,   260,     0,   263,     0,     0,   883,   892,
       0,   450,   449,   487,   448,   447,     0,     0,   892,   337,
       0,   271,   892,   807,     0,   344,   245,   246,     0,    75,
     358,   225,   519,     0,   232,   239,   240,   291,   296,   305,
       0,   352,     0,   231,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   475,   476,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,   224,     0,   613,
       0,   631,   633,   648,     0,     0,   538,     0,   591,     0,
     595,     0,   599,   367,     0,     0,     0,   706,   719,     0,
       0,   694,   696,     0,     0,   128,     0,   128,     0,   128,
     566,     0,   572,     0,   695,     0,     0,   892,   723,   708,
       0,   700,   792,     0,   570,   782,     0,   576,   787,   561,
       0,     0,   578,     0,   577,     0,   580,     0,     0,     0,
      80,     0,     0,   830,     0,     0,     0,     0,   864,   867,
     870,     0,   892,   293,   290,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   892,   275,     0,    81,
      82,     0,     0,     0,     0,    56,    57,     0,     0,   892,
       0,     0,   892,     0,     0,   298,   295,   344,   352,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     850,   808,   816,     0,   859,     0,     0,     0,   816,     0,
       0,     0,     0,     0,   819,   886,     0,   251,     0,    33,
       0,    31,     0,   893,     0,   248,     0,     0,   893,     0,
       0,     0,     0,     0,     0,   478,     0,     0,   414,   411,
     413,    61,     0,   123,     0,     0,   430,     0,     0,     0,
       0,   318,   317,     0,     0,   125,   266,     0,     0,   498,
     497,     0,     0,   499,   503,     0,     0,     0,   389,   398,
     377,   399,   378,   401,   380,   400,   379,   402,   381,   392,
     371,   393,   372,   394,   373,   451,   452,   464,   403,   382,
     404,   383,   465,   462,   463,     0,   391,   369,   492,     0,
     483,     0,   516,   517,   518,   370,   453,   454,   405,   384,
     406,   385,   469,   470,   471,   395,   374,   396,   375,   397,
     376,   472,   390,   368,     0,     0,   467,   468,   466,   460,
     461,   456,   455,   457,   458,   459,     0,     0,     0,   420,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
     546,   549,     0,     0,   594,   597,   367,   598,   717,   740,
     743,     0,   746,   736,     0,   702,   750,     0,   757,     0,
     764,     0,   770,     0,   773,     0,     0,   258,   720,   253,
     709,   565,   571,     0,   654,   655,   581,   584,   583,     0,
       0,     0,     0,   831,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   338,   377,   378,
     380,   379,   381,   371,   372,   373,   382,   383,   369,   384,
     385,   374,   375,   376,   368,   299,     0,     0,   887,   257,
     488,     0,     0,     0,   489,     0,   520,     0,     0,     0,
       0,     0,     0,   352,   125,   522,   523,   524,   525,   526,
       0,     0,   817,     0,   337,   816,     0,     0,     0,   825,
     826,     0,   833,     0,     0,   823,     0,   862,     0,     0,
       0,     0,   821,   863,     0,   853,   818,   882,     0,     0,
       0,     0,   884,   885,     0,     0,     0,   861,     0,   799,
     798,   477,     0,     0,     0,     0,     0,   125,     0,    59,
      60,     0,    74,    66,   352,     0,   431,     0,     0,     0,
     434,   432,   307,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,   128,   494,     0,   500,     0,   388,   386,
     387,     0,     0,   481,     0,     0,   504,   508,     0,     0,
     484,     0,     0,     0,     0,   421,     0,   428,     0,   479,
     436,   614,   634,   124,   547,   548,   549,   550,   541,   554,
     352,   596,   718,   741,   744,   707,   747,   737,   697,   703,
     751,   753,   758,   760,   765,   767,   771,   567,   774,   573,
       0,     0,   653,     0,     0,   839,   842,     0,   277,     0,
     282,   283,   281,     0,   331,     0,     0,   334,   329,     0,
       0,     0,   875,   873,   256,     0,    83,   320,   323,   326,
     879,   877,   848,   857,   855,   845,     0,   128,     0,     0,
     816,     0,   851,   809,   832,   824,   852,   860,   822,     0,
     816,     0,   828,   829,   836,   820,     0,   261,   264,    32,
       0,   223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   128,    62,     0,    67,     0,   125,   433,     0,     0,
       0,     0,   588,     0,   347,   348,     0,   346,   345,     0,
       0,     0,     0,     0,     0,     0,   409,     0,     0,   505,
       0,   493,     0,   482,     0,     0,   422,     0,     0,   480,
     429,   425,     0,   559,   544,   541,   542,   543,   546,     0,
     754,   761,   768,   252,   259,   656,   582,     0,     0,    78,
     278,   284,     0,     0,     0,     0,   330,   865,   868,   871,
       0,     0,     0,     0,     0,     0,     0,   830,     0,     0,
       0,   226,     0,   528,     0,     0,     0,   834,     0,     0,
     827,     0,     0,   254,     0,     0,     0,     0,     0,     0,
     801,   800,   412,   537,   415,     0,    61,     0,    76,   128,
     407,     0,   308,   588,     0,     0,   354,     0,   351,   353,
       0,   339,   357,     0,   536,     0,   534,   410,   531,     0,
       0,     0,   530,     0,   423,     0,   426,   545,   555,   544,
       0,     0,   830,   830,     0,     0,     0,     0,     0,     0,
       0,   252,   888,   256,   321,   324,   327,     0,   831,   849,
     830,     0,   490,     0,   356,   529,   890,   835,   890,   837,
     890,   262,   252,   265,   890,   890,     0,   803,   802,     0,
       0,   418,    63,   288,     0,    68,    72,    73,    70,    71,
      69,     0,     0,     0,     0,     0,     0,   349,     0,   340,
     495,   501,     0,   535,   533,     0,   532,     0,   557,   551,
     722,   815,   815,   332,     0,     0,   335,   830,   830,     0,
       0,     0,   892,   815,   806,     0,   491,     0,     0,     0,
       0,   892,     0,     0,   890,     0,     0,   416,     0,     0,
       0,   294,   359,   408,     0,     0,     0,   316,   367,   341,
     496,   502,   506,   424,     0,     0,     0,   812,   892,   814,
       0,     0,     0,     0,   815,   815,     0,   876,   889,     0,
       0,   846,     0,     0,     0,     0,   893,     0,   898,   894,
       0,   805,   804,   419,   295,     0,   314,   367,   312,   367,
     315,   507,   556,     0,     0,   893,   813,   840,   843,   333,
     336,     0,     0,   872,   880,   858,   891,   896,   897,   899,
     255,   895,   310,   367,   313,   311,   558,   810,     0,   866,
     869,   309,     0,   811
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1481, -1481, -1481, -1481, -1481, -1481,   596,  1236, -1481, -1481,
   -1481,  1321, -1481, -1481, -1481,   770,  1279, -1481,  1188, -1481,
   -1481,  1242, -1481, -1481, -1481,  -159, -1481, -1481, -1481,  -158,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,  1105,
   -1481, -1481,   -61,   -49, -1481, -1481, -1481,   657,   524,  -530,
    -571,  -797, -1481, -1481, -1481, -1462, -1481, -1481,  -214,  -374,
   -1481,   381, -1481, -1360, -1481, -1289,  -407,  -358, -1481, -1481,
   -1481, -1481,  -444,   -14, -1481, -1481, -1481, -1481, -1481,  -157,
    -156,  -148, -1481,  -147, -1481, -1481, -1481,  1342, -1481,   364,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481,   156,  -116,    60,   -47,   105,  -885,  -451,
   -1481,  -516, -1481, -1481,  -372,  2045, -1481, -1481, -1481, -1480,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,   859,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481,  -154,   -66,  -166,
     -60,    70, -1481, -1481, -1481, -1481, -1481,   936, -1481,  -582,
   -1481, -1481,  -585, -1481, -1481,  -622,  -150,  -574, -1339, -1481,
    -371, -1481, -1481,  1314, -1481, -1481, -1481,   819,   962,   228,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
    -597,  -228, -1481,   950, -1481, -1481, -1481, -1481, -1481, -1481,
    -383, -1481, -1481,  -379, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481,   -86, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481,   951,  -680,  -227,
    -694,  -673, -1481, -1481, -1431,  -898, -1481, -1481, -1481, -1169,
     -96, -1267, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,
   -1481,   181,  -482, -1481, -1481, -1481,   678, -1481, -1481, -1481,
   -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481, -1481,   879,
   -1481, -1236,  -687, -1481
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   147,    52,    17,   169,   175,   706,   511,
     153,   512,   101,    19,    20,    45,    46,    47,    90,    21,
      39,    40,   513,   514,  1231,  1232,   515,  1385,  1487,   516,
     517,   972,   518,   635,   519,   520,   521,   522,  1161,   176,
     177,    35,    36,    37,   225,    67,    68,    69,    70,    22,
     290,   380,   215,    23,   113,   216,   114,   160,   684,   998,
     524,   381,   525,   836,  1553,   895,  1106,   573,   946,  1472,
     948,  1473,   527,   528,   529,   637,   862,  1440,   530,   531,
     532,   533,   534,   535,   536,   537,   432,   538,   740,  1243,
     981,   539,   540,   901,  1453,   902,  1454,   904,  1455,   541,
     867,  1446,   542,   718,  1502,   543,  1251,  1252,   985,   686,
     544,   797,   973,   545,   651,   999,   547,   548,   549,   970,
     550,  1226,  1560,  1227,  1618,   551,  1072,  1420,   552,   719,
    1402,  1630,  1404,  1631,  1509,  1671,   554,   376,  1428,  1518,
    1286,  1288,  1082,   567,   806,  1587,  1635,   377,   378,   618,
     831,   425,   623,   833,   426,   962,   629,   581,   396,   313,
     314,   222,   306,    80,   125,    25,   165,   382,    91,    92,
     179,    93,    26,    49,   117,   162,    27,   293,   564,   565,
    1078,   385,   220,   221,    78,   122,   387,    28,   163,   304,
     630,   555,   302,   359,   360,   823,   424,   361,   589,  1299,
    1311,   816,   422,   362,   582,  1292,   835,   587,  1297,   583,
    1293,   584,  1294,   586,  1296,   590,  1300,   591,  1430,   592,
    1302,   593,  1431,   594,  1304,   595,  1432,   596,  1306,   597,
    1308,   620,    29,    97,   182,   365,   621,    30,    98,   183,
     366,   625,    31,    96,   181,   364,   614,   556,   723,  1605,
     724,   932,  1638,  1639,  1640,   933,   945,  1203,  1197,  1192,
    1364,  1126,   557,   858,  1437,   859,  1438,   913,  1459,   910,
    1457,   934,   708,   558,   911,  1458,   935,   559,  1132,  1528,
    1133,  1529,  1134,  1530,   871,  1450,   908,  1456,   702,   896,
     560,  1608,   954,   561
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    71,   292,   811,   358,   649,   701,   523,   857,   546,
     731,   226,   832,   741,   830,   577,   931,   568,   931,  1354,
     825,   134,   827,   526,   829,   938,   687,   688,  1097,   679,
    1099,   959,  1101,  1174,  1188,   963,   643,   805,   861,   978,
    1200,   601,   619,   624,   611,  1451,  -123,  1121,   126,   127,
      71,    71,    71,  1495,    86,   983,   155,   767,   768,  1181,
    1198,  1204,   926,   603,   852,  1211,    32,    33,  1215,  1373,
      53,  1426,   926,  1521,    58,   523,    54,   742,   744,   745,
    1617,   374,   106,   107,   108,  1500,   223,   223,   732,    87,
     599,   526,    71,    71,    71,    71,   291,   109,   925,   523,
     936,    59,   940,   852,   430,    38,   616,   696,   698,   600,
     952,   100,   617,   764,   423,   526,   767,   768,  1088,   956,
    1102,   412,   110,   291,   720,  1634,   854,   743,     2,   423,
     669,  1427,   733,   644,   645,     3,  1235,   812,  1663,   795,
     796,   170,   171,   224,   224,   640,   363,   431,   413,   414,
    1109,   358,   388,   154,  1574,   918,   744,   745,     4,   843,
       5,  1641,     6,   308,   853,   854,   358,   855,     7,  1125,
     856,  1600,  1650,  1673,   307,   756,   757,   720,     8,   118,
     119,  1236,  1467,   764,     9,  1136,   767,   768,   769,  1235,
    1539,  1187,  1611,   770,   142,   358,   128,   358,   795,   796,
     578,   129,   602,   130,   873,    50,   131,   523,    10,  1159,
     579,   984,   415,  1681,  1682,   853,   416,    34,   357,   803,
      79,   311,  1168,   526,   604,  1171,   646,   853,   853,   853,
      11,    12,    88,   853,   915,   844,   853,   853,   653,   312,
     847,   374,   605,    89,  1601,   647,   223,   132,   606,   390,
     804,    51,   670,   756,   757,  1591,  1592,   309,   358,   358,
     720,   764,   580,   766,   767,   768,   769,   654,   795,   796,
     616,   770,  1235,  1603,   523,   894,   617,    94,   417,   423,
    1237,    48,   418,  1089,  1156,   419,   655,  1351,  1346,   172,
     526,   986,   310,   435,   173,  1238,   174,  1547,   914,   131,
     420,   375,  1424,   224,   523,    72,   421,   291,    73,    74,
      13,    75,  1609,   852,  1610,   852,  1636,   852,  1612,  1613,
     526,   143,  1287,  1376,   412,   961,    58,  1477,   433,    14,
    1644,  1645,   790,   791,   792,   793,   794,   423,   852,    76,
      15,  1092,   411,   509,   683,   627,   795,   796,  1615,  1386,
      81,   413,   414,    59,   358,   358,   358,    82,   358,   358,
    1216,   434,   358,   628,   358,   357,   358,  1085,   358,   982,
     566,   852,    41,    42,    43,   854,    83,   854,  1660,   854,
     357,   961,   389,  1094,  1189,  1190,   622,   100,   526,   526,
     526,   526,   526,   526,   526,   526,  1246,  1095,   616,   671,
     854,   526,   526,    44,   617,  1429,  1247,   526,    82,   357,
      77,   357,  1191,  1117,  1253,   415,   526,   526,   672,   416,
     860,   526,   526,   526,  1194,   526,   358,  1195,  1083,  1319,
     616,   357,   641,   854,  1091,   185,   617,   523,    58,  1248,
     230,  1239,   562,  1234,  1104,   899,  1325,   966,  1103,   358,
    1105,   311,  1466,   526,   563,  1196,  1401,    61,  1249,   979,
    1326,  1207,  1469,  1250,   875,    59,   875,   231,  1240,   312,
     109,  1212,   357,   357,   291,  1182,    82,   964,   217,  1119,
    1120,   417,   875,   523,    84,   418,    62,  1173,   419,   218,
     947,  1280,  1135,  1320,  1314,  1283,   814,   815,   817,   526,
     819,   820,  1388,   420,   824,   931,   826,  1353,   828,   421,
     100,  1157,  1350,   980,   971,  1390,   990,   994,  1399,   931,
    1309,  1307,  1162,  1163,  1390,  1165,  1360,  1399,   875,  1167,
      63,  1169,  1170,   875,  1172,   875,   685,   685,   685,  1208,
    1391,  1209,  1034,  1400,  1258,  1259,  1260,    58,   875,    99,
    1462,  1392,   744,   745,  1496,  1507,   966,   105,   964,    64,
    1514,   967,  1516,   964,   964,   612,    58,  1073,   357,   357,
     357,   964,   357,   357,    59,  1545,   357,   613,   357,   358,
     357,   674,   357,  1228,  1485,  1697,   730,  -752,  1572,   744,
     745,  1703,  -752,    59,  1229,  1230,  1585,   135,   294,   968,
     675,  1377,   295,  1347,    89,   650,  -759,   358,  1029,  -766,
    -752,  -759,   141,   730,  -766,   358,   296,   297,   358,   865,
    1030,   298,   299,   300,   301,   137,   864,   358,    65,  -759,
     358,   358,  -766,  1494,   358,   799,   395,    66,   866,   358,
     357,  1222,   800,   358,   509,   683,  1269,   754,   755,   756,
     757,   758,  1081,  1223,   761,   140,  1381,   764,  1270,   766,
     767,   768,   769,   357,   722,  1274,  -417,   770,   144,   772,
     773,  -417,   720,   358,   358,   393,   358,  1275,   394,  1155,
     358,   395,   358,   358,  1235,   358,   756,   757,   106,  -417,
     108,   423,  1571,   152,   764,  1093,   766,   767,   768,   769,
     616,   744,   745,   148,   770,   149,   617,   616,   150,  1491,
     616,   969,   730,   617,  1310,   151,   617,   423,   358,   616,
     358,   102,   103,   104,  1367,   617,   788,   789,   790,   791,
     792,   793,   794,  1368,   423,  1513,  1201,  1194,  1096,  1202,
     730,    53,   795,   796,   616,  1361,   161,    54,  1362,  1107,
     617,  1363,  1478,  1329,  1330,  1331,   837,  1111,  1442,   164,
    1112,   111,   115,   156,   157,   158,   159,   112,   116,  1116,
     423,   437,   438,    87,  1098,   184,  1122,   120,  1335,   795,
     796,  1131,   123,   121,  1573,   180,   145,   395,   124,   616,
      71,   448,   146,   357,  1284,   617,   106,   453,   756,   757,
    1285,  1465,   685,   872,   874,   219,   764,   875,   766,   767,
     768,   769,  1468,  1471,  1474,  1489,   770,  1475,   875,   875,
     875,   357,  1460,   875,   509,   683,  1267,   412,   909,   357,
    1461,   912,   357,  1556,   467,   468,   106,   107,   108,   875,
     523,   357,   546,   233,   357,   357,   423,  1380,   357,   232,
    1100,   227,   228,   357,   413,   414,   526,   357,   138,   139,
    1107,  1604,  1107,   358,   287,   234,   423,   423,   470,   471,
    1318,  1328,   288,   730,   423,   358,   235,   291,  1581,   792,
     793,   794,   423,    41,    42,    43,  1632,   357,   357,   977,
     357,   795,   796,   289,   357,   305,   357,   357,   303,   357,
     166,   167,   949,   950,   358,   358,   358,   370,   367,    58,
     368,  1637,  1637,   369,   371,  1649,   372,   391,   415,  1604,
     373,  1576,   416,  1637,  1657,   379,   486,   487,   488,   358,
     383,   386,   357,   384,   357,   400,    59,  1616,   392,   397,
    1443,   403,   398,   412,   399,   410,   499,   166,   167,   168,
     401,  1676,   402,   404,  1076,   405,  1674,   227,   228,   229,
     406,   636,   407,   408,  1637,  1637,   942,   943,   944,   409,
     413,   414,    55,    56,    57,   427,   730,   428,   507,   423,
     575,   576,   588,   429,   417,   569,   685,   609,   418,  1550,
    1290,   419,  1554,   632,   633,   642,   658,   660,   358,   661,
    1625,  1626,   662,   663,   664,  1295,   420,   666,   665,   673,
     676,   677,   421,   705,  1382,   689,   690,   691,  1702,  1124,
     692,   710,   693,  1503,   726,   727,   725,   729,   734,   735,
     736,   694,   737,   738,   415,   739,    15,   798,   416,   801,
     669,   809,   822,   821,   807,   622,   838,   703,   841,   842,
    1665,   845,   846,   744,   745,   685,   685,   685,   848,   850,
     730,   851,   730,   742,   730,   893,   730,   863,   730,   868,
     730,   869,   730,   870,   730,   875,   905,   357,   900,   730,
     906,   730,   917,   358,   953,   722,   955,   730,   957,   357,
     958,   964,  1025,   722,   960,  1080,   965,   412,  1084,   730,
     417,   730,  1114,  1439,   418,  1090,   730,   419,   730,  1108,
     730,  1115,  1129,   730,  1158,  1575,  1206,  1193,   357,   357,
     357,  1213,   420,  1221,   413,   414,  1217,  1241,   421,  1224,
    1225,  1242,  1627,  1245,  1254,  1255,  1277,  1257,  1256,  1261,
    1262,  1272,  1289,   357,  1273,  1282,   730,  1278,   754,   755,
     756,   757,   758,  1287,  1301,   761,   762,   763,   764,  1298,
     766,   767,   768,   769,  1303,  1313,  1305,  1321,   770,  1486,
     772,   773,  1322,  1323,   358,  1394,   358,  1334,   730,  1355,
    1333,  1666,  1668,  1395,  1397,  1358,  1341,  1501,   415,  1344,
    1348,   578,   416,  1365,  1624,  1375,  1398,  1387,  1423,  1396,
    1410,   579,   523,  1409,  1412,  1418,  1433,  1435,  1441,  1444,
    1445,  1488,   357,  1470,  1490,  1492,  1497,  1493,   526,  1499,
    1517,  1692,  1525,  1460,  1522,  1434,   787,   788,   789,   790,
     791,   792,   793,   794,  1523,  1531,  1526,  1532,  1533,  1537,
     553,  1538,  1540,   795,   796,  1541,   810,   412,  1501,  1552,
     574,   523,   523,   580,   417,  1561,  1577,  1590,   418,   585,
    1607,   419,  1594,  1595,  1597,  1652,   358,   526,   526,   598,
    1598,  1599,  1654,  1623,   413,   414,   420,  1317,  1633,   608,
    1647,   358,   421,  1648,   744,   745,  1651,  1653,   626,  1655,
    1656,   523,   412,  1658,  1659,  1672,  1687,   357,   634,  1675,
    1677,  1689,   951,  1678,  1683,  1684,  1688,   526,   652,  1685,
    1690,  1691,  1699,   657,  1698,   659,  1700,   133,  1505,   413,
     414,   412,    18,    85,   668,   178,   136,  1562,  1079,  1565,
    1566,  1567,   680,   681,   682,   315,  1214,  1349,   415,  1568,
    1569,   578,   416,    24,  1244,   358,  1544,   700,   413,   414,
    1393,   579,  1498,  1588,  1359,   704,  1425,  1622,   700,  1519,
    1366,   711,   712,   713,   714,   715,    95,  1370,  1520,   631,
    1589,  1372,  1646,  1549,   728,  1357,   939,     0,   709,   754,
     755,   756,   757,   415,   802,     0,     0,   416,   357,   764,
     357,   766,   767,   768,   769,   638,   639,   412,     0,   770,
       0,   772,   773,   580,   417,     0,   730,     0,   418,     0,
       0,   419,   415,     0,   412,     0,   416,  1407,     0,     0,
       0,     0,     0,  1582,   413,   414,   420,     0,     0,  1417,
       0,     0,   421,     0,  1422,     0,     0,     0,     0,     0,
     813,   413,   414,     0,     0,     0,     0,     0,     0,   417,
       0,     0,     0,   418,     0,  1315,   419,     0,     0,  1696,
     790,   791,   792,   793,   794,     0,     0,     0,   840,     0,
       0,   420,     0,     0,   795,   796,     0,   421,   417,     0,
     357,     0,   418,     0,  1316,   419,     0,     0,   415,     0,
       0,     0,   416,     0,     0,   357,     0,     0,     0,     0,
     420,     0,     0,     0,  1464,   415,   421,   652,     0,   416,
       0,     0,     0,     0,   700,   897,     0,   898,     0,     0,
       0,     0,   903,     0,     0,     0,     0,     0,   907,  1476,
       0,     0,     0,     0,     0,     0,     0,   916,     0,     0,
       0,     0,     0,     0,     0,     0,   969,     0,   919,   920,
     921,   922,   923,   924,   417,   930,     0,   930,   418,   357,
    1332,   419,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   417,     0,     0,     0,   418,   420,  1337,   419,     0,
       0,     0,   421,     0,     0,     0,     0,   744,   745,   837,
       0,     0,     0,   420,     0,     0,     0,     0,     0,   421,
       0,     0,     0,     0,   969,  1000,  1002,  1004,  1006,  1008,
    1010,  1012,  1014,  1015,  1016,     0,     0,  1017,  1019,  1021,
    1022,  1023,  1024,     0,  1027,  1028,     0,  1031,  1032,  1033,
    1035,  1036,  1037,  1039,  1041,  1042,  1043,  1044,  1046,  1048,
    1050,  1051,  1053,  1055,  1056,  1057,  1058,  1059,  1060,  1061,
    1062,  1063,  1064,  1065,     0,  1074,     0,     0,     0,  1077,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1086,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   754,   755,   756,   757,   758,   837,   730,   761,
     762,   763,   764,  1602,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,   837,  1113,
     776,   777,   778,     0,     0,     0,  1118,     0,     0,   650,
       0,     0,     0,  1128,     0,  1130,     0,   730,     0,   730,
       0,     0,     0,     0,  1137,  1138,  1139,  1140,  1141,  1142,
    1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,
    1153,  1154,     0,   730,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,   874,     0,   795,   796,     0,
       0,     0,     0,     0,     0,     0,   711,  1184,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1205,     0,   700,     0,     0,     0,     0,     0,     0,  1210,
       0,     0,   700,     0,     0,     0,     0,  1137,   436,  1218,
    1219,  1220,   437,   438,     3,     0,   439,   440,   441,     0,
     442,  1233,   443,   444,   445,   446,   447,     0,     0,     0,
       0,     0,   448,   449,   450,   451,   452,     0,   453,     0,
       0,     0,     0,     0,     0,   454,   455,     0,     0,   456,
       0,   457,   458,     0,     0,   459,     0,     8,   460,   461,
       0,   462,   463,     0,     0,   464,   465,     0,     0,     0,
       0,     0,   466,     0,     0,   467,   468,     0,   322,   323,
     324,     0,   326,   327,   328,   329,   330,   469,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,     0,
     344,   345,   346,     0,     0,   349,   350,   351,   352,   470,
     471,   472,   473,     0,     0,     0,  1271,     0,     0,     0,
    1276,     0,     0,     0,     0,   474,   475,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     476,   477,   478,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   479,   480,
     481,   482,   483,     0,   484,     0,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,    59,   496,   497,
     498,     0,     0,     0,     0,     0,     0,   499,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   412,     0,     0,   502,   503,   504,     0,    14,     0,
    1336,   505,   506,     0,     0,     0,     0,     0,     0,   507,
       0,   508,     0,   509,   510,     0,     0,     0,   413,   414,
       0,     0,     0,     0,     0,  1352,     0,     0,     0,     0,
       0,     0,  1356,   930,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   553,     0,   695,     0,     0,     0,     0,
       0,   316,     0,  1378,  1379,     0,     0,   317,     0,     0,
    1383,     0,     0,   318,     0,     0,     0,     0,  1137,     0,
       0,     0,   415,   319,     0,     0,   416,     0,     0,     0,
       0,   320,   744,   745,  1403,     0,  1405,     0,     0,     0,
       0,  1408,     0,     0,     0,     0,   321,  1411,     0,     0,
       0,  1414,     0,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,     0,     0,   417,     0,
       0,     0,   418,  1436,  1338,   419,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     420,     0,     0,   700,     0,     0,   421,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,   754,   755,   756,
     757,   758,     0,     0,   761,   762,   763,   764,   355,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,    59,     0,  1479,  1480,  1481,     0,     0,     0,
       0,     0,     0,     0,  1384,     0,     0,     0,     0,     0,
       0,     0,     0,   744,   745,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1510,
       0,  1511,     0,     0,     0,     0,     0,  1515,   356,     0,
       0,     0,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,   412,     0,
       0,  1524,   795,   796,  1527,     0,     0,     0,     0,     0,
       0,     0,  1534,  1535,  1536,     0,     0,     0,     0,  1543,
       0,     0,     0,     0,  1546,   413,   414,  1548,     0,     0,
     700,  1551,     0,   700,  1555,     0,  1557,  1558,     0,     0,
       0,     0,     0,     0,     0,     0,  1570,     0,   754,   755,
     756,   757,   758,     0,     0,   761,   762,   763,   764,  1579,
     766,   767,   768,   769,     0,     0,     0,     0,   770,     0,
     772,   773,     0,     0,     0,     0,   776,   777,   778,     0,
       0,     0,   782,     0,     0,     0,     0,     0,     0,   415,
       0,     0,   700,   416,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1614,     0,     0,     0,     0,
       0,     0,   784,  1621,   785,   786,   787,   788,   789,   790,
     791,   792,   793,   794,     0,  1628,     0,  1629,     0,     0,
       0,     0,     0,   795,   796,     0,     0,     0,     0,     0,
       0,     0,     0,  1642,  1643,   417,     0,     0,     0,   418,
       0,  1339,   419,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1661,  1662,     0,   420,     0,  1664,
       0,     0,   436,   421,  1667,  1669,   437,   438,     3,     0,
     439,   440,   441,     0,   442,     0,   443,   444,   445,   446,
     447,     0,     0,     0,     0,     0,   448,   449,   450,   451,
     452,  1686,   453,     0,     0,     0,     0,     0,     0,   454,
     455,     0,     0,   456,  1693,   457,   458,     0,     0,   459,
       0,     8,   460,   461,     0,   462,   463,     0,     0,   464,
     465,     0,     0,     0,     0,     0,   466,     0,     0,   467,
     468,     0,   322,   323,   324,     0,   326,   327,   328,   329,
     330,   469,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,     0,   344,   345,   346,     0,     0,   349,
     350,   351,   352,   470,   471,   472,   473,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   474,
     475,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   476,   477,   478,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   479,   480,   481,   482,   483,     0,   484,     0,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,    59,   496,   497,   498,     0,     0,     0,     0,     0,
       0,   499,  1371,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
     504,     0,    14,     0,     0,   505,   506,     0,     0,   437,
     438,     0,     0,   507,     0,   508,     0,   509,   510,   443,
     444,   445,   446,   447,     0,     0,     0,     0,     0,   448,
       0,   450,     0,     0,     0,   453,     0,   412,     0,     0,
       0,     0,     0,   455,     0,     0,     0,     0,     0,   458,
       0,     0,   459,     0,     0,   460,     0,   926,     0,   463,
       0,     0,     0,     0,   413,   414,     0,     0,     0,   570,
       0,     0,   467,   468,     0,   322,   323,   324,     0,   326,
     327,   328,   329,   330,   469,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,   344,   345,   346,
       0,     0,   349,   350,   351,   352,   470,   471,   571,     0,
       0,     0,  1001,  1003,  1005,  1007,  1009,  1011,  1013,     0,
       0,     0,   474,   475,  1018,  1020,     0,     0,   415,     0,
    1026,     0,   416,     0,     0,     0,     0,     0,     0,  1038,
    1040,     0,     0,     0,  1045,  1047,  1049,    58,  1052,     0,
       0,     0,     0,     0,     0,   479,   480,   481,   482,   483,
     412,   484,   720,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   721,   572,   497,   498,     0,     0,
       0,     0,     0,     0,   499,     0,     0,   413,   414,     0,
       0,     0,     0,     0,   417,     0,     0,     0,   418,     0,
       0,   927,   503,   504,     0,    14,     0,     0,   505,   506,
       0,     0,     0,   437,   438,     0,   928,     0,   929,     0,
     509,   510,   421,   443,   444,   445,   446,   447,     0,     0,
       0,     0,     0,   448,     0,   450,     0,     0,     0,   453,
       0,   412,     0,     0,     0,     0,     0,   455,     0,     0,
       0,   415,     0,   458,     0,   416,   459,     0,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,   413,   414,
       0,     0,     0,   570,     0,     0,   467,   468,     0,   322,
     323,   324,     0,   326,   327,   328,   329,   330,   469,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,   344,   345,   346,     0,     0,   349,   350,   351,   352,
     470,   471,   571,     0,     0,     0,     0,   417,     0,     0,
       0,   418,     0,  1340,   419,     0,   474,   475,     0,     0,
       0,     0,   415,     0,     0,     0,   416,     0,     0,   420,
       0,     0,     0,     0,     0,   421,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   479,
     480,   481,   482,   483,   412,   484,   720,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   721,   572,
     497,   498,     0,   412,     0,     0,     0,     0,   499,     0,
       0,   413,   414,     0,     0,     0,     0,     0,   417,     0,
       0,     0,   418,     0,     0,   927,   503,   504,     0,    14,
     413,   414,   505,   506,     0,     0,     0,   437,   438,     0,
     928,     0,   937,     0,   509,   510,   421,   443,   444,   445,
     446,   447,     0,     0,     0,     0,     0,   448,     0,   450,
       0,     0,     0,   453,     0,   603,     0,     0,     0,     0,
       0,   455,     0,     0,     0,   415,     0,   458,     0,   416,
     459,     0,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,   415,     0,     0,   570,   416,     0,
     467,   468,     0,   322,   323,   324,     0,   326,   327,   328,
     329,   330,   469,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,   344,   345,   346,     0,     0,
     349,   350,   351,   352,   470,   471,   571,     0,     0,     0,
       0,   417,     0,     0,     0,   418,     0,  1342,   419,     0,
     474,   475,     0,     0,     0,     0,     0,     0,     0,     0,
     417,     0,     0,   420,   418,     0,  1343,   419,     0,   421,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,   420,   479,   480,   481,   482,   483,   421,   484,
       0,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,    59,   572,   497,   498,     0,     0,     0,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   604,     0,     0,   502,
     503,   504,     0,    14,     0,     0,   505,   506,     0,     0,
       0,   437,   438,     0,  1183,     0,   508,     0,   509,   510,
     606,   443,   444,   445,   446,   447,     0,     0,     0,     0,
       0,   448,     0,   450,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,     0,   455,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,     0,     0,   467,   468,     0,   322,   323,   324,
       0,   326,   327,   328,   329,   330,   469,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,   344,
     345,   346,     0,     0,   349,   350,   351,   352,   470,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   474,   475,     0,     0,     0,     0,
       0,     0,     0,   648,     0,     0,     0,     0,     0,   476,
     477,   478,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   479,   480,   481,
     482,   483,     0,   484,     0,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,    59,   572,   497,   498,
       0,     0,     0,     0,     0,     0,   499,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,   504,     0,    14,     0,     0,
     505,   506,     0,     0,   437,   438,     0,     0,   507,     0,
     508,     0,   509,   510,   443,   444,   445,   446,   447,     0,
       0,     0,     0,     0,   448,     0,   450,     0,     0,     0,
     453,     0,     0,     0,     0,     0,     0,     0,   455,     0,
       0,     0,     0,     0,   458,     0,     0,   459,     0,     0,
     460,     0,     0,     0,   463,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   570,     0,     0,   467,   468,     0,
     322,   323,   324,     0,   326,   327,   328,   329,   330,   469,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,     0,   344,   345,   346,     0,     0,   349,   350,   351,
     352,   470,   471,   472,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   474,   475,     0,
       0,     0,     0,     0,     0,     0,   678,     0,     0,     0,
       0,     0,   476,   477,   478,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     479,   480,   481,   482,   483,     0,   484,     0,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,    59,
     572,   497,   498,     0,     0,     0,     0,     0,     0,   499,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,     0,
      14,     0,     0,   505,   506,     0,     0,   437,   438,     0,
       0,   507,     0,   508,     0,   509,   510,   443,   444,   445,
     446,   447,     0,     0,     0,     0,     0,   448,  1563,   450,
     451,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,   455,     0,     0,     0,     0,     0,   458,     0,     0,
     459,     0,     0,   460,   461,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,     0,     0,
     467,   468,     0,   322,   323,   324,     0,   326,   327,   328,
     329,   330,   469,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,   344,   345,   346,     0,     0,
     349,   350,   351,   352,   470,   471,   571,  1564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   479,   480,   481,   482,   483,     0,   484,
       0,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,    59,   572,   497,   498,     0,     0,     0,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
     503,   504,     0,    14,     0,     0,   505,   506,     0,     0,
     437,   438,     0,     0,   507,     0,   508,     0,   509,   510,
     443,   444,   445,   446,   447,     0,     0,     0,     0,     0,
     448,     0,   450,     0,     0,     0,   453,     0,     0,     0,
       0,     0,     0,     0,   455,     0,     0,     0,     0,     0,
     458,     0,     0,   459,     0,     0,   460,     0,     0,     0,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,     0,     0,   467,   468,     0,   322,   323,   324,     0,
     326,   327,   328,   329,   330,   469,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,   344,   345,
     346,     0,     0,   349,   350,   351,   352,   470,   471,   472,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   476,   477,
     478,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   479,   480,   481,   482,
     483,     0,   484,     0,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,    59,   572,   497,   498,     0,
       0,     0,     0,     0,     0,   499,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   502,   503,   504,     0,    14,     0,     0,   505,
     506,     0,     0,   437,   438,     0,     0,   507,     0,   508,
       0,   509,   510,   443,   444,   445,   446,   447,     0,     0,
       0,     0,     0,   448,     0,   450,     0,     0,     0,   453,
       0,     0,     0,     0,     0,     0,     0,   455,     0,     0,
       0,     0,     0,   458,     0,     0,   459,     0,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,     0,     0,   467,   468,   974,   322,
     323,   324,     0,   326,   327,   328,   329,   330,   469,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,   344,   345,   346,     0,     0,   349,   350,   351,   352,
     470,   471,   571,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   479,
     480,   481,   482,   483,     0,   484,   720,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   721,   572,
     497,   498,     0,     0,     0,     0,     0,     0,   499,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   503,   504,     0,    14,
       0,     0,   505,   506,     0,     0,   437,   438,     0,     0,
     975,     0,   508,   976,   509,   510,   443,   444,   445,   446,
     447,     0,     0,     0,     0,     0,   448,     0,   450,     0,
       0,   412,   453,     0,     0,     0,     0,     0,     0,     0,
     455,     0,     0,     0,     0,     0,   458,     0,     0,   459,
       0,     0,   460,     0,     0,     0,   463,     0,   413,   414,
       0,     0,     0,     0,     0,     0,   570,     0,     0,   467,
     468,     0,   322,   323,   324,     0,   326,   327,   328,   329,
     330,   469,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,     0,   344,   345,   346,     0,     0,   349,
     350,   351,   352,   470,   471,   472,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   474,
     475,     0,   415,     0,     0,     0,   416,     0,     0,     0,
       0,     0,     0,     0,   995,   996,   997,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   479,   480,   481,   482,   483,   412,   484,     0,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,    59,   572,   497,   498,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   413,   414,     0,     0,   417,     0,
       0,     0,   418,     0,  1345,   419,     0,     0,   502,   503,
     504,     0,    14,     0,     0,   505,   506,     0,     0,     0,
     420,   437,   438,   507,     0,   508,   421,   509,   510,   716,
       0,   443,   444,   445,   446,   447,     0,     0,     0,     0,
       0,   448,     0,   450,     0,     0,   412,   453,     0,     0,
       0,     0,     0,     0,     0,   455,     0,     0,   415,     0,
       0,   458,   416,     0,   459,   717,     0,   460,     0,     0,
       0,   463,     0,   413,   414,     0,     0,     0,     0,     0,
       0,   570,     0,     0,   467,   468,     0,   322,   323,   324,
       0,   326,   327,   328,   329,   330,   469,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,   344,
     345,   346,     0,     0,   349,   350,   351,   352,   470,   471,
     571,     0,     0,     0,   417,     0,     0,     0,   418,     0,
    1447,   419,     0,     0,   474,   475,     0,   415,     0,     0,
       0,   416,     0,     0,     0,     0,   420,     0,     0,     0,
       0,     0,   421,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   479,   480,   481,
     482,   483,   412,   484,     0,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,    59,   572,   497,   498,
       0,     0,     0,     0,     0,     0,   499,     0,     0,   413,
     414,     0,     0,   417,     0,     0,     0,   418,     0,  1448,
     419,     0,     0,   502,   503,   504,     0,    14,     0,     0,
     505,   506,     0,     0,     0,   420,   437,   438,   507,   607,
     508,   421,   509,   510,   716,     0,   443,   444,   445,   446,
     447,     0,     0,     0,     0,     0,   448,     0,   450,     0,
       0,   412,   453,     0,     0,     0,     0,     0,     0,     0,
     455,     0,     0,   415,     0,     0,   458,   416,     0,   459,
     717,     0,   460,     0,     0,     0,   463,     0,   413,   414,
       0,     0,     0,     0,     0,     0,   570,     0,     0,   467,
     468,     0,   322,   323,   324,     0,   326,   327,   328,   329,
     330,   469,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,     0,   344,   345,   346,     0,     0,   349,
     350,   351,   352,   470,   471,   571,     0,     0,     0,   417,
       0,     0,     0,   418,     0,  1449,   419,     0,     0,   474,
     475,     0,   415,     0,     0,     0,   416,     0,     0,     0,
       0,   420,     0,     0,     0,     0,     0,   421,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   479,   480,   481,   482,   483,   412,   484,   720,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   721,   572,   497,   498,     0,     0,     0,     0,     0,
       0,   499,     0,     0,   413,   414,     0,     0,   417,     0,
       0,     0,   418,     0,  1452,   419,     0,     0,   502,   503,
     504,     0,    14,     0,     0,   505,   506,     0,     0,     0,
     420,   437,   438,   507,     0,   508,   421,   509,   510,   716,
       0,   443,   444,   445,   446,   447,     0,     0,     0,     0,
       0,   448,     0,   450,     0,     0,   412,   453,     0,     0,
       0,     0,     0,     0,     0,   455,     0,     0,   415,     0,
       0,   458,   416,     0,   459,   717,     0,   460,     0,     0,
       0,   463,     0,   413,   414,     0,     0,     0,     0,     0,
       0,   570,     0,     0,   467,   468,     0,   322,   323,   324,
       0,   326,   327,   328,   329,   330,   469,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,   344,
     345,   346,     0,     0,   349,   350,   351,   352,   470,   471,
     571,     0,     0,     0,   417,     0,     0,     0,   418,     0,
    1484,   419,     0,     0,   474,   475,     0,   415,     0,     0,
       0,   416,     0,     0,     0,     0,   420,     0,     0,     0,
       0,     0,   421,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   479,   480,   481,
     482,   483,     0,   484,     0,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,    59,   572,   497,   498,
       0,     0,     0,     0,     0,     0,   499,     0,     0,     0,
       0,     0,     0,   417,     0,     0,     0,   418,     0,  1580,
     419,     0,     0,   502,   503,   504,     0,    14,     0,     0,
     505,   506,     0,     0,     0,   420,   437,   438,   507,   838,
     508,   421,   509,   510,   716,     0,   443,   444,   445,   446,
     447,     0,     0,     0,     0,     0,   448,     0,   450,     0,
       0,     0,   453,     0,     0,     0,     0,     0,     0,     0,
     455,     0,     0,     0,     0,     0,   458,     0,     0,   459,
     717,     0,   460,     0,     0,     0,   463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,     0,     0,   467,
     468,     0,   322,   323,   324,     0,   326,   327,   328,   329,
     330,   469,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,     0,   344,   345,   346,     0,     0,   349,
     350,   351,   352,   470,   471,   571,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   474,
     475,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   479,   480,   481,   482,   483,     0,   484,     0,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,    59,   572,   497,   498,     0,     0,     0,     0,     0,
       0,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
     504,     0,    14,     0,     0,   505,   506,     0,     0,   437,
     438,     0,     0,   507,     0,   508,     0,   509,   510,   443,
     444,   445,   446,   447,     0,     0,     0,     0,     0,   448,
       0,   450,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,   455,     0,     0,     0,     0,     0,   458,
       0,     0,   459,     0,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
       0,     0,   467,   468,  1123,   322,   323,   324,     0,   326,
     327,   328,   329,   330,   469,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,   344,   345,   346,
       0,     0,   349,   350,   351,   352,   470,   471,   571,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   479,   480,   481,   482,   483,
       0,   484,   720,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   721,   572,   497,   498,     0,     0,
       0,     0,     0,     0,   499,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   502,   503,   504,     0,    14,     0,     0,   505,   506,
       0,     0,   437,   438,     0,     0,   507,     0,   508,     0,
     509,   510,   443,   444,   445,   446,   447,     0,     0,     0,
       0,     0,   448,     0,   450,     0,     0,     0,   453,     0,
       0,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,     0,   458,     0,     0,   459,     0,     0,   460,     0,
       0,     0,   463,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,     0,     0,   467,   468,     0,   322,   323,
     324,     0,   326,   327,   328,   329,   330,   469,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,     0,
     344,   345,   346,     0,     0,   349,   350,   351,   352,   470,
     471,   571,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   479,   480,
     481,   482,   483,     0,   484,   720,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,   721,   572,   497,
     498,     0,     0,     0,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   503,   504,     0,    14,     0,
       0,   505,   506,     0,     0,   437,   438,     0,     0,   507,
       0,   508,  1185,   509,   510,   443,   444,   445,   446,   447,
       0,     0,     0,     0,     0,   448,     0,   450,     0,     0,
       0,   453,     0,     0,     0,     0,     0,     0,     0,   455,
       0,     0,     0,     0,     0,   458,     0,     0,   459,     0,
       0,   460,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,     0,     0,   467,   468,
       0,   322,   323,   324,     0,   326,   327,   328,   329,   330,
     469,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,     0,   344,   345,   346,     0,     0,   349,   350,
     351,   352,   470,   471,   571,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   479,   480,   481,   482,   483,     0,   484,   720,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     721,   572,   497,   498,     0,     0,     0,     0,     0,     0,
     499,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,   504,
       0,    14,     0,     0,   505,   506,     0,     0,   437,   438,
       0,     0,   507,     0,   508,  1199,   509,   510,   443,   444,
     445,   446,   447,     0,     0,     0,     0,     0,   448,     0,
     450,     0,     0,     0,   453,     0,     0,     0,     0,     0,
       0,     0,   455,     0,     0,     0,     0,     0,   458,     0,
       0,   459,     0,     0,   460,     0,     0,     0,   463,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   570,     0,
       0,   467,   468,     0,   322,   323,   324,     0,   326,   327,
     328,   329,   330,   469,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,     0,   344,   345,   346,     0,
       0,   349,   350,   351,   352,   470,   471,   571,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   474,   475,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   479,   480,   481,   482,   483,     0,
     484,     0,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    59,   572,   497,   498,     0,     0,     0,
       0,     0,     0,   499,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   503,   504,     0,    14,     0,     0,   505,   506,     0,
       0,   437,   438,     0,     0,   507,   607,   508,     0,   509,
     510,   443,   444,   445,   446,   447,     0,     0,     0,     0,
       0,   448,     0,   450,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,     0,   455,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,   656,     0,     0,
       0,   570,     0,     0,   467,   468,     0,   322,   323,   324,
       0,   326,   327,   328,   329,   330,   469,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,   344,
     345,   346,     0,     0,   349,   350,   351,   352,   470,   471,
     571,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   474,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   479,   480,   481,
     482,   483,     0,   484,     0,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,    59,   572,   497,   498,
       0,     0,     0,     0,     0,     0,   499,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,   504,     0,    14,     0,     0,
     505,   506,     0,     0,   437,   438,     0,     0,   507,     0,
     508,     0,   509,   510,   443,   444,   445,   446,   447,     0,
       0,     0,     0,     0,   448,     0,   450,     0,     0,     0,
     453,     0,     0,     0,     0,     0,     0,     0,   455,     0,
       0,     0,     0,     0,   458,     0,     0,   459,     0,     0,
     460,     0,     0,     0,   463,     0,     0,   667,     0,     0,
       0,     0,     0,     0,   570,     0,     0,   467,   468,     0,
     322,   323,   324,     0,   326,   327,   328,   329,   330,   469,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,     0,   344,   345,   346,     0,     0,   349,   350,   351,
     352,   470,   471,   571,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   474,   475,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     479,   480,   481,   482,   483,     0,   484,     0,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,    59,
     572,   497,   498,     0,     0,     0,     0,     0,     0,   499,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,     0,
      14,     0,     0,   505,   506,     0,     0,     0,     0,   437,
     438,   507,     0,   508,     0,   509,   510,   699,     0,   443,
     444,   445,   446,   447,     0,     0,     0,     0,     0,   448,
       0,   450,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,   455,     0,     0,     0,     0,     0,   458,
       0,     0,   459,     0,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
       0,     0,   467,   468,     0,   322,   323,   324,     0,   326,
     327,   328,   329,   330,   469,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,   344,   345,   346,
       0,     0,   349,   350,   351,   352,   470,   471,   571,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   479,   480,   481,   482,   483,
       0,   484,     0,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,    59,   572,   497,   498,     0,     0,
       0,     0,     0,     0,   499,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   502,   503,   504,     0,    14,     0,     0,   505,   506,
       0,     0,   437,   438,     0,     0,   507,     0,   508,     0,
     509,   510,   443,   444,   445,   446,   447,     0,     0,     0,
       0,     0,   448,     0,   450,     0,     0,     0,   453,     0,
       0,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,     0,   458,     0,     0,   459,     0,     0,   460,     0,
       0,     0,   463,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,     0,     0,   467,   468,     0,   322,   323,
     324,     0,   326,   327,   328,   329,   330,   469,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,     0,
     344,   345,   346,     0,     0,   349,   350,   351,   352,   470,
     471,   571,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   479,   480,
     481,   482,   483,     0,   484,     0,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,    59,   572,   497,
     498,     0,     0,     0,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   703,     0,   502,   503,   504,     0,    14,     0,
       0,   505,   506,     0,     0,     0,     0,   437,   438,   507,
       0,   508,     0,   509,   510,   707,     0,   443,   444,   445,
     446,   447,     0,     0,     0,     0,     0,   448,     0,   450,
       0,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,   455,     0,     0,     0,     0,     0,   458,     0,     0,
     459,     0,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,     0,     0,
     467,   468,     0,   322,   323,   324,     0,   326,   327,   328,
     329,   330,   469,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,   344,   345,   346,     0,     0,
     349,   350,   351,   352,   470,   471,   571,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   479,   480,   481,   482,   483,     0,   484,
       0,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,    59,   572,   497,   498,     0,     0,     0,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
     503,   504,     0,    14,     0,     0,   505,   506,     0,     0,
     437,   438,     0,     0,   507,     0,   508,     0,   509,   510,
     443,   444,   445,   446,   447,     0,     0,     0,     0,     0,
     448,     0,   450,     0,     0,     0,   453,     0,     0,     0,
       0,     0,     0,     0,   455,     0,     0,     0,     0,     0,
     458,     0,     0,   459,     0,     0,   460,     0,     0,     0,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,     0,     0,   467,   468,     0,   322,   323,   324,     0,
     326,   327,   328,   329,   330,   469,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,   344,   345,
     346,     0,     0,   349,   350,   351,   352,   470,   471,   571,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   479,   480,   481,   482,
     483,     0,   484,   720,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   721,   572,   497,   498,     0,
       0,     0,     0,     0,     0,   499,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   502,   503,   504,     0,    14,     0,     0,   505,
     506,     0,     0,   437,   438,     0,     0,   507,     0,   508,
       0,   509,   510,   443,   444,   445,   446,   447,     0,     0,
    1054,     0,     0,   448,     0,   450,     0,     0,     0,   453,
       0,     0,     0,     0,     0,     0,     0,   455,     0,     0,
       0,     0,     0,   458,     0,     0,   459,     0,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,     0,     0,   467,   468,     0,   322,
     323,   324,     0,   326,   327,   328,   329,   330,   469,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,   344,   345,   346,     0,     0,   349,   350,   351,   352,
     470,   471,   571,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   479,
     480,   481,   482,   483,     0,   484,     0,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,    59,   572,
     497,   498,     0,     0,     0,     0,     0,     0,   499,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   503,   504,     0,    14,
       0,     0,   505,   506,     0,     0,   437,   438,     0,     0,
     507,     0,   508,     0,   509,   510,   443,   444,   445,   446,
     447,     0,     0,     0,     0,     0,   448,     0,   450,     0,
       0,     0,   453,     0,     0,     0,     0,     0,     0,     0,
     455,     0,     0,     0,     0,     0,   458,     0,     0,   459,
       0,     0,   460,     0,     0,     0,   463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,     0,     0,   467,
     468,     0,   322,   323,   324,     0,   326,   327,   328,   329,
     330,   469,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,     0,   344,   345,   346,     0,     0,   349,
     350,   351,   352,   470,   471,   571,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   474,
     475,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   479,   480,   481,   482,   483,     0,   484,     0,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,    59,   572,   497,   498,     0,     0,     0,     0,     0,
       0,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
     504,     0,    14,     0,     0,   505,   506,     0,     0,   437,
     438,     0,     0,   507,     0,   508,  1075,   509,   510,   443,
     444,   445,   446,   447,     0,     0,     0,     0,     0,   448,
       0,   450,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,   455,     0,     0,     0,     0,     0,   458,
       0,     0,   459,     0,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
       0,     0,   467,   468,     0,   322,   323,   324,     0,   326,
     327,   328,   329,   330,   469,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,   344,   345,   346,
       0,     0,   349,   350,   351,   352,   470,   471,   571,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   479,   480,   481,   482,   483,
       0,   484,     0,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,    59,   572,   497,   498,     0,     0,
       0,     0,     0,     0,   499,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1127,
       0,   502,   503,   504,     0,    14,     0,     0,   505,   506,
       0,     0,   437,   438,     0,     0,   507,     0,   508,     0,
     509,   510,   443,   444,   445,   446,   447,     0,     0,     0,
       0,     0,   448,     0,   450,     0,     0,     0,   453,     0,
       0,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,     0,   458,     0,     0,   459,     0,     0,   460,     0,
       0,     0,   463,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,     0,     0,   467,   468,     0,   322,   323,
     324,     0,   326,   327,   328,   329,   330,   469,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,     0,
     344,   345,   346,     0,     0,   349,   350,   351,   352,   470,
     471,   571,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   474,   475,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   479,   480,
     481,   482,   483,     0,   484,     0,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,    59,   572,   497,
     498,     0,     0,     0,     0,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   503,   504,     0,    14,     0,
       0,   505,   506,     0,     0,   437,   438,     0,     0,   507,
       0,   508,  1406,   509,   510,   443,   444,   445,   446,   447,
       0,     0,     0,     0,     0,   448,     0,   450,     0,     0,
       0,   453,     0,     0,     0,     0,     0,     0,     0,   455,
       0,     0,     0,     0,     0,   458,     0,     0,   459,     0,
       0,   460,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,     0,     0,   467,   468,
       0,   322,   323,   324,     0,   326,   327,   328,   329,   330,
     469,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,     0,   344,   345,   346,     0,     0,   349,   350,
     351,   352,   470,   471,   571,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   479,   480,   481,   482,   483,     0,   484,     0,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
      59,   572,   497,   498,     0,     0,     0,     0,     0,     0,
     499,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,   504,
       0,    14,     0,     0,   505,   506,     0,     0,   437,   438,
       0,     0,  1415,     0,   508,  1416,   509,   510,   443,   444,
     445,   446,   447,     0,     0,     0,     0,     0,   448,     0,
     450,     0,     0,     0,   453,     0,     0,     0,     0,     0,
       0,     0,   455,     0,     0,     0,     0,     0,   458,     0,
       0,   459,     0,     0,   460,     0,     0,     0,   463,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   570,     0,
       0,   467,   468,     0,   322,   323,   324,     0,   326,   327,
     328,   329,   330,   469,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,     0,   344,   345,   346,     0,
       0,   349,   350,   351,   352,   470,   471,   571,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   474,   475,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   479,   480,   481,   482,   483,     0,
     484,     0,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    59,   572,   497,   498,     0,     0,     0,
       0,     0,     0,   499,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   503,   504,     0,    14,     0,     0,   505,   506,     0,
       0,   437,   438,     0,     0,   507,     0,   508,  1421,   509,
     510,   443,   444,   445,   446,   447,     0,     0,     0,     0,
       0,   448,     0,   450,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,     0,   455,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,     0,     0,   467,   468,     0,   322,   323,   324,
       0,   326,   327,   328,   329,   330,   469,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,     0,   344,
     345,   346,     0,     0,   349,   350,   351,   352,   470,   471,
     571,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   474,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   479,   480,   481,
     482,   483,     0,   484,     0,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,    59,   572,   497,   498,
       0,     0,     0,     0,     0,     0,   499,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,   504,     0,    14,     0,     0,
     505,   506,     0,     0,   437,   438,     0,     0,   507,     0,
     508,  1463,   509,   510,   443,   444,   445,   446,   447,     0,
       0,     0,     0,     0,   448,     0,   450,     0,     0,     0,
     453,     0,     0,     0,     0,     0,     0,     0,   455,     0,
       0,     0,     0,     0,   458,     0,     0,   459,     0,     0,
     460,     0,     0,     0,   463,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   570,     0,     0,   467,   468,     0,
     322,   323,   324,     0,   326,   327,   328,   329,   330,   469,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,     0,   344,   345,   346,     0,     0,   349,   350,   351,
     352,   470,   471,   571,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   474,   475,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     479,   480,   481,   482,   483,     0,   484,     0,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,    59,
     572,   497,   498,     0,     0,     0,     0,     0,     0,   499,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,     0,
      14,     0,     0,   505,   506,     0,     0,   437,   438,     0,
       0,   507,     0,   508,  1542,   509,   510,   443,   444,   445,
     446,   447,     0,     0,     0,     0,     0,   448,     0,   450,
       0,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,   455,     0,     0,     0,     0,     0,   458,     0,     0,
     459,     0,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,     0,     0,
     467,   468,     0,   322,   323,   324,     0,   326,   327,   328,
     329,   330,   469,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,     0,   344,   345,   346,     0,     0,
     349,   350,   351,   352,   470,   471,   571,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,   475,     0,     0,     0,     0,     0,     0,     0,  1578,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   479,   480,   481,   482,   483,     0,   484,
       0,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,    59,   572,   497,   498,     0,     0,     0,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
     503,   504,     0,    14,     0,     0,   505,   506,     0,     0,
     437,   438,     0,     0,   507,     0,   508,     0,   509,   510,
     443,   444,   445,   446,   447,     0,     0,     0,     0,     0,
     448,     0,   450,     0,     0,     0,   453,     0,     0,     0,
       0,     0,     0,     0,   455,     0,     0,     0,     0,     0,
     458,     0,     0,   459,     0,     0,   460,     0,     0,     0,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,     0,     0,   467,   468,     0,   322,   323,   324,     0,
     326,   327,   328,   329,   330,   469,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,   344,   345,
     346,     0,     0,   349,   350,   351,   352,   470,   471,   571,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   474,   475,     0,     0,     0,     0,     0,
       0,     0,  1619,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   479,   480,   481,   482,
     483,     0,   484,     0,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,    59,   572,   497,   498,     0,
       0,     0,     0,     0,     0,   499,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   502,   503,   504,     0,    14,     0,     0,   505,
     506,     0,     0,   437,   438,     0,     0,   507,     0,   508,
       0,   509,   510,   443,   444,   445,   446,   447,     0,     0,
       0,     0,     0,   448,     0,   450,     0,     0,     0,   453,
       0,     0,     0,     0,     0,     0,     0,   455,     0,     0,
       0,     0,     0,   458,     0,     0,   459,     0,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,     0,     0,   467,   468,     0,   322,
     323,   324,     0,   326,   327,   328,   329,   330,   469,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,   344,   345,   346,     0,     0,   349,   350,   351,   352,
     470,   471,   571,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   474,   475,     0,     0,
       0,     0,     0,     0,     0,  1620,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   479,
     480,   481,   482,   483,     0,   484,     0,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,    59,   572,
     497,   498,     0,     0,     0,     0,     0,     0,   499,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   503,   504,     0,    14,
       0,     0,   505,   506,     0,     0,   437,   438,     0,     0,
     507,     0,   508,     0,   509,   510,   443,   444,   445,   446,
     447,     0,     0,     0,     0,     0,   448,     0,   450,     0,
       0,     0,   453,     0,     0,     0,     0,     0,     0,     0,
     455,     0,     0,     0,     0,     0,   458,     0,     0,   459,
       0,     0,   460,     0,     0,     0,   463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   570,     0,     0,   467,
     468,     0,   322,   323,   324,     0,   326,   327,   328,   329,
     330,   469,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,     0,   344,   345,   346,     0,     0,   349,
     350,   351,   352,   470,   471,   571,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   474,
     475,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   479,   480,   481,   482,   483,     0,   484,     0,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,    59,   572,   497,   498,     0,     0,     0,     0,     0,
       0,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
     504,     0,    14,     0,     0,   505,   506,     0,     0,   437,
     438,     0,     0,   507,     0,   508,     0,   509,   510,   443,
     444,   445,   446,   447,     0,     0,     0,     0,     0,   448,
       0,   450,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,   455,     0,     0,     0,     0,     0,   458,
       0,     0,   459,     0,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
       0,     0,   467,   468,     0,   322,   323,   324,     0,   326,
     327,   328,   329,   330,   469,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,     0,   344,   345,   346,
       0,     0,   349,   350,   351,   352,   470,   471,   571,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   -69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   744,   745,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   479,   480,   481,   482,   483,
       0,   484,     0,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,    59,   572,   497,   498,     0,     0,
       0,     0,   744,   745,   499,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   502,   503,   504,     0,    14,     0,     0,   505,   506,
       0,     0,     0,     0,     0,     0,  1389,     0,   508,     0,
     509,   510,   746,   747,   748,   749,   750,   751,   752,   753,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
     764,   765,   766,   767,   768,   769,     0,     0,     0,     0,
     770,   771,   772,   773,   774,   775,     0,     0,   776,   777,
     778,   779,   780,   781,   782,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   754,   755,   756,
     757,   758,     0,     0,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,   783,   784,   776,   785,   786,   787,   788,
     789,   790,   791,   792,   793,   794,   697,     0,     0,     0,
       0,     0,   316,     0,     0,   795,   796,     0,   317,     0,
     509,   683,     0,     0,   318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,   320,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,   321,     0,     0,
       0,     0,   795,   796,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,   317,     0,
       0,     0,     0,     0,   318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   319,     0,    58,     0,     0,     0,
       0,     0,   320,     0,     0,     0,     0,     0,     0,   355,
       0,     0,     0,     0,     0,     0,     0,   321,     0,     0,
       0,     0,     0,    59,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
       0,     0,   316,     0,     0,     0,     0,     0,   317,     0,
       0,     0,     0,     0,   318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   319,     0,    58,     0,     0,     0,
       0,     0,   320,     0,     0,     0,     0,     0,     0,   355,
       0,     0,     0,     0,     0,     0,     0,   321,     0,     0,
       0,     0,     0,    59,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
       0,   610,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   355,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,   615,   317,     0,     0,     0,     0,     0,
     318,     0,     0,     0,     0,   616,     0,     0,     0,     0,
     319,   617,     0,     0,     0,     0,     0,     0,   320,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   321,     0,     0,     0,     0,     0,   356,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,   317,     0,     0,     0,     0,     0,
     318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     319,     0,    58,     0,     0,     0,     0,     0,   320,     0,
       0,     0,     0,     0,     0,   355,     0,     0,     0,     0,
       0,     0,     0,   321,     0,     0,     0,     0,     0,    59,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   744,   745,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   356,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   355,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   615,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     876,   877,   878,   879,   880,   881,   882,   883,   754,   755,
     756,   757,   758,   884,   885,   761,   762,   763,   764,   886,
     766,   767,   768,   769,  -367,   356,   744,   745,   770,   771,
     772,   773,   887,   888,     0,     0,   776,   777,   778,   889,
     890,   891,   782,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   892,   784,     0,   785,   786,   787,   788,   789,   790,
     791,   792,   793,   794,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,   796,     0,     0,     0,   509,   683,
       0,     0,     0,   876,   877,   878,   879,   880,   881,   882,
     883,   754,   755,   756,   757,   758,   884,   885,   761,   762,
     763,   764,   886,   766,   767,   768,   769,   744,   745,     0,
       0,   770,   771,   772,   773,   887,   888,     0,     0,   776,
     777,   778,   889,   890,   891,   782,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1087,     0,     0,
       0,     0,     0,     0,   892,   784,     0,   785,   786,   787,
     788,   789,   790,   791,   792,   793,   794,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   795,   796,     0,     0,
       0,   509,   683,     0,   876,   877,   878,   879,   880,   881,
     882,   883,   754,   755,   756,   757,   758,   884,   885,   761,
     762,   763,   764,   886,   766,   767,   768,   769,   744,   745,
       0,     0,   770,   771,   772,   773,   887,   888,     0,     0,
     776,   777,   778,   889,   890,   891,   782,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1291,     0,
       0,     0,     0,     0,     0,   892,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,     0,   509,   683,     0,   876,   877,   878,   879,   880,
     881,   882,   883,   754,   755,   756,   757,   758,   884,   885,
     761,   762,   763,   764,   886,   766,   767,   768,   769,   744,
     745,     0,     0,   770,   771,   772,   773,   887,   888,     0,
       0,   776,   777,   778,   889,   890,   891,   782,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1670,
       0,     0,     0,     0,     0,     0,   892,   784,     0,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   795,   796,
       0,     0,     0,   509,   683,     0,   876,   877,   878,   879,
     880,   881,   882,   883,   754,   755,   756,   757,   758,   884,
     885,   761,   762,   763,   764,   886,   766,   767,   768,   769,
     744,   745,     0,     0,   770,   771,   772,   773,   887,   888,
       0,     0,   776,   777,   778,   889,   890,   891,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1694,     0,     0,     0,     0,     0,     0,   892,   784,     0,
     785,   786,   787,   788,   789,   790,   791,   792,   793,   794,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   795,
     796,     0,     0,     0,   509,   683,     0,   876,   877,   878,
     879,   880,   881,   882,   883,   754,   755,   756,   757,   758,
     884,   885,   761,   762,   763,   764,   886,   766,   767,   768,
     769,   744,   745,     0,     0,   770,   771,   772,   773,   887,
     888,     0,     0,   776,   777,   778,   889,   890,   891,   782,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   744,   745,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1695,     0,     0,     0,     0,     0,     0,   892,   784,
       0,   785,   786,   787,   788,   789,   790,   791,   792,   793,
     794,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     795,   796,     0,     0,     0,   509,   683,     0,   876,   877,
     878,   879,   880,   881,   882,   883,   754,   755,   756,   757,
     758,   884,   885,   761,   762,   763,   764,   886,   766,   767,
     768,   769,     0,     0,     0,     0,   770,   771,   772,   773,
     887,   888,     0,     0,   776,   777,   778,   889,   890,   891,
     782,   754,   755,   756,   757,   758,     0,     0,   761,   762,
     763,   764,     0,   766,   767,   768,   769,   744,   745,     0,
       0,   770,     0,   772,   773,     0,     0,     0,     0,   776,
     777,   778,  1701,     0,     0,   782,     0,     0,     0,   892,
     784,     0,   785,   786,   787,   788,   789,   790,   791,   792,
     793,   794,   744,   745,     0,     0,     0,     0,     0,     0,
       0,   795,   796,     0,     0,     0,   509,   683,     0,     0,
       0,     0,     0,     0,     0,   784,     0,   785,   786,   787,
     788,   789,   790,   791,   792,   793,   794,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   795,   796,     0,     0,
       0,   509,   683,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   754,   755,   756,   757,   758,     0,     0,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,   808,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,   818,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,     0,   744,   745,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,   834,   754,   755,   756,
     757,   758,     0,     0,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,   849,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1160,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,     0,   744,   745,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1164,   754,   755,   756,   757,   758,     0,     0,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1175,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,     0,   744,   745,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1176,   754,   755,   756,
     757,   758,     0,     0,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1178,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,     0,   744,   745,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1179,   754,   755,   756,   757,   758,     0,     0,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1312,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,     0,   744,   745,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1324,   754,   755,   756,
     757,   758,     0,     0,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1327,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1374,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,     0,   744,   745,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1482,   754,   755,   756,   757,   758,     0,     0,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1483,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1504,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,     0,   744,   745,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1506,   754,   755,   756,
     757,   758,     0,     0,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1508,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1512,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,     0,   744,   745,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1559,   754,   755,   756,   757,   758,     0,     0,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1583,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1584,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,     0,   744,   745,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1586,   754,   755,   756,
     757,   758,     0,     0,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1593,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1596,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,     0,   744,   745,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1606,   754,   755,   756,   757,   758,     0,     0,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,     0,
       0,  1679,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,     0,     0,  1680,   754,   755,   756,
     757,   758,   744,   745,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,     0,     0,     0,     0,     0,   744,   745,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,   839,     0,     0,   754,   755,   756,
     757,   758,     0,     0,   761,   762,   763,   764,     0,   766,
     767,   768,   769,     0,     0,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
       0,   782,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,  1110,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,  1263,
       0,     0,   754,   755,   756,   757,   758,   744,   745,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,     0,     0,     0,
       0,     0,   744,   745,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,   796,  1279,
       0,     0,   754,   755,   756,   757,   758,     0,     0,   761,
     762,   763,   764,     0,   766,   767,   768,   769,     0,     0,
       0,     0,   770,     0,   772,   773,     0,     0,     0,     0,
     776,   777,   778,     0,     0,     0,   782,   754,   755,   756,
     757,   758,     0,     0,   761,   762,   763,   764,     0,   766,
     767,   768,   769,   236,   237,     0,     0,   770,     0,   772,
     773,     0,     0,     0,     0,   776,   777,   778,     0,     0,
     238,   782,     0,     0,     0,     0,   784,     0,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,   744,   745,
       0,     0,     0,     0,     0,     0,     0,   795,   796,  1413,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   784,     0,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,   796,  1419,     0,     0,     0,     0,     0,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,     0,     0,
     257,   258,   259,     0,     0,     0,     0,     0,     0,   260,
     261,   262,   263,   264,     0,     0,   265,   266,   267,   268,
     269,   270,   271,   754,   755,   756,   757,   758,   744,   745,
     761,   762,   763,   764,     0,   766,   767,   768,   769,     0,
       0,     0,     0,   770,     0,   772,   773,     0,     0,     0,
       0,   776,   777,   778,     0,     0,     0,   782,     0,     0,
       0,     0,   272,     0,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,     0,     0,   285,   286,     0,     0,   291,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   784,     0,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   795,   796,
       0,     0,     0,   754,   755,   756,   757,   758,   744,   745,
     761,   762,   763,   764,     0,   766,   767,   768,   769,     0,
       0,     0,     0,   770,     0,   772,   773,     0,     0,   941,
       0,   776,   777,   778,     0,     0,     0,   782,     0,     0,
       0,     0,     0,   744,   745,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   784,     0,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   795,   796,
       0,     0,     0,   754,   755,   756,   757,   758,     0,     0,
     761,   762,   763,   764,     0,   766,   767,   768,   769,     0,
       0,     0,     0,   770,     0,   772,   773,     0,     0,  1186,
       0,   776,   777,   778,     0,     0,     0,   782,   754,   755,
     756,   757,   758,   744,   745,   761,   762,   763,   764,     0,
     766,   767,   768,   769,     0,     0,     0,     0,   770,     0,
     772,   773,     0,     0,     0,     0,   776,   777,   778,     0,
       0,     0,   782,     0,     0,     0,     0,   784,     0,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   795,   796,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   784,  1268,   785,   786,   787,   788,   789,   790,
     791,   792,   793,   794,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,   796,     0,     0,     0,   754,   755,
     756,   757,   758,   744,   745,   761,   762,   763,   764,     0,
     766,   767,   768,   769,     0,     0,     0,     0,   770,     0,
     772,   773,     0,     0,     0,     0,   776,   777,   778,     0,
       0,     0,   782,     0,     0,     0,     0,     0,   744,   745,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1281,     0,     0,     0,     0,     0,
       0,     0,   784,     0,   785,   786,   787,   788,   789,   790,
     791,   792,   793,   794,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,   796,     0,     0,     0,   754,   755,
     756,   757,   758,     0,     0,   761,   762,   763,   764,     0,
     766,   767,   768,   769,     0,     0,     0,     0,   770,     0,
     772,   773,     0,     0,     0,     0,   776,   777,   778,     0,
       0,     0,   782,   754,   755,   756,   757,   758,   744,   745,
     761,   762,   763,   764,     0,   766,   767,   768,   769,     0,
       0,     0,     0,   770,     0,   772,   773,     0,     0,     0,
    1369,   776,   777,   778,     0,     0,     0,   782,     0,     0,
       0,     0,   784,     0,   785,   786,   787,   788,   789,   790,
     791,   792,   793,   794,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,   796,     0,     0,     0,     0,  1607,
       0,     0,     0,     0,     0,     0,     0,   784,     0,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   795,   796,
       0,     0,     0,   754,   755,   756,   757,   758,   744,   745,
     761,   762,   763,   764,     0,   766,   767,   768,   769,     0,
       0,     0,     0,   770,     0,   772,   773,     0,     0,     0,
       0,   776,   777,   778,     0,     0,     0,   782,     0,     0,
       0,     0,     0,   744,   745,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   784,     0,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   795,   796,
       0,     0,     0,   754,   755,   756,   757,   758,     0,     0,
     761,   762,   763,   764,     0,   766,   767,   768,   769,     0,
       0,     0,     0,   770,     0,   772,   773,     0,     0,     0,
       0,   776,   777,   778,     0,     0,     0,  -767,   754,   755,
     756,   757,   758,   744,   745,   761,   762,   763,   764,     0,
     766,   767,   768,   769,     0,     0,     0,     0,   770,     0,
     772,   773,     0,     0,     0,     0,   776,     0,   778,   744,
     745,     0,     0,     0,     0,     0,     0,   784,     0,   785,
     786,   787,   788,   789,   790,   791,   792,   793,   794,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   795,   796,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   785,   786,   787,   788,   789,   790,
     791,   792,   793,   794,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,   796,     0,     0,     0,   754,   755,
     756,   757,   758,     0,     0,   761,   762,   763,   764,     0,
     766,   767,   768,   769,     0,     0,     0,     0,   770,     0,
     772,   773,     0,     0,   754,   755,   756,   757,   758,     0,
       0,   761,   762,   763,   764,     0,   766,   767,   768,   769,
       0,     0,     0,     0,   770,     0,   772,   773,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   987,
       0,     0,     0,     0,     0,   786,   787,   788,   789,   790,
     791,   792,   793,   794,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,   796,     0,     0,     0,     0,     0,
     991,     0,     0,   788,   789,   790,   791,   792,   793,   794,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   795,
     796,   322,   323,   324,     0,   326,   327,   328,   329,   330,
     469,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,     0,   344,   345,   346,     0,     0,   349,   350,
     351,   352,   322,   323,   324,     0,   326,   327,   328,   329,
     330,   469,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,     0,   344,   345,   346,     0,     0,   349,
     350,   351,   352,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   988,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     989,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   992,
       0,     0,     0,     0,     0,     0,     0,     0,   322,   323,
     324,   993,   326,   327,   328,   329,   330,   469,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,     0,
     344,   345,   346,     0,     0,   349,   350,   351,   352,   322,
     323,   324,     0,   326,   327,   328,   329,   330,   469,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
       0,   344,   345,   346,     0,     0,   349,   350,   351,   352,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1265,     0,  1066,  1067,     0,
       0,     0,     0,     0,     0,     0,     0,  1266,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   187,     0,   188,
       0,   189,   190,   191,   192,   193,  1068,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,  1069,   205,
     206,   207,     0,     0,   208,   209,   210,   211,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1070,
    1071,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   214
};

static const yytype_int16 yycheck[] =
{
      14,    15,   216,   577,   232,   449,   488,   381,   630,   381,
     526,   165,   597,   543,   596,   394,   696,   388,   698,  1188,
     591,    82,   593,   381,   595,   698,   477,   478,   825,   473,
     827,   718,   829,   918,   932,   722,     5,   567,   635,   733,
     938,    33,   425,   426,   423,  1334,     8,    20,    15,    16,
      64,    65,    66,  1392,    22,     4,   105,   129,   130,    20,
      20,    20,    53,    33,   127,    20,    19,    20,    20,    20,
      57,    46,    53,  1433,   143,   449,    63,   132,    21,    22,
    1560,   155,   144,   145,   146,   142,   160,   160,   170,    57,
     195,   449,   106,   107,   108,   109,   180,   188,   695,   473,
     697,   170,   699,   127,     7,   163,   182,   486,   487,   214,
     707,   143,   188,   126,   192,   473,   129,   130,   196,   716,
     196,    33,   213,   180,   158,  1587,   189,   182,     0,   192,
     214,   106,   214,   102,   103,     7,   170,   581,  1618,   211,
     212,    15,    16,   217,   217,   214,   232,    50,    60,    61,
     837,   379,   306,   215,  1493,   685,    21,    22,    30,   191,
      32,  1592,    34,   224,   188,   189,   394,   191,    40,   863,
     194,  1531,  1603,  1635,   223,   118,   119,   158,    50,     5,
       6,   215,  1351,   126,    56,   872,   129,   130,   131,   170,
    1457,   182,  1552,   136,   181,   423,   163,   425,   211,   212,
     127,   168,   194,   170,   648,   170,   173,   581,    80,   896,
     137,   160,   124,  1644,  1645,   188,   128,   170,   232,   181,
      62,   152,   909,   581,   194,   912,   195,   188,   188,   188,
     102,   103,   200,   188,   678,   618,   188,   188,   452,   170,
     623,   155,   212,   211,  1533,   214,   160,   214,   218,   310,
     212,   216,   466,   118,   119,  1522,  1523,   182,   486,   487,
     158,   126,   189,   128,   129,   130,   131,   195,   211,   212,
     182,   136,   170,  1540,   648,   654,   188,    49,   190,   192,
     974,   180,   194,   196,   196,   197,   214,  1185,  1173,   163,
     648,   742,   217,   379,   168,   975,   170,  1466,   677,   173,
     212,   215,    47,   217,   678,   182,   218,   180,     5,     6,
     182,     8,  1548,   127,  1550,   127,   214,   127,  1554,  1555,
     678,    93,    67,   137,    33,   137,   143,   137,   182,   201,
    1597,  1598,   197,   198,   199,   200,   201,   192,   127,    36,
     212,   196,   356,   216,   217,   152,   211,   212,   137,  1234,
     189,    60,    61,   170,   582,   583,   584,   188,   586,   587,
     957,   215,   590,   170,   592,   379,   594,   811,   596,   740,
     384,   127,   170,   171,   172,   189,   191,   189,  1614,   189,
     394,   137,   213,   182,   185,   186,   170,   143,   746,   747,
     748,   749,   750,   751,   752,   753,   127,   196,   182,   195,
     189,   759,   760,   201,   188,  1290,   137,   765,   188,   423,
     107,   425,   213,   857,   985,   124,   774,   775,   214,   128,
     634,   779,   780,   781,   182,   783,   654,   185,   807,  1123,
     182,   445,   446,   189,   817,   215,   188,   811,   143,   170,
     188,   188,   170,   973,   196,   659,   182,   152,   831,   677,
     833,   152,  1350,   811,   182,   213,  1253,    34,   189,   160,
     196,   943,  1360,   194,   188,   170,   188,   215,   215,   170,
     188,   953,   486,   487,   180,   926,   188,   188,   180,   858,
     859,   190,   188,   857,   188,   194,    63,   196,   197,   191,
     704,   215,   871,   215,  1116,   213,   582,   583,   584,   857,
     586,   587,   213,   212,   590,  1185,   592,  1187,   594,   218,
     143,   894,  1185,   214,   728,   188,   744,   745,   188,  1199,
    1105,  1103,   901,   902,   188,   904,  1199,   188,   188,   908,
     107,   910,   911,   188,   913,   188,   476,   477,   478,   946,
     213,   948,   770,   213,   995,   996,   997,   143,   188,   170,
    1347,   215,    21,    22,   215,   215,   152,   214,   188,   136,
     215,   157,   215,   188,   188,   170,   143,   795,   582,   583,
     584,   188,   586,   587,   170,   215,   590,   182,   592,   807,
     594,   195,   596,    12,  1381,   215,   526,   191,   213,    21,
      22,   215,   196,   170,    23,    24,   213,   163,    75,   195,
     214,  1223,    79,  1174,   211,   449,   191,   835,   158,   191,
     214,   196,   182,   553,   196,   843,    93,    94,   846,   195,
     170,    98,    99,   100,   101,   170,   640,   855,   205,   214,
     858,   859,   214,   191,   862,   182,   194,   214,   214,   867,
     654,   158,   189,   871,   216,   217,   158,   116,   117,   118,
     119,   120,   806,   170,   123,   170,  1227,   126,   170,   128,
     129,   130,   131,   677,   508,   158,   191,   136,   182,   138,
     139,   196,   158,   901,   902,   188,   904,   170,   191,   893,
     908,   194,   910,   911,   170,   913,   118,   119,   144,   214,
     146,   192,  1489,   174,   126,   196,   128,   129,   130,   131,
     182,    21,    22,   170,   136,   170,   188,   182,   170,  1389,
     182,   725,   652,   188,   196,   170,   188,   192,   946,   182,
     948,    64,    65,    66,   196,   188,   195,   196,   197,   198,
     199,   200,   201,   196,   192,  1415,   181,   182,   196,   184,
     680,    57,   211,   212,   182,   181,   170,    63,   184,   835,
     188,   187,  1374,  1132,  1133,  1134,   600,   843,   196,   180,
     846,    57,    57,   106,   107,   108,   109,    63,    63,   855,
     192,     5,     6,    57,   196,   174,   862,    57,  1157,   211,
     212,   867,    57,    63,   191,   182,    57,   194,    63,   182,
     804,    25,    63,   807,    57,   188,   144,    31,   118,   119,
      63,   182,   742,   647,   648,   106,   126,   188,   128,   129,
     130,   131,   182,   182,   182,  1386,   136,   182,   188,   188,
     188,   835,   214,   188,   216,   217,  1054,    33,   672,   843,
    1346,   675,   846,   182,    68,    69,   144,   145,   146,   188,
    1214,   855,  1214,   180,   858,   859,   192,  1226,   862,   189,
     196,   174,   175,   867,    60,    61,  1214,   871,    88,    89,
     946,  1541,   948,  1091,    35,   180,   192,   192,   102,   103,
     196,   196,    35,   813,   192,  1103,   180,   180,   196,   199,
     200,   201,   192,   170,   171,   172,   196,   901,   902,   733,
     904,   211,   212,   214,   908,   181,   910,   911,   170,   913,
     174,   175,   176,   177,  1132,  1133,  1134,   213,   170,   143,
     170,  1591,  1592,   170,    22,  1602,   170,   214,   124,  1599,
     213,  1495,   128,  1603,  1611,   191,   160,   161,   162,  1157,
     180,   180,   946,   191,   948,   214,   170,  1559,    43,   195,
    1323,   214,   195,    33,   195,   214,   180,   174,   175,   176,
     195,  1638,   195,   195,   798,   195,  1636,   174,   175,   176,
     195,   195,   195,   195,  1644,  1645,   181,   182,   183,   195,
      60,    61,    10,    11,    12,   170,   916,   137,   212,   192,
     170,   170,   170,   214,   190,   213,   926,   212,   194,  1471,
     196,   197,  1474,   195,   195,   195,   214,   170,  1226,   195,
    1574,  1575,   195,   214,   182,  1091,   212,   163,   195,   195,
     195,   195,   218,   195,  1228,   214,   214,   214,  1698,   863,
     214,    37,   214,  1402,   182,   182,   217,    10,   182,   182,
     182,   214,   182,   182,   124,    66,   212,   214,   128,   181,
     214,    43,   170,   215,   195,   170,   213,   195,   182,   181,
    1624,   181,   191,    21,    22,   995,   996,   997,   181,   214,
    1000,    43,  1002,   132,  1004,    14,  1006,   214,  1008,   195,
    1010,   195,  1012,   195,  1014,   188,   191,  1091,   189,  1019,
     163,  1021,   217,  1311,   188,   929,   180,  1027,    13,  1103,
     188,   188,   170,   937,   214,     8,   215,    33,   170,  1039,
     190,  1041,   170,  1317,   194,   196,  1046,   197,  1048,   215,
    1050,   170,   170,  1053,   215,  1494,   181,   188,  1132,  1133,
    1134,   181,   212,   215,    60,    61,   213,   215,   218,   214,
     214,   170,  1576,   214,   195,   214,     1,   214,   195,   214,
     214,   214,   182,  1157,   214,   181,  1086,   214,   116,   117,
     118,   119,   120,    67,   196,   123,   124,   125,   126,   170,
     128,   129,   130,   131,   196,    43,   196,   215,   136,  1383,
     138,   139,   215,   170,  1402,   170,  1404,   214,  1118,   213,
     215,  1625,  1626,   170,   170,   213,   215,  1401,   124,   215,
     214,   127,   128,   213,  1573,   214,   170,   215,   182,   214,
     214,   137,  1576,   195,   214,   214,   214,   170,   215,   170,
     214,   182,  1226,   213,   215,   170,   170,   213,  1576,   213,
      33,  1665,   170,   214,   214,  1311,   194,   195,   196,   197,
     198,   199,   200,   201,   214,   214,   196,   215,   214,   214,
     381,    70,   214,   211,   212,   214,   182,    33,  1462,   214,
     391,  1625,  1626,   189,   190,   196,   215,   215,   194,   400,
     182,   197,   196,   214,   214,    53,  1494,  1625,  1626,   410,
     214,   214,   181,   215,    60,    61,   212,  1121,   215,   420,
     215,  1509,   218,   215,    21,    22,   215,   213,   429,   181,
     188,  1665,    33,   181,   213,   182,   213,  1311,   439,   188,
     215,   181,   706,   215,   215,   215,   213,  1665,   449,   215,
     215,   213,   215,   454,   214,   456,   215,    81,  1404,    60,
      61,    33,     1,    44,   465,   137,    84,  1486,   804,  1487,
    1487,  1487,   473,   474,   475,   230,   955,  1181,   124,  1487,
    1487,   127,   128,     1,   980,  1573,  1462,   488,    60,    61,
    1245,   137,  1399,  1519,  1198,   496,  1286,  1571,   499,  1425,
    1204,   502,   503,   504,   505,   506,    52,  1211,  1428,   433,
    1520,  1215,  1599,  1469,   515,  1194,   698,    -1,   499,   116,
     117,   118,   119,   124,   565,    -1,    -1,   128,  1402,   126,
    1404,   128,   129,   130,   131,   445,   445,    33,    -1,   136,
      -1,   138,   139,   189,   190,    -1,  1346,    -1,   194,    -1,
      -1,   197,   124,    -1,    33,    -1,   128,  1261,    -1,    -1,
      -1,    -1,    -1,  1509,    60,    61,   212,    -1,    -1,  1273,
      -1,    -1,   218,    -1,  1278,    -1,    -1,    -1,    -1,    -1,
     581,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,    -1,    -1,   194,    -1,   196,   197,    -1,    -1,  1673,
     197,   198,   199,   200,   201,    -1,    -1,    -1,   609,    -1,
      -1,   212,    -1,    -1,   211,   212,    -1,   218,   190,    -1,
    1494,    -1,   194,    -1,   196,   197,    -1,    -1,   124,    -1,
      -1,    -1,   128,    -1,    -1,  1509,    -1,    -1,    -1,    -1,
     212,    -1,    -1,    -1,  1348,   124,   218,   648,    -1,   128,
      -1,    -1,    -1,    -1,   655,   656,    -1,   658,    -1,    -1,
      -1,    -1,   663,    -1,    -1,    -1,    -1,    -1,   669,  1373,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   678,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1560,    -1,   689,   690,
     691,   692,   693,   694,   190,   696,    -1,   698,   194,  1573,
     196,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,    -1,    -1,   194,   212,   196,   197,    -1,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    21,    22,  1433,
      -1,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,   218,
      -1,    -1,    -1,    -1,  1618,   746,   747,   748,   749,   750,
     751,   752,   753,   754,   755,    -1,    -1,   758,   759,   760,
     761,   762,   763,    -1,   765,   766,    -1,   768,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,   790,
     791,   792,   793,   794,    -1,   796,    -1,    -1,    -1,   800,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     811,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,  1531,  1628,   123,
     124,   125,   126,  1537,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,  1552,   850,
     144,   145,   146,    -1,    -1,    -1,   857,    -1,    -1,  1563,
      -1,    -1,    -1,   864,    -1,   866,    -1,  1667,    -1,  1669,
      -1,    -1,    -1,    -1,   875,   876,   877,   878,   879,   880,
     881,   882,   883,   884,   885,   886,   887,   888,   889,   890,
     891,   892,    -1,  1693,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1619,    -1,   211,   212,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   927,   928,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     941,    -1,   943,    -1,    -1,    -1,    -1,    -1,    -1,   950,
      -1,    -1,   953,    -1,    -1,    -1,    -1,   958,     1,   960,
     961,   962,     5,     6,     7,    -1,     9,    10,    11,    -1,
      13,   972,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    42,
      -1,    44,    45,    -1,    -1,    48,    -1,    50,    51,    52,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,  1067,    -1,    -1,    -1,
    1071,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,   180,   181,   182,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,   197,   198,   199,    -1,   201,    -1,
    1161,   204,   205,    -1,    -1,    -1,    -1,    -1,    -1,   212,
      -1,   214,    -1,   216,   217,    -1,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,  1186,    -1,    -1,    -1,    -1,
      -1,    -1,  1193,  1194,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1214,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,  1224,  1225,    -1,    -1,    25,    -1,    -1,
    1231,    -1,    -1,    31,    -1,    -1,    -1,    -1,  1239,    -1,
      -1,    -1,   124,    41,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    49,    21,    22,  1255,    -1,  1257,    -1,    -1,    -1,
      -1,  1262,    -1,    -1,    -1,    -1,    64,  1268,    -1,    -1,
      -1,  1272,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,   190,    -1,
      -1,    -1,   194,  1314,   196,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     212,    -1,    -1,  1334,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,   156,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,   170,    -1,  1375,  1376,  1377,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1410,
      -1,  1412,    -1,    -1,    -1,    -1,    -1,  1418,   216,    -1,
      -1,    -1,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,  1442,   211,   212,  1445,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1453,  1454,  1455,    -1,    -1,    -1,    -1,  1460,
      -1,    -1,    -1,    -1,  1465,    60,    61,  1468,    -1,    -1,
    1471,  1472,    -1,  1474,  1475,    -1,  1477,  1478,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1487,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,  1500,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,  1533,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1556,    -1,    -1,    -1,    -1,
      -1,    -1,   190,  1564,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,  1576,    -1,  1578,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1594,  1595,   190,    -1,    -1,    -1,   194,
      -1,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1615,  1616,    -1,   212,    -1,  1620,
      -1,    -1,     1,   218,  1625,  1626,     5,     6,     7,    -1,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,  1652,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    42,  1665,    44,    45,    -1,    -1,    48,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      -1,   180,   181,   182,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,     5,
       6,    -1,    -1,   212,    -1,   214,    -1,   216,   217,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,   747,   748,   749,   750,   751,   752,   753,    -1,
      -1,    -1,   118,   119,   759,   760,    -1,    -1,   124,    -1,
     765,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,   774,
     775,    -1,    -1,    -1,   779,   780,   781,   143,   783,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      33,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,   190,    -1,    -1,    -1,   194,    -1,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,    -1,     5,     6,    -1,   212,    -1,   214,    -1,
     216,   217,   218,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,   124,    -1,    45,    -1,   128,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    60,    61,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,   190,    -1,    -1,
      -1,   194,    -1,   196,   197,    -1,   118,   119,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,   212,
      -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    33,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,    -1,    33,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,   190,    -1,
      -1,    -1,   194,    -1,    -1,   197,   198,   199,    -1,   201,
      60,    61,   204,   205,    -1,    -1,    -1,     5,     6,    -1,
     212,    -1,   214,    -1,   216,   217,   218,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,   124,    -1,    45,    -1,   128,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    65,   128,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,   190,    -1,    -1,    -1,   194,    -1,   196,   197,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,    -1,    -1,   212,   194,    -1,   196,   197,    -1,   218,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,   212,   151,   152,   153,   154,   155,   218,   157,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,    -1,   197,
     198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,
      -1,     5,     6,    -1,   212,    -1,   214,    -1,   216,   217,
     218,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,
     204,   205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,
     214,    -1,   216,   217,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,
     201,    -1,    -1,   204,   205,    -1,    -1,     5,     6,    -1,
      -1,   212,    -1,   214,    -1,   216,   217,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    52,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,
       5,     6,    -1,    -1,   212,    -1,   214,    -1,   216,   217,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,
     205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,
      -1,   216,   217,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    70,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,
      -1,    -1,   204,   205,    -1,    -1,     5,     6,    -1,    -1,
     212,    -1,   214,   215,   216,   217,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    33,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    33,   157,    -1,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    60,    61,    -1,    -1,   190,    -1,
      -1,    -1,   194,    -1,   196,   197,    -1,    -1,   197,   198,
     199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,    -1,
     212,     5,     6,   212,    -1,   214,   218,   216,   217,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    33,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,   124,    -1,
      -1,    45,   128,    -1,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,   190,    -1,    -1,    -1,   194,    -1,
     196,   197,    -1,    -1,   118,   119,    -1,   124,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    33,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    60,
      61,    -1,    -1,   190,    -1,    -1,    -1,   194,    -1,   196,
     197,    -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,
     204,   205,    -1,    -1,    -1,   212,     5,     6,   212,   213,
     214,   218,   216,   217,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    33,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,   124,    -1,    -1,    45,   128,    -1,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,   190,
      -1,    -1,    -1,   194,    -1,   196,   197,    -1,    -1,   118,
     119,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,   212,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    33,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    60,    61,    -1,    -1,   190,    -1,
      -1,    -1,   194,    -1,   196,   197,    -1,    -1,   197,   198,
     199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,    -1,
     212,     5,     6,   212,    -1,   214,   218,   216,   217,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    33,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,   124,    -1,
      -1,    45,   128,    -1,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,   190,    -1,    -1,    -1,   194,    -1,
     196,   197,    -1,    -1,   118,   119,    -1,   124,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,   190,    -1,    -1,    -1,   194,    -1,   196,
     197,    -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,
     204,   205,    -1,    -1,    -1,   212,     5,     6,   212,   213,
     214,   218,   216,   217,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,     5,
       6,    -1,    -1,   212,    -1,   214,    -1,   216,   217,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    70,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,    -1,
     216,   217,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,
      -1,   204,   205,    -1,    -1,     5,     6,    -1,    -1,   212,
      -1,   214,   215,   216,   217,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,
      -1,   201,    -1,    -1,   204,   205,    -1,    -1,     5,     6,
      -1,    -1,   212,    -1,   214,   215,   216,   217,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,
      -1,     5,     6,    -1,    -1,   212,   213,   214,    -1,   216,
     217,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,
     204,   205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,
     214,    -1,   216,   217,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,
     201,    -1,    -1,   204,   205,    -1,    -1,    -1,    -1,     5,
       6,   212,    -1,   214,    -1,   216,   217,    13,    -1,    15,
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
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,    -1,
     216,   217,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,    -1,   197,   198,   199,    -1,   201,    -1,
      -1,   204,   205,    -1,    -1,    -1,    -1,     5,     6,   212,
      -1,   214,    -1,   216,   217,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,
       5,     6,    -1,    -1,   212,    -1,   214,    -1,   216,   217,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,
     205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,
      -1,   216,   217,    15,    16,    17,    18,    19,    -1,    -1,
      22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,
      -1,    -1,   204,   205,    -1,    -1,     5,     6,    -1,    -1,
     212,    -1,   214,    -1,   216,   217,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,     5,
       6,    -1,    -1,   212,    -1,   214,   215,   216,   217,    15,
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
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,    -1,
     216,   217,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,
      -1,   204,   205,    -1,    -1,     5,     6,    -1,    -1,   212,
      -1,   214,   215,   216,   217,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,
      -1,   201,    -1,    -1,   204,   205,    -1,    -1,     5,     6,
      -1,    -1,   212,    -1,   214,   215,   216,   217,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,
      -1,     5,     6,    -1,    -1,   212,    -1,   214,   215,   216,
     217,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,
     204,   205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,
     214,   215,   216,   217,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,
     201,    -1,    -1,   204,   205,    -1,    -1,     5,     6,    -1,
      -1,   212,    -1,   214,   215,   216,   217,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,
       5,     6,    -1,    -1,   212,    -1,   214,    -1,   216,   217,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,
     205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,
      -1,   216,   217,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   199,    -1,   201,
      -1,    -1,   204,   205,    -1,    -1,     5,     6,    -1,    -1,
     212,    -1,   214,    -1,   216,   217,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,     5,
       6,    -1,    -1,   212,    -1,   214,    -1,   216,   217,    15,
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
      -1,    -1,   118,   119,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    21,    22,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,    -1,    -1,    -1,    -1,   212,    -1,   214,    -1,
     216,   217,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,    -1,    -1,   144,   145,
     146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,   189,   190,   144,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,   211,   212,    -1,    25,    -1,
     216,   217,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,   211,   212,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   170,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   170,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,   170,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,
      41,   188,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   216,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,   143,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   170,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   216,    21,    22,   136,   137,
     138,   139,   140,   141,    -1,    -1,   144,   145,   146,   147,
     148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   216,   217,
      -1,    -1,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,   137,   138,   139,   140,   141,    -1,    -1,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,    -1,    -1,
      -1,    -1,    -1,    -1,   189,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,
      -1,   216,   217,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,    21,    22,
      -1,    -1,   136,   137,   138,   139,   140,   141,    -1,    -1,
     144,   145,   146,   147,   148,   149,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,    -1,
      -1,    -1,    -1,    -1,    -1,   189,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,    -1,   216,   217,    -1,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,   137,   138,   139,   140,   141,    -1,
      -1,   144,   145,   146,   147,   148,   149,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
      -1,    -1,    -1,    -1,    -1,    -1,   189,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,    -1,   216,   217,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,   137,   138,   139,   140,   141,
      -1,    -1,   144,   145,   146,   147,   148,   149,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,
     212,    -1,    -1,    -1,   216,   217,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,   137,   138,   139,   140,
     141,    -1,    -1,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   182,    -1,    -1,    -1,    -1,    -1,    -1,   189,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     211,   212,    -1,    -1,    -1,   216,   217,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,    -1,    -1,   144,   145,   146,   147,   148,   149,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,   182,    -1,    -1,   150,    -1,    -1,    -1,   189,
     190,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   212,    -1,    -1,    -1,   216,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,
      -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,   118,
     119,   120,    21,    22,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,   213,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,   213,
      -1,    -1,   116,   117,   118,   119,   120,    21,    22,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,   213,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      38,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,   213,    -1,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    -1,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,   144,   145,   146,   147,
     148,   149,   150,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,   204,   205,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,   142,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,   142,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
     178,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,    -1,    -1,   182,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,    -1,   146,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,
     212,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,   170,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    76,    77,    78,    79,   158,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,   170,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,
     212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   220,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   182,   201,   212,   221,   224,   230,   232,
     233,   238,   268,   272,   306,   384,   391,   395,   406,   451,
     456,   461,    19,    20,   170,   260,   261,   262,   163,   239,
     240,   170,   171,   172,   201,   234,   235,   236,   180,   392,
     170,   216,   223,    57,    63,   387,   387,   387,   143,   170,
     292,    34,    63,   107,   136,   205,   214,   264,   265,   266,
     267,   292,   182,     5,     6,     8,    36,   107,   403,    62,
     382,   189,   188,   191,   188,   235,    22,    57,   200,   211,
     237,   387,   388,   390,   388,   382,   462,   452,   457,   170,
     143,   231,   266,   266,   266,   214,   144,   145,   146,   188,
     213,    57,    63,   273,   275,    57,    63,   393,     5,     6,
      57,    63,   404,    57,    63,   383,    15,    16,   163,   168,
     170,   173,   214,   226,   261,   163,   240,   170,   234,   234,
     170,   182,   181,   388,   182,    57,    63,   222,   170,   170,
     170,   170,   174,   229,   215,   262,   266,   266,   266,   266,
     276,   170,   394,   407,   180,   385,   174,   175,   176,   225,
      15,    16,   163,   168,   170,   226,   258,   259,   237,   389,
     182,   463,   453,   458,   174,   215,    35,    71,    73,    75,
      76,    77,    78,    79,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    93,    94,    95,    98,    99,
     100,   101,   118,   119,   170,   271,   274,   180,   191,   106,
     401,   402,   380,   160,   217,   263,   356,   174,   175,   176,
     188,   215,   189,   180,   180,   180,    21,    22,    38,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   128,   129,   130,
     137,   138,   139,   140,   141,   144,   145,   146,   147,   148,
     149,   150,   190,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   204,   205,   211,   212,    35,    35,   214,
     269,   180,   277,   396,    75,    79,    93,    94,    98,    99,
     100,   101,   411,   170,   408,   181,   381,   262,   261,   182,
     217,   152,   170,   378,   379,   258,    19,    25,    31,    41,
      49,    64,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   156,   216,   292,   410,   412,
     413,   416,   422,   450,   464,   454,   459,   170,   170,   170,
     213,    22,   170,   213,   155,   215,   356,   366,   367,   191,
     270,   280,   386,   180,   191,   400,   180,   405,   356,   213,
     261,   214,    43,   188,   191,   194,   377,   195,   195,   195,
     214,   195,   195,   214,   195,   195,   195,   195,   195,   195,
     214,   292,    33,    60,    61,   124,   128,   190,   194,   197,
     212,   218,   421,   192,   415,   370,   373,   170,   137,   214,
       7,    50,   305,   182,   215,   450,     1,     5,     6,     9,
      10,    11,    13,    15,    16,    17,    18,    19,    25,    26,
      27,    28,    29,    31,    38,    39,    42,    44,    45,    48,
      51,    52,    54,    55,    58,    59,    65,    68,    69,    80,
     102,   103,   104,   105,   118,   119,   133,   134,   135,   151,
     152,   153,   154,   155,   157,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   171,   172,   173,   180,
     181,   182,   197,   198,   199,   204,   205,   212,   214,   216,
     217,   228,   230,   241,   242,   245,   248,   249,   251,   253,
     254,   255,   256,   278,   279,   281,   286,   291,   292,   293,
     297,   298,   299,   300,   301,   302,   303,   304,   306,   310,
     311,   318,   321,   324,   329,   332,   333,   335,   336,   337,
     339,   344,   347,   348,   355,   410,   466,   481,   492,   496,
     509,   512,   170,   182,   397,   398,   292,   362,   379,   213,
      65,   104,   171,   286,   348,   170,   170,   422,   127,   137,
     189,   376,   423,   428,   430,   348,   432,   426,   170,   417,
     434,   436,   438,   440,   442,   444,   446,   448,   348,   195,
     214,    33,   194,    33,   194,   212,   218,   213,   348,   212,
     218,   422,   170,   182,   465,   170,   182,   188,   368,   419,
     450,   455,   170,   371,   419,   460,   348,   152,   170,   375,
     409,   366,   195,   195,   348,   252,   195,   294,   412,   466,
     214,   292,   195,     5,   102,   103,   195,   214,   127,   291,
     322,   333,   348,   277,   195,   214,    61,   348,   214,   348,
     170,   195,   195,   214,   182,   195,   163,    58,   348,   214,
     277,   195,   214,   195,   195,   214,   195,   195,   127,   291,
     348,   348,   348,   217,   277,   324,   328,   328,   328,   214,
     214,   214,   214,   214,   214,    13,   422,    13,   422,    13,
     348,   491,   507,   195,   348,   195,   227,    13,   491,   508,
      37,   348,   348,   348,   348,   348,    13,    49,   322,   348,
     158,   170,   322,   467,   469,   217,   182,   182,   348,    10,
     324,   330,   170,   214,   182,   182,   182,   182,   182,    66,
     307,   268,   132,   182,    21,    22,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     136,   137,   138,   139,   140,   141,   144,   145,   146,   147,
     148,   149,   150,   189,   190,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   211,   212,   330,   214,   182,
     189,   181,   386,   181,   212,   268,   363,   195,   215,    43,
     182,   376,   291,   348,   450,   450,   420,   450,   215,   450,
     450,   215,   170,   414,   450,   269,   450,   269,   450,   269,
     368,   369,   371,   372,   215,   425,   282,   322,   213,   213,
     348,   182,   181,   191,   419,   181,   191,   419,   181,   215,
     214,    43,   127,   188,   189,   191,   194,   374,   482,   484,
     277,   409,   295,   214,   292,   195,   214,   319,   195,   195,
     195,   503,   322,   291,   322,   188,   108,   109,   110,   111,
     112,   113,   114,   115,   121,   122,   127,   140,   141,   147,
     148,   149,   189,    14,   422,   284,   508,   348,   348,   277,
     189,   312,   314,   348,   316,   191,   163,   348,   505,   322,
     488,   493,   322,   486,   422,   291,   348,   217,   268,   348,
     348,   348,   348,   348,   348,   409,    53,   197,   212,   214,
     348,   467,   470,   474,   490,   495,   409,   214,   470,   495,
     409,   142,   181,   182,   183,   475,   287,   277,   289,   176,
     177,   225,   409,   188,   511,   180,   409,    13,   188,   511,
     214,   137,   374,   511,   188,   215,   152,   157,   195,   292,
     338,   277,   250,   331,    70,   212,   215,   322,   469,   160,
     214,   309,   379,     4,   160,   327,   328,    19,   158,   170,
     410,    19,   158,   170,   410,   133,   134,   135,   278,   334,
     348,   334,   348,   334,   348,   334,   348,   334,   348,   334,
     348,   334,   348,   334,   348,   348,   348,   348,   334,   348,
     334,   348,   348,   348,   348,   170,   334,   348,   348,   158,
     170,   348,   348,   348,   410,   348,   348,   348,   334,   348,
     334,   348,   348,   348,   348,   334,   348,   334,   348,   334,
     348,   348,   334,   348,    22,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   129,   130,   158,   170,
     211,   212,   345,   410,   348,   215,   322,   348,   399,   267,
       8,   356,   361,   422,   170,   291,   348,   182,   196,   196,
     196,   419,   196,   196,   182,   196,   196,   270,   196,   270,
     196,   270,   196,   419,   196,   419,   285,   450,   215,   511,
     213,   450,   450,   348,   170,   170,   450,   291,   348,   422,
     422,    20,   450,    70,   322,   469,   480,   195,   348,   170,
     348,   450,   497,   499,   501,   422,   511,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   277,   196,   419,   215,   511,
     215,   257,   422,   422,   215,   422,   215,   422,   511,   422,
     422,   511,   422,   196,   327,   215,   215,   215,   215,   215,
     215,    20,   328,   212,   348,   215,   142,   182,   474,   185,
     186,   213,   478,   188,   182,   185,   213,   477,    20,   215,
     474,   181,   184,   476,    20,   348,   181,   491,   285,   285,
     348,    20,   491,   181,   280,    20,   409,   213,   348,   348,
     348,   215,   158,   170,   214,   214,   340,   342,    12,    23,
      24,   243,   244,   348,   268,   170,   215,   469,   467,   188,
     215,   215,   170,   308,   308,   214,   127,   137,   170,   189,
     194,   325,   326,   269,   195,   214,   195,   214,   328,   328,
     328,   214,   214,   213,    19,   158,   170,   410,   191,   158,
     170,   348,   214,   214,   158,   170,   348,     1,   214,   213,
     215,   182,   181,   213,    57,    63,   359,    67,   360,   182,
     196,   182,   424,   429,   431,   450,   433,   427,   170,   418,
     435,   196,   439,   196,   443,   196,   447,   368,   449,   371,
     196,   419,   215,    43,   374,   196,   196,   322,   196,   469,
     215,   215,   215,   170,   215,   182,   196,   215,   196,   422,
     422,   422,   196,   215,   214,   422,   348,   196,   196,   196,
     196,   215,   196,   196,   215,   196,   327,   269,   214,   322,
     470,   474,   348,   467,   478,   213,   348,   490,   213,   322,
     470,   181,   184,   187,   479,   213,   322,   196,   196,   178,
     322,   181,   322,    20,   215,   214,   137,   374,   348,   348,
     422,   269,   277,   348,    12,   246,   327,   215,   213,   212,
     188,   213,   215,   326,   170,   170,   214,   170,   170,   188,
     213,   270,   349,   348,   351,   348,   215,   322,   348,   195,
     214,   348,   214,   213,   348,   212,   215,   322,   214,   213,
     346,   215,   322,   182,    47,   360,    46,   106,   357,   327,
     437,   441,   445,   214,   450,   170,   348,   483,   485,   277,
     296,   215,   196,   419,   170,   214,   320,   196,   196,   196,
     504,   284,   196,   313,   315,   317,   506,   489,   494,   487,
     214,   330,   270,   215,   322,   182,   474,   478,   182,   474,
     213,   182,   288,   290,   182,   182,   322,   137,   374,   348,
     348,   348,   215,   215,   196,   270,   277,   247,   182,   269,
     215,   467,   170,   213,   191,   377,   215,   170,   325,   213,
     142,   277,   323,   422,   215,   450,   215,   215,   215,   353,
     348,   348,   215,   467,   215,   348,   215,    33,   358,   357,
     359,   282,   214,   214,   348,   170,   196,   348,   498,   500,
     502,   214,   215,   214,   348,   348,   348,   214,    70,   480,
     214,   214,   215,   348,   323,   215,   348,   478,   348,   479,
     491,   348,   214,   283,   491,   348,   182,   348,   348,   215,
     341,   196,   244,    26,   105,   248,   298,   299,   300,   302,
     348,   270,   213,   191,   377,   422,   376,   215,   127,   348,
     196,   196,   450,   215,   215,   213,   215,   364,   358,   375,
     215,   480,   480,   215,   196,   214,   215,   214,   214,   214,
     282,   284,   322,   480,   467,   468,   215,   182,   510,   510,
     510,   282,   510,   510,   348,   137,   374,   338,   343,   127,
     127,   348,   277,   215,   422,   376,   376,   291,   348,   348,
     350,   352,   196,   215,   274,   365,   214,   467,   471,   472,
     473,   473,   348,   348,   480,   480,   468,   215,   215,   511,
     473,   215,    53,   213,   181,   181,   188,   511,   181,   213,
     510,   348,   348,   338,   348,   376,   291,   348,   291,   348,
     182,   354,   182,   274,   467,   188,   511,   215,   215,   215,
     215,   473,   473,   215,   215,   215,   348,   213,   213,   181,
     215,   213,   291,   348,   182,   182,   277,   215,   214,   215,
     215,   182,   467,   215
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   219,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   221,   222,   222,
     222,   223,   223,   224,   225,   225,   225,   225,   226,   226,
     227,   227,   227,   228,   229,   229,   231,   230,   232,   233,
     234,   234,   234,   235,   235,   235,   235,   236,   236,   237,
     237,   238,   239,   239,   240,   240,   241,   242,   242,   243,
     243,   244,   244,   244,   245,   245,   246,   247,   246,   248,
     248,   248,   248,   248,   249,   250,   249,   252,   251,   253,
     254,   255,   257,   256,   258,   258,   258,   258,   258,   258,
     259,   259,   260,   260,   260,   261,   261,   261,   261,   261,
     261,   261,   261,   262,   262,   263,   263,   263,   264,   264,
     264,   264,   265,   265,   266,   266,   266,   266,   266,   266,
     266,   267,   267,   268,   268,   269,   269,   269,   270,   270,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   272,   273,   273,   273,   274,
     276,   275,   277,   277,   278,   278,   278,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   280,   280,
     280,   281,   282,   282,   283,   283,   284,   284,   285,   285,
     287,   288,   286,   289,   290,   286,   291,   291,   291,   291,
     291,   292,   292,   292,   293,   293,   295,   296,   294,   294,
     297,   297,   297,   297,   297,   297,   298,   299,   300,   300,
     300,   301,   301,   301,   302,   302,   303,   303,   303,   304,
     305,   305,   305,   306,   306,   307,   307,   308,   308,   309,
     309,   309,   309,   309,   309,   309,   309,   310,   310,   312,
     313,   311,   314,   315,   311,   316,   317,   311,   319,   320,
     318,   321,   321,   321,   321,   321,   321,   322,   322,   323,
     323,   323,   324,   324,   324,   325,   325,   325,   325,   325,
     326,   326,   327,   327,   327,   328,   328,   329,   331,   330,
     332,   332,   332,   332,   332,   332,   332,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   334,   334,   334,   334,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   336,   336,   337,
     337,   338,   338,   339,   340,   341,   339,   342,   343,   339,
     344,   344,   344,   344,   344,   344,   344,   345,   346,   344,
     347,   347,   347,   347,   347,   347,   347,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   349,   350,   348,   348,   348,   348,
     351,   352,   348,   348,   348,   353,   354,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   356,   356,
     356,   357,   357,   357,   358,   358,   359,   359,   359,   360,
     360,   361,   362,   363,   362,   364,   362,   365,   362,   362,
     366,   366,   367,   367,   368,   368,   369,   369,   370,   370,
     370,   371,   372,   372,   373,   373,   373,   374,   374,   375,
     375,   375,   375,   375,   375,   376,   376,   376,   377,   377,
     378,   378,   378,   378,   378,   379,   379,   379,   379,   379,
     380,   381,   380,   382,   382,   383,   383,   383,   384,   385,
     384,   386,   386,   386,   386,   387,   387,   387,   389,   388,
     390,   390,   391,   392,   391,   393,   393,   393,   394,   396,
     397,   395,   398,   399,   395,   400,   400,   401,   401,   402,
     403,   403,   403,   403,   404,   404,   404,   405,   405,   407,
     408,   406,   409,   409,   409,   409,   409,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   411,   411,   411,   411,   411,   411,
     411,   411,   412,   413,   413,   413,   414,   414,   415,   415,
     415,   417,   418,   416,   419,   419,   420,   420,   421,   421,
     422,   422,   422,   422,   422,   422,   423,   424,   422,   422,
     422,   425,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   426,   427,   422,   422,   428,
     429,   422,   430,   431,   422,   432,   433,   422,   422,   434,
     435,   422,   436,   437,   422,   422,   438,   439,   422,   440,
     441,   422,   422,   442,   443,   422,   444,   445,   422,   446,
     447,   422,   448,   449,   422,   450,   450,   450,   452,   453,
     454,   455,   451,   457,   458,   459,   460,   456,   462,   463,
     464,   465,   461,   466,   466,   466,   466,   466,   467,   467,
     467,   467,   467,   467,   467,   467,   468,   469,   470,   470,
     471,   471,   472,   472,   473,   473,   474,   474,   475,   475,
     476,   476,   477,   477,   478,   478,   478,   479,   479,   479,
     480,   480,   481,   481,   481,   481,   481,   481,   482,   483,
     481,   484,   485,   481,   486,   487,   481,   488,   489,   481,
     490,   490,   490,   491,   491,   492,   493,   494,   492,   495,
     495,   496,   496,   496,   497,   498,   496,   499,   500,   496,
     501,   502,   496,   496,   503,   504,   496,   496,   505,   506,
     496,   507,   507,   508,   508,   509,   509,   509,   509,   509,
     510,   510,   511,   511,   512,   512,   512,   512,   512,   512
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       1,     1,     1,     4,     1,     1,     2,     2,     3,     2,
       0,     2,     4,     3,     1,     2,     0,     4,     2,     2,
       1,     1,     1,     1,     2,     3,     3,     2,     4,     0,
       1,     2,     1,     3,     1,     3,     3,     3,     2,     1,
       1,     0,     2,     4,     1,     1,     0,     0,     3,     1,
       1,     1,     1,     1,     4,     0,     6,     0,     6,     2,
       3,     3,     0,     5,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     1,     5,     1,     3,     2,     3,     2,     1,     1,
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
       0,     4,     3,     7,     2,     2,     6,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     2,     2,     2,     0,     2,
       2,     3,     0,     2,     0,     4,     0,     2,     1,     3,
       0,     0,     7,     0,     0,     7,     3,     2,     2,     2,
       1,     1,     3,     2,     2,     3,     0,     0,     5,     1,
       2,     5,     5,     5,     6,     2,     1,     1,     1,     2,
       3,     2,     2,     3,     2,     3,     2,     2,     3,     4,
       1,     1,     0,     1,     1,     1,     0,     1,     3,     9,
       8,     8,     7,     8,     7,     7,     6,     3,     3,     0,
       0,     7,     0,     0,     7,     0,     0,     7,     0,     0,
       6,     5,     8,    10,     5,     8,    10,     1,     3,     1,
       2,     3,     1,     1,     2,     2,     2,     2,     2,     4,
       1,     3,     0,     4,     4,     1,     6,     6,     0,     7,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     6,     8,     5,
       6,     1,     4,     3,     0,     0,     8,     0,     0,     9,
       3,     4,     5,     6,     8,     5,     6,     0,     0,     5,
       3,     4,     4,     5,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     4,     3,     4,
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
       1,     1,     2,     2,     0,     1,     1,     0,     3,     0,
       0,     7,     1,     4,     3,     3,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     1,     3,     0,     2,
       3,     0,     0,     6,     1,     1,     1,     3,     3,     4,
       1,     1,     1,     1,     2,     3,     0,     0,     6,     4,
       5,     0,     9,     4,     2,     2,     3,     2,     3,     2,
       2,     3,     3,     3,     2,     0,     0,     6,     2,     0,
       0,     6,     0,     0,     6,     0,     0,     6,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     0,
       0,     6,     0,     0,     6,     1,     3,     3,     0,     0,
       0,     0,    10,     0,     0,     0,     0,    10,     0,     0,
       0,     0,    10,     1,     1,     1,     1,     1,     3,     3,
       5,     5,     6,     6,     8,     8,     1,     1,     1,     3,
       3,     5,     1,     2,     1,     0,     0,     2,     2,     1,
       2,     1,     2,     1,     2,     1,     1,     2,     1,     1,
       0,     1,     5,     4,     6,     7,     5,     7,     0,     0,
      10,     0,     0,    10,     0,     0,     9,     0,     0,     7,
       1,     3,     3,     3,     1,     5,     0,     0,    10,     1,
       3,     4,     4,     4,     0,     0,    11,     0,     0,    11,
       0,     0,    10,     5,     0,     0,     9,     5,     0,     0,
      10,     1,     3,     1,     3,     4,     3,     4,     7,     9,
       0,     3,     0,     1,     9,    10,    10,    10,     9,    10
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

  case 29: /* string_constant: "start of the string" "end of the string"  */
                                                           { (yyval.s) = new string(); }
    break;

  case 30: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 31: /* string_builder_body: string_builder_body character_sequence  */
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

  case 32: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                                        {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 33: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 34: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 35: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 36: /* $@1: %empty  */
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

  case 37: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
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

  case 38: /* options_declaration: "options" annotation_argument_list  */
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

  case 40: /* keyword_or_name: "name"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 41: /* keyword_or_name: "keyword"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 42: /* keyword_or_name: "type function"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 43: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 44: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 45: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 46: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 47: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 48: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 49: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 50: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 54: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 55: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 56: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 57: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 58: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 59: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 60: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 61: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 62: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 63: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 64: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 65: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 66: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 67: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 68: /* expression_else_one_liner: "else" $@2 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 69: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 70: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 71: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 72: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 73: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 74: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 75: /* $@3: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 76: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner "end of expression"  */
                                                                                                                                                   {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 77: /* $@4: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 78: /* expression_for_loop: "for" $@4 variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                                                                 {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 79: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 80: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 81: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 82: /* $@5: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 83: /* expression_with_alias: "assume" "name" '=' $@5 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 84: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 85: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 86: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 87: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 88: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 89: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 90: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 91: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 92: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 93: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 94: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 95: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 96: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 97: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 98: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 99: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 100: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 101: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 102: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 103: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 104: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 105: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 106: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 107: /* metadata_argument_list: metadata_argument_list "end of expression"  */
                                         {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 108: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 109: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 110: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 111: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 112: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 113: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 114: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 115: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 116: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 117: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 118: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 119: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 120: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 121: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 122: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 123: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 124: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 125: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 126: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 127: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 128: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 129: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 130: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 131: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 132: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 133: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 134: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 135: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 136: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 137: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 138: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 139: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 140: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 141: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 142: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 143: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 144: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 145: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 146: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 147: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 148: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 149: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 150: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 151: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 152: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 153: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 154: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 155: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 156: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 157: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 158: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 159: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 160: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 161: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 162: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 163: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 164: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 165: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 166: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 167: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 168: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 169: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 170: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 171: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 172: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 173: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 174: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 175: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 176: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 177: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 178: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 179: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 180: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 181: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 182: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 183: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 184: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 185: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 186: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 187: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 188: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 189: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 190: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 191: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 192: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 193: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 194: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 195: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 196: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 197: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 198: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 199: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 200: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 201: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 202: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 203: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 204: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 205: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 206: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 207: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 208: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 209: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 210: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 211: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 212: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 213: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 214: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 215: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 216: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 217: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 218: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 219: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 220: /* $@6: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 221: /* function_declaration: optional_public_or_private_function $@6 function_declaration_header expression_block  */
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

  case 222: /* expression_block: "begin of code block" expressions "end of code block"  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 223: /* expression_block: "begin of code block" expressions "end of code block" "finally" "begin of code block" expressions "end of code block"  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 224: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 225: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 226: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 227: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 228: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 232: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 233: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 235: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 236: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 237: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 238: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 239: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 240: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 241: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 242: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 243: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 244: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 245: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 246: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 247: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 248: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 249: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 250: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 251: /* expr_keyword: "keyword" expr expression_block  */
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

  case 252: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 253: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 254: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 255: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 256: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 257: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 258: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 259: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 260: /* $@7: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 261: /* $@8: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 262: /* expression_keyword: "keyword" '<' $@7 type_declaration_no_options_list '>' $@8 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 263: /* $@9: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 264: /* $@10: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 265: /* expression_keyword: "type function" '<' $@9 type_declaration_no_options_list '>' $@10 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 266: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 267: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 268: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 269: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 270: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 271: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 272: /* name_in_namespace: "name" "::" "name"  */
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

  case 273: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 274: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 275: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 276: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 277: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 278: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 279: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 280: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 281: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 282: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 283: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 284: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 285: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 286: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 287: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 288: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 289: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 290: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 291: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 292: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 293: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 294: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 295: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 296: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 297: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 298: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 299: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 300: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 301: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 302: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 303: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 304: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 305: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 306: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 307: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 308: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 309: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 310: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 311: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 312: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                           {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 313: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 314: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-5]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 315: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 316: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                   {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameList),tokAt(scanner,(yylsp[-4])),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 317: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 318: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 319: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 320: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 321: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 322: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 323: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 324: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 325: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 326: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 327: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 328: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 329: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 330: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 331: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 332: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 333: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 334: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 335: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 336: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 337: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 338: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 339: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 340: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 341: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 342: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 343: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 344: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 345: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 346: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 347: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 348: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 349: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 350: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 351: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 352: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 353: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 354: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 355: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 356: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 357: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 358: /* $@21: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 359: /* expr_full_block_assumed_piped: block_or_lambda $@21 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 360: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 361: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 362: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 363: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 364: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 365: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 366: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 367: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 368: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 387: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 388: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 389: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 390: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 402: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 404: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 405: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 406: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 407: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 408: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 409: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 410: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 411: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 412: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 413: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 414: /* $@22: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 415: /* $@23: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 416: /* func_addr_expr: '@' '@' '<' $@22 type_declaration_no_options '>' $@23 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 417: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 418: /* $@25: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 419: /* func_addr_expr: '@' '@' '<' $@24 optional_function_argument_list optional_function_type '>' $@25 func_addr_name  */
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

  case 420: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 421: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 422: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 423: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 424: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                       {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back(ExpressionPtr((yyvsp[-7].pExpression)));
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 425: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 426: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 427: /* $@26: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 428: /* $@27: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 429: /* expr_field: expr '.' $@26 error $@27  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 430: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 431: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 432: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 433: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
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

  case 434: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 435: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 436: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 437: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 438: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 439: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 440: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 442: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 443: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 444: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 445: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 446: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 447: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 448: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 449: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 450: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 451: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 452: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 453: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 454: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 455: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 456: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 457: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 458: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 459: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 460: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 461: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 462: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 463: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 464: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 465: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 466: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 467: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 468: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 469: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 470: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 471: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 472: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 473: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 474: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 475: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 476: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 477: /* expr: '(' expr_list optional_comma ')'  */
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

  case 478: /* expr: '(' make_struct_single ')'  */
                                      {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        for ( auto & arg : *(((ExprMakeStruct *)(yyvsp[-1].pExpression))->structs.back()) ) {
            mkt->values.push_back(arg->value);
            mkt->recordNames.push_back(arg->name);
        }
        delete (yyvsp[-1].pExpression);
        (yyval.pExpression) = mkt;
    }
    break;

  case 479: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 480: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 481: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 482: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 483: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 484: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 485: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 486: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 487: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 488: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 489: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 490: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 491: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 492: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 493: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 494: /* $@28: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 495: /* $@29: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 496: /* expr: expr "is" "type" '<' $@28 type_declaration_no_options '>' $@29  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 497: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 498: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 499: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 500: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 501: /* $@31: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 502: /* expr: expr "as" "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 503: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 504: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 505: /* $@32: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 506: /* $@33: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 507: /* expr: expr '?' "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 508: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 509: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 510: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 511: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 512: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 513: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 514: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 515: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 516: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 517: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 518: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 519: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 520: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 521: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 522: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 523: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 524: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 525: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 526: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 527: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 528: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 529: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 530: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 531: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 532: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 533: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 534: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 535: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 536: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 537: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 538: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 539: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 540: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 541: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 542: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 543: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 544: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 545: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 546: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 547: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 548: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 549: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 550: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 551: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 552: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 553: /* $@34: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 554: /* struct_variable_declaration_list: struct_variable_declaration_list $@34 structure_variable_declaration "end of expression"  */
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

  case 555: /* $@35: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 556: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@35 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 557: /* $@36: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 558: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@36 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 559: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' "end of expression"  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 560: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 561: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 562: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 563: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 564: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 565: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 566: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 567: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 568: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 569: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 570: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 571: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 572: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 573: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 574: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 575: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 576: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 577: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 578: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 579: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 580: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 581: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 582: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 583: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 584: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 585: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 586: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 587: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 588: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 589: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 590: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 591: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 592: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 593: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 594: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 595: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 596: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 597: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 598: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 599: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 600: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 601: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 602: /* global_variable_declaration_list: global_variable_declaration_list $@37 optional_field_annotation let_variable_declaration  */
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

  case 603: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 604: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 605: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 606: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 607: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 608: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 609: /* $@38: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 610: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@38 optional_field_annotation let_variable_declaration  */
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

  case 611: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 612: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 613: /* enum_list: enum_list "name" "end of expression"  */
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

  case 614: /* enum_list: enum_list "name" '=' expr "end of expression"  */
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

  case 615: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 616: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 617: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 618: /* $@39: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 619: /* single_alias: optional_public_or_private_alias "name" $@39 '=' type_declaration  */
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

  case 623: /* $@40: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 625: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 626: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 627: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 628: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 629: /* $@41: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 630: /* $@42: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 631: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@41 enum_list $@42 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 632: /* $@43: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 633: /* $@44: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 634: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@43 enum_list $@44 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 635: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 636: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 637: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 638: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 639: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 640: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 641: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 642: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 643: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 644: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 645: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 646: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 647: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 648: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 649: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 650: /* $@46: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 651: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
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

  case 652: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 653: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 654: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 655: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 656: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 657: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 658: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 659: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 660: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 661: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 662: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 663: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 664: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 665: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 666: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 667: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 668: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 669: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 670: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 671: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 672: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 673: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 674: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 675: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 676: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 677: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 678: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 679: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 680: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 681: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 682: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 683: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 684: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 685: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 686: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 687: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 688: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 689: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 690: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 691: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 692: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 693: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 694: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 695: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 696: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 697: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 698: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 699: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 700: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 701: /* $@47: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 702: /* $@48: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 703: /* bitfield_type_declaration: "bitfield" '<' $@47 bitfield_bits '>' $@48  */
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

  case 706: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 707: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 708: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 709: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 710: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 711: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 712: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 713: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 714: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 715: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 716: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 717: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 718: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 719: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 720: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 721: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 722: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 723: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 724: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 725: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 726: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 727: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 728: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 729: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 730: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 731: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 732: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 733: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 734: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 735: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 736: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 737: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 738: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 739: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 740: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 741: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 742: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 743: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 744: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 745: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 746: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 747: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 748: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 749: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 750: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 751: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 752: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 753: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 754: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 755: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 756: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 757: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 758: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 759: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 760: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 761: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 762: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 763: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 764: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 765: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 766: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 767: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 768: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 769: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 770: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 771: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 772: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 773: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 774: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 775: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 776: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 777: /* type_declaration: type_declaration '|' '#'  */
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

  case 778: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 779: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 780: /* $@78: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 781: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 782: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 "begin of code block" $@78 tuple_alias_type_list $@79 "end of code block"  */
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

  case 783: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 784: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 785: /* $@82: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 786: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 787: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 "begin of code block" $@82 variant_alias_type_list $@83 "end of code block"  */
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

  case 788: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 789: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 790: /* $@86: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 791: /* $@87: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 792: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 "begin of code block" $@86 bitfield_alias_bits $@87 "end of code block"  */
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

  case 793: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 794: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 795: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 796: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 797: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 798: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 799: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 800: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 801: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 802: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 803: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 804: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 805: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 806: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 807: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 808: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 809: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 810: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 811: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 812: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 813: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 814: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 815: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 816: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 817: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 830: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 831: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 832: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 833: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 834: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 835: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 836: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 837: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 838: /* $@88: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 839: /* $@89: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 840: /* make_struct_decl: "struct" '<' $@88 type_declaration_no_options '>' $@89 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 841: /* $@90: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 842: /* $@91: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 843: /* make_struct_decl: "class" '<' $@90 type_declaration_no_options '>' $@91 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 844: /* $@92: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 845: /* $@93: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 846: /* make_struct_decl: "variant" '<' $@92 type_declaration_no_options '>' $@93 '(' make_variant_dim ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 847: /* $@94: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 848: /* $@95: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 849: /* make_struct_decl: "default" '<' $@94 type_declaration_no_options '>' $@95 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 850: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 851: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 852: /* make_tuple: make_tuple ',' expr  */
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

  case 853: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 854: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 855: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 856: /* $@96: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 857: /* $@97: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 858: /* make_tuple_call: "tuple" '<' $@96 type_declaration_no_options '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 859: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 860: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 861: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 862: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 863: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 864: /* $@98: %empty  */
                                       { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 865: /* $@99: %empty  */
                                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 866: /* make_dim_decl: "array" "struct" '<' $@98 type_declaration_no_options '>' $@99 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 867: /* $@100: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 868: /* $@101: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 869: /* make_dim_decl: "array" "tuple" '<' $@100 type_declaration_no_options '>' $@101 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 870: /* $@102: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 871: /* $@103: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 872: /* make_dim_decl: "array" "variant" '<' $@102 type_declaration_no_options '>' $@103 '(' make_variant_dim ')'  */
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

  case 873: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 874: /* $@104: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 875: /* $@105: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 876: /* make_dim_decl: "array" '<' $@104 type_declaration_no_options '>' $@105 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
            mka->gen2 = true;
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

  case 877: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 878: /* $@106: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 879: /* $@107: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 880: /* make_dim_decl: "fixed_array" '<' $@106 type_declaration_no_options '>' $@107 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 881: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 882: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 883: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 884: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 885: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 886: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 887: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 888: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 889: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 890: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 891: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 892: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 893: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 894: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 895: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 896: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 897: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 898: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 899: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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


