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
#define YYLAST   15291

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  219
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  294
/* YYNRULES -- Number of rules.  */
#define YYNRULES  893
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1697

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
     971,   975,   988,  1006,  1009,  1017,  1028,  1039,  1050,  1053,
    1060,  1064,  1071,  1072,  1076,  1077,  1078,  1082,  1085,  1092,
    1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,
    1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,
    1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,
    1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,
    1136,  1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,
    1146,  1147,  1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,
    1156,  1157,  1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,
    1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,
    1176,  1177,  1178,  1179,  1184,  1202,  1203,  1204,  1208,  1214,
    1214,  1231,  1235,  1246,  1255,  1264,  1270,  1271,  1272,  1273,
    1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,  1283,
    1284,  1285,  1286,  1287,  1288,  1289,  1290,  1294,  1299,  1305,
    1311,  1322,  1323,  1327,  1328,  1332,  1333,  1337,  1341,  1348,
    1348,  1348,  1354,  1354,  1354,  1363,  1397,  1400,  1403,  1406,
    1412,  1413,  1424,  1428,  1431,  1439,  1439,  1439,  1442,  1448,
    1451,  1455,  1459,  1466,  1473,  1479,  1483,  1487,  1490,  1493,
    1501,  1504,  1507,  1515,  1518,  1526,  1529,  1532,  1540,  1546,
    1547,  1548,  1552,  1553,  1557,  1558,  1562,  1567,  1575,  1581,
    1587,  1596,  1608,  1611,  1617,  1617,  1617,  1620,  1620,  1620,
    1625,  1625,  1625,  1633,  1633,  1633,  1639,  1649,  1660,  1673,
    1683,  1694,  1709,  1712,  1718,  1719,  1726,  1737,  1738,  1739,
    1743,  1744,  1745,  1746,  1747,  1751,  1756,  1764,  1765,  1766,
    1770,  1775,  1782,  1789,  1789,  1798,  1799,  1800,  1801,  1802,
    1803,  1804,  1808,  1809,  1810,  1811,  1812,  1813,  1814,  1815,
    1816,  1817,  1818,  1819,  1820,  1821,  1822,  1823,  1824,  1825,
    1826,  1830,  1831,  1832,  1833,  1838,  1839,  1840,  1841,  1842,
    1843,  1844,  1845,  1846,  1847,  1848,  1849,  1850,  1851,  1852,
    1853,  1854,  1859,  1866,  1878,  1883,  1893,  1897,  1904,  1907,
    1907,  1907,  1912,  1912,  1912,  1925,  1929,  1933,  1938,  1945,
    1954,  1959,  1966,  1966,  1966,  1973,  1977,  1987,  1996,  2005,
    2009,  2012,  2018,  2019,  2020,  2021,  2022,  2023,  2024,  2025,
    2026,  2027,  2028,  2029,  2030,  2031,  2032,  2033,  2034,  2035,
    2036,  2037,  2038,  2039,  2040,  2041,  2042,  2043,  2044,  2045,
    2046,  2047,  2048,  2049,  2050,  2051,  2052,  2053,  2059,  2060,
    2061,  2062,  2063,  2076,  2077,  2078,  2079,  2080,  2081,  2082,
    2083,  2084,  2085,  2086,  2087,  2090,  2093,  2094,  2097,  2097,
    2097,  2100,  2105,  2109,  2113,  2113,  2113,  2118,  2121,  2125,
    2125,  2125,  2130,  2133,  2134,  2135,  2136,  2137,  2138,  2139,
    2140,  2141,  2143,  2147,  2148,  2153,  2157,  2158,  2159,  2160,
    2161,  2162,  2163,  2167,  2171,  2175,  2179,  2183,  2187,  2191,
    2195,  2199,  2206,  2207,  2208,  2212,  2213,  2214,  2218,  2219,
    2223,  2224,  2225,  2229,  2230,  2234,  2245,  2248,  2248,  2267,
    2266,  2282,  2281,  2295,  2304,  2313,  2323,  2324,  2328,  2331,
    2340,  2341,  2345,  2348,  2351,  2367,  2376,  2377,  2381,  2384,
    2387,  2401,  2402,  2406,  2412,  2418,  2421,  2425,  2431,  2440,
    2441,  2442,  2446,  2447,  2451,  2458,  2463,  2472,  2478,  2489,
    2492,  2497,  2502,  2510,  2521,  2524,  2524,  2544,  2545,  2549,
    2550,  2551,  2555,  2558,  2558,  2577,  2580,  2583,  2598,  2617,
    2618,  2619,  2624,  2624,  2650,  2651,  2655,  2656,  2656,  2660,
    2661,  2662,  2666,  2676,  2681,  2676,  2693,  2698,  2693,  2713,
    2714,  2718,  2719,  2723,  2729,  2730,  2731,  2732,  2736,  2737,
    2738,  2742,  2745,  2751,  2756,  2751,  2776,  2783,  2788,  2797,
    2803,  2814,  2815,  2816,  2817,  2818,  2819,  2820,  2821,  2822,
    2823,  2824,  2825,  2826,  2827,  2828,  2829,  2830,  2831,  2832,
    2833,  2834,  2835,  2836,  2837,  2838,  2839,  2840,  2844,  2845,
    2846,  2847,  2848,  2849,  2850,  2851,  2855,  2866,  2870,  2877,
    2889,  2896,  2905,  2910,  2913,  2926,  2926,  2926,  2939,  2940,
    2944,  2948,  2955,  2959,  2966,  2967,  2968,  2969,  2970,  2985,
    2991,  2991,  2991,  2995,  3000,  3007,  3007,  3014,  3018,  3022,
    3027,  3032,  3037,  3042,  3046,  3050,  3055,  3059,  3063,  3068,
    3068,  3068,  3074,  3081,  3081,  3081,  3086,  3086,  3086,  3092,
    3092,  3092,  3097,  3101,  3101,  3101,  3106,  3106,  3106,  3115,
    3119,  3119,  3119,  3124,  3124,  3124,  3133,  3137,  3137,  3137,
    3142,  3142,  3142,  3151,  3151,  3151,  3157,  3157,  3157,  3166,
    3169,  3180,  3196,  3196,  3201,  3206,  3196,  3231,  3231,  3236,
    3242,  3231,  3267,  3267,  3272,  3277,  3267,  3307,  3308,  3309,
    3310,  3311,  3315,  3322,  3329,  3335,  3341,  3348,  3355,  3361,
    3370,  3376,  3384,  3389,  3396,  3401,  3408,  3413,  3419,  3420,
    3424,  3425,  3430,  3431,  3435,  3436,  3440,  3441,  3445,  3446,
    3447,  3451,  3452,  3453,  3457,  3458,  3462,  3468,  3475,  3483,
    3490,  3498,  3507,  3507,  3507,  3515,  3515,  3515,  3522,  3522,
    3522,  3529,  3529,  3529,  3540,  3543,  3549,  3563,  3569,  3575,
    3581,  3581,  3581,  3591,  3596,  3603,  3612,  3617,  3624,  3624,
    3624,  3634,  3634,  3634,  3644,  3644,  3644,  3654,  3663,  3663,
    3663,  3683,  3690,  3690,  3690,  3700,  3705,  3712,  3715,  3721,
    3729,  3738,  3746,  3766,  3791,  3792,  3796,  3797,  3802,  3805,
    3808,  3811,  3814,  3817
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

#define YYPACT_NINF (-1474)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-761)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1474,    39, -1474, -1474,    40,   -85,   355,   -65, -1474,   -70,
     191,   191,   191, -1474,   124,   252, -1474, -1474,    57, -1474,
   -1474, -1474,   268, -1474,    63, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474,    88, -1474,   147,    65,   193,
   -1474, -1474, -1474, -1474,   355, -1474,    26, -1474,   191,   191,
   -1474, -1474,    63, -1474, -1474, -1474, -1474, -1474,   273,   349,
   -1474, -1474, -1474,   252,   252,   252,   285, -1474,   709,   152,
   -1474, -1474, -1474, -1474,   593,   648,   339,   683, -1474,   692,
      50,    40,   430,   -85,   418,   470, -1474,   591,   591, -1474,
     499,   492,    51,   501,   707,   545,   555,   557, -1474,   567,
     548, -1474, -1474,   -34,    40,   252,   252,   252,   252, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474,   574, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474,   627, -1474, -1474, -1474, -1474, -1474,
     691,   180, -1474, -1474, -1474, -1474,   741, -1474, -1474, -1474,
   -1474, -1474,   623, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474,   657, -1474,   111, -1474,    30,   666,   709, 15121,
   -1474,   304,   736, -1474,  -108, -1474, -1474, -1474,   696, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474,   171, -1474,   655, -1474,
     667,   688,   693, -1474, -1474, 13518, -1474, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474,   844,   846, -1474,   674,   710, -1474,   659, -1474,   723,
   -1474,   720,    40,    40,    67,   478, -1474, -1474, -1474,   180,
   -1474,  9999, -1474, -1474, -1474,   726,   733, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474,   735,   697, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474,   889, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474,   742,   700, -1474, -1474,   196,   724,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474,   737,   727,   739, -1474,  -108,   224, -1474, -1474,    40,
     706,   878,   541, -1474, -1474,   728,   729,   730,   708,   731,
     732, -1474, -1474, -1474,   714, -1474, -1474, -1474, -1474, -1474,
     744, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474,   746, -1474, -1474, -1474,   753,   754, -1474, -1474,
   -1474, -1474,   756,   758,   716,   124, -1474, -1474, -1474, -1474,
   -1474,  1015,   740, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
     784,   818, -1474,   743, -1474,   149, -1474,   -77,  9999, -1474,
    2260,   -52, -1474,   124, -1474, -1474, -1474,   478,   745, -1474,
    9391,   786,   790,  9999, -1474,    -6, -1474, -1474, -1474,  9391,
   -1474, -1474,   792, -1474,   242,   368,   400, -1474, -1474,  9391,
     293, -1474, -1474, -1474,    20, -1474, -1474, -1474,    94,  5530,
   -1474,   751,  9753,   370,  9853,   446, -1474, -1474,  9391, -1474,
   -1474,   484,   168, -1474,   740, -1474,   770,   771,  9391, -1474,
   -1474, -1474, -1474, -1474,    98,   -71,   772,    32,  3289, -1474,
   -1474,   710,   346,  5733,   750,  9391,   798,   775,   776,   759,
   -1474,   793,   779,   809,  5936,   225,   388,   781, -1474,   410,
     782,   783,  3492,  9391,  9391,   207,   207,   207,   766,   767,
     778,   780,   785,   787, -1474,  1584,  2151,  6141, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474,  6344,   789, -1474,  6549,   948,
   -1474,  9391,  9391,  9391,  9391,  9391,  5124,  9391, -1474,   773,
   -1474, -1474,   811,   815,  9391,   981, -1474, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474,   570, -1474,   -64,   820, -1474,
     821,   823,   824, -1474,   827, -1474, -1474,   934, -1474, -1474,
   -1474, -1474,   800, -1474, -1474,   -35, -1474, -1474, -1474, -1474,
   -1474, -1474,  1723, -1474,   796, -1474, -1474, -1474, -1474, -1474,
   -1474,   125, -1474,   833, -1474, -1474,    35, -1474, -1474,   802,
     828,   830, -1474, 10552, -1474,   979,  1567, -1474, -1474, -1474,
    3898,  9999,  9999,  9999, 10587,  9999,  9999,   822,   856,  9999,
     674,  9999,   674,  9999,   674, 10099,   860, 10687, -1474,  9391,
   -1474, -1474, -1474, -1474,   825, -1474, -1474, 13037,  9391, -1474,
    1015,   849, -1474,   854,   -19, -1474, -1474,   583, -1474,   740,
     855,   848,   583, -1474,   859, 10787,   829,   998, -1474,   -40,
   -1474, -1474, -1474, 13553,   484, -1474,   831, -1474, -1474,   124,
     423, -1474,   847,   857,   858, -1474,  9391,  3898, -1474,   861,
     912, 10182,  1036,  9999,  9391,  9391, 14458,  9391, 13553,   862,
   -1474, -1474,  9391, -1474, -1474,   863,   893, 14458,  9391, -1474,
   -1474,  9391, -1474, -1474,  9391, -1474,  9999,  3898, -1474, 10182,
     172,   172,   840, -1474,   800, -1474, -1474, -1474,  9391,  9391,
    9391,  9391,  9391,  9391,   484,  2677,   484,  2881,   484, 13653,
   -1474,   695, -1474, 13553, -1474,   599,   484, -1474,   871,   880,
     172,   172,   -53,   172,   172,   484,  1053,   881, 14458,   881,
     385, -1474, -1474, 13553, -1474, -1474, -1474, -1474,  4101, -1474,
   -1474, -1474, -1474, -1474, -1474,   177,    71,   207, -1474, 14885,
   14916,  4304,  4304,  4304,  4304,  4304,  4304,  4304,  4304,  9391,
    9391, -1474, -1474,  9391,  4304,  4304,  9391,  9391,  9391,   898,
    4304,  9391,   463,  9391,  9391,  9391,  9391,  9391,  9391,  4304,
    4304,  9391,  9391,  9391,  4304,  4304,  4304,  9391,  4304,  6752,
    9391,  9391,  9391,  9391,  9391,  9391,  9391,  9391,  9391,  9391,
   15043,  9391, -1474,  6955, -1474,  9391, -1474,   -52, -1474,   252,
    1063,  -108,  9999, -1474,   902, -1474,  3898, -1474, 10295,   459,
     474,   877,   521, -1474,   528,   616, -1474, -1474,   366,   617,
     724,   619,   724,   625,   724, -1474,   239, -1474,   244, -1474,
    9999,   868,   881, -1474, -1474, 13137, -1474, -1474,  9999, -1474,
   -1474,  9999, -1474, -1474, -1474,  9391,   923, -1474,   925, -1474,
    9999, -1474,  3898,  9999,  9999, -1474,    24,  9999,  5327,  7158,
     928,  9391,  9999, -1474, -1474, -1474,  9999,   881, -1474,   861,
    9391,  9391,  9391,  9391,  9391,  9391,  9391,  9391,  9391,  9391,
    9391,  9391,  9391,  9391,  9391,  9391,  9391,  9391,   710,   142,
     886,   871, 14458, 10822, -1474, -1474,  9999,  9999, 10922,  9999,
   -1474, -1474, 11022,  9999,   881,  9999,  9999,   881,  9999,   137,
   -1474, 10182, -1474,    71, 11057, 11157, 11257, 11292, 11392, 11492,
      27,   207,   890,    82,  3085,  4509,  7361, 13753,   915,   -17,
     176,   920,    68,    34,  7564,   -17,   422,    36,  9391,   929,
    9391, -1474, -1474,  9999, -1474,  9999, -1474,  9391,   676,    41,
    9391,   939, -1474,    42,   484,  9391,   896,   906,   917,   918,
     431, -1474, -1474,   677,  9391,   800,   -42,  4714, -1474,   219,
     915,   911,   963,   963, -1474, -1474,   927,   -57,   674, -1474,
     947,   930, -1474, -1474,   950,   932, -1474, -1474,   207,   207,
     207, -1474, -1474, 10441, -1474, 10441, -1474, 10441, -1474, 10441,
   -1474, 10441, -1474, 10441, -1474, 10441, -1474, 10441,   663,   663,
     481, -1474, 10441, -1474, 10441,   481, 10144, 10144,   933, -1474,
   10441,   816,   935, -1474, 13172,    31,    31,   796, 14458,   663,
     663, -1474, 10441, -1474, 10441, 14714, 14593, 14624, -1474, 10441,
   -1474, 10441, -1474, 10441, 14493, -1474, 10441, 15012, 13788, 14743,
    1270,   999,   481,   481,   568,   568,   816,   816,   816,   473,
    9391,   936,   938,   502,  9391,  1133,   951, 13272, -1474,   226,
   13888,   967,   231,   715,  1097,   984,   690, -1474, -1474, 10406,
   -1474, -1474, -1474, -1474,  9999, -1474, -1474,   997, -1474, -1474,
     973, -1474,   974, -1474,   975, -1474, 10099, -1474,   860,   450,
     740, -1474, -1474, -1474,   740,   740, 11527, -1474,  1131,    -5,
   -1474, 10182,  1687,  1765,  9391,   630,   518,   301,   960,   962,
    1006, 11627,   412, 11727,   633,  9999,  9999,  9999,  1913,   964,
   14458, 14458, 14458, 14458, 14458, 14458, 14458, 14458, 14458, 14458,
   14458, 14458, 14458, 14458, 14458, 14458, 14458, 14458, -1474,   966,
    9999, -1474, -1474, -1474,  9391,  2406,  2800, -1474,  3004, -1474,
    3023,   969,  4301,  4427,   970,  4506,    71,   674, -1474, -1474,
   -1474, -1474, -1474,   972,  9391, -1474,  9391,  9391,  9391,  4919,
   13037,    21,  9391,   532,   518,   176, -1474, -1474,   965, -1474,
    9391,  9391, -1474,   976, -1474,  9391,   518,   552,   977, -1474,
   -1474,  9391, 14458, -1474, -1474,   471,   475, 13988,  9391, -1474,
   -1474,  2474,  9391,    48, -1474, -1474,  9391,  9391,  9999,   674,
     710, -1474, -1474,  9391, -1474,  1945,    71,    -1, -1474,   986,
     288,  9594, -1474, -1474, -1474,   317,   305,   -57,  1017,  1018,
     978,  1021,  1032, -1474,   341,   724, -1474,  9391, -1474,  9391,
   -1474, -1474, -1474,  7767,  9391, -1474,  1008,  1002, -1474, -1474,
    9391,  1004, -1474, 13372,  9391,  7970,  1005, -1474, 13407, -1474,
    8173, -1474, -1474, -1474, -1474,  1031, -1474, -1474,   451, -1474,
      38, -1474,    71, -1474, -1474, -1474, -1474,   740, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474,  1009,  9999, -1474,  1050,  9391, -1474, -1474,   248,
   -1474,  1011, -1474, -1474, -1474,   530, -1474,  1054,  1016, -1474,
   -1474,  4632,  4711,  4837, -1474, -1474,  9391,  4916, 14458, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,   531,   724,
    8376,   595, 11762, 14458, 14458,   -17,   176, 14458,  1020,   236,
     915, -1474, -1474, 14458,   920, -1474,   602,   -17,  1019, -1474,
   -1474, -1474, -1474,   613, -1474, -1474, -1474,   614, -1474,   615,
    9391, 11862, 11962,  5042,   724, -1474, 13553, -1474,  1047,   674,
   -1474,  1023,  4714,  1061,  1022,   629,   308, -1474, -1474,  1066,
   -1474, -1474,   -57,  1026,   -25,  9999, 11997,  9999, 12097, -1474,
     318, 12197, -1474,  9391,   961,  9391, -1474, 12232,  4714, -1474,
     343,  9391, -1474, -1474, -1474,   350, -1474,  1207,    38, -1474,
   -1474,   715,  1027, -1474, -1474, -1474,  9391,   740, -1474, 14458,
    1029,  1030, -1474, -1474, -1474,  9391,  1075,  1051,  9391, -1474,
   -1474, -1474, -1474,  1040,  1041,  1044,  9391,  9391,  9391,  1045,
    1176,  1046,  1048,  8579, -1474,   -25, -1474,   351,  9391,   253,
     176, -1474,  9391,  9391,  9391,  9391,   552, -1474,  9391,  9391,
    1049,  9391,  9391,   618, -1474, -1474, -1474,  1065,   677,  3695,
   -1474,   724, -1474,   365, -1474,   639,  9999,    -6, -1474,  1052,
   -1474, -1474,  8782, -1474, -1474,  6059, -1474,   636, -1474, -1474,
   -1474,  9999, 12332, 12432, -1474,   382, -1474, 12467, -1474, -1474,
   -1474,  1207,   484,  1055,  1176,  1176, 12567,  1068,  1057, 12667,
    1058,  1059,  1060,  9391, -1474,  9391,   481,   481,   481,  9391,
   -1474, -1474,  1176,   518, -1474, 12702, -1474, -1474, 14023,  9391,
    9391, -1474, 12802, 14458, 14458, 14023, -1474,  1083,   481,  9391,
   -1474,  1083, 14023,  9391,   334, -1474, -1474,  8985,  9188, -1474,
   -1474, -1474, -1474, -1474, 14458,   710,  1062,  9999,    -6,  1569,
    9391, -1474,  9391, 14493, -1474, -1474,   653, -1474, -1474,  1067,
   -1474, 15121, -1474, -1474, -1474,   105,   105, -1474,  9391,  9391,
   -1474,  1176,  1176,   518,  1070,  1072,   881,   105,   915,  1073,
   -1474,  1213,  1056, 14458, 14458,   272,  1087,  1094,  1088,  1099,
    1077, 14023, -1474,   334,  9391,  9391, 14458, -1474, -1474,  1569,
    9391,  9391, 14123,   961, -1474, -1474, -1474, -1474,  1096, 15121,
     518,   915,  1093, -1474,  1078,  1079, 12902, 12937,   105,   105,
    1080, -1474, -1474,  1081,  1084, -1474,  9391,  1085,  9391,  9391,
    1098,  1102, -1474,  1104, -1474, -1474,  1109, -1474, 14458,  9391,
   14223, 14258, -1474, -1474, -1474,   710,   358,  1086, -1474, -1474,
   -1474, -1474, -1474,  1138,  1139, -1474, -1474, -1474, 14458, -1474,
   14458, 14458, -1474, -1474, -1474, -1474, 14358, -1474, -1474, -1474,
   -1474,   518, -1474, -1474, -1474,   359, -1474
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   122,     1,   302,     0,     0,     0,   617,   303,     0,
     609,   609,   609,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   597,     6,    11,     5,     4,    13,
      12,    14,    93,    94,    92,   101,   103,    38,    54,    51,
      52,    40,    41,    42,     0,    43,    49,    39,   609,   609,
      22,    21,   597,   611,   610,   782,   772,   777,     0,   270,
      36,   109,   110,     0,     0,     0,   111,   113,   120,     0,
     108,    17,   635,   634,   215,   619,     0,   638,   598,   599,
       0,     0,     0,     0,    44,     0,    50,     0,     0,    47,
       0,     0,   609,     0,    18,     0,     0,     0,   272,     0,
       0,   119,   114,     0,     0,     0,     0,     0,     0,   123,
     217,   216,   219,   214,   621,   620,     0,   637,   636,   640,
     639,   643,   601,   600,   603,    99,   100,    97,    98,    96,
       0,     0,    95,   104,    55,    53,    49,    46,    45,   612,
     614,   616,     0,   618,    19,    20,    23,   783,   773,   778,
     271,    34,    37,   118,     0,   115,   116,   117,   121,     0,
     622,     0,   631,   594,   532,    24,    25,    29,     0,    88,
      89,    86,    87,    85,    84,    90,     0,    48,     0,   615,
       0,     0,     0,    35,   112,     0,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,     0,     0,   129,   124,     0,   623,     0,   632,     0,
     644,   595,     0,     0,   534,     0,    26,    27,    28,     0,
     102,     0,   784,   774,   779,   183,   184,   181,   132,   133,
     135,   134,   136,   137,   138,   139,   165,   166,   163,   164,
     156,   167,   168,   157,   154,   155,   182,   176,     0,   180,
     169,   170,   171,   172,   143,   144,   145,   140,   141,   142,
     153,     0,   159,   160,   158,   151,   152,   147,   146,   148,
     149,   150,   131,   130,   175,     0,   161,   162,   532,   127,
     247,   220,   605,   678,   681,   684,   685,   679,   682,   680,
     683,     0,   629,   641,   602,   532,     0,   105,   107,     0,
       0,   584,   582,   604,    91,     0,     0,     0,     0,     0,
       0,   651,   671,   652,   687,   653,   657,   658,   659,   660,
     677,   664,   665,   666,   667,   668,   669,   670,   672,   673,
     674,   675,   742,   656,   663,   676,   749,   756,   654,   661,
     655,   662,     0,     0,     0,     0,   686,   704,   707,   705,
     706,   769,   613,   692,   562,   568,   185,   186,   179,   174,
     187,   177,   173,     0,   125,   301,   556,     0,     0,   218,
       0,   624,   626,     0,   633,   546,   645,     0,     0,   106,
       0,     0,     0,     0,   583,     0,   710,   733,   736,     0,
     739,   729,     0,   695,   743,   750,   757,   763,   766,     0,
       0,   719,   724,   718,     0,   732,   728,   721,     0,     0,
     723,   708,     0,   785,   775,   780,   188,   178,     0,   299,
     300,     0,   532,   126,   128,   249,     0,     0,     0,    64,
      65,    77,   438,   439,     0,     0,     0,     0,   287,   432,
     285,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     286,     0,     0,     0,     0,     0,     0,     0,   677,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,     0,     0,     0,   355,   357,
     356,   358,   359,   360,   361,     0,     0,    30,     0,   221,
     226,     0,     0,     0,     0,     0,     0,     0,   337,   338,
     436,   435,     0,     0,     0,     0,   242,   237,   234,   233,
     235,   236,   269,   248,   228,   515,   227,   433,     0,   506,
      72,    73,    70,   240,    71,   241,   243,   305,   232,   505,
     504,   503,   122,   509,   434,     0,   229,   508,   507,   479,
     440,   480,   362,   441,     0,   437,   787,   791,   788,   789,
     790,     0,   606,     0,   605,   630,   547,   596,   533,     0,
       0,     0,   515,     0,   586,   587,     0,   580,   581,   579,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     124,     0,   124,     0,   124,     0,     0,     0,   715,   251,
     726,   727,   720,   722,     0,   725,   709,     0,     0,   771,
     770,     0,   693,     0,   270,   699,   698,     0,   563,   558,
       0,     0,     0,   569,     0,     0,     0,   646,   554,   573,
     557,   832,   835,     0,     0,   275,   279,   278,   284,     0,
       0,   323,     0,     0,     0,   868,     0,     0,   291,   288,
       0,   332,     0,     0,   255,     0,   273,     0,     0,     0,
     314,   317,     0,   246,   320,     0,     0,    58,     0,    79,
     872,     0,   841,   850,     0,   838,     0,     0,   296,   293,
     468,   469,   338,   350,   122,   268,   266,   267,     0,     0,
       0,     0,     0,     0,     0,   810,     0,     0,     0,   848,
     875,     0,   259,     0,   262,     0,     0,   877,   886,     0,
     445,   444,   481,   443,   442,     0,     0,   886,   332,   886,
     339,   244,   245,     0,    75,   353,   224,   513,     0,   231,
     238,   239,   290,   295,   304,     0,   347,     0,   230,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   470,   471,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     422,     0,   223,     0,   607,     0,   625,   627,   642,     0,
       0,   532,     0,   585,     0,   589,     0,   593,   362,     0,
       0,     0,   700,   713,     0,     0,   688,   690,     0,     0,
     127,     0,   127,     0,   127,   560,     0,   566,     0,   689,
       0,     0,   886,   717,   702,     0,   694,   786,     0,   564,
     776,     0,   570,   781,   555,     0,     0,   572,     0,   571,
       0,   574,     0,     0,     0,    80,     0,     0,   824,     0,
       0,     0,     0,   858,   861,   864,     0,   886,   292,   289,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   886,   274,     0,    81,    82,     0,     0,     0,     0,
      56,    57,     0,     0,   886,     0,     0,   886,     0,     0,
     297,   294,   339,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,     0,     0,     0,   844,   802,   810,
       0,   853,     0,     0,     0,   810,     0,     0,     0,     0,
       0,   813,   880,     0,   250,     0,    33,     0,    31,     0,
     887,     0,   247,     0,     0,   887,     0,     0,     0,     0,
     409,   406,   408,    61,     0,   122,     0,     0,   425,     0,
     801,     0,     0,     0,   313,   312,     0,     0,   124,   265,
       0,     0,   492,   491,     0,     0,   493,   497,     0,     0,
       0,   384,   393,   372,   394,   373,   396,   375,   395,   374,
     397,   376,   387,   366,   388,   367,   389,   368,   446,   447,
     459,   398,   377,   399,   378,   460,   457,   458,     0,   386,
     364,   486,     0,   477,     0,   510,   511,   512,   365,   448,
     449,   400,   379,   401,   380,   464,   465,   466,   390,   369,
     391,   370,   392,   371,   467,   385,   363,     0,     0,   462,
     463,   461,   455,   456,   451,   450,   452,   453,   454,     0,
       0,     0,   415,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,   540,   543,     0,     0,   588,   591,   362,
     592,   711,   734,   737,     0,   740,   730,     0,   696,   744,
       0,   751,     0,   758,     0,   764,     0,   767,     0,     0,
     257,   714,   252,   703,   559,   565,     0,   648,   649,   575,
     578,   577,     0,     0,     0,     0,   825,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     333,   372,   373,   375,   374,   376,   366,   367,   368,   377,
     378,   364,   379,   380,   369,   370,   371,   363,   298,     0,
       0,   881,   256,   482,     0,     0,     0,   483,     0,   514,
       0,     0,     0,     0,     0,     0,   347,   124,   516,   517,
     518,   519,   520,     0,     0,   811,     0,     0,     0,     0,
     332,   810,     0,     0,     0,     0,   819,   820,     0,   827,
       0,     0,   817,     0,   856,     0,     0,     0,     0,   815,
     857,     0,   847,   812,   876,     0,     0,     0,     0,   878,
     879,     0,     0,     0,   855,   472,     0,     0,     0,   124,
       0,    59,    60,     0,    74,    66,   347,     0,   426,     0,
       0,     0,   429,   427,   306,     0,     0,     0,     0,     0,
       0,     0,     0,   345,     0,   127,   488,     0,   494,     0,
     383,   381,   382,     0,     0,   475,     0,     0,   498,   502,
       0,     0,   478,     0,     0,     0,     0,   416,     0,   423,
       0,   473,   431,   608,   628,   123,   541,   542,   543,   544,
     535,   548,   347,   590,   712,   735,   738,   701,   741,   731,
     691,   697,   745,   747,   752,   754,   759,   761,   765,   561,
     768,   567,     0,     0,   647,     0,     0,   833,   836,     0,
     276,     0,   281,   282,   280,     0,   326,     0,     0,   329,
     324,     0,     0,     0,   869,   867,   255,     0,    83,   315,
     318,   321,   873,   871,   842,   851,   849,   839,     0,   127,
       0,     0,     0,   793,   792,   810,     0,   845,     0,     0,
     803,   826,   818,   846,   854,   816,     0,   810,     0,   822,
     823,   830,   814,     0,   260,   263,    32,     0,   222,     0,
       0,     0,     0,     0,   127,    62,     0,    67,     0,   124,
     428,     0,     0,     0,     0,   582,     0,   342,   343,     0,
     341,   340,     0,     0,     0,     0,     0,     0,     0,   404,
       0,     0,   499,     0,   487,     0,   476,     0,     0,   417,
       0,     0,   474,   424,   420,     0,   553,   538,   535,   536,
     537,   540,     0,   748,   755,   762,   251,   258,   650,   576,
       0,     0,    78,   277,   283,     0,     0,     0,     0,   325,
     859,   862,   865,     0,     0,     0,     0,     0,     0,     0,
     824,     0,     0,     0,   225,     0,   522,     0,     0,     0,
       0,   828,     0,     0,     0,     0,     0,   821,     0,     0,
     253,     0,     0,     0,   407,   531,   410,     0,    61,     0,
      76,   127,   402,     0,   307,   582,     0,     0,   349,     0,
     346,   348,     0,   334,   352,     0,   530,     0,   528,   405,
     525,     0,     0,     0,   524,     0,   418,     0,   421,   539,
     549,   538,     0,     0,   824,   824,     0,     0,     0,     0,
       0,     0,     0,   251,   882,   255,   316,   319,   322,     0,
     825,   843,   824,     0,   484,     0,   351,   523,   884,     0,
       0,   829,     0,   795,   794,   884,   831,   884,   261,   251,
     264,   884,   884,     0,     0,   413,    63,   287,     0,    68,
      72,    73,    70,    71,    69,     0,     0,     0,     0,     0,
       0,   344,     0,   335,   489,   495,     0,   529,   527,     0,
     526,     0,   551,   545,   716,   809,   809,   327,     0,     0,
     330,   824,   824,     0,     0,     0,   886,   809,   800,     0,
     485,     0,     0,   797,   796,     0,     0,     0,   886,     0,
       0,   884,   411,     0,     0,     0,   293,   354,   403,     0,
       0,     0,     0,   336,   490,   496,   500,   419,     0,     0,
       0,   806,   886,   808,     0,     0,     0,     0,   809,   809,
       0,   870,   883,     0,     0,   840,     0,     0,     0,     0,
       0,     0,   887,     0,   892,   888,     0,   414,   294,     0,
       0,     0,   311,   501,   550,     0,     0,   887,   807,   834,
     837,   328,   331,     0,     0,   866,   874,   852,   885,   890,
     799,   798,   891,   893,   254,   889,     0,   310,   309,   552,
     804,     0,   860,   863,   308,     0,   805
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1474, -1474, -1474, -1474, -1474, -1474,   651,  1277, -1474, -1474,
   -1474,  1357, -1474, -1474, -1474,   769,  1315, -1474,  1224, -1474,
   -1474,  1278, -1474, -1474, -1474,  -116, -1474, -1474, -1474,  -115,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,  1134,
   -1474, -1474,   -54,   -37, -1474, -1474, -1474,   480,   566,  -502,
    -575,  -790, -1474, -1474, -1474, -1309, -1474, -1474,  -213,  -352,
   -1474,   415, -1474, -1371, -1474, -1291,  -382,  -295, -1474, -1474,
   -1474, -1474,  -390,   -14, -1474, -1474, -1474, -1474, -1474,  -111,
    -110,  -109, -1474,  -107, -1474, -1474, -1474,  1370, -1474,   401,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474, -1474,   777,   -82,   542,   -15,   138,  -884,  -454,
   -1474,  -519, -1474, -1474,  -376,  1442, -1474, -1474, -1474, -1473,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,   560,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474,  -143,   -39,  -131,
     -36,   104, -1474, -1474, -1474, -1474, -1474,   952, -1474,  -588,
   -1474, -1474,  -584, -1474, -1474,  -618,  -121,  -573, -1340, -1474,
    -363, -1474, -1474,  1340, -1474, -1474, -1474,   838,   888,   115,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
    -608,  -189, -1474,   953, -1474, -1474, -1474, -1474, -1474, -1474,
    -411, -1474, -1474,  -384, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474,  -215, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474,   959,  -687,  -186,
    -827,  -664, -1474, -1474, -1281,  -872, -1474, -1474, -1474, -1160,
     -62, -1270, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,
   -1474,   220,  -482, -1474, -1474, -1474,   713, -1474, -1474, -1474,
   -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474,   916,
   -1474, -1234,  -699, -1474
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   146,    52,    17,   168,   174,   705,   510,
     152,   511,   100,    19,    20,    45,    46,    47,    89,    21,
      39,    40,   512,   513,  1223,  1224,   514,  1378,  1479,   515,
     516,   964,   517,   634,   518,   519,   520,   521,  1154,   175,
     176,    35,    36,    37,   224,    66,    67,    68,    69,    22,
     289,   379,   214,    23,   112,   215,   113,   159,   683,   991,
     523,   380,   524,   831,  1550,   890,  1099,   572,   943,  1469,
     945,  1470,   526,   527,   528,   636,   857,  1433,   529,   530,
     531,   532,   533,   534,   535,   536,   431,   537,   735,  1235,
     974,   538,   539,   896,  1446,   897,  1447,   899,  1448,   540,
     862,  1439,   541,   717,  1494,   542,  1243,  1244,   978,   685,
     543,   792,   965,   544,   650,   992,   546,   547,   548,   962,
     549,  1218,  1554,  1219,  1613,   550,  1065,  1413,   551,   718,
    1395,  1624,  1397,  1625,  1501,  1663,   553,   375,  1421,  1510,
    1278,  1280,  1075,   566,   801,  1581,  1629,   376,   377,   617,
     826,   424,   622,   828,   425,  1178,   628,   580,   395,   312,
     313,   221,   305,    79,   124,    25,   164,   381,    90,    91,
     178,    92,    26,    49,   116,   161,    27,   292,   563,   564,
    1071,   384,   219,   220,    77,   121,   386,    28,   162,   303,
     629,   554,   301,   358,   359,   818,   423,   360,   588,  1291,
    1303,   811,   421,   361,   581,  1284,   830,   586,  1289,   582,
    1285,   583,  1286,   585,  1288,   589,  1292,   590,  1423,   591,
    1294,   592,  1424,   593,  1296,   594,  1425,   595,  1298,   596,
    1300,   619,    29,    96,   181,   364,   620,    30,    97,   182,
     365,   624,    31,    95,   180,   363,   613,   555,  1631,  1599,
     971,   929,  1632,  1633,  1634,   930,   942,  1200,  1194,  1189,
    1361,  1119,   556,   853,  1430,   854,  1431,   908,  1452,   905,
    1450,   931,   707,   557,   906,  1451,   932,   558,  1125,  1520,
    1126,  1521,  1127,  1522,   866,  1443,   903,  1449,   701,   891,
     559,  1602,   951,   560
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    70,   291,   806,   545,   700,   726,   825,   928,   576,
     928,   852,   827,   618,   623,   820,   362,   822,   956,   824,
     957,   225,   686,   687,   567,  1351,   856,   133,   522,  1167,
    1090,  1118,  1092,   935,  1094,  1444,   921,   642,   610,     2,
     736,   970,   357,  -122,  1114,  1487,     3,  1174,    85,    70,
      70,    70,   222,   600,  1195,  1513,  1201,  1185,   648,    32,
      33,  1208,  1212,  1197,   800,   125,   126,   154,  1370,     4,
    1238,     5,    58,     6,   921,   976,   762,   763,    38,     7,
    1239,  1612,   678,    86,  1419,   525,   920,   847,   933,     8,
     937,    70,    70,    70,    70,     9,   522,   737,   949,    59,
      50,   695,   697,   436,   437,   432,   727,   953,    53,   223,
     105,   106,   107,  1240,    54,    48,   922,  1492,   561,    10,
     522,   577,   847,   447,    99,    78,   847,   602,  1227,   452,
     562,   578,  1241,  1102,   643,   644,  1177,  1242,   433,  1229,
    1657,    11,    12,   639,  1420,  1568,    51,   738,   848,   849,
     728,   850,  1594,   525,   851,   290,   429,   759,   790,   791,
     762,   763,   387,   434,    93,  1184,   466,   467,  1129,   307,
     411,   970,   838,  1228,   105,   411,   107,   525,  1608,   922,
    1531,   153,   913,   579,   849,   306,  1461,   422,   849,   357,
     807,  1227,  1152,   739,   740,   169,   170,   412,   413,   430,
     469,   470,   412,   413,   357,  1161,   839,   142,  1164,   847,
      34,   842,   848,   127,   601,   848,   798,   356,   128,  1177,
     129,    13,   848,   130,   848,    99,    87,   645,   522,   848,
     848,   977,   141,   357,  1595,   357,   848,    88,   652,    71,
      14,    58,   790,   791,  1585,  1586,   646,   799,    53,   308,
    1191,    15,   669,  1192,    54,   389,    82,   868,   485,   486,
     487,   414,  1597,   922,   131,   415,   414,    58,    59,   889,
     415,   849,  1628,    72,    73,  1227,    74,    80,   498,   970,
    1230,  1193,  1338,   979,   309,   525,    61,   910,   603,  1311,
     751,   752,   909,   635,    59,   522,   357,   357,   759,    81,
    1541,   762,   763,   764,    75,  1635,   604,   794,   765,  1346,
     506,  1606,   605,  1607,   795,    62,  1644,  1609,  1610,  1630,
    1665,  1638,  1639,   373,   615,   522,   184,   416,   222,   310,
     616,   417,   416,  1166,   418,    81,   417,   972,  1149,   418,
     108,   410,  1379,   171,   117,   118,  1213,   311,   172,   419,
     173,   373,   525,   130,   419,   420,   222,  1673,  1674,   229,
     420,  1186,  1187,   847,   356,   109,   809,   810,   812,   565,
     814,   815,   975,  1463,   819,    76,   821,  1656,   823,   356,
     847,    83,   525,   790,   791,   223,   230,   290,    63,  1188,
    1539,   973,   357,   357,   357,    58,   357,   357,  1422,   847,
     357,  1084,   357,  1245,   357,   290,   357,  1231,   356,  1648,
     356,   374,    81,   223,   870,  1096,  1078,  1098,  1076,   108,
     855,   615,    59,   508,   682,   849,   615,   616,   290,   970,
     356,   640,   616,  -746,  1232,  1095,   870,   388,  -746,   668,
    1097,  1272,   849,    98,  1275,   894,   525,   525,   525,   525,
     525,   525,   525,   525,   522,  1394,  -746,    64,  1204,   525,
     525,   849,  1110,  1226,   357,   525,    65,  1175,  1209,  1112,
    1113,   356,   356,  1460,   525,   525,  1183,    58,  1150,   525,
     525,   525,  1128,   525,   216,  1466,   958,   357,   598,   870,
     944,  1306,    99,  1383,   928,   217,  1392,  1350,  1417,   104,
     522,  1381,   739,   740,    59,  1383,   870,   599,  1299,   928,
     963,   525,  1155,  1156,  1301,  1158,  1312,  1345,  1279,  1160,
    1385,  1162,  1163,  1488,  1165,    41,    42,    43,    58,  1392,
    1384,   870,  1357,  1499,  1250,  1251,  1252,   958,   870,   870,
     611,   653,   959,   101,   102,   103,  1183,  1183,  1087,  1455,
     983,   987,   612,  1183,  1393,    59,    44,   525,  1506,  -753,
     654,  1205,  1088,  1206,  -753,  1508,  1537,   356,   356,   356,
    1183,   356,   356,  1690,  1696,   356,  1027,   356,  1566,   356,
     960,   356,  -753,   670,  1477,   155,   156,   157,   158,   739,
     740,  -760,  1339,   134,  1317,  1579,  -760,   749,   750,   751,
     752,  1066,   671,  1198,  1191,   673,  1199,   759,  1318,   761,
     762,   763,   764,   357,  -760,  1100,   621,   765,   860,   767,
     768,  1022,  -412,  1104,   674,   859,  1105,  -412,   615,    88,
     310,  1261,   615,  1023,   616,  1109,   626,   861,   616,   356,
     136,   357,  1115,  1262,  1374,  -412,  1302,  1124,   311,   357,
     110,   422,   357,   615,   627,  1081,   111,   615,  1074,   616,
    1266,   357,   356,   616,   357,   357,   422,  1364,   357,   139,
    1082,  1365,  1267,   357,   140,  1148,   922,   357,   785,   786,
     787,   788,   789,   143,   739,   740,   751,   752,  1227,  1220,
    1348,  1565,   790,   791,   759,  1483,   761,   762,   763,   764,
    1221,  1222,  1349,   615,   765,   114,   961,   357,   357,   616,
     357,   115,   615,   422,   357,   147,   357,   357,   616,   357,
     422,  1505,   151,   411,  1085,   148,  1435,   149,  1100,   392,
    1100,  1464,   393,  1358,   293,   394,  1359,   150,   294,  1360,
     119,  1321,  1322,  1323,   160,  1453,   120,   508,   682,   122,
     412,   413,   295,   296,   357,   123,   357,   297,   298,   299,
     300,    41,    42,    43,   144,   615,  1327,   787,   788,   789,
     145,   616,  1276,   165,   166,   946,   947,  1458,  1277,   790,
     791,   751,   752,   870,  1465,    70,   508,   682,   356,   759,
     870,   761,   762,   763,   764,  1468,  1471,  1472,    86,   765,
    1553,   870,   870,   870,  1481,   179,   870,   163,   422,   422,
     105,   422,  1086,  1089,   414,  1091,   356,   422,   415,  1454,
    1486,  1093,   422,   394,   356,   422,  1310,   356,   422,  1320,
    1567,   183,  1575,   394,  1373,   545,   356,   739,   740,   356,
     356,  1540,   218,   356,   231,   422,  1598,   232,   356,  1626,
     226,   227,   356,   105,   106,   107,   137,   138,  1259,   522,
     785,   786,   787,   788,   789,   165,   166,   167,   233,  1287,
     226,   227,   228,   234,   790,   791,   939,   940,   941,   286,
     416,   287,   356,   356,   417,   356,  1282,   418,   288,   356,
     290,   356,   356,   302,   356,   357,   366,  1643,    55,    56,
      57,   304,   419,   367,  1436,   368,  1598,   357,   420,  1653,
     369,   370,   371,   372,  1570,   378,   525,   382,   383,   385,
     390,   391,   399,   396,   397,   398,   400,   401,   402,   356,
     409,   356,   422,  1668,   751,   752,   357,   357,   357,   403,
     552,   404,   759,  1666,   761,   762,   763,   764,   405,   406,
     573,   407,   765,   408,   426,   427,   574,   428,   568,   584,
     575,   357,   587,   608,   657,   631,   632,   641,   659,   597,
     660,   661,   665,   662,   664,   663,   672,   675,   676,   607,
     688,   689,   739,   740,   704,   709,  1547,  1649,   625,  1551,
     720,   724,   690,   721,   691,  1620,  1621,   722,   633,   692,
     734,   693,   729,   730,  1695,   731,   732,  1375,   651,   733,
     793,  1495,    15,   656,   796,   658,   668,   684,   684,   684,
     739,   740,   804,   802,   667,   702,   817,   790,   791,   357,
     621,   836,   679,   680,   681,   837,   840,   816,   833,   841,
     843,   846,   863,   845,   737,   858,  1659,   699,   411,   870,
     888,   895,   864,   865,   900,   703,   901,   912,   699,   950,
     952,   710,   711,   712,   713,   714,   954,   725,  1018,   955,
     356,  1073,  1077,  1083,   723,   412,   413,   749,   750,   751,
     752,   753,   356,  1101,   756,   757,   758,   759,  1427,   761,
     762,   763,   764,  1107,   725,  1108,  1432,   765,  1122,   767,
     768,  1151,  1569,  1183,  1176,   771,   772,   773,  1190,  1214,
    1203,   356,   356,   356,   357,   749,   750,   751,   752,   753,
    1210,  1215,   756,   757,   758,   759,  1233,   761,   762,   763,
     764,  1216,  1217,  1234,  1269,   765,   356,   767,   768,   414,
     808,  1237,  1246,   415,  1247,  1248,  1249,  1253,  1274,  1254,
    1264,   779,  1265,   780,   781,   782,   783,   784,   785,   786,
     787,   788,   789,  1478,  1279,  1270,  1281,  1290,   835,  1293,
    1295,  1297,   790,   791,  1305,  1313,  1315,  1314,  1352,  1325,
    1326,  1493,  1497,  1619,  1333,  1336,  1340,  1387,  1388,  1355,
    1362,  1390,  1389,   725,   783,   784,   785,   786,   787,   788,
     789,  1380,  1391,  1402,   356,   416,   357,   651,   357,   417,
     790,   791,   418,  1416,   699,   892,  1403,   893,  1405,  1411,
    1428,   725,   898,  1426,  1437,   649,  1434,   419,   902,  1480,
    1438,  1484,  1467,   420,  1462,  1485,  1489,   911,  1482,  1491,
    1509,  1453,  1493,  1514,  1515,  1517,  1530,  1518,   914,   915,
     916,   917,   918,   919,  1523,   927,  1524,   927,  1525,  1529,
    1532,  1555,  1533,  1549,  1588,  1601,  1646,  1571,  1650,  1647,
    1584,  1589,  1591,  1592,  1593,  1651,  1652,  1618,  1664,   684,
    1654,  1667,  1627,  1683,   719,  1641,  1576,  1642,  1645,   356,
    1655,   739,   740,  1669,  1670,  1675,  1676,   357,  1679,  1677,
    1691,   993,   995,   997,   999,  1001,  1003,  1005,  1007,  1008,
    1009,  1682,   357,  1010,  1012,  1014,  1015,  1016,  1017,  1684,
    1020,  1021,  1685,  1024,  1025,  1026,  1028,  1029,  1030,  1032,
    1034,  1035,  1036,  1037,  1039,  1041,  1043,  1044,  1046,  1048,
    1049,  1050,  1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,
     725,  1067,  1617,  1692,  1693,  1070,   948,   132,    18,    84,
     177,   135,  1556,   314,  1559,  1072,  1079,  1211,  1560,  1561,
    1562,    24,  1563,  1536,  1236,  1386,   832,  1490,   357,  1511,
    1582,   356,  1418,   356,   630,  1512,   749,   750,   751,   752,
     753,  1583,    94,   756,   757,   758,   759,   637,   761,   762,
     763,   764,   797,   638,  1546,  1106,   765,  1640,   767,   768,
     936,  1354,  1111,     0,   708,     0,     0,     0,     0,  1121,
       0,  1123,     0,   867,   869,     0,     0,     0,     0,     0,
    1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,
    1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,   904,     0,
       0,   907,  1689,   725,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   684,   782,   783,   784,   785,   786,   787,
     788,   789,   356,     0,     0,     0,     0,     0,     0,     0,
       0,   790,   791,     0,   710,  1180,     0,   356,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1202,     0,
     699,     0,     0,     0,     0,   969,     0,  1207,     0,     0,
     699,     0,     0,     0,     0,  1130,     0,     0,     0,     0,
       0,     0,     0,     0,  1225,     0,     0,     0,     0,     0,
     684,   684,   684,     0,     0,   725,     0,   725,     0,   725,
     961,   725,     0,   725,     0,   725,     0,   725,     0,   725,
       0,     0,     0,   356,   725,     0,   725,     0,     0,     0,
       0,     0,   725,     0,     0,     0,     0,     0,     0,     0,
    1069,     0,     0,     0,   725,     0,   725,     0,     0,     0,
       0,   725,     0,   725,     0,   725,     0,     0,   725,     0,
       0,     0,     0,     0,     0,     0,     0,   694,     0,   961,
     411,     0,   411,   315,     0,     0,     0,     0,     0,   316,
       0,     0,     0,     0,     0,   317,     0,     0,     0,     0,
    1263,   725,     0,     0,  1268,   318,     0,   412,   413,   412,
     413,     0,     0,   319,     0,  1117,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   320,     0,
       0,     0,     0,   725,     0,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,     0,     0,
       0,   414,     0,   414,   577,   415,   577,   415,     0,     0,
       0,     0,     0,   719,   578,     0,   578,     0,     0,     0,
       0,   719,     0,     0,  1328,     0,     0,     0,     0,     0,
     411,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,   -69,     0,     0,  1342,  1343,  1344,     0,
     354,     0,  1347,     0,   739,   740,     0,   412,   413,   805,
    1353,   927,     0,     0,    59,     0,   579,   416,   579,   416,
       0,   417,     0,   417,   418,     0,   418,     0,     0,     0,
       0,   552,     0,     0,     0,     0,  1371,  1372,     0,   419,
       0,   419,     0,  1376,     0,   420,     0,   420,     0,     0,
       0,  1130,     0,     0,     0,     0,     0,     0,   411,     0,
     355,     0,     0,     0,     0,     0,     0,  1396,     0,  1398,
       0,   414,     0,     0,  1401,   415,     0,     0,     0,     0,
    1404,     0,     0,     0,  1407,   412,   413,     0,     0,     0,
       0,   741,   742,   743,   744,   745,   746,   747,   748,   749,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   759,
     760,   761,   762,   763,   764,     0,     0,     0,     0,   765,
     766,   767,   768,   769,   770,     0,  1429,   771,   772,   773,
     774,   775,   776,   777,     0,     0,     0,   416,     0,     0,
     725,   417,     0,  1307,   418,     0,   699,     0,     0,   414,
       0,  1309,     0,   415,     0,     0,     0,     0,     0,   419,
       0,     0,     0,     0,     0,   420,     0,     0,     0,     0,
       0,     0,   778,   779,     0,   780,   781,   782,   783,   784,
     785,   786,   787,   788,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   790,   791,     0,     0,     0,   508,
     682,     0,     0,     0,     0,     0,   411,     0,     0,     0,
       0,  1341,     0,     0,     0,   416,     0,  1377,     0,   417,
       0,  1308,   418,  1502,     0,  1503,   739,   740,     0,     0,
       0,  1507,  1356,   412,   413,     0,     0,   419,  1363,     0,
       0,     0,     0,   420,     0,  1367,     0,     0,     0,  1369,
       0,     0,     0,     0,     0,  1516,     0,     0,  1519,     0,
       0,     0,     0,     0,     0,     0,  1526,  1527,  1528,     0,
       0,     0,     0,  1535,     0,     0,     0,     0,  1538,     0,
       0,     0,  1542,  1543,  1544,  1545,     0,     0,   699,  1548,
    1400,   699,  1552,     0,     0,     0,     0,   414,     0,  1564,
       0,   415,  1410,     0,     0,     0,     0,  1415,     0,     0,
       0,     0,  1573,     0,     0,     0,     0,     0,     0,     0,
       0,   749,   750,   751,   752,   753,     0,     0,   756,   757,
     758,   759,     0,   761,   762,   763,   764,     0,     0,     0,
       0,   765,     0,   767,   768,   699,     0,     0,     0,   771,
     772,   773,     0,     0,     0,   777,     0,     0,     0,  1603,
    1604,     0,     0,   416,     0,     0,     0,   417,     0,  1324,
     418,     0,     0,  1611,     0,     0,     0,  1457,  1616,     0,
       0,     0,     0,     0,     0,   419,     0,     0,     0,     0,
    1622,   420,  1623,     0,     0,   779,     0,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   789,  1473,  1636,  1637,
       0,     0,     0,     0,     0,     0,   790,   791,     0,     0,
       0,     0,     0,     0,   696,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,  1658,   316,     0,     0,     0,
    1660,  1661,   317,     0,   994,   996,   998,  1000,  1002,  1004,
    1006,     0,   318,     0,     0,     0,  1011,  1013,     0,     0,
     319,     0,  1019,   832,     0,     0,  1678,     0,  1680,  1681,
       0,  1031,  1033,     0,     0,   320,  1038,  1040,  1042,  1686,
    1045,     0,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,     0,     0,     0,     0,     0,
       0,   435,     0,     0,     0,   436,   437,     3,     0,   438,
     439,   440,     0,   441,     0,   442,   443,   444,   445,   446,
       0,     0,     0,     0,     0,   447,   448,   449,   450,   451,
       0,   452,     0,     0,    58,     0,     0,     0,   453,   454,
     832,     0,   455,     0,   456,   457,  1596,   354,   458,     0,
       8,   459,   460,     0,   461,   462,     0,     0,   463,   464,
       0,    59,     0,     0,     0,   465,   832,     0,   466,   467,
       0,   321,   322,   323,   649,   325,   326,   327,   328,   329,
     468,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,     0,   343,   344,   345,     0,     0,   348,   349,
     350,   351,   469,   470,   471,   472,     0,   355,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   473,   474,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   869,     0,   475,   476,   477,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   478,   479,   480,   481,   482,     0,   483,     0,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
      59,   495,   496,   497,     0,     0,     0,     0,     0,   411,
     498,   499,   500,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   501,   502,   503,
       0,    14,     0,     0,   504,   505,   412,   413,     0,     0,
       0,     0,   506,     0,   507,   435,   508,   509,     0,   436,
     437,     3,     0,   438,   439,   440,     0,   441,     0,   442,
     443,   444,   445,   446,     0,     0,     0,     0,     0,   447,
     448,   449,   450,   451,     0,   452,     0,     0,     0,     0,
       0,     0,   453,   454,     0,     0,   455,     0,   456,   457,
       0,     0,   458,     0,     8,   459,   460,     0,   461,   462,
     414,     0,   463,   464,   415,     0,     0,     0,     0,   465,
       0,     0,   466,   467,     0,   321,   322,   323,     0,   325,
     326,   327,   328,   329,   468,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,     0,   343,   344,   345,
       0,     0,   348,   349,   350,   351,   469,   470,   471,   472,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,   474,     0,     0,   416,     0,     0,     0,
     417,     0,  1329,   418,     0,     0,     0,   475,   476,   477,
       0,     0,     0,     0,     0,     0,     0,    58,   419,     0,
       0,     0,     0,     0,   420,   478,   479,   480,   481,   482,
       0,   483,     0,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,    59,   495,   496,   497,     0,     0,
       0,     0,     0,     0,   498,  1368,   500,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   501,   502,   503,     0,    14,     0,     0,   504,   505,
       0,     0,   436,   437,     0,     0,   506,     0,   507,     0,
     508,   509,   442,   443,   444,   445,   446,     0,     0,     0,
       0,     0,   447,     0,   449,     0,     0,     0,   452,     0,
     411,     0,     0,     0,     0,     0,   454,     0,     0,     0,
       0,     0,   457,     0,     0,   458,     0,     0,   459,     0,
     921,     0,   462,     0,     0,     0,     0,   412,   413,     0,
       0,     0,   569,     0,     0,   466,   467,     0,   321,   322,
     323,     0,   325,   326,   327,   328,   329,   468,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,     0,
     343,   344,   345,     0,     0,   348,   349,   350,   351,   469,
     470,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   473,   474,     0,     0,     0,
       0,   414,     0,     0,     0,   415,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   478,   479,
     480,   481,   482,   411,   483,   922,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   923,   571,   496,
     497,     0,     0,     0,     0,     0,     0,   498,     0,     0,
     412,   413,     0,     0,     0,     0,     0,   416,     0,     0,
       0,   417,     0,     0,   924,   502,   503,     0,    14,     0,
       0,   504,   505,     0,     0,     0,   436,   437,     0,   925,
       0,   926,     0,   508,   509,   420,   442,   443,   444,   445,
     446,     0,     0,     0,     0,     0,   447,     0,   449,     0,
       0,     0,   452,     0,   411,     0,     0,     0,     0,     0,
     454,     0,     0,     0,   414,     0,   457,     0,   415,   458,
       0,     0,   459,     0,     0,     0,   462,     0,     0,     0,
       0,   412,   413,     0,     0,     0,   569,     0,     0,   466,
     467,     0,   321,   322,   323,     0,   325,   326,   327,   328,
     329,   468,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,     0,   343,   344,   345,     0,     0,   348,
     349,   350,   351,   469,   470,   570,     0,     0,     0,     0,
     416,     0,     0,     0,   417,     0,  1330,   418,     0,   473,
     474,     0,     0,     0,     0,   414,     0,     0,     0,   415,
       0,     0,   419,     0,     0,     0,     0,     0,   420,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   478,   479,   480,   481,   482,   411,   483,   922,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   923,   571,   496,   497,     0,   411,     0,     0,     0,
       0,   498,     0,     0,   412,   413,     0,     0,     0,     0,
       0,   416,     0,     0,     0,   417,     0,     0,   924,   502,
     503,     0,    14,   412,   413,   504,   505,     0,     0,     0,
     436,   437,     0,   925,     0,   934,     0,   508,   509,   420,
     442,   443,   444,   445,   446,     0,     0,     0,     0,     0,
     447,     0,   449,     0,     0,     0,   452,     0,   602,     0,
       0,     0,     0,     0,   454,     0,     0,     0,   414,     0,
     457,     0,   415,   458,     0,     0,   459,     0,     0,     0,
     462,     0,     0,     0,     0,     0,     0,   414,     0,     0,
     569,   415,     0,   466,   467,     0,   321,   322,   323,     0,
     325,   326,   327,   328,   329,   468,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,     0,   343,   344,
     345,     0,     0,   348,   349,   350,   351,   469,   470,   570,
       0,     0,     0,     0,   416,     0,     0,     0,   417,     0,
    1331,   418,     0,   473,   474,     0,     0,     0,     0,     0,
       0,     0,     0,   416,     0,     0,   419,   417,     0,  1332,
     418,     0,   420,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,   419,   478,   479,   480,   481,
     482,   420,   483,     0,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,    59,   571,   496,   497,     0,
       0,     0,     0,     0,     0,   498,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   603,
       0,     0,   501,   502,   503,     0,    14,     0,     0,   504,
     505,     0,     0,     0,   436,   437,     0,  1179,     0,   507,
       0,   508,   509,   605,   442,   443,   444,   445,   446,     0,
       0,     0,     0,     0,   447,     0,   449,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,   454,     0,
       0,     0,     0,     0,   457,     0,     0,   458,     0,     0,
     459,     0,     0,     0,   462,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   569,     0,     0,   466,   467,     0,
     321,   322,   323,     0,   325,   326,   327,   328,   329,   468,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,     0,   343,   344,   345,     0,     0,   348,   349,   350,
     351,   469,   470,   471,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,   474,     0,
       0,     0,     0,     0,     0,     0,   647,     0,     0,     0,
       0,     0,   475,   476,   477,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     478,   479,   480,   481,   482,     0,   483,     0,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,    59,
     571,   496,   497,     0,     0,     0,     0,     0,     0,   498,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   501,   502,   503,     0,
      14,     0,     0,   504,   505,     0,     0,   436,   437,     0,
       0,   506,     0,   507,     0,   508,   509,   442,   443,   444,
     445,   446,     0,     0,     0,     0,     0,   447,     0,   449,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,   454,     0,     0,     0,     0,     0,   457,     0,     0,
     458,     0,     0,   459,     0,     0,     0,   462,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   569,     0,     0,
     466,   467,     0,   321,   322,   323,     0,   325,   326,   327,
     328,   329,   468,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,     0,   343,   344,   345,     0,     0,
     348,   349,   350,   351,   469,   470,   471,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,   474,     0,     0,     0,     0,     0,     0,     0,   677,
       0,     0,     0,     0,     0,   475,   476,   477,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   478,   479,   480,   481,   482,     0,   483,
       0,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,    59,   571,   496,   497,     0,     0,     0,     0,
       0,     0,   498,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   501,
     502,   503,     0,    14,     0,     0,   504,   505,     0,     0,
     436,   437,     0,     0,   506,     0,   507,     0,   508,   509,
     442,   443,   444,   445,   446,     0,     0,     0,     0,     0,
     447,  1557,   449,   450,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,   454,     0,     0,     0,     0,     0,
     457,     0,     0,   458,     0,     0,   459,   460,     0,     0,
     462,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     569,     0,     0,   466,   467,     0,   321,   322,   323,     0,
     325,   326,   327,   328,   329,   468,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,     0,   343,   344,
     345,     0,     0,   348,   349,   350,   351,   469,   470,   570,
    1558,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,   474,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   478,   479,   480,   481,
     482,     0,   483,     0,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,    59,   571,   496,   497,     0,
       0,     0,     0,     0,     0,   498,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   501,   502,   503,     0,    14,     0,     0,   504,
     505,     0,     0,   436,   437,     0,     0,   506,     0,   507,
       0,   508,   509,   442,   443,   444,   445,   446,     0,     0,
       0,     0,     0,   447,     0,   449,     0,     0,     0,   452,
       0,     0,     0,     0,     0,     0,     0,   454,     0,     0,
       0,     0,     0,   457,     0,     0,   458,     0,     0,   459,
       0,     0,     0,   462,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   569,     0,     0,   466,   467,     0,   321,
     322,   323,     0,   325,   326,   327,   328,   329,   468,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
       0,   343,   344,   345,     0,     0,   348,   349,   350,   351,
     469,   470,   471,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,   474,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   475,   476,   477,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   478,
     479,   480,   481,   482,     0,   483,     0,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,    59,   571,
     496,   497,     0,     0,     0,     0,     0,     0,   498,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   501,   502,   503,     0,    14,
       0,     0,   504,   505,     0,     0,   436,   437,     0,     0,
     506,     0,   507,     0,   508,   509,   442,   443,   444,   445,
     446,     0,     0,     0,     0,     0,   447,     0,   449,     0,
       0,     0,   452,     0,     0,     0,     0,     0,     0,     0,
     454,     0,     0,     0,     0,     0,   457,     0,     0,   458,
       0,     0,   459,     0,     0,     0,   462,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   569,     0,     0,   466,
     467,   966,   321,   322,   323,     0,   325,   326,   327,   328,
     329,   468,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,     0,   343,   344,   345,     0,     0,   348,
     349,   350,   351,   469,   470,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   473,
     474,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   478,   479,   480,   481,   482,     0,   483,   922,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   923,   571,   496,   497,     0,     0,     0,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   501,   502,
     503,     0,    14,     0,     0,   504,   505,     0,     0,   436,
     437,     0,     0,   967,     0,   507,   968,   508,   509,   442,
     443,   444,   445,   446,     0,     0,     0,     0,     0,   447,
       0,   449,     0,     0,   411,   452,     0,     0,     0,     0,
       0,     0,     0,   454,     0,     0,     0,     0,     0,   457,
       0,     0,   458,     0,     0,   459,     0,     0,     0,   462,
       0,   412,   413,     0,     0,     0,     0,     0,     0,   569,
       0,     0,   466,   467,     0,   321,   322,   323,     0,   325,
     326,   327,   328,   329,   468,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,     0,   343,   344,   345,
       0,     0,   348,   349,   350,   351,   469,   470,   471,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,   474,     0,   414,     0,     0,     0,   415,
       0,     0,     0,     0,     0,     0,     0,   988,   989,   990,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   478,   479,   480,   481,   482,
     411,   483,     0,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,    59,   571,   496,   497,     0,     0,
       0,     0,     0,     0,   498,     0,     0,   412,   413,     0,
       0,   416,     0,     0,     0,   417,     0,  1334,   418,     0,
       0,   501,   502,   503,     0,    14,     0,     0,   504,   505,
       0,     0,     0,   419,   436,   437,   506,     0,   507,   420,
     508,   509,   715,     0,   442,   443,   444,   445,   446,     0,
       0,     0,     0,     0,   447,     0,   449,     0,     0,   411,
     452,     0,     0,     0,     0,     0,     0,     0,   454,     0,
       0,   414,     0,     0,   457,   415,     0,   458,   716,     0,
     459,     0,     0,     0,   462,     0,   412,   413,     0,     0,
       0,     0,     0,     0,   569,     0,     0,   466,   467,     0,
     321,   322,   323,     0,   325,   326,   327,   328,   329,   468,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,     0,   343,   344,   345,     0,     0,   348,   349,   350,
     351,   469,   470,   570,     0,     0,     0,   416,     0,     0,
       0,   417,     0,  1335,   418,     0,     0,   473,   474,     0,
     414,     0,     0,     0,   415,     0,     0,     0,     0,   419,
       0,     0,     0,     0,     0,   420,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     478,   479,   480,   481,   482,   411,   483,     0,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,    59,
     571,   496,   497,     0,     0,     0,     0,     0,     0,   498,
       0,     0,   412,   413,     0,     0,   416,     0,     0,     0,
     417,     0,  1337,   418,     0,     0,   501,   502,   503,     0,
      14,     0,     0,   504,   505,     0,     0,     0,   419,   436,
     437,   506,   606,   507,   420,   508,   509,   715,     0,   442,
     443,   444,   445,   446,     0,     0,     0,     0,     0,   447,
       0,   449,     0,     0,   411,   452,     0,     0,     0,     0,
       0,     0,     0,   454,     0,     0,   414,     0,     0,   457,
     415,     0,   458,   716,     0,   459,     0,     0,     0,   462,
       0,   412,   413,     0,     0,     0,     0,     0,     0,   569,
       0,     0,   466,   467,     0,   321,   322,   323,     0,   325,
     326,   327,   328,   329,   468,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,     0,   343,   344,   345,
       0,     0,   348,   349,   350,   351,   469,   470,   570,     0,
       0,     0,   416,     0,     0,     0,   417,     0,  1440,   418,
       0,     0,   473,   474,     0,   414,     0,     0,     0,   415,
       0,     0,     0,     0,   419,     0,     0,     0,     0,     0,
     420,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   478,   479,   480,   481,   482,
     411,   483,   922,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   923,   571,   496,   497,     0,     0,
       0,     0,     0,     0,   498,     0,     0,   412,   413,     0,
       0,   416,     0,     0,     0,   417,     0,  1441,   418,     0,
       0,   501,   502,   503,     0,    14,     0,     0,   504,   505,
       0,     0,     0,   419,   436,   437,   506,     0,   507,   420,
     508,   509,   715,     0,   442,   443,   444,   445,   446,     0,
       0,     0,     0,     0,   447,     0,   449,     0,     0,   411,
     452,     0,     0,     0,     0,     0,     0,     0,   454,     0,
       0,   414,     0,     0,   457,   415,     0,   458,   716,     0,
     459,     0,     0,     0,   462,     0,   412,   413,     0,     0,
       0,     0,     0,     0,   569,     0,     0,   466,   467,     0,
     321,   322,   323,     0,   325,   326,   327,   328,   329,   468,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,     0,   343,   344,   345,     0,     0,   348,   349,   350,
     351,   469,   470,   570,     0,     0,     0,   416,     0,     0,
       0,   417,     0,  1442,   418,     0,     0,   473,   474,     0,
     414,     0,     0,     0,   415,     0,     0,     0,     0,   419,
       0,     0,     0,     0,     0,   420,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     478,   479,   480,   481,   482,   411,   483,     0,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,    59,
     571,   496,   497,     0,     0,     0,     0,     0,     0,   498,
       0,     0,   412,   413,     0,     0,   416,     0,     0,     0,
     417,     0,  1445,   418,     0,     0,   501,   502,   503,     0,
      14,     0,     0,   504,   505,     0,     0,     0,   419,   436,
     437,   506,   833,   507,   420,   508,   509,   715,     0,   442,
     443,   444,   445,   446,     0,     0,     0,     0,     0,   447,
       0,   449,     0,     0,     0,   452,     0,     0,     0,     0,
       0,     0,     0,   454,     0,     0,   414,     0,     0,   457,
     415,     0,   458,   716,     0,   459,     0,     0,     0,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   569,
       0,     0,   466,   467,     0,   321,   322,   323,     0,   325,
     326,   327,   328,   329,   468,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,     0,   343,   344,   345,
       0,     0,   348,   349,   350,   351,   469,   470,   570,     0,
       0,     0,   416,     0,     0,     0,   417,     0,  1476,   418,
       0,     0,   473,   474,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   419,     0,     0,     0,     0,     0,
     420,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   478,   479,   480,   481,   482,
       0,   483,     0,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,    59,   571,   496,   497,     0,     0,
       0,     0,     0,     0,   498,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   501,   502,   503,     0,    14,     0,     0,   504,   505,
       0,     0,   436,   437,     0,     0,   506,     0,   507,     0,
     508,   509,   442,   443,   444,   445,   446,     0,     0,     0,
       0,     0,   447,     0,   449,     0,     0,     0,   452,     0,
       0,     0,     0,     0,     0,     0,   454,     0,     0,     0,
       0,     0,   457,     0,     0,   458,     0,     0,   459,     0,
       0,     0,   462,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   569,     0,     0,   466,   467,  1116,   321,   322,
     323,     0,   325,   326,   327,   328,   329,   468,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,     0,
     343,   344,   345,     0,     0,   348,   349,   350,   351,   469,
     470,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   473,   474,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   478,   479,
     480,   481,   482,     0,   483,   922,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   923,   571,   496,
     497,     0,     0,     0,     0,     0,     0,   498,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   501,   502,   503,     0,    14,     0,
       0,   504,   505,     0,     0,   436,   437,     0,     0,   506,
       0,   507,     0,   508,   509,   442,   443,   444,   445,   446,
       0,     0,     0,     0,     0,   447,     0,   449,     0,     0,
       0,   452,     0,     0,     0,     0,     0,     0,     0,   454,
       0,     0,     0,     0,     0,   457,     0,     0,   458,     0,
       0,   459,     0,     0,     0,   462,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   569,     0,     0,   466,   467,
       0,   321,   322,   323,     0,   325,   326,   327,   328,   329,
     468,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,     0,   343,   344,   345,     0,     0,   348,   349,
     350,   351,   469,   470,   570,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   473,   474,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   478,   479,   480,   481,   482,     0,   483,     0,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
      59,   571,   496,   497,     0,     0,     0,     0,     0,     0,
     498,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   501,   502,   503,
       0,    14,     0,     0,   504,   505,     0,     0,   436,   437,
       0,     0,   506,   606,   507,     0,   508,   509,   442,   443,
     444,   445,   446,     0,     0,     0,     0,     0,   447,     0,
     449,     0,     0,     0,   452,     0,     0,     0,     0,     0,
       0,     0,   454,     0,     0,     0,     0,     0,   457,     0,
       0,   458,     0,     0,   459,     0,     0,     0,   462,     0,
       0,     0,     0,     0,   655,     0,     0,     0,   569,     0,
       0,   466,   467,     0,   321,   322,   323,     0,   325,   326,
     327,   328,   329,   468,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,     0,   343,   344,   345,     0,
       0,   348,   349,   350,   351,   469,   470,   570,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   473,   474,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   478,   479,   480,   481,   482,     0,
     483,     0,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,    59,   571,   496,   497,     0,     0,     0,
       0,     0,     0,   498,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     501,   502,   503,     0,    14,     0,     0,   504,   505,     0,
       0,   436,   437,     0,     0,   506,     0,   507,     0,   508,
     509,   442,   443,   444,   445,   446,     0,     0,     0,     0,
       0,   447,     0,   449,     0,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,   454,     0,     0,     0,     0,
       0,   457,     0,     0,   458,     0,     0,   459,     0,     0,
       0,   462,     0,     0,   666,     0,     0,     0,     0,     0,
       0,   569,     0,     0,   466,   467,     0,   321,   322,   323,
       0,   325,   326,   327,   328,   329,   468,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,     0,   343,
     344,   345,     0,     0,   348,   349,   350,   351,   469,   470,
     570,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,   474,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   478,   479,   480,
     481,   482,   411,   483,     0,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,    59,   571,   496,   497,
       0,     0,     0,     0,     0,     0,   498,     0,     0,   412,
     413,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   501,   502,   503,     0,    14,     0,     0,
     504,   505,     0,     0,     0,     0,   436,   437,   506,     0,
     507,     0,   508,   509,   698,     0,   442,   443,   444,   445,
     446,     0,     0,     0,     0,     0,   447,     0,   449,     0,
       0,     0,   452,     0,     0,     0,     0,     0,     0,     0,
     454,     0,     0,   414,     0,     0,   457,   415,     0,   458,
       0,     0,   459,     0,     0,     0,   462,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   569,     0,     0,   466,
     467,     0,   321,   322,   323,     0,   325,   326,   327,   328,
     329,   468,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,     0,   343,   344,   345,     0,     0,   348,
     349,   350,   351,   469,   470,   570,     0,     0,     0,   416,
       0,     0,     0,   417,     0,  1574,   418,     0,     0,   473,
     474,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   419,     0,     0,     0,     0,     0,   420,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   478,   479,   480,   481,   482,     0,   483,     0,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,    59,   571,   496,   497,     0,     0,     0,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   501,   502,
     503,     0,    14,     0,     0,   504,   505,     0,     0,   436,
     437,     0,     0,   506,     0,   507,     0,   508,   509,   442,
     443,   444,   445,   446,     0,     0,     0,     0,     0,   447,
       0,   449,     0,     0,     0,   452,     0,     0,     0,     0,
       0,     0,     0,   454,     0,     0,     0,     0,     0,   457,
       0,     0,   458,     0,     0,   459,     0,     0,     0,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   569,
       0,     0,   466,   467,     0,   321,   322,   323,     0,   325,
     326,   327,   328,   329,   468,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,     0,   343,   344,   345,
       0,     0,   348,   349,   350,   351,   469,   470,   570,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,   474,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   478,   479,   480,   481,   482,
       0,   483,     0,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,    59,   571,   496,   497,     0,     0,
       0,     0,     0,     0,   498,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   702,
       0,   501,   502,   503,     0,    14,     0,     0,   504,   505,
       0,     0,     0,     0,   436,   437,   506,     0,   507,     0,
     508,   509,   706,     0,   442,   443,   444,   445,   446,     0,
       0,     0,     0,     0,   447,     0,   449,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,   454,     0,
       0,     0,     0,     0,   457,     0,     0,   458,     0,     0,
     459,     0,     0,     0,   462,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   569,     0,     0,   466,   467,     0,
     321,   322,   323,     0,   325,   326,   327,   328,   329,   468,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,     0,   343,   344,   345,     0,     0,   348,   349,   350,
     351,   469,   470,   570,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,   474,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     478,   479,   480,   481,   482,     0,   483,     0,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,    59,
     571,   496,   497,     0,     0,     0,     0,     0,     0,   498,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   501,   502,   503,     0,
      14,     0,     0,   504,   505,     0,     0,   436,   437,     0,
       0,   506,     0,   507,     0,   508,   509,   442,   443,   444,
     445,   446,     0,     0,  1047,     0,     0,   447,     0,   449,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,   454,     0,     0,     0,     0,     0,   457,     0,     0,
     458,     0,     0,   459,     0,     0,     0,   462,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   569,     0,     0,
     466,   467,     0,   321,   322,   323,     0,   325,   326,   327,
     328,   329,   468,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,     0,   343,   344,   345,     0,     0,
     348,   349,   350,   351,   469,   470,   570,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,   474,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   478,   479,   480,   481,   482,     0,   483,
       0,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,    59,   571,   496,   497,     0,     0,     0,     0,
       0,     0,   498,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   501,
     502,   503,     0,    14,     0,     0,   504,   505,     0,     0,
     436,   437,     0,     0,   506,     0,   507,     0,   508,   509,
     442,   443,   444,   445,   446,     0,     0,     0,     0,     0,
     447,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,   454,     0,     0,     0,     0,     0,
     457,     0,     0,   458,     0,     0,   459,     0,     0,     0,
     462,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     569,     0,     0,   466,   467,     0,   321,   322,   323,     0,
     325,   326,   327,   328,   329,   468,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,     0,   343,   344,
     345,     0,     0,   348,   349,   350,   351,   469,   470,   570,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,   474,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   478,   479,   480,   481,
     482,     0,   483,     0,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,    59,   571,   496,   497,     0,
       0,     0,     0,     0,     0,   498,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   501,   502,   503,     0,    14,     0,     0,   504,
     505,     0,     0,   436,   437,     0,     0,   506,     0,   507,
    1068,   508,   509,   442,   443,   444,   445,   446,     0,     0,
       0,     0,     0,   447,     0,   449,     0,     0,     0,   452,
       0,     0,     0,     0,     0,     0,     0,   454,     0,     0,
       0,     0,     0,   457,     0,     0,   458,     0,     0,   459,
       0,     0,     0,   462,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   569,     0,     0,   466,   467,     0,   321,
     322,   323,     0,   325,   326,   327,   328,   329,   468,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
       0,   343,   344,   345,     0,     0,   348,   349,   350,   351,
     469,   470,   570,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,   474,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   478,
     479,   480,   481,   482,     0,   483,     0,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,    59,   571,
     496,   497,     0,     0,     0,     0,     0,     0,   498,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1120,     0,   501,   502,   503,     0,    14,
       0,     0,   504,   505,     0,     0,   436,   437,     0,     0,
     506,     0,   507,     0,   508,   509,   442,   443,   444,   445,
     446,     0,     0,     0,     0,     0,   447,     0,   449,     0,
       0,     0,   452,     0,     0,     0,     0,     0,     0,     0,
     454,     0,     0,     0,     0,     0,   457,     0,     0,   458,
       0,     0,   459,     0,     0,     0,   462,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   569,     0,     0,   466,
     467,     0,   321,   322,   323,     0,   325,   326,   327,   328,
     329,   468,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,     0,   343,   344,   345,     0,     0,   348,
     349,   350,   351,   469,   470,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   473,
     474,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   478,   479,   480,   481,   482,     0,   483,     0,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,    59,   571,   496,   497,     0,     0,     0,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   501,   502,
     503,     0,    14,     0,     0,   504,   505,     0,     0,   436,
     437,     0,     0,   506,     0,   507,  1181,   508,   509,   442,
     443,   444,   445,   446,     0,     0,     0,     0,     0,   447,
       0,   449,     0,     0,     0,   452,     0,     0,     0,     0,
       0,     0,     0,   454,     0,     0,     0,     0,     0,   457,
       0,     0,   458,     0,     0,   459,     0,     0,     0,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   569,
       0,     0,   466,   467,     0,   321,   322,   323,     0,   325,
     326,   327,   328,   329,   468,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,     0,   343,   344,   345,
       0,     0,   348,   349,   350,   351,   469,   470,   570,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,   474,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   478,   479,   480,   481,   482,
       0,   483,     0,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,    59,   571,   496,   497,     0,     0,
       0,     0,     0,     0,   498,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   501,   502,   503,     0,    14,     0,     0,   504,   505,
       0,     0,   436,   437,     0,     0,   506,     0,   507,  1196,
     508,   509,   442,   443,   444,   445,   446,     0,     0,     0,
       0,     0,   447,     0,   449,     0,     0,     0,   452,     0,
       0,     0,     0,     0,     0,     0,   454,     0,     0,     0,
       0,     0,   457,     0,     0,   458,     0,     0,   459,     0,
       0,     0,   462,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   569,     0,     0,   466,   467,     0,   321,   322,
     323,     0,   325,   326,   327,   328,   329,   468,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,     0,
     343,   344,   345,     0,     0,   348,   349,   350,   351,   469,
     470,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   473,   474,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   478,   479,
     480,   481,   482,     0,   483,     0,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,    59,   571,   496,
     497,     0,     0,     0,     0,     0,     0,   498,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   501,   502,   503,     0,    14,     0,
       0,   504,   505,     0,     0,   436,   437,     0,     0,   506,
       0,   507,  1399,   508,   509,   442,   443,   444,   445,   446,
       0,     0,     0,     0,     0,   447,     0,   449,     0,     0,
       0,   452,     0,     0,     0,     0,     0,     0,     0,   454,
       0,     0,     0,     0,     0,   457,     0,     0,   458,     0,
       0,   459,     0,     0,     0,   462,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   569,     0,     0,   466,   467,
       0,   321,   322,   323,     0,   325,   326,   327,   328,   329,
     468,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,     0,   343,   344,   345,     0,     0,   348,   349,
     350,   351,   469,   470,   570,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   473,   474,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   478,   479,   480,   481,   482,     0,   483,     0,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
      59,   571,   496,   497,     0,     0,     0,     0,     0,     0,
     498,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   501,   502,   503,
       0,    14,     0,     0,   504,   505,     0,     0,   436,   437,
       0,     0,  1408,     0,   507,  1409,   508,   509,   442,   443,
     444,   445,   446,     0,     0,     0,     0,     0,   447,     0,
     449,     0,     0,     0,   452,     0,     0,     0,     0,     0,
       0,     0,   454,     0,     0,     0,     0,     0,   457,     0,
       0,   458,     0,     0,   459,     0,     0,     0,   462,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   569,     0,
       0,   466,   467,     0,   321,   322,   323,     0,   325,   326,
     327,   328,   329,   468,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,     0,   343,   344,   345,     0,
       0,   348,   349,   350,   351,   469,   470,   570,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   473,   474,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   478,   479,   480,   481,   482,     0,
     483,     0,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,    59,   571,   496,   497,     0,     0,     0,
       0,     0,     0,   498,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     501,   502,   503,     0,    14,     0,     0,   504,   505,     0,
       0,   436,   437,     0,     0,   506,     0,   507,  1414,   508,
     509,   442,   443,   444,   445,   446,     0,     0,     0,     0,
       0,   447,     0,   449,     0,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,   454,     0,     0,     0,     0,
       0,   457,     0,     0,   458,     0,     0,   459,     0,     0,
       0,   462,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   569,     0,     0,   466,   467,     0,   321,   322,   323,
       0,   325,   326,   327,   328,   329,   468,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,     0,   343,
     344,   345,     0,     0,   348,   349,   350,   351,   469,   470,
     570,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,   474,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   478,   479,   480,
     481,   482,     0,   483,     0,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,    59,   571,   496,   497,
       0,     0,     0,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   501,   502,   503,     0,    14,     0,     0,
     504,   505,     0,     0,   436,   437,     0,     0,   506,     0,
     507,  1456,   508,   509,   442,   443,   444,   445,   446,     0,
       0,     0,     0,     0,   447,     0,   449,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,   454,     0,
       0,     0,     0,     0,   457,     0,     0,   458,     0,     0,
     459,     0,     0,     0,   462,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   569,     0,     0,   466,   467,     0,
     321,   322,   323,     0,   325,   326,   327,   328,   329,   468,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,     0,   343,   344,   345,     0,     0,   348,   349,   350,
     351,   469,   470,   570,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,   474,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     478,   479,   480,   481,   482,     0,   483,     0,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,    59,
     571,   496,   497,     0,     0,     0,     0,     0,     0,   498,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   501,   502,   503,     0,
      14,     0,     0,   504,   505,     0,     0,   436,   437,     0,
       0,   506,     0,   507,  1534,   508,   509,   442,   443,   444,
     445,   446,     0,     0,     0,     0,     0,   447,     0,   449,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,   454,     0,     0,     0,     0,     0,   457,     0,     0,
     458,     0,     0,   459,     0,     0,     0,   462,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   569,     0,     0,
     466,   467,     0,   321,   322,   323,     0,   325,   326,   327,
     328,   329,   468,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,     0,   343,   344,   345,     0,     0,
     348,   349,   350,   351,   469,   470,   570,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,   474,     0,     0,     0,     0,     0,     0,     0,  1572,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   478,   479,   480,   481,   482,     0,   483,
       0,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,    59,   571,   496,   497,     0,     0,     0,     0,
       0,     0,   498,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   501,
     502,   503,     0,    14,     0,     0,   504,   505,     0,     0,
     436,   437,     0,     0,   506,     0,   507,     0,   508,   509,
     442,   443,   444,   445,   446,     0,     0,     0,     0,     0,
     447,     0,   449,     0,     0,     0,   452,     0,     0,     0,
       0,     0,     0,     0,   454,     0,     0,     0,     0,     0,
     457,     0,     0,   458,     0,     0,   459,     0,     0,     0,
     462,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     569,     0,     0,   466,   467,     0,   321,   322,   323,     0,
     325,   326,   327,   328,   329,   468,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,     0,   343,   344,
     345,     0,     0,   348,   349,   350,   351,   469,   470,   570,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,   474,     0,     0,     0,     0,     0,
       0,     0,  1614,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   478,   479,   480,   481,
     482,     0,   483,     0,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,    59,   571,   496,   497,     0,
       0,     0,     0,     0,     0,   498,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   501,   502,   503,     0,    14,     0,     0,   504,
     505,     0,     0,   436,   437,     0,     0,   506,     0,   507,
       0,   508,   509,   442,   443,   444,   445,   446,     0,     0,
       0,     0,     0,   447,     0,   449,     0,     0,     0,   452,
       0,     0,     0,     0,     0,     0,     0,   454,     0,     0,
       0,     0,     0,   457,     0,     0,   458,     0,     0,   459,
       0,     0,     0,   462,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   569,     0,     0,   466,   467,     0,   321,
     322,   323,     0,   325,   326,   327,   328,   329,   468,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
       0,   343,   344,   345,     0,     0,   348,   349,   350,   351,
     469,   470,   570,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,   474,     0,     0,
       0,     0,     0,     0,     0,  1615,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   478,
     479,   480,   481,   482,     0,   483,     0,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,    59,   571,
     496,   497,     0,     0,     0,     0,     0,     0,   498,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   501,   502,   503,     0,    14,
       0,     0,   504,   505,     0,     0,   436,   437,     0,     0,
     506,     0,   507,     0,   508,   509,   442,   443,   444,   445,
     446,     0,     0,     0,     0,     0,   447,     0,   449,     0,
       0,     0,   452,     0,     0,     0,     0,     0,     0,     0,
     454,     0,     0,     0,     0,     0,   457,     0,     0,   458,
       0,     0,   459,     0,     0,     0,   462,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   569,     0,     0,   466,
     467,     0,   321,   322,   323,     0,   325,   326,   327,   328,
     329,   468,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,     0,   343,   344,   345,     0,     0,   348,
     349,   350,   351,   469,   470,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   473,
     474,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   478,   479,   480,   481,   482,     0,   483,     0,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,    59,   571,   496,   497,     0,     0,     0,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   501,   502,
     503,     0,    14,     0,     0,   504,   505,     0,     0,   436,
     437,     0,     0,   506,     0,   507,     0,   508,   509,   442,
     443,   444,   445,   446,     0,     0,     0,     0,     0,   447,
       0,   449,     0,     0,     0,   452,     0,     0,     0,     0,
       0,     0,     0,   454,     0,     0,     0,     0,     0,   457,
       0,     0,   458,     0,     0,   459,     0,     0,     0,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   569,
       0,     0,   466,   467,     0,   321,   322,   323,     0,   325,
     326,   327,   328,   329,   468,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,     0,   343,   344,   345,
       0,     0,   348,   349,   350,   351,   469,   470,   570,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,   474,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   478,   479,   480,   481,   482,
       0,   483,     0,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,    59,   571,   496,   497,     0,     0,
       0,     0,   315,     0,   498,     0,     0,     0,   316,     0,
       0,     0,     0,     0,   317,     0,     0,     0,     0,     0,
       0,   501,   502,   503,   318,    14,     0,     0,   504,   505,
       0,     0,   319,     0,     0,     0,  1382,     0,   507,     0,
     508,   509,     0,     0,     0,     0,     0,   320,     0,     0,
       0,     0,     0,     0,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   318,     0,    58,     0,     0,     0,
       0,     0,   319,     0,     0,     0,     0,     0,     0,   354,
       0,     0,     0,     0,     0,     0,     0,   320,     0,     0,
       0,     0,     0,    59,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   355,
       0,   609,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   354,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,   614,   316,     0,     0,     0,     0,     0,
     317,     0,     0,     0,     0,   615,     0,     0,     0,     0,
     318,   616,     0,     0,     0,     0,     0,     0,   319,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   320,     0,     0,     0,     0,     0,   355,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,     0,     0,   316,     0,     0,     0,     0,     0,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     318,     0,    58,     0,     0,     0,     0,     0,   319,     0,
       0,     0,     0,     0,     0,   354,     0,     0,     0,     0,
       0,     0,     0,   320,     0,   739,   740,     0,     0,    59,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   739,   740,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   355,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   354,     0,     0,     0,     0,
     749,   750,   751,   752,   753,     0,     0,   756,     0,   614,
     759,     0,   761,   762,   763,   764,     0,     0,     0,     0,
     765,     0,   767,   768,     0,     0,     0,     0,     0,     0,
     871,   872,   873,   874,   875,   876,   877,   878,   749,   750,
     751,   752,   753,   879,   880,   756,   757,   758,   759,   881,
     761,   762,   763,   764,  -362,   355,   739,   740,   765,   766,
     767,   768,   882,   883,     0,     0,   771,   772,   773,   884,
     885,   886,   777,     0,     0,     0,     0,     0,     0,   783,
     784,   785,   786,   787,   788,   789,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   790,   791,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   887,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,     0,   508,   682,
       0,     0,     0,   871,   872,   873,   874,   875,   876,   877,
     878,   749,   750,   751,   752,   753,   879,   880,   756,   757,
     758,   759,   881,   761,   762,   763,   764,   739,   740,     0,
       0,   765,   766,   767,   768,   882,   883,     0,     0,   771,
     772,   773,   884,   885,   886,   777,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   739,   740,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1080,     0,     0,
       0,     0,     0,     0,   887,   779,     0,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   789,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   790,   791,     0,     0,
       0,   508,   682,     0,   871,   872,   873,   874,   875,   876,
     877,   878,   749,   750,   751,   752,   753,   879,   880,   756,
     757,   758,   759,   881,   761,   762,   763,   764,     0,     0,
       0,     0,   765,   766,   767,   768,   882,   883,     0,     0,
     771,   772,   773,   884,   885,   886,   777,   749,   750,   751,
     752,   753,     0,     0,   756,   757,   758,   759,     0,   761,
     762,   763,   764,   739,   740,     0,     0,   765,     0,   767,
     768,     0,     0,     0,     0,   771,   772,   773,  1283,     0,
       0,   777,     0,     0,     0,   887,   779,     0,   780,   781,
     782,   783,   784,   785,   786,   787,   788,   789,   739,   740,
       0,     0,     0,     0,     0,     0,     0,   790,   791,     0,
       0,     0,   508,   682,     0,     0,     0,     0,     0,     0,
       0,   779,     0,   780,   781,   782,   783,   784,   785,   786,
     787,   788,   789,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   790,   791,     0,     0,     0,   508,   682,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   749,   750,
     751,   752,   753,     0,     0,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,   803,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,   813,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,     0,   739,   740,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,   829,   749,   750,   751,   752,   753,     0,     0,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,   844,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1153,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,     0,   739,   740,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1157,   749,   750,
     751,   752,   753,     0,     0,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1159,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1168,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,     0,   739,   740,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1169,   749,   750,   751,   752,   753,     0,     0,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1171,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,     0,   739,   740,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1172,   749,   750,
     751,   752,   753,     0,     0,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1173,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1304,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,     0,   739,   740,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1316,   749,   750,   751,   752,   753,     0,     0,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1319,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1459,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,     0,   739,   740,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1474,   749,   750,
     751,   752,   753,     0,     0,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1496,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,     0,   739,   740,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1498,   749,   750,   751,   752,   753,     0,     0,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1500,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1504,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,     0,   739,   740,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1577,   749,   750,
     751,   752,   753,     0,     0,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1578,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1580,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,     0,   739,   740,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1587,   749,   750,   751,   752,   753,     0,     0,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1590,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1600,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,     0,   739,   740,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1605,   749,   750,
     751,   752,   753,     0,     0,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,     0,     0,  1671,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
       0,     0,  1672,   749,   750,   751,   752,   753,   739,   740,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,     0,     0,
       0,     0,     0,   739,   740,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
     834,     0,     0,   749,   750,   751,   752,   753,     0,     0,
     756,   757,   758,   759,     0,   761,   762,   763,   764,     0,
       0,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,     0,   777,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
    1103,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,  1255,     0,     0,   749,   750,
     751,   752,   753,   739,   740,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,     0,     0,     0,     0,     0,   739,   740,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,   791,  1271,     0,     0,   749,   750,
     751,   752,   753,     0,     0,   756,   757,   758,   759,     0,
     761,   762,   763,   764,     0,     0,     0,     0,   765,     0,
     767,   768,     0,     0,     0,     0,   771,   772,   773,     0,
       0,     0,   777,   749,   750,   751,   752,   753,     0,     0,
     756,   757,   758,   759,     0,   761,   762,   763,   764,   235,
     236,     0,     0,   765,     0,   767,   768,     0,     0,     0,
       0,   771,   772,   773,     0,     0,   237,   777,     0,     0,
       0,     0,   779,     0,   780,   781,   782,   783,   784,   785,
     786,   787,   788,   789,   739,   740,     0,     0,     0,     0,
       0,     0,     0,   790,   791,  1406,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,   791,
    1412,     0,     0,     0,     0,     0,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,     0,     0,   256,   257,   258,     0,
       0,     0,     0,     0,     0,   259,   260,   261,   262,   263,
       0,     0,   264,   265,   266,   267,   268,   269,   270,   749,
     750,   751,   752,   753,   739,   740,   756,   757,   758,   759,
       0,   761,   762,   763,   764,     0,     0,     0,     0,   765,
       0,   767,   768,     0,     0,     0,     0,   771,   772,   773,
       0,     0,     0,   777,     0,     0,     0,     0,   271,     0,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
       0,     0,   282,   283,     0,     0,     0,     0,     0,   284,
     285,     0,     0,   290,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   779,     0,   780,   781,   782,   783,   784,
     785,   786,   787,   788,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   790,   791,     0,     0,     0,   749,
     750,   751,   752,   753,   739,   740,   756,   757,   758,   759,
       0,   761,   762,   763,   764,     0,     0,     0,     0,   765,
       0,   767,   768,     0,     0,   938,     0,   771,   772,   773,
       0,     0,     0,   777,     0,     0,     0,     0,     0,   739,
     740,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   779,     0,   780,   781,   782,   783,   784,
     785,   786,   787,   788,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   790,   791,     0,     0,     0,   749,
     750,   751,   752,   753,     0,     0,   756,   757,   758,   759,
       0,   761,   762,   763,   764,     0,     0,     0,     0,   765,
       0,   767,   768,     0,     0,  1182,     0,   771,   772,   773,
       0,     0,     0,   777,   749,   750,   751,   752,   753,   739,
     740,   756,   757,   758,   759,     0,   761,   762,   763,   764,
       0,     0,     0,     0,   765,     0,   767,   768,     0,     0,
       0,     0,   771,   772,   773,     0,     0,     0,   777,     0,
       0,     0,     0,   779,     0,   780,   781,   782,   783,   784,
     785,   786,   787,   788,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   790,   791,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   779,  1260,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   790,
     791,     0,     0,     0,   749,   750,   751,   752,   753,   739,
     740,   756,   757,   758,   759,     0,   761,   762,   763,   764,
       0,     0,     0,     0,   765,     0,   767,   768,     0,     0,
       0,     0,   771,   772,   773,     0,     0,     0,   777,     0,
       0,     0,     0,     0,   739,   740,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1273,     0,     0,     0,     0,     0,     0,     0,   779,     0,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   790,
     791,     0,     0,     0,   749,   750,   751,   752,   753,     0,
       0,   756,   757,   758,   759,     0,   761,   762,   763,   764,
       0,     0,     0,     0,   765,     0,   767,   768,     0,     0,
       0,     0,   771,   772,   773,     0,     0,     0,   777,   749,
     750,   751,   752,   753,   739,   740,   756,   757,   758,   759,
       0,   761,   762,   763,   764,     0,     0,     0,     0,   765,
       0,   767,   768,     0,     0,     0,  1366,   771,   772,   773,
       0,     0,     0,   777,     0,     0,     0,     0,   779,     0,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   790,
     791,     0,     0,     0,     0,  1601,     0,     0,     0,     0,
       0,     0,     0,   779,     0,   780,   781,   782,   783,   784,
     785,   786,   787,   788,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   790,   791,     0,     0,     0,   749,
     750,   751,   752,   753,   739,   740,   756,   757,   758,   759,
       0,   761,   762,   763,   764,     0,     0,     0,     0,   765,
       0,   767,   768,     0,     0,     0,     0,   771,   772,   773,
       0,     0,     0,   777,     0,     0,     0,     0,     0,   739,
     740,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1662,     0,     0,     0,     0,
       0,     0,     0,   779,     0,   780,   781,   782,   783,   784,
     785,   786,   787,   788,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   790,   791,     0,     0,     0,   749,
     750,   751,   752,   753,     0,     0,   756,   757,   758,   759,
       0,   761,   762,   763,   764,     0,     0,     0,     0,   765,
       0,   767,   768,     0,     0,     0,     0,   771,   772,   773,
       0,     0,     0,   777,   749,   750,   751,   752,   753,   739,
     740,   756,   757,   758,   759,     0,   761,   762,   763,   764,
       0,     0,     0,     0,   765,     0,   767,   768,     0,     0,
       0,     0,   771,   772,   773,  1687,     0,     0,   777,     0,
       0,     0,     0,   779,     0,   780,   781,   782,   783,   784,
     785,   786,   787,   788,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   790,   791,     0,     0,     0,     0,
    1688,     0,     0,     0,     0,     0,     0,     0,   779,     0,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   790,
     791,     0,     0,     0,   749,   750,   751,   752,   753,   739,
     740,   756,   757,   758,   759,     0,   761,   762,   763,   764,
       0,     0,     0,     0,   765,     0,   767,   768,     0,     0,
       0,     0,   771,   772,   773,     0,     0,     0,   777,     0,
       0,     0,     0,     0,   739,   740,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1694,     0,     0,     0,     0,     0,     0,     0,   779,     0,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   790,
     791,     0,     0,     0,   749,   750,   751,   752,   753,     0,
       0,   756,   757,   758,   759,     0,   761,   762,   763,   764,
       0,     0,     0,     0,   765,     0,   767,   768,     0,     0,
       0,     0,   771,   772,   773,     0,     0,     0,   777,   749,
     750,   751,   752,   753,   739,   740,   756,   757,   758,   759,
       0,   761,   762,   763,   764,     0,     0,     0,     0,   765,
       0,   767,   768,     0,     0,     0,     0,   771,   772,   773,
       0,     0,     0,  -761,     0,   739,   740,     0,   779,     0,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   790,
     791,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   779,     0,   780,   781,   782,   783,   784,
     785,   786,   787,   788,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   790,   791,     0,     0,     0,   749,
     750,   751,   752,   753,     0,     0,   756,   757,   758,   759,
       0,   761,   762,   763,   764,     0,     0,     0,     0,   765,
       0,   767,   768,     0,     0,   739,   740,   771,     0,   773,
     749,   750,   751,   752,   753,     0,     0,   756,   757,   758,
     759,     0,   761,   762,   763,   764,     0,     0,     0,     0,
     765,     0,   767,   768,   739,   740,     0,     0,   771,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   780,   781,   782,   783,   784,
     785,   786,   787,   788,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   790,   791,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   780,   781,   782,   783,
     784,   785,   786,   787,   788,   789,     0,     0,     0,     0,
     749,   750,   751,   752,   753,   790,   791,   756,   757,   758,
     759,     0,   761,   762,   763,   764,     0,     0,     0,     0,
     765,     0,   767,   768,     0,     0,     0,     0,     0,   749,
     750,   751,   752,   753,     0,     0,   756,   757,   758,   759,
       0,   761,   762,   763,   764,     0,     0,     0,     0,   765,
       0,   767,   768,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   980,     0,   780,   781,   782,   783,
     784,   785,   786,   787,   788,   789,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   790,   791,     0,     0,     0,
       0,     0,     0,     0,     0,   984,   781,   782,   783,   784,
     785,   786,   787,   788,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   790,   791,   321,   322,   323,     0,
     325,   326,   327,   328,   329,   468,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,     0,   343,   344,
     345,     0,     0,   348,   349,   350,   351,   321,   322,   323,
       0,   325,   326,   327,   328,   329,   468,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,     0,   343,
     344,   345,     0,     0,   348,   349,   350,   351,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1256,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   981,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   982,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   985,     0,     0,     0,     0,     0,
       0,     0,     0,   321,   322,   323,   986,   325,   326,   327,
     328,   329,   468,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,     0,   343,   344,   345,     0,     0,
     348,   349,   350,   351,   321,   322,   323,     0,   325,   326,
     327,   328,   329,   468,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,     0,   343,   344,   345,     0,
       0,   348,   349,   350,   351,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   185,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1257,     0,  1059,  1060,     0,     0,     0,     0,     0,     0,
       0,     0,  1258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,     0,   187,     0,   188,   189,   190,   191,
     192,  1061,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,  1062,   204,   205,   206,     0,     0,   207,
     208,   209,   210,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   211,
     212,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1063,  1064,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   213
};

static const yytype_int16 yycheck[] =
{
      14,    15,   215,   576,   380,   487,   525,   595,   695,   393,
     697,   629,   596,   424,   425,   590,   231,   592,   717,   594,
     719,   164,   476,   477,   387,  1185,   634,    81,   380,   913,
     820,   858,   822,   697,   824,  1326,    53,     5,   422,     0,
     542,   728,   231,     8,    20,  1385,     7,    20,    22,    63,
      64,    65,   160,    33,    20,  1426,    20,   929,   448,    19,
      20,    20,    20,   935,   566,    15,    16,   104,    20,    30,
     127,    32,   143,    34,    53,     4,   129,   130,   163,    40,
     137,  1554,   472,    57,    46,   380,   694,   127,   696,    50,
     698,   105,   106,   107,   108,    56,   448,   132,   706,   170,
     170,   485,   486,     5,     6,   182,   170,   715,    57,   217,
     144,   145,   146,   170,    63,   180,   158,   142,   170,    80,
     472,   127,   127,    25,   143,    62,   127,    33,   170,    31,
     182,   137,   189,   832,   102,   103,   137,   194,   215,   966,
    1613,   102,   103,   214,   106,  1485,   216,   182,   188,   189,
     214,   191,  1523,   448,   194,   180,     7,   126,   211,   212,
     129,   130,   305,   378,    49,   182,    68,    69,   867,   223,
      33,   858,   191,   215,   144,    33,   146,   472,  1549,   158,
    1450,   215,   684,   189,   189,   222,  1346,   192,   189,   378,
     580,   170,   891,    21,    22,    15,    16,    60,    61,    50,
     102,   103,    60,    61,   393,   904,   617,    92,   907,   127,
     170,   622,   188,   163,   194,   188,   181,   231,   168,   137,
     170,   182,   188,   173,   188,   143,   200,   195,   580,   188,
     188,   160,   181,   422,  1525,   424,   188,   211,   451,   182,
     201,   143,   211,   212,  1514,  1515,   214,   212,    57,   182,
     182,   212,   465,   185,    63,   309,   191,   647,   160,   161,
     162,   124,  1532,   158,   214,   128,   124,   143,   170,   653,
     128,   189,  1581,     5,     6,   170,     8,   189,   180,   966,
     967,   213,  1166,   737,   217,   580,    34,   677,   194,  1116,
     118,   119,   676,   195,   170,   647,   485,   486,   126,   188,
    1460,   129,   130,   131,    36,  1586,   212,   182,   136,  1181,
     212,  1545,   218,  1547,   189,    63,  1597,  1551,  1552,   214,
    1629,  1591,  1592,   155,   182,   677,   215,   190,   160,   152,
     188,   194,   190,   196,   197,   188,   194,   160,   196,   197,
     188,   355,  1226,   163,     5,     6,   954,   170,   168,   212,
     170,   155,   647,   173,   212,   218,   160,  1638,  1639,   188,
     218,   185,   186,   127,   378,   213,   581,   582,   583,   383,
     585,   586,   735,   137,   589,   107,   591,  1611,   593,   393,
     127,   188,   677,   211,   212,   217,   215,   180,   136,   213,
     137,   214,   581,   582,   583,   143,   585,   586,  1282,   127,
     589,   812,   591,   978,   593,   180,   595,   188,   422,   137,
     424,   215,   188,   217,   188,   826,   806,   828,   802,   188,
     633,   182,   170,   216,   217,   189,   182,   188,   180,  1116,
     444,   445,   188,   191,   215,   196,   188,   213,   196,   214,
     196,   215,   189,   170,   213,   658,   741,   742,   743,   744,
     745,   746,   747,   748,   806,  1245,   214,   205,   940,   754,
     755,   189,   852,   965,   653,   760,   214,   921,   950,   853,
     854,   485,   486,  1345,   769,   770,   188,   143,   889,   774,
     775,   776,   866,   778,   180,  1357,   152,   676,   195,   188,
     703,  1109,   143,   188,  1181,   191,   188,  1184,    47,   214,
     852,   213,    21,    22,   170,   188,   188,   214,  1096,  1196,
     723,   806,   896,   897,  1098,   899,   215,  1181,    67,   903,
     215,   905,   906,   215,   908,   170,   171,   172,   143,   188,
     213,   188,  1196,   215,   988,   989,   990,   152,   188,   188,
     170,   195,   157,    63,    64,    65,   188,   188,   182,  1339,
     739,   740,   182,   188,   213,   170,   201,   852,   215,   191,
     214,   943,   196,   945,   196,   215,   215,   581,   582,   583,
     188,   585,   586,   215,   215,   589,   765,   591,   213,   593,
     195,   595,   214,   195,  1374,   105,   106,   107,   108,    21,
      22,   191,  1167,   163,   182,   213,   196,   116,   117,   118,
     119,   790,   214,   181,   182,   195,   184,   126,   196,   128,
     129,   130,   131,   802,   214,   830,   170,   136,   195,   138,
     139,   158,   191,   838,   214,   639,   841,   196,   182,   211,
     152,   158,   182,   170,   188,   850,   152,   214,   188,   653,
     170,   830,   857,   170,  1219,   214,   196,   862,   170,   838,
      57,   192,   841,   182,   170,   196,    63,   182,   801,   188,
     158,   850,   676,   188,   853,   854,   192,   196,   857,   170,
     196,   196,   170,   862,   182,   888,   158,   866,   197,   198,
     199,   200,   201,   182,    21,    22,   118,   119,   170,    12,
     158,  1481,   211,   212,   126,  1382,   128,   129,   130,   131,
      23,    24,   170,   182,   136,    57,   720,   896,   897,   188,
     899,    63,   182,   192,   903,   170,   905,   906,   188,   908,
     192,  1408,   174,    33,   196,   170,   196,   170,   943,   188,
     945,  1349,   191,   181,    75,   194,   184,   170,    79,   187,
      57,  1125,  1126,  1127,   170,   214,    63,   216,   217,    57,
      60,    61,    93,    94,   943,    63,   945,    98,    99,   100,
     101,   170,   171,   172,    57,   182,  1150,   199,   200,   201,
      63,   188,    57,   174,   175,   176,   177,   182,    63,   211,
     212,   118,   119,   188,   182,   799,   216,   217,   802,   126,
     188,   128,   129,   130,   131,   182,   182,   182,    57,   136,
     182,   188,   188,   188,  1379,   182,   188,   180,   192,   192,
     144,   192,   196,   196,   124,   196,   830,   192,   128,  1338,
     191,   196,   192,   194,   838,   192,   196,   841,   192,   196,
     191,   174,   196,   194,  1218,  1211,   850,    21,    22,   853,
     854,  1459,   106,   857,   189,   192,  1533,   180,   862,   196,
     174,   175,   866,   144,   145,   146,    87,    88,  1047,  1211,
     197,   198,   199,   200,   201,   174,   175,   176,   180,  1084,
     174,   175,   176,   180,   211,   212,   181,   182,   183,    35,
     190,    35,   896,   897,   194,   899,   196,   197,   214,   903,
     180,   905,   906,   170,   908,  1084,   170,  1596,    10,    11,
      12,   181,   212,   170,  1315,   170,  1593,  1096,   218,  1608,
     213,    22,   170,   213,  1487,   191,  1211,   180,   191,   180,
     214,    43,   214,   195,   195,   195,   195,   195,   214,   943,
     214,   945,   192,  1632,   118,   119,  1125,  1126,  1127,   195,
     380,   195,   126,  1630,   128,   129,   130,   131,   195,   195,
     390,   195,   136,   195,   170,   137,   170,   214,   213,   399,
     170,  1150,   170,   212,   214,   195,   195,   195,   170,   409,
     195,   195,   163,   214,   195,   182,   195,   195,   195,   419,
     214,   214,    21,    22,   195,    37,  1468,  1605,   428,  1471,
     217,    10,   214,   182,   214,  1568,  1569,   182,   438,   214,
      66,   214,   182,   182,  1691,   182,   182,  1220,   448,   182,
     214,  1395,   212,   453,   181,   455,   214,   475,   476,   477,
      21,    22,    43,   195,   464,   195,   170,   211,   212,  1218,
     170,   182,   472,   473,   474,   181,   181,   215,   213,   191,
     181,    43,   195,   214,   132,   214,  1619,   487,    33,   188,
      14,   189,   195,   195,   191,   495,   163,   217,   498,   188,
     180,   501,   502,   503,   504,   505,    13,   525,   170,   188,
    1084,     8,   170,   196,   514,    60,    61,   116,   117,   118,
     119,   120,  1096,   215,   123,   124,   125,   126,  1303,   128,
     129,   130,   131,   170,   552,   170,  1309,   136,   170,   138,
     139,   215,  1486,   188,   214,   144,   145,   146,   188,   213,
     181,  1125,  1126,  1127,  1303,   116,   117,   118,   119,   120,
     181,   215,   123,   124,   125,   126,   215,   128,   129,   130,
     131,   214,   214,   170,     1,   136,  1150,   138,   139,   124,
     580,   214,   195,   128,   214,   195,   214,   214,   181,   214,
     214,   190,   214,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,  1376,    67,   214,   182,   170,   608,   196,
     196,   196,   211,   212,    43,   215,   170,   215,   213,   215,
     214,  1394,  1397,  1567,   215,   215,   214,   170,   170,   213,
     213,   170,   214,   651,   195,   196,   197,   198,   199,   200,
     201,   215,   170,   195,  1218,   190,  1395,   647,  1397,   194,
     211,   212,   197,   182,   654,   655,   214,   657,   214,   214,
     170,   679,   662,   214,   170,   448,   215,   212,   668,   182,
     214,   170,   213,   218,   214,   213,   170,   677,   215,   213,
      33,   214,  1455,   214,   214,   170,    70,   196,   688,   689,
     690,   691,   692,   693,   214,   695,   215,   697,   214,   214,
     214,   196,   214,   214,   196,   182,    53,   215,   181,   213,
     215,   214,   214,   214,   214,   181,   188,   215,   182,   737,
     181,   188,   215,   181,   507,   215,  1501,   215,   215,  1303,
     213,    21,    22,   215,   215,   215,   215,  1486,   213,   215,
     214,   741,   742,   743,   744,   745,   746,   747,   748,   749,
     750,   213,  1501,   753,   754,   755,   756,   757,   758,   215,
     760,   761,   213,   763,   764,   765,   766,   767,   768,   769,
     770,   771,   772,   773,   774,   775,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
     808,   791,  1565,   215,   215,   795,   705,    80,     1,    44,
     136,    83,  1478,   229,  1479,   799,   806,   952,  1479,  1479,
    1479,     1,  1479,  1455,   973,  1237,   599,  1392,  1567,  1418,
    1511,  1395,  1278,  1397,   432,  1421,   116,   117,   118,   119,
     120,  1512,    52,   123,   124,   125,   126,   444,   128,   129,
     130,   131,   564,   444,  1466,   845,   136,  1593,   138,   139,
     697,  1191,   852,    -1,   498,    -1,    -1,    -1,    -1,   859,
      -1,   861,    -1,   646,   647,    -1,    -1,    -1,    -1,    -1,
     870,   871,   872,   873,   874,   875,   876,   877,   878,   879,
     880,   881,   882,   883,   884,   885,   886,   887,   671,    -1,
      -1,   674,  1665,   911,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   921,   194,   195,   196,   197,   198,   199,
     200,   201,  1486,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   212,    -1,   924,   925,    -1,  1501,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   938,    -1,
     940,    -1,    -1,    -1,    -1,   728,    -1,   947,    -1,    -1,
     950,    -1,    -1,    -1,    -1,   955,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   964,    -1,    -1,    -1,    -1,    -1,
     988,   989,   990,    -1,    -1,   993,    -1,   995,    -1,   997,
    1554,   999,    -1,  1001,    -1,  1003,    -1,  1005,    -1,  1007,
      -1,    -1,    -1,  1567,  1012,    -1,  1014,    -1,    -1,    -1,
      -1,    -1,  1020,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     793,    -1,    -1,    -1,  1032,    -1,  1034,    -1,    -1,    -1,
      -1,  1039,    -1,  1041,    -1,  1043,    -1,    -1,  1046,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,  1613,
      33,    -1,    33,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
    1060,  1079,    -1,    -1,  1064,    41,    -1,    60,    61,    60,
      61,    -1,    -1,    49,    -1,   858,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,  1111,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,   124,    -1,   124,   127,   128,   127,   128,    -1,    -1,
      -1,    -1,    -1,   926,   137,    -1,   137,    -1,    -1,    -1,
      -1,   934,    -1,    -1,  1154,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,  1176,  1177,  1178,    -1,
     156,    -1,  1182,    -1,    21,    22,    -1,    60,    61,   182,
    1190,  1191,    -1,    -1,   170,    -1,   189,   190,   189,   190,
      -1,   194,    -1,   194,   197,    -1,   197,    -1,    -1,    -1,
      -1,  1211,    -1,    -1,    -1,    -1,  1216,  1217,    -1,   212,
      -1,   212,    -1,  1223,    -1,   218,    -1,   218,    -1,    -1,
      -1,  1231,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,  1247,    -1,  1249,
      -1,   124,    -1,    -1,  1254,   128,    -1,    -1,    -1,    -1,
    1260,    -1,    -1,    -1,  1264,    60,    61,    -1,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,    -1,  1306,   144,   145,   146,
     147,   148,   149,   150,    -1,    -1,    -1,   190,    -1,    -1,
    1338,   194,    -1,   196,   197,    -1,  1326,    -1,    -1,   124,
      -1,  1114,    -1,   128,    -1,    -1,    -1,    -1,    -1,   212,
      -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,   189,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   216,
     217,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,  1174,    -1,    -1,    -1,   190,    -1,    12,    -1,   194,
      -1,   196,   197,  1403,    -1,  1405,    21,    22,    -1,    -1,
      -1,  1411,  1195,    60,    61,    -1,    -1,   212,  1201,    -1,
      -1,    -1,    -1,   218,    -1,  1208,    -1,    -1,    -1,  1212,
      -1,    -1,    -1,    -1,    -1,  1435,    -1,    -1,  1438,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1446,  1447,  1448,    -1,
      -1,    -1,    -1,  1453,    -1,    -1,    -1,    -1,  1458,    -1,
      -1,    -1,  1462,  1463,  1464,  1465,    -1,    -1,  1468,  1469,
    1253,  1471,  1472,    -1,    -1,    -1,    -1,   124,    -1,  1479,
      -1,   128,  1265,    -1,    -1,    -1,    -1,  1270,    -1,    -1,
      -1,    -1,  1492,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,  1525,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,  1539,
    1540,    -1,    -1,   190,    -1,    -1,    -1,   194,    -1,   196,
     197,    -1,    -1,  1553,    -1,    -1,    -1,  1340,  1558,    -1,
      -1,    -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,    -1,
    1570,   218,  1572,    -1,    -1,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,  1370,  1588,  1589,
      -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,  1615,    25,    -1,    -1,    -1,
    1620,  1621,    31,    -1,   742,   743,   744,   745,   746,   747,
     748,    -1,    41,    -1,    -1,    -1,   754,   755,    -1,    -1,
      49,    -1,   760,  1426,    -1,    -1,  1646,    -1,  1648,  1649,
      -1,   769,   770,    -1,    -1,    64,   774,   775,   776,  1659,
     778,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,    -1,    -1,     5,     6,     7,    -1,     9,
      10,    11,    -1,    13,    -1,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      -1,    31,    -1,    -1,   143,    -1,    -1,    -1,    38,    39,
    1523,    -1,    42,    -1,    44,    45,  1529,   156,    48,    -1,
      50,    51,    52,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,   170,    -1,    -1,    -1,    65,  1549,    -1,    68,    69,
      -1,    71,    72,    73,  1557,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1614,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,    33,
     180,   181,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,
      -1,   201,    -1,    -1,   204,   205,    60,    61,    -1,    -1,
      -1,    -1,   212,    -1,   214,     1,   216,   217,    -1,     5,
       6,     7,    -1,     9,    10,    11,    -1,    13,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    42,    -1,    44,    45,
      -1,    -1,    48,    -1,    50,    51,    52,    -1,    54,    55,
     124,    -1,    58,    59,   128,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,   190,    -1,    -1,    -1,
     194,    -1,   196,   197,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   212,    -1,
      -1,    -1,    -1,    -1,   218,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   180,   181,   182,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,    -1,
     216,   217,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    60,    61,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    33,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,   190,    -1,    -1,
      -1,   194,    -1,    -1,   197,   198,   199,    -1,   201,    -1,
      -1,   204,   205,    -1,    -1,    -1,     5,     6,    -1,   212,
      -1,   214,    -1,   216,   217,   218,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,   124,    -1,    45,    -1,   128,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
     190,    -1,    -1,    -1,   194,    -1,   196,   197,    -1,   118,
     119,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,
      -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    33,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,    -1,    33,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,   190,    -1,    -1,    -1,   194,    -1,    -1,   197,   198,
     199,    -1,   201,    60,    61,   204,   205,    -1,    -1,    -1,
       5,     6,    -1,   212,    -1,   214,    -1,   216,   217,   218,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,   124,    -1,
      45,    -1,   128,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      65,   128,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,   190,    -1,    -1,    -1,   194,    -1,
     196,   197,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,    -1,    -1,   212,   194,    -1,   196,
     197,    -1,   218,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,   212,   151,   152,   153,   154,
     155,   218,   157,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
      -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,
     205,    -1,    -1,    -1,     5,     6,    -1,   212,    -1,   214,
      -1,   216,   217,   218,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,
       5,     6,    -1,    -1,   212,    -1,   214,    -1,   216,   217,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    52,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
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
      69,    70,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,    -1,   201,    -1,    -1,   204,   205,    -1,    -1,     5,
       6,    -1,    -1,   212,    -1,   214,   215,   216,   217,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    33,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,   124,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      33,   157,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    60,    61,    -1,
      -1,   190,    -1,    -1,    -1,   194,    -1,   196,   197,    -1,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,    -1,   212,     5,     6,   212,    -1,   214,   218,
     216,   217,    13,    -1,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    33,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,   124,    -1,    -1,    45,   128,    -1,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,   190,    -1,    -1,
      -1,   194,    -1,   196,   197,    -1,    -1,   118,   119,    -1,
     124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,   212,
      -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    33,   157,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    60,    61,    -1,    -1,   190,    -1,    -1,    -1,
     194,    -1,   196,   197,    -1,    -1,   197,   198,   199,    -1,
     201,    -1,    -1,   204,   205,    -1,    -1,    -1,   212,     5,
       6,   212,   213,   214,   218,   216,   217,    13,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    33,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,   124,    -1,    -1,    45,
     128,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,   190,    -1,    -1,    -1,   194,    -1,   196,   197,
      -1,    -1,   118,   119,    -1,   124,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      33,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    60,    61,    -1,
      -1,   190,    -1,    -1,    -1,   194,    -1,   196,   197,    -1,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,    -1,   212,     5,     6,   212,    -1,   214,   218,
     216,   217,    13,    -1,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    33,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,   124,    -1,    -1,    45,   128,    -1,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,   190,    -1,    -1,
      -1,   194,    -1,   196,   197,    -1,    -1,   118,   119,    -1,
     124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,   212,
      -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    33,   157,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    60,    61,    -1,    -1,   190,    -1,    -1,    -1,
     194,    -1,   196,   197,    -1,    -1,   197,   198,   199,    -1,
     201,    -1,    -1,   204,   205,    -1,    -1,    -1,   212,     5,
       6,   212,   213,   214,   218,   216,   217,    13,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,   124,    -1,    -1,    45,
     128,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,   190,    -1,    -1,    -1,   194,    -1,   196,   197,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
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
      -1,    -1,    65,    -1,    -1,    68,    69,    70,    71,    72,
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
      -1,   214,    -1,   216,   217,    15,    16,    17,    18,    19,
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
      -1,    -1,   212,   213,   214,    -1,   216,   217,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    -1,
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
      -1,     5,     6,    -1,    -1,   212,    -1,   214,    -1,   216,
     217,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    33,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,
     204,   205,    -1,    -1,    -1,    -1,     5,     6,   212,    -1,
     214,    -1,   216,   217,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,   124,    -1,    -1,    45,   128,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,   190,
      -1,    -1,    -1,   194,    -1,   196,   197,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   212,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,
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
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,    -1,    -1,     5,     6,   212,    -1,   214,    -1,
     216,   217,    13,    -1,    15,    16,    17,    18,    19,    -1,
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
      -1,   212,    -1,   214,    -1,   216,   217,    15,    16,    17,
      18,    19,    -1,    -1,    22,    -1,    -1,    25,    -1,    27,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,
     205,    -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,
     215,   216,   217,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,    -1,   197,   198,   199,    -1,   201,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,    -1,   201,    -1,    -1,   204,   205,
      -1,    -1,     5,     6,    -1,    -1,   212,    -1,   214,   215,
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
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    19,    -1,   180,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,    41,   201,    -1,    -1,   204,   205,
      -1,    -1,    49,    -1,    -1,    -1,   212,    -1,   214,    -1,
     216,   217,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,    76,
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
      -1,    -1,    -1,    64,    -1,    21,    22,    -1,    -1,   170,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,    -1,   170,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   216,    21,    22,   136,   137,
     138,   139,   140,   141,    -1,    -1,   144,   145,   146,   147,
     148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,
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
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,    -1,    -1,
      -1,    -1,    -1,    -1,   189,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,
      -1,   216,   217,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,   137,   138,   139,   140,   141,    -1,    -1,
     144,   145,   146,   147,   148,   149,   150,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,   182,    -1,
      -1,   150,    -1,    -1,    -1,   189,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,    -1,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   211,   212,    -1,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
      -1,    -1,   215,   116,   117,   118,   119,   120,    21,    22,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
     213,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,   213,    -1,    -1,   116,   117,
     118,   119,   120,    21,    22,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,   213,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    38,   150,    -1,    -1,
      -1,    -1,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,   213,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,   212,
     213,    -1,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,    -1,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,   144,   145,   146,   147,   148,   149,   150,   116,
     117,   118,   119,   120,    21,    22,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   190,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,   204,   205,    -1,    -1,    -1,    -1,    -1,   211,
     212,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    21,    22,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,   142,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,   142,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    21,
      22,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,
     212,    -1,    -1,    -1,   116,   117,   118,   119,   120,    21,
      22,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,
     212,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    21,    22,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,   178,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   190,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,
     212,    -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    21,    22,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    21,
      22,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,   182,    -1,    -1,   150,    -1,
      -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,    -1,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,
     212,    -1,    -1,    -1,   116,   117,   118,   119,   120,    21,
      22,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,
     212,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    21,    22,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    21,    22,    -1,   190,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,
     212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    21,    22,   144,    -1,   146,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    21,    22,    -1,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,   211,   212,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,   170,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    76,    77,    78,
      79,   158,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,   170,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,   212,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170
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
     292,    34,    63,   136,   205,   214,   264,   265,   266,   267,
     292,   182,     5,     6,     8,    36,   107,   403,    62,   382,
     189,   188,   191,   188,   235,    22,    57,   200,   211,   237,
     387,   388,   390,   388,   382,   462,   452,   457,   170,   143,
     231,   266,   266,   266,   214,   144,   145,   146,   188,   213,
      57,    63,   273,   275,    57,    63,   393,     5,     6,    57,
      63,   404,    57,    63,   383,    15,    16,   163,   168,   170,
     173,   214,   226,   261,   163,   240,   170,   234,   234,   170,
     182,   181,   388,   182,    57,    63,   222,   170,   170,   170,
     170,   174,   229,   215,   262,   266,   266,   266,   266,   276,
     170,   394,   407,   180,   385,   174,   175,   176,   225,    15,
      16,   163,   168,   170,   226,   258,   259,   237,   389,   182,
     463,   453,   458,   174,   215,    35,    71,    73,    75,    76,
      77,    78,    79,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    93,    94,    95,    98,    99,   100,
     101,   118,   119,   170,   271,   274,   180,   191,   106,   401,
     402,   380,   160,   217,   263,   356,   174,   175,   176,   188,
     215,   189,   180,   180,   180,    21,    22,    38,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   128,   129,   130,   137,
     138,   139,   140,   141,   144,   145,   146,   147,   148,   149,
     150,   190,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   204,   205,   211,   212,    35,    35,   214,   269,
     180,   277,   396,    75,    79,    93,    94,    98,    99,   100,
     101,   411,   170,   408,   181,   381,   262,   261,   182,   217,
     152,   170,   378,   379,   258,    19,    25,    31,    41,    49,
      64,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   156,   216,   292,   410,   412,   413,
     416,   422,   450,   464,   454,   459,   170,   170,   170,   213,
      22,   170,   213,   155,   215,   356,   366,   367,   191,   270,
     280,   386,   180,   191,   400,   180,   405,   356,   213,   261,
     214,    43,   188,   191,   194,   377,   195,   195,   195,   214,
     195,   195,   214,   195,   195,   195,   195,   195,   195,   214,
     292,    33,    60,    61,   124,   128,   190,   194,   197,   212,
     218,   421,   192,   415,   370,   373,   170,   137,   214,     7,
      50,   305,   182,   215,   450,     1,     5,     6,     9,    10,
      11,    13,    15,    16,    17,    18,    19,    25,    26,    27,
      28,    29,    31,    38,    39,    42,    44,    45,    48,    51,
      52,    54,    55,    58,    59,    65,    68,    69,    80,   102,
     103,   104,   105,   118,   119,   133,   134,   135,   151,   152,
     153,   154,   155,   157,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   171,   172,   173,   180,   181,
     182,   197,   198,   199,   204,   205,   212,   214,   216,   217,
     228,   230,   241,   242,   245,   248,   249,   251,   253,   254,
     255,   256,   278,   279,   281,   286,   291,   292,   293,   297,
     298,   299,   300,   301,   302,   303,   304,   306,   310,   311,
     318,   321,   324,   329,   332,   333,   335,   336,   337,   339,
     344,   347,   348,   355,   410,   466,   481,   492,   496,   509,
     512,   170,   182,   397,   398,   292,   362,   379,   213,    65,
     104,   171,   286,   348,   170,   170,   422,   127,   137,   189,
     376,   423,   428,   430,   348,   432,   426,   170,   417,   434,
     436,   438,   440,   442,   444,   446,   448,   348,   195,   214,
      33,   194,    33,   194,   212,   218,   213,   348,   212,   218,
     422,   170,   182,   465,   170,   182,   188,   368,   419,   450,
     455,   170,   371,   419,   460,   348,   152,   170,   375,   409,
     366,   195,   195,   348,   252,   195,   294,   412,   466,   214,
     292,   195,     5,   102,   103,   195,   214,   127,   291,   322,
     333,   348,   277,   195,   214,    61,   348,   214,   348,   170,
     195,   195,   214,   182,   195,   163,    58,   348,   214,   277,
     195,   214,   195,   195,   214,   195,   195,   127,   291,   348,
     348,   348,   217,   277,   324,   328,   328,   328,   214,   214,
     214,   214,   214,   214,    13,   422,    13,   422,    13,   348,
     491,   507,   195,   348,   195,   227,    13,   491,   508,    37,
     348,   348,   348,   348,   348,    13,    49,   322,   348,   322,
     217,   182,   182,   348,    10,   324,   330,   170,   214,   182,
     182,   182,   182,   182,    66,   307,   268,   132,   182,    21,
      22,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   136,   137,   138,   139,   140,
     141,   144,   145,   146,   147,   148,   149,   150,   189,   190,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     211,   212,   330,   214,   182,   189,   181,   386,   181,   212,
     268,   363,   195,   215,    43,   182,   376,   291,   348,   450,
     450,   420,   450,   215,   450,   450,   215,   170,   414,   450,
     269,   450,   269,   450,   269,   368,   369,   371,   372,   215,
     425,   282,   322,   213,   213,   348,   182,   181,   191,   419,
     181,   191,   419,   181,   215,   214,    43,   127,   188,   189,
     191,   194,   374,   482,   484,   277,   409,   295,   214,   292,
     195,   214,   319,   195,   195,   195,   503,   322,   291,   322,
     188,   108,   109,   110,   111,   112,   113,   114,   115,   121,
     122,   127,   140,   141,   147,   148,   149,   189,    14,   422,
     284,   508,   348,   348,   277,   189,   312,   314,   348,   316,
     191,   163,   348,   505,   322,   488,   493,   322,   486,   422,
     291,   348,   217,   268,   348,   348,   348,   348,   348,   348,
     409,    53,   158,   170,   197,   212,   214,   348,   467,   470,
     474,   490,   495,   409,   214,   470,   495,   409,   142,   181,
     182,   183,   475,   287,   277,   289,   176,   177,   225,   409,
     188,   511,   180,   409,    13,   188,   511,   511,   152,   157,
     195,   292,   338,   277,   250,   331,    70,   212,   215,   322,
     467,   469,   160,   214,   309,   379,     4,   160,   327,   328,
      19,   158,   170,   410,    19,   158,   170,   410,   133,   134,
     135,   278,   334,   348,   334,   348,   334,   348,   334,   348,
     334,   348,   334,   348,   334,   348,   334,   348,   348,   348,
     348,   334,   348,   334,   348,   348,   348,   348,   170,   334,
     348,   348,   158,   170,   348,   348,   348,   410,   348,   348,
     348,   334,   348,   334,   348,   348,   348,   348,   334,   348,
     334,   348,   334,   348,   348,   334,   348,    22,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   129,
     130,   158,   170,   211,   212,   345,   410,   348,   215,   322,
     348,   399,   267,     8,   356,   361,   422,   170,   291,   348,
     182,   196,   196,   196,   419,   196,   196,   182,   196,   196,
     270,   196,   270,   196,   270,   196,   419,   196,   419,   285,
     450,   215,   511,   213,   450,   450,   348,   170,   170,   450,
     291,   348,   422,   422,    20,   450,    70,   322,   469,   480,
     195,   348,   170,   348,   450,   497,   499,   501,   422,   511,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   277,   196,
     419,   215,   511,   215,   257,   422,   422,   215,   422,   215,
     422,   511,   422,   422,   511,   422,   196,   327,   215,   215,
     215,   215,   215,   215,    20,   328,   214,   137,   374,   212,
     348,   215,   142,   188,   182,   474,   185,   186,   213,   478,
     188,   182,   185,   213,   477,    20,   215,   474,   181,   184,
     476,    20,   348,   181,   491,   285,   285,   348,    20,   491,
     181,   280,    20,   409,   213,   215,   214,   214,   340,   342,
      12,    23,    24,   243,   244,   348,   268,   170,   215,   469,
     467,   188,   215,   215,   170,   308,   308,   214,   127,   137,
     170,   189,   194,   325,   326,   269,   195,   214,   195,   214,
     328,   328,   328,   214,   214,   213,    19,   158,   170,   410,
     191,   158,   170,   348,   214,   214,   158,   170,   348,     1,
     214,   213,   215,   182,   181,   213,    57,    63,   359,    67,
     360,   182,   196,   182,   424,   429,   431,   450,   433,   427,
     170,   418,   435,   196,   439,   196,   443,   196,   447,   368,
     449,   371,   196,   419,   215,    43,   374,   196,   196,   322,
     196,   469,   215,   215,   215,   170,   215,   182,   196,   215,
     196,   422,   422,   422,   196,   215,   214,   422,   348,   196,
     196,   196,   196,   215,   196,   196,   215,   196,   327,   269,
     214,   322,   348,   348,   348,   470,   474,   348,   158,   170,
     467,   478,   213,   348,   490,   213,   322,   470,   181,   184,
     187,   479,   213,   322,   196,   196,   178,   322,   181,   322,
      20,   348,   348,   422,   269,   277,   348,    12,   246,   327,
     215,   213,   212,   188,   213,   215,   326,   170,   170,   214,
     170,   170,   188,   213,   270,   349,   348,   351,   348,   215,
     322,   348,   195,   214,   348,   214,   213,   348,   212,   215,
     322,   214,   213,   346,   215,   322,   182,    47,   360,    46,
     106,   357,   327,   437,   441,   445,   214,   450,   170,   348,
     483,   485,   277,   296,   215,   196,   419,   170,   214,   320,
     196,   196,   196,   504,   284,   196,   313,   315,   317,   506,
     489,   494,   487,   214,   330,   270,   215,   322,   182,   215,
     474,   478,   214,   137,   374,   182,   474,   213,   182,   288,
     290,   182,   182,   322,   215,   215,   196,   270,   277,   247,
     182,   269,   215,   467,   170,   213,   191,   377,   215,   170,
     325,   213,   142,   277,   323,   422,   215,   450,   215,   215,
     215,   353,   348,   348,   215,   467,   215,   348,   215,    33,
     358,   357,   359,   282,   214,   214,   348,   170,   196,   348,
     498,   500,   502,   214,   215,   214,   348,   348,   348,   214,
      70,   480,   214,   214,   215,   348,   323,   215,   348,   137,
     374,   478,   348,   348,   348,   348,   479,   491,   348,   214,
     283,   491,   348,   182,   341,   196,   244,    26,   105,   248,
     298,   299,   300,   302,   348,   270,   213,   191,   377,   422,
     376,   215,   127,   348,   196,   196,   450,   215,   215,   213,
     215,   364,   358,   375,   215,   480,   480,   215,   196,   214,
     215,   214,   214,   214,   282,   284,   322,   480,   467,   468,
     215,   182,   510,   348,   348,   215,   510,   510,   282,   510,
     510,   348,   338,   343,   127,   127,   348,   277,   215,   422,
     376,   376,   348,   348,   350,   352,   196,   215,   274,   365,
     214,   467,   471,   472,   473,   473,   348,   348,   480,   480,
     468,   215,   215,   511,   473,   215,    53,   213,   137,   374,
     181,   181,   188,   511,   181,   213,   510,   338,   348,   376,
     348,   348,   182,   354,   182,   274,   467,   188,   511,   215,
     215,   215,   215,   473,   473,   215,   215,   215,   348,   213,
     348,   348,   213,   181,   215,   213,   348,   182,   182,   277,
     215,   214,   215,   215,   182,   467,   215
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
     264,   265,   265,   266,   266,   266,   266,   266,   266,   266,
     267,   267,   268,   268,   269,   269,   269,   270,   270,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   272,   273,   273,   273,   274,   276,
     275,   277,   277,   278,   278,   278,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   280,   280,   280,
     281,   282,   282,   283,   283,   284,   284,   285,   285,   287,
     288,   286,   289,   290,   286,   291,   291,   291,   291,   291,
     292,   292,   292,   293,   293,   295,   296,   294,   294,   297,
     297,   297,   297,   297,   297,   298,   299,   300,   300,   300,
     301,   301,   301,   302,   302,   303,   303,   303,   304,   305,
     305,   305,   306,   306,   307,   307,   308,   308,   309,   309,
     309,   309,   310,   310,   312,   313,   311,   314,   315,   311,
     316,   317,   311,   319,   320,   318,   321,   321,   321,   321,
     321,   321,   322,   322,   323,   323,   323,   324,   324,   324,
     325,   325,   325,   325,   325,   326,   326,   327,   327,   327,
     328,   328,   329,   331,   330,   332,   332,   332,   332,   332,
     332,   332,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   334,   334,   334,   334,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   336,   336,   337,   337,   338,   338,   339,   340,
     341,   339,   342,   343,   339,   344,   344,   344,   344,   344,
     344,   344,   345,   346,   344,   347,   347,   347,   347,   347,
     347,   347,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   349,   350,
     348,   348,   348,   348,   351,   352,   348,   348,   348,   353,
     354,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   356,   356,   356,   357,   357,   357,   358,   358,
     359,   359,   359,   360,   360,   361,   362,   363,   362,   364,
     362,   365,   362,   362,   366,   366,   367,   367,   368,   368,
     369,   369,   370,   370,   370,   371,   372,   372,   373,   373,
     373,   374,   374,   375,   375,   375,   375,   375,   375,   376,
     376,   376,   377,   377,   378,   378,   378,   378,   378,   379,
     379,   379,   379,   379,   380,   381,   380,   382,   382,   383,
     383,   383,   384,   385,   384,   386,   386,   386,   386,   387,
     387,   387,   389,   388,   390,   390,   391,   392,   391,   393,
     393,   393,   394,   396,   397,   395,   398,   399,   395,   400,
     400,   401,   401,   402,   403,   403,   403,   403,   404,   404,
     404,   405,   405,   407,   408,   406,   409,   409,   409,   409,
     409,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   411,   411,
     411,   411,   411,   411,   411,   411,   412,   413,   413,   413,
     414,   414,   415,   415,   415,   417,   418,   416,   419,   419,
     420,   420,   421,   421,   422,   422,   422,   422,   422,   422,
     423,   424,   422,   422,   422,   425,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   426,
     427,   422,   422,   428,   429,   422,   430,   431,   422,   432,
     433,   422,   422,   434,   435,   422,   436,   437,   422,   422,
     438,   439,   422,   440,   441,   422,   422,   442,   443,   422,
     444,   445,   422,   446,   447,   422,   448,   449,   422,   450,
     450,   450,   452,   453,   454,   455,   451,   457,   458,   459,
     460,   456,   462,   463,   464,   465,   461,   466,   466,   466,
     466,   466,   467,   467,   467,   467,   467,   467,   467,   467,
     468,   469,   470,   470,   471,   471,   472,   472,   473,   473,
     474,   474,   475,   475,   476,   476,   477,   477,   478,   478,
     478,   479,   479,   479,   480,   480,   481,   481,   481,   481,
     481,   481,   482,   483,   481,   484,   485,   481,   486,   487,
     481,   488,   489,   481,   490,   490,   490,   491,   491,   492,
     493,   494,   492,   495,   495,   496,   496,   496,   497,   498,
     496,   499,   500,   496,   501,   502,   496,   496,   503,   504,
     496,   496,   505,   506,   496,   507,   507,   508,   508,   509,
     509,   509,   509,   509,   510,   510,   511,   511,   512,   512,
     512,   512,   512,   512
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
       1,     1,     4,     1,     2,     3,     3,     3,     3,     2,
       1,     3,     0,     3,     0,     2,     3,     0,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     2,     2,     3,     4,     3,
       2,     2,     2,     2,     2,     3,     3,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     1,     1,     3,     0,
       4,     3,     7,     2,     2,     6,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     2,     2,     2,     0,     2,     2,
       3,     0,     2,     0,     4,     0,     2,     1,     3,     0,
       0,     7,     0,     0,     7,     3,     2,     2,     2,     1,
       1,     3,     2,     2,     3,     0,     0,     5,     1,     2,
       5,     5,     5,     6,     2,     1,     1,     1,     2,     3,
       2,     2,     3,     2,     3,     2,     2,     3,     4,     1,
       1,     0,     1,     1,     1,     0,     1,     3,     9,     8,
       8,     7,     3,     3,     0,     0,     7,     0,     0,     7,
       0,     0,     7,     0,     0,     6,     5,     8,    10,     5,
       8,    10,     1,     3,     1,     2,     3,     1,     1,     2,
       2,     2,     2,     2,     4,     1,     3,     0,     4,     4,
       1,     6,     6,     0,     7,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     8,     5,     6,     1,     4,     3,     0,
       0,     8,     0,     0,     9,     3,     4,     5,     6,     8,
       5,     6,     0,     0,     5,     3,     4,     4,     5,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     4,     4,     5,     4,     5,     3,     4,     1,
       1,     2,     4,     4,     7,     8,     3,     5,     0,     0,
       8,     3,     3,     3,     0,     0,     8,     3,     4,     0,
       0,     9,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     2,     4,     1,     4,     4,     4,     4,
       4,     1,     6,     7,     6,     6,     7,     7,     6,     7,
       6,     6,     0,     4,     1,     0,     1,     1,     0,     1,
       0,     1,     1,     0,     1,     5,     0,     0,     4,     0,
       9,     0,    10,     5,     3,     4,     1,     3,     1,     3,
       1,     3,     0,     2,     3,     3,     1,     3,     0,     2,
       3,     1,     1,     1,     2,     3,     5,     3,     3,     1,
       1,     1,     0,     1,     1,     4,     3,     3,     5,     4,
       6,     5,     5,     4,     0,     0,     4,     0,     1,     0,
       1,     1,     6,     0,     6,     0,     2,     3,     5,     0,
       1,     1,     0,     5,     2,     3,     4,     0,     4,     0,
       1,     1,     1,     0,     0,     9,     0,     0,    11,     0,
       2,     0,     1,     3,     1,     1,     2,     2,     0,     1,
       1,     0,     3,     0,     0,     7,     1,     4,     3,     3,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     3,     0,     2,     3,     0,     0,     6,     1,     1,
       1,     3,     3,     4,     1,     1,     1,     1,     2,     3,
       0,     0,     6,     4,     5,     0,     9,     4,     2,     2,
       3,     2,     3,     2,     2,     3,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     6,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     1,
       3,     3,     0,     0,     0,     0,    10,     0,     0,     0,
       0,    10,     0,     0,     0,     0,    10,     1,     1,     1,
       1,     1,     3,     3,     5,     5,     6,     6,     8,     8,
       1,     1,     1,     3,     3,     5,     1,     2,     1,     0,
       0,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       1,     2,     1,     1,     0,     1,     5,     4,     6,     7,
       5,     7,     0,     0,    10,     0,     0,    10,     0,     0,
       9,     0,     0,     7,     1,     3,     3,     3,     1,     5,
       0,     0,    10,     1,     3,     4,     4,     4,     0,     0,
      11,     0,     0,    11,     0,     0,    10,     5,     0,     0,
       9,     5,     0,     0,    10,     1,     3,     1,     3,     4,
       3,     4,     7,     9,     0,     3,     0,     1,     9,    10,
      10,    10,     9,    10
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

  case 111: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 112: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 113: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 114: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 115: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 116: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 117: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 118: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 119: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 120: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 121: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 122: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 123: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 124: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 125: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 126: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 127: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 128: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 129: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 130: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 131: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 132: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 133: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 134: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 135: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 136: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 137: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 138: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 139: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 140: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 141: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 142: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 143: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 144: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 145: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 146: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 147: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 148: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 149: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 150: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 151: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 152: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 153: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 154: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 155: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 156: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 157: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 158: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 159: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 160: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 161: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 162: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 163: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 164: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 165: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 166: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 167: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 168: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 169: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 170: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 171: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 172: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 173: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 174: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 175: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 176: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 177: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 178: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 179: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 180: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 181: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 182: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 183: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 184: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 185: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 186: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 187: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 188: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 189: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 190: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 191: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 192: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 193: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 194: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 195: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 196: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 197: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 198: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 199: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 200: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 201: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 202: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 203: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 204: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 205: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 206: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 207: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 208: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 209: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 210: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 211: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 212: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 213: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 214: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 215: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 216: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 217: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 218: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 219: /* $@6: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 220: /* function_declaration: optional_public_or_private_function $@6 function_declaration_header expression_block  */
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

  case 221: /* expression_block: "begin of code block" expressions "end of code block"  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 222: /* expression_block: "begin of code block" expressions "end of code block" "finally" "begin of code block" expressions "end of code block"  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 223: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 224: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 225: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 226: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 227: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 231: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 232: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 235: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 236: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 237: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 238: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 239: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 240: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 241: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 242: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 243: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 244: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 245: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 246: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 247: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 248: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 249: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 250: /* expr_keyword: "keyword" expr expression_block  */
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

  case 251: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 252: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 253: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 254: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 255: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 256: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 257: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 258: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 259: /* $@7: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 260: /* $@8: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 261: /* expression_keyword: "keyword" '<' $@7 type_declaration_no_options_list '>' $@8 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 262: /* $@9: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 263: /* $@10: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 264: /* expression_keyword: "type function" '<' $@9 type_declaration_no_options_list '>' $@10 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 265: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 266: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 267: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 268: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 269: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 270: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 271: /* name_in_namespace: "name" "::" "name"  */
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

  case 272: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 273: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 274: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 275: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 276: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 277: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 278: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 279: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 280: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 281: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 282: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 283: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 284: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 285: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 286: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 287: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 288: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 289: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 290: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 291: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 292: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 293: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 294: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 295: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 296: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 297: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 298: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 299: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 300: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 301: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 302: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 303: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 304: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 305: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 306: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 307: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 308: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 309: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 310: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 311: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 312: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 313: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 314: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 315: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 316: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 317: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 318: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 319: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 320: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 321: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 322: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 323: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 324: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 325: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 326: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 327: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 328: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 329: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 330: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 331: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 332: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 333: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 334: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 335: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 336: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 337: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 338: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 339: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 340: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 341: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 342: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 343: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 344: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 345: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 346: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 347: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 348: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 349: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 350: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 351: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 352: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 353: /* $@21: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 354: /* expr_full_block_assumed_piped: block_or_lambda $@21 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 355: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 356: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 357: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 358: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 359: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 360: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 361: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 362: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 363: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 382: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 383: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 384: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 385: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 402: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 403: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 404: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 405: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 406: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 407: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 408: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 409: /* $@22: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 410: /* $@23: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 411: /* func_addr_expr: '@' '@' '<' $@22 type_declaration_no_options '>' $@23 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 412: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 413: /* $@25: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 414: /* func_addr_expr: '@' '@' '<' $@24 optional_function_argument_list optional_function_type '>' $@25 func_addr_name  */
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

  case 415: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 416: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 417: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 418: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 419: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 420: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 421: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 422: /* $@26: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 423: /* $@27: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 424: /* expr_field: expr '.' $@26 error $@27  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 425: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 426: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 427: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 428: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
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

  case 429: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 430: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 431: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 432: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 433: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 434: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 435: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 436: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 437: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 438: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 439: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 440: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 442: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 443: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 444: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 445: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 447: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 448: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 449: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 450: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 451: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 452: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 453: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 454: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 455: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 456: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 457: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 458: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 459: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 460: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 461: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 462: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 463: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 464: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 465: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 466: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 467: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 468: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 469: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 470: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 471: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 472: /* expr: '(' expr_list optional_comma ')'  */
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

  case 473: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 474: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 475: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 476: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 477: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 478: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 479: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 480: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 481: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 482: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 483: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 484: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 485: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 486: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 487: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 488: /* $@28: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 489: /* $@29: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 490: /* expr: expr "is" "type" '<' $@28 type_declaration_no_options '>' $@29  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 491: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 492: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 493: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 494: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 495: /* $@31: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 496: /* expr: expr "as" "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 497: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 498: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 499: /* $@32: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 500: /* $@33: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 501: /* expr: expr '?' "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 502: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 503: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 504: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 505: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 506: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 507: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 508: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 509: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 510: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 511: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 512: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 513: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 514: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 515: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 516: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 517: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 518: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 519: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 520: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 521: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 522: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 523: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 524: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 525: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 526: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 527: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 528: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 529: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 530: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 531: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 532: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 533: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 534: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 535: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 536: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 537: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 538: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 539: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 540: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 541: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 542: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 543: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 544: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 545: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 546: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 547: /* $@34: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 548: /* struct_variable_declaration_list: struct_variable_declaration_list $@34 structure_variable_declaration "end of expression"  */
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

  case 549: /* $@35: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 550: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@35 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 551: /* $@36: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 552: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@36 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 553: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' "end of expression"  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 554: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 555: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 556: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 557: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 558: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 559: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 560: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 561: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 562: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 563: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 564: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 565: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 566: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 567: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 568: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 569: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 570: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 571: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 572: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 573: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 574: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 575: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 576: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 577: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 578: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 579: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 580: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 581: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 582: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 583: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 584: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 585: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 586: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 587: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 588: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 589: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 590: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 591: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 592: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 593: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 594: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 595: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 596: /* global_variable_declaration_list: global_variable_declaration_list $@37 optional_field_annotation let_variable_declaration  */
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

  case 597: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 598: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 599: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 600: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 601: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 602: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 603: /* $@38: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 604: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@38 optional_field_annotation let_variable_declaration  */
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

  case 605: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 606: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 607: /* enum_list: enum_list "name" "end of expression"  */
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

  case 608: /* enum_list: enum_list "name" '=' expr "end of expression"  */
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

  case 609: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 610: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 611: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 612: /* $@39: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 613: /* single_alias: optional_public_or_private_alias "name" $@39 '=' type_declaration  */
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

  case 617: /* $@40: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 619: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 620: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 621: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 622: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 623: /* $@41: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 624: /* $@42: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 625: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@41 enum_list $@42 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 626: /* $@43: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 627: /* $@44: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 628: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@43 enum_list $@44 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 629: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 630: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 631: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 632: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 633: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 634: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 635: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 636: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 637: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 638: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 639: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 640: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 641: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 642: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 643: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 644: /* $@46: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 645: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
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

  case 646: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 647: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 648: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 649: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 650: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 651: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 652: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 653: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 654: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 655: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 656: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 657: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 658: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 659: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 660: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 661: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 662: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 663: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 664: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 665: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 666: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 667: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 668: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 669: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 670: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 671: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 672: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 673: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 674: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 675: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 676: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 677: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 678: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 679: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 680: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 681: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 682: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 683: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 684: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 685: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 686: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 687: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 688: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 689: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 690: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 691: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 692: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 693: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 694: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 695: /* $@47: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 696: /* $@48: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 697: /* bitfield_type_declaration: "bitfield" '<' $@47 bitfield_bits '>' $@48  */
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

  case 700: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 701: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 702: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 703: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 704: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 705: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 706: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 707: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 708: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 709: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 710: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 711: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 712: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 713: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 714: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 715: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 716: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 717: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 718: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 719: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 720: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 721: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 722: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 723: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 724: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 725: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 726: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 727: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 728: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 729: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 730: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 731: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 732: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 733: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 735: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 736: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 737: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 738: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 739: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 740: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 741: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 742: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 743: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 744: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 745: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 746: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 747: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 748: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 749: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 750: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 751: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 752: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 753: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 754: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 755: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 756: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 757: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 758: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 759: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 760: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 761: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 762: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 763: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 764: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 765: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 766: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 767: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 768: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 769: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 770: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 771: /* type_declaration: type_declaration '|' '#'  */
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

  case 772: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 773: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 774: /* $@78: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 775: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 776: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 "begin of code block" $@78 tuple_alias_type_list $@79 "end of code block"  */
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

  case 777: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 778: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 779: /* $@82: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 780: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 781: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 "begin of code block" $@82 variant_alias_type_list $@83 "end of code block"  */
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

  case 782: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 783: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 784: /* $@86: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 785: /* $@87: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 786: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 "begin of code block" $@86 bitfield_alias_bits $@87 "end of code block"  */
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

  case 787: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 788: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 789: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 790: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 791: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 792: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 793: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 794: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 795: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 796: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 797: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 798: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 799: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 800: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 801: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 802: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 803: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 804: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 805: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 806: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 807: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 808: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 809: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 810: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 811: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 824: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 825: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 826: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 827: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 828: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 829: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 830: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 831: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 832: /* $@88: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 833: /* $@89: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 834: /* make_struct_decl: "struct" '<' $@88 type_declaration_no_options '>' $@89 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 835: /* $@90: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 836: /* $@91: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 837: /* make_struct_decl: "class" '<' $@90 type_declaration_no_options '>' $@91 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 838: /* $@92: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 839: /* $@93: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 840: /* make_struct_decl: "variant" '<' $@92 type_declaration_no_options '>' $@93 '(' make_variant_dim ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 841: /* $@94: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 842: /* $@95: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 843: /* make_struct_decl: "default" '<' $@94 type_declaration_no_options '>' $@95 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 844: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 845: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 846: /* make_tuple: make_tuple ',' expr  */
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

  case 847: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 848: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 849: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 850: /* $@96: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 851: /* $@97: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 852: /* make_tuple_call: "tuple" '<' $@96 type_declaration_no_options '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 853: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 854: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 855: /* make_dim_decl: '[' expr_list optional_comma ']'  */
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

  case 856: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 857: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 858: /* $@98: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 859: /* $@99: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 860: /* make_dim_decl: "array" "struct" '<' $@98 type_declaration_no_options '>' $@99 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 861: /* $@100: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 862: /* $@101: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 863: /* make_dim_decl: "array" "tuple" '<' $@100 type_declaration_no_options '>' $@101 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 864: /* $@102: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 865: /* $@103: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 866: /* make_dim_decl: "array" "variant" '<' $@102 type_declaration_no_options '>' $@103 '(' make_variant_dim ')'  */
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

  case 867: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 868: /* $@104: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 869: /* $@105: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 870: /* make_dim_decl: "array" '<' $@104 type_declaration_no_options '>' $@105 '(' optional_expr_list ')'  */
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

  case 871: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 872: /* $@106: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 873: /* $@107: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 874: /* make_dim_decl: "fixed_array" '<' $@106 type_declaration_no_options '>' $@107 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 875: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 876: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 877: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 878: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 879: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 880: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 881: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 882: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 883: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 884: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 885: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 886: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 887: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 888: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 889: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 890: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 891: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 892: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 893: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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


